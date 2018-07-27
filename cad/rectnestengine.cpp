#include "rectnestengine.h"

/*
 * RectNestEngine: 矩形排版引擎
*/
RectNestEngine::RectNestEngine()
{
}

RectNestEngine::~RectNestEngine()
{
}

void RectNestEngine::quickSort(QList<RectNestEngine::EmptyRectArea> &s, int l, int r)
{
    if (l< r)
    {
        int i = l, j = r;
        EmptyRectArea x = s[l];
        while (i < j)
        {
            while(i < j && s[j]> x) // 从右向左找第一个小于x的数
                j--;
            if(i < j)
                s[i++] = s[j];
            while(i < j && s[i]< x) // 从左向右找第一个大于等于x的数
                i++;
            if(i < j)
                s[j--] = s[i];
        }
        s[i] = x;
        quickSort(s, l, i - 1); // 递归调用
        quickSort(s, i + 1, r);
    }
}

void RectNestEngine::updateEmptyRectArea(RectNestEngine::EmptyRectArea *eRect)
{
    // 遍历整个二叉树，做差集运算
    // 如果二叉树根节点为空，则根节点为插入值
    // 否则，二叉树各节点减去该矩形元素

    // 最后，对该树进行整理：
    // 去掉面积为零的或已无法排下所剩的任何一个矩形件的剩余矩形；
    // 把具有完全包含关系的剩余矩形中面积小的矩形去除、有相交关系的矩形全部保留。
    if(emptyRectArea.length() == 0){
        emptyRectArea.append(*eRect);
        return;
    }

    QList<EmptyRectArea> newList;  // 保存作差之后新生成的矩形
    for(int i=0;i<emptyRectArea.length();i++){
        // 每个矩形都减去该矩形集合
        QList<EmptyRectArea> area = emptyRectArea[i].subtraction(*eRect);
        int num = area.length();

        // 若两矩形相交，则最多产生四个新矩形
        // 通过与生成的矩形列表比较，判断该矩形是否需要加入新生成矩形列表
        for(int j=0; j<num; j++){
            // 若生成矩形的面积小于矩形件中的最小面积，直接忽略
            if(area[j].area() < minRectsArea){
                continue;
            }
            int len = newList.length();
            bool continueFlag = false;
            for(int m=0; m<len; m++){
                // 如果新生成的矩形中包含该生成的矩形，则不加入新生成矩形列表
                if(newList[m].contains(area[j])){
#ifdef NESTDEBUG
                    qDebug() << "----- start";
                    qDebug() << newList[m].leftBottom() << "  " << newList[m].rightTop();
                    qDebug() << area[j].leftBottom() << "  " << area[j].rightTop();
                    qDebug() << "新生成的矩形中包含该生成的矩形，则不加入新生成矩形列表";
                    qDebug() << "----- end";
#endif
                    continueFlag = true;
                    break;
                }
                // 如果该生成的矩形中包含新生成的矩形，则将原矩形替换
                if(area[j].contains(newList[m])){
#ifdef NESTDEBUG
                    qDebug() << "----- start";
                    qDebug() << newList[m].leftBottom() << "  " << newList[m].rightTop();
                    qDebug() << "该生成的矩形中包含新生成的矩形，则将原矩形替换";
                    qDebug() << area[j].leftBottom() << "  " << area[j].rightTop();
                    qDebug() << "----- end";
#endif
                    newList[m] = area[j];
                    continueFlag = true;
                    break;
                }
                // 如果两矩形相等，则不加入新生成矩形列表
                if(area[j] == newList[m]){
#ifdef NESTDEBUG
                    qDebug() << newList[m].leftBottom() << "  " << newList[m].rightTop();
                    qDebug() << area[j].leftBottom() << "  " << area[j].rightTop();
                    qDebug() << "两矩形相等，则不加入新生成矩形列表";
#endif
                    continueFlag = true;
                    break;
                }
            }

            // 如果矩形不需要加入，继续下次循环
            if(continueFlag){
                continue;
            }
            // 直接添加该区域
            newList.append(area[j]);
        }
    }

    // 更新剩余矩形集合
    emptyRectArea.clear();
    emptyRectArea.append(newList);

    int len = emptyRectArea.length();
    quickSort(emptyRectArea,0,len-1);

#ifdef NESTDEBUG
    qDebug() << "-------------空白矩形集合------------- start";
    for(int i=0;i<len;i++){
        qDebug() << i << "  " <<  emptyRectArea[i].leftBottom() << emptyRectArea[i].rightTop();
    }
    qDebug() << "-------------空白矩形集合------------- end";
#endif
}

void RectNestEngine::resetAllParameter()
{
    // 重置矩形集合及最大高度
    maxHight = 0;
    emptyRectArea.clear();
    EmptyRectArea a(0,0, mWidth, mHeight);
    updateEmptyRectArea(&a);
}

double RectNestEngine::fitnessFunc(Genome &g)
{
    // 根据“高度调整填充法”计算适应度
    //!
    //! 第二个是放入的矩形面积占待放入的矩形面积的百分比，
    //! 第二个是当前排入的矩形的最大高度，
    //! 第三个是排入矩形的整齐程度（高的种类个数）
    //!
    // 对基因进行解码
    QVector<double> gVector = g.getGenome();

    // 重置所有参数
    resetAllParameter();

    // 排版算法
    layoutAlg(gVector);

    // 计算适应度评分
    g.setFitness(calculateScore());

    return g.getFitness();
}

void RectNestEngine::layoutAlg(QVector<double> gVector)
{
    QList<MinRect> layRects;  // 存放已排矩形
    for(int i=0;i<gVector.length();i++){
        // 解码
        int index = qAbs(gVector[i]) - 1;  // 得到矩形的序号
        MinRect *currentRect = &RectNestEngine::compMinRects[index]; // 得到矩形指针
        currentRect->page = 1;
        currentRect->layFlag = false;
        currentRect->setRotate(gVector[i] < 0);  // 如果基因为负值，则需要旋转90*
        double rectWidth = currentRect->getWidth();  // 得到矩形宽度
        double rectHight = currentRect->getHeight();  // 得到矩形长度

        // 下面对该零件进行排版，即确定每个矩形的位置
        /*
        //!
        //! "高度调整法"算法描述：
        //! Step1: 设置初始最高轮廓线为板材的最上边的边。
        //! Step2: 每当要排入一个零件时，在最高轮廓线集中选取最低的一段水平线，
        //! 如有数段，则选取选取最左边的一段，
        //! 测试该线段的宽度是否大于或等于要排入零件的宽度：
        //!     a. 如果该线段的宽度大于排入零件的宽度，
        //! 则将该零件在此位置排放，零件排放之后，测试整体排样高度的变化。
        //! 如果整体排样高度增加（甚至超出了板材边界），
        //! 那么就从前面已排的零件中寻找宽度适合（零件宽度和排放零件处的
        //! 线段的宽度均适合）的零件：
        //!     如果没有，则按原先排样序列进行排样，更新零件最高轮廓线后
        //! 转到Step2排放下一个零件；
        //!     如有数个，则先选取最左边的一个，相互交换排样位置，
        //! 并测试交换前后的整体排样高度。然后依次选取另外几个零件，进行相同操作；
        //!     a1: 如果其中部分交换操作降低了整体排样高度，
        //! 则选取整体排样高度最低的进行交换，并更新排样序列和零件的最高轮廓线。
        //! 而其他的零件排样位置保持不变。
        //!     a2: 如果都没有降低整体排样高度，则不交换排样位置。
        //! 按原排样序列进行排样。更新零件最高轮廓线。
        //!     b. 否则，查询与最低水平线相邻的左右两段水平线，
        //! 将最低水平线提升与高度较低的一段平齐，更新零件最高轮廓线。
        //!
        //! 重复Step2，直至能排入该零件。
        //! 重复上述过程，直至所有零件排放完毕。
        //!
        //
        */

        double currentMaxHight = maxHight;  // 记录最高高度
        for(int j=0; j<emptyRectArea.length();j++){
            EmptyRectArea lowestRect = emptyRectArea[j];
            if(lowestRect.width() >= rectWidth && lowestRect.height() >= rectHight){
                // 如果该线段的宽度大于排入零件的宽度，则可排放该零件
                // 更新排放标识，表明已排放
                currentRect->layFlag = true;

                // 将该零件排放在此位置
                currentRect->position = lowestRect.leftBottom();
                // 计算排放后的最大排样高度
                currentMaxHight = qMax(maxHight,
                                       lowestRect.leftBottom().ry() + rectHight);
                // 如果整体排样高度增加（甚至超出了板材边界），
                // 那么就从前面已排的零件中寻找宽度适合（零件宽度和排放零件处的
                // 线段的宽度均适合）的零件：
                // ......

                updateEmptyRectArea(new EmptyRectArea(*currentRect));
                break;
            }
        }

        // 如果未找到合适位置，即未排放，则需要在某高度上
        if(!currentRect->layFlag){
            qDebug() << "未找合适位置摆放";
            return;
        }

        maxHight = currentMaxHight;  // 更新最大高度
        layRects.append(*currentRect);  // 将当前矩形加入到已排矩形集合中
    }

#ifdef NESTDEBUG
    qDebug() << layRects.length();
    for(int i=0;i<layRects.length();i++){
        int index = layRects[i].index;
        QPointF position = layRects[i].position;
        bool rotate = layRects[i].rotate;
        qDebug() << index << "号矩形" << "第" << i << "个排放,  排放位置：" << position << "  ，旋转" << rotate;
    }
    qDebug() << "";
#endif
}

double RectNestEngine::calculateScore()
{
    // 计算适应度评分
    double mArea = mWidth * maxHight;
    double rate = allRectsArea / mArea;
#ifdef NESTDEBUG
    qDebug() << "最大高度： " << maxHight << "\t有效面积：" << mArea << "\t矩形件总面积：" << allRectsArea << "\t适应度评分值：" << rate;
#endif
    return rate;
}

#include "nest.h"
#include "ui_nest.h"
#include <QMap>
#include <QDebug>

#define COUNT 20

// 初始化矩形排版引擎
QList<Nest::Component> RectNestEngine::components;  // 零件
QList<RectNestEngine::MinRect> RectNestEngine::compMinRects;  // 零件的最小矩形
QList<RectNestEngine::EmptyRectArea> RectNestEngine::emptyRectArea;  // 空白矩形
double RectNestEngine::mWidth = 0;  // 材料宽度
double RectNestEngine::mHeight = 0;  // 材料高度
double RectNestEngine::maxHight = 0;  // 最大高度值
long RectNestEngine::allRectsArea = 0; // 矩形零件面积
long RectNestEngine::minRectsArea = LONG_MAX; // 矩形零件面积

Nest::Nest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Nest)
{
    ui->setupUi(this);
    setWindowTitle("CADPRO");
    setWindowState(Qt::WindowMaximized);
    setMouseTracking(true);     // 开启鼠标追踪

    //删除中央窗体
    QWidget* p = takeCentralWidget();
    if(p) delete p;

    view = new View;    // 初始化view
    setCentralWidget(view);    // 将该视图加入到central widget

    scene = new Scene();
    scene->setSceneRect(0, 0, 1000, 1000);
    view->setScene(scene);
    view->centerOn(view->mapFromScene(500,500));
    Line *line = new Line;
    Configure::PenStyle pen;
    pen.setPenStyle(Qt::red, Qt::SolidLine, 2);
    line->setPenStyle(pen);
    line->setLine(QLineF(500, 500, 1000, 1000));
    scene->addCustomLineItem(line);;

    scene->setCurShape(Shape::Rectangle);

    // 首先构造10个矩形对象作为零件，
    // 后期要通过可视化进行添加
    // 即通过界面化操作将图形元素或个数添加入零件列表
    for(int i = 0; i < COUNT; i++){
        int w = GA::randT<int>(10, 100);
        int h = GA::randT<int>(10, 100);
        int n = GA::randT<int>(1, 5);
        RectNestEngine::components.append(Component(QRectF(0, 0, w, h),
                                                   n));
    }

    double len = RectNestEngine::components.length();
    // 将零件列表转化为最小矩形列表
    int index = 0;
    for(int i=0; i<len; i++){
        Component com = RectNestEngine::components[i];
        for(int j=0; j<com.count; j++){
            // 更新矩形件总面积和最小面积
            long area = com.component.width() * com.component.height();
            RectNestEngine::allRectsArea += area;
            if(area < RectNestEngine::minRectsArea){
                RectNestEngine::minRectsArea = area;
            }
            RectNestEngine::MinRect minRect(com.component.width(),
                                            com.component.height(),
                                            index++,
                                            false);
            RectNestEngine::compMinRects.append(minRect);
        }
    }

    // 矩形列表的长度即为基因数
    int totalNum = RectNestEngine::compMinRects.length();

    // 初始化材料
    // ...

    // 初始化空白矩形集合
    QRectF r = material.layoutRect;
    RectNestEngine::mWidth = r.width();
    RectNestEngine::mHeight = 1000; //r.height();

    // 使用遗传算法进行求解最优解
    GA g(COUNT, totalNum, 3, 20, 0.1, RectNestEngine::fitnessFunc, 0.97, 1, 0.3);
    g.initPopulation();
    int n = 10;
    while(n-- > 0 && !g.isStop()){
        g.getNewGeneration();
    }
    // 进化结束后，在新的种群中选择最优个体
    g.evaluateFitness();

    // 重置所有参数
    RectNestEngine::resetAllParameter();

    // 排版算法
    RectNestEngine::layoutAlg(g.getFittestGenome().getGenome());

    for(int i=0;i<g.getFittestGenome().getGenome().length();i++){
        int index = qAbs(g.getFittestGenome().getGenome()[i]) - 1;  // 得到矩形的序号
        RectNestEngine::MinRect *currentRect = &RectNestEngine::compMinRects[index]; // 得到矩形指针
        bool rotate = g.getFittestGenome().getGenome()[i] < 0; // 如果基因为负值，则需要旋转90*
        currentRect->setRotate(rotate);

        Rect *rect = new Rect;
        Configure::PenStyle pen;
        pen.setPenStyle(Qt::red, Qt::SolidLine, 2);
        rect->setPenStyle(pen);
        rect->setRect(currentRect->rect());
        scene->addCustomRectItem(rect);
    }
    qDebug() << "材料使用率： " << g.getFittestGenome().getFitness();
}

Nest::~Nest()
{
    delete ui;
}

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
        // 寻找最低水平线集合
        /*
//        BBSTNode<BaseLine *> *minNode = highestProfileLine.findMinAVL(highestProfileLine.root);
//        double currentMaxHight = maxHight;
//        bool lay = false;  // 排放标识
//        while(minNode){
//            bool breakFlag = false;
//            BaseLine *lowestLine = minNode->data;
//            ////qDebug() << "最低水平线：" << lowestLine->line() << "  width: " << lowestLine->width() << "  最大高度: " << maxHight;
//            // 遍历最低水平线集合
//            while(lowestLine){
//                if(lowestLine->width() >= rectWidth){
//                    lay = true;  // 更新排放标识，表明已排放
//                    // 如果该线段的宽度大于排入零件的宽度，则可排放该零件
//                    // 将该零件排放在此位置
//                    currentRect->position = lowestLine->left();

//                    // 计算排放后的最大排样高度
//                    currentMaxHight = qMax(maxHight,
//                                           lowestLine->left().ry() + rectHight);
//                    //qDebug() << "存在一个位置排放该矩形, 排放在此位置: " << currentRect->position;
//                    //qDebug() << "排放后的最大排样高度: " << currentMaxHight;
//                    // 如果整体排样高度增加（甚至超出了板材边界），
//                    // 那么就从前面已排的零件中寻找宽度适合（零件宽度和排放零件处的
//                    // 线段的宽度均适合）的零件：
//                    if(currentMaxHight > maxHight || currentMaxHight > mHeight){
//                        //qDebug() << "整体高度增加，需要调换";
//                        // 如果已排零件集合不为空
//                        if(!layRects.isEmpty()){
//                            //qDebug() << "已排零件集合不为空";
//                            //qDebug() << "原始摆放位置： " << currentRect->position;
//                            // 用map来记录交换时的高度变化
//                            // <double hight, int n>
//                            // <交换之后排样高度, 已排矩形列表中的序号>
//                            QMap<double, int> map;
//                            // 遍历已排矩形列表
//                            for(int n=0; n<layRects.length();n++){
//                                // 判断两宽度是否合适，如果合适，尝试调换位置
//                                if(lowestLine->width() >= layRects[n].width && layRects[n].width >= rectWidth){
//                                    // 计算调换位置之后的最大高度
//                                    double heightTemp1 = lowestLine->left().ry() + layRects[n].height;
//                                    double heightTemp2 = layRects[n].position.ry() + rectHight;
//                                    double maxHightTemp = qMax(heightTemp1, heightTemp2);
//                                    // 若最大高度变小，则将调换记录保存
//                                    if(maxHightTemp < currentMaxHight){
//                                        //qDebug() << "最大高度变小，则将调换记录保存";
//                                        // 如果高度值不相同，直接保存
//                                        if(!map.contains(maxHightTemp)){
//                                            //qDebug() << "高度值不相同，直接保存maxHeightTemp: "<<maxHightTemp;
//                                            map.insert(maxHightTemp, n);
//                                        } else {// 如果高度值相同，则保存宽度较大的，这样可提高材料利用率
//                                            //qDebug() <<"高度值相同,存在map[maxHightTemp]:\t " << maxHightTemp << "  " << map[maxHightTemp];
//                                            if(layRects[n].width > map[maxHightTemp]){
//                                                //qDebug() << "替换map[maxHightTemp]为: " << n;
//                                                map[maxHightTemp] = n;
//                                            }
//                                        }
//                                    }
//                                }
//                            }
//                            QMap<double, int>::iterator it;
//                            for(it=map.begin(); it!=map.end(); it++){
//                                //qDebug() << it.key() <<"\t" << it.value();
//                            }

//                            ////qDebug() << "map count: " << map.count();
//                            // 如果map不为空
//                            if(map.count()){
//                                // 比较调换记录中最大高度值，取最小者进行调换
//                                // 由于map具有对key自动排序，所以取map的首记录
//                                int exchangeIndex = map.begin().value();

//                                // 还原该位置
//                                //qDebug() <<  index << "与第" << exchangeIndex << "个矩形(" << layRects[exchangeIndex].position << ")交换";
//                                updateHightProfileLine(new BaseLine(layRects[exchangeIndex].upper()));
//                                updateHightProfileLine(new BaseLine(layRects[exchangeIndex].lower()));

//                                // 调换位置，直接改变两矩形的位置坐标即可
//                                layRects[exchangeIndex].exchange(*currentRect);
//                                //qDebug() << "更换后该矩形位置： " << currentRect->position;
//                                //qDebug() << "更换后交换矩形位置： " << layRects[exchangeIndex].position;

//                                // 更新最大高度及最高轮廓线
//                                currentMaxHight = map.begin().key();
//                                updateHightProfileLine(new BaseLine(layRects[exchangeIndex].upper()));
//                                updateHightProfileLine(new BaseLine(layRects[exchangeIndex].lower()));
//                            }
//                            else{
//                                //qDebug() << "该矩形不用更换位置";
//                            }
//                        }
//                        else{
//                            //qDebug() << "已排零件集合为空, 直接排放";
//                        }
//                    }

//                    breakFlag = true;
//                    break;
//                }
//                lowestLine = lowestLine->next;
//            }
//            if(breakFlag){
//                break;
//            }
//            // 寻找比最低水平线高的最近的水平线集合
//            qDebug() << "-------寻找比最低水平线高的最近的水平线集合 start------";
//            minNode = NULL;//highestProfileLine.findMinLargerAVL(minNode);
//            qDebug() << "-------寻找比最低水平线高的最近的水平线集合 end------";
//            qDebug() << "";
//            if(minNode){
//                qDebug() << "该线段的宽度小于排入零件的宽度,寻找比最低水平线高的最近的水平线集合: " << minNode->data->line();
//            } else{
//                qDebug() << "寻找比最低水平线高的最近的水平线集合---------为空";
//            }
//        }

//        // 如果未找到合适位置，即未排放，则需要在某高度上
//        if(!lay){
//            qDebug() << "未找到该位置";
////            continue;
//        }
//        // 将当前矩形加入到已排矩形集合中
//        layRects.append(*currentRect);
//        // 更新最大高度及最高轮廓线
//        //qDebug() << "更新最大高度及最高轮廓线";
//        //qDebug() << "最大高度：" << currentMaxHight;
//        //qDebug() << "矩形上边界线：" << currentRect->upper();
//        //qDebug() << "矩形下边界线：" << currentRect->lower();
//        qDebug() << "-------------更新最大高度及最高轮廓线 start-------------";
//        maxHight = currentMaxHight;
//        updateHightProfileLine(new BaseLine(currentRect->upper()));
//        updateHightProfileLine(new BaseLine(currentRect->lower()));
//        qDebug() << "-------------更新最大高度及最高轮廓线 start-------------";
//        qDebug() << "";
        */

        bool lay = false;  // 排放标识
        double currentMaxHight = maxHight;  // 记录最高高度
        for(int j=0; j<emptyRectArea.length();j++){
            EmptyRectArea lowestRect = emptyRectArea[j];
            if(lowestRect.width() >= rectWidth && lowestRect.height() >= rectHight){
                // 如果该线段的宽度大于排入零件的宽度，则可排放该零件
                // 更新排放标识，表明已排放
                lay = true;

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
        if(!lay){
            qDebug() << "未找到该位置";
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


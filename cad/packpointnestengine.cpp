#include "packpointnestengine.h"

PackPointNestEngine::PackPointNestEngine() :
    NestEngine(),
    PPD(5),
    RN(4),
    minHeight(LONG_MAX)
{
    setNestEngineType(NestEngine::PackPointNest);
}

PackPointNestEngine::PackPointNestEngine(const QVector<Piece> pieceList, const QVector<Sheet> sheetList, qreal PPD, int RN) :
    NestEngine(pieceList, sheetList),
    minHeight(LONG_MAX)
{
    setNestEngineType(NestEngine::PackPointNest);
    this->PPD = PPD;
    this->RN = RN;
    initPackPoint(sheetList, PPD);
}

PackPointNestEngine::~PackPointNestEngine()
{

}

void PackPointNestEngine::initPackPoint(QVector<Sheet> sheetList, qreal PPD)
{
    // 清空材料-排样点Map
    sheetPackPointPositionMap.clear();
    // 初始化材料-排样点Map
    for(int i=0; i<sheetList.length(); i++){
        initPackPointOneSheet(i, PPD);
    }
}

void PackPointNestEngine::initPackPointOneSheet(int sheetID, qreal PPD)
{
    Sheet sheet = sheetList[sheetID];
    QRectF layoutRect = sheet.layoutRect();
    qreal width = layoutRect.width();
    qreal height = layoutRect.height();
    qreal XOffset = sheet.leftMargin;
    qreal YOffset = sheet.topMargin;

    // 记录第i个材料的排样点信息
    QMap<int, PackPoint> packPointMap;
    // 记录剩余排样点
    QList<int> unusedPackPointList;
    // 获取行列数即排样点个数总数
    int rows = qFloor(height / PPD) + 1;
    int columns = qFloor(width / PPD)  + 1;
    int PPN = rows * columns;
    for(int j=0; j<PPN; j++){
        int row = j / columns;  // 获取序号为j的排样点在第几行
        int column = (j - columns * row) % columns;  // 获取序号为j的排样点在第几列
        // 将序号为j的排样点添加至map中
        qreal px = column * PPD + XOffset;
        qreal py = row * PPD + YOffset;
        PackPoint packPoint(j, QPointF(px, py), false);
        packPointMap.insert(j, packPoint);
        // 将序号为j的排样点加入到剩余排样点map中
        unusedPackPointList.append(j);
    }
    // 记录该材料的排样点信息
    PackPointInfo info(sheetID, rows, columns, XOffset, YOffset);
    packPointInfoList.append(info);
    // 将第i个材料的排样点信息加入到map中去
    sheetPackPointPositionMap.insert(sheetID, packPointMap);
    // 剩余排样点
    unusedSheetPackPointMap.insert(sheetID,unusedPackPointList);
}

void PackPointNestEngine::updatePackPointOneSheet(int sheetID, Piece piece)
{
    //qDebug() << "";
    //qDebug() << "更新排样点";
    // 获取该材料的排样点状态
    PackPointInfo &info = packPointInfoList[sheetID];
    int columns = info.columns;
    qreal XOffset = info.XOffset;
    qreal YOffset = info.YOffset;

    // 获取该零件的最小包络矩形
    QRectF minBoundingRect = piece.getMinBoundingRect();
    // 首先根据包络矩形，判断出排样点大致范围
    // 获取边界
    qreal minX, minY, maxX, maxY;
    getRectBoundValue(minBoundingRect, minX, minY, maxX, maxY);

    // 获取边界范围内排样点的序号
    int minRow = qFloor((minY - YOffset) / PPD);  // 最小行
    int minColumn = qFloor((minX - XOffset) / PPD);  // 最小列
    int maxRow = qFloor((maxY - YOffset) / PPD);  // 最大行
    int maxColumn = qFloor((maxX - XOffset) / PPD);  // 最大列

    //qDebug() << "矩形边界" << minBoundingRect;
    //qDebug() << "minRow: " << minRow;
    //qDebug() << "minColumn: " << minColumn;
    //qDebug() << "maxRow: " << maxRow;
    //qDebug() << "maxColumn: " << maxColumn;
    // 记录多边形可能覆盖的排样点
    //qDebug() << "多边形可能覆盖的排样点";
    QList<int> packPointsList;
    for(int row=minRow; row<=maxRow; row++){
        for(int column=minColumn; column<=maxColumn; column++){
            int packPointID = row * columns + column;
            packPointsList.append(packPointID);
            //qDebug() << packPointID;
        }
    }
    //qDebug() << "长度为：" << packPointsList.length();
    // debug
    int count = 0;
    // 判断这些可疑点是否被覆盖，及更新剩余排样点
    QMap<int, PackPoint> &packPointMap = sheetPackPointPositionMap[sheetID];  // 获取该材料的排样点状态
    foreach(int id, packPointsList){
        QPointF point = packPointMap[id].position;
        //qDebug() << "排样点 #" << id << ", pos: " << point;
        if(piece.hasRelationToPoint(point)){
            count++;
            packPointMap[id].coverd = true;
            info.coverdList.append(id);
            unusedSheetPackPointMap[sheetID].removeOne(id);
        }
    }
    //qDebug() << "真正覆盖的点长度为： " << count;
    //qDebug() << "更新后覆盖点列表的长度： " << packPointInfoList[sheetID].coverdList.length();
    //qDebug() << "更新后未使用的排样点： " << unusedSheetPackPointMap[sheetID].length();
}

void PackPointNestEngine::packPieces(QVector<int> indexList)
{
    // 清空未排零件列表
    unnestedPieceIndexlist.clear();
    // 可进行多材料排版
    for(int i=0; i<indexList.length(); i++){
        int index = indexList[i];  // 获取该排样零件的序号
        int typeID = nestPieceList[index].typeID;  // 获取该排样零件的类型
        //qDebug() << "index: #" << index << ", typeID: $" << typeID << "   " << nestPieceList[index].alpha;
        Piece piece = pieceList[typeID];
        bool res = packOnePiece(piece, nestPieceList[index]); // 排放该零件
        if(res) {  // 如果该零件成功排放，则可以排放下一个零件
            nestedPieceIndexlist.append(index);
        } else {  // 如果该零件未成功排放，则记录该零件的ID
            unnestedPieceIndexlist.append(index);
            qDebug() << "#" << index << "未排放成功";
        }
    }

    // 如果没有设置自动重复最后一张材料 或 没有剩余零件，则排版结束
    if(!autoRepeatLastSheet || unnestedPieceIndexlist.length() == 0){
        return;
    }

    // 如果存在未排放成功的零件并且设置为自动添加材料，则继续排剩余的零件
    qDebug() << "自动添加材料";
    int len = sheetList.length();
    Sheet sheet = sheetList[len-1];
    sheetList.append(sheet);
    initPackPointOneSheet(len, PPD);  // 初始化该材料的排样点
    initQuadTreeMap(len);  // 初始化该张材料的四叉树
    packPieces(unnestedPieceIndexlist);  // 进行排版
}

bool PackPointNestEngine::packOnePiece(Piece piece, NestEngine::NestPiece &nestPiece)
{
    /***
     * 将零件排至第i张材料上，
     * 如何材料不够排下目标零件，则判断比该零件面积小的零件是否可排下，
     * 如果可以排下，就排该零件，跳出循环，目标零件排放完毕
     * 如果面积最小的都无法排下，则继续循环至下张材料，继续排版；
     *
     * 优化方案：
     * 1. 排版之前判断材料的有效排版面积的大小，
     *    如果有效排版面积小于零件面积，则判断下一材料
     * 2. 如果零件属于再排版，即在第一次排版中没有找到合适位置，
     *    那么可直接让其进入最后一张材料进行排版
     */
    // 零件在排版，直接进入最后一张材料
    if(nestPiece.sheetID != -1 && !nestPiece.nested){
        return packOnePieceOnSheet(piece, nestPiece.sheetID+1, nestPiece);
    }
    // 该过程针对于首次排放的零件
    for(int i=0; i<sheetList.length(); i++){
        if(packOnePieceOnSheet(piece, i, nestPiece)){  // 优化方案，可通过设置材料可排区域的面积，判断该材料是否可排
            return true;
        }
    }
    return false;
}

bool PackPointNestEngine::packOnePieceOnSheet(Piece piece, int sheetID, NestEngine::NestPiece &nestPiece)
{
    /***
     * 尝试排放该零件
     */
    bool nestRet = packOnePieceAttempt(piece, sheetID, nestPiece, unusedSheetPackPointMap[sheetID], maxRotateAngle, RN);
    /***
     * 如果零件没有排入，最大的可能是在尾行处，
     * 此时如果开启了尾行优化，则允许任意角度排入
     */
    if(!nestRet && (mixingTyes & NestEngine::TailLineMixing) == NestEngine::TailLineMixing){
//        Piece p = piece;
//        qreal minBoundingAngle = p.getAngle();  // 获取得到最小包络矩形需要顺时针旋转的角度
//        p.rotate(p.getMinBoundingRect().center(), -minBoundingAngle);  // 将该零件旋转至最小包络矩形的位置
//        qreal w = qrealPrecision(p.getMinBoundingRect().width(), PRECISION);  // 宽度
//        qreal h = qrealPrecision(p.getMinBoundingRect().height(), PRECISION);  // 高度
//        if(w < h){
//            p.rotate(p.getMinBoundingRect().center(), 90);  // 保证横放可放下
//            qreal temp = w;
//            w = h;
//            h = temp;
//        }
        qDebug() << "尾行优化";
        nestRet = packOnePieceAttempt(piece, sheetID, nestPiece, unusedSheetPackPointMap[sheetID], 360, 36);
    }

    /***
     * 判断目标零件是否已排，
     * 如果已排则直接跳出循环，该零件排版成功；
     * 否则，记录该零件尝试排版的材料ID(sheetID)，再排放时会用到，
     * 然后继续循环，在下一张材料上对目标零件排版
     */
    if(nestRet && compact(sheetID, nestPiece)){
        piece.moveTo(nestPiece.position);
        piece.rotate(nestPiece.position, nestPiece.alpha);  // 确定此零件的位置
        updatePackPointOneSheet(sheetID, piece);  // 更新排样点状态
        if(!nestSheetPieceMap.contains(sheetID)){  // 更新材料-零件索引
            QVector<int> pieceIDList;
            pieceIDList.append(nestPiece.index);
        }
        nestSheetPieceMap[sheetID].append(nestPiece.index);

        // 将该对象加入四叉树中
        qDebug() << "加入四叉树：ID："<< nestPiece.index;
        Object *obj = new Object(nestPiece.index, piece.getMinBoundingRect());
        QuadTreeNode<Object> *quadTree = quadTreeMap[sheetID];
        quadTree->insert(obj);

        qDebug() << "排放位置: " << nestPiece.position;
        qDebug() << "旋转度数：" << nestPiece.alpha;
        qDebug() << "最终选择" << (nestPiece.position.ry() - 5) / 50 * 20 + ((nestPiece.position.rx() - 5) / 50);
        qDebug() << "行：" << (nestPiece.position.ry() - 5) / 50 << ", 列：" << (nestPiece.position.rx() - 5) / 50;
        qDebug() << "";
        return true;
    }

    return false;
}

/***
 * 优化方案：
 * 1. 只访问可排排样点
 * 2. 界定上界，即第一个可排位置确定时，确定有效排样点
 */
bool PackPointNestEngine::packOnePieceAttempt(Piece piece, int sheetID, NestEngine::NestPiece &nestPiece, QList<int> packPointList, int maxRotateAngle, int RN)
{
    qDebug() << "排放零件:#" << nestPiece.index;
    if(sheetID > sheetList.length()){  // 判断编号是否越界
        return false;
    }
    minHeight = sheetList[sheetID].height;  // 初始化最小高度
    QMap<int, PackPoint> packPointMap = sheetPackPointPositionMap[sheetID];  // 获取该材料的排样点状态

    int maxPackPointIndex = pieceMaxPackPointMap[nestPiece.typeID];  //  之前的排样点最大值
    if(nestPiece.typeID >= 1 && pieceMaxPackPointMap[nestPiece.typeID] == 0){
        maxPackPointIndex = pieceMaxPackPointMap[nestPiece.typeID-1];
    }
    int maxPackPointIndexTemp = maxPackPointIndex;  // 用该值记录排样点最大值
    //qDebug() << "最大排样点为：" << maxPackPointIndexTemp << packPointMap[maxPackPointIndexTemp].position;

    PackPointInfo info = packPointInfoList[sheetID];  // 获取该材料的排样点状态
    int rows = info.rows;
    int columns = info.columns;
    int PPN =  rows * columns;
    int upperIndex = PPN;  // 上界
    bool upperFlag = false;  // 上界已设置标志
    foreach(int j, packPointList){
        // 如果未设置尾只混合方式，则需要从该种零件最大的排样点去排
        if(((mixingTyes & NestEngine::TailPieceMixing) == NestEngine::NoMixing) && j < maxPackPointIndexTemp){
            //qDebug() << "从最大的排样点出发去排";
            continue;
        }
        // 如果排样点序号大于上界，则结束
        if(j > upperIndex){
            break;
        }
        //qDebug() << "尝试排样点：" << j;
        for(int k=0; k<=RN; k++){
            /**
             * @brief pos
             * 参考点的选择也是一个值得优化的问题。
             * 选择凸点，是一个较好的选择，
             * 现在默认是将最小包络矩形的中心设置为参考点
             */
            Piece pieceTmp = piece;
            QPointF pos = packPointMap[j].position;  // 该排样点对应的位置坐标
            qreal alpha;
            if(RN == 0){
                alpha = nestPiece.alpha;  // 如果RN=0，代表不能对该零件进行额外的旋转
            }else{
                alpha = maxRotateAngle * k / RN;  // 旋转角度
            }
            pieceTmp.moveTo(pos);  // 将零件最小包络矩形中心移至该位置
            pieceTmp.rotate(pos, alpha);  // 绕参考点旋转alpha度
            qreal height = pieceTmp.getCenterPoint().ry();  // 得到零件形心
            //qDebug() << "尝试位置: #" << j << " - " << pieceTmp.getMinBoundingRect().center();
            //qDebug() << ", 旋转角度: " << alpha << "  ";
            //qDebug() << "alpha - " << int(alpha) << " = " << alpha - int(alpha);
            //qDebug() << "零件的包络矩形：" << pieceTmp.getMinBoundingRect();
            //qDebug() << "此时质心为：" << pieceTmp.getCenterPoint();
            //qDebug() << "最小高度为：" << height;
            //qDebug() << "minHeight: " << minHeight;
            /***
             * 如果高度（重心）小于最小高度（重心）&&
             * 目标零件完全包含在材料内 &&
             * 目标零件不与其他已排零件重叠
             * 则更新最优排样姿态
             */
            if(!pieceTmp.containsInSheet(sheetList[sheetID])){
                //qDebug() << "零件不在材料内部";
                continue;
            }
            if(collidesWithOtherPieces(sheetID, pieceTmp)){
                //qDebug() << "该零件与其他零件碰撞";
                continue;
            }
            if(qrealPrecision(height, PRECISION) < qrealPrecision(minHeight, PRECISION)){
                //qDebug() << "可以排放" << j << height;
                minHeight = height;
                nestPiece.sheetID = sheetID;
                nestPiece.position = pos;
                nestPiece.alpha = alpha;
                nestPiece.nested = true;
                if(j>maxPackPointIndex){
                    pieceMaxPackPointMap[nestPiece.typeID] = j;  // 更新该种型号零件排样点最大值
                }
                //qDebug() << "在$" << j << "找到合适位置排放该零件:" << pos << " " << alpha;
                //qDebug() << "靠接前的位置：" << nestPiece.position;
                //qDebug() << "最小高度 " << minHeight;
                // 如果未设置上界，则进行设置
                if(!upperFlag){
                    qreal width = pieceTmp.getMinBoundingRect().width();
                    qreal height = pieceTmp.getMinBoundingRect().height();
                    int deltaRows = (width + height) / PPD + 1;  // 有效点的行数
                    int row = j / columns;  // 获取行号
                    upperIndex = row * columns + columns * deltaRows;  // 设置上界
                    if(oneKnifeCut){  // 如果设置为一刀切

                    }
                    upperFlag = true;
                    //qDebug() << "j = " << j;
                    //qDebug() << "width: " << width << ", height: " << height;
                    //qDebug() << "跨越的行数: " << deltaRows;
                    //qDebug() << "当前行号: " << row;
                    //qDebug() << "上界: " << upperIndex;
                }
            }
        }
    }

    // 记录尝试的材料ID
    nestPiece.sheetID = sheetID;
    return nestPiece.nested;
}

/**
 * @brief PackPointNestEngine::compact
 * @param sheetID
 * @param nestPiece
 * @return
 * 靠接时优先进行重力方向上的靠接，
 * 然后再进行与重力方向垂直的方向上进行靠接
 */
bool PackPointNestEngine::compact(int sheetID, NestEngine::NestPiece &nestPiece)
{
    //qDebug() << "靠接算法";
    Piece piece = pieceList[nestPiece.typeID];  // 获取零件信息
    QPointF pos = nestPiece.position;  // 获取已排零件的位置
    piece.moveTo(pos);  // 将零件移动至pos
    piece.rotate(pos, nestPiece.alpha);  // 旋转
    //qDebug() << "原始位置：" << pos;
    // 重力方向靠接
    qreal stepY = compactStep;
    while(stepY > compactAccuracy){
        QPointF posTemp = pos;
        QPointF forwardPos(posTemp.rx(), posTemp.ry()-stepY);  // 重力反方向移动
        //qDebug() << "移动至：" << forwardPos;
        piece.moveTo(forwardPos);  // 将零件移至前进位置
        if(!piece.containsInSheet(sheetList[sheetID])){
            //qDebug() << "超出材料范围";
            stepY /= 2;
            continue;
        } else{
            //qDebug() << "包含在材料内";
        }
        //qDebug() << "零件 *" << nestPiece.index << " 位置： " << piece.getMinBoundingRect().center() << " 旋转：" << nestPiece.alpha;
        if(collidesWithOtherPieces(sheetID, piece)){
            // 往右移
            //qDebug() << "碰撞";
            stepY /= 2;
            continue;
        } else{
            //qDebug() << "没有碰撞";
        }
        pos.setY(pos.ry() - stepY);
    }
    //qDebug() << "重力方向靠接后的位置：" << pos;

    // 水平方向靠接
    qreal stepX = compactStep;
    while(stepX > compactAccuracy){
        QPointF posTemp = pos;
        QPointF forwardPos(posTemp.rx()-stepX, posTemp.ry());  // 水平方向移动
        //qDebug() << "移动至：" << forwardPos;
        piece.moveTo(forwardPos);  // 将零件移至新位置
        if(!piece.containsInSheet(sheetList[sheetID])){
            //qDebug() << "超出材料范围";
            stepX /= 2;
            continue;
        } else{
            //qDebug() << "包含在材料内";
        }
        //qDebug() << "零件 *" << nestPiece.index << " 位置： " << nestPiece.position << " 旋转：" << nestPiece.alpha;
        if(collidesWithOtherPieces(sheetID, piece)){
            //qDebug() << "碰撞";
            stepX /= 2;
            continue;
        } else{
            //qDebug() << "没有碰撞";
        }
        pos.setX(pos.rx() - stepX);
    }
    //qDebug() << "水平方向靠接后的位置：" << pos;
    //qDebug() << "";

    /***
     * 对最小高度进行优化：
     * 一般来说，通过靠接算法，会消除零件之间、零件与材料之间的的间隙。
     * 但是，如果仅仅按照上述方法进行靠接，可能会导致最小高度并不是最低的。
     * 这种情况往往发生在行尾，因此针对这种情况进行优化。
     *
     * 优化方案：
     * 将材料视图向右上方移动
     */
    if(minHeightOpt && pos.rx() >= 0.8 * sheetList[sheetID].width){
        qreal maxX = pos.rx() + piece.getMinBoundingRect().width();  // 右移的最大值
        qreal maxY = pos.ry() + piece.getMinBoundingRect().height();  // 上移的最大值
        while(pos.rx() <= maxX && pos.ry() <= maxY){
            bool moveFlag = false;  // 移动标志
            stepX = compactStep;  // 设置移动步长
            QPointF posTemp1 = pos;
            while(stepX > compactAccuracy) {
                QPointF forwardPos1(posTemp1.rx()+stepX, posTemp1.ry());  // 向右移动
                qDebug() << "移动至：" << forwardPos1;
                piece.moveTo(forwardPos1);  // 将零件移至新位置
                if(!piece.containsInSheet(sheetList[sheetID])){
                    qDebug() << "超出材料范围";
                    stepX /= 2;
                    continue;
                } else{
                    qDebug() << "包含在材料内";
                }
                //qDebug() << "零件 *" << nestPiece.index << " 位置： " << nestPiece.position << " 旋转：" << nestPiece.alpha;
                if(collidesWithOtherPieces(sheetID, piece)){
                    qDebug() << "碰撞";
                    stepX /= 2;
                    continue;
                } else{
                    qDebug() << "没有碰撞";
                }
                posTemp1.setX(posTemp1.rx() + stepX);  // 记录新的坐标

                stepY = compactStep;  // 设置步长
                while(stepY > compactAccuracy){
                    QPointF posTemp2 = posTemp1;
                    QPointF forwardPos2(posTemp2.rx(), posTemp2.ry()-stepY);
                    qDebug() << "移动至：" << forwardPos2;
                    piece.moveTo(forwardPos2);  // 将零件移至新位置2
                    if(!piece.containsInSheet(sheetList[sheetID])){
                        qDebug() << "超出材料范围";
                        stepY /= 2;
                        continue;
                    } else{
                        qDebug() << "包含在材料内";
                    }
                    //qDebug() << "零件 *" << nestPiece.index << " 位置： " << nestPiece.position << " 旋转：" << nestPiece.alpha;
                    if(collidesWithOtherPieces(sheetID, piece)){
                        qDebug() << "碰撞";
                        stepY /= 2;
                        continue;
                    } else{
                        qDebug() << "没有碰撞";
                    }
                    moveFlag = true;  // 更新移动标志
                    posTemp1.setY(posTemp1.ry() - stepY);
                }
            }
            if(!moveFlag){  // 如果无法继续移动，则结束
                qDebug() << "无法移动";
                break;
            }
            // 更新位置
            pos = posTemp1;
            qDebug() << "新的pos: " << pos;
        }
        qDebug() << "向右上方靠接后的位置：" << pos;
        // 水平方向靠接
        stepX = compactStep;
        while(stepX > compactAccuracy){
            QPointF posTemp = pos;
            QPointF forwardPos(posTemp.rx()-stepX, posTemp.ry());  // 水平方向移动
            //qDebug() << "移动至：" << forwardPos;
            piece.moveTo(forwardPos);  // 将零件移至新位置
            if(!piece.containsInSheet(sheetList[sheetID])){
                //qDebug() << "超出材料范围";
                stepX /= 2;
                continue;
            } else{
                //qDebug() << "包含在材料内";
            }
            //qDebug() << "零件 *" << nestPiece.index << " 位置： " << nestPiece.position << " 旋转：" << nestPiece.alpha;
            if(collidesWithOtherPieces(sheetID, piece)){
                //qDebug() << "碰撞";
                stepX /= 2;
                continue;
            } else{
                //qDebug() << "没有碰撞";
            }
            pos.setX(pos.rx() - stepX);
        }
        //qDebug() << "水平方向靠接后的位置：" << pos;
    }
    nestPiece.position = pos;
    return true;
}

bool PackPointNestEngine::collidesWithOtherPieces(int sheetID, Piece piece)
{
    // 判断两两零件是否碰撞, 优化方案：使用四叉树进行管理
    //qDebug() << "与对象: " << piece.getMinBoundingRect() << " 在同一象限的对象：";
    std::list<Object *> resObjects =
            quadTreeMap[sheetID]->retrieve(new Object(piece.getMinBoundingRect()));
    for(auto &t:resObjects){
        //qDebug()<< t->id << ' ' << t->x<<' '<<t->y<<' '<<t->width<<' '<<t->height;
        int id = t->id;
        int typeID = nestPieceList[id].typeID;
        Piece pieceNested = pieceList[typeID];
        pieceNested.moveTo(nestPieceList[id].position);
        pieceNested.rotate(nestPieceList[id].position, nestPieceList[id].alpha);
        if(pieceNested.collidesWithPiece(piece)){
            //qDebug() << "与 &" << nestPieceList[i].index << "，位置：" << nestPieceList[i].position;
            //qDebug() << "";
            return true;
        }
        collisionCount++;
    }
    //qDebug() << "";
    return false;

    QVector<int> pieceIDList = nestSheetPieceMap[sheetID];
    for(int i=0; i<pieceIDList.length(); i++){
        collisionCount++;
        int typeID = nestPieceList[i].typeID;
        Piece pieceNested = pieceList[typeID];
        pieceNested.moveTo(nestPieceList[i].position);
        pieceNested.rotate(nestPieceList[i].position, nestPieceList[i].alpha);
        if(pieceNested.collidesWithPiece(piece)){
            //qDebug() << "与 &" << nestPieceList[i].index << "，位置：" << nestPieceList[i].position;
            //qDebug() << "";
            return true;
        }
    }
    return false;
}








#include "continuenestengine.h"

ContinueNestEngine::ContinueNestEngine(QObject *parent) :
    NestEngine(parent)
{
    setNestEngineType(NestEngine::ContinueNest);
}

ContinueNestEngine::ContinueNestEngine(QObject *parent,
                                       const QVector<Piece> pieceList,
                                       const QVector<Sheet> sheetList) :
    NestEngine(parent, pieceList, sheetList)
{
    setNestEngineType(NestEngine::ContinueNest);
}

ContinueNestEngine::ContinueNestEngine(QObject *parent,
                                       const QVector<Piece> pieceList,
                                       const QVector<Sheet> sheetList,
                                       QVector<NestEngine::SameTypePiece> sameTypePieceList) :
    NestEngine(parent, pieceList, sheetList, sameTypePieceList)
{
    setNestEngineType(NestEngine::ContinueNest);
}

ContinueNestEngine::~ContinueNestEngine()
{

}

/**
 * @brief ContinueNestEngine::initPairPieceStatus  获取排版零件的初始状态
 * @param piece  待排零件
 * @param bestNestType  最佳排版方式
 * @param pieceWidth  最佳状态时零件宽度
 * @param pieceHeight  最佳状态时零件高度
 * @param pos1  零件1位置
 * @param pos2  零件2位置
 * @param alpha1  零件1角度
 * @param alpha2  零件2角度
 * @param xStep  送料步距
 * @param yStep  送料高度
 */
NestEngine::PairPieceStatus ContinueNestEngine::initPairPieceStatus(const QRectF& layoutRect, Piece &piece,
                                                                    const NestEngine::BestNestType &bestNestType)
{
    qreal pairWidth, pairHeight, pieceWidth, pieceHeight, alpha1, alpha2, xStep, yStep;
    QPointF pos1, pos2, pairCenter;

    piece.moveTo(QPointF(0, 0));  // 将零件移至坐标原点
    piece.rotate(piece.getPosition(), bestNestType.alpha);  // 按最佳方式旋转该零件
    QRectF boundingRect = piece.getBoundingRect();  // 获取该角度下的外包矩形
    pieceWidth = boundingRect.width();  // 获取外包矩形的宽
    pieceHeight = boundingRect.height();  // 获取外包矩形的高

    // 计算组合零件外包矩形
    pos1 = piece.getPosition();
    pos2 = pos1 + bestNestType.pOffset;
    QRectF pairBoundingRect = getPairBoundingRect(pos1, pos2, pieceWidth, pieceHeight);
    pairCenter = pairBoundingRect.center();
    pairWidth = pairBoundingRect.width();
    pairHeight = pairBoundingRect.height();

    // 确定两零件的初始位置
    QPointF offset = layoutRect.topLeft() - pairBoundingRect.topLeft();  // 确定该零件至材料左上角的偏移
    pos1 += offset;  // 将基准零件的左上角移动到材料可排区域的左上角
    pos2 += offset;  // 确定影子零件位置
    pairCenter += offset;  // 确定组合零件的中心

    switch (bestNestType.nestType) {
    case SingleRow:
        alpha1 = bestNestType.alpha;
        alpha2 = bestNestType.alpha;  // 确定零件旋转角度
        xStep = 2 * bestNestType.xStep;  // 送料步距
        break;
    case DoubleRow:
        alpha1 = bestNestType.alpha;
        alpha2 = bestNestType.alpha;  // 确定零件旋转角度
        xStep = bestNestType.xStep;  // 送料步距
        break;
    case OppositeSingleRow:
    case OppositeDoubleRow:
        alpha1 = bestNestType.alpha;  // 确定零件旋转角度
        alpha2 = alpha1 + 180;
        xStep = bestNestType.xStep;
        break;
    default:
        alpha1 = alpha2 = 0;
        xStep = 0;
        break;
    }
    yStep = bestNestType.yStep;

    // 返回组合状态
    PairPieceStatus status(pos1, pos2,
                           pieceWidth, pieceHeight,
                           alpha1, alpha2,
                           xStep, yStep,
                           pairCenter,
                           pairWidth, pairHeight);

    QRectF testRect = layoutRect;
    testRect.moveCenter(piece.getPosition());  // 移至原点
    // 判断矩形框内是否可以放下该矩形，如果不可以，则提前返回
    if(!testRect.contains(pairBoundingRect)){
        status.errorFlag = true;
    }

    return status;
}

bool ContinueNestEngine::packPieceByLayoutRect(const int sheetID,
                                               const QRectF& layoutRect,
                                               const int pieceType,
                                               int pieceIndex,
                                               const int pieceMaxIndex,
                                               const BestNestType bestNestType,
                                               PairPieceStatus &status,
                                               QPointF &anchorPos,
                                               qreal &spaceDelta,
                                               bool &sheetAvailable,
                                               QVector<int> &sameRowPieceList,
                                               QList<int> &nestedList,
                                               QList<int> &unnestedList,
                                               const RectTypes rectType,
                                               bool wholeSheetFlag,
                                               QRectF &layoutRect1,
                                               QRectF &layoutRect2)
{
    // 计算初始化状态
    qreal pieceWidth, pieceHeight, alpha1, alpha2, xStep;
    QPointF pos1, pos2;

    if((rectType & FirstRow) != NoRectType){
        qDebug() << "this is first row";
        Piece piece = pieceList[pieceType];  // 复制零件
        status = initPairPieceStatus(layoutRect, piece, bestNestType);
    }
    // 判断最佳排版方式是否在排版矩形内，
    if(status.errorFlag){
        qDebug() << "errorFlag" << endl;
        status.errorFlag = false;  // 一定要还原
        // 代表矩形内放不下该零件，需要重新进行排版方式计算
        Piece piece = pieceList[pieceType];  // 复制零件
        qreal alpha, xStep;
        QPointF pOffset, rCOffset;
        qreal maxWidth = layoutRect.width();
        qreal maxHeight = layoutRect.height();
        NestType nestType = getPieceBestNestType(piece, alpha, xStep, pOffset,
                                           rCOffset, 360, maxWidth, maxHeight);
        qDebug() << "best nest type: " << nestType;
        if(nestType == NoNestType){
            qDebug() << "can not pack";
            //return false;
        }
        BestNestType bestNestType(pieceType, nestType, alpha, xStep,
                                  pOffset, rCOffset);  // 获取该类型零件在限制材料高度时最佳排版方式
        status = initPairPieceStatus(layoutRect, piece, bestNestType);
        if(status.errorFlag){
            qDebug() << "should have return false!" << endl;
            //return false;
        }
    }
    if((rectType & TailPiece) != NoRectType){
        qDebug() << "this is tail piece row";
//        QPointF bottomLeft(status.pairCenter.rx() - 0.5 * status.pairWidth,
//                           status.pairCenter.ry() + 0.5 * status.pairHeight);
//        emit nestDebug(sheetID, bottomLeft, layoutRect.bottomLeft());
//        QPointF offset = layoutRect.bottomLeft() - bottomLeft;
//        status.pos1 += offset;
//        status.pos2 += offset;

        // 向上靠接
        // 向左靠接
    }
    if((rectType & SecondRow) != NoRectType){
        qDebug() << "This is second row";
        qDebug() << "status.yStep = " << status.yStep;
#if 0
        // 通过顶点计算y方向的
        if(status.yStep == 0){
            // 找出零件所在外包矩形所包含的零件列表
            QRectF testRect(layoutRect.left(),
                            layoutRect.top() - status.pieceHeight,
                            status.pieceWidth, status.pieceHeight);
            emit nestDebugRemainRect(sheetID, testRect);

            std::list<Object *> resObjects =
                    quadTreeMap[sheetID]->retrieve(new Object(testRect));

            QList<QPointF> vertexList;  // 记录所有在检测矩形内的顶点
            QList<Piece> pList;  // 记录所有参与计算的零件列表
            int i = 0;
            for(auto t : resObjects){
                Piece p = pieceList[nestPieceList[t->id].typeID];  // 复制零件
                p.moveTo(nestPieceList[t->id].position);  // 移动至指定位置
                p.rotate(p.getPosition(), nestPieceList[t->id].alpha);  // 旋转指定角度
                pList.append(p);  // 加入pieceList
                for(auto vertex : p.getPointsList()){
                    if(pointContainsInRect(testRect, vertex)){  // 如果顶点在矩形区域内就加入pList
                        if(i == 0){
                            vertexList.append(vertex);
                        }
                        if(i != 0){
                            for(auto p : vertexList){
                                if(vertex.ry() > p.ry()){
                                    vertexList.append(vertex);
                                }
                            }
                        }
                    }
                }
                i++;
            }

            qreal yStep = 0;  // 希望找到顶点与所有零件y方向上的最大距离
            QMap<qreal , QList<qreal>> intersectionMap;  // 交点Map
            for(auto vertex : vertexList){
                emit nestDebug(sheetID, vertex, vertex);
                // 过所有顶点做与y方向平行的直线，直线长度设置为pairHeight
                qreal x = vertex.rx();
                if(!intersectionMap.contains(x)){
                    QList<qreal> yList;
                    intersectionMap.insert(x, yList);
                }

                qreal y = vertex.ry();
                intersectionMap[x].append(y);

                QLineF line(x, testRect.top(), x, testRect.top() + status.pairHeight);
                emit nestDebugLine(sheetID, line);
                // 计算与零件交点坐标
                for(auto piece : pList){
                    QList<QPointF> points;
                    if(lineIntersectWithPolygon(piece.getPointsList(), line, points)){
                        for(auto point : points){
                            intersectionMap[x].append(point.ry());
                        }
                    }
                }
            }
            // 从得到的y坐标中，找出最大值与最小值，计算差值，然后再取最大值，即为yStep
            for(auto x : intersectionMap.keys()){
                qreal yMin = LONG_MAX;
                qreal yMax = 0;
                for(auto y : intersectionMap[x]){
                    if(y < yMin){
                        yMin = y;
                    }
                    if(y > yMax){
                        yMax = y;
                    }
                }
                qreal delta = yMax - yMin;
                if(delta > yStep){
                    yStep = delta;
                }
            }
            // 设置步长
            status.yStep = yStep;
        }
#endif

        // 通过靠接计算
        qDebug() << "status.yStep == 0" << (status.yStep == 0);
        if((int)status.yStep == 0){
#if 1
            // 首先将零件排放至矩形左上角，然后再向上靠接
            Piece p1 = pieceList[pieceType];  // 复制零件->p1
            p1.moveTo(status.pos1 + QPointF(0, status.pairHeight));
            p1.rotate(p1.getPosition(), status.alpha1);

            Piece p2 = pieceList[pieceType];  // 复制零件->p1
            p2.moveTo(status.pos2 + QPointF(0, status.pairHeight));
            p2.rotate(p2.getPosition(), status.alpha2);

            qreal yStep1 = compactOnVD(sheetID, p1);
            qreal yStep2 = compactOnVD(sheetID, p2);

            qDebug() << "yStep1: " << yStep1 << ", " << "yStep2: " << yStep2;

            qreal yStep = yStep1 < yStep2 ? yStep1 : yStep2;

            status.yStep = status.pairHeight - yStep;
#endif
        }
        // 更新位置
        status.pos1 += QPointF(0, status.yStep);
        status.pos2 += QPointF(0, status.yStep);
    }
    if((rectType & TailLine) != NoRectType){
        qDebug() << "This is tail line row";
    }

    pieceWidth = status.pieceWidth;
    pieceHeight = status.pieceHeight;
    alpha1 = status.alpha1;
    alpha2 = status.alpha2;
    xStep = status.xStep;
    pos1 = status.pos1;
    pos2 = status.pos2;

    // 获取边界信息
    qreal xMax = layoutRect.right();  // 获取矩形宽度最大值的x坐标
    qreal yMax = layoutRect.bottom();  // 获取矩形高度最大值的y坐标
    qreal rightDis = layoutRect.width();  // 记录矩形横向剩余距离
    qreal xCover = layoutRect.left();  // 记录零件所占矩形的宽度
    qreal yCover = layoutRect.top();  // 记录零件所占矩形的高度
    int columnCounter = 0;  // 记录之前的列计数器

#ifndef DEBUG
    qDebug() << "pieceWidth: " << pieceWidth;
    qDebug() << "pieceHeight: " << pieceHeight;
    qDebug() << "pos1: " << pos1;
    qDebug() << "pos2: " << pos2;
    qDebug() << "column: " << columnCounter << endl;
#endif

    while(pieceIndex <= pieceMaxIndex){
        qDebug() << "nesting: #" << pieceIndex;
        QPointF pos = (columnCounter % 2 == 0 ? pos1 : pos2)
                + QPointF(xStep * int(columnCounter/2), 0);
        qreal alpha = columnCounter % 2 == 0 ? alpha1 : alpha2;

        qreal x = pos.rx() + 0.5 * pieceWidth;  // 已排宽度
        qreal y = pos.ry() + 0.5 * pieceHeight;  // 已排高度

        // 零件超出矩形右边缘
        if(x > xMax){
            qDebug() << "#" << pieceIndex << " over right";
            // 如果一个零件都放不下，那么该材料太小，需要重新设置材料大小
            if(columnCounter < 1){
                qDebug() << "this sheet is pretty not enough!";
                emit(nestException(SheetSizeError));  // 发送材料大小异常
                return false;
            }

            // 如果矩形类型不为尾只 且 设置了尾只优化
            if((mixingTyes & NestEngine::TailPieceMixing) != NestEngine::NoMixing){
                // 在同型体内寻找较小
                qDebug() << "set tailPieceMix!" << endl << "searching the sameTypePiece ... " << endl;
                // 判断是否设置了同型体列表
                if(sameTypeNestPieceIndexMap.contains(pieceType)){
                    QVector<int> typeIdList;
                    typeIdList.append(pieceType);  // 先在同种类型中尝试
                    typeIdList.append(sameTypeNestPieceIndexMap[pieceType]);  // 然后再尝试该类型零件的同型体
                    foreach (int type, typeIdList) {  // 遍历该列表
                        // 尝试该零件是否能排下
                        int minIndexTmp = nestPieceIndexRangeMap[type].minIndex;  // 获取该类型零件的最小值
                        int maxIndexTmp = nestPieceIndexRangeMap[type].maxIndex;  // 获取该类型零件的最大值
                        int chooseIndex = -1;  // 选择未排版的那个零件进行排放
                        for(int k = minIndexTmp; k <= maxIndexTmp; k++){
                            if(!nestPieceList[k].nested){
                                chooseIndex = k;
                                break;
                            }
                        }
                        if(chooseIndex == -1){  // 如果该类型所有零件都已排放，则继续寻找下一同型体
                            continue;
                        }

                        /*
                         * 运行到这里代表已经找到了满足条件的同型体零件
                         * 尝试排放该零件，这里递归调用本身
                         * 否则，继续尝试排放下一个同型体.
                         *
                         * 计算出前一个零件与剩余材料构成的矩形，
                         * 然后在这个小的范围内尝试排放
                         */
                        qreal x, y;
                        QPointF posPre1 = ((columnCounter-1) % 2 == 0 ? pos1 : pos2)
                                + QPointF(xStep * int((columnCounter-1)/2), 0);  // 找到前一个零件的位置
                        x = posPre1.rx() + 0.5 * pieceWidth;  // 计算前一个零件纵坐标最小值
                        y = posPre1.ry() - 0.5 * pieceHeight;  // 计算前一个零件纵坐标最小值

                        qreal sheetRemainWidth = xMax - x;
                        qreal sheetRemainHeight = pieceHeight;  // 计算两组合零件外包矩形的高

                        if(columnCounter >= 2){  // 如果排放的零件多于2个，则可能错开一定高度
                            QPointF posPre2 = ((columnCounter-2) % 2 == 0 ? pos1 : pos2)
                                    + QPointF(xStep * int((columnCounter-2)/2), 0); // 找到前前一个零件的位置
                            qreal y1 = posPre2.ry() - 0.5 * pieceHeight;
                            qreal y2 = posPre2.ry() + 0.5 * pieceHeight;
                            if(y1 < y){
                                y = y1;
                            }
                            sheetRemainHeight = y2 - y;
                        }

                        if((rectType & TailLine) != NoRectType){  // 如果是尾行的话，剩余高度为
                            sheetRemainHeight = layoutRect.height();
                        }
                        QRectF tailPieceRect(x, y, sheetRemainWidth, sheetRemainHeight);  // 这个为尾只零件测试区域
                        qDebug() << "tail piece Rect: " << tailPieceRect << endl;
                        emit nestDebugRemainRect(sheetID, tailPieceRect);

                        // 尾只排放，递归调用
                        PairPieceStatus status;
                        BestNestType bestNestType = pieceBestNestTypeMap[type];
                        QRectF layoutRect1, layoutRect2;
                        bool sheetAvailable = true;
                        bool res = packPieceByLayoutRect(sheetID, tailPieceRect, type, chooseIndex, maxIndexTmp,
                                                         bestNestType, status, anchorPos, spaceDelta, sheetAvailable, sameRowPieceList,
                                                         nestedList, unnestedList, TailPiece|FirstRow, false, layoutRect1, layoutRect2);
                        if(res){
                            qDebug() << "tail piece mixing successful!" << endl;
                            break;
                        } else{
                            qDebug() << "tail piece mixing failed!" << endl;
                        }
                    }
                }
            }

            // 如果设置了横向自适应
            if((adaptiveSpacingTypes & HorizontalAdaptiveSpacing) !=  NoAdaptiveSpacing){
                // 计算右边剩余距离
                rightDis = xMax - xCover;
                qDebug() << "xMax: " << xMax << ", width: " << xCover << "right distance: " << rightDis;
                spaceDelta = rightDis / (sameRowPieceList.length()-1);

                // 每一零件的偏移量
                for(int i=0; i<sameRowPieceList.length(); i++){
                    int id = sameRowPieceList.at(i);
                    nestPieceList[id].position += QPointF(spaceDelta, 0) * i;  // 记录下一零件排放位置
                }
            }

            // 换行，同样递归调用本身
            columnCounter = 0;  // 列计数器清零
            spaceDelta = 0.0f;
            sameRowPieceList.clear();

            // 更新排版矩形
            QRectF secondRowRect(layoutRect.left(), yCover, layoutRect.width(), layoutRect.bottom() - yCover);
            emit nestDebugRemainRect(sheetID, secondRowRect);
            qDebug() << "second row Rect: " << secondRowRect;
            bool res = packPieceByLayoutRect(sheetID, secondRowRect, pieceType, pieceIndex, pieceMaxIndex,
                                             bestNestType, status, anchorPos, spaceDelta, sheetAvailable, sameRowPieceList,
                                             nestedList, unnestedList, SecondRow, true, layoutRect1, layoutRect2);
            return res;
        }

        // 零件超出矩形下边缘
        if(y > yMax){
            qDebug() << "#" << pieceIndex << " over bottom";
            // 如果矩形类型不为尾行 且 设置了尾行优化
            if((mixingTyes & NestEngine::TailLineMixing) != NestEngine::NoMixing){  // (rectType & TailLine) == NoRectType &&
                qDebug() << "tail line mixing" << endl;
                qreal maxWidth = layoutRect.width();
                qreal maxHeight = layoutRect.bottom()-yCover;  // 获取剩余材料最大高度
                QRectF tailLineRect(layoutRect.left(), yCover, layoutRect.width(), maxHeight);  // 剩余排版矩形
                emit nestDebugRemainRect(sheetID, tailLineRect);  //debug
                QVector<int> typeList;  // 记录零件类型集合
                typeList.append(pieceType);  // 先把自己加入该类型列表
                if(sameTypeNestPieceIndexMap.contains(pieceType)){
                    typeList.append(sameTypeNestPieceIndexMap[pieceType]);  // 获取该类型零件的同型体
                }
                foreach (int type, typeList) {
                    qDebug() << "type: " << type;
                    Piece pTailLine = pieceList[type];  // 复制该零件
                    qreal alphaTailLine, xStepTailLine;
                    QPointF pOffsetTailLine, rCOffsetTailLine;
                    NestType nestType = getPieceBestNestType(pTailLine, alphaTailLine, xStepTailLine, pOffsetTailLine,
                                                       rCOffsetTailLine, 360, maxWidth, maxHeight);
                    qDebug() << "best nest type: " << nestType;
                    if(nestType == NoNestType){
                        qDebug() << "can not pack";
                        break;
                    }
                    BestNestType bestNestType(pieceType, nestType, alphaTailLine, xStepTailLine,
                                     pOffsetTailLine, rCOffsetTailLine);  // 获取该类型零件在限制材料高度时最佳排版方式

                    bool res = packPieceByLayoutRect(sheetID, tailLineRect, type, pieceIndex, pieceMaxIndex,
                                                     bestNestType, status, anchorPos, spaceDelta, sheetAvailable, sameRowPieceList,
                                                     nestedList, unnestedList, TailLine|FirstRow, true, layoutRect1, layoutRect2);
                    if(res){
                        break;
                    }
                }
            }

            // 材料不足并需要返回
            sheetAvailable = false;  // 这里有个bug，当当前尾行排放的零件类型排完时，材料不一定排完
            break;
        }

        nestPieceList[pieceIndex].sheetID = sheetID;  // 记录下一零件排放材料id
        nestPieceList[pieceIndex].position = pos;  // 记录下一零件排放位置
        nestPieceList[pieceIndex].alpha = alpha;  // 记录下一零件旋转角度
        nestPieceList[pieceIndex].nested = true;  // 设置已排
        sameRowPieceList.append(pieceIndex);  // 更新同行零件列表
        nestedList.append(pieceIndex);  // 更新已排零件列表

        // 将该对象加入四叉树中
        QRectF boundingRect(pos.rx()-0.5*pieceWidth, pos.ry()-0.5*pieceHeight, pieceWidth, pieceHeight);
        Object *obj = new Object(pieceIndex, boundingRect);
        QuadTreeNode<Object> *quadTree = quadTreeMap[sheetID];
        quadTree->insert(obj);

        // debug
        emit nestPieceUpdate(nestPieceList[pieceIndex]);

        xCover = pos.rx() + 0.5 * pieceWidth;  // 已排宽度
        yCover = pos.ry() + 0.5 * pieceHeight;  // 已排高度
        anchorPos = QPointF(xCover, yCover);  // 记录排放完该零件的右下角位置，即锚点
        columnCounter++;  // 计数器
        pieceIndex++;  // 更新下一零件index
    }

    qDebug() << "sheetAvaliable: " << sheetAvailable << ",  wholeSheetFlag: " << wholeSheetFlag;
    if(sheetAvailable && wholeSheetFlag){  // 如果材料没用完  //
        QPointF availablePos1 = QPointF(anchorPos.rx(), anchorPos.ry()-status.pairHeight);
        QPointF availablePos2 = QPointF(layoutRect.left(), anchorPos.ry());
        layoutRect1 = QRectF(availablePos1.rx(), availablePos1.ry(),
                             layoutRect.right() - availablePos1.rx(), status.pairHeight);
        layoutRect2 = QRectF(availablePos2.rx(), availablePos2.ry(),
                             layoutRect.width(), layoutRect.bottom() - availablePos2.ry());
        //emit nestDebugRemainRect(sheetID, layoutRect1);
        //emit nestDebugRemainRect(sheetID, layoutRect2);
        //qDebug() << layoutRect << "  " << layoutRect1 << "  " << layoutRect2;
        //emit(nestDebug(sheetID, availablePos1, availablePos2));
    }

    // 如果所有材料都已排完，同行零件列表不为空，则默认按上一行的间隔进行自适应
    if(!sameRowPieceList.isEmpty()){
        for(int i=0; i<sameRowPieceList.length(); i++){
            int id = sameRowPieceList.at(i);
            nestPieceList[id].position += QPointF(spaceDelta, 0) * i;  // 记录下一零件排放位置
        }
    }

    // 更新未排列表，在未排列表中删除已排列表
    foreach (int index, nestedList) {
        nestedPieceIndexlist.append(index);  // 更新已排列表
        nestSheetPieceMap[sheetID].append(index);  // 更新材料-零件索引
        unnestedList.removeOne(index);  // 更新未排列表
    }
    return true;
}

void ContinueNestEngine::packPieces(QVector<int> indexList)
{
    // 零件排放未结束标志
    bool interruptFlag = false;

    // 将向量转化为列表，因为后面会经常删除元素
    QList<int> unnestedList = indexList.toList();

    // 排版，按照材料进行排版，排满一张后再排下一张
    int sheetID = 0;
    // 初始排版矩形区域
    QRectF layoutRect = sheetList[sheetID].layoutRect();
    QRectF layoutRect1 = layoutRect;
    QRectF layoutRect2 = layoutRect;

    PairPieceStatus status;  // 组合零件状态
    RectTypes rectType = FirstRow;  // 初始矩形类型

    // 排放零件直到所有零件都排放完
    while(!interruptFlag && !unnestedList.isEmpty()){
        // 初始化状态信息，用来记录排版过程中的中间数据，或一些标志位
        if(!nestSheetPieceMap.contains(sheetID)){  // 初始化材料-零件索引
            QVector<int> pieceList;
            nestSheetPieceMap.insert(sheetID, pieceList);
        }

        // 确定初始状态
        int index = unnestedList.first();  // 获取未排零件的第一个零件
        int type = nestPieceList[index].typeID;  // 获取该零件的类型号
        BestNestType bestNestType = pieceBestNestTypeMap[type];  // 获取该类型零件的最佳排版方式

        bool sheetAvailable = true;  // 记录材料是否用完
        qreal spaceDelta = 0.0f;  // 自适应间隔增量
        QPointF anchorPos;  // 记录锚点位置，记录的是零件排放后外包矩形右下角位置
        QVector<int> sameRowPieceList;  // 记录同一行零件列表
        QList<int> nestedList;  // 记录已排零件
        int maxIndex = nestPieceIndexRangeMap[type].maxIndex;  // 获取该类型零件的最大值

        bool res = packPieceByLayoutRect(sheetID, layoutRect, type, index, maxIndex, bestNestType,
                                         status, anchorPos, spaceDelta, sheetAvailable, sameRowPieceList,
                                         nestedList, unnestedList, rectType, true, layoutRect1, layoutRect2);
        if(!res){
            emit nestInterrupted(unnestedList.length());
            //emit nestFinished(nestPieceList);
            return;
        }
        qDebug() << "sheet #" << sheetID << " has finished" << endl;
        if(unnestedList.isEmpty()){
            break;
        }
        if(!sheetAvailable){
            int sheetLength = sheetList.length();  // 获取材料列表长度
            if(sheetLength <= sheetID+1){  // 如果材料不足
                if(autoRepeatLastSheet){  // 如果设置了自动重复最后一张材料
                    Sheet sheet = sheetList[sheetID];
                    layoutRect = sheet.layoutRect();
                    layoutRect1 = layoutRect;
                    layoutRect2 = layoutRect;
                    sheetList.append(sheet);
                    initQuadTreeMap(sheetID+1);  // 初始化该张材料的四叉树
                    sheetID++;
                    emit autoRepeatedLastSheet(sheet);  // 排版结束后发送 重复了最后一张材料
                } else{
                    int num = unnestedList.length();
                    emit nestInterrupted(num);
                    interruptFlag = true;  // 设置中断标志为true
                }
            }
            rectType = FirstRow;  // 更新矩形类型
        } else {  // 如果该矩形未排完，则
            qDebug() << "sheet is still available";
            int index = unnestedList.first();  // 获取未排零件的第一个零件
            qDebug() << "pack #" << index;
            int type = nestPieceList[index].typeID;  // 获取该零件的类型号
            BestNestType bestNestType = pieceBestNestTypeMap[type];  // 获取该类型零件的最佳排版方式
            int maxIndex = nestPieceIndexRangeMap[type].maxIndex;  // 获取该类型零件的最大值
            QRectF layoutRectTmp = layoutRect2;

            emit nestDebugRemainRect(sheetID, layoutRect);
            qreal top0 = layoutRect1.top()-status.yStep+status.pairHeight;
            qreal deltaHeight = top0 - layoutRect1.top();
            if(layoutRect1.height()-deltaHeight != 0){
                layoutRect = QRectF(layoutRect1.left(),
                                    layoutRect1.top()-status.yStep+status.pairHeight,
                                    layoutRect1.width(),
                                    layoutRect1.height()-deltaHeight);
            } else{
                layoutRect = layoutRect1;
            }
            layoutRect1 = layoutRect;
            layoutRect2 = layoutRect;
            emit nestDebugRemainRect(sheetID, layoutRect);  // debug

            packPieceByLayoutRect(sheetID, layoutRect, type, index, maxIndex, bestNestType,
                                  status, anchorPos, spaceDelta,
                                  sheetAvailable, sameRowPieceList,
                                  nestedList, unnestedList, TailPiece|FirstRow, false, layoutRect1, layoutRect2);

            // 为下次循环更新条件
            rectType = FirstRow;
            layoutRect = layoutRectTmp;
            layoutRect1 = layoutRect;
            layoutRect2 = layoutRect;
        }
    }

    // 发送排放结束信号
    //emit(nestFinished(nestPieceList));
}

bool ContinueNestEngine::packOnePiece(Piece piece, NestEngine::NestPiece &nestPiece)
{
    Q_UNUSED(piece);
    Q_UNUSED(nestPiece);
    return true;
}

bool ContinueNestEngine::packOnePieceOnSheet(Piece piece, int sheetID, NestEngine::NestPiece &nestPiece)
{
    Q_UNUSED(piece);
    Q_UNUSED(sheetID);
    Q_UNUSED(nestPiece);
    return true;
}

bool ContinueNestEngine::compact(int sheetID, NestEngine::NestPiece &nestPiece)
{
    Q_UNUSED(sheetID);
    Q_UNUSED(nestPiece);
    return true;
}

qreal ContinueNestEngine::compactOnVD(int sheetID, Piece piece)
{
    //qDebug() << "垂直方向靠接算法";
    QPointF pos = piece.getPosition();
    QPointF posOld = pos;  // 记录之前的位置
    //qDebug() << "origin position: " << pos;
    // 重力方向靠接
    qreal stepY = compactStep;
    while(stepY > compactAccuracy){
        QPointF posTemp = pos;
        QPointF forwardPos(posTemp.rx(), posTemp.ry()-stepY);  // 重力反方向移动
        //qDebug() << "move to" << forwardPos;
        piece.moveTo(forwardPos);  // 将零件移至前进位置
        if(collidesWithOtherPieces(sheetID, piece)){
            // 往下移
            ///qDebug() << "collides with others";
            stepY /= 2;
            continue;
        }
        pos.setY(pos.ry() - stepY);
    }
    //qDebug() << "now position: " << pos;
    qreal step = posOld.ry() - pos.ry();
    return step;
}

bool ContinueNestEngine::collidesWithOtherPieces(int sheetID, Piece piece)
{
    // 判断两两零件是否碰撞, 优化方案：使用四叉树进行管理
    //qDebug() << "与对象: " << piece.getBoundingRect() << " 在同一象限的对象：";
    std::list<Object *> resObjects =
            quadTreeMap[sheetID]->retrieve(new Object(piece.getBoundingRect()));
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
}

QRectF ContinueNestEngine::getPairBoundingRect(QPointF &pos1, QPointF &pos2,
                                               const qreal pieceWidth, const qreal pieceHeight)
{
    qreal xMin1 = pos1.rx()-0.5*pieceWidth;
    qreal yMin1 = pos1.ry()-0.5*pieceHeight;
    qreal xMax1 = pos1.rx()+0.5*pieceWidth;
    qreal yMax1 = pos1.ry()+0.5*pieceHeight;

    qreal xMin2 = pos2.rx()-0.5*pieceWidth;
    qreal yMin2 = pos2.ry()-0.5*pieceHeight;
    qreal xMax2 = pos2.rx()+0.5*pieceWidth;
    qreal yMax2 = pos2.ry()+0.5*pieceHeight;

    qreal pairLeft = xMin1 < xMin2 ? xMin1 : xMin2;  // 组合零件左边界
    qreal pairTop = yMin1 < yMin2 ? yMin1 : yMin2;  // 组合零件上边界
    qreal pairRight = xMax1 > xMax2 ? xMax1 : xMax2;  // 组合零件右边界
    qreal pairBottom = yMax1 > yMax2 ? yMax1 : yMax2;  // 组合零件下边界

    qreal pairWidth = pairRight - pairLeft;  // 组合零件的宽
    qreal pairHeight = pairBottom - pairTop;  // 组合零件的高
    QRectF pairBoundingRect(pairLeft, pairTop, pairWidth, pairHeight);  // 组合零件的外包矩形
    return pairBoundingRect;
}

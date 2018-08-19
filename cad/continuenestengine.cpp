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

    bool single;  // 是否是单排
    switch (bestNestType.nestType) {
    case SingleRow:
        single = true;
        alpha1 = bestNestType.alpha;
        alpha2 = bestNestType.alpha;  // 确定零件旋转角度
        xStep = bestNestType.xStep;  // 送料步距
        break;
    case DoubleRow:
        single = false;
        alpha1 = bestNestType.alpha;
        alpha2 = bestNestType.alpha;  // 确定零件旋转角度
        xStep = bestNestType.xStep;  // 送料步距
        break;
    case OppositeSingleRow:
        single = true;
        alpha1 = bestNestType.alpha;  // 确定零件旋转角度
        alpha2 = alpha1 + 180;
        xStep = bestNestType.xStep;
        break;
    case OppositeDoubleRow:
        single = false;
        alpha1 = bestNestType.alpha;  // 确定零件旋转角度
        alpha2 = alpha1 + 180;
        xStep = bestNestType.xStep;
        break;
    default:
        single = -1;
        alpha1 = alpha2 = 0;
        xStep = 0;
        break;
    }
    yStep = bestNestType.yStep;

    // 返回组合状态
    PairPieceStatus status(pos1, pos2,
                           pieceWidth, pieceHeight,
                           pieceWidth, pieceHeight,
                           alpha1, alpha2,
                           xStep, yStep,
                           pairCenter,
                           pairWidth, pairHeight);

    // 判断矩形框内是否可以放下该矩形，如果不可以，使能错误标志
    qreal layoutRectWidth = layoutRect.width();  // 排版区域宽度
    qreal layoutRectHeight = layoutRect.height();  // 排版区域高度
    if(single){
        if(layoutRectWidth < pieceWidth || layoutRectHeight < pieceHeight){
            status.errorFlag = true;
        }
    } else{
        if(layoutRectWidth < pairWidth || layoutRectHeight < pairHeight){
            status.errorFlag = true;
        }
    }

    return status;
}

NestEngine::PairPieceStatus ContinueNestEngine::initPairPieceStatus(const QRectF &layoutRect, Piece &piece1, Piece &piece2)
{
    qreal xStep1;
    QPointF pos11, pos21;
    bool flag1 = true;
    QRectF bRect11, bRect21, pairBoundRect1;
    qreal rate1 = oppositeDoubleRowNestWithVerAlgForStrip(layoutRect, piece1, piece2,
                                                          pos11, pos21, xStep1, flag1,
                                                          bRect11, bRect21, pairBoundRect1);

    qreal xStep2;
    QPointF pos12, pos22;
    bool flag2 = false;
    QRectF bRect12, bRect22, pairBoundRect2;
    qreal rate2 = oppositeDoubleRowNestWithVerAlgForStrip(layoutRect, piece1, piece2,
                                                          pos12, pos22, xStep2, flag2,
                                                          bRect12, bRect22, pairBoundRect2);

    qreal xStep;
    QPointF pos1, pos2;
    qreal alpha1, alpha2;
    QRectF bRect1, bRect2, pairBoundRect;
    if(rate1 > rate2){
        xStep = xStep1;
        pos1 = pos11;
        pos2 = pos21;
        alpha1 = 0;
        alpha2 = 180;
        bRect1 = bRect11;
        bRect2 = bRect21;
        pairBoundRect = pairBoundRect1;
    } else{
        xStep = xStep2;
        pos1 = pos12;
        pos2 = pos22;
        alpha1 = 180;
        alpha2 = 0;
        bRect1 = bRect12;
        bRect2 = bRect22;
        pairBoundRect = pairBoundRect2;
    }
    // 返回组合状态
    PairPieceStatus status(pos1, pos2,
                           bRect1.width(), bRect1.height(),
                           bRect2.width(), bRect2.height(),
                           alpha1, alpha2,
                           xStep, 0,
                           QPointF(),
                           pairBoundRect.width(), pairBoundRect.height());
    return status;
}

NestEngine::SinglePieceStatus ContinueNestEngine::initSinglePieceStatus(const QRectF &layoutRect, const Piece &piece, int &flag)
{
    QPointF pos;
    qreal alpha;
    qreal pieceWidth, pieceHeight;
    if(flag == 0){  // 如果flag = 0，则需要对比两种方法的使用率
        Piece p1 = piece;  // 复制，零件1
        p1.moveTo(QPointF(0,0));  // 移动至原点，非必须
        p1.rotateByReferenceLine(p1.getPosition(), true);  // 旋转
        QPointF offset1 = p1.getOffsetForReferenceLine(layoutRect);
        QPointF gPoint1 = p1.getCenterPoint();
        Piece p2 = piece;  // 复制，零件1
        p2.moveTo(QPointF(0,0));  // 移动至原点，非必须
        p2.rotateByReferenceLine(p2.getPosition(), false);  // 旋转
        QPointF offset2 = p2.getOffsetForReferenceLine(layoutRect);
        QPointF gPoint2 = p2.getCenterPoint();
        if(gPoint1.rx() <= gPoint2.rx()){
            pos = p1.getPosition() + offset1;
            QRectF bRect = p1.getBoundingRect();
            pieceWidth = bRect.width();
            pieceHeight = bRect.height();
            alpha = 0;
            flag = 1;
        } else{
            pos = p2.getPosition() + offset2;
            QRectF bRect = p2.getBoundingRect();
            pieceWidth = bRect.width();
            pieceHeight = bRect.height();
            alpha = 180;
            flag = -1;
        }
    }
    else{
        // 确定初始位置
        Piece p = piece;  // 复制，零件1
        p.moveTo(QPointF(0,0));  // 移动至原点，非必须

        if(flag == -1){  // 如果flag=-1，则代表false
            p.rotateByReferenceLine(p.getPosition(), false);  // 旋转
            alpha = 180;
        } else{  //  如果flag=1，则代表true
            p.rotateByReferenceLine(p.getPosition(), true);  // 旋转
            alpha = 0;
        }

        // 把需要排放在条板内的零件区域标注出来
        QPointF offset = p.getOffsetForReferenceLine(layoutRect);
        pos = p.getPosition() + offset;
        QRectF bRect = p.getBoundingRect();
        pieceWidth = bRect.width();
        pieceHeight = bRect.height();
    }

    SinglePieceStatus status(pos, pieceWidth, pieceHeight, alpha);
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
                                               QRectF &layoutRect1,
                                               QRectF &layoutRect2)
{
    bool packRes = true;  // 排版结果
    // 计算初始化状态
    qreal pairWidth, pairHeight, pieceWidth, pieceHeight, alpha1, alpha2, xStep;
    QPointF pos1, pos2;

    Piece piece = pieceList[pieceType];  // 复制零件
    // qreal yStep = status.yStep;  // 记录yStep
    qDebug() << "to nest $" << pieceIndex << ", bestNestType: " << (int)bestNestType.nestType;
    status = initPairPieceStatus(layoutRect, piece, bestNestType);  // 初始化排版状态

    // 判断最佳排版方式是否在排版矩形内
    if(status.errorFlag){
        qDebug() << "errorFlag";
        status.errorFlag = false;  // 一定要还原

        if(((rectType & FirstRow) != NoRectType &&
            (mixingTyes & NestEngine::TailPieceMixing) != NestEngine::NoMixing) ||
                (bestNestType.nestType & DoubleRow) != NoNestType ||
                (bestNestType.nestType & OppositeDoubleRow) != NoNestType){
            // 代表矩形内放不下该零件，需要重新进行排版方式计算
            Piece piece = pieceList[pieceType];  // 复制零件
            qreal alpha, xStep;
            QPointF pOffset, rCOffset;
            qreal maxWidth = layoutRect.width();
            qreal maxHeight = layoutRect.height();
            NestType nestType = getPieceBestNestType(piece, alpha, xStep, pOffset,
                                               rCOffset, 360, maxWidth, maxHeight);
            qDebug() << "best nest type: " << nestType;
            if(nestType == NoNestType){  // 无论如何都放不下该零件，则返回false
                qDebug() << "can not pack" << endl;
                return false;
            }
            // 获取该类型零件在限制材料高度时最佳排版方式
            BestNestType bestNestType(pieceType, nestType, alpha, xStep,
                                      pOffset, rCOffset);
            status = initPairPieceStatus(layoutRect, piece, bestNestType);
            if(status.errorFlag){
                qDebug() << "return false" << endl;
                status.errorFlag = false;  // 返回
                return false;
            }
        }
        if(((rectType & FirstRow) != NoRectType) &&
                (mixingTyes & NestEngine::TailPieceMixing) == NestEngine::NoMixing){
            return false;
        }

        if((rectType & SecondRow) != NoRectType &&
                (mixingTyes & NestEngine::TailLineMixing) == NestEngine::NoMixing){  // 尾行 并且 未设置尾行混合
            return false;
        }

        if((rectType & SecondRow) != NoRectType &&
                (mixingTyes & NestEngine::TailLineMixing) != NestEngine::NoMixing){
            if(status.pos1.rx() + 0.5 * status.pieceWidth1 > layoutRect.right()){
                return false;
            }
        }
    }

#if 0
    if((rectType & FirstRow) != NoRectType){
        qDebug() << "this is first row";
    }
    if((rectType & TailPiece) != NoRectType){
        qDebug() << "this is tail piece row";
    }
    if((rectType & SecondRow) != NoRectType){
        // 判断是否可以放下该零件
        qDebug() << "This is second row";
        if(!oneKnifeCut){  // 如果为一刀切，则不再向上靠接
            status.yStep = yStep;
            qDebug() << "status.yStep = " << status.yStep;
            // 通过靠接计算
            if((int)status.yStep == 0){
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
            }
            // 更新位置
            status.pos1 += QPointF(0, status.yStep);
            status.pos2 += QPointF(0, status.yStep);
        }
    }
    if((rectType & TailLine) != NoRectType){
        qDebug() << "This is tail line row";
    }
#endif

    pairWidth = status.pairWidth;
    pairHeight = status.pairHeight;
    pieceWidth = status.pieceWidth1;
    pieceHeight = status.pieceHeight1;
    alpha1 = status.alpha1;
    alpha2 = status.alpha2;
    xStep = status.xStep;
    pos1 = status.pos1;
    pos2 = status.pos2;

    // 获取边界信息
    layoutRect1 = layoutRect2 = layoutRect;  // 初始化剩余矩形
    qreal xMin = layoutRect.left();  // 获取矩形宽度最小值的x坐标
    qreal xMax = layoutRect.right();  // 获取矩形宽度最大值的x坐标
    qreal yMin = layoutRect.top();  // 获取矩形高度最小值的y坐标
    qreal yMax = layoutRect.bottom();  // 获取矩形高度最大值的y坐标
    qreal rightDis = layoutRect.width();  // 记录矩形横向剩余距离
    qreal xCover = xMin;  // 记录零件所占矩形的宽度
    qreal yCover = yMin;  // 记录零件所占矩形的高度
    int columnCounter = 0;  // 记录之前的列计数器

    while(pieceIndex <= pieceMaxIndex){
        if(nestPieceList[pieceIndex].nested){
            pieceIndex += 1;
            continue;
        }
        qDebug() << "nesting: #" << pieceIndex;
        QPointF pos = (columnCounter % 2 == 0 ? pos1 : pos2)
                + QPointF(xStep * int(columnCounter/2), 0);
        qreal alpha = columnCounter % 2 == 0 ? alpha1 : alpha2;

        qreal x = pos.rx() + 0.5 * pieceWidth;  // 已排宽度
        qreal y = pos.ry() + 0.5 * pieceHeight;  // 已排高度

        // 零件超出矩形右边缘
        if(x > xMax){
            qDebug() << "over x";
            // 如果设置了尾只优化
            if((mixingTyes & NestEngine::TailPieceMixing) != NestEngine::NoMixing){
                // 在同型体内寻找较小
                qDebug() << "set tailPieceMix!";
                // 判断是否设置了同型体列表
                // 在同型体内寻找较小
                qDebug() << "searching the sameTypePiece ... ";
                // 判断是否设置了同型体列表
                qDebug() << "searching #" << pieceType << "'s sameType";
                if(sameTypeNestPieceIndexMap.contains(pieceType)) {
                    QVector<int> typeIdList;
                    typeIdList.append(pieceType);  // 先在同种类型中尝试
                    typeIdList.append(sameTypeNestPieceIndexMap[pieceType]);  // 然后再尝试该类型零件的同型体
                    foreach (int type, typeIdList) {  // 遍历该列表
                        qDebug() << "type: " << type;
                        // 尝试该零件是否能排下
                        int minIndexTmp = nestPieceIndexRangeMap[type].minIndex;  // 获取该类型零件的最小值
                        int maxIndexTmp = nestPieceIndexRangeMap[type].maxIndex;  // 获取该类型零件的最大值
                        int step = nestPieceIndexRangeMap[type].step;   // 获取该零件的步长
                        int chooseIndex = -1;  // 选择未排版的那个零件进行排放
                        for(int k = maxIndexTmp; k >= minIndexTmp; k -= step){
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
                        // 尾只排放，递归调用
                        PairPieceStatus status;
                        BestNestType bestNestType = pieceBestNestTypeMap[type];
                        bool sheetAvailable = true;
                        QRectF layoutRect2;
                        emit nestDebugRemainRect(sheetID, layoutRect1);
                        bool res = packPieceByLayoutRect(sheetID, layoutRect1, type, chooseIndex,
                                                         maxIndexTmp, bestNestType, status, anchorPos,
                                                         spaceDelta, sheetAvailable, sameRowPieceList,
                                                         nestedList, unnestedList, FirstRow,
                                                         layoutRect1, layoutRect2);
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
                rightDis = xMax - layoutRect1.left();
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

            emit nestDebugRemainRect(sheetID, layoutRect2);
            // 更新排版矩形
            bool res = packPieceByLayoutRect(sheetID, layoutRect2, pieceType, pieceIndex,
                                             pieceMaxIndex, bestNestType, status, anchorPos,
                                             spaceDelta, sheetAvailable, sameRowPieceList,
                                             nestedList, unnestedList, SecondRow,
                                             layoutRect1, layoutRect2);
            if(res){
                qDebug() << "second row pack successful";
                return true;
            } else{
                qDebug() << "second row pack fialed";
                sheetAvailable = false;
                packRes = false;
                break;
            }
        }

        // 零件超出矩形下边缘
        if(y > yMax){
            qDebug() <<"over y";
            // 如果矩形类型不为尾行 且 设置了尾行优化
            if((mixingTyes & NestEngine::TailLineMixing) != NestEngine::NoMixing){
                qDebug() << "tail line mixing" << endl;
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
                    NestType nestType = getPieceBestNestType(pTailLine, alphaTailLine,
                                                             xStepTailLine, pOffsetTailLine,
                                                             rCOffsetTailLine, 360,
                                                             layoutRect2.width(), layoutRect2.height());
                    qDebug() << "best nest type: " << nestType;
                    if(nestType == NoNestType){
                        qDebug() << "can not pack";
                        break;
                    }
                    BestNestType bestNestType(pieceType, nestType, alphaTailLine, xStepTailLine,
                                              pOffsetTailLine, rCOffsetTailLine);  // 获取该类型零件在限制材料高度时最佳排版方式

                    // 尝试该零件是否能排下
                    int minIndexTmp = nestPieceIndexRangeMap[type].minIndex;  // 获取该类型零件的最小值
                    int maxIndexTmp = nestPieceIndexRangeMap[type].maxIndex;  // 获取该类型零件的最大值
                    int step = nestPieceIndexRangeMap[type].step;   // 获取该零件的步长
                    int chooseIndex = -1;  // 选择未排版的那个零件进行排放
                    for(int k = minIndexTmp; k <= maxIndexTmp; k += step){
                        if(!nestPieceList[k].nested){
                            chooseIndex = k;
                            break;
                        }
                    }
                    if(chooseIndex == -1){  // 如果该类型所有零件都已排放，则继续寻找下一同型体
                        continue;
                    }

                    PairPieceStatus status;
                    bool sheetAvailable = true;
                    QRectF layoutRect1;
                    emit nestDebugRemainRect(sheetID, layoutRect2);

                    bool res = packPieceByLayoutRect(sheetID, layoutRect2, type, chooseIndex,
                                                     pieceMaxIndex, bestNestType, status, anchorPos,
                                                     spaceDelta, sheetAvailable, sameRowPieceList,
                                                     nestedList, unnestedList, FirstRow,
                                                     layoutRect1, layoutRect2);
                    if(res){
                        qDebug() << "tail line mixing successful!" << endl;
                        break;
                    } else{
                        qDebug() << "tail line mixing failed!" << endl;
                    }
                }
            }

            // 材料不足并需要返回
            sheetAvailable = false;
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

        qDebug() << "nested *" << pieceIndex;
        qreal xCoverTmp = pos.rx() + 0.5 * pieceWidth;  // 已排宽度
        qreal yCoverTmp = pos.ry() + 0.5 * pieceHeight;  // 已排高度
        if(xCover < xCoverTmp){
            xCover = xCoverTmp;  // 已排宽度
        }
        if(yCover < yCoverTmp){
            yCover = yCoverTmp;  // 已排高度
        }
        anchorPos = QPointF(xCover, yCover);  // 记录排放完该零件的右下角位置，即锚点
        layoutRect1 = QRectF(xCover, yMin,
                             xMax - xCover, yCover - yMin);
        layoutRect2 = QRectF(xMin, yCover,
                             layoutRect.width(), yMax - yCover);
        columnCounter++;  // 计数器
        pieceIndex++;  // 更新下一零件index
    }

    // 如果所有材料都已排完，同行零件列表不为空，则默认按上一行的间隔进行自适应
    if((adaptiveSpacingTypes & HorizontalAdaptiveSpacing) !=  NoAdaptiveSpacing && !sameRowPieceList.isEmpty()){
        for(int i=0; i<sameRowPieceList.length(); i++){
            int id = sameRowPieceList.at(i);
            nestPieceList[id].position += QPointF(spaceDelta, 0) * i;  // 记录下一零件排放位置
        }
    }

    // 更新未排列表，在未排列表中删除已排列表
    foreach (int index, nestedList) {
        //emit nestPieceUpdate(nestPieceList[index]);
        nestedPieceIndexlist.append(index);  // 更新已排列表
        nestSheetPieceMap[sheetID].append(index);  // 更新材料-零件索引
        unnestedList.removeOne(index);  // 更新未排列表
    }
    return packRes;
}

bool ContinueNestEngine::packPieceByReferenceLine(const int sheetID,
                                                  const QRectF &layoutRect,
                                                  int pieceType,
                                                  int pieceIndex,
                                                  const int pieceMaxIndex,
                                                  NestEngine::PairPieceStatus &status,
                                                  qreal &spaceDelta,
                                                  bool &sheetAvailable,
                                                  QVector<int> &sameRowPieceList,
                                                  QList<int> &nestedList,
                                                  QList<int> &unnestedList,
                                                  QRectF &layoutRect1,
                                                  int &packFlag)
{
    // 计算初始化状态
    qreal pieceWidth1, pieceHeight1, pieceWidth2, pieceHeight2, alpha1, alpha2, xStep;
    QPointF pos1, pos2;

    // 获取边界信息
    qreal xMax = layoutRect.right();  // 获取矩形宽度最大值的x坐标
    qreal rightDis = layoutRect.width();  // 记录矩形横向剩余距离
    qreal xCover = layoutRect.left();  // 记录零件所占矩形的宽度
    int columnCounter = 0;  // 记录之前的列计数器
    int lastPieceIndex = -1;  // 记录前一个零件的id
    int testTime = 0;  // 尝试次数
    while(pieceIndex <= pieceMaxIndex){
        if(nestPieceList[pieceIndex].nested){
            qDebug() << pieceIndex << "has nested";
            pieceIndex += 1;
            columnCounter = 0;
            testTime = 0;
            continue;
        }
        qDebug() << "tring $ " << pieceIndex;
        pieceType = nestPieceList[pieceIndex].typeID;
        packFlag = -packFlag;  // 改变状态
        if(columnCounter <= 1){
            if(testTime > 1){
                sheetAvailable = false;
                return false;
            }
            Piece piece = pieceList[pieceType];
            SinglePieceStatus singlePieceStatus = initSinglePieceStatus(layoutRect1, piece, packFlag);

            if(columnCounter == 0){
                // 如果该材料已排了零件，则向左靠接
                qreal xOffset = 0.0f;
                if(layoutRect1.width() < sheetList[sheetID].layoutRect().width()){
                    bool flag = packFlag == 1 ? true : false;
                    piece.moveTo(singlePieceStatus.pos);
                    piece.rotateByReferenceLine(piece.getPosition(), flag);

                    // 与其他零件不碰撞
                    xOffset = compactOnHD(sheetID, piece);
                }
                pos1 = status.pos1 = singlePieceStatus.pos - QPointF(xOffset, 0);
                alpha1 = status.alpha1 = singlePieceStatus.alpha;
                pieceWidth1 = status.pieceWidth1 = singlePieceStatus.pieceWidth;
                pieceHeight1 = status.pieceHeight1 = singlePieceStatus.pieceHeight;

                // 判断是否超出矩形右边界
                if(pos1.rx() + 0.5 * pieceWidth1 > xMax){
                    testTime++;
                    continue;
                }
                lastPieceIndex = pieceIndex;
            }
            if(columnCounter == 1){
                if(lastPieceIndex == -1){
                    qDebug() << "lastPieceIndex == -1";
                    return false;
                }
                // 首先进行靠接
                bool flag = -packFlag == 1 ? true : false;
                int pieceTypeLast = nestPieceList[lastPieceIndex].typeID;
                Piece pieceLast = pieceList[pieceTypeLast];
                pieceLast.moveTo(pos1);
                pieceLast.rotateByReferenceLine(pieceLast.getPosition(), flag);

                piece.moveTo(singlePieceStatus.pos);
                piece.rotateByReferenceLine(piece.getPosition(), !flag);

                // 向前一个零件靠接
                Piece pieceTmp = piece;
                qreal xOffset = pieceTmp.compactToOnHD(pieceLast, compactStep, compactAccuracy);
                QPointF pos2Temp = singlePieceStatus.pos - QPointF(xOffset, 0);
                QPointF pos1Temp = pos1;
                // 检测与其他零件是否碰撞
                if(collidesWithOtherPieces(sheetID, pieceTmp)){
                    QPointF offset = pos2Temp - pos1;
                    qreal xOffset1 = compactOnHD(sheetID, piece);
                    pos2Temp = singlePieceStatus.pos - QPointF(xOffset1, 0);
                    pos1Temp.setX(pos2Temp.rx() - offset.rx());
                    qDebug() << "changed position";
                }

                pos2 = status.pos2 = pos2Temp;
                alpha2 = status.alpha2 = singlePieceStatus.alpha;
                pieceWidth2 = status.pieceWidth2 = singlePieceStatus.pieceWidth;
                pieceHeight2 = status.pieceHeight2 = singlePieceStatus.pieceHeight;

                // 如果超出右边界，表示不能排放此零件
                if(pos2.rx() + 0.5 * pieceWidth2 > xMax){
                    sheetAvailable = false;
                    return false;
                }
                //  更改前一零件的位置
                pos1 = pos1Temp;
                nestPieceList[lastPieceIndex].position = pos1;

                // 计算步距
                qreal d1 = calVerToOppSideXDis(pieceLast.getPointsList());  // 计算各顶点到对边距离的最大值
                // 计算各顶点到错开零件各边最大值与min值的差
                qreal d2 = cal2PolygonMaxMinDiff(pieceLast.getPointsList(), piece.getPointsList());
                xStep = status.xStep = d1 > d2 ? d1 : d2;  // 取二者最大值
                QRectF boundRect1 = pieceLast.getBoundingRect();
                QRectF boundRect2 = piece.getBoundingRect();
                QRectF pairBoundRect = getPairBoundingRect(boundRect1, boundRect2);
                status.pairCenter = pairBoundRect.center();
                status.pairWidth = pairBoundRect.width();
                status.pairHeight = pairBoundRect.height();
            }
        }

        qDebug() << "nesting: #" << pieceIndex;
        QPointF pos = (columnCounter % 2 == 0 ? pos1 : pos2)
                + QPointF(xStep * int(columnCounter/2), 0);
        qreal alpha = columnCounter % 2 == 0 ? alpha1 : alpha2;
        qreal pieceWidth = columnCounter % 2 == 0 ? pieceWidth1 : pieceWidth2;
        qreal pieceHeight = columnCounter % 2 == 0 ? pieceHeight1 : pieceHeight2;
        qreal x = pos.rx() + 0.5 * pieceWidth;  // 已排宽度

        if(x > xMax){
            qDebug() << "over x";
            // 如果矩形类型不为尾只 且 设置了尾只优化
            if((mixingTyes & NestEngine::TailPieceMixing) != NestEngine::NoMixing){
                // 在同型体内寻找较小
                qDebug() << "searching the sameTypePiece ... " << endl;
                // 判断是否设置了同型体列表
                qDebug() << "searching #" << pieceType << "'s sameType";

                if(sameTypeNestPieceIndexMap.contains(pieceType)){
                    foreach (int type, sameTypeNestPieceIndexMap[pieceType]) {  // 遍历该列表
                        qDebug() << "type: " << type;
                        // 尝试该零件是否能排下
                        int minIndexTmp = nestPieceIndexRangeMap[type].minIndex;  // 获取该类型零件的最小值
                        int maxIndexTmp = nestPieceIndexRangeMap[type].maxIndex;  // 获取该类型零件的最大值
                        int step = nestPieceIndexRangeMap[type].step;   // 获取该零件的步长
                        int maxIndexReal = (maxIndexTmp - minIndexTmp) % step == 0 ? maxIndexTmp : maxIndexTmp - 1;
                        int chooseIndex = -1;  // 选择未排版的那个零件进行排放
                        for(int k = maxIndexReal; k >= minIndexTmp; k -= step){
                            if(!nestPieceList[k].nested && k != pieceIndex){
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
                        // 尾只排放，递归调用
                        qDebug() << "pack tail piece";
                        PairPieceStatus status;
                        bool sheetAvailable = true;
                        bool res = packPieceByReferenceLine(sheetID, layoutRect1, type, chooseIndex, maxIndexTmp,
                                                            status, spaceDelta, sheetAvailable,
                                                            sameRowPieceList, nestedList, unnestedList,
                                                            layoutRect1, packFlag);
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
                rightDis = xMax - layoutRect1.left();
                qDebug() << "xMax: " << xMax << ", width: " << xCover << "right distance: " << rightDis;
                spaceDelta = rightDis / (sameRowPieceList.length()-1);
                // 每一零件的偏移量
                for(int i=0; i<sameRowPieceList.length(); i++){
                    int id = sameRowPieceList[i];
                    //qDebug() << "id: " << id;
                    nestPieceList[id].position += QPointF(spaceDelta, 0) * i;  // 记录下一零件排放位置
                }
            }

            // 换行，同样递归调用本身
            columnCounter = 0;  // 列计数器清零
            spaceDelta = 0.0f;
            sameRowPieceList.clear();

            sheetAvailable = false;
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

        qDebug() << "nested: *" << pieceIndex;

        xCover = pos.rx() + 0.5 * pieceWidth;  // 已排宽度
        layoutRect1 = QRectF(xCover, layoutRect.top(),
                             layoutRect.right() - xCover, layoutRect.height());
        columnCounter++;  // 计数器
        pieceIndex++;
    }

    emit nestDebugRemainRect(sheetID, layoutRect1);

    // 如果所有材料都已排完，同行零件列表不为空，则默认按上一行的间隔进行自适应
    if((adaptiveSpacingTypes & HorizontalAdaptiveSpacing) !=  NoAdaptiveSpacing && !sameRowPieceList.isEmpty()){
        for(int i=0; i<sameRowPieceList.length(); i++){
            int id = sameRowPieceList.at(i);
            nestPieceList[id].position += QPointF(spaceDelta, 0) * i;  // 记录下一零件排放位置
        }
    }

    // 更新未排列表，在未排列表中删除已排列表
    foreach (int index, nestedList) {
        // emit nestPieceUpdate(nestPieceList[index]);
        nestedPieceIndexlist.append(index);  // 更新已排列表
        nestSheetPieceMap[sheetID].append(index);  // 更新材料-零件索引
        unnestedList.removeOne(index);  // 更新未排列表
    }
    return true;
}

bool ContinueNestEngine::packTailPiece(const int sheetID,
                                       const QRectF &layoutRect,
                                       int pieceType,
                                       int pieceIndex,
                                       PairPieceStatus &status,
                                       qreal &spaceDelta,
                                       bool &sheetAvailable,
                                       QVector<int> &sameRowPieceList,
                                       QList<int> &nestedList,
                                       QList<int> &unnestedList,
                                       QRectF &layoutRect1,
                                       int &packFlag,
                                       bool isStripSheet)
{
    // 在同型体内寻找较小
    qDebug() << "searching the sameTypePiece ... " << endl;
    // 判断是否设置了同型体列表
    qDebug() << "searching #" << pieceType << "'s sameType";

    if(!isStripSheet){  // for whole or package

    } else{
        if(sameTypeNestPieceIndexMap.contains(pieceType)){
            foreach (int type, sameTypeNestPieceIndexMap[pieceType]) {  // 遍历该列表
                qDebug() << "type: " << type;
                // 尝试该零件是否能排下
                int minIndexTmp = nestPieceIndexRangeMap[type].minIndex;  // 获取该类型零件的最小值
                int maxIndexTmp = nestPieceIndexRangeMap[type].maxIndex;  // 获取该类型零件的最大值
                int step = nestPieceIndexRangeMap[type].step;   // 获取该零件的步长
                int maxIndexReal = (maxIndexTmp - minIndexTmp) % step == 0 ? maxIndexTmp : maxIndexTmp - 1;
                int chooseIndex = -1;  // 选择未排版的那个零件进行排放
                for(int k = maxIndexReal; k >= minIndexTmp; k -= step){
                    if(!nestPieceList[k].nested){
                        chooseIndex = k;
                        break;
                    }
                }
                if(chooseIndex == -1){  // 如果该类型所有零件都已排放，则继续寻找下一同型体
                    continue;
                }

                if(chooseIndex == pieceIndex){
                    qDebug() << "chooseIndex == pieceIndex";
                    break;
                }

                /*
                 * 运行到这里代表已经找到了满足条件的同型体零件
                 * 尝试排放该零件，这里递归调用本身
                 * 否则，继续尝试排放下一个同型体.
                 *
                 * 计算出前一个零件与剩余材料构成的矩形，
                 * 然后在这个小的范围内尝试排放
                 */
                // 尾只排放，递归调用
                bool res = packPieceByReferenceLine(sheetID, layoutRect, type, chooseIndex, maxIndexTmp,
                                                    status, spaceDelta, sheetAvailable,
                                                    sameRowPieceList, nestedList, unnestedList,
                                                    layoutRect1, packFlag);
                if(res){
                    qDebug() << "tail piece mixing successful!" << endl;
                    return true;
                } else{
                    qDebug() << "tail piece mixing failed!" << endl;
                }
            }
        }
    }

    return false;
}

void ContinueNestEngine::packPieces(QVector<int> indexList)
{
    // 零件排放未结束标志
    bool interruptFlag = false;

    // 将向量转化为列表，因为后面会经常删除元素
    QList<int> unnestedList = indexList.toList();

    // 排版，按照材料进行排版，排满一张后再排下一张
    int sheetID = 0;

    if(!isStripSheet){  // 整体/卷状排版
        // 初始排版矩形区域
        QRectF layoutRect = sheetList[sheetID].layoutRect();  // 排版区域
        QVector<QRectF> layoutRects;  // 获取上插板矩形区域
        layoutRects.append(layoutRect);

        PairPieceStatus status;  // 组合零件状态
        RectTypes rectType = FirstRow;  // 初始矩形类型
        bool wholeSheet = true;  // 整张材料标志

        // 排放零件直到所有零件都排放完
        while(!interruptFlag && !unnestedList.isEmpty()){
            // 初始化状态信息，用来记录排版过程中的中间数据，或一些标志位
            if(!nestSheetPieceMap.contains(sheetID)){  // 初始化材料-零件索引
                QVector<int> pieceList;
                nestSheetPieceMap.insert(sheetID, pieceList);
            }
            // 遍历所有排版矩形
            foreach (QRectF layoutRect, layoutRects) {
                QRectF layoutRect1, layoutRect2;  // 同一行剩余矩形 下一行剩余矩形

                if(unnestedList.isEmpty()){  // 如果待排列表为空，则提前结束
                    qDebug() << "待排列表为空";
                    break;
                }

                // 确定初始状态
                int index = unnestedList.first();  // 获取未排零件的第一个零件
                int type = nestPieceList[index].typeID;  // 获取该零件的类型号
                BestNestType bestNestType = pieceBestNestTypeMap[type];  // 获取该类型零件的最佳排版方式

                bool sheetAvailable = true;  // 记录材料是否可用
                qreal spaceDelta = 0.0f;  // 自适应间隔增量
                QPointF anchorPos;  // 记录锚点位置，记录的是零件排放后外包矩形右下角位置
                QVector<int> sameRowPieceList;  // 记录同一行零件列表
                QList<int> nestedList;  // 记录已排零件
                int maxIndex = nestPieceIndexRangeMap[type].maxIndex;  // 获取该类型零件的最大值

                bool res = packPieceByLayoutRect(sheetID, layoutRect, type, index,
                                                 maxIndex, bestNestType,
                                                 status, anchorPos, spaceDelta,
                                                 sheetAvailable, sameRowPieceList,
                                                 nestedList, unnestedList, rectType,
                                                 layoutRect1, layoutRect2);
                if(!res){
                    // 该矩形内未能排放任何一个零件，则需要检查同行零件列表是否为空；
                    // 如果同行零件列表不为空，则需要清空该列表
                    qDebug() << "pack pieces failed";
                    sheetAvailable = false;

                    emit nestDebugRemainRect(sheetID, layoutRect1);
                    emit nestDebugRemainRect(sheetID, layoutRect2);
                }
                if(!wholeSheet){  // 如果不是整张材料，则无需增加材料，只需切换至下一个矩形
                    qDebug() << "is not whole sheet, continue";
                    wholeSheet = true;
                    continue;
                }
                if(!sheetAvailable){  // 如果材料不可用，则增加材料
                    qDebug() << "sheet #" << sheetID << " has finished" << endl;
                    int sheetLength = sheetList.length();  // 获取材料列表长度
                    if(sheetLength <= sheetID+1){  // 如果材料不足
                        if(autoRepeatLastSheet){  // 如果设置了自动重复最后一张材料
                            Sheet sheet = sheetList[sheetID];
                            layoutRect = sheet.layoutRect();
                            sheetList.append(sheet);
                            initQuadTreeMap(sheetID+1);  // 初始化该张材料的四叉树
                            sheetID++;
                            emit autoRepeatedLastSheet(sheet);  // 排版结束后发送 重复了最后一张材料

                            // 更新排版矩形列表
                            layoutRects.clear();
                            layoutRects.append(layoutRect);
                        } else{
                            int num = unnestedList.length();
                            emit nestInterrupted(num);
                            interruptFlag = true;  // 设置中断标志为true
                        }
                    } else{  // 切换至下一材料
                        layoutRect = sheetList[sheetID++].layoutRect();
                        layoutRects.clear();
                        layoutRects.append(layoutRect);
                    }
                    rectType = FirstRow;  // 更新矩形类型
                } else {  // 如果材料仍可用，则先在同行内剩余材料排放
                    qDebug() << "sheet is still available";
                    layoutRects.clear();
                    layoutRects.append(layoutRect1);
                    layoutRects.append(layoutRect2);
                    wholeSheet = false;

                    emit nestDebugRemainRect(sheetID, layoutRect1);
                    emit nestDebugRemainRect(sheetID, layoutRect2);
                }
            }
        }
    } else{  // 条形板排版
        qDebug() << "条形板排版";
        QVector<QRectF> layoutRects = sheetList[sheetID].inforcementRects();  // 获取上插板矩形区域
        PairPieceStatus status;  // 组合零件状态
        QVector<int> sameRowPieceList;  // 记录同一行零件列表
        qreal spaceDelta = 0.0f;  // 自适应间隔增量
        while(!interruptFlag && !unnestedList.isEmpty()){
            // 初始化状态信息，用来记录排版过程中的中间数据，或一些标志位
            if(!nestSheetPieceMap.contains(sheetID)){  // 初始化材料-零件索引
                QVector<int> pieceList;
                nestSheetPieceMap.insert(sheetID, pieceList);
            }

            int layoutRectCounter = 0;
            foreach (QRectF layoutRect, layoutRects) {
                if(unnestedList.isEmpty()){  // 如果待排列表为空，则提前结束
                    qDebug() << "待排列表为空";
                    break;
                }
                // 排放同一类型零件
                int pieceIndex  = unnestedList.first();  // 获取零件索引
                int pieceType = nestPieceList[pieceIndex].typeID;  // 获取零件类型
                int pieceMaxIndex = nestPieceIndexRangeMap[pieceType].maxIndex;  // 获取零件最大索引

                int packFlag = 0;
                bool sheetAvailable = true;  // 记录材料是否可用
                QList<int> nestedList;  // 记录已排零件
                QRectF layoutRect1 = layoutRect;

                bool res = packPieceByReferenceLine(sheetID, layoutRect, pieceType, pieceIndex, pieceMaxIndex,
                                                    status, spaceDelta,
                                                    sheetAvailable, sameRowPieceList, nestedList, unnestedList,
                                                    layoutRect1, packFlag);
                if(!res){
                    // 该矩形内未能排放任何一个零件，则需要检查同行零件列表是否为空；
                    // 如果同行零件列表不为空，则需要清空该列表
                    if(!sameRowPieceList.isEmpty()){
                        qDebug() << "go here";
                        if((adaptiveSpacingTypes & HorizontalAdaptiveSpacing) !=  NoAdaptiveSpacing){
                            spaceDelta = layoutRect.width() / (sameRowPieceList.length() - 1);
                            for(int i=0; i<sameRowPieceList.length(); i++){
                                int id = sameRowPieceList.at(i);
                                nestPieceList[id].position += QPointF(spaceDelta, 0) * i;  // 记录下一零件排放位置
                            }
                            spaceDelta = 0.0f;
                        }
                        // 清空同行零件列表
                        sameRowPieceList.clear();
                    } else{
                        qDebug() << "pack piece failed!";
                        emit nestInterrupted(unnestedList.length());
                    }
                }
                if(!sheetAvailable){
                    layoutRectCounter++;
                    if(layoutRectCounter == layoutRects.length()){  // 如果材料不可用，则增加材料
                        layoutRectCounter = 0;
                        qDebug() << "sheet #" << sheetID << " has finished" << endl;
                        int sheetLength = sheetList.length();  // 获取材料列表长度
                        if(sheetLength <= sheetID+1){  // 如果材料不足
                            if(autoRepeatLastSheet){  // 如果设置了自动重复最后一张材料
                                Sheet sheet = sheetList[sheetID];
                                sheetList.append(sheet);
                                initQuadTreeMap(sheetID+1);  // 初始化该张材料的四叉树
                                sheetID++;
                                emit autoRepeatedLastSheet(sheet);  // 排版结束后发送 重复了最后一张材料
                                // 更新排版矩形列表
                                layoutRects = sheet.inforcementRects();
                            } else{
                                int num = unnestedList.length();
                                emit nestInterrupted(num);
                                interruptFlag = true;  // 设置中断标志为true
                            }
                        } else{
                            layoutRects = sheetList[sheetID++].inforcementRects();
                        }
                    } else{
                        qDebug() << "next layoutRect";
                        layoutRect = layoutRects[layoutRectCounter];
                    }
                } else{
                    qDebug() << "sheet is still available";
                    qDebug() << "layoutRectCounter: " << layoutRectCounter;
                    QVector<QRectF> rects;
                    rects.append(layoutRect1);
                    for(int i = layoutRectCounter+1; i < layoutRects.length(); i++){
                        rects.append(layoutRects[i]);
                    }
                    layoutRects = rects;
                    break;
                }
            }
        }
    }

    // 发送排放结束信号
    emit(nestFinished(nestPieceList));
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

qreal ContinueNestEngine::compactOnHD(int sheetID, Piece piece)
{
    //qDebug() << "水平方向靠接算法";
    QPointF pos = piece.getPosition();
    QPointF posOld = pos;  // 记录之前的位置
    //qDebug() << "origin position: " << pos;
    // 重力方向靠接
    qreal stepX = compactStep;
    while(stepX > compactAccuracy){
        QPointF posTemp = pos;
        QPointF forwardPos(posTemp.rx()-stepX, posTemp.ry());  // 重力反方向移动
        //qDebug() << "move to" << forwardPos;
        piece.moveTo(forwardPos);  // 将零件移至前进位置
        if(piece.getBoundingRect().left() < sheetList[sheetID].layoutRect().left()){
            //qDebug() << "超出材料范围";
            stepX /= 2;
            continue;
        }
        if(collidesWithOtherPieces(sheetID, piece)){
            // 往下移
            ///qDebug() << "collides with others";
            stepX /= 2;
            continue;
        }
        pos.setX(pos.rx() - stepX);
    }
    //qDebug() << "now position: " << pos;
    piece.moveTo(pos);
    qreal step = posOld.rx() - pos.rx();
    return step;
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
    std::list<Object *> resObjects =
            quadTreeMap[sheetID]->retrieve(new Object(piece.getBoundingRect()));
    for(auto &t:resObjects){
        int id = t->id;
        int typeID = nestPieceList[id].typeID;
        Piece pieceNested = pieceList[typeID];
        pieceNested.moveTo(nestPieceList[id].position);
        if(!isStripSheet){
            pieceNested.rotate(nestPieceList[id].position, nestPieceList[id].alpha);
        } else{
            bool flag = nestPieceList[id].alpha == 0 ? true : false;
            pieceNested.rotateByReferenceLine(nestPieceList[id].position, flag);
        }
        if(pieceNested.collidesWithPiece(piece)){
            return true;
        }
        collisionCount++;
    }
    return false;
}

#include "nestengine.h"

#include <QDebug>

NestEngine::NestEngine() :
    autoRepeatLastSheet(false),
    compactStep(1),
    compactAccuracy(0.1),
    nestType(SigleRow),
    mixingTyes(NoMixing),
    adaptiveSpacingTypes(NoAdaptiveSpacing),
    orientations(AllOrientationNest),
    nestEngineStrategys(NoStrategy),
    oneKnifeCut(false),
    cutStep(0),
    rotatable(false),
    maxRotateAngle(0),
    minHeightOpt(false),
    collisionCount(0)
{

}

NestEngine::NestEngine(const QVector<Piece> pieceList, QVector<Sheet> sheetList) :
    autoRepeatLastSheet(false),
    compactStep(1),
    compactAccuracy(0.1),
    nestType(SigleRow),
    mixingTyes(NoMixing),
    adaptiveSpacingTypes(NoAdaptiveSpacing),
    orientations(AllOrientationNest),
    nestEngineStrategys(NoStrategy),
    oneKnifeCut(false),
    cutStep(0),
    rotatable(false),
    maxRotateAngle(0),
    minHeightOpt(false),
    collisionCount(0)
{
    this->pieceList = pieceList;
    this->sheetList = sheetList;

    // 初始化每张材料的四叉树
    for(int i=0; i<sheetList.length(); i++){
        initQuadTreeMap(i);
    }
}

NestEngine::NestEngine(const QVector<Piece> pieceList, QVector<Sheet> sheetList, QVector<NestEngine::SameTypePiece> sameTypePieceList) :
    NestEngine(pieceList, sheetList)
{
    this->sameTypePieceList = sameTypePieceList;
}

NestEngine::~NestEngine()
{
    pieceList.clear();
}

void NestEngine::setAutoRepeatLastSheet(bool flag)
{
    autoRepeatLastSheet = flag;
}

bool NestEngine::getAutoRepeatLastSheet()
{
    return autoRepeatLastSheet;
}

void NestEngine::setCompactStep(qreal step)
{
    compactStep = step;
}

qreal NestEngine::getCompactStep()
{
    return compactStep;
}

void NestEngine::setCompactAccuracy(qreal accuracy)
{
    compactAccuracy = accuracy;
}

qreal NestEngine::getCompactAccuracy()
{
    return compactAccuracy;
}

void NestEngine::setNestEngineType(NestEngine::NestEngineType type)
{
    nestEngineType = type;
}

NestEngine::NestEngineType NestEngine::getNestEngineType()
{
    return nestEngineType;
}

void NestEngine::setNestType(NestEngine::NestType type)
{
    nestType = type;
}

NestEngine::NestType NestEngine::getNestType()
{
    return nestType;
}

void NestEngine::setNestMixingTypes(NestMixingTypes types)
{
    mixingTyes = types;
}

NestEngine::NestMixingTypes NestEngine::getNestMixingTypes()
{
    return mixingTyes;
}

void NestEngine::setNestAdaptiveSpacingTypes(NestAdaptiveSpacingTypes types)
{
    adaptiveSpacingTypes = types;
}

NestEngine::NestAdaptiveSpacingTypes NestEngine::getNestAdaptiveSpacingTypes()
{
    return adaptiveSpacingTypes;
}

void NestEngine::setNestOrientations(NestEngine::NestOrientations oriens)
{
    orientations = oriens;
}

NestEngine::NestOrientations NestEngine::getNestOrientations()
{
    return orientations;
}

void NestEngine::setNestEngineStrategys(NestEngineStrategys strategys)
{
    nestEngineStrategys = strategys;
}

NestEngine::NestEngineStrategys NestEngine::getNestEngineStrategys()
{
    return nestEngineStrategys;
}

void NestEngine::setCutStep(qreal step)
{
    cutStep = step;
    if(cutStep != 0){
        oneKnifeCut = true;
    }
}

qreal NestEngine::getCutStep()
{
    return cutStep;
}

void NestEngine::setMaxRotateAngle(int angle)
{
    maxRotateAngle = angle;
    if(maxRotateAngle != 0){
        rotatable = true;
    }
}

qreal NestEngine::getMaxRotateAngle()
{
    return maxRotateAngle;
}

void NestEngine::setMinHeightOpt(bool flag)
{
    minHeightOpt = flag;
}

bool NestEngine::getMinHeightOpt()
{
    return minHeightOpt;
}

QVector<Piece> NestEngine::getSortedPieceListByArea(QVector<Piece> pieceList, QMap<int, int> &transformMap)
{
    // QMap 默认按key值升序排列
    QMap<qreal, QVector<int>> pieceAreaMap;
    for(int i=0; i<pieceList.length(); i++){
        Piece piece = pieceList[i];
        qreal area = piece.getArea();
        if(!pieceAreaMap.contains(area)){
            QVector<int> list;
            pieceAreaMap.insert(-area, list);
        }
        pieceAreaMap[-area].append(i);
    }

    // 获取排序后的零件列表
    QVector<Piece> pieceListRet;

    int count = 0;
    transformMap.clear();
    QMap<qreal, QVector<int>>::const_iterator i;
    for(i=pieceAreaMap.constBegin(); i!=pieceAreaMap.constEnd(); ++i){
        foreach (int index, i.value()) {
            pieceListRet.append(pieceList[index]);
            transformMap.insert(index, count++);  // 记录变换前后的位置映射关系，变换前为index，变换后为count++
        }
    }
    return pieceListRet;
}

void NestEngine::initQuadTreeMap(int sheetID)
{
    QRectF rect = sheetList[sheetID].layoutRect();
    QuadTreeNode<Object> *quadTree =
            new QuadTreeNode<Object>(rect.x(), rect.y(), rect.width(), rect.height(), 1, 5, 10, ROOT, NULL);
    if(!quadTreeMap.contains(sheetID)){
        quadTreeMap.insert(sheetID, quadTree);
    }
}

void NestEngine::initNestPieceList()
{
    // 首先将零件按规定方向进行旋转
    for(int i=0; i<pieceList.length(); i++){
        Piece &piece = pieceList[i];
        bool pieceHorizontal = piece.isHorizontal();
        if(pieceHorizontal && orientations == NestEngine::VerticalNest){  // 如果零件是横置的
            piece.rotate(piece.getMinBoundingRect().center(), 90);
        }
        if(!pieceHorizontal && orientations == NestEngine::HorizontalNest){  // 如果零件是横置的
            piece.rotate(piece.getMinBoundingRect().center(), 90);
        }
    }

    // 根据排版策略初始化排版零件
    int count=0;
    switch (nestEngineStrategys){
    case LeftRightTurn: {
        for(int i=0; i<pieceList.length(); i++){
            Piece piece = pieceList[i];
            PieceIndexRange indexRange(i, count, count+piece.getCount()-1);
            nestPieceIndexRangeMap.insert(i, indexRange);  // 初始化排版零件序号范围
            for(int j=0; j<piece.getCount(); j++){
                // 左右交替，即两个图形相差180*
                NestPiece nestPiece(count++, i);
                nestPiece.alpha = j % 2 == 0 ? 0 : 180;
                nestPieceList.append(nestPiece);  // 初始化排版零件列表
            }
            pieceMaxPackPointMap.insert(i, 0);  // 初始化零件排样点最大值map
        }
        break;
    }
    case SizeDown: {
        QMap<int, int> transformMap;
        pieceList = getSortedPieceListByArea(pieceList, transformMap);
        int sameLen = sameTypePieceList.length();
        if(sameLen != 0){
            for(int i=0; i<sameLen; i++){
                QVector<int> &pieceIDList = sameTypePieceList[i].pieceIDList;
                for(int j=0; j<pieceIDList.length(); j++){
                    int oldID = pieceIDList[j];
                    int newID = transformMap[oldID];
                    pieceIDList[j] = newID;
                }
            }
        }
        for(int i=0; i<pieceList.length(); i++){
            Piece piece = pieceList[i];
            PieceIndexRange indexRange(i, count, count+piece.getCount()-1);
            nestPieceIndexRangeMap.insert(i, indexRange);  // 初始化排版零件序号范围
            for(int j=0; j<piece.getCount(); j++){
                nestPieceList.append(NestPiece(count++, i));
            }
            pieceMaxPackPointMap.insert(i, 0);  // 初始化零件排样点最大值map
        }
        break;
    }
    case AllStrategys: {
        QMap<int, int> transformMap;
        pieceList = getSortedPieceListByArea(pieceList, transformMap);
        int sameLen = sameTypePieceList.length();
        if(sameLen != 0){
            for(int i=0; i<sameLen; i++){
                QVector<int> &pieceIDList = sameTypePieceList[i].pieceIDList;
                for(int j=0; j<pieceIDList.length(); j++){
                    int oldID = pieceIDList[j];
                    int newID = transformMap[oldID];
                    pieceIDList[j] = newID;
                }
            }
        }
        for(int i=0; i<pieceList.length(); i++){
            Piece piece = pieceList[i];
            PieceIndexRange indexRange(i, count, count+piece.getCount()-1);
            nestPieceIndexRangeMap.insert(i, indexRange);  // 初始化排版零件序号范围
            for(int j=0; j<piece.getCount(); j++){
                // 左右交替，即两个图形相差180*
                NestPiece nestPiece(count++, i);
                nestPiece.alpha = j % 2 == 0 ? 0 : 180;
                nestPieceList.append(nestPiece);
            }
            pieceMaxPackPointMap.insert(i, 0);  // 初始化零件排样点最大值map
        }
        break;
    }
    default:
        break;
    }
}

void NestEngine::initsameTypeNestPieceIndexListMap()
{
    switch (mixingTyes) {
    case SameTypeSizeMixing:
        for(int i=0; i<sameTypePieceList.length(); i++){
            QVector<int> subNestPieceIDList;  // 用来保存子排版零件id列表
            SameTypePiece sameTypePiece = sameTypePieceList[i];  // 获取同型体列表
            //int typeID = sameTypePiece.typeID;   // 获取同型体编号
            QVector<int> pieceIDList = sameTypePiece.pieceIDList;  // 获取该同型体编号下的零件列表
            for(int j=0; j<pieceIDList.length(); j++){
                int id = pieceIDList[j];
                PieceIndexRange indexRange = nestPieceIndexRangeMap[id];
                int min = indexRange.minIndex;
                int max = indexRange.maxIndex;
                for(int index=min; index<=max; index++){
                    subNestPieceIDList.append(index);
                }
            }
            sameTypeNestPieceIndexListMap.insert(i, subNestPieceIDList);  // 保存入map
        }
        break;
    default:
        break;
    }
}

void NestEngine::packAlg()
{
    // 如果没有设置同型体，则直接对排样零件列表排样
    if(sameTypeNestPieceIndexListMap.size() < 1){
        QVector<int> indexList;
        for(int j=0; j<nestPieceList.length(); j++){
            indexList.append(nestPieceList[j].index);
        }
        packPieces(indexList);
        return;
    }

    for(int i=0; i<sameTypeNestPieceIndexListMap.size(); i++){
        // 获取编号为i的同型体零件列表
        QVector<int> sameTypeNestPieceList= sameTypeNestPieceIndexListMap[i];
        QVector<int> indexList;
        for(int j=0; j<sameTypeNestPieceList.length(); j++){
            indexList.append(sameTypeNestPieceList[j]);
        }
        packPieces(indexList);
    }
}

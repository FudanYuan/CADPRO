#include "nestengine.h"
#include "nestengineconfigure.h"
#include <QDebug>

NestEngine::NestEngine(QObject *parent) :
    QObject(parent),
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

NestEngine::NestEngine(QObject *parent, const QVector<Piece> pieceList, QVector<Sheet> sheetList) :
    QObject(parent),
    autoRepeatLastSheet(false),
    compactStep(5),
    compactAccuracy(1),
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

NestEngine::NestEngine(QObject *parent, const QVector<Piece> pieceList, QVector<Sheet> sheetList, QVector<NestEngine::SameTypePiece> sameTypePieceList) :
    NestEngine(parent, pieceList, sheetList)
{
    this->sameTypePieceList = sameTypePieceList;
}

NestEngine::~NestEngine()
{
    pieceList.clear();
    sheetList.clear();
    sameTypePieceList.clear();
    nestPieceList.clear();
    nestedPieceIndexlist.clear();
    sameTypeNestPieceIndexListMap.clear();
    nestedPieceIndexlist.clear();
    unnestedPieceIndexlist.clear();
    nestSheetPieceMap.clear();
    pieceMaxPackPointMap.clear();
    quadTreeMap.clear();
}

void NestEngine::setPieceList(const QVector<Piece> &pieceList)
{
    this->pieceList = pieceList;
}

QVector<Piece> NestEngine::getPieceList()
{
    return pieceList;
}

void NestEngine::setSheetList(QVector<Sheet> &sheetList)
{
    this->sheetList = sheetList;
}

QVector<Sheet> NestEngine::getSheetList()
{
    return sheetList;
}

void NestEngine::setSameTypePieceList(QVector<NestEngine::SameTypePiece> sameTypePieceList)
{
    this->sameTypePieceList = sameTypePieceList;
}

QVector<NestEngine::SameTypePiece> NestEngine::getSameTypePieceList()
{
    return this->sameTypePieceList;
}

QVector<NestEngine::NestPiece> NestEngine::getNestPieceList()
{
    return nestPieceList;
}

QVector<int> NestEngine::getNestedPieceIndexlist()
{
    return nestedPieceIndexlist;
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
        if(!pieceAreaMap.contains(-area)){
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
    case ReferenceLine:{  // 参考线一定是左右交替
        // 该策略下，需要根据参考线的方向指定旋转方向，
        // 如果排版线列号为奇数，就是顺时针；
        // 如果排版线列号为偶数，就是逆时针；
        for(int i=0; i<sheetList.length(); i++){
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

void NestEngine::initNestEngineConfig(Sheet::SheetType sheetType, NestEngineConfigure *proConfig)
{
    switch (sheetType) {  // 根据材料类型，初始化排版引擎
    case (Sheet::Whole):{
        NestEngineConfigure::WholeSheetNest wholeSheetNest = proConfig->getWholeSheetNest();
        setMaxRotateAngle(wholeSheetNest.wholedegree);  // 摆动最大角度
        setNestEngineStrategys(NestEngine::SizeDown);  // 设置排版策略
        setNestOrientations(wholeSheetNest.wholeorientation);  // 设置排版方向
        setNestMixingTypes(wholeSheetNest.wholemixing);  // 排版混合方式
        qDebug() <<"whole";
        qDebug() << "混合方式"<<wholeSheetNest.wholemixing;
        qDebug() <<"排版"<<wholeSheetNest.wholeorientation;
        qDebug() <<"旋转角度"<<wholeSheetNest.wholedegree;
        break;
    }
    case (Sheet::Strip):{
        NestEngineConfigure::StripSheetNest stripSheetNest = proConfig->getStripSheetNest();
        setNestEngineStrategys(stripSheetNest.strategy);  // 排版策略
        setNestAdaptiveSpacingTypes(stripSheetNest.stripadaptive);  // 自适应间隔
        setNestMixingTypes(stripSheetNest.stripmixing);  // 排版混合方式
        qDebug() <<"strip";
        qDebug() <<"排版方式"<<stripSheetNest.strategy;
        qDebug() <<"适应方式"<<stripSheetNest.stripadaptive;
        qDebug() <<"混合方式"<<stripSheetNest.stripmixing;
        break;
    }
    case (Sheet::Package):{
        NestEngineConfigure::PackageSheetNest packageSheetNest = proConfig->getPackageSheetNest();
        setMaxRotateAngle(packageSheetNest.packagedegree);  // 摆动最大角度
        setNestEngineStrategys(NestEngine::SizeDown);  // 设置排版策略
        setNestOrientations(packageSheetNest.packageorientation);  // 设置排版方向
        setNestMixingTypes(packageSheetNest.packagemixing);  // 排版混合方式
        qDebug() <<"package";
        qDebug() << "混合方式"<<packageSheetNest.packagemixing;
        qDebug() <<"排版"<<packageSheetNest.packageorientation;
        qDebug() <<"旋转角度"<<packageSheetNest.packagedegree;
        break;
    }
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

void NestEngine::packPieces(QVector<int> indexList)
{
    Q_UNUSED(indexList);
}

bool NestEngine::packOnePiece(Piece piece, NestEngine::NestPiece &nestPiece)
{
    Q_UNUSED(piece);
    Q_UNUSED(nestPiece);
    return true;
}

bool NestEngine::packOnePieceOnSheet(Piece piece, int sheetID, NestEngine::NestPiece &nestPiece)
{
    Q_UNUSED(piece);
    Q_UNUSED(sheetID);
    Q_UNUSED(nestPiece);
    return true;
}

bool NestEngine::compact(int sheetID, NestEngine::NestPiece &nestPiece)
{
    Q_UNUSED(sheetID);
    Q_UNUSED(nestPiece);
    return true;
}

bool NestEngine::collidesWithOtherPieces(int sheetID, Piece piece)
{
    Q_UNUSED(piece);
    Q_UNUSED(sheetID);
    return true;
}

void NestEngine::onNestStart()
{
    initNestPieceList();  // 初始化排样零件
    packAlg();  // 进行排版
}

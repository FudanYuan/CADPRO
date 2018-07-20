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
    collisionCount(0),
    counter(0)
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
        if((pieceHorizontal && orientations == NestEngine::VerticalNest)
                ||(!pieceHorizontal && orientations == NestEngine::HorizontalNest)){
            piece.setTranspose(true);
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
                qreal transposeAngle = piece.isTranspose() ? 90 : 0;
                nestPiece.alpha = j % 2 == 0 ? 0 : 180 + transposeAngle;
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
                qreal transposeAngle = piece.isTranspose() ? 90 : 0;
                nestPieceList.append(NestPiece(count++, i, transposeAngle));
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
                qreal transposeAngle = piece.isTranspose() ? 90 : 0;
                nestPiece.alpha = j % 2 == 0 ? 0 : 180 + transposeAngle;
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

/**
 * @brief NestEngine::signalRowNest 最优单排计算函数
 * @param piece 零件
 * @param alpha 旋转角度
 * @param stepX 另一个零件与piece的水平方向偏移量
 * @param width 单排后整体包络矩形的宽度
 * @param height 单排后整体包络矩形的高度
 */
void NestEngine::singleRowNest(Piece piece, qreal &alpha, qreal &stepX, qreal &width, qreal &height)
{
    qreal minArea = LONG_MAX;  // 整体包络矩形的最小面积
    qreal pieceWidth = piece.getMinBoundingRect().width();  // 获取零件外包矩形宽度
    qreal pieceHeight = piece.getMinBoundingRect().height();  // 获取零件外包矩形高度
    qreal d = qSqrt(pieceWidth*pieceWidth + pieceHeight*pieceHeight);  // 获取外包矩形对角线距离
    d = qrealPrecision(d, PRECISION);
    for(int i=0; i<=180; i++){
        qDebug() << "i:  " << i;
        Piece piece1 = piece;
        QPointF pos1(0, 0);
        piece1.moveTo(pos1);  // 将零件移至原点
        piece1.rotate(pos1, i);  // 将零件绕中心点旋转

        Piece piece2 = piece;  // 构造影子零件
        QPointF pos2(d, 0);
        piece2.moveTo(pos2);  // 将影子零件移动至(d, 0)处,以保证在旋转过程中,两零件不会碰撞
        piece2.rotate(pos2, i+180);  // 将影子零件绕中心点旋转

        qreal h = piece1.getMinBoundingRect().height();  // 获取外包矩形的高度
        qreal w = piece1.getMinBoundingRect().width();  // 获取外包矩形的宽度
        piece2.compactToOnHD(piece1, compactStep, compactAccuracy);  // 水平靠接
        qreal step = piece2.getPosition().rx()-piece1.getPosition().rx();  // 返回靠接移动距离
        qreal area = h * (w+step);  // 获取整体外包矩形的面积
        if(area < minArea){
            minArea = qrealPrecision(area, PRECISION);  // 最小面积
            alpha = i;  // 旋转角度
            height = qrealPrecision(h, PRECISION);  // 整体外包矩形的高
            width = qrealPrecision(w+step, PRECISION);  // 整体外包举行的宽
            stepX = qrealPrecision(step, PRECISION); // 水平步距
        }
    }

    qDebug() << "最小面积: " << minArea << ", alpha: " << alpha << ", stepX: " << stepX;
}

/**
 * @brief NestEngine::doubleRowNest  双排排版
 * @param piece 零件
 * @param n 控制精度的正整数
 * @param alpha 旋转角度
 * @param stepX 最外侧零件水平方向偏移量
 * @param cOffset 中间零件中心偏移量
 * @param width  整体外包矩形宽度
 * @param height  整体外包矩形高度
 */
void NestEngine::doubleRowNest(Piece piece, const int n, qreal &alpha, qreal &stepX, QPointF &cOffset, qreal &width, qreal &height)
{
    qreal minArea = LONG_MAX;  // 整体包络矩形的最小面积
    qreal pieceWidth = piece.getMinBoundingRect().width();  // 获取零件外包矩形宽度
    qreal pieceHeight = piece.getMinBoundingRect().height();  // 获取零件外包矩形高度
    qreal d = qSqrt(pieceWidth*pieceWidth + pieceHeight*pieceHeight);  // 获取外包矩形对角线距离
    d = qrealPrecision(d, PRECISION);
    for(int i=0; i<=90; i++){
        qDebug() << "i:  " << i;
        Piece piece1 = piece;
        QPointF pos1(0, 0);
        piece1.moveTo(pos1);  // 将零件移至原点
        piece1.rotate(pos1, i);  // 将零件绕中心点旋转

        Piece piece2 = piece;  // 构造影子零件
        QPointF pos2(d, 0);
        piece2.moveTo(pos2);  // 将影子零件移动至(d, 0)处,以保证在旋转过程中,两零件不会碰撞
        piece2.rotate(pos2, i);  // 将影子零件绕中心点旋转

        Piece piece3 = piece;  // 构造影子2零件
        QPointF pos3(2*d, 0);
        piece3.moveTo(pos3);  // 将影子2零件移动至(d, 0)处,以保证在旋转过程中,两零件不会碰撞
        piece3.rotate(pos3, i);  // 将影子2零件绕中心点旋转

        qreal h = piece1.getMinBoundingRect().height();  // 获取外包矩形的高度
        qreal w = piece1.getMinBoundingRect().width();  // 获取外包矩形的宽度
        // 寻找最合适的下沉深度
        qreal deltaV = d/n;
        for(qreal v=0; v<h; v=v+deltaV){
            v = qrealPrecision(v, PRECISION);  // 保留精度
            qDebug() << "v: " << v;
            Piece piece2Copy = piece2;  // 保存零件2的状态
            Piece piece3Copy = piece3;  // 保存零件3的状态
            piece2Copy.moveTo(pos2+QPointF(0, v));  // 中间零件下沉
            piece2Copy.compactToOnHD(piece1, compactStep, compactAccuracy);  // 零件2向左平移，与零件1靠接
            piece3Copy.compactToOnHD(piece2Copy, compactStep, compactAccuracy);  // 零件3向左平移，与零件1靠接
            qreal step = piece3Copy.getPosition().rx()-piece1.getPosition().rx();  // 获取附加偏移
            qreal area = (h+v) * (w+step);  // 获取整体外包矩形的面积
            if(area < minArea){
                minArea = qrealPrecision(area, PRECISION);  // 最小面积
                alpha = i;  // 旋转角度
                height = qrealPrecision(h+v, PRECISION);  // 整体外包矩形的高
                width = qrealPrecision(w+step, PRECISION);  // 整体外包举行的宽
                stepX = qrealPrecision(step, PRECISION); // 水平步距
                cOffset = QPointF(piece2Copy.getPosition().rx()-piece1.getPosition().rx(), v);  // 零件2相较于零件1的偏移
                cOffset = pointPrecision(cOffset, PRECISION);
            }
        }
    }
    qDebug() << "最小面积: " << minArea << ", alpha: " << alpha << ", stepX: " << stepX << ", cOffset: " << cOffset;
}

/**
 * @brief NestEngine::pairwiseDoubleRowNest  对头双排排版
 * @param piece 零件
 * @param alpha 旋转角度
 * @param cOffset  零件2相较于零件偏移
 * @param width  整体外包矩形的宽
 * @param height  整体外包矩形的高
 */
void NestEngine::pairwiseDoubleRowNest(Piece piece, qreal &alpha, QPointF &cOffset, qreal &width, qreal &height)
{
    qreal minArea = LONG_MAX;  // 整体包络矩形的最小面积
    qreal pieceWidth = piece.getMinBoundingRect().width();  // 获取零件外包矩形宽度
    qreal pieceHeight = piece.getMinBoundingRect().height();  // 获取零件外包矩形高度
    qreal d = qSqrt(pieceWidth*pieceWidth + pieceHeight*pieceHeight);  // 获取外包矩形对角线距离
    d = qrealPrecision(d, PRECISION);

    for(int i=0; i<180; i++){
        qDebug() << "i:  " << i;
        Piece piece1 = piece;
        QPointF pos1(0, 0);
        piece1.moveTo(pos1);  // 将零件移至原点
        piece1.rotate(pos1, i);  // 将零件绕中心点旋转

        Piece piece2 = piece;  // 构造影子零件
        QPointF pos2(pieceWidth, pieceHeight);
        piece2.moveTo(pos2);  // 将影子零件移动至(d, 0)处,以保证在旋转过程中,两零件不会碰撞
        piece2.rotate(pos2, i+180);  // 将影子零件绕中心点旋转

        qreal theta = i * M_PI / 180;
        piece2.compactToOnAlpha(piece1, theta, compactStep, compactAccuracy);  // 零件2沿i方向向零件1靠接
        QPointF offset = piece2.getPosition()-piece1.getPosition();  // 获取附加偏移

        // 获取外包矩形的宽高
        qreal h = piece2.getMinBoundingRect().top() - piece1.getMinBoundingRect().bottom();  // 获取外包矩形的高度
        qreal w = piece2.getMinBoundingRect().right() - piece1.getMinBoundingRect().left();  // 获取外包矩形的宽度
        qreal area = h * w;  // 获取整体外包矩形的面积
        if(area < minArea){
            minArea = qrealPrecision(area, PRECISION);  // 最小面积
            alpha = i;  // 旋转角度
            height = qrealPrecision(h, PRECISION);  // 整体外包矩形的高
            width = qrealPrecision(w, PRECISION);  // 整体外包举行的宽
            cOffset = pointPrecision(offset, PRECISION);
        }
    }
    qDebug() << "最小面积: " << minArea << ", alpha: " << alpha << ", cOffset: " << cOffset;
}

/**
 * @brief NestEngine::singleRowNestWithVerAlg 使用顶点算法的单排算法
 * @param piece  待排零件
 * @param alpha  旋转角度
 * @param step  送料步距
 * @return
 */
qreal NestEngine::singleRowNestWithVerAlg(Piece piece, qreal &alpha, qreal &step)
{
    qreal minZ = LONG_MAX;  // 目标值，希望其最小
    for(int i=0; i<=180; i++){  // 遍历180
        Piece p = piece;  // 复制该零件
        p.moveTo(QPointF(0, 0));  // 移动至原点，非必须
        p.rotate(p.getPosition(), i);  // 旋转
        qreal h = p.getMinBoundingRect().height();  // 获取旋转之后的高度
        QVector<QPointF> points = p.getPointsList();
        qreal d = calVerToOppSideXDis(points);  // 直接计算步距
        qreal hd = h * d;
        if(hd < minZ){  // 寻找最小值，并记录最小值时各变量的值
            minZ = hd;
            alpha = i;
            step = d;
        }
    }
    qDebug() << "最小Z: " << minZ << ", alpha: " << alpha << ", step: " << step;
    qreal rate = piece.getArea() / minZ;
    return rate;
}

/**
 * @brief NestEngine::doubleRowNestWithVerAlg 普通双排
 * @param piece  零件
 * @param alpha  旋转角度
 * @param step  送料步距
 * @param H  错开高度
 * @param X  错开零件x方向偏移
 * @param n  错开量精度，默认为100
 * @return
 */
qreal NestEngine::doubleRowNestWithVerAlg(Piece piece, qreal &alpha, qreal &step, qreal &X, qreal &H, const qreal n)
{
    qreal minZ = LONG_MAX;  // 目标值，希望其最小
    for(int i=0; i<=180; i++){
        qreal pieceHeight = piece.getMinBoundingRect().height();  // 切割件高度
        for(qreal delta=-pieceHeight; delta<pieceHeight; delta+=pieceHeight/n){
            Piece p = piece;  // 复制，零件1
            p.moveTo(QPointF(0, 0));  // 移动至原点，非必须
            p.rotate(p.getPosition(), i);  // 旋转
            qreal h = p.getMinBoundingRect().height() + delta;  // 获取旋转之后的高度，注意要加上错开量
            // 计算步距
            QVector<QPointF> points = p.getPointsList();
            qreal d1 = calVerToOppSideXDis(points);  // 计算各顶点到对边距离的最大值

            // 计算各顶点到错开零件各边最大值与最小值的差
            qreal offset = p.getMinBoundingRect().width();  // 将零件移动至外包矩形相切处
            qreal moveLeft;
            qreal d2 = calVerToCrossMaxMinDiff(points, offset, delta, moveLeft);

            qreal d = d1 > d2 ? d1 : d2;  // 取二者最大值
            qreal hd = h * d;
            if(hd < minZ){  // 寻找最小值，并记录最小值时各变量的值
                minZ = hd;
                alpha = i;
                step = d;
                X = offset - moveLeft;
                H = delta;
            }
        }
    }

    qDebug() << "最小Z: " << minZ << ", alpha: " << alpha << ", step: " << step << "X: " << X << ", H: " << H;
    qreal rate = 2 * piece.getArea() / minZ;
    return rate;
}

/**
 * @brief NestEngine::oppositeSingleRowNestWithVerAlg  对头单排算法
 * @param piece  待排零件
 * @param alpha  旋转角度
 * @param step  送料步距
 * @param offset  旋转中心较零件位置的偏移量
 * @return
 */
qreal NestEngine::oppositeSingleRowNestWithVerAlg(Piece piece, qreal &alpha, qreal &step, QPointF &offset)
{
    qreal minZ = LONG_MAX;  // 目标值，希望其最小
    for(int i=0; i<=180; i++){
        Piece p = piece;  // 复制，零件1
        p.moveTo(QPointF(0, 0));  // 移动至原点，非必须
        p.rotate(p.getPosition(), i);  // 旋转
        qreal h = p.getMinBoundingRect().height();  // 获取旋转之后的高度
        // 直接计算步距
        QVector<QPointF> points = p.getPointsList();
        qreal d = calVerToLeftXDis(points);  // 计算
        qreal hd = h * d;
        if(hd < minZ){  // 寻找最小值，并记录最小值时各变量的值
            // 计算旋转中心点
            qreal xMin = p.getMinBoundingRect().left();  // xMin
            qreal yMin = p.getMinBoundingRect().bottom();  // yMin
            qreal yMax = p.getMinBoundingRect().top();  // yMax
            minZ = hd;
            alpha = i;
            step = d;
            QPointF rotateCenter;  // 旋转中心
            rotateCenter.setX(xMin + d / 2);
            rotateCenter.setY((yMin + yMax) / 2);
            offset = rotateCenter - p.getPosition();  // 相较与位置点的偏移
            offset = pointPrecision(offset, PRECISION);  // 保留精度
        }
    }

    qDebug() << "最小Z: " << minZ << ", alpha: " << alpha << ", step: " << step << ", center: " << offset;
    qreal rate = 2 * piece.getArea() / minZ;
    return rate;
}

/**
 * @brief NestEngine::oppositeDoubleRowNestWithVerAlg  对头双排算法
 * @param piece  待排零件
 * @param alpha  旋转角度
 * @param step  送料步距
 * @param offset  旋转中心较零件位置的偏移量
 * @param H  错开量
 * @param n  错开量精度，默认为100
 * @return
 */
qreal NestEngine::oppositeDoubleRowNestWithVerAlg(Piece piece, qreal &alpha, qreal &step, QPointF &offset, qreal &H, const qreal n)
{
    qreal minZ = LONG_MAX;  // 目标值，希望其最小
    qreal pieceHeight = piece.getMinBoundingRect().height();  // 切割件高度
    qreal pieceWidth = piece.getMinBoundingRect().width();  // 切割件宽度
    qreal L = pieceHeight > pieceWidth ? pieceHeight : pieceWidth;  // 选择两者较长的一个作为上界
    for(qreal delta=0; delta<L; delta+=L/n){
        for(int i=0; i<=180; i++){
            Piece p = piece;  // 复制，零件1
            p.moveTo(QPointF(0, 0));  // 移动至原点，非必须
            p.rotate(p.getPosition(), i);  // 旋转
            qreal h = p.getMinBoundingRect().height() + delta;  // 获取旋转之后的高度，注意要加上错开量
            // 直接计算步距
            QVector<QPointF> points = p.getPointsList();
            qreal d = calVerToLeftXDis(points, delta);  // 计算步距
            qreal hd = h * d;
            if(hd < minZ){  // 寻找最小值，并记录最小值时各变量的值
                // 计算旋转中心点
                qreal xMin = p.getMinBoundingRect().left();  // xMin
                qreal yMin = p.getMinBoundingRect().bottom();  // yMin
                qreal yMax = p.getMinBoundingRect().top();  // yMax
                minZ = hd;
                alpha = i;
                step = d;
                H = delta;
                QPointF rotateCenter;  // 旋转中心
                rotateCenter.setX(xMin + d / 2);
                rotateCenter.setY((yMin + yMax + H) / 2);
                offset = rotateCenter - p.getPosition();  // 相较与位置点的偏移
                offset = pointPrecision(offset, PRECISION);  // 保留精度
            }
        }
    }

    qDebug() << "最小Z: " << minZ << ", alpha: " << alpha << ", step: " << step << ", offset: " << offset;
    qreal rate = 2 * piece.getArea() / minZ;
    return rate;
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

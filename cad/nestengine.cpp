#include "nestengine.h"
#include "nestengineconfigure.h"
#include <QDebug>

NestEngine::NestEngine(QObject *parent) :
    QObject(parent),
    isStripSheet(false),
    autoRepeatLastSheet(false),
    compactStep(5),
    compactAccuracy(1),
    nestType(SingleRow),
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

NestEngine::NestEngine(QObject *parent,
                       const QVector<Piece> pieceList,
                       QVector<Sheet> sheetList) :
    QObject(parent),
    isStripSheet(false),
    autoRepeatLastSheet(false),
    compactStep(5),
    compactAccuracy(1),
    nestType(SingleRow),
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
    // 将零件按由大到小排序
    sortedPieceListByArea(pieceList, transformMap);
    // 初始化每张材料的四叉树
    for(int i=0; i<sheetList.length(); i++){
        initQuadTreeMap(i);
    }
}

NestEngine::NestEngine(QObject *parent,
                       const QVector<Piece> pieceList,
                       QVector<Sheet> sheetList,
                       QVector<NestEngine::SameTypePiece> sameTypePieceList) :
    NestEngine(parent, pieceList, sheetList)
{
    this->sameTypePieceList = sameTypePieceList;
    initSameTypeNestPieceIndexMap();
}

NestEngine::~NestEngine()
{
    pieceList.clear();
    sheetList.clear();
    sameTypePieceList.clear();
    nestPieceList.clear();
    nestedPieceIndexlist.clear();
    sameTypeNestPieceIndexMap.clear();
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
    initSameTypeNestPieceIndexMap();
}

QVector<NestEngine::SameTypePiece> NestEngine::getSameTypePieceList()
{
    return this->sameTypePieceList;
}

void NestEngine::setPairPieceList(QVector<PairPiece> pairPieceList)
{
    this->pairPieceList = pairPieceList;
    initSamePairNestPieceIndexMap();
}

QVector<NestEngine::PairPiece> NestEngine::getPairPieceList()
{
    return this->pairPieceList;
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

void NestEngine::sortedPieceListByArea(QVector<Piece> pieceList, QMap<int, QVector<int>> &transformMap)
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

    int newID = 0;
    transformMap.clear();
    QMap<qreal, QVector<int>>::const_iterator i;
    for(i=pieceAreaMap.constBegin(); i!=pieceAreaMap.constEnd(); ++i){
        foreach (int oldID, i.value()) {
            if(!transformMap.contains(oldID)){
                QVector<int> dic;
                transformMap.insert(oldID, dic);
            }
            transformMap[oldID].append(newID++);
            // 记录变换前后的位置映射关系，变换后为newID，变换前为oldID
        }
    }

    foreach (int newID, transformMap.keys()) {
        int oldID = -1;
        foreach (int id, transformMap.keys()) {
            if(transformMap[id][0] == newID){
                oldID = id;
                break;
            }
        }
        transformMap[newID].append(oldID);
    }

#ifdef DEBUG
    foreach (int id, transformMap.keys()) {
        qDebug() << "id: " << id << ", " << transformMap[id][0] << ", " << transformMap[id][1] ;
    }
#endif
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
    if((nestEngineStrategys & SizeDown) != NoStrategy) {
        for(int i=0; i<pieceList.length(); i++){
            Piece piece = pieceList[transformMap[i][1]];
            PieceIndexRange indexRange(transformMap[i][1], count, count+piece.getCount()-1);
            nestPieceIndexRangeMap.insert(transformMap[i][1], indexRange);  // 初始化排版零件序号范围
            for(int j=0; j<piece.getCount(); j++){
                qreal transposeAngle = piece.isTranspose() ? 90 : 0;
                nestPieceList.append(NestPiece(count++, transformMap[i][1], transposeAngle));
            }
            pieceMaxPackPointMap.insert(transformMap[i][1], 0);  // 初始化零件排样点最大值map
        }
    }
    if((nestEngineStrategys & LeftRightTurn) != NoStrategy){
        foreach (PairPiece pairPiece, pairPieceList) {
            int left = pairPiece.leftID;
            int right = pairPiece.rightID;
            Piece leftPiece = pieceList[left];
            Piece rightPiece = pieceList[right];
            int leftNum = leftPiece.getCount();
            int rightNum = rightPiece.getCount();
            if(leftNum != rightNum){
                emit nestException(PiecePairError);
                break;
            }
            PieceIndexRange indexRange1(transformMap[left][0], count, count+leftNum+rightNum-1, 2);
            nestPieceIndexRangeMap.insert(transformMap[left][0], indexRange1);  // 初始化排版零件序号范围

            PieceIndexRange indexRange2(transformMap[right][0], count+1, count+leftNum+rightNum-1, 2);
            nestPieceIndexRangeMap.insert(transformMap[right][0], indexRange2);  // 初始化排版零件序号范围

            for(int i=0; i<leftNum; i++){
                nestPieceList.append(NestPiece(count++, transformMap[left][0], 0));
                nestPieceList.append(NestPiece(count++, transformMap[right][0], 180));
            }
        }
    }
}

void NestEngine::initSameTypeNestPieceIndexMap()
{
    // 使用sameTypeNestPieceIndexMap来记录每个零件的同型体编号
    for(int i=0; i<sameTypePieceList.length(); i++){
        SameTypePiece sameTypePiece = sameTypePieceList[i];  // 获取同型体列表
        QVector<int> pieceIDList = sameTypePiece.pieceIDList;  // 获取该同型体编号下的零件列表

        for(int j=0; j<pieceIDList.length()-1; j++){
            int oldIndex1 = pieceIDList[j];  // 之前的索引值
            int newIndex1 = transformMap[oldIndex1][0];  // 现在的索引值

            if(!sameTypeNestPieceIndexMap.contains(oldIndex1)){
                sameTypeNestPieceIndexMap.insert(oldIndex1, QVector<int>());
            }
            for(int k=j+1; k<pieceIDList.length(); k++){
                int oldIndex2 = pieceIDList[k];  // 之前的索引值
                int newIndex2 = transformMap[oldIndex2][0];  // 现在的索引值

                if(!sameTypeNestPieceIndexMap.contains(oldIndex2)){
                    sameTypeNestPieceIndexMap.insert(oldIndex2, QVector<int>());
                }
                if(newIndex2 > newIndex1){
                    if(!sameTypeNestPieceIndexMap[oldIndex1].contains(oldIndex2)){
                        sameTypeNestPieceIndexMap[oldIndex1].append(oldIndex2);
                    }
                } else{
                    if(!sameTypeNestPieceIndexMap[oldIndex2].contains(oldIndex1)){
                        sameTypeNestPieceIndexMap[oldIndex2].append(oldIndex1);
                    }
                }
            }
        }
    }
#ifdef DEBUG
    qDebug() << "result";
    foreach (int index, sameTypeNestPieceIndexMap.keys()) {
        qDebug() << "# " << index << " => [";
        foreach (int id, sameTypeNestPieceIndexMap[index]) {
            qDebug() << id << "  ";
        }
        qDebug() << "]";
    }
#endif
}

void NestEngine::initSamePairNestPieceIndexMap()
{
    foreach (PairPiece pairPiece, pairPieceList) {
        int left = pairPiece.leftID;  // 获取左支编号
        int right = pairPiece.rightID;  // 获取右支编号
        if(!samePairNestPieceIndexMap.contains(left)){
            samePairNestPieceIndexMap.insert(left, right);
        }
        if(!samePairNestPieceIndexMap.contains(right)){
            samePairNestPieceIndexMap.insert(right, left);
        }

        // 同双对默认为同型体
        if(!sameTypeNestPieceIndexMap.contains(left)){
            QVector<int> list;
            sameTypeNestPieceIndexMap.insert(left, list);
        }
        if(!sameTypeNestPieceIndexMap[left].contains(right)){
            sameTypeNestPieceIndexMap[left].append(right);
        }

        if(!sameTypeNestPieceIndexMap.contains(right)){
            QVector<int> list;
            sameTypeNestPieceIndexMap.insert(right, list);
        }
        if(!sameTypeNestPieceIndexMap[right].contains(left)){
            sameTypeNestPieceIndexMap[right].append(left);
        }
    }

#ifdef DEBUG
    qDebug() << "samePairNestPieceIndexMap: ";
    foreach (int id, samePairNestPieceIndexMap.keys()) {
        qDebug() << "# " << id << "->" << "# " << samePairNestPieceIndexMap[id];
    }
    qDebug() << endl;
#endif
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
        //setNestAdaptiveSpacingTypes(NestEngine::HorizontalAdaptiveSpacing);  // 默认横向自适应
        qDebug() <<"whole";
        qDebug() << "混合方式"<<wholeSheetNest.wholemixing << "--" << mixingTyes;
        qDebug() <<"排版"<<wholeSheetNest.wholeorientation << "--" << orientations;
        qDebug() <<"旋转角度"<<wholeSheetNest.wholedegree << "--" << maxRotateAngle;
        break;
    }
    case (Sheet::Strip):{
        isStripSheet = true;  // 条形板材料标志为true
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
        //setNestAdaptiveSpacingTypes(NestEngine::HorizontalAdaptiveSpacing);  // 默认横向自适应
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
    qreal minArea = LONG_MAX;  // 整体包络矩形的min面积
    qreal pieceWidth = piece.getBoundingRect().width();  // 获取零件外包矩形宽度
    qreal pieceHeight = piece.getBoundingRect().height();  // 获取零件外包矩形高度
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

        qreal h = piece1.getBoundingRect().height();  // 获取外包矩形的高度
        qreal w = piece1.getBoundingRect().width();  // 获取外包矩形的宽度
        piece2.compactToOnHD(piece1, compactStep, compactAccuracy);  // 水平靠接
        qreal step = piece2.getPosition().rx()-piece1.getPosition().rx();  // 返回靠接移动距离
        qreal area = h * (w+step);  // 获取整体外包矩形的面积
        if(area < minArea){
            minArea = qrealPrecision(area, PRECISION);  // min面积
            alpha = i;  // 旋转角度
            height = qrealPrecision(h, PRECISION);  // 整体外包矩形的高
            width = qrealPrecision(w+step, PRECISION);  // 整体外包举行的宽
            stepX = qrealPrecision(step, PRECISION); // 水平步距
        }
    }

    qDebug() << "min面积: " << minArea << ", alpha: " << alpha << ", stepX: " << stepX;
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
    qreal minArea = LONG_MAX;  // 整体包络矩形的min面积
    qreal pieceWidth = piece.getBoundingRect().width();  // 获取零件外包矩形宽度
    qreal pieceHeight = piece.getBoundingRect().height();  // 获取零件外包矩形高度
    qreal d = qSqrt(pieceWidth*pieceWidth + pieceHeight*pieceHeight);  // 获取外包矩形对角线距离
    d = qrealPrecision(d, PRECISION);
    for(int i=0; i<=90; i++){
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

        qreal h = piece1.getBoundingRect().height();  // 获取外包矩形的高度
        qreal w = piece1.getBoundingRect().width();  // 获取外包矩形的宽度
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
                minArea = qrealPrecision(area, PRECISION);  // min面积
                alpha = i;  // 旋转角度
                height = qrealPrecision(h+v, PRECISION);  // 整体外包矩形的高
                width = qrealPrecision(w+step, PRECISION);  // 整体外包举行的宽
                stepX = qrealPrecision(step, PRECISION); // 水平步距
                cOffset = QPointF(piece2Copy.getPosition().rx()-piece1.getPosition().rx(), v);  // 零件2相较于零件1的偏移
                cOffset = pointPrecision(cOffset, PRECISION);
            }
        }
    }
    qDebug() << "min面积: " << minArea << ", alpha: " << alpha << ", stepX: " << stepX << ", cOffset: " << cOffset;
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
    qreal minArea = LONG_MAX;  // 整体包络矩形的min面积
    qreal pieceWidth = piece.getBoundingRect().width();  // 获取零件外包矩形宽度
    qreal pieceHeight = piece.getBoundingRect().height();  // 获取零件外包矩形高度
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
        qreal h = piece2.getBoundingRect().top() - piece1.getBoundingRect().bottom();  // 获取外包矩形的高度
        qreal w = piece2.getBoundingRect().right() - piece1.getBoundingRect().left();  // 获取外包矩形的宽度
        qreal area = h * w;  // 获取整体外包矩形的面积
        if(area < minArea){
            minArea = qrealPrecision(area, PRECISION);  // min面积
            alpha = i;  // 旋转角度
            height = qrealPrecision(h, PRECISION);  // 整体外包矩形的高
            width = qrealPrecision(w, PRECISION);  // 整体外包举行的宽
            cOffset = pointPrecision(offset, PRECISION);
        }
    }
    qDebug() << "min面积: " << minArea << ", alpha: " << alpha << ", cOffset: " << cOffset;
}

qreal NestEngine::calRealBestXStep(const Piece &piece, const qreal alpha, const QPointF &offset, bool forOppFlag) const
{
    Piece p1 = piece;
    p1.moveTo(QPointF(0, 0));
    p1.rotate(p1.getPosition(), alpha);

    Piece p2 = p1;
    if(forOppFlag){
        p2.rotate(p1.getPosition()+offset, 180);
    } else{
        p2.moveTo(p1.getPosition()+offset);
    }

    QVector<QPointF> pList1 = p1.getPointsList();  // 零件1的点集
    QVector<QPointF> pList2 = p2.getPointsList();  // 零件2的点集

    qreal d1 = calVerToOppSideXDis(pList1);  // 直接计算步距
    qreal d2 = cal2PolygonMaxMinDiff(pList1, pList2);  // 计算两多边形最大值与最小值的差
    qreal step = d1 > d2 ? d1 : d2;  // 取较大值作为最佳送料步距
    return step;
}

/**
 * @brief NestEngine::singleRowNestWithVerAlg 使用顶点算法的单排算法
 * @param piece  待排零件
 * @param alpha  旋转角度
 * @param step  送料步距
 * @param maxRotateAngle 旋转最大角度
 * @param maxHeight  最大高度限制
 * @return
 */
qreal NestEngine::singleRowNestWithVerAlg(const Piece &piece, qreal &alpha, qreal &step,
                                          const int maxRotateAngle, const qreal maxWidth,
                                          const qreal maxHeight)
{
    // 初始化
    alpha = step = 0.0f;
    qreal minZ = LONG_MAX;  // 目标值，希望其min
    for(int i=0; i<=maxRotateAngle; i++){  // 遍历180
        Piece p = piece;  // 复制该零件
        p.moveTo(QPointF(0, 0));  // 移动至原点，非必须
        p.rotate(p.getPosition(), i);  // 旋转
        qreal w = p.getBoundingRect().width();  // 获取旋转之后的宽度
        qreal h = p.getBoundingRect().height();  // 获取旋转之后的高度
        if(w > maxWidth || h > maxHeight){  // 如果旋转高度大于材料高度，直接进行下次循环
            continue;
        }
        QVector<QPointF> points = p.getPointsList();
        qreal d = calVerToOppSideXDis(points);  // 直接计算步距
        qreal hd = h * d;
        if(hd < minZ){  // 寻找min值，并记录min值时各变量的值
            minZ = hd;
            alpha = i;
            step = d;
        }
    }
    // 如果目标值仍为LONG_MAX,直接返回0.0f
    if(minZ == LONG_MAX){
        return 0.0f;
    }
    qreal rate = piece.getArea() / minZ;
    qDebug() << "minZ: " << minZ << ", alpha: " << alpha << ", step: " << step << ", rate: " << rate;
    return rate;
}

/**
 * @brief NestEngine::doubleRowNestWithVerAlg 普通双排
 * @param piece  零件
 * @param alpha  旋转角度
 * @param step  送料步距
 * @param X  错开零件x方向偏移
 * @param H  错开高度
 * @param n  错开量精度，默认为100
 * @param maxRotateAngle 旋转最大角度
 * @param maxHeight  最大高度限制
 * @return
 */
qreal NestEngine::doubleRowNestWithVerAlg(const Piece &piece,
                                          qreal &alpha,
                                          qreal &step,
                                          qreal &X,
                                          qreal &H,
                                          const qreal n,
                                          const int maxRotateAngle,
                                          const qreal maxWidth,
                                          const qreal maxHeight)
{
    // 初始化
    alpha = step = X = H = 0.0f;
    qreal minZ = LONG_MAX;  // 目标值，希望其min
    for(int i=0; i<=maxRotateAngle; i++){
        Piece p = piece;  // 复制，零件1
        p.moveTo(QPointF(0, 0));  // 移动至原点，非必须
        p.rotate(p.getPosition(), i);  // 旋转

        QVector<QPointF> points = p.getPointsList();  // 获取旋转之后的点集
        qreal pieceWidth = p.getBoundingRect().width();  // 切割件高度
        qreal pieceHeight = p.getBoundingRect().height();  // 切割件高度
        if(pieceWidth > maxWidth || pieceHeight > maxHeight){// 如果零件高度大于材料高度，直接进行下次循环
            continue;
        }
        for(qreal delta=0; delta<pieceHeight; delta+=pieceHeight/n){
            qreal h = pieceHeight + qAbs(delta);  // 获取旋转之后的高度，注意要加上错开量
            if(h>maxHeight){  // 如果旋转高度大于材料高度，直接进行下次循环
                continue;
            }
            // 计算步距
            qreal d1 = calVerToOppSideXDis(points);  // 计算各顶点到对边距离的最大值
            // 计算各顶点到错开零件各边最大值与min值的差
            qreal offset = p.getBoundingRect().width();  // 将零件移动至外包矩形相切处
            qreal moveLeft;
            qreal d2 = calVerToCrossMaxMinDiff(points, offset, delta, moveLeft);
            if(d2 < 0){  // 如果差值小于0，则代表无交点
                continue;
            }

            qreal moveRight = offset - moveLeft;  // 右移距离
            if(moveRight < 0){  // 如果移动距离小于0，继续循环
                continue;
            }

            qreal d = d1 > d2 ? d1 : d2;  // 取二者最大值
            if(d < moveRight){  // 如果右移距离大于步距，继续循环
                continue;
            }
            qreal hd = h * d;
            if(hd < minZ){  // 寻找min值，并记录min值时各变量的值
                minZ = hd;
                alpha = i;
                step = d;
                X = offset - moveLeft;
                H = delta;
            }
        }
    }
    // 如果目标值仍为LONG_MAX,直接返回0.0f
    if(minZ == LONG_MAX){
        return 0.0f;
    }
    qreal rate = 2 * piece.getArea() / minZ;
    qDebug() << "minZ: " << minZ << ", alpha: " << alpha << ", step: " << step << "X: " << X << ", H: " << H << ", rate: " << rate;
    return rate;
}

/**
 * @brief NestEngine::oppositeSingleRowNestWithVerAlg  对头单排算法
 * @param piece  待排零件
 * @param alpha  旋转角度
 * @param step  送料步距
 * @param offset  旋转中心较零件位置的偏移量
 * @param maxRotateAngle 旋转最大角度
 * @param maxHeight  最大高度限制
 * @return
 */
qreal NestEngine::oppositeSingleRowNestWithVerAlg(const Piece &piece,
                                                  qreal &alpha,
                                                  qreal &step,
                                                  QPointF &offset,
                                                  const int maxRotateAngle,
                                                  const qreal maxWidth,
                                                  const qreal maxHeight)
{
    // 初始化
    alpha = step = 0.0f;
    offset = QPointF(0, 0);
    qreal minZ = LONG_MAX;  // 目标值，希望其min
    for(int i=0; i<=maxRotateAngle; i++){
        Piece p = piece;  // 复制，零件1
        p.moveTo(QPointF(0, 0));  // 移动至原点，非必须
        p.rotate(p.getPosition(), i);  // 旋转
        qreal w = p.getBoundingRect().width();  // 获取旋转之后的宽度
        qreal h = p.getBoundingRect().height();  // 获取旋转之后的高度
        if(w > maxWidth || h > maxHeight){  // 如果旋转高度大于材料高度，直接进行下次循环
            continue;
        }
        // 直接计算步距
        QVector<QPointF> points = p.getPointsList();
        qreal d = calVerToLeftXDis(points);  // 计算各顶点到外包矩形最左侧的距离与关于(yMin+yMax)/2对称位置水平距离之和的最大值
        qreal hd = h * d;
        if(hd < minZ){  // 寻找min值，并记录min值时各变量的值
            // 计算旋转中心点
            qreal xMin = p.getBoundingRect().left();  // xMin
            qreal yMin = p.getBoundingRect().bottom();  // yMin
            qreal yMax = p.getBoundingRect().top();  // yMax
            minZ = hd;
            alpha = i;
            step = d;  // 最佳送料步距其实不是这个d
            QPointF rotateCenter;  // 旋转中心
            rotateCenter.setX(xMin + d / 2);
            rotateCenter.setY((yMin + yMax) / 2);
            offset = rotateCenter - p.getPosition();  // 相较与位置点的偏移
            offset = pointPrecision(offset, PRECISION);  // 保留精度
        }
    }

    // 如果目标值仍为LONG_MAX,直接返回0.0f
    if(minZ == LONG_MAX){
        return 0.0f;
    }
    // 计算最佳送料步距
    step = calRealBestXStep(piece, alpha, offset);
    // 计算利用率
    qreal rate = 2 * piece.getArea() / minZ;
    qDebug() << "minZ: " << minZ << ", alpha: " << alpha << ", step: " << step << ", center: " << offset << ", rate: " << rate;
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
 * @param maxRotateAngle 旋转最大角度
 * @param maxHeight  最大高度限制
 * @return
 */
qreal NestEngine::oppositeDoubleRowNestWithVerAlg(const Piece &piece,
                                                  qreal &alpha,
                                                  qreal &step,
                                                  QPointF &offset,
                                                  qreal &H,
                                                  const qreal n,
                                                  int maxRotateAngle,
                                                  const qreal maxWidth,
                                                  const qreal maxHeight)
{
    // 初始化
    alpha = step = H = 0.0f;
    offset = QPointF(0, 0);
    qreal minZ = LONG_MAX;  // 目标值，希望其min
    for(int i=0; i<=maxRotateAngle; i++){
        Piece p = piece;  // 复制，零件1
        p.moveTo(QPointF(0, 0));  // 移动至原点，非必须
        p.rotate(p.getPosition(), i);  // 旋转
        qreal pieceWidth = p.getBoundingRect().width();  // 切割件宽度
        qreal pieceHeight = p.getBoundingRect().height();  // 切割件高度
        if(pieceWidth > maxWidth || pieceHeight > maxHeight){// 如果零件高度大于材料高度，直接进行下次循环
            continue;
        }
        for(qreal delta=0; delta<pieceHeight; delta+=pieceHeight/n){
            qreal h = pieceHeight + delta;  // 获取旋转之后的高度，注意要加上错开量
            if(h>maxHeight){  // 如果旋转高度大于材料高度，直接进行下次循环
                continue;
            }
            // 直接计算步距
            QVector<QPointF> points = p.getPointsList();
            qreal d = calVerToLeftXDis(points, delta);  // 计算步距
            qreal hd = h * d;
            if(hd < minZ){  // 寻找min值，并记录min值时各变量的值
                // 计算旋转中心点
                qreal xMin = p.getBoundingRect().left();  // xMin
                qreal yMin = p.getBoundingRect().bottom();  // yMin
                qreal yMax = p.getBoundingRect().top();  // yMax
                minZ = hd;
                alpha = i;
                step = d;  // 最佳送料步距其实不是这个d
                H = delta;
                QPointF rotateCenter;  // 旋转中心
                rotateCenter.setX(xMin + d / 2);
                rotateCenter.setY((yMin + yMax + H) / 2);
                offset = rotateCenter - p.getPosition();  // 相较与位置点的偏移
                offset = pointPrecision(offset, PRECISION);  // 保留精度
            }
        }
    }

    if(minZ == LONG_MAX){
        return 0.0f;
    }
    // 计算最佳送料步距
    step = calRealBestXStep(piece, alpha, offset);
    // 计算利用率
    qreal rate = 2 * piece.getArea() / minZ;
    qDebug() << "minZ: " << minZ << ", alpha: " << alpha << ", step: " << step << "H: " << H << ", offset: " << offset << ", rate: " << rate;
    return rate;
}

/**
 * @brief NestEngine::getBestNestType  获取最佳排版
 * @param piece  待排零件
 * @param alpha  旋转角度
 * @param xStep  x方向送料步距
 * @param pOffset 影子零件较基准零件位置的偏移量
 * @param rCOffset 外包矩形中心较基准零件位置的偏移量
 * @param yStep  y方向送料步距
 * @param maxRotateAngle 旋转最大角度
 * @param maxHeight  最大高度限制
 * @return
 */
NestEngine::NestType NestEngine::getPieceBestNestType(const Piece &piece,
                                                      qreal &alpha,
                                                      qreal &xStep,
                                                      QPointF &pOffset,
                                                      QPointF &rCOffset,
                                                      const int maxRotateAngle,
                                                      const qreal maxWidth,
                                                      const qreal maxHeight)
{
    NestType type = NoNestType;  // 最佳排版方式
    qreal rateMax = 0;  // 最佳利用率
    qreal a, s, x, h;
    QPointF o;

#if 0
    qreal rate1 = singleRowNestWithVerAlg(piece, a, s, maxRotateAngle, maxWidth, maxHeight);  // 普通单排方式
    if(rate1 > rateMax){
        rateMax = rate1;
        alpha = a;
        xStep = s;
        pOffset = QPointF(xStep, 0);
        type = NestType::SingleRow;
    }
#endif

#if 1
    qreal rate2 = doubleRowNestWithVerAlg(piece, a, s, x, h, 100, maxRotateAngle, maxWidth, maxHeight);  // 普通双排方式
    if(rate2 > rateMax){
        rateMax = rate2;
        alpha = a;
        xStep = s;
        pOffset = QPointF(x, h);
        type = h == 0 ? NestType::SingleRow : NestType::DoubleRow;
    }
#endif

#if 0
    qreal rate3 = oppositeSingleRowNestWithVerAlg(piece, a, s, o, maxRotateAngle, maxWidth, maxHeight); // 对头单排方式
    if(rate3 > rateMax){
        rateMax = rate3;
        alpha = a;
        xStep = s;
        QPointF pos2 = pointPrecision(transformRotate(piece.getPosition()+o, piece.getPosition(), 180), PRECISION);
        pOffset = pointPrecision(pos2-piece.getPosition(), PRECISION);
        type = NestType::OppositeSingleRow;
    }
#endif

#if 1
    qreal rate4 = oppositeDoubleRowNestWithVerAlg(piece, a, s, o, h, 100, maxRotateAngle, maxWidth, maxHeight);  // 对头双排方式
    if(rate4 > rateMax){
        rateMax = rate4;
        alpha = a;
        xStep = s;
        QPointF pos2 = pointPrecision(transformRotate(piece.getPosition()+o, piece.getPosition(), 180), PRECISION);
        pOffset = pointPrecision(pos2-piece.getPosition(), PRECISION);
        type = h == 0 ? NestType::OppositeSingleRow : NestType::OppositeDoubleRow;
    }
#endif

    rCOffset = QPointF((2*piece.getPosition().rx()+pOffset.rx())/2, (2*piece.getPosition().ry()+pOffset.ry())/2);
    return type;
}

void NestEngine::getAllBestNestTypes(QVector<Piece> pieceList)
{
    // 按照零件类型确定最佳排版方式
    for(int i=0; i<pieceList.length(); i++) {
        Piece piece = pieceList[i];
        qreal alpha, xStep;
        QPointF pOffset, rCOffset;
        NestType type = getPieceBestNestType(piece, alpha, xStep, pOffset, rCOffset, maxRotateAngle);
        qDebug() << "#" << i << ", bestNestType: " << (NestType)type;

        // 保存每个零件的最佳排版信息
        if(!pieceBestNestTypeMap.contains(i)){
            pieceBestNestTypeMap.insert(i, BestNestType());
        }
        BestNestType bestNestType(i, type, alpha, xStep, pOffset, rCOffset);
        pieceBestNestTypeMap[i] = bestNestType;
    }
}

/**
 * @brief NestEngine::oppositeDoubleRowNestWithVerAlgForStrip
 * @param layoutRect
 * @param piece1
 * @param piece2
 * @param pos1
 * @param pos2
 * @param xStep
 * @param flag
 * @return
 */
qreal NestEngine::oppositeDoubleRowNestWithVerAlgForStrip(const QRectF &layoutRect,
                                                          const Piece &piece1,
                                                          const Piece &piece2,
                                                          QPointF &pos1,
                                                          QPointF &pos2,
                                                          qreal &xStep,
                                                          const bool flag,
                                                          QRectF &boundRect1,
                                                          QRectF &boundRect2,
                                                          QRectF &pairBoundRect)
{
    qreal rate = 0.0f;
    // 初始化
    xStep = 0.0f;

    // 确定初始位置
    Piece p1 = piece1;  // 复制，零件1
    p1.moveTo(QPointF(0,0));  // 移动至原点，非必须
    p1.rotateByReferenceLine(p1.getPosition(), flag);  // 旋转
    // 把需要排放在条板内的零件区域标注出来
    QPointF offset1 = p1.getOffsetForReferenceLine(layoutRect);
    pos1 = p1.getPosition() + offset1;
    p1.moveTo(pos1);

    Piece p2 = piece2;  // 复制，零件2
    p2.moveTo(QPointF(0,0));  // 移动至原点，非必须
    p2.rotateByReferenceLine(p2.getPosition(), !flag);  // 旋转
    QPointF offset2 = p2.getOffsetForReferenceLine(layoutRect);
    p2.moveTo(p2.getPosition() + offset2 + QPointF(p1.getBoundingRect().width(), 0));

    // 进行水平靠接
    p2.compactToOnHD(p1, compactStep, compactAccuracy);
    pos2 = p2.getPosition();

    // 计算步距
    qreal d1 = calVerToOppSideXDis(p1.getPointsList());  // 计算各顶点到对边距离的最大值
    // 计算各顶点到错开零件各边最大值与min值的差
    qreal d2 = cal2PolygonMaxMinDiff(p1.getPointsList(), p2.getPointsList());
    xStep = d1 > d2 ? d1 : d2;  // 取二者最大值
    boundRect1 = p1.getBoundingRect();
    boundRect2 = p2.getBoundingRect();
    pairBoundRect = getPairBoundingRect(boundRect1, boundRect2);
    qreal pairBoundRectArea = pairBoundRect.width() * pairBoundRect.height();
    rate = (p1.getArea() + p2.getArea()) / pairBoundRectArea;
    return rate;
}

void NestEngine::packAlg()
{
    if(!unnestedPieceIndexlist.isEmpty()){
        qDebug() << "unnest index is not null!";
    }
    for(int j=0; j<nestPieceList.length(); j++){
        unnestedPieceIndexlist.append(nestPieceList[j].index);
    }
    packPieces(unnestedPieceIndexlist);
    return;

    // 以下内容可能要删除
    // 如果没有设置同型体，则直接对排样零件列表排样
    if(sameTypeNestPieceIndexMap.size() < 1){
        QVector<int> indexList;
        for(int j=0; j<nestPieceList.length(); j++){
            indexList.append(nestPieceList[j].index);
        }
        packPieces(indexList);
        return;
    }

    for(int i=0; i<sameTypeNestPieceIndexMap.size(); i++){
        // 获取编号为i的同型体零件列表
        QVector<int> sameTypeNestPieceList= sameTypeNestPieceIndexMap[i];
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
    if(!isStripSheet){  // 如果不为条形材料排版，则首先计算每个零件的最佳排版类型
        qDebug() << "如果不为条形材料排版，则首先计算每个零件的最佳排版类型";
        getAllBestNestTypes(pieceList);  // 获取每个零件最佳排样类型
    }
    initNestPieceList();  // 初始化排样零件
    packAlg();  // 进行连续排版
}

QRectF NestEngine::getPairBoundingRect(QPointF &pos1, QPointF &pos2,
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

QRectF NestEngine::getPairBoundingRect(QRectF &bRect1, QRectF &bRect2)
{
    qreal xMin1 = bRect1.left();
    qreal xMax1 = bRect1.right();
    qreal yMin1 = bRect1.top();
    qreal yMax1 = bRect1.bottom();

    qreal xMin2 = bRect2.left();
    qreal xMax2 = bRect2.right();
    qreal yMin2 = bRect2.top();
    qreal yMax2 = bRect2.bottom();

    qreal pairLeft = xMin1 < xMin2 ? xMin1 : xMin2;  // 组合零件左边界
    qreal pairTop = yMin1 < yMin2 ? yMin1 : yMin2;  // 组合零件上边界
    qreal pairRight = xMax1 > xMax2 ? xMax1 : xMax2;  // 组合零件右边界
    qreal pairBottom = yMax1 > yMax2 ? yMax1 : yMax2;  // 组合零件下边界

    qreal pairWidth = pairRight - pairLeft;  // 组合零件的宽
    qreal pairHeight = pairBottom - pairTop;  // 组合零件的高
    QRectF pairBoundingRect(pairLeft, pairTop, pairWidth, pairHeight);  // 组合零件的外包矩形
    return pairBoundingRect;
}

#include "continuenestengine.h"

ContinueNestEngine::ContinueNestEngine(QObject *parent) :
    NestEngine(parent)
{
    setNestEngineType(NestEngine::ContinueNest);
    getAllBestNestTypes(pieceList);
}

ContinueNestEngine::ContinueNestEngine(QObject *parent,
                                       const QVector<Piece> pieceList,
                                       const QVector<Sheet> sheetList) :
    NestEngine(parent, pieceList, sheetList)
{
    setNestEngineType(NestEngine::ContinueNest);
    getAllBestNestTypes(pieceList);
}

ContinueNestEngine::ContinueNestEngine(QObject *parent,
                                       const QVector<Piece> pieceList,
                                       const QVector<Sheet> sheetList,
                                       QVector<NestEngine::SameTypePiece> sameTypePieceList) :
    NestEngine(parent, pieceList, sheetList, sameTypePieceList)
{
    setNestEngineType(NestEngine::ContinueNest);
    getAllBestNestTypes(pieceList);
}

ContinueNestEngine::~ContinueNestEngine()
{

}

/**
 * @brief ContinueNestEngine::signalRowNest 最优单排计算函数
 * @param piece 零件
 * @param alpha 旋转角度
 * @param stepX 另一个零件与piece的水平方向偏移量
 * @param width 单排后整体包络矩形的宽度
 * @param height 单排后整体包络矩形的高度
 */
void ContinueNestEngine::singleRowNest(Piece piece, qreal &alpha, qreal &stepX, qreal &width, qreal &height)
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
 * @brief ContinueNestEngine::doubleRowNest  双排排版
 * @param piece 零件
 * @param n 控制精度的正整数
 * @param alpha 旋转角度
 * @param stepX 最外侧零件水平方向偏移量
 * @param cOffset 中间零件中心偏移量
 * @param width  整体外包矩形宽度
 * @param height  整体外包矩形高度
 */
void ContinueNestEngine::doubleRowNest(Piece piece, const int n, qreal &alpha, qreal &stepX, QPointF &cOffset, qreal &width, qreal &height)
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
 * @brief ContinueNestEngine::pairwiseDoubleRowNest  对头双排排版
 * @param piece 零件
 * @param alpha 旋转角度
 * @param cOffset  零件2相较于零件偏移
 * @param width  整体外包矩形的宽
 * @param height  整体外包矩形的高
 */
void ContinueNestEngine::pairwiseDoubleRowNest(Piece piece, qreal &alpha, QPointF &cOffset, qreal &width, qreal &height)
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

/**
 * @brief ContinueNestEngine::singleRowNestWithVerAlg 使用顶点算法的单排算法
 * @param piece  待排零件
 * @param alpha  旋转角度
 * @param step  送料步距
 * @return
 */
qreal ContinueNestEngine::singleRowNestWithVerAlg(Piece piece, qreal &alpha, qreal &step, int maxRotateAngle)
{
    qreal minZ = LONG_MAX;  // 目标值，希望其min
    for(int i=0; i<=maxRotateAngle; i++){  // 遍历180
        Piece p = piece;  // 复制该零件
        p.moveTo(QPointF(0, 0));  // 移动至原点，非必须
        p.rotate(p.getPosition(), i);  // 旋转
        qreal h = p.getBoundingRect().height();  // 获取旋转之后的高度
        QVector<QPointF> points = p.getPointsList();
        qreal d = calVerToOppSideXDis(points);  // 直接计算步距
        qreal hd = h * d;
        if(hd < minZ){  // 寻找min值，并记录min值时各变量的值
            minZ = hd;
            alpha = i;
            step = d;
        }
    }
    qreal rate = piece.getArea() / minZ;
    qDebug() << "minZ: " << minZ << ", alpha: " << alpha << ", step: " << step << ", rate: " << rate;
    return rate;
}

/**
 * @brief ContinueNestEngine::doubleRowNestWithVerAlg 普通双排
 * @param piece  零件
 * @param alpha  旋转角度
 * @param step  送料步距
 * @param X  错开零件x方向偏移
 * @param H  错开高度
 * @param n  错开量精度，默认为100
 * @return
 */
qreal ContinueNestEngine::doubleRowNestWithVerAlg(Piece piece, qreal &alpha, qreal &step, qreal &X, qreal &H, const qreal n)
{
    qreal minZ = LONG_MAX;  // 目标值，希望其min
    for(int i=0; i<=180; i++){
        Piece p = piece;  // 复制，零件1
        p.moveTo(QPointF(0, 0));  // 移动至原点，非必须
        p.rotate(p.getPosition(), i);  // 旋转

        QVector<QPointF> points = p.getPointsList();  // 获取旋转之后的点集
        qreal pieceHeight = piece.getBoundingRect().height();  // 切割件高度

        for(qreal delta=0; delta<pieceHeight; delta+=pieceHeight/n){
            qreal h = pieceHeight + qAbs(delta);  // 获取旋转之后的高度，注意要加上错开量
            // 计算步距
            qreal d1 = calVerToOppSideXDis(points);  // 计算各顶点到对边距离的最大值

            // 计算各顶点到错开零件各边最大值与min值的差
            qreal offset = p.getBoundingRect().width();  // 将零件移动至外包矩形相切处
            qreal moveLeft;
            qreal d2 = calVerToCrossMaxMinDiff(points, offset, delta, moveLeft);
            if(d2 < 0){  // 如果差值小于0，则代表无交点
                continue;
            }
            qreal d = d1 > d2 ? d1 : d2;  // 取二者最大值
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

    qreal rate = 2 * piece.getArea() / minZ;
    qDebug() << "minZ: " << minZ << ", alpha: " << alpha << ", step: " << step << "X: " << X << ", H: " << H << ", rate: " << rate;
    return rate;
}

/**
 * @brief ContinueNestEngine::oppositeSingleRowNestWithVerAlg  对头单排算法
 * @param piece  待排零件
 * @param alpha  旋转角度
 * @param step  送料步距
 * @param offset  旋转中心较零件位置的偏移量
 * @return
 */
qreal ContinueNestEngine::oppositeSingleRowNestWithVerAlg(Piece piece, qreal &alpha, qreal &step, QPointF &offset)
{
    qreal minZ = LONG_MAX;  // 目标值，希望其min
    for(int i=0; i<=180; i++){
        Piece p = piece;  // 复制，零件1
        p.moveTo(QPointF(0, 0));  // 移动至原点，非必须
        p.rotate(p.getPosition(), i);  // 旋转
        qreal h = p.getBoundingRect().height();  // 获取旋转之后的高度
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
            step = d;
            QPointF rotateCenter;  // 旋转中心
            rotateCenter.setX(xMin + d / 2);
            rotateCenter.setY((yMin + yMax) / 2);
            offset = rotateCenter - p.getPosition();  // 相较与位置点的偏移
            offset = pointPrecision(offset, PRECISION);  // 保留精度
        }
    }

    qreal rate = 2 * piece.getArea() / minZ;
    qDebug() << "minZ: " << minZ << ", alpha: " << alpha << ", step: " << step << ", center: " << offset << ", rate: " << rate;
    return rate;
}

/**
 * @brief ContinueNestEngine::oppositeDoubleRowNestWithVerAlg  对头双排算法
 * @param piece  待排零件
 * @param alpha  旋转角度
 * @param step  送料步距
 * @param offset  旋转中心较零件位置的偏移量
 * @param H  错开量
 * @param n  错开量精度，默认为100
 * @return
 */
qreal ContinueNestEngine::oppositeDoubleRowNestWithVerAlg(Piece piece, qreal &alpha, qreal &step, QPointF &offset, qreal &H, const qreal n)
{
    qreal minZ = LONG_MAX;  // 目标值，希望其min
    for(int i=0; i<=180; i++){
        Piece p = piece;  // 复制，零件1
        p.moveTo(QPointF(0, 0));  // 移动至原点，非必须
        p.rotate(p.getPosition(), i);  // 旋转
        qreal pieceHeight = piece.getBoundingRect().height();  // 切割件高度
        for(qreal delta=0; delta<pieceHeight; delta+=pieceHeight/n){
            qreal h = pieceHeight + delta;  // 获取旋转之后的高度，注意要加上错开量
            // 直接计算步距
            QVector<QPointF> points = p.getPointsList();
            //
            qreal d = calVerToLeftXDis(points, delta);  // 计算步距
            qreal hd = h * d;
            if(hd < minZ){  // 寻找min值，并记录min值时各变量的值
                // 计算旋转中心点
                qreal xMin = p.getBoundingRect().left();  // xMin
                qreal yMin = p.getBoundingRect().bottom();  // yMin
                qreal yMax = p.getBoundingRect().top();  // yMax
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

    qreal rate = 2 * piece.getArea() / minZ;
    qDebug() << "minZ: " << minZ << ", alpha: " << alpha << ", step: " << step << "H: " << H << ", offset: " << offset << ", rate: " << rate;
    return rate;
}

/**
 * @brief ContinueNestEngine::getBestNestType  获取最佳排版
 * @param piece  待排零件
 * @param alpha  旋转角度
 * @param xStep  x方向送料步距
 * @param pOffset 影子零件较基准零件位置的偏移量
 * @param boundingRectCenter 外包矩形中心较基准零件位置的偏移量
 * @param yStep  y方向送料步距
 * @return
 */
NestEngine::NestType ContinueNestEngine::getPieceBestNestType(const Piece &piece, qreal &alpha, qreal &xStep, QPointF &pOffset, QPointF &boundingRectCenter, qreal &yStep)
{
    NestType type;  // 最佳排版方式
    qreal rateMax = 0;  // 最佳利用率
    qreal a, s, x, h;
    QPointF o;
    qreal rate1 = singleRowNestWithVerAlg(piece, a, s, 0);  // 普通单排方式
    if(rate1 > rateMax){
        rateMax = rate1;
        alpha = a;
        xStep = s;
        pOffset = QPointF(xStep, 0);
        type = NestType::SingleRow;

        // 计算y方向步距
        Piece p = piece;  // 复制该零件
        p.moveTo(QPointF(0, 0));  // 移动至原点，非必须
        p.rotate(p.getPosition(), alpha+90);  // 旋转
        yStep = calVerToOppSideXDis(p.getPointsList());
    }
    boundingRectCenter = QPointF((2*piece.getPosition().rx()+pOffset.rx())/2, (2*piece.getPosition().ry()+pOffset.ry())/2);
    return type;
    /*
    qreal rate2 = doubleRowNestWithVerAlg(piece, a, s, x, h);  // 普通双排方式
    if(rate2 > rateMax){
        rateMax = rate2;
        alpha = a;
        xStep = s;
        pOffset = QPointF(x, h);
        type = NestType::DoubleRow;

        // 计算y方向步距
        Piece p = piece;  // 复制该零件
        p.moveTo(QPointF(0, 0));  // 移动至原点，非必须
        p.rotate(p.getPosition(), alpha+90);  // 旋转

        qreal d1 = calVerToOppSideXDis(p.getPointsList());  // 计算各顶点到对边距离的最大值
        // 计算各顶点到错开零件各边最大值与min值的差
        qreal offset = p.getBoundingRect().width();  // 将零件移动至外包矩形相切处
        qreal moveLeft;
        qreal d2 = calVerToCrossMaxMinDiff(p.getPointsList(), offset, x, moveLeft);
        yStep = d1 > d2 ? d1 : d2;  // 取二者最大值
    }
    */
    qreal rate3 = oppositeSingleRowNestWithVerAlg(piece, a, s, o); // 对头单排方式
    if(rate3 > rateMax){
        rateMax = rate3;
        alpha = a;
        xStep = s;
        QPointF pos2 = pointPrecision(transformRotate(piece.getPosition()+o, piece.getPosition(), 180), PRECISION);
        pOffset = pointPrecision(pos2-piece.getPosition(), PRECISION);
        type = NestType::OppositeSingleRow;

        // 计算y方向步距
        Piece p = piece;  // 复制该零件
        p.moveTo(QPointF(0, 0));  // 移动至原点，非必须
        p.rotate(p.getPosition(), alpha+90);  // 旋转
        yStep = calVerToOppSideXDis(p.getPointsList());
    }

    qreal rate4 = oppositeDoubleRowNestWithVerAlg(piece, a, s, o, h);  // 对头双排方式
    if(rate4 > rateMax){
        rateMax = rate4;
        alpha = a;
        xStep = s;
        QPointF pos2 = pointPrecision(transformRotate(piece.getPosition()+o, piece.getPosition(), 180), PRECISION);
        pOffset = pointPrecision(pos2-piece.getPosition(), PRECISION);
        type = NestType::OppositeDoubleRow;

        // 计算y方向步距
        Piece p = piece;  // 复制该零件
        p.moveTo(QPointF(0, 0));  // 移动至原点，非必须
        p.rotate(p.getPosition(), alpha+90);  // 旋转

        QPointF c = transformRotate(o, QPointF(0, 0), alpha+90);
        Piece p2 = p;
        p2.rotate(c, alpha+270);
        qreal d1 = calVerToOppSideXDis(p.getPointsList());  // 计算各顶点到对边距离的最大值
        // 计算各顶点到错开零件各边最大值与min值的差
        qreal d2 = cal2PolygonMaxMinDiff(p.getPointsList(), p2.getPointsList());
        yStep = d1 > d2 ? d1 : d2;  // 取二者最大值
    }
    boundingRectCenter = QPointF((2*piece.getPosition().rx()+pOffset.rx())/2, (2*piece.getPosition().ry()+pOffset.ry())/2);
    return type;
}

void ContinueNestEngine::getAllBestNestTypes(QVector<Piece> pieceList)
{
    // 按照零件类型确定最佳排版方式
    for(int i=0; i<pieceList.length(); i++){
        Piece piece = pieceList[i];
        qreal alpha, xStep, yStep;
        QPointF pOffset, rCOffset;
        NestType type = getPieceBestNestType(piece, alpha, xStep, pOffset, rCOffset, yStep);
        qDebug() << "#" << i << ", bestNestType: " << (NestType)type;

        // 保存每个零件的最佳排版信息
        if(!pieceBestNestTypeMap.contains(i)){
            pieceBestNestTypeMap.insert(i, BestNestType());
        }
        BestNestType bestNestType(i, type, alpha, xStep, pOffset, rCOffset, yStep);
        pieceBestNestTypeMap[i] = bestNestType;
    }
}

void ContinueNestEngine::packPieces(QVector<int> indexList)
{
    /**
      连续排版算法
      算法描述：
      Step0: 遍历零件类型ID，确定每种类型零件的最佳排版方法。
             选择材料利用率较高的一种连续排版方法（普单、普双、对单、对双中的一种），
             确定送料步距step，以及所占材料高度h。并且，初始化每个类型零件的剩余个数。
      Step1: 判断未排零件列表，如果为空，则结束；否则转到Step2。
      Step2: 初始化排放位置。先确定该零件的初始位置p1(x1, y1)，不超出材料排版范围即可。转到Step3。
      Step3: 用Step1确定的方法进行连续排版，每排完一个便需要判断零件右侧是否超出材料排版范围。
             如果超出材料范围且未使用尾只优化，则转到Step4；否则，转到Step5。
      Step4: 换行。首先，确定一个初始位置p2(x2=x1, y2=y1+h)，然后向上靠接，双排时要两两判断。
             靠接距离为下一行零件各顶点与上一行零件垂直方向上的距离的min值。转到Step6。
      Step5: 判断该行材料剩余宽度，如果该尺寸零件对应的min外包矩形的短边都无容纳，则尝试放置min码数的切割件；
             如果可以容纳，则通过旋转放置该尺寸零件。转到Step4。
      Step6: 确定下一行的位置后，要判断是否超过材料下边界。这里要注意是否使用了尾行优化，允许任意角度。
             如果超过了材料边界且未使用尾行优化，则转到Step7；否则转到Step8。
      Step7: 添加新材料，转到Step1。
      Step8: 先判断剩余材料高度，如果该尺寸零件对应的min外包矩形的短边都无法放下，则尝试放置min码数的切割件；
             如果可以容纳，则通过旋转、合并进行排版，同样根据四种排版方法的性能，只是多了一个限制条件，及材料高度。
     */

    // 将向量转化为列表
    QList<int> unnestedList = indexList.toList();

    // 排版，按照材料进行排版，排满一张后再排下一张
    int sheetID = 0;
    QPointF availablePos1, availablePos2;  // 可用材料的区域左上角顶点值
    availablePos1 = availablePos2 = sheetList[sheetID].layoutRect().topLeft();
    // 排放零件直到所有零件都排放完
    while(!unnestedList.isEmpty()){
        bool sheetAvailable = true;  // 记录材料是否用完
        QList<int> nestedList;  // 记录已排零件
        int index = unnestedList.first();  // 获取未排零件的第一个零件
        int type = nestPieceList[index].typeID;  // 获取该零件的类型号
        BestNestType bestNestType = pieceBestNestTypeMap[type];  // 获取该类型零件的最佳排版方式

        Piece piece = pieceList[type];  // 复制该零件
        piece.moveTo(QPointF(0, 0));  // 将零件移至坐标原点
        piece.rotate(piece.getPosition(), bestNestType.alpha);  // 按最佳方式旋转该零件
        QRectF boundingRect = piece.getBoundingRect();  // 获取该角度下的外包矩形
        qreal pieceWidth = boundingRect.width();  // 获取外包举行的宽
        qreal pieceHeight = boundingRect.height();  // 获取外包举行的高
        /*
//        qreal pairLeft = piece.getPosition().rx()-0.5*width;  // 组合零件左边界
//        qreal pairTop = piece.getPosition().ry()-0.5*height;  // 组合零件上边界
//        qreal pairWidth = 2 * (0.5*width+bestNestType.rCOffset.rx());  // 组合零件的宽
//        qreal pairHeight = 2 * (0.5*height+bestNestType.rCOffset.ry());  // 组合零件的高
//        QRectF pairBoundingRect(pairLeft, pairTop, pairWidth, pairHeight);  // 组合零件的外包矩形
        */
        QPointF offset = sheetList[sheetID].layoutRect().topLeft()
                - piece.getBoundingRect().topLeft();  // 确定该零件至材料左上角的偏移

        // 确定两零件的初始位置
        QPointF pos1, pos2;
        pos1 = piece.getPosition() + offset;  // 将基准零件的左上角移动到材料可排区域的左上角
        pos2 = pos1 + bestNestType.pOffset;  // 确定影子零件位置

        if(pos2.rx()-piece.getBoundingRect().width()/2 < sheetList[sheetID].layoutRect().left()){
            QPointF rightMove(sheetList[sheetID].layoutRect().left() - pos2.rx() + piece.getBoundingRect().width()/2, 0);
            pos1 += rightMove;
            pos2 += rightMove;
        }

        qreal xStep;  // 送料步距
        qreal alpha1, alpha2;
        switch (bestNestType.nestType) {
        case SingleRow:
            xStep = 2 * bestNestType.xStep;  // 送料步距
            alpha1 = alpha2 = bestNestType.alpha;  // 确定零件旋转角度
            break;
        case DoubleRow:
            xStep = bestNestType.xStep;  // 送料步距
            alpha1 = alpha2 = bestNestType.alpha;  // 确定零件旋转角度
            break;
        case OppositeSingleRow:
        case OppositeDoubleRow:
            xStep = bestNestType.xStep;
            alpha1 = bestNestType.alpha;  // 确定零件旋转角度
            alpha2 = alpha1 + 180;
            break;
        default:
            break;
        }
        qreal yStep = bestNestType.yStep;

        // 获取边界信息
        qreal xMax = sheetList[sheetID].layoutRect().right();  // 获取零件宽度最大值的x坐标
        qreal yMax = sheetList[sheetID].layoutRect().bottom();  // 获取零件高度最大值的y坐标

        // 列计数器，当列计数器为偶数时，代表为第一个零件位置，否则为影子零件位置。注意，当换行之后列计数器要清零。
        int columnCounter = 0;

        // 行计数器，当重新排放某一类型零件或更新材料时，置零
        int rowCounter = 0;

        // 排放相同类型零件
        int maxIndex = nestPieceIndexRangeMap[type].maxIndex;  // 获取该类型零件的最大值
        int pieceCounter = 0;  // 排放零件的计数器
        QPointF anchorPos;  // 记录锚点位置
        while(index <= maxIndex){
            // 确定该零件排放位置和角度，零件所占宽度及高度
            QPointF offset;  // 如果该零件不是从材料左上角开始排放，需要设置偏移量
            if(rowCounter == 0){
                offset = QPointF(availablePos1.rx()-sheetList[sheetID].layoutRect().left(),
                                 availablePos1.ry()-sheetList[sheetID].layoutRect().top());
            }
            if(rowCounter == 1){
                if(availablePos2.ry()-sheetList[sheetID].layoutRect().top() != 0){
                    pos1 += QPointF(0, availablePos2.ry()-sheetList[sheetID].layoutRect().top()-yStep);
                    pos2 += QPointF(0, availablePos2.ry()-sheetList[sheetID].layoutRect().top()-yStep);
                }
                availablePos2.ry() = sheetList[sheetID].layoutRect().top();
            }
            QPointF pos = (columnCounter % 2 == 0 ? pos1 : pos2)
                    + QPointF(xStep * int(columnCounter/2), 0)
                    + offset;
            qreal alpha = columnCounter % 2 == 0 ? alpha1 : alpha2;

            // 判断是否超出材料右边界
            qreal width = pos.rx() + 0.5 * pieceWidth;
            if(width > xMax){
                // 如果一个零件都放不下，那么该材料太小，需要重新设置材料大小
                if(columnCounter < 1){
                    qDebug() << "this sheet is pretty not enough!";
                    emit(nestException(SheetSizeError));  // 发送材料大小异常
                    return;
                }
                qDebug() << "over right" << index;
                // 如果设置尾只优化
                if((mixingTyes & NestEngine::TailPieceMixing) != NestEngine::NoMixing){
                    // 在同型体内寻找较小
                    qDebug() << "set tailPieceMix!" << endl << "searching the sameTypePiece ... " << endl;
                    // 判断是否设置了同型体列表
                    if(sameTypeNestPieceIndexMap.contains(type)){
                        QVector<int> typeIdList = sameTypeNestPieceIndexMap[type];  // 获取该类型零件的同型体
                        foreach (int id, typeIdList) {  // 遍历该列表
                            // 尝试该零件是否能排下
                            int minIndexTmp = nestPieceIndexRangeMap[id].minIndex;  // 获取该类型零件的最小值
                            int maxIndexTmp = nestPieceIndexRangeMap[id].maxIndex;  // 获取该类型零件的最小值
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

                            // 运行到这里代表已经找到了满足条件的同型体零件
                            // 尝试排放该零件，如果排放完该零件就break，否则，继续尝试排放下一个同型体

                            /**
                             * 这里尝试一个方法：
                             * 计算出前一个零件与剩余材料构成的矩形，然后在这个小的范围内尝试排放
                             */
                            qreal x, y;
                            QPointF posPre1 = ((columnCounter-1) % 2 == 0 ? pos1 : pos2)
                                    + QPointF(xStep * int((columnCounter-1)/2), 0)
                                    + offset;  // 找到前一个零件的位置
                            x = posPre1.rx() - 0.5 * pieceWidth;  // 计算前一个零件纵坐标最小值
                            y = posPre1.ry() - 0.5 * pieceHeight;  // 计算前一个零件纵坐标最小值

                            qreal sheetRemainWidth = xMax - (posPre1.rx()-0.5*pieceWidth);
                            qreal sheetRemainHeight = pieceHeight;  // 计算两组合零件外包矩形的高

                            if(columnCounter >= 2){  // 如果排放的零件多于2个，则可能错开一定高度
                                QPointF posPre2 = ((columnCounter-2) % 2 == 0 ? pos1 : pos2)
                                        + QPointF(xStep * int((columnCounter-2)/2), 0)
                                        + offset;  // 找到前前一个零件的位置
                                qreal y1 = posPre2.ry() - 0.5 * pieceHeight;
                                qreal y2 = posPre2.ry() + 0.5 * pieceHeight;
                                if(y1 < y){
                                    y = y1;
                                }
                                sheetRemainHeight = y2 - y;
                            }

                            QRectF testRect(x, y, sheetRemainWidth, sheetRemainHeight);  // 这个为尾只零件测试区域
                            qDebug() << testRect << endl;
                            emit nestDebugRemainRect(testRect);

                            // 确定初始位置
                            Piece p = pieceList[id];  // 复制该零件
                            QRectF minBR = p.getMinBoundingRect();
                            // 先判断是否会超出材料，如果超出，则继续寻找下一个同型体，否则就将零件排放在此
                            if(x+pieceWidth+minBR.width() > xMax){
                                qDebug() << "tailMix over right" << endl;
                                break;
                            }

                            p.moveTo(QPointF(0, 0));  // 移至零点
                            QPointF c = p.getPosition();  // 获取零件初始位置
                            qreal angle = p.getAngle();  // 该零件拥有最小外包矩形时对应旋转的角度
                            p.rotate(c, angle);  // 旋转

                            minBR = p.getMinBoundingRect();  // 该零件最小外包矩形
                            QPointF topLeft(x+pieceWidth, y);  // 基准位置左上角坐标
                            QPointF offset = topLeft - minBR.topLeft();  // 计算偏移量
                            QPointF newCenter = c + offset;  // 得到新的位置
                            p.moveTo(newCenter); // 移动至新位置

                            Piece p1 = piece;
                            p1.moveTo(posPre1);
                            p1.rotate(posPre1, columnCounter % 2 == 0 ? alpha1 : alpha2);

                            qreal step = p.compactToOnHD(p1, compactStep, compactAccuracy);
                            if(step == 0){
                                qDebug() << "compact failed" << endl;
                            }

                            nestPieceList[chooseIndex].sheetID = sheetID;  // 记录下一零件排放材料id
                            nestPieceList[chooseIndex].position = p.getPosition();  // 记录下一零件排放位置
                            nestPieceList[chooseIndex].alpha = p.getAngle();  // 记录下一零件旋转角度
                            nestPieceList[chooseIndex].nested = true;  // 设置已排
                            nestedList.append(chooseIndex);  // 将该零件加入已排列表
                        }
                    }
                }

                // 换行，设置偏移量
                pos1 += QPointF(0, yStep);
                pos2 += QPointF(0, yStep);
                columnCounter = 0;
                rowCounter++;
                continue;
            }

            // 判断是否超出材料下边界
            qreal height = pos.ry() + 0.5 * pieceHeight;
            if(height > yMax){
                qDebug() << "over bottom" << index;
                sheetAvailable = false;
                if((mixingTyes & NestEngine::TailLineMixing) == NestEngine::NoMixing){  // 如果没有设置尾行优化
                    // 换材料
                    if(autoRepeatLastSheet){  // 如果材料不够了，必须退出，继续大循环
                        Sheet sheet = sheetList[sheetID];
                        sheetList.append(sheet);
                        initQuadTreeMap(sheetID+1);  // 初始化该张材料的四叉树
                        sheetID++;
                        emit autoRepeatedLastSheet(sheet);  // 排版结束后发送 重复了最后一张材料
                        break;
                    }
                } else{
                    // 尾行优化，即在给定高度的材料内进行
                }
            }

            nestPieceList[index].sheetID = sheetID;  // 记录下一零件排放材料id
            nestPieceList[index].position = pos;  // 记录下一零件排放位置
            nestPieceList[index].alpha = alpha;  // 记录下一零件旋转角度
            nestPieceList[index].nested = true;  // 设置已排
            nestedList.append(index);  // 将该零件加入已排列表
            anchorPos = QPointF(width, height);  // 记录排放完该零件的右下角位置，即锚点

            columnCounter++;  // 计数器
            if(++pieceCounter >= unnestedList.length()){  // 如果计数器超出排放零件总个数，则退出
                break;
            }
            index = unnestedList.at(pieceCounter);  // 更新下一零件index
        }

        // 更新未排列表，在未排列表中删除已排列表
        foreach (int index, nestedList) {
            unnestedList.removeOne(index);
        }

        // 如果排完某类零件，材料仍有剩余时，则需要记录剩余材料的起始位置
        if(sheetAvailable){
            availablePos1 = QPointF(anchorPos.rx(), anchorPos.ry()-yStep);
            availablePos2 = QPointF(sheetList[sheetID].layoutRect().left(), anchorPos.ry());
            emit(nestDebug(availablePos1, availablePos2));
        } else{
            availablePos1 = availablePos2 = sheetList[sheetID].layoutRect().topLeft();
        }
    }

    // 发送排放结束信号
    emit(nestFinished(nestPieceList));
}

bool ContinueNestEngine::packOnePiece(Piece piece, NestEngine::NestPiece &nestPiece)
{

}

bool ContinueNestEngine::packOnePieceOnSheet(Piece piece, int sheetID, NestEngine::NestPiece &nestPiece)
{

}

bool ContinueNestEngine::compact(int sheetID, NestEngine::NestPiece &nestPiece)
{

}

bool ContinueNestEngine::collidesWithOtherPieces(int sheetID, Piece piece)
{

}

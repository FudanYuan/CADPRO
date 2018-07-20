#include "piece.h"

Piece::Piece() :
    pointsList(QVector<QPointF>()),
    area(0),
    minBoundingRect(QRectF()),
    angle(0),
    squareness(0),
    centerPoint(QPointF()),
    count(0),
    precision(6),
    transpose(false)
{
}

Piece::Piece(Polyline *p, int n, short i) :
    transpose(false)
{
    QVector<QPointF> points = p->getPoints();
    if(points.length() < 3){
        return;
    }
    pointsList = pointsListPrecision(points, i);  // 读入边时，保留i位小数位
    // referenceLine = linesListPrecision(p->getReferenceLines(), i);  // 读入参考线
    area = calculatePolygonArea(pointsList);  // 计算多边形面积
    qreal minBoundingRectArea = calculatePloygonMinBoundingRectArea(pointsList, angle, minBoundingRect);
    minBoundingRect = rectPrecision(minBoundingRect, i);  // 边缘矩形，保留i位小数位
    squareness = qrealPrecision(area / minBoundingRectArea, i);  // 计算方正度，保留i位小数位
    centerPoint = pointPrecision(calculatePolygonGravityCenter(pointsList), i); // 计算多边形的质心，保留i位小数位
    count = n;
    precision = i;
}

Piece::Piece(QVector<QPointF> points, int n, short i) :
    transpose(false)
{
    if(points.length() < 3){
        return;
    }
    pointsList = pointsListPrecision(points, i);  // 读入边时，保留i位小数位
    area = calculatePolygonArea(pointsList);  // 计算多边形面积
    qreal minBoundingRectArea = calculatePloygonMinBoundingRectArea(pointsList, angle, minBoundingRect);
    minBoundingRect = rectPrecision(minBoundingRect, i);  // 边缘矩形，保留i位小数位
    squareness = qrealPrecision(area / minBoundingRectArea, i);  // 计算方正度，保留i位小数位
    centerPoint = pointPrecision(calculatePolygonGravityCenter(pointsList), i); // 计算多边形的质心，保留i位小数位
    count = n;
    precision = i;
}

Piece::Piece(QVector<QPointF> points, QVector<QLineF> lines, int n, short i) :
    Piece(points, n, i)
{
    if(lines.length() < 1){
        return;
    }
    // 对参考线进行旋转，角度为angle
    if(qrealPrecision(angle, precision) != 0.0){
        for(int i=0; i<lines.length(); i++){
            QPointF p1 = lines[i].p1();
            QPointF p2 = lines[i].p2();
            p1 = transformRotate(QPointF(0,0), p1, angle);  // 旋转之后的点
            p2 = transformRotate(QPointF(0,0), p2, angle);  // 旋转之后的点
            lines[i].setP1(p1);
            lines[i].setP2(p2);
        }
    }

    referenceLines = linesListPrecision(lines, i);
}

Polyline *Piece::getPolyline()
{
    Polyline *polyline = new Polyline;
    polyline->setPolyline(pointsList, Polyline::line);
    return polyline;
}

Object *Piece::toObject()
{
    return new Object(minBoundingRect.topLeft().rx(),
                      minBoundingRect.topLeft().ry(),
                      minBoundingRect.width(),
                      minBoundingRect.height());
}

QVector<QPointF> &Piece::getPointsList()
{
    return pointsList;
}

QVector<QLineF> Piece::getReferenceLinesList()
{
    return referenceLines;
}

qreal Piece::getArea() const
{
    return qrealPrecision(area, precision);
}

QRectF Piece::getMinBoundingRect() const
{
    return rectPrecision(minBoundingRect, precision);
}

QPointF Piece::getPosition() const
{
    return pointPrecision(minBoundingRect.center(), precision);
}

void Piece::setAngle(const int angle)
{
    this->angle = angle;
}

qreal Piece::getAngle() const
{
    return qrealPrecision(angle, precision);
}

qreal Piece::getSquareness() const
{
    return qrealPrecision(squareness, precision);
}

QPointF Piece::getCenterPoint() const
{
    return pointPrecision(centerPoint, precision);
}

void Piece::setCount(const int c)
{
    count = c;
}

int Piece::getCount() const
{
    // return count;
    return 10;
}

QVector<QPointF> Piece::getOffset()
{
    QVector<QPointF> offsetList;
    for(int i=0; i<pointsList.length(); i++){
        offsetList.append(pointsList[i] - minBoundingRect.center());
    }
    return offsetList;
}

void Piece::setPrecision(const short i)
{
    precision = i;
}

short Piece::getPrecision() const
{
    return precision;
}

bool Piece::isHorizontal() const
{
    return minBoundingRect.width() > minBoundingRect.height();
}

void Piece::setTranspose(const bool transpose)
{
    this->transpose = transpose;
}

bool Piece::isTranspose() const
{
    return this->transpose;
}

QPointF Piece::refLineCenterToMinBoundRectCenter() const
{
    return minBoundingRect.center() - referenceLines[0].center();
}

void Piece::moveTo(const QPointF position)
{
    QPointF offset = position - minBoundingRect.center();  // 偏移量
    offset = pointPrecision(offset, precision);  // 保留i位小数位
    // 更新多边形点集
    QVector<QPointF> oldPointsList = pointsList;  // 多边形点集
    QVector<QPointF> newPointsList;  // 存储移动之后的点坐标
    for(int i=0; i<oldPointsList.length(); i++){
        QPointF oldPoint = oldPointsList[i];  // 初始点
        QPointF newPoint = oldPoint + offset;  // 移动之后的点
        newPointsList.append(newPoint);
    }
    pointsList = newPointsList;  // 更新点集

    // 更新参考线集
    QVector<QLineF> oldLinesList = referenceLines;  // 参考线集
    QVector<QLineF> newLinesList;  // 存储移动之后的线
    for(int i=0; i<oldLinesList.length(); i++){
        QLineF oldLine = oldLinesList[i];  // 初始线
        QLineF newLine(oldLine.p1()+offset, oldLine.p2()+offset);  // 移动之后的线
        newLinesList.append(newLine);
    }
    referenceLines = newLinesList;  // 更新参考线集

    centerPoint += offset; // 更新多边形质心
    // 更新最小包络矩形
    minBoundingRect.moveCenter(position);
}

void Piece::moveToByReferenceLine(const QPointF position)
{
    if(referenceLines.length() == 0){
        qDebug() << "无参考线";
        return;
    }
    // 有多条参考线，这里首先默认第一条
    QLineF line = referenceLines[0];
    QPointF offset = position - line.center();  // 偏移量
    offset = pointPrecision(offset, precision);  // 保留i位小数位
    // 更新多边形点集
    QVector<QPointF> oldPointsList = pointsList;  // 多边形点集
    QVector<QPointF> newPointsList;  // 存储移动之后的点坐标
    for(int i=0; i<oldPointsList.length(); i++){
        QPointF oldPoint = oldPointsList[i];  // 初始点
        QPointF newPoint = oldPoint + offset;  // 移动之后的点
        newPointsList.append(newPoint);
    }
    pointsList = newPointsList;  // 更新点集

    // 更新参考线集
    QVector<QLineF> oldLinesList = referenceLines;  // 参考线集
    QVector<QLineF> newLinesList;  // 存储移动之后的线
    for(int i=0; i<oldLinesList.length(); i++){
        QLineF oldLine = oldLinesList[i];  // 初始线
        QLineF newLine(oldLine.p1()+offset, oldLine.p2()+offset);  // 移动之后的线
        newLinesList.append(newLine);
    }
    referenceLines = newLinesList;  // 更新参考线集
    centerPoint += offset; // 更新多边形质心
    // 更新最小包络矩形
    minBoundingRect.moveCenter(position);
}

void Piece::rotate(const QPointF cPoint, const qreal alpha)
{
    if(qrealPrecision(alpha, 0) == 0.0 || qrealPrecision(alpha, 0) == 360.0){
        return;
    }
    // 更新多边形点集
    QVector<QPointF> oldPointsList = pointsList;  // 多边形点集
    QVector<QPointF> newPointsList;  // 存储移动之后的点坐标
    for(int i=0; i<oldPointsList.length(); i++){
        QPointF oldPoint = oldPointsList[i];  // 初始点
        QPointF newPoint = transformRotate(cPoint, oldPoint, alpha);  // 移动之后的点
        newPoint = pointPrecision(newPoint, precision);  // 保留i位小数位
        newPointsList.append(newPoint);
    }
    pointsList = newPointsList;  // 更新点集

    // 更新参考线集
    QVector<QLineF> oldLinesList = referenceLines;  // 参考线集
    QVector<QLineF> newLinesList;  // 存储移动之后的线
    for(int i=0; i<oldLinesList.length(); i++){
        QLineF oldLine = oldLinesList[i];  // 初始线

        QPointF p1 = oldLine.p1();
        QPointF p2 = oldLine.p2();
        p1 = transformRotate(cPoint, p1, alpha);  // 旋转之后的点
        p2 = transformRotate(cPoint, p2, alpha);  // 旋转之后的点

        QLineF newLine(p1, p2);  // 移动之后的线

        newLine = linePrecision(newLine, precision);  // 保留i位小数位
        newLinesList.append(newLine);
    }
    referenceLines = newLinesList;  // 更新参考线集

    centerPoint = pointPrecision(transformRotate(cPoint, centerPoint, alpha), precision); // 更新多边形质心
    minBoundingRect = rectPrecision(calculatePolygonBoundingRect(pointsList), precision);  // 更新最小包络矩形
    squareness = area / (minBoundingRect.width() * minBoundingRect.height());  // 计算方正度
    squareness = qrealPrecision(squareness, precision);  // 保留i位小数位
    angle += alpha;  // 该图形对应最小矩形顺时针转angle度
}

void Piece::rotateByReferenceLine(const QPointF cPoint, bool flag)
{
    if(referenceLines.length() == 0){
        qDebug() << "无参考线";
        return;
    }

    // 使参考线处于垂直方向
    QLineF packLine;
    if(flag){
        packLine.setLine(0, 0, 0, 10);
    } else{
        packLine.setLine(0, 0, 0, -10);
    }
    qreal theta = referenceLines[0].angleTo(packLine);
    qDebug() << packLine <<  " " << referenceLines[0] << " 参考线垂直角度需旋转" << theta;
    // 更新多边形点集
    QVector<QPointF> oldPointsList = pointsList;  // 多边形点集
    QVector<QPointF> newPointsList;  // 存储移动之后的点坐标
    for(int i=0; i<oldPointsList.length(); i++){
        QPointF oldPoint = oldPointsList[i];  // 初始点
        QPointF newPoint = transformRotate(cPoint, oldPoint, theta);  // 移动之后的点
        newPoint = pointPrecision(newPoint, precision);  // 保留i位小数位
        newPointsList.append(newPoint);
    }
    pointsList = newPointsList;  // 更新点集

    // 更新参考线集
    QVector<QLineF> oldLinesList = referenceLines;  // 参考线集
    QVector<QLineF> newLinesList;  // 存储移动之后的线
    for(int i=0; i<oldLinesList.length(); i++){
        QLineF oldLine = oldLinesList[i];  // 初始线

        QPointF p1 = oldLine.p1();
        QPointF p2 = oldLine.p2();
        p1 = transformRotate(cPoint, p1, theta);  // 旋转之后的点
        p2 = transformRotate(cPoint, p2, theta);  // 旋转之后的点

        QLineF newLine(p1, p2);  // 移动之后的线

        newLine = linePrecision(newLine, precision);  // 保留i位小数位
        newLinesList.append(newLine);
    }
    referenceLines = newLinesList;  // 更新参考线集

    centerPoint = pointPrecision(transformRotate(cPoint, centerPoint, theta), precision); // 更新多边形质心
    minBoundingRect = rectPrecision(calculatePolygonBoundingRect(pointsList), precision);  // 更新最小包络矩形
    squareness = area / (minBoundingRect.width() * minBoundingRect.height());  // 计算方正度
    squareness = qrealPrecision(squareness, precision);  // 保留i位小数位
    angle += theta;  // 该图形对应最小矩形顺时针转angle度
    qDebug() << "angle: " << angle;
}

bool Piece::hasRelationToPoint(const QPointF &point)
{
    PointRealtionToPiece relation = relationToPoint(point);
    if(relation == Outside){
        return false;
    }
    return true;
}

Piece::PointRealtionToPiece Piece::relationToPoint(const QPointF &point)
{
    if(!inMinBoundingRect(point)){
        //qDebug() << "不在包络矩形内";
        return Outside;
    }
    if(onBoundary(point)){
        //qDebug() << "在边界上";
        return OnBoundary;
    }
    if(contains(point)){
        //qDebug() << "包含该点";
        return Inside;
    }
    //qDebug() << "在包络矩形内, 但不包含该点";
    return Outside;
}

bool Piece::inMinBoundingRect(const QPointF &point)
{
    qreal minX, minY, maxX, maxY;
    getRectBoundValue(minBoundingRect, minX, minY, maxX, maxY);
    qreal x = point.x();
    qreal y = point.y();
    if(x < minX || x > maxX || y < minY || y > maxY){
        return false;
    }
    return true;
}

bool Piece::onBoundary(const QPointF &point)
{
    return pointOnPolygonBoundary(pointsList, point);
}

bool Piece::contains(const QPointF &point)
{
    return pointContainsInPolygon(pointsList, point);
}

bool Piece::containsInSheet(const Sheet &sheet)
{
    // 获取材料的排版区域
    QRectF layoutRect = sheet.layoutRect();
    return boundingRectContain(layoutRect, minBoundingRect);
}

bool Piece::collidesWithPiece(Piece piece, const Piece::CollisionsMode mode)
{
    /***
     * 判断零件与其他零件是否重叠，
     * 1. 首先判断两多边形的外包矩形是否重叠，如果不重叠，则两多边形一定不会重叠
     * 2. 如果两矩形框重叠，再去进一步判断两多边形是否重叠
     */
    QRectF minBoundingRect1 = piece.getMinBoundingRect();
    //qDebug() << minBoundingRect << "  " << minBoundingRect1;
    if(boundingRectSeperate(minBoundingRect, minBoundingRect1)){
        //qDebug() << "边缘矩形不碰撞，结束判断";
        return false;
    }
    // 如果模式为边缘矩形的碰撞，则返回
    if(mode == BoundingRectCollisionMode){
        //qDebug() << "边缘矩形碰撞";
        return true;
    }
    CollisionDectect collisionDectect(pointsList, piece.getPointsList());
    return collisionDectect.collision();
}

qreal Piece::compactToOnHD(Piece p, qreal compactStep, qreal compactAccuracy)
{
    // 水平方向靠接
    qreal stepX = compactStep;  // 默认移动的单位距离
    QPointF pos = getPosition();  // 获取零件质心,该点为零件初始位置
    QPointF posOld = pos;  // 记录之前的位置
    //qDebug() << "初始位置: " << pos;
    while(stepX > compactAccuracy){
        QPointF posTemp = pos;
        QPointF forwardPos(posTemp.rx()-stepX, posTemp.ry());  // 水平方向移动
        //qDebug() << "移动至：" << forwardPos;
        moveTo(forwardPos);  // 将零件移至新位置
        if(collidesWithPiece(p)){
            // 往右移
            stepX /= 2;
            continue;
        }
        pos.setX(pos.rx() - stepX);
    }
    qreal step = posOld.rx() - pos.rx();
    return step;
}

qreal Piece::compactToOnVD(Piece p, qreal compactStep, qreal compactAccuracy)
{
    // 重力方向靠接
    qreal stepY = compactStep;  // 默认移动的单位距离
    QPointF pos = getPosition();  // 获取零件质心,该点为零件初始位置
    QPointF posOld = pos;  // 记录之前的位置
    //qDebug() << "初始位置: " << pos;
    while(stepY > compactAccuracy){
        QPointF posTemp = pos;
        QPointF forwardPos(posTemp.rx(), posTemp.ry()-stepY);  // 重力反方向移动
        //qDebug() << "移动至：" << forwardPos;
        moveTo(forwardPos);  // 将零件移至前进位置
        if(collidesWithPiece(p)){
            // 往下移
            stepY /= 2;
            continue;
        }
        pos.setY(pos.ry() - stepY);
    }
    qreal step = posOld.ry() - pos.ry();
    return step;
}

QPointF Piece::compactToOnAlpha(Piece p, qreal alpha, qreal compactStep, qreal compactAccuracy)
{
    // alpha方向靠接
    qreal step = compactStep;  // 默认移动的单位距离
    QPointF pos = getPosition();  // 获取零件质心,该点为零件初始位置
    QPointF posOld = pos;  // 记录之前的位置
    qDebug() << "初始位置: " << pos;
    while(step > compactAccuracy){
        if(pos.ry() < p.getMinBoundingRect().top() - p.getMinBoundingRect().height()
                || pos.rx() < p.getMinBoundingRect().right() - p.getMinBoundingRect().width()){
            break;
        }
        qreal stepX = qAbs(step * qCos(alpha));  // 水平方向偏移量
        qreal stepY = step * qSin(alpha);  // 垂直方向偏移量

        qDebug() << "step: " << step;
        qDebug() << "stepX: " << stepX;
        qDebug() << "stepY: " << stepY;
        QPointF posTemp = pos;
        QPointF forwardPos(posTemp.rx()-stepX, posTemp.ry()-stepY);  // 水平方向移动
        qDebug() << "移动至：" << forwardPos;
        moveTo(forwardPos);  // 将零件移至新位置
        if(collidesWithPiece(p)){
            // 往右移
            qDebug() << "碰撞";
            step /= 2;
            continue;
        }
        pos.setX(pos.rx() - stepX);
        pos.setY(pos.ry() - stepY);
    }
    qDebug() << "返回偏移：" << posOld - pos;
    return posOld - pos;
}

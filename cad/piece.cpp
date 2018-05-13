#include "piece.h"

Piece::Piece() :
    pointsList(QVector<QPointF>()),
    area(0),
    minBoundingRect(QRectF()),
    angle(0),
    squareness(0),
    centerPoint(QPointF()),
    count(0),
    precision(6)
{
}

Piece::Piece(Polyline *p, int n, short i)
{
    pointsList = pointsListPrecision(p->getPoints(), i);  // 读入边时，保留i位小数位
    area = calculatePolygonArea(pointsList);  // 计算多边形面积
    qreal minBoundingRectArea = calculatePloygonMinBoundingRectArea(pointsList, angle, minBoundingRect);
    minBoundingRect = rectPrecision(minBoundingRect, i);  // 边缘矩形，保留i位小数位
    squareness = qrealPrecision(area / minBoundingRectArea, i);  // 计算方正度，保留i位小数位
    centerPoint = pointPrecision(calculatePolygonGravityCenter(pointsList), i); // 计算多边形的质心，保留i位小数位
    count = n;
    precision = i;
}

Piece::Piece(QVector<QPointF> points, int n, short i)
{
    pointsList = pointsListPrecision(points, i);  // 读入边时，保留i位小数位
    area = calculatePolygonArea(pointsList);  // 计算多边形面积
    qreal minBoundingRectArea = calculatePloygonMinBoundingRectArea(pointsList, angle, minBoundingRect);
    minBoundingRect = rectPrecision(minBoundingRect, i);  // 边缘矩形，保留i位小数位
    squareness = qrealPrecision(area / minBoundingRectArea, i);  // 计算方正度，保留i位小数位
    centerPoint = pointPrecision(calculatePolygonGravityCenter(pointsList), i); // 计算多边形的质心，保留i位小数位
    count = n;
    precision = i;
}

Polyline *Piece::getPolyline()
{
    Polyline *polyline = new Polyline;
    polyline->setPolyline(pointsList, Polyline::line);
    return polyline;
}

QVector<QPointF> &Piece::getPointsList()
{
    return pointsList;
}

qreal Piece::getArea() const
{
    return qrealPrecision(area, precision);
}

QRectF Piece::getMinBoundingRect() const
{
    return rectPrecision(minBoundingRect, precision);
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

int Piece::getCount() const
{
    return count;
}

void Piece::setPrecision(short i)
{
    precision = i;
}

short Piece::getPrecision() const
{
    return precision;
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
    centerPoint = pointPrecision(transformRotate(cPoint, centerPoint, alpha), precision); // 更新多边形质心
    minBoundingRect = rectPrecision(calculatePolygonBoundingRect(pointsList), precision);  // 更新最小包络矩形
    squareness = area / (minBoundingRect.width() * minBoundingRect.height());  // 计算方正度
    squareness = qrealPrecision(squareness, precision);  // 保留i位小数位
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
    // qDebug() << minBoundingRect << "  " << minBoundingRect1;
    if(boundingRectSeperate(minBoundingRect, minBoundingRect1)){
        // qDebug() << "边缘矩形不碰撞，结束判断";
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

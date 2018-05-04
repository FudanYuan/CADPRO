#include "piece.h"

Piece::Piece() :
    pointsList(QVector<QPointF>()),
    area(0),
    minBoundingRect(QRectF()),
    angle(0),
    squareness(0),
    centerPoint(QPointF()),
    count(0)
{
}

Piece::Piece(Polyline *p, int n)
{
    pointsList = p->getPoints();
    area = calculatePolygonArea(pointsList);  // 计算多边形面积
    qreal minBoundingRectArea = calculatePloygonMinBoundingRectArea(pointsList, angle, minBoundingRect);
    squareness = area / minBoundingRectArea;  // 计算方正度
    centerPoint = calculatePolygonGravityCenter(pointsList); // 计算多边形的质心
    count = n;
}

Piece::Piece(QVector<QPointF> points, int n)
{
    pointsList = points;
    area = calculatePolygonArea(pointsList);  // 计算多边形面积
    qreal minBoundingRectArea = calculatePloygonMinBoundingRectArea(pointsList, angle, minBoundingRect);
    squareness = area / minBoundingRectArea;  // 计算方正度
    centerPoint = calculatePolygonGravityCenter(pointsList); // 计算多边形的质心
    count = n;
}

Polyline *Piece::getPolyline()
{
    Polyline *polyline = new Polyline;
    polyline->setPolyline(pointsList, Polyline::line);
    return polyline;
}

QVector<QPointF> Piece::getPointsList()
{
    return pointsList;
}

qreal Piece::getArea() const
{
    return area;
}

QRectF Piece::getMinBoundingRect() const
{
    return minBoundingRect;
}

qreal Piece::getAngle() const
{
    return angle;
}

qreal Piece::getSquareness() const
{
    return squareness;
}

QPointF Piece::getCenterPoint() const
{
    return centerPoint;
}

int Piece::getCount() const
{
    return count;
}

void Piece::moveTo(const QPointF position)
{
    QPointF offset = position - minBoundingRect.center();  // 偏移量
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
    // 更新多边形点集
    QVector<QPointF> oldPointsList = pointsList;  // 多边形点集
    QVector<QPointF> newPointsList;  // 存储移动之后的点坐标
    for(int i=0; i<oldPointsList.length(); i++){
        QPointF oldPoint = oldPointsList[i];  // 初始点
        QPointF newPoint = transformRotate(cPoint, oldPoint, alpha);  // 移动之后的点
        newPointsList.append(newPoint);
    }
    pointsList = newPointsList;  // 更新点集
    centerPoint = transformRotate(cPoint, centerPoint, alpha); ; // 更新多边形质心
    // 更新最小包络矩形
    minBoundingRect = calculatePolygonBoundingRect(pointsList);
    squareness = area / (minBoundingRect.width() * minBoundingRect.height());  // 计算方正度
}

bool Piece::contains(const QPointF point)
{
    qreal sigma = 0;
    for(int i=0; i<pointsList.length()-1; i++){
        QPointF p1 = pointsList[i];
        QPointF p2 = pointsList[i+1];
        QLineF v1(point, p1);  // 向量v1
        QLineF v2(point, p2);  // 向量v2
        qreal theta = v1.angleTo(v2);  // 获取两向量夹角
        if(theta > 180){
            theta -= 360;
        }
        sigma += theta;
    }
    return sigma == 0 ? false : true;
}

bool Piece::containsInSheet(const Sheet sheet)
{
    // 获取材料的排版区域
    QRectF layoutRect = sheet.layoutRect();
    QPointF topLeftPoint = layoutRect.topLeft();  // 材料排版区域左上角坐标
    QPointF bottomRightPoint = layoutRect.bottomRight();  // 材料排版区域右下角坐标
    // 获取边界值
    qreal minX = topLeftPoint.rx();
    qreal minY = topLeftPoint.ry();
    qreal maxX = bottomRightPoint.rx();
    qreal maxY = bottomRightPoint.ry();
    // 遍历点集，判断每个点的坐标是否越界
    for(int i=0; i<pointsList.length(); i++){
        QPointF point = pointsList[i];
        qreal xPos = point.rx();
        qreal yPos = point.ry();
        if(xPos < minX){
            return false;
        }
        if(yPos < minY){
            return false;
        }
        if(xPos > maxX){
            return false;
        }
        if(yPos > maxY){
            return false;
        }
    }
    return true;
}

bool Piece::collidesWithPiece(Piece piece, const Piece::CollisionsMode mode)
{
    /***
     * 判断零件与其他零件是否重叠，
     * 1. 首先判断两多边形的外包矩形是否重叠，如果不重叠，则两多边形一定不会重叠
     * 2. 如果两矩形框重叠，再去进一步判断两多边形是否重叠
     */
    QRectF minBoundingRect1 = piece.getMinBoundingRect();
    if(boundingRectSeperate(minBoundingRect, minBoundingRect1)){
        return false;
    }
    // 如果模式为边缘矩形的碰撞，则返回
    if(mode == BoundingRectCollisionMode){
        return true;
    }
    CollisionDectect collisionDectect(pointsList, piece.getPointsList());
    return collisionDectect.collision();
}

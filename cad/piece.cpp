#include "piece.h"

Piece::Piece() :
    minBoundingRect(QRectF()),
    count(0)
{
}

Piece::Piece(Polyline *p, int n)
{
    polyline = p;
    pointsList = p->getPoints();
    // area = calculatePolylineArea(pointsList);  // 计算多边形面积
    minBoundingRect = p->boundingRect();  // 改成最小包络矩形
    // qreal rectArea = minBoundingRect.width() * minBoundingRect.height();  // 矩形面积
    // squareness = area / rectArea;  // 计算方正度
    // centerPoint = calculatePolylineCenterPoint(pointsList); // 计算多边形的质心
    count = n;
}

Piece::Piece(QList<QPointF> points, int n)
{
    Polyline *p = new Polyline;
    polyline->setPolyline(points, Polyline::line);
    pointsList = points;
    // area = calculatePolylineArea(pointsList);
    // minBoundingRect = calculatePolylineMinBoundingRect(pointsList);  // 改成最小包络矩形
    // qreal rectArea = minBoundingRect.width() * minBoundingRect.height();  // 矩形面积
    // squareness = area / rectArea;  // 计算方正度
    // centerPoint = calculatePolylineCenterPoint(pointsList); // 计算多边形的质心
    count = n;
}

Polyline *Piece::getPolyline()
{
    return polyline;
}

QList<QPointF> Piece::getPointsList()
{
    return pointsList;
}

qreal Piece::getArea()
{
    return area;
}

QRectF Piece::getMinBoundingRect()
{
    return minBoundingRect;
}

qreal Piece::getAngle()
{
    return angle;
}

qreal Piece::getSquareness()
{
    return squareness;
}

QPointF Piece::getCenterPoint()
{
    return centerPoint;
}

int Piece::getCount()
{
    return count;
}

void Piece::moveTo(const QPointF position)
{
    QPointF offset = position - minBoundingRect.center();  // 偏移量
    // 更新多边形点集
    QList<QPointF> oldPointsList = polyline->getPoints();  // 多边形点集
    QList<QPointF> newPointsList;  // 存储移动之后的点坐标
    for(int i=0; i<oldPointsList.length(); i++){
        QPointF oldPoint = oldPointsList[i];  // 初始点
        QPointF newPoint = oldPoint + offset;  // 移动之后的点
        newPointsList.append(newPoint);
    }
    polyline->setPoints(newPointsList);  // 更新多边形点集
    centerPoint += offset; // 更新多边形质心
    // 更新最小包络矩形
    minBoundingRect.moveCenter(position);
}

void Piece::rotate(const QPointF cPoint, const qreal alpha)
{
    // 更新多边形点集
    QList<QPointF> oldPointsList = polyline->getPoints();  // 多边形点集
    QList<QPointF> newPointsList;  // 存储移动之后的点坐标
    for(int i=0; i<oldPointsList.length(); i++){
        QPointF oldPoint = oldPointsList[i];  // 初始点
        QPointF newPoint = transformRotate(cPoint, oldPoint, alpha);  // 移动之后的点
        newPointsList.append(newPoint);
    }
    polyline->setPoints(newPointsList);  // 更新多边形点集
    centerPoint = transformRotate(cPoint, centerPoint, alpha); ; // 更新多边形质心
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

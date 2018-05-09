#ifndef PIECE_H
#define PIECE_H

#include <QObject>
#include <polyline.h>
#include <sheet.h>
#include "collisiondectect.h"

class Piece
{
public:
    enum CollisionsMode{
        BoundingRectCollisionMode,
        ShapeCollisionMode
    };

    enum PointRealtionToPiece{
        Outside,
        Inside,
        OnBoundary
    };

    Piece();
    Piece(Polyline *p, int n=1, short i=6);
    Piece(QVector<QPointF> points, int n=1, short i=6);

    Polyline* getPolyline();
    QVector<QPointF> &getPointsList();
    qreal getArea() const;
    QRectF getMinBoundingRect() const;
    qreal getAngle() const;
    qreal getSquareness() const;
    QPointF getCenterPoint() const;
    int getCount() const;

    void setPrecision(short i);
    short getPrecision() const;

    void moveTo(const QPointF position);  // 移动零件至给定位置
    void rotate(const QPointF cPoint, const qreal alpha);  // 将零件旋转alpha度
    bool hasRelationToPoint(const QPointF &point);  // 点与零件有关系
    PointRealtionToPiece relationToPoint(const QPointF &point);  // 返回点与零件的关系
    bool inMinBoundingRect(const QPointF &point);  // 点在零件的最小包络矩形范围内
    bool contains(const QPointF &point);  // 判断零件是否包含某点
    bool onBoundary(const QPointF &point);  // 判断点是否在零件的边上
    bool containsInSheet(const Sheet &sheet);  // 判断该零件是否在材料内部
    bool collidesWithPiece(Piece piece, const CollisionsMode mode = ShapeCollisionMode);  // 判断该零件是否与给定零件碰撞

//private:
    QVector<QPointF> pointsList;  // 多边形点集
    qreal area;  // 零件面积
    QRectF minBoundingRect;  // 零件对应的最小包络矩形,其中心为参考点
    qreal angle;  // 零件最小包络矩形对应多变形顺时针旋转angle之后的外包矩形
    qreal squareness;  // 方正度
    QPointF centerPoint;  // 零件的质心
    int count;  // 零件个数
    short precision;  // 小数保留位数
};

#endif // PIECE_H

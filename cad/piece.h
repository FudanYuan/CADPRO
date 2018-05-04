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

    Piece();
    Piece(Polyline *p, int n=1);
    Piece(QVector<QPointF> points, int n=1);

    Polyline* getPolyline();
    QVector<QPointF> getPointsList();
    qreal getArea() const;
    QRectF getMinBoundingRect() const;
    qreal getAngle() const;
    qreal getSquareness() const;
    QPointF getCenterPoint() const;
    int getCount() const;

    void moveTo(const QPointF position);  // 移动零件至给定位置
    void rotate(const QPointF cPoint, const qreal alpha);  // 将零件旋转alpha度

    bool contains(const QPointF point);  // 判断零件是否包含某点
    bool containsInSheet(const Sheet sheet);  // 判断该零件是否在材料内部
    bool collidesWithPiece(Piece piece, const CollisionsMode mode = ShapeCollisionMode);  // 判断该零件是否与给定零件碰撞

private:
    QVector<QPointF> pointsList;  // 多边形点集
    qreal area;  // 零件面积
    QRectF minBoundingRect;  // 零件对应的最小包络矩形,其中心为参考点
    qreal angle;  // 零件最小包络矩形对应多变形顺时针旋转angle之后的外包矩形
    qreal squareness;  // 方正度
    QPointF centerPoint;  // 零件的质心
    int count;  // 零件个数
};

#endif // PIECE_H

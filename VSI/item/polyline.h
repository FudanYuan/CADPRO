#ifndef POLYLINE_H
#define POLYLINE_H

#include <QGraphicsPathItem>
#include <QGraphicsSceneMouseEvent>
#include "shape.h"
#include <QPointF>

//!
//! 折线类，所有的多边形、曲线、
//! \brief The Polyline class
//!
class Polyline : public Shape, public QGraphicsPathItem
{
    Q_OBJECT
public:
    enum Type{
        none = 0,
        line = 1,
        curve = 2,
        cubic = 3
    };
    Polyline(QGraphicsItem *parent=0);
    void startDraw(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 开始绘图
    void drawing(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 绘图开始
    bool updateFlag(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE; // paint
//    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;

    bool collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode = Qt::IntersectsItemShape) const Q_DECL_OVERRIDE;

    void setPolyline(QVector<QPointF> pList, int flag=1, qreal ele=0, qreal angle=0, const QPointF off=QPointF());

    void setPoints(const QVector<QPointF> &value);
    QVector<QPointF> getPoints();  // 获取点

    void setType(Type type);  // 设置类型
    Type getType();  // 获取类型

    void setElevation(qreal elevation);  // 设置高程
    qreal getElevation();  // 获取高程

    void setAlpha(const qreal alpha);  //设置旋转角度
    qreal getAlpha() const;  // 获取旋转角度

    QRectF getBoundingRect();  // 获取包围矩形

    Polyline *copy();

    int i;

protected:
    //鼠标事件
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    //拖拽事件
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;

private:
    QVector<QPointF> points;  // 各个点的坐标
    QPointF newPoint;  // 最后一个点
    Type type;  // 类型
    double elevation;  // 高程
    qreal alpha;  // 旋转角度
    QPointF offset;  // 偏移量
signals:
    void select(Polyline *polyline);  // 图形被选择

public slots:
    void onSceneMoveableChanged(bool moveable) Q_DECL_OVERRIDE;  //  响应场景可移动性改变
};

#endif // POLYLINE_H

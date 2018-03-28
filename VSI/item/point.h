#ifndef POINT_H
#define POINT_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include "shape.h"
#include <QPointF>
#include <QRectF>

class Point : public Shape, public QGraphicsItem
{
    Q_OBJECT
public:
    Point(QGraphicsItem *parent=0);
    void startDraw(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 开始绘图
    void drawing(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 绘图开始
    bool updateFlag(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE; // paint

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    bool contains(const QPointF &point) const Q_DECL_OVERRIDE;

    void setOffset(int offset);
    int getOffset();

    QPen pen() const;
    void setPen(const QPen &pen);

    QPointF point() const;
    void setPoint(const QPointF &point);
    inline void setPoint(qreal x1, qreal y1)
    { setPoint(QPointF(x1, y1)); }

    enum {
        Type = 11,
    };
    int type() const Q_DECL_OVERRIDE;

    crossType getCrossType();
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
    QPointF pos;  // 位置
    int offset;  //  点的偏移量
    QPen pen_style;  // 笔类型
    crossType ctype;  // 交叉类型
signals:
    void select(Point *point);  // 图形被选择

public slots:
    void onSceneMoveableChanged(bool moveable) Q_DECL_OVERRIDE;  //  响应场景可移动性改变
};

#endif // POINT_H

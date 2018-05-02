#ifndef CIRCLE_H
#define CIRCLE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include "shape.h"
#include <QPointF>

class Circle : public Shape, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    Circle(QGraphicsItem *parent=0);
    void startDraw(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 开始绘图
    void drawing(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 绘图开始
    bool updateFlag(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE; // paint

    void setCPoint(QPointF p);  // 设置圆心
    QPointF getCPoint();  // 获取圆心

    void setRadius(qreal r);  // 设置半径
    qreal getRadius();  // 获取半径

    void setFilled(bool filled);  // 设置填充
    bool isFilled() const;  // 是否填充

    void setCircle(qreal px, qreal py, qreal radius);  // 设置圆形
    Circle circle();  // 圆形

    Circle *copy();

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
    QPointF cPoint, sPoint; // 圆心
    qreal r; // 半径
    bool filled;  // 设置填充
    qreal crossSize;  // 交叉线大小
signals:
    void select(Circle *circle);  // 图形被选择

public slots:
    void onSceneMoveableChanged(bool moveable) Q_DECL_OVERRIDE;  //  响应场景可移动性改变
};

#endif // CIRCLE_H

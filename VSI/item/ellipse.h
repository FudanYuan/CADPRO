#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include "shape.h"
#include <QPointF>

class Ellipse : public Shape, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    Ellipse(QGraphicsItem *parent=0);
    void startDraw(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 开始绘图
    void drawing(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 绘图开始
    bool updateFlag(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE; // paint
    qreal getLineAngle(QPointF sPoint, QPointF ePoint);

    void setEllipse(qreal px, qreal py, qreal radius1, qreal radius2, qreal angle);  // 设置椭圆元素
    Ellipse ellipse();  // 获取椭圆元素
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
    QPointF cPoint, sPoint, ePoint; // 圆心
    qreal r1, r2; // 半径1，2
    qreal alpha;  // 旋转角度
    bool cFlag;  // 确定圆心
    bool r1Flag;  // 确定r1
    bool r2Flag;  // 确定r2

signals:
    void select(Ellipse *ellipse);  // 图形被选择

public slots:
    void onSceneMoveableChanged(bool moveable) Q_DECL_OVERRIDE;  //  响应场景可移动性改变
};

#endif // ELLIPSE_H

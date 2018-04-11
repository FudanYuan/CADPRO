#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include "shape.h"
#include <QPointF>
#include "itemproperties.h"

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

    void setCPoint(QPointF p);  // 设置圆心
    QPointF getCPoint();  // 获取圆心

    void setRadius1(qreal r1);  // 设置半径1
    qreal getRadius1();  // 获取半径1

    void setRadius2(qreal r2);  // 设置半径2
    qreal getRadius2();  // 获取半径2

    void setAlpha(qreal a);  // 设置旋转角度
    qreal getAlpha();  // 获取旋转角度

    void setEllipse(qreal px, qreal py, qreal radius1, qreal radius2, qreal angle);  // 设置椭圆元素
    Ellipse ellipse();  // 获取椭圆元素

    ItemProperties *ellipseproperties;

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
    void typechange();
};

#endif // ELLIPSE_H

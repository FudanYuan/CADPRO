//绘制梯形


#ifndef TRAPEZIUM_H
#define TRAPEZIUM_H

#include <QGraphicsPathItem>
#include <QGraphicsSceneMouseEvent>
#include "shape.h"
#include <QPointF>

class Trapezium : public Shape, public QGraphicsPathItem
{
    Q_OBJECT

public:

    Trapezium(QGraphicsItem *parent=0);
    void startDraw(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 开始绘图
    void drawing(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 绘图开始
    bool updateFlag(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE; // paint

    double getTrapezium_alpha1() const;
    void setTrapezium_alpha1(double value);

    double getTrapezium_alpha2() const;
    void setTrapezium_alpha2(double value);

    double getTrapezium_H() const;
    void setTrapezium_H(double value);

    double getTrapezium_toplength() const;
    void setTrapezium_toplength(double value);

    double getTrapezium_type() const;
    void setTrapezium_type(double value);

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

    QPointF cPoint;

    double trapezium_alpha1 = 60;//角度1
    double trapezium_alpha2 = 60;//角度2
    double trapezium_H = 100;//高
    double trapezium_toplength = 100;//上底
    double trapezium_type = 1;//是否插入一半



public slots:
    void on_commandLinkButton_2_clicked();
    void onSceneMoveableChanged(bool moveable) Q_DECL_OVERRIDE;  //  响应场景可移动性改变
};

#endif // TRAPEZIUM_H

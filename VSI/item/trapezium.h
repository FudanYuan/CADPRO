//绘制梯形


#ifndef TRAPEZIUM_H
#define TRAPEZIUM_H

#include <QGraphicsPathItem>
#include <QGraphicsSceneMouseEvent>
#include "shape.h"
#include <QPointF>
#include "itemproperties.h"

class Trapezium : public Shape, public QGraphicsPathItem
{
    Q_OBJECT

public:
    Trapezium(QGraphicsItem *parent=0);
    void startDraw(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 开始绘图
    void drawing(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 绘图开始
    bool updateFlag(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE; // paint
    QList<QPointF> toPolyline();

    double getTrapeziumAlpha1() const;
    void setTrapeziumAlpha1(double value);

    double getTrapeziumAlpha2() const;
    void setTrapeziumAlpha2(double value);

    double getTrapeziumH() const;
    void setTrapeziumH(double value);

    double getTrapeziumToplength() const;
    void setTrapeziumToplength(double value);

    double getTrapeziumType() const;
    void setTrapeziumType(double value);

    ItemProperties *trapeziumproperties;

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

    double trapeziumAlpha1;//角度1
    double trapeziumAlpha2;//角度2
    double trapeziumH;//高
    double trapeziumToplength;//上底
    double trapeziumType;//是否插入一半

signals:
    void select(Trapezium *trapezium);

public slots:
    void on_commandLinkButton_2_clicked();
    void onSceneMoveableChanged(bool moveable) Q_DECL_OVERRIDE;  //  响应场景可移动性改变
    void typechange();
};

#endif // TRAPEZIUM_H

#ifndef LINE_H
#define LINE_H

#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include "shape.h"
#include <QPointF>

class Line : public Shape, public QGraphicsLineItem
{
    Q_OBJECT
public:
    Line(QGraphicsItem *parent=0);
    void startDraw(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 开始绘图
    void drawing(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 绘图开始
    bool updateFlag(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE; // paint

    void setCustomLine(const QLineF &line);

    void setCrossSize(qreal size);  // 设置交叉线大小
    qreal getCrossSize();  // 获取交叉线大小

    void setCrossCircleR(qreal r);  // 设置交叉线外圆半径
    qreal getCrossCircleR();  // 获取交叉线外圆半径

    void setArrowSize(qreal size);  // 设置箭头大小
    qreal getArrowSize();  // 获取箭头大小

    qreal getPerimeter();  // 获取周长
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
    QPointF sPoint, ePoint;  // 起止点
    qreal perimeter;  // 周长
    qreal crossSize;  // 交叉线大小
    qreal crossCircleR;  // 交叉线外圆半径
    qreal arrowSize;  //  箭头大小
    bool sPointEdit;  // 更改起点
    bool ePointEdit;  // 更改终点
signals:
    void select(Line *line);  // 图形被选择
public slots:
    void onSceneMoveableChanged(bool moveable) Q_DECL_OVERRIDE;  //  响应场景可移动性改变
};

#endif // LINE_H

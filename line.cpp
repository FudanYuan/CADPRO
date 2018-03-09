#include "line.h"
#include <QCursor>
#include <QPainter>
#include <QPen>
#include <QDebug>

Line::Line(QGraphicsItem *parent) :
    QGraphicsLineItem(parent)
{
    setShapeType(Shape::Line);
    // 设置图元为可焦点的
    setFlag(QGraphicsItem::ItemIsFocusable);
    // 设置图元为可移动的
    setFlag(QGraphicsItem::ItemIsMovable, false);
    // 设置图元为可接受拖拽事件
    setAcceptDrops(true);
    // 设置图元为可接受hover事件
    setAcceptHoverEvents(true);
}

void Line::startDraw(QGraphicsSceneMouseEvent *event)
{
    QPen pen = QPen();
    pen.setColor(penStyle.color);
    pen.setStyle(penStyle.style);
    pen.setWidthF(penStyle.width);
    setPen(pen);
    sPoint = event->scenePos();
//    sPoint = event->pos();
    overFlag = true;  // 马上就要结束
}

void Line::drawing(QGraphicsSceneMouseEvent *event)
{
    ePoint = event->scenePos();
    QLineF newLine(sPoint, ePoint);
    setLine(newLine);
    update();
}

bool Line::updateFlag(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    return overFlag;
}

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    scaleFactor =  painter->matrix().m11();
    // 获取到当前的线宽，这里的线宽其实还是之前设置的线宽值;
    // 比如我们之前设置线宽为 2 ，这里返回的线宽还是 2 ，但是当前的缩放比例变了；
    // 其实当前的线宽就相当于 penWidth * scaleFactor;
    // 所以如果我们想要让线宽保持不变，那就需要进行转换，即 penWidth = penWidth / scaleFactor;
    QPen pen = this->pen();
    // 重新设置画笔线宽;
    pen.setWidthF(pen.widthF() / scaleFactor);

    Shape::ShapeType type = getShapeType();
    switch (type) {
    case Shape::Line:
    case Shape::MiddleAxis:

        painter->setPen(pen);
        painter->drawLine(this->line());
        break;
    case Shape::Direction:
    {
        pen.setColor(Qt::red);
        painter->setPen(pen);
        drawCrossPoint(painter, sPoint, 2, upright);
        painter->drawEllipse(sPoint, 2, 2);
        QBrush brush = QBrush();
        brush.setColor(Qt::red);
        brush.setStyle(Qt::SolidPattern);
        painter->setBrush(brush);
        drawLineWithArrow(painter, QLineF(sPoint, ePoint), 5);
        break;
    }
    default:
        break;
    }
}

qreal Line::getPerimeter()
{
    return qSqrt(qPow((sPoint.rx()-ePoint.rx()), 2)
                 + qPow((sPoint.ry()-ePoint.ry()), 2));
}

void Line::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(selectable){
        selected = true;
        qDebug() << "type: " << getShapeType();
        qDebug() << "id: " << getShapeId();
        setCursor(Qt::ClosedHandCursor);
        QPen pen = QPen();
        pen.setColor(selectedEntity.color);
        pen.setStyle(selectedEntity.style);
        pen.setWidthF(selectedEntity.width);
        setPen(pen);
        emit select(this);
    }
    QGraphicsItem::mousePressEvent(event);
}

void Line::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "Line::mouseMoveEvent";
    QGraphicsItem::mouseMoveEvent(event);
}

void Line::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "Line::mouseReleaseEvent";
    QGraphicsItem::mouseReleaseEvent(event);
}

void Line::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Line::dragEnterEvent";
    QGraphicsItem::dragEnterEvent(event);
}

void Line::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Line::dragLeaveEvent";
    QGraphicsItem::dragLeaveEvent(event);
}

void Line::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Line::dragMoveEvent";
    QGraphicsItem::dragMoveEvent(event);
}

void Line::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Line::dropEvent";
    QGraphicsItem::dropEvent(event);
}

void Line::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if(selectable){
        QPen pen = QPen();
        if(!selected){
            pen.setColor(underCursorStyle.color);
            pen.setStyle(underCursorStyle.style);
            pen.setWidthF(underCursorStyle.width);
        } else{
            pen.setColor(selectedEntity.color);
            pen.setStyle(selectedEntity.style);
            pen.setWidthF(selectedEntity.width);
        }
        setPen(pen);
        setCursor(Qt::OpenHandCursor);
        QGraphicsItem::hoverEnterEvent(event);
    }
}

void Line::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(selectable){
        setCursor(Qt::OpenHandCursor);
        QGraphicsItem::hoverMoveEvent(event);
    }
}

void Line::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if(selectable){
        QPen pen = QPen();
        if(!selected){
            pen.setColor(penStyle.color);
            pen.setStyle(penStyle.style);
            pen.setWidthF(penStyle.width);
        } else{
            pen.setColor(selectedEntity.color);
            pen.setStyle(selectedEntity.style);
            pen.setWidthF(selectedEntity.width);
        }
        setPen(pen);
        QGraphicsItem::hoverLeaveEvent(event);
    }
}

void Line::onSceneMoveableChanged(bool moveable)
{
    this->moveable = moveable;
    setFlag(QGraphicsItem::ItemIsMovable, moveable);
}


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
    sPoint = event->scenePos();
}

void Line::drawing(QGraphicsSceneMouseEvent *event)
{
    ePoint = event->scenePos();
    QLineF newLine(sPoint, ePoint);
    setLine(newLine);
}

void Line::setStyle(EntityStyle style)
{
    QPen pen = QPen();
    pen.setColor(style.perimeterLine.color);
    pen.setStyle(style.perimeterLine.style);
    qDebug() << " new width " << style.perimeterLine.width / this->scaleFactor;
    pen.setWidth(style.perimeterLine.width / this->scaleFactor);
    setPen(pen);
}

void Line::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(overFlag){
        qDebug() << "type: " << getShapeType();
        qDebug() << "id: " << getShapeId();
        setCursor(Qt::ClosedHandCursor);
    }
}

void Line::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Line::mouseMoveEvent";
    QGraphicsItem::mouseMoveEvent(event);
}

void Line::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Line::mouseReleaseEvent";
    QGraphicsItem::mouseReleaseEvent(event);
}

void Line::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    qDebug() << "Line::dragEnterEvent";
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
    if(overFlag){
        QPen pen = QPen();
        pen.setColor(Qt::blue);
        pen.setWidth(2 / this->scaleFactor);
        setPen(pen);
        setCursor(Qt::OpenHandCursor);
        QGraphicsItem::hoverEnterEvent(event);
    }
}

void Line::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(overFlag){
        setCursor(Qt::OpenHandCursor);
        QGraphicsItem::hoverMoveEvent(event);
    }
}

void Line::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if(overFlag){
        QPen pen = QPen();
        pen.setColor(Qt::red);
        pen.setWidth(1 / this->scaleFactor);
        setPen(pen);
        QGraphicsItem::hoverLeaveEvent(event);
    }
}

void Line::onSceneScaleChanged(qreal scaleFactor)
{
    this->scaleFactor = scaleFactor;
    qDebug() << "line on view scale changed: " << this->scaleFactor;
    update();
}

void Line::onSceneMoveableChanged(bool moveable)
{
    this->moveable = moveable;
    setFlag(QGraphicsItem::ItemIsMovable, moveable);
}


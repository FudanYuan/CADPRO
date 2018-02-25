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
}

void Line::drawing(QGraphicsSceneMouseEvent *event)
{
    ePoint = event->scenePos();
    QLineF newLine(sPoint, ePoint);
    setLine(newLine);
}

void Line::setPenStyle(PenStyle penStyle)
{
    this->penStyle = penStyle;
}

void Line::setEntityUnderCursorStyle(PenStyle underCursorStyle)
{
    this->underCursorStyle = underCursorStyle;
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
    QPen myPen = this->pen();
    // 重新设置画笔线宽;
    myPen.setWidthF(myPen.widthF() / scaleFactor);
    painter->setPen(myPen);
    painter->drawLine(this->line());
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
    if(overFlag){
        QPen pen = QPen();
        pen.setColor(underCursorStyle.color);
        pen.setStyle(underCursorStyle.style);
        pen.setWidthF(underCursorStyle.width);
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
        pen.setColor(penStyle.color);
        pen.setStyle(penStyle.style);
        pen.setWidthF(penStyle.width);
        setPen(pen);
        QGraphicsItem::hoverLeaveEvent(event);
    }
}

void Line::onSceneMoveableChanged(bool moveable)
{
    this->moveable = moveable;
    setFlag(QGraphicsItem::ItemIsMovable, moveable);
}


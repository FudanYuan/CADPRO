#include "rect.h"
#include <QCursor>
#include <QPainter>
#include <QPen>
#include <QDebug>

Rect::Rect(QGraphicsItem *parent) :
    QGraphicsRectItem(parent)
{
    setShapeType(Shape::Rectangle);
    // 设置图元为可焦点的
    setFlag(QGraphicsItem::ItemIsFocusable);
    // 设置图元为可移动的
    setFlag(QGraphicsItem::ItemIsMovable, false);
    // 设置图元为可接受拖拽事件
    setAcceptDrops(true);
    // 设置图元为可接受hover事件
//    setAcceptHoverEvents(true);
}

void Rect::startDraw(QGraphicsSceneMouseEvent *event)
{
    topLeftPoint = event->scenePos();
    setRect(QRectF(event->scenePos(), QSizeF(0, 0)));
}

void Rect::drawing(QGraphicsSceneMouseEvent *event)
{
    bottomRightPoint = event->scenePos();
    QRectF r(rect().topLeft(), QSizeF((event->scenePos().x() - rect().topLeft().x()),
                                      (event->scenePos().y() - rect().topLeft().y())));
    setRect(r);
}

void Rect::setStyle(EntityStyle style)
{
    QPen pen = QPen();
    pen.setColor(style.perimeterLine.color);
    pen.setStyle(style.perimeterLine.style);
    qDebug() << " new width " << style.perimeterLine.width / this->scaleFactor;
    pen.setWidth(style.perimeterLine.width / this->scaleFactor);
    setPen(pen);
}

void Rect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "type: " << getShapeType();
    qDebug() << "id: " << getShapeId();
    setCursor(Qt::OpenHandCursor);
}

void Rect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Rect::mouseMoveEvent";
    QGraphicsItem::mouseMoveEvent(event);
}

void Rect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Rect::mouseReleaseEvent";
    QGraphicsItem::mouseReleaseEvent(event);
}

void Rect::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    qDebug() << "Rect::dragEnterEvent";
    QGraphicsItem::dragEnterEvent(event);
}

void Rect::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Rect::dragLeaveEvent";
    QGraphicsItem::dragLeaveEvent(event);
}

void Rect::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Rect::dragMoveEvent";
    QGraphicsItem::dragMoveEvent(event);
}

void Rect::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Rect::dropEvent";
    QGraphicsItem::dropEvent(event);
}

void Rect::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "Rect::hoverEnterEvent";
//    QPen pen = QPen();
//    pen.setColor(Qt::blue);
//    pen.setWidth(2 / this->scaleFactor);
//    setPen(pen);
    QGraphicsItem::hoverEnterEvent(event);
}

void Rect::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "Rect::hoverMoveEvent";
    QGraphicsItem::hoverMoveEvent(event);
}

void Rect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "Rect::hoverLeaveEvent";
    QGraphicsItem::hoverLeaveEvent(event);
}

void Rect::onSceneScaleChanged(qreal scaleFactor)
{
    this->scaleFactor = scaleFactor;
    qDebug() << "rect on view scale changed: " << this->scaleFactor;
    update();
}

void Rect::onSceneMoveableChanged(bool moveable)
{
    this->moveable = moveable;
    setFlag(QGraphicsItem::ItemIsMovable, moveable);
}

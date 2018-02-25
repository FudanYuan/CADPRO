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
    setAcceptHoverEvents(true);
}

void Rect::startDraw(QGraphicsSceneMouseEvent *event)
{
    QPen pen = QPen();
    pen.setColor(penStyle.color);
    pen.setStyle(penStyle.style);
    pen.setWidthF(penStyle.width);
    setPen(pen);

    qDebug() << penStyle.color;
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

void Rect::setPenStyle(PenStyle penStyle)
{
    this->penStyle = penStyle;
}

void Rect::setEntityUnderCursorStyle(PenStyle underCursorStyle)
{
    this->underCursorStyle = underCursorStyle;
}

void Rect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
    painter->drawRect(this->rect());
}

void Rect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    if(overFlag){
        qDebug() << "type: " << getShapeType();
        qDebug() << "id: " << getShapeId();
        setCursor(Qt::ClosedHandCursor);
    }
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

void Rect::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(overFlag){
        setCursor(Qt::OpenHandCursor);
        QGraphicsItem::hoverMoveEvent(event);
    }
}

void Rect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
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

void Rect::onSceneMoveableChanged(bool moveable)
{
    this->moveable = moveable;
    setFlag(QGraphicsItem::ItemIsMovable, moveable);
}

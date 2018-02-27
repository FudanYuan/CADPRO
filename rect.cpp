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
    topLeftPoint = event->scenePos();
    setRect(QRectF(event->scenePos(), QSizeF(0, 0)));
    overFlag = true;
}

void Rect::drawing(QGraphicsSceneMouseEvent *event)
{
    bottomRightPoint = event->scenePos();
    QRectF r(rect().topLeft(), QSizeF((event->scenePos().x() - rect().topLeft().x()),
                                      (event->scenePos().y() - rect().topLeft().y())));
    setRect(r);
}

bool Rect::updateFlag(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    return overFlag;
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
    }
    QGraphicsItem::mousePressEvent(event);
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

void Rect::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(selectable){
        setCursor(Qt::OpenHandCursor);
        QGraphicsItem::hoverMoveEvent(event);
    }
}

void Rect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
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
        QGraphicsItem::hoverLeaveEvent(event);
    }
}

void Rect::onSceneMoveableChanged(bool moveable)
{
    this->moveable = moveable;
    setFlag(QGraphicsItem::ItemIsMovable, moveable);
}

#include "circle.h"
#include <QCursor>
#include <QPainter>
#include <QPen>
#include <QtCore/qmath.h>
#include <QDebug>

Circle::Circle(QGraphicsItem *parent) :
    QGraphicsEllipseItem(parent),
    filled(false)
{
    setShapeType(Shape::Circle);
    // 设置图元为可焦点的
    setFlag(QGraphicsItem::ItemIsFocusable);
    // 设置图元为可移动的
    setFlag(QGraphicsItem::ItemIsMovable, false);
    // 设置图元为可接受拖拽事件
    setAcceptDrops(true);
    // 设置图元为可接受hover事件
    setAcceptHoverEvents(true);
}

void Circle::startDraw(QGraphicsSceneMouseEvent *event)
{
    QPen pen = QPen();
    pen.setColor(penStyle.color);
    pen.setStyle(penStyle.style);
    pen.setWidthF(penStyle.width);
    setPen(pen);
    cPoint = event->scenePos();
    overFlag = true;
}

void Circle::drawing(QGraphicsSceneMouseEvent *event)
{
    sPoint = event->scenePos();
    r = qSqrt((sPoint.rx() - cPoint.rx()) * (sPoint.rx() - cPoint.rx())
            + (sPoint.ry() - cPoint.ry()) * (sPoint.ry() - cPoint.ry()));
    setRect(cPoint.rx()-r, cPoint.ry()-r, r*2, r*2);
    update();
}

bool Circle::updateFlag(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    return overFlag;
}

void Circle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
    if(this->filled){
        painter->setBrush(QBrush(penStyle.color, Qt::SolidPattern));
    }
    painter->setPen(pen);
    painter->drawEllipse(cPoint, r, r);
    drawCrossPoint(painter, cPoint, 5, upright);
}

void Circle::setCPoint(QPointF p)
{
    this->cPoint = p;
}

QPointF Circle::getCPoint()
{
    return this->cPoint;
}

void Circle::setRadius(qreal r)
{
    this->r = r;
}

qreal Circle::getRadius()
{
    return this->r;
}

void Circle::setCircle(qreal px, qreal py, qreal radius)
{
    QPen pen = QPen();
    pen.setColor(penStyle.color);
    pen.setStyle(penStyle.style);
    pen.setWidthF(penStyle.width);
    setPen(pen);

    cPoint = QPointF(px, py);
    sPoint = cPoint + QPointF(0, radius);
    r = radius;
    setRect(cPoint.rx()-r, cPoint.ry()-r, r*2, r*2);
    update();
}

Circle Circle::circle()
{

}

void Circle::setFilled(bool filled)
{
    this->filled = filled;
}

void Circle::mousePressEvent(QGraphicsSceneMouseEvent *event)
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
        if(this->filled){
            setBrush(QBrush(selectedEntity.color, Qt::SolidPattern));
        }
        setPen(pen);
        emit select(this);
    }
    QGraphicsItem::mousePressEvent(event);
}

void Circle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "Circle::mouseMoveEvent";
    QGraphicsItem::mouseMoveEvent(event);
}

void Circle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "Circle::mouseReleaseEvent";
    QGraphicsItem::mouseReleaseEvent(event);
}

void Circle::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Circle::dragEnterEvent";
    QGraphicsItem::dragEnterEvent(event);
}

void Circle::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Circle::dragLeaveEvent";
    QGraphicsItem::dragLeaveEvent(event);
}

void Circle::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Circle::dragMoveEvent";
    QGraphicsItem::dragMoveEvent(event);
}

void Circle::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Circle::dropEvent";
    QGraphicsItem::dropEvent(event);
}

void Circle::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if(selectable){
        QPen pen = QPen();
        if(!selected){
            pen.setColor(underCursorStyle.color);
            pen.setStyle(underCursorStyle.style);
            pen.setWidthF(underCursorStyle.width);
            if(this->filled){
                setBrush(QBrush(underCursorStyle.color, Qt::SolidPattern));
            }
        } else{
            pen.setColor(selectedEntity.color);
            pen.setStyle(selectedEntity.style);
            pen.setWidthF(selectedEntity.width);
            if(this->filled){
                setBrush(QBrush(selectedEntity.color, Qt::SolidPattern));
            }
        }
        setPen(pen);
        setCursor(Qt::OpenHandCursor);
        QGraphicsItem::hoverEnterEvent(event);
    }
}

void Circle::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(selectable){
        setCursor(Qt::OpenHandCursor);
        QGraphicsItem::hoverMoveEvent(event);
    }
}

void Circle::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if(selectable){
        QPen pen = QPen();
        if(!selected){
            pen.setColor(penStyle.color);
            pen.setStyle(penStyle.style);
            pen.setWidthF(penStyle.width);
            if(this->filled){
                setBrush(QBrush(penStyle.color, Qt::SolidPattern));
            }
        } else{
            pen.setColor(selectedEntity.color);
            pen.setStyle(selectedEntity.style);
            pen.setWidthF(selectedEntity.width);
            if(this->filled){
                setBrush(QBrush(selectedEntity.color, Qt::SolidPattern));
            }
        }
        setPen(pen);
        QGraphicsItem::hoverLeaveEvent(event);
    }
}

void Circle::onSceneMoveableChanged(bool moveable)
{
    this->moveable = moveable;
    setFlag(QGraphicsItem::ItemIsMovable, moveable);
}

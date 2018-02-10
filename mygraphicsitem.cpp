#include "mygraphicsitem.h"
#include <QDebug>
#include <QCursor>

MyGraphicsItem::MyGraphicsItem()
{
    color = Qt::red;
    setFlag(QGraphicsItem::ItemIsFocusable);
    //设置图元为可移动的
    setFlag(QGraphicsItem::ItemIsMovable);
    setAcceptDrops(true);
}

void MyGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    qDebug() << "MyGraphicsItem::paint";
    if(hasFocus()) {
        painter->setPen(QPen(QColor(255,255,255,200)));
    } else {
        painter->setPen(QPen(QColor(100,100,100,100)));
    }
    painter->setBrush(color);
    painter->drawRect(-10, -10, 20, 20);
    painter->drawLine(-10, -10, 20, 20);
}

QRectF MyGraphicsItem::boundingRect() const
{
    qreal adjust = 0.5;
    return QRectF(-10 - adjust, -10 - adjust, 20 + adjust, 20 + adjust);
}

void MyGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    qDebug() << "MyGraphicsItem::mousePressEvent";
    setCursor(Qt::OpenHandCursor);
}

void MyGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "MyGraphicsItem::mouseMoveEvent";
    QGraphicsItem::mouseMoveEvent(event);
}

void MyGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "MyGraphicsItem::mouseReleaseEvent";
    QGraphicsItem::mouseReleaseEvent(event);
}

void MyGraphicsItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    qDebug() << "MyGraphicsItem::dragEnterEvent";
    QGraphicsItem::dragEnterEvent(event);
}

void MyGraphicsItem::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    qDebug() << "MyGraphicsItem::dragLeaveEvent";
    QGraphicsItem::dragLeaveEvent(event);
}

void MyGraphicsItem::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    qDebug() << "MyGraphicsItem::dragMoveEvent";
    QGraphicsItem::dragMoveEvent(event);
}

void MyGraphicsItem::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    qDebug() << "MyGraphicsItem::dragMoveEvent";
    QGraphicsItem::dragMoveEvent(event);
}

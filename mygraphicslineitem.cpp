#include "mygraphicslineitem.h"
#include <QCursor>
#include <QDebug>

MyGraphicsLineItem::MyGraphicsLineItem()
{
    // 设置图元为可焦点的
    setFlag(QGraphicsItem::ItemIsFocusable);
    // 设置图元为可移动的
    setFlag(QGraphicsItem::ItemIsMovable);
    // 设置图元为可接受拖拽事件
    setAcceptDrops(true);
}

void MyGraphicsLineItem::startDraw(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "start draw a line";
    qDebug() << event->scenePos().x() << "  " << event->scenePos().y();
    sPoint = event->scenePos();
}

void MyGraphicsLineItem::drawing(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "drawing a line";
    ePoint = event->scenePos();
    QLineF newLine(sPoint, ePoint);
    setLine(newLine);
}

void MyGraphicsLineItem::setStyle(EntityStyle style)
{

}

void MyGraphicsLineItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    qDebug() << "MyGraphicsLineItem::mousePressEvent";
    setCursor(Qt::OpenHandCursor);
}

void MyGraphicsLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "MyGraphicsLineItem::mouseMoveEvent";
    QGraphicsItem::mouseMoveEvent(event);
}

void MyGraphicsLineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "MyGraphicsLineItem::mouseReleaseEvent";
    QGraphicsItem::mouseReleaseEvent(event);
}

void MyGraphicsLineItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    qDebug() << "MyGraphicsLineItem::dragEnterEvent";
    QGraphicsItem::dragEnterEvent(event);
}

void MyGraphicsLineItem::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    qDebug() << "MyGraphicsLineItem::dragLeaveEvent";
    QGraphicsItem::dragLeaveEvent(event);
}

void MyGraphicsLineItem::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    qDebug() << "MyGraphicsLineItem::dragMoveEvent";
    QGraphicsItem::dragMoveEvent(event);
}

void MyGraphicsLineItem::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    qDebug() << "MyGraphicsLineItem::dragMoveEvent";
    QGraphicsItem::dragMoveEvent(event);
}


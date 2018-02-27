#include "ellipse.h"
#include <QCursor>
#include <QPainter>
#include <QPen>
#include <QtCore/qmath.h>
#include <QDebug>

Ellipse::Ellipse(QGraphicsItem *parent) :
    QGraphicsEllipseItem(parent)
{
    setShapeType(Shape::Ellipse);
    // 设置图元为可焦点的
    setFlag(QGraphicsItem::ItemIsFocusable);
    // 设置图元为可移动的
    setFlag(QGraphicsItem::ItemIsMovable, false);
    // 设置图元为可接受拖拽事件
    setAcceptDrops(true);
    // 设置图元为可接受hover事件
    setAcceptHoverEvents(true);
}

void Ellipse::startDraw(QGraphicsSceneMouseEvent *event)
{
    QPen pen = QPen();
    pen.setColor(penStyle.color);
    pen.setStyle(penStyle.style);
    pen.setWidthF(penStyle.width);
    setPen(pen);
    cPoint = event->scenePos();
    sPoint = event->scenePos();
    cFlag = true;
}

void Ellipse::drawing(QGraphicsSceneMouseEvent *event)
{
    if(r2Flag){
        Q_UNUSED(event);
        update();
        return;
    }
    if(r1Flag){
        ePoint = event->scenePos();
        r2 = qSqrt((ePoint.rx() - cPoint.rx()) * (ePoint.rx() - cPoint.rx())
                + (ePoint.ry() - cPoint.ry()) * (ePoint.ry() - cPoint.ry()));
        update();
        return;
    }
    if(cFlag){
        sPoint = event->scenePos();
        update();
    }
}

bool Ellipse::updateFlag(QGraphicsSceneMouseEvent *event)
{
    if(!r1Flag){ // 确定r1
        sPoint = event->scenePos();
        r1 = qSqrt((sPoint.rx() - cPoint.rx()) * (sPoint.rx() - cPoint.rx())
                + (sPoint.ry() - cPoint.ry()) * (sPoint.ry() - cPoint.ry()));
        r1Flag = true;
        update();
        return false;
    }
    if(!r2Flag){// 确定r2
        ePoint = event->scenePos();
        r2 = qSqrt((ePoint.rx() - cPoint.rx()) * (ePoint.rx() - cPoint.rx())
                + (ePoint.ry() - cPoint.ry()) * (ePoint.ry() - cPoint.ry()));
        setRect(cPoint.rx()-r1, cPoint.ry()-r2, r1*2, r2*2);
        r2Flag = true;
        overFlag = true;
        update();
    }
    return overFlag;
}

void Ellipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
    if(r2Flag){
        painter->setPen(myPen);
        painter->drawPoint(cPoint);
        painter->drawEllipse(cPoint, r1, r2);
        return;
    }
    if(r1Flag){
        painter->setPen(myPen);
        painter->drawEllipse(cPoint, r1, r2);
        myPen.setColor(Qt::black);
        myPen.setStyle(Qt::DashLine);
        painter->setPen(myPen);
        painter->drawPoint(cPoint);
        painter->drawLine(cPoint, sPoint);
        painter->drawLine(cPoint, ePoint);
        return;
    }
    if(cFlag){ // 画直线
        myPen.setColor(Qt::black);
        myPen.setStyle(Qt::DashLine);
        painter->setPen(myPen);
        painter->drawPoint(cPoint);
        painter->drawLine(cPoint, sPoint);
        return;
    }
}

void Ellipse::mousePressEvent(QGraphicsSceneMouseEvent *event)
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

void Ellipse::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "Ellipse::mouseMoveEvent";
    QGraphicsItem::mouseMoveEvent(event);
}

void Ellipse::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "Ellipse::mouseReleaseEvent";
    QGraphicsItem::mouseReleaseEvent(event);
}

void Ellipse::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Ellipse::dragEnterEvent";
    QGraphicsItem::dragEnterEvent(event);
}

void Ellipse::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Ellipse::dragLeaveEvent";
    QGraphicsItem::dragLeaveEvent(event);
}

void Ellipse::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Ellipse::dragMoveEvent";
    QGraphicsItem::dragMoveEvent(event);
}

void Ellipse::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Ellipse::dropEvent";
    QGraphicsItem::dropEvent(event);
}

void Ellipse::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
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

void Ellipse::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(selectable){
        setCursor(Qt::OpenHandCursor);
        QGraphicsItem::hoverMoveEvent(event);
    }
}

void Ellipse::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
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

void Ellipse::onSceneMoveableChanged(bool moveable)
{
    this->moveable = moveable;
    setFlag(QGraphicsItem::ItemIsMovable, moveable);
}

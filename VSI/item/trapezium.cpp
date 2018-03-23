#include "trapezium.h"
#include <QCursor>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <qmath.h>
#include <QDebug>

Trapezium::Trapezium(QGraphicsItem *parent) :
    QGraphicsPathItem(parent)
{
    setShapeType(Shape::Trapezium);
    // 设置图元为可焦点的
    setFlag(QGraphicsItem::ItemIsFocusable);
    // 设置图元为可移动的
    setFlag(QGraphicsItem::ItemIsMovable, false);
    // 设置图元为可接受拖拽事件
    setAcceptDrops(true);
    // 设置图元为可接受hover事件
    setAcceptHoverEvents(true);
}

void Trapezium::startDraw(QGraphicsSceneMouseEvent *event)
{
    QPen pen = QPen();
    pen.setColor(penStyle.color);
    pen.setStyle(penStyle.style);
    pen.setWidthF(penStyle.width);
    setPen(pen);

    overFlag = true;
}

void Trapezium::drawing(QGraphicsSceneMouseEvent *event)
{
    cPoint = event->scenePos();
    update();
}

bool Trapezium::updateFlag(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    return overFlag;
}

void Trapezium::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
    painter->setPen(pen);

    QPainterPath path;
    QPointF sPoint;
    //画出一半的梯形
    if(trapezium_type == 2){
        path.moveTo(cPoint);
        if(trapezium_alpha1<=90)
        {
            sPoint.setX(cPoint.rx()+trapezium_toplength+trapezium_H/(qTan(M_PI*trapezium_alpha1/180)));sPoint.setY(cPoint.ry());
        }
        else
        {
            sPoint.setX(cPoint.rx()+trapezium_toplength-trapezium_H/(qTan(M_PI*(180-trapezium_alpha1)/180)));sPoint.setY(cPoint.ry());
        }
        path.lineTo(sPoint);
        sPoint.setX(cPoint.rx()+trapezium_toplength);sPoint.setY(cPoint.ry()-trapezium_H);
        path.lineTo(sPoint);
        sPoint.setX(cPoint.rx());sPoint.setY(cPoint.ry()-trapezium_H);
        path.lineTo(sPoint);
        path.lineTo(cPoint);
        painter->drawPath(path);
        painter->setPen(Qt::DashLine);
        painter->drawPoint(cPoint);
    }
    //画出整个梯形
    if(trapezium_type == 1){

        path.moveTo(cPoint);
        if(trapezium_alpha1<=90)
        {
            sPoint.setX(cPoint.rx()+trapezium_toplength/2+trapezium_H/(qTan(M_PI*trapezium_alpha1/180)));sPoint.setY(cPoint.ry());
        }
        else
        {
            sPoint.setX(cPoint.rx()+trapezium_toplength/2-trapezium_H/(qTan(M_PI*(180-trapezium_alpha1)/180)));sPoint.setY(cPoint.ry());
        }
        path.lineTo(sPoint);
        sPoint.setX(cPoint.rx()+trapezium_toplength/2);sPoint.setY(cPoint.ry()-trapezium_H);
        path.lineTo(sPoint);
        sPoint.setX(cPoint.rx());sPoint.setY(cPoint.ry()-trapezium_H);
        path.lineTo(sPoint);
        sPoint.setX(cPoint.rx()-trapezium_toplength/2);sPoint.setY(cPoint.ry()-trapezium_H);
        path.lineTo(sPoint);

        if(trapezium_alpha2<=90)
        {
            sPoint.setX(cPoint.rx()-trapezium_toplength/2-trapezium_H/(qTan(M_PI*trapezium_alpha2/180)));sPoint.setY(cPoint.ry());
        }
        else
        {
            sPoint.setX(cPoint.rx()-trapezium_toplength/2+trapezium_H/(qTan(M_PI*(180-trapezium_alpha2)/180)));sPoint.setY(cPoint.ry());
        }
        path.lineTo(sPoint);
        path.lineTo(cPoint);
        painter->drawPath(path);
        painter->drawPoint(cPoint);
        painter->setPen(Qt::DashLine);
    }

    setPath(path);
    drawCrossPoint(painter, cPoint, 5, upright);
}

void Trapezium::mousePressEvent(QGraphicsSceneMouseEvent *event)
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

void Trapezium::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "PolyLine::mouseMoveEvent";
    QGraphicsItem::mouseMoveEvent(event);
}

void Trapezium::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "PolyLine::mouseReleaseEvent";
    QGraphicsItem::mouseReleaseEvent(event);
}

void Trapezium::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "PolyLine::dragEnterEvent";
    QGraphicsItem::dragEnterEvent(event);
}

void Trapezium::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "PolyLine::dragLeaveEvent";
    QGraphicsItem::dragLeaveEvent(event);
}

void Trapezium::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "PolyLine::dragMoveEvent";
    QGraphicsItem::dragMoveEvent(event);
}

void Trapezium::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "PolyLine::dropEvent";
    QGraphicsItem::dropEvent(event);
}

void Trapezium::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
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

void Trapezium::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(selectable){
        setCursor(Qt::OpenHandCursor);
        QGraphicsItem::hoverMoveEvent(event);
    }
}

void Trapezium::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
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

double Trapezium::getTrapezium_type() const
{
    return trapezium_type;
}

void Trapezium::setTrapezium_type(double value)
{
    trapezium_type = value;
}

double Trapezium::getTrapezium_toplength() const
{
    return trapezium_toplength;
}

void Trapezium::setTrapezium_toplength(double value)
{
    trapezium_toplength = value;
}

double Trapezium::getTrapezium_H() const
{
    return trapezium_H;
}

void Trapezium::setTrapezium_H(double value)
{
    trapezium_H = value;
}

double Trapezium::getTrapezium_alpha2() const
{
    return trapezium_alpha2;
}

void Trapezium::setTrapezium_alpha2(double value)
{
    trapezium_alpha2 = value;
}

double Trapezium::getTrapezium_alpha1() const
{
    return trapezium_alpha1;
}

void Trapezium::setTrapezium_alpha1(double value)
{
    trapezium_alpha1 = value;
}

void Trapezium::onSceneMoveableChanged(bool moveable)
{
    this->moveable = moveable;
    setFlag(QGraphicsItem::ItemIsMovable, moveable);
}

void Trapezium::on_commandLinkButton_2_clicked()
{

}

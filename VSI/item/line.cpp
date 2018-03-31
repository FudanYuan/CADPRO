#include "line.h"
#include <QCursor>
#include <QPainter>
#include <QPen>
#include <QDebug>

Line::Line(QGraphicsItem *parent) :
    QGraphicsLineItem(parent),
    crossSize(2),
    crossCircleR(2),
    arrowSize(5),
    sPointEdit(false),
    ePointEdit(false),
    itemp(false)
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
    lineproperties =new ItemProperties;
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
    // 如果处于编辑状态，则根据编辑标识进行更新
    if(editable){
        if(sPointEdit){
            sPoint = event->scenePos();
        } else if(ePointEdit){
            ePoint = event->scenePos();
        }
    } else{
        ePoint = event->scenePos();
        if(ePoint == sPoint){
            overFlag = false;
            return;
        }
        overFlag = true;
    }

    QLineF newLine(sPoint, ePoint);
    setLine(newLine);
    update();
}

bool Line::updateFlag(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    return overFlag;
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
    QPen pen = this->pen();
    // 重新设置画笔线宽;
    pen.setWidthF(pen.widthF() / scaleFactor);
    painter->save();

    Shape::ShapeType type = getShapeType();
    switch (type) {
    case Shape::Line:
    case Shape::MiddleAxis:
        painter->setPen(pen);
        painter->drawLine(this->line());
        break;
    case Shape::Direction:
    {
        pen.setColor(Qt::red);
        painter->setPen(pen);
        // 设置交叉线大小
        crossSize /= scaleFactor;
        crossCircleR /= scaleFactor;
        drawCrossPoint(painter, sPoint, crossSize, upright);
        painter->drawEllipse(sPoint, crossCircleR, crossCircleR);
        QBrush brush = QBrush();
        brush.setColor(Qt::red);
        brush.setStyle(Qt::SolidPattern);
        painter->setBrush(brush);
        // 设置方向箭头大小
        arrowSize /= scaleFactor;
        drawLineWithArrow(painter, QLineF(sPoint, ePoint), arrowSize);
        break;
    }
    default:
        break;
    }

    if(selected){
        pen.setColor(selectedEntity.color);
        painter->setPen(pen);
        QBrush brush = QBrush();
        brush.setColor(selectedEntity.color);
        brush.setStyle(Qt::SolidPattern);
        painter->setBrush(brush);
        drawRectPoint(painter, sPoint, objSize.knotSizeInPix);
        drawRectPoint(painter, ePoint, objSize.knotSizeInPix);
    }

    painter->restore();
}

void Line::setCustomLine(const QLineF &line)
{
    QPen pen = QPen();
    pen.setColor(penStyle.color);
    pen.setStyle(penStyle.style);
    pen.setWidthF(penStyle.width);
    setPen(pen);

    sPoint = line.p1();
    ePoint = line.p2();
    setLine(line);
    update();
}

void Line::setCrossSize(qreal size)
{
    this->crossSize = size;
}

qreal Line::getCrossSize()
{
    return this->crossSize;
}

void Line::setCrossCircleR(qreal r)
{
    this->crossCircleR = r;
}

qreal Line::getCrossCircleR()
{
    return this->crossCircleR;
}

void Line::setArrowSize(qreal size)
{
    this->arrowSize = size;
}

qreal Line::getArrowSize()
{
    return this->arrowSize;
}

Line *Line::copy()
{
    Line *l = new Line(this);
    l->setLine(QLineF(sPoint.rx(), sPoint.ry(), ePoint.rx(), ePoint.ry()));
    return l;
}

qreal Line::getPerimeter()
{
    return qSqrt(qPow((sPoint.rx()-ePoint.rx()), 2)
                 + qPow((sPoint.ry()-ePoint.ry()), 2));
}

void Line::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(selectable){
        selected = true;
        QPen pen = QPen();
        pen.setColor(selectedEntity.color);
        pen.setStyle(selectedEntity.style);
        pen.setWidthF(selectedEntity.width);
        setPen(pen);
        if(!editOverFlag){
            if(qAbs(event->scenePos().rx() - sPoint.rx()) <= objSize.snapDistanceInPix ||
                    qAbs(event->scenePos().ry() - sPoint.ry()) <= objSize.snapDistanceInPix){
                editable = true;
                sPointEdit = true;
            }
            if(qAbs(event->scenePos().rx() - ePoint.rx()) <= objSize.snapDistanceInPix ||
                    qAbs(event->scenePos().ry() - ePoint.ry()) <= objSize.snapDistanceInPix){
                editable = true;
                ePointEdit = true;
            }
        } else{
            sPointEdit = false;
            ePointEdit = false;
        }
        editOverFlag = false;
        update();
        emit select(this);
    }
    QGraphicsItem::mousePressEvent(event);
}

void Line::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "Line::mouseMoveEvent";
    QGraphicsItem::mouseMoveEvent(event);
}

void Line::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "Line::mouseReleaseEvent";
    selected = false;
    update();
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

void Line::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(selectable){
        setCursor(Qt::OpenHandCursor);
        QGraphicsItem::hoverMoveEvent(event);
    }
}

void Line::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
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

void Line::onSceneMoveableChanged(bool moveable)
{
    this->moveable = moveable;
    setFlag(QGraphicsItem::ItemIsMovable, moveable);
}

void Line::typechange()
{
//    qDebug()<<"状态改变了";
    itemp = this->lineproperties->getOk();
    if(itemp)
    {
        this->setPen(this->lineproperties->getPen());
    }
}

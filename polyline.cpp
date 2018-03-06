#include "polyline.h"
#include <QCursor>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QDebug>

PolyLine::PolyLine(QGraphicsItem *parent) :
    QGraphicsPathItem(parent),
    type(line)
{
    setShapeType(Shape::PolyLine);
    // 设置图元为可焦点的
    setFlag(QGraphicsItem::ItemIsFocusable);
    // 设置图元为可移动的
    setFlag(QGraphicsItem::ItemIsMovable, false);
    // 设置图元为可接受拖拽事件
    setAcceptDrops(true);
    // 设置图元为可接受hover事件
    setAcceptHoverEvents(true);
}

void PolyLine::startDraw(QGraphicsSceneMouseEvent *event)
{
    QPen pen = QPen();
    pen.setColor(penStyle.color);
    pen.setStyle(penStyle.style);
    pen.setWidthF(penStyle.width);
    setPen(pen);
    QPointF point = event->scenePos();
    points.append(point);
}

void PolyLine::drawing(QGraphicsSceneMouseEvent *event)
{
    newPoint = event->scenePos();
    update();
}

bool PolyLine::updateFlag(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    QPointF point = event->scenePos();
    points.append(point);
    update();
    return overFlag;
}

void PolyLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
    painter->setRenderHint(QPainter::Antialiasing);
    // 如果已经结束绘图则直接绘原图
    if(overFlag){
        setPath(this->path());
    }
    //  如果点集为空，则返回
    if(points.isEmpty()){
        return;
    }
    int offset = 2;
    int len = points.length();
    QPainterPath path(points.at(0));
    drawRectPoint(painter, points.at(0), offset);
    switch (type) {
    case line:
        for (int i = 0; i < len - 1; ++i) {
            painter->setBrush(QBrush(penStyle.color));
            drawRectPoint(painter, points.at(i), offset);
            painter->setBrush(QBrush());
            path.lineTo(points.at(i+1));
        }
        if(!overFlag){
            path.lineTo(newPoint);
        }
        break;
    case curve:

        break;
    case cubic:
        for (int i = 0; i < len - 1; ++i) {
            // 控制点的 x 坐标为 sp 与 ep 的 x 坐标和的一半
            // 第一个控制点 c1 的 y 坐标为起始点 sp 的 y 坐标
            // 第二个控制点 c2 的 y 坐标为结束点 ep 的 y 坐标
            QPointF sp = points.at(i);
            QPointF ep = points.at(i+1);

            painter->setBrush(QBrush(penStyle.color));
            drawRectPoint(painter, points.at(0), 1);
            painter->setBrush(QBrush());

            QPointF c1 = QPointF((sp.rx() + ep.rx()) / 2, sp.ry());
            QPointF c2 = QPointF((sp.rx() + ep.rx()) / 2, ep.ry());
            path.cubicTo(c1, c2, ep);
        }
        if(!overFlag){
            QPointF sp = points.at(len-1);
            QPointF ep = newPoint;
            QPointF c1 = QPointF((sp.rx() + ep.rx()) / 2, sp.ry());
            QPointF c2 = QPointF((sp.rx() + ep.rx()) / 2, ep.ry());
            path.cubicTo(c1, c2, ep);
        }
        break;
    }
//    const QPointF* head = &points.at(0);
//    painter->drawPolyline(head, points.length());
    painter->drawPath(path);
    setPath(path);
}

void PolyLine::setType(PolyLine::Type type)
{
    this->type = type;
}

PolyLine::Type PolyLine::getType()
{
    return this->type;
}

void PolyLine::mousePressEvent(QGraphicsSceneMouseEvent *event)
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

void PolyLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "PolyLine::mouseMoveEvent";
    QGraphicsItem::mouseMoveEvent(event);
}

void PolyLine::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "PolyLine::mouseReleaseEvent";
    QGraphicsItem::mouseReleaseEvent(event);
}

void PolyLine::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "PolyLine::dragEnterEvent";
    QGraphicsItem::dragEnterEvent(event);
}

void PolyLine::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "PolyLine::dragLeaveEvent";
    QGraphicsItem::dragLeaveEvent(event);
}

void PolyLine::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "PolyLine::dragMoveEvent";
    QGraphicsItem::dragMoveEvent(event);
}

void PolyLine::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "PolyLine::dropEvent";
    QGraphicsItem::dropEvent(event);
}

void PolyLine::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
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

void PolyLine::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(selectable){
        setCursor(Qt::OpenHandCursor);
        QGraphicsItem::hoverMoveEvent(event);
    }
}

void PolyLine::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
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

void PolyLine::onSceneMoveableChanged(bool moveable)
{
    this->moveable = moveable;
    setFlag(QGraphicsItem::ItemIsMovable, moveable);
}


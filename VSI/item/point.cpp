#include "point.h"
#include <QCursor>
#include <QPainter>
#include <QPen>
#include "qmath.h"
#include <QDebug>

Point::Point(QGraphicsItem *parent) :
    QGraphicsItem(parent),
    offset(1),
    ctype(normal)
{
    setShapeType(Shape::Point);
    // 设置图元为可焦点的
    setFlag(QGraphicsItem::ItemIsFocusable);
    // 设置图元为可移动的
    setFlag(QGraphicsItem::ItemIsMovable, false);
    // 设置图元为可接受拖拽事件
    setAcceptDrops(true);
    // 设置图元为可接受hover事件
    setAcceptHoverEvents(true);
}

void Point::startDraw(QGraphicsSceneMouseEvent *event)
{
    QPen pen = QPen();
    pen.setColor(penStyle.color);
    pen.setStyle(penStyle.style);
    pen.setWidthF(penStyle.width);
    setPen(pen);

    pos = event->scenePos();
    update();
    overFlag = true;  // 马上就要结束
}

void Point::drawing(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

bool Point::updateFlag(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    return overFlag;
}

void Point::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
    drawCrossPoint(painter, pos, offset, normal);
}

QRectF Point::boundingRect() const
{
    QPointF p = this->pos;
    return QRectF(p.rx()-offset,
                  p.ry()-offset,
                  offset * 2,
                  offset * 2);
}

QPainterPath Point::shape() const
{
    QPainterPath path;
    QPointF p = this->pos;
    path.addRect(QRectF(p.rx()-offset,
                        p.ry()-offset,
                        offset * 2,
                        offset * 2));
    return path;
}

bool Point::contains(const QPointF &point) const
{
    bool res = false;
    if (point == this->pos)
        res = true;
    return res;
}

void Point::setOffset(int offset)
{
    this->offset = offset;
}

int Point::getOffset()
{
    return this->offset;
}

QPen Point::pen() const
{
    return pen_style;
}

void Point::setPen(const QPen &pen)
{
    this->pen_style = pen;
}

QPointF Point::point() const
{
    return this->pos;
}

void Point::setPoint(const QPointF &point)
{
    QPen pen = QPen();
    pen.setColor(penStyle.color);
    pen.setStyle(penStyle.style);
    pen.setWidthF(penStyle.width);
    setPen(pen);

    this->pos = point;
}

int Point::type() const
{
    return Type;
}

crossType Point::getCrossType()
{
    return this->ctype;
}

Point *Point::copy()
{
    Point *p = new Point(this);
    p->setPoint(this->pos);
    return p;
}

void Point::mousePressEvent(QGraphicsSceneMouseEvent *event)
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
        emit select(this);
    }
    QGraphicsItem::mousePressEvent(event);
}

void Point::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "Point::mouseMoveEvent";
    QGraphicsItem::mouseMoveEvent(event);
}

void Point::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "Point::mouseReleaseEvent";
    QGraphicsItem::mouseReleaseEvent(event);
}

void Point::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Point::dragEnterEvent";
    QGraphicsItem::dragEnterEvent(event);
}

void Point::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Point::dragLeaveEvent";
    QGraphicsItem::dragLeaveEvent(event);
}

void Point::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Point::dragMoveEvent";
    QGraphicsItem::dragMoveEvent(event);
}

void Point::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Point::dropEvent";
    QGraphicsItem::dropEvent(event);
}

void Point::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
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

void Point::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(selectable){
        setCursor(Qt::OpenHandCursor);
        QGraphicsItem::hoverMoveEvent(event);
    }
}

void Point::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
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

void Point::onSceneMoveableChanged(bool moveable)
{
    this->moveable = moveable;
    setFlag(QGraphicsItem::ItemIsMovable, moveable);
}


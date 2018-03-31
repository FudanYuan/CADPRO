#include "polyline.h"
#include <QCursor>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QDebug>

Polyline::Polyline(QGraphicsItem *parent) :
    QGraphicsPathItem(parent),
    type(line),
    elevation(1),
    alpha(0),
    offset(QPointF())
{
    setShapeType(Shape::Polyline);
    // 设置图元为可焦点的
    setFlag(QGraphicsItem::ItemIsFocusable);
    // 设置图元为可移动的
    setFlag(QGraphicsItem::ItemIsMovable, false);
    // 设置图元为可接受拖拽事件
    setAcceptDrops(true);
    // 设置图元为可接受hover事件
    setAcceptHoverEvents(true);
}

void Polyline::startDraw(QGraphicsSceneMouseEvent *event)
{
    QPen pen = QPen();
    pen.setColor(penStyle.color);
    pen.setStyle(penStyle.style);
    pen.setWidthF(penStyle.width);
    setPen(pen);
    QPointF point = event->scenePos();
    points.append(point);
}

void Polyline::drawing(QGraphicsSceneMouseEvent *event)
{
    newPoint = event->scenePos();
    update();
}

bool Polyline::updateFlag(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    QPointF point = event->scenePos();
    points.append(point);
    update();
    return overFlag;
}

void Polyline::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    scaleFactor = painter->matrix().m11();
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
    int size = 2;
    int len = points.length();
    QPainterPath path(points.at(0));
    switch (type) {
    case line:
        for (int i = 0; i < len - 1; ++i) {
            painter->setBrush(QBrush(penStyle.color));
            //drawRectPoint(painter, points.at(i), size);
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
//    drawCrossPoint(painter, boundingRect().center(), 2, upright);
//    painter->drawRect(boundingRect());

    if(offset!=QPointF(0, 0)){
        qDebug() << offset;
    }

    if(alpha != 0){
        painter->save();
        painter->translate(boundingRect().center());
        painter->rotate(alpha); //以中心点为中心，顺时针旋转alpha度
        painter->translate(-boundingRect().center());
        painter->drawPath(path);
        setPath(path);
        painter->restore();
        return;
    }
//    drawRectPoint(painter, points.at(0), size);
    painter->drawPath(path);
    setPath(path);
}

//QRectF Polyline::boundingRect() const
//{
//    qreal xMin = LONG_MAX;
//    qreal xMax = LONG_MIN;
//    qreal yMin = LONG_MAX;
//    qreal yMax = LONG_MIN;
//    for(int i=0; i < points.length(); i++){
//        QPointF point = points[i];
//        qreal px = point.rx();
//        qreal py = point.ry();
//        if(px < xMin){
//            xMin = px;
//        }
//        if(px > xMax){
//            xMax = px;
//        }
//        if(py < yMin){
//            yMin = py;
//        }
//        if(py > yMax){
//            yMax = py;
//        }
//    }
//    QRectF rect(xMin, yMin, xMax-xMin, yMax-yMin);
//    // 如果图形旋转
//    if(alpha!=0){
//        xMin = LONG_MAX;
//        xMax = LONG_MIN;
//        yMin = LONG_MAX;
//        yMax = LONG_MIN;
//        QPointF center = rect.center();
//        for(int i=0; i < points.length(); i++){
//            QPointF point = points[i];
////            qDebug() << "原坐标" << point;
//            point = transformRotate(center, point, alpha);
////            qDebug() << "新坐标" << point;
////            qDebug() << "";
//            qreal px = point.rx();
//            qreal py = point.ry();
//            if(px < xMin){
//                xMin = px;
//            }
//            if(px > xMax){
//                xMax = px;
//            }
//            if(py < yMin){
//                yMin = py;
//            }
//            if(py > yMax){
//                yMax = py;
//            }
//        }
//        QRect newRect(xMin, xMax, yMin, yMax);
//        return newRect;
//    }
//    return rect;
//}

QPainterPath Polyline::shape() const
{
    return this->path();
}

void Polyline::setPolyline(QList<QPointF> pList, int flag, qreal ele, qreal angle, const QPointF off)
{
    QPen pen = QPen();
    pen.setColor(penStyle.color);
    pen.setStyle(penStyle.style);
    pen.setWidthF(penStyle.width);
    setPen(pen);

    points.append(pList);
    type = (Type)(flag == 0 ? 1 : flag);
    elevation = ele;
    alpha = angle;
    offset = off;
    update();
    overFlag = true;
}

void Polyline::setPoints(const QList<QPointF> &value)
{
    points = value;
}

QList<QPointF> Polyline::getPoints()
{
    return this->points;
}

void Polyline::setType(Polyline::Type type)
{
    this->type = type;
}

Polyline::Type Polyline::getType()
{
    return this->type;
}

void Polyline::setElevation(qreal elevation)
{
    this->elevation = elevation;
}

qreal Polyline::getElevation()
{
    return this->elevation;
}

Polyline *Polyline::copy()
{
    Polyline *p = new Polyline(this);
    p->setPolyline(points, type, elevation, alpha, offset);
    return p;
}

void Polyline::mousePressEvent(QGraphicsSceneMouseEvent *event)
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

void Polyline::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "Polyline::mouseMoveEvent";
    QGraphicsItem::mouseMoveEvent(event);
}

void Polyline::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "Polyline::mouseReleaseEvent";
    QGraphicsItem::mouseReleaseEvent(event);
}

void Polyline::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Polyline::dragEnterEvent";
    QGraphicsItem::dragEnterEvent(event);
}

void Polyline::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Polyline::dragLeaveEvent";
    QGraphicsItem::dragLeaveEvent(event);
}

void Polyline::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Polyline::dragMoveEvent";
    QGraphicsItem::dragMoveEvent(event);
}

void Polyline::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Polyline::dropEvent";
    QGraphicsItem::dropEvent(event);
}

void Polyline::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
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

void Polyline::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(selectable){
        setCursor(Qt::OpenHandCursor);
        QGraphicsItem::hoverMoveEvent(event);
    }
}

void Polyline::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
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

void Polyline::onSceneMoveableChanged(bool moveable)
{
    this->moveable = moveable;
    setFlag(QGraphicsItem::ItemIsMovable, moveable);
}


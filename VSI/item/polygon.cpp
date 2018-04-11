#include "polygon.h"
#include <QCursor>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <qmath.h>
#include <QDebug>

Polygon::Polygon(QGraphicsItem *parent) :
    QGraphicsPathItem(parent)
{
    setShapeType(Shape::Polygon);
    // 设置图元为可焦点的
    setFlag(QGraphicsItem::ItemIsFocusable);
    // 设置图元为可移动的
    setFlag(QGraphicsItem::ItemIsMovable, false);
    // 设置图元为可接受拖拽事件
    setAcceptDrops(true);
    // 设置图元为可接受hover事件
    setAcceptHoverEvents(true);
    movePointEdit =false;
    alpha=0;  // 旋转角度
    lineNum=4;//边的个数
    radius=100;//半径
    offset = 50;
    polygonproperties =new ItemProperties();
}

void Polygon::startDraw(QGraphicsSceneMouseEvent *event)
{
    QPen pen = QPen();
    pen.setColor(penStyle.color);
    pen.setStyle(penStyle.style);
    pen.setWidthF(penStyle.width);
    setPen(pen);
    cPoint = event->scenePos();
    overFlag = true;
}

void Polygon::drawing(QGraphicsSceneMouseEvent *event)
{
    cPoint = event->scenePos();
    update();
}

bool Polygon::updateFlag(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    return overFlag;
}

void Polygon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

    int r=radius;
    int num = lineNum;
    double angle=alpha;
    QPainterPath path;
    QPointF sPoint(cPoint.rx()+r*qCos(M_PI*angle/180),
                   cPoint.ry()+r*qSin(M_PI*angle/180));
    path.moveTo(sPoint);
    for(int i=1;i < num;i++){
       QPointF p(cPoint.rx()+r*qCos(2*M_PI/num * i+M_PI*angle/180),
                 cPoint.ry()+r*qSin(2*M_PI/num * i+M_PI*angle/180));
       path.lineTo(p);
    }
    path.lineTo(sPoint);
    painter->drawPath(path);
    setPath(path);
    drawCrossPoint(painter, cPoint, 5, upright);

    if(isoffset)
    {
        r=radius+offset;
        num = lineNum;
        angle=alpha;
        QPainterPath path;
        QPointF sPoint(cPoint.rx()+r*qCos(M_PI*angle/180),
                       cPoint.ry()+r*qSin(M_PI*angle/180));
        path.moveTo(sPoint);
        for(int i=1;i < num;i++){
           QPointF p(cPoint.rx()+r*qCos(2*M_PI/num * i+M_PI*angle/180),
                     cPoint.ry()+r*qSin(2*M_PI/num * i+M_PI*angle/180));
           path.lineTo(p);
        }
        path.lineTo(sPoint);
        painter->drawPath(path);
    }

    if(selected){
        pen.setColor(selectedEntity.color);
        painter->setPen(pen);
        QBrush brush = QBrush();
        brush.setColor(selectedEntity.color);
        brush.setStyle(Qt::SolidPattern);
        painter->setBrush(brush);
        drawRectPoint(painter,cPoint,objSize.snapDistanceInPix);
        qDebug()<<"移动点";
    }

}

QList<QPointF> Polygon::toPolyline()
{
    QList<QPointF> points;  // 各个点的坐标
    int r=radius;
    int num = lineNum;
    double angle=alpha;
    QPointF sPoint(cPoint.rx()+r*qCos(M_PI*angle/180),
                   cPoint.ry()+r*qSin(M_PI*angle/180));
    points.append(sPoint);
    for(int i=1;i < lineNum;i++){
       QPointF p(cPoint.rx()+r*qCos(2*M_PI/num * i+M_PI*angle/180),
                 cPoint.ry()+r*qSin(2*M_PI/num * i+M_PI*angle/180));
       points.append(p);
    }
    points.append(sPoint);
    return points;
}

void Polygon::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(selectable){
        selected = true;
        setCursor(Qt::ClosedHandCursor);
        if(!itemp)
        {
            qDebug() << "type: " << getShapeType();
            qDebug() << "id: " << getShapeId();
            QPen pen = QPen();
            pen.setColor(selectedEntity.color);
            pen.setStyle(selectedEntity.style);
            pen.setWidthF(selectedEntity.width);
            setPen(pen);
        }
        if(selected){
            qDebug()<<"可移动";
            setFlag(GraphicsItemFlag::ItemIsMovable, true);
            }
        else{
            qDebug()<<"不可移动";
            setFlag(GraphicsItemFlag::ItemIsMovable, false);
        }
        editOverFlag = false;
        emit select(this);
    }
    QGraphicsItem::mousePressEvent(event);
}

void Polygon::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "PolyLine::mouseMoveEvent";
    QGraphicsItem::mouseMoveEvent(event);
}

void Polygon::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "PolyLine::mouseReleaseEvent";
    QGraphicsItem::mouseReleaseEvent(event);
}

void Polygon::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "PolyLine::dragEnterEvent";
    QGraphicsItem::dragEnterEvent(event);
}

void Polygon::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "PolyLine::dragLeaveEvent";
    QGraphicsItem::dragLeaveEvent(event);
}

void Polygon::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "PolyLine::dragMoveEvent";
    QGraphicsItem::dragMoveEvent(event);
}

void Polygon::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "PolyLine::dropEvent";
    QGraphicsItem::dropEvent(event);
}

void Polygon::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if(selectable && !itemp){
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

void Polygon::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(selectable && !itemp){
        setCursor(Qt::OpenHandCursor);
        QGraphicsItem::hoverMoveEvent(event);
    }
}

void Polygon::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if(selectable && !itemp){
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

double Polygon::getOffset() const
{
    return offset;
}

void Polygon::setOffset(double value)
{
    offset = value;
}

void Polygon::onSceneMoveableChanged(bool moveable)
{
    this->moveable = moveable;
    setFlag(QGraphicsItem::ItemIsMovable, moveable);
}

void Polygon::typechange()
{
    if(itemp = this->polygonproperties->getOk())
    {
        this->setPen(this->polygonproperties->getPen());
        this->setPenStyle(this->polygonproperties->getPenstyle());
        isoffset = this->polygonproperties->getIsinsertoffset();
        this->setOffset(this->polygonproperties->getOffset());
        lineNum = this->polygonproperties->getPolygonEdge();
        radius = this->polygonproperties->getPolygonEdgeLength();
        alpha = this->polygonproperties->getPolygonRad();
    }

}

void Polygon::setAlpha(const qreal &value)
{
    alpha = value;
}

qreal Polygon::getAlpha() const
{
    return alpha;
}

void Polygon::setType(int type)
{
    this->type = type;
}

int Polygon::getType()
{
    return this->type;
}

void Polygon::setRadius(double value)
{
    radius = value;
}

int Polygon::getLineNum() const
{
    return lineNum;
}

double Polygon::getRadius() const
{
    return radius;
}

void Polygon::setLineNum(int value)
{
    lineNum = value;
}


void Polygon::on_commandLinkButton_2_clicked()
{

}

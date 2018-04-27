#include "eyelet.h"
#include <QCursor>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <qmath.h>
#include <QDebug>

Eyelet::Eyelet(QGraphicsItem *parent) :
    QGraphicsPathItem(parent)
{
    setShapeType(Shape::Eyelet);
    // 设置图元为可焦点的
    setFlag(QGraphicsItem::ItemIsFocusable);
    // 设置图元为可移动的
    setFlag(QGraphicsItem::ItemIsMovable, false);
    // 设置图元为可接受拖拽事件
    setAcceptDrops(true);
    // 设置图元为可接受hover事件
    setAcceptHoverEvents(true);
    eyeletdialog =new EyeletDialog();
    eyeletproperties = new ItemProperties();
    eyeletAlpha = 0;
}

void Eyelet::startDraw(QGraphicsSceneMouseEvent *event)
{
    QPen pen = QPen();
    pen.setColor(penStyle.color);
    pen.setStyle(penStyle.style);
    pen.setWidthF(penStyle.width);
    setPen(pen);

    overFlag = true;
}

void Eyelet::drawing(QGraphicsSceneMouseEvent *event)
{
    cPoint = event->scenePos();
    update();
}

bool Eyelet::updateFlag(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    return overFlag;
}

void Eyelet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    scaleFactor =  painter->matrix().m11();
    QPen pen = this->pen();
    pen.setWidthF(pen.widthF() / scaleFactor);
    painter->setPen(pen);

    //绘制鸡眼孔
    int width = eyeletW;//鸡眼孔宽度
    int height = eyeletH;//鸡眼孔高度
    double alpha = qAbs(eyeletAlpha);
    QPointF sPoint;
    QPainterPath path;

    sPoint.setX(cPoint.rx()+width*qCos(M_PI*alpha/180));sPoint.setY(cPoint.ry()+width*qSin(M_PI*alpha/180));
    path.moveTo(cPoint);
    path.lineTo(sPoint);
    path.arcTo(sPoint.rx()+height/2*qSin(qAbs(M_PI*alpha/180))-height/2,sPoint.ry()-height/2*qCos(qAbs(M_PI*alpha/180))-height/2,height,height,-90-alpha,180);//图形左上角为点，宽和高，以某个角度开始，划过多少度
    sPoint.setX(cPoint.rx()+height*qSin(M_PI*alpha/180));sPoint.setY(cPoint.ry()-height*qCos(M_PI*alpha/180));
    path.lineTo(sPoint);
    path.arcTo(sPoint.rx()-height/2*qSin(qAbs(M_PI*alpha/180))-height/2,sPoint.ry()+height/2*qCos(qAbs(M_PI*alpha/180))-height/2,height,height,90-alpha,180);

    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    painter->drawPath(path);

    path.lineTo(sPoint);
    path.moveTo(cPoint.rx()+width*qCos(qAbs(M_PI*alpha/180)),cPoint.ry()+width*qSin(qAbs(M_PI*alpha/180)));
    sPoint.setX(cPoint.rx()+width*qCos(qAbs(M_PI*alpha/180))+height*qSin(qAbs(M_PI*alpha/180)));sPoint.setY(cPoint.ry()+width*qSin(qAbs(M_PI*alpha/180))-height*qCos(qAbs(M_PI*alpha/180)));
    path.lineTo(sPoint);

    pen.setStyle(Qt::DotLine);
    painter->setPen(pen);
    painter->drawPath(path);
    setPath(path);
}

QList<QPointF> Eyelet::toPolyline()
{
    QList<QPointF> points;
    QPointF sPoint;
    int width = eyeletW;//鸡眼孔宽度
    int height = eyeletH;//鸡眼孔高度
    int yanben = 1000;//采样点数
    sPoint.setX(cPoint.rx()+width);sPoint.setY(cPoint.ry());
    points.append(cPoint);
    points.append(sPoint);
    double angle = -90.0;
    for(int i=0;i<yanben;i++)
    {
        QPointF p(cPoint.rx()+width+height/2*qCos(M_PI*angle/180),cPoint.ry()-height/2-height/2*qSin(M_PI*angle/180));
        angle = angle + (double)180/yanben;
        points.append(p);
    }
    sPoint.setX(cPoint.rx()+width);sPoint.setY(cPoint.ry()-height);
    points.append(sPoint);
    sPoint.setX(cPoint.rx());sPoint.setY(cPoint.ry()-height);
    points.append(sPoint);
    angle = 90.0;
    for(int i=0;i<yanben;i++)
    {
        QPointF p(cPoint.rx()+height/2*qCos(M_PI*angle/180),cPoint.ry()-height/2-height/2*qSin(M_PI*angle/180));
        angle = angle + (double)180/yanben;
        points.append(p);
    }
    return points;
}

void Eyelet::mousePressEvent(QGraphicsSceneMouseEvent *event)
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
            setFlag(GraphicsItemFlag::ItemIsMovable, true);
        }
        else{
            setFlag(GraphicsItemFlag::ItemIsMovable, false);
        }
        emit select(this);
    }
    QGraphicsItem::mousePressEvent(event);
}

void Eyelet::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "PolyLine::mouseMoveEvent";
    QGraphicsItem::mouseMoveEvent(event);
}

void Eyelet::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "PolyLine::mouseReleaseEvent";
    QGraphicsItem::mouseReleaseEvent(event);
}

void Eyelet::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "PolyLine::dragEnterEvent";
    QGraphicsItem::dragEnterEvent(event);
}

void Eyelet::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "PolyLine::dragLeaveEvent";
    QGraphicsItem::dragLeaveEvent(event);
}

void Eyelet::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "PolyLine::dragMoveEvent";
    QGraphicsItem::dragMoveEvent(event);
}

void Eyelet::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "PolyLine::dropEvent";
    QGraphicsItem::dropEvent(event);
}

void Eyelet::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
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

void Eyelet::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(selectable){
        setCursor(Qt::OpenHandCursor);
        QGraphicsItem::hoverMoveEvent(event);
    }
}

void Eyelet::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
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

double Eyelet::getEyeletAlpha() const
{
    return eyeletAlpha;
}

void Eyelet::setEyeletAlpha(double value)
{
    eyeletAlpha = value;
}

double Eyelet::getEyeletW() const
{
    return eyeletW;
}

void Eyelet::setEyeletW(double value)
{
    eyeletW = value;
}

double Eyelet::getEyeletH() const
{
    return eyeletH;
}

void Eyelet::setEyeletH(double value)
{
    eyeletH = value;
}

void Eyelet::onSceneMoveableChanged(bool moveable)
{
    this->moveable = moveable;
    setFlag(QGraphicsItem::ItemIsMovable, moveable);
}

void Eyelet::typechange()
{
    if(itemp = this->eyeletproperties->getOk())
    {
        this->setPen(this->eyeletproperties->getPen());
        this->setPenStyle(this->eyeletproperties->getPenstyle());
        this->eyeletH = this->eyeletproperties->getRectHeigth();
        this->eyeletW = this->eyeletproperties->getRectLength();
        this->eyeletAlpha = this->eyeletproperties->getEllipseAlpha();
    }
}

void Eyelet::on_commandLinkButton_2_clicked()
{

}

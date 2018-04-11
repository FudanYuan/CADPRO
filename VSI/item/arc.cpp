#include "arc.h"
#include <QCursor>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QtCore/qmath.h>
#include <QDebug>

Arc::Arc(QGraphicsItem *parent) :
    QGraphicsEllipseItem(parent),
    type(normal),
    fFlag(false),
    sFlag(false),
    tFlag(false),
    eFlag(false)

{
    setShapeType(Shape::Arc);
    // 设置图元为可焦点的
    setFlag(QGraphicsItem::ItemIsFocusable, false);
    // 设置图元为可移动的
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    // 设置图元为可接受拖拽事件
    setAcceptDrops(true);
    // 设置图元为可接受hover事件
    setAcceptHoverEvents(true);
    arcproperties = new ItemProperties;
}

void Arc::startDraw(QGraphicsSceneMouseEvent *event)
{
    QPen pen = QPen();
    pen.setColor(penStyle.color);
    pen.setStyle(penStyle.style);
    pen.setWidthF(penStyle.width);
    setPen(pen);
    fPoint = event->scenePos();
    fFlag = true;
}

void Arc::drawing(QGraphicsSceneMouseEvent *event)
{
    if(sFlag){
        eFlag = false;
        tPoint = event->scenePos();
        // 计算圆心、半径
        cPoint = getArcCenter(fPoint, tPoint, sPoint, r);

        // 如果三点共线，则画直线
        if(r == -1){
            eFlag = true;
            qDebug() << "三点共线";
            update();
            return;
        }
        setRect(cPoint.rx()-r, cPoint.ry()-r, r*2, r*2);
        // 计算起点角度和终点角度
        switch (type) {
        case normal:
            if (isClockWise(fPoint, tPoint, sPoint)){
                // 顺时针
                sAngle = getLineAngle(cPoint, sPoint);
                eAngle = getLineAngle(cPoint, fPoint);
            } else {
                // 逆时针
                sAngle = getLineAngle(cPoint, fPoint);
                eAngle = getLineAngle(cPoint, sPoint);
            }
            break;
        case updated:
            if (isClockWise(fPoint, sPoint, tPoint)){
                // 顺时针
                sAngle = getLineAngle(cPoint, tPoint);
                eAngle = getLineAngle(cPoint, fPoint);
            } else {
                // 逆时针
                sAngle = getLineAngle(cPoint, fPoint);
                eAngle = getLineAngle(cPoint, tPoint);
            }
            break;
        }

        if(eAngle - sAngle < 0){
            eAngle += 360;
        }
        angleRange = eAngle - sAngle;
        update();
        return;
    }
    if(fFlag){
        sPoint = event->scenePos();
        update();
    }
}

bool Arc::updateFlag(QGraphicsSceneMouseEvent *event)
{
    if(!sFlag){ // 确定第2个点
        sPoint = event->scenePos();
        sFlag = true;
        update();
        return false;
    }
    if(!tFlag){// 确定第3个点
        tPoint = event->scenePos();
        tFlag = true;
        overFlag = true;
        update();
    }
    return overFlag;
}

void Arc::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

    if(tFlag){
        if(eFlag){
            painter->setPen(pen);

            QLineF line = getLine(fPoint, sPoint, tPoint);
            if(line.isNull()){
                qDebug() << "line.isNull";
                return;
            }
            painter->drawLine(line);

            painter->setBrush(QBrush(penStyle.color));
            drawRectPoint(painter, line.p1(), 2);
            drawRectPoint(painter, line.p2(), 2);
        }
        else{
            painter->setPen(pen);
            painter->drawPoint(cPoint);
            drawCrossPoint(painter, cPoint, 5, upright);
            painter->drawArc(rect(), sAngle*16, angleRange*16);

//            drawRectPoint(painter, tPoint, 2);
        }
        return;
    }
    if(sFlag){
        // no error, draw arc
        if(!eFlag){
            painter->setPen(pen);
            painter->drawArc(rect(), sAngle*16, angleRange*16);
            drawCrossPoint(painter, cPoint, 5, upright);
        }
        // 画直线
        pen.setColor(Qt::black);
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
        painter->drawLine(fPoint, sPoint);

        if(type == updated){
            qDebug() << "updated";
            pen.setColor(Qt::black);
            pen.setStyle(Qt::DashLine);
            painter->setPen(pen);
            painter->drawLine(sPoint, tPoint);
        }
        return;
    }
    if(fFlag){ // 画直线
        pen.setColor(Qt::black);
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
        painter->drawLine(fPoint, sPoint);
        return;
    }
}

void Arc::setCPoint(QPointF p)
{
    this->cPoint = p;
}

QPointF Arc::getCPoint()
{
    return this->cPoint;
}

void Arc::setRadius(qreal r)
{
    this->r = r;
}

qreal Arc::getRadius()
{
    return this->r;
}

void Arc::setSAngle(qreal angle)
{
    this->sAngle = angle;
}

qreal Arc::getSAngle()
{
    return this->sAngle;
}

void Arc::setEAngle(qreal angle)
{
    this->eAngle= angle;
}

qreal Arc::getEAngle()
{
    return this->eAngle;
}

void Arc::setArc(qreal px, qreal py, qreal radius, qreal angle1, qreal angle2)
{
    QPen pen = QPen();
    pen.setColor(penStyle.color);
    pen.setStyle(penStyle.style);
    pen.setWidthF(penStyle.width);
    setPen(pen);

    cPoint.setX(px);
    cPoint.setY(py);
    r = radius;
    sAngle = angle1;
    eAngle = angle2;
    angleRange = angle2 - angle1;
    fPoint = cPoint;
    sPoint = transformRotate(cPoint, radius, angle1);
    tPoint = transformRotate(cPoint, radius, angle2);
    setRect(cPoint.rx()-r, cPoint.ry()-r, r*2, r*2);  // 可优化
    type = normal;
    fFlag = true;
    sFlag = true;
    tFlag = true;
    eFlag = false;
    update();
}

Arc Arc::arc()
{

}

Arc *Arc::copy()
{
    Arc *a = new Arc(this);
    a->setArc(cPoint.rx(), cPoint.ry(), r, sAngle, eAngle);
    return a;
}

void Arc::setType(Arc::Type type)
{
    this->type = type;
}

Arc::Type Arc::getType()
{
    return this->type;
}

void Arc::mousePressEvent(QGraphicsSceneMouseEvent *event)
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

void Arc::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "Arc::mouseMoveEvent";
    QGraphicsItem::mouseMoveEvent(event);
}

void Arc::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "Arc::mouseReleaseEvent";
    QGraphicsItem::mouseReleaseEvent(event);
}

void Arc::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Arc::dragEnterEvent";
    QGraphicsItem::dragEnterEvent(event);
}

void Arc::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Arc::dragLeaveEvent";
    QGraphicsItem::dragLeaveEvent(event);
}

void Arc::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Arc::dragMoveEvent";
    QGraphicsItem::dragMoveEvent(event);
}

void Arc::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Arc::dropEvent";
    QGraphicsItem::dropEvent(event);
}

void Arc::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::ArrowCursor);
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

void Arc::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::ArrowCursor);
    if(selectable){
        setCursor(Qt::OpenHandCursor);
        QGraphicsItem::hoverMoveEvent(event);
    }
}

void Arc::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::ArrowCursor);
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

QPointF Arc::getArcCenter(QPointF p1, QPointF p2, QPointF p3, qreal &r)
{
    qreal xysm, xyse, xy;
    qreal cPointX, cPointY;
    xy = qPow(p1.rx(), 2) + qPow(p1.ry(), 2);
    xyse = xy - qPow(p3.rx(), 2) - qPow(p3.ry(), 2);
    xysm = xy - qPow(p2.rx(), 2) - qPow(p2.ry(), 2);
    xy = (p1.rx() - p2.rx()) * (p1.ry() - p3.ry()) - (p1.rx() - p3.rx()) * (p1.ry() - p2.ry());
    // 判断参数有效性
    if (qAbs(xy)<0.000001)
    {
        r = -1;
        return QPointF(0, 0);
    }

    // 获得圆心和半径
    cPointX = ((xysm * (p1.ry() - p3.ry())) - (xyse * (p1.ry() - p2.ry()))) / (2 * xy);
    cPointY = ((xyse * (p1.rx() - p2.rx())) - (xysm * (p1.rx() - p3.rx()))) / (2 * xy);
    r = qSqrt((p1.rx() - cPointX) * (p1.rx() - cPointX) + (p1.ry() - cPointY) * (p1.ry() - cPointY));
    if (r<0.000001)
    {
        r = -1;
        return QPointF(0, 0);
    }

    // 函数返回圆心的位置，而半径在参数中通过引用方式返回
    return QPointF(cPointX, cPointY);
}

//!
//! 判断是否顺时针方向
//! \brief Arc::isClockWise
//! \param pc
//! \param p1
//! \param p2
//! \param p3
//! \return
//!
bool Arc::isClockWise(QPointF p1, QPointF p2, QPointF p3)
{
    qreal res = (p2.rx() - p1.rx()) * (p3.ry() - p2.ry()) - (p2.ry() - p1.ry()) * (p3.rx() - p2.rx());
    bool isClock = res > 0;
    return isClock;
}

bool Arc::isClockWise(QPointF pc, QPointF p1, QPointF p2, QPointF p3)
{
    qreal a1, a2, a3;
    a1 = getLineAngle(pc, p1);
    a2 = getLineAngle(pc, p2);
    a3 = getLineAngle(pc, p3);
    bool isClock = (a1 < a2) | (a2 < a3) | (a1 < a3);
    return isClock;
}

QLineF Arc::getLine(QPointF p1, QPointF p2, QPointF p3)
{
    int i = 0;
    while(i++ < 3){
        if (qMin(p1.rx() , p2.rx()) <= p3.rx()
                && p3.rx() <= qMax(p1.rx() , p2.rx())
                && qMin(p1.ry() , p2.ry()) <= p3.ry()
                && p3.ry() <= qMax(p1.ry() , p2.ry())){
            return QLineF(p1, p2);
        }
        else{
            QPointF p = p1;
            p1 = p2;
            p2 = p3;
            p3 = p;
        }
    }
    return QLineF();
}

//!
//! 返回两点的夹角[0, 360]
//! \brief Arc::getLineAngle
//! \param sPoint
//! \param ePoint
//! \return
//!
qreal Arc::getLineAngle(QPointF sPoint, QPointF ePoint)
{
    qreal X, Y;
    qreal lineAngle = 0.0;
    sPoint = transformY(sPoint);
    ePoint = transformY(ePoint);
    X = ePoint.rx() - sPoint.rx();
    Y = ePoint.ry() - sPoint.ry();
    if (X > 0 && Y >= 0)  // 第一象限
    {
        lineAngle = qAtan(Y / X);
    }
    else if (X < 0 && Y >= 0) // 第二象限
    {
        lineAngle = M_PI + qAtan(Y / X);
    }
    else if (X < 0 && Y <= 0) // 第三象限
    {
        lineAngle = M_PI + qAtan(Y / X);
    }
    else if (X > 0 && Y <= 0)  // 第四象限
    {
        lineAngle = 2 * M_PI + qAtan(Y / X);
    }
    else if (X == 0)
    {
        lineAngle = M_PI_2;
    }
    // 返回degree
    return lineAngle * 180.0 / M_PI;
}

void Arc::onSceneMoveableChanged(bool moveable)
{
    this->moveable = moveable;
    setFlag(QGraphicsItem::ItemIsMovable, moveable);
}

void Arc::typechange()
{
    itemp = this->arcproperties->getOk();
    if(itemp)
    {
        this->setPen(this->arcproperties->getPen());
        this->setPenStyle(this->arcproperties->getPenstyle());
    }
}

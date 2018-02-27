#include "arc.h"
#include <QCursor>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QtCore/qmath.h>
#include <QDebug>
#include <QMessageBox>

Arc::Arc(QGraphicsItem *parent) :
    QGraphicsEllipseItem(parent)
{
    setShapeType(Shape::Ellipse);
    // 设置图元为可焦点的
    setFlag(QGraphicsItem::ItemIsFocusable, false);
    // 设置图元为可移动的
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    // 设置图元为可接受拖拽事件
    setAcceptDrops(true);
    // 设置图元为可接受hover事件
    setAcceptHoverEvents(true);
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
    if(tFlag){
        Q_UNUSED(event);
        // 计算圆心、半径
        cPoint = getArcCenter(fPoint, tPoint, sPoint, r);
        if(cPoint.isNull()){
            qDebug() << "QPointF(0,0)";
            return;
        }
        setRect(cPoint.rx()-r, cPoint.ry()-r, r*2, r*2);
        // 计算起点角度和终点角度
        if (isClockWise(cPoint, fPoint, tPoint, sPoint)){
            qDebug() << "顺时针";
            sAngle = getLineAngle(cPoint, sPoint) - 360.0;
            eAngle = getLineAngle(cPoint, fPoint);
        } else {
            qDebug() << "逆时针";
            sAngle = getLineAngle(cPoint, fPoint);
            eAngle = getLineAngle(cPoint, sPoint);
        }
        update();
        return;
    }
    if(sFlag){
        tPoint = event->scenePos();
        // 计算圆心、半径
        cPoint = getArcCenter(fPoint, tPoint, sPoint, r);
        if(cPoint.isNull()){
            qDebug() << "QPointF(0,0)";
            return;
        }
        setRect(cPoint.rx()-r, cPoint.ry()-r, r*2, r*2);
        // 计算起点角度和终点角度
        if (isClockWise(cPoint, fPoint, tPoint, sPoint)){
            qDebug() << "顺时针";
            sAngle = getLineAngle(cPoint, sPoint) - 360.0;
            eAngle = getLineAngle(cPoint, fPoint);
        } else {
            qDebug() << "逆时针";
            sAngle = getLineAngle(cPoint, fPoint);
            eAngle = getLineAngle(cPoint, sPoint);
        }
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
        // 此处就可确定
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
    QPen myPen = this->pen();
    // 重新设置画笔线宽;
    myPen.setWidthF(myPen.widthF() / scaleFactor);
    if(tFlag){
        if(!overFlag){
            qDebug() << cPoint << " " << sAngle << "   " << eAngle;
        }
        painter->setPen(myPen);
        painter->drawPoint(cPoint);

        qreal x0 = cPoint.rx();
        qreal y0 = cPoint.ry();
        qreal x1 = cPoint.rx() - 5;
        qreal x2 = cPoint.rx() + 5;
        qreal y1 = cPoint.ry() - 5;
        qreal y2 = cPoint.ry() + 5;
        painter->drawLine(QPointF(x1, y0), QPointF(x2, y0));
        painter->drawLine(QPointF(x0, y1), QPointF(x0, y2));

        painter->drawArc(rect(), sAngle*16, (eAngle - sAngle)*16);
        return;
    }
    if(sFlag){
        if(!overFlag){
            qDebug() << cPoint << " " << sAngle << "   " << eAngle;
        }
        painter->setPen(myPen);
        painter->drawPoint(cPoint);

        qreal x0 = cPoint.rx();
        qreal y0 = cPoint.ry();
        qreal x1 = cPoint.rx() - 5;
        qreal x2 = cPoint.rx() + 5;
        qreal y1 = cPoint.ry() - 5;
        qreal y2 = cPoint.ry() + 5;
        painter->drawLine(QPointF(x1, y0), QPointF(x2, y0));
        painter->drawLine(QPointF(x0, y1), QPointF(x0, y2));

        painter->drawArc(rect(), sAngle*16, (eAngle - sAngle)*16);
        myPen.setColor(Qt::black);
        myPen.setStyle(Qt::DashLine);
        painter->setPen(myPen);
        painter->drawLine(fPoint, sPoint);
        return;
    }
    if(fFlag){ // 画直线
        myPen.setColor(Qt::black);
        myPen.setStyle(Qt::DashLine);
        painter->setPen(myPen);
        painter->drawLine(fPoint, sPoint);
        return;
    }
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
    if(selectable){
        setCursor(Qt::OpenHandCursor);
        QGraphicsItem::hoverMoveEvent(event);
    }
}

void Arc::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
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
        qDebug() << "qAbs(xy)<0.000001";
        QMessageBox::warning(NULL, tr("错误"), "qAbs(xy)<0.000001", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        overFlag = true;
        return QPointF(0, 0);
    }

    // 获得圆心和半径
    cPointX = ((xysm * (p1.ry() - p3.ry())) - (xyse * (p1.ry() - p2.ry()))) / (2 * xy);
    cPointY = ((xyse * (p1.rx() - p2.rx())) - (xysm * (p1.rx() - p3.rx()))) / (2 * xy);
    r = qSqrt((p1.rx() - cPointX) * (p1.rx() - cPointX) + (p1.ry() - cPointY) * (p1.ry() - cPointY));
    if (r<0.000001)
    {
        qDebug() << "r<0.000001";
        QMessageBox::warning(NULL, tr("错误"), "r<0.000001", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        overFlag = true;
        return QPointF(0, 0);
    }

    // 函数返回圆心的位置，而半径在参数中通过引用方式返回
    return QPointF(cPointX, cPointY);
}

bool Arc::isClockWise(QPointF pc, QPointF p1, QPointF p2, QPointF p3)
{
//    qreal a1, a2, a3;
//    a1 = getLineAngle(pc, p1);
//    a2 = getLineAngle(pc, p2);
//    a3 = getLineAngle(pc, p3);
//    bool isClock = (a1 < a2) | (a2 < a3) | (a1 < a3);

    qreal res = (p2.rx() - p1.rx()) * (p3.ry() - p2.ry()) - (p2.ry() - p1.ry()) * (p3.rx() - p2.rx());
    bool isClock = res > 0;
    return isClock;
}

qreal Arc::getLineAngle(QPointF sPoint, QPointF ePoint)
{
    qreal X, Y;
    qreal lineAngle = 0.0;
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

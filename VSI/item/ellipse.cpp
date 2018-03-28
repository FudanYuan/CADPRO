#include "ellipse.h"
#include <QCursor>
#include <QPainter>
#include <QPen>
#include <qmath.h>
#include <QDebug>

Ellipse::Ellipse(QGraphicsItem *parent) :
    QGraphicsEllipseItem(parent),
    alpha(0),
    cFlag(false),
    r1Flag(false),
    r2Flag(false)
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
        alpha = getLineAngle(cPoint, sPoint);
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
    QPen pen = this->pen();
    // 重新设置画笔线宽;
    pen.setWidthF(pen.widthF() / scaleFactor);
    if(r2Flag){
        painter->setPen(pen);
        drawCrossPoint(painter, cPoint, 2, upright);

        // 准圆半径：R = sqrt(r1*r1+r2*r2)；
        // 准圆圆心：cPoint
        // 外切矩形（旋转前, 绕椭圆中心旋转）
        // topLeft: (cPoint.rx(), R)
        // w: R*cos(theta)  0 <= theta <=  90
        // h: R*sin(theta)
        qreal R = qSqrt(qPow(r1, 2) + qPow(r2, 2));

        qreal W = qAbs(R * qSin(alpha));
        qreal H = qAbs(R * qCos(alpha));

        if(alpha == 0){
            W = 2 * r1;
            H = 2 * r2;
        } else if(alpha < 90){
            W = 2 * R * qCos(alpha);
            H = 2 * R * qSin(alpha);
        } else if(alpha == 90){
            W = 2 * r2;
            H = 2 * r1;
        }
        painter->save();
        painter->translate(cPoint);
        painter->rotate(-alpha); //以中心点为中心，顺时针旋转alpha
        painter->drawEllipse(QRect(-r1, -r2, 2*r1, 2*r2));
        painter->restore();

        setRect(QRect(cPoint.rx()-W/2, cPoint.ry()-H/2, W, H));
//        painter->drawRect(QRect(cPoint.rx()-W/2, cPoint.ry()-H/2, W, H));
        return;
    }
    if(r1Flag){
        painter->setPen(pen);
        drawCrossPoint(painter, cPoint, 2, upright);

        if(r1 > r2){
            qDebug() << "r1 > r2" << alpha;
        } else{
            qDebug() << "r1 < r2" << alpha;
        }
        painter->save();
        painter->translate(cPoint);
        painter->rotate(-alpha); //以原点为中心，顺时针旋转alpha度
        painter->drawEllipse(QRect(-r1, -r2, 2*r1, 2*r2));
        painter->restore();

        pen.setColor(Qt::black);
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
        painter->drawLine(cPoint, sPoint);
        painter->drawLine(cPoint, ePoint);
        return;
    }
    if(cFlag){ // 画直线
        pen.setColor(Qt::black);
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
        painter->drawPoint(cPoint);
        painter->drawLine(cPoint, sPoint);
        return;
    }
}

qreal Ellipse::getLineAngle(QPointF sPoint, QPointF ePoint)
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

void Ellipse::setCPoint(QPointF p)
{
    this->cPoint = p;
}

QPointF Ellipse::getCPoint()
{
    return this->cPoint;
}

void Ellipse::setRadius1(qreal r1)
{
    this->r1 = r1;
}

qreal Ellipse::getRadius1()
{
    return this->r1;
}

void Ellipse::setRadius2(qreal r2)
{
    this->r2 = r2;
}

qreal Ellipse::getRadius2()
{
    return this->r2;
}

void Ellipse::setAlpha(qreal a)
{
    this->alpha = a;
}

qreal Ellipse::getAlpha()
{
    return this->alpha;
}

void Ellipse::setEllipse(qreal px, qreal py, qreal radius1, qreal radius2, qreal angle)
{
    QPen pen = QPen();
    pen.setColor(penStyle.color);
    pen.setStyle(penStyle.style);
    pen.setWidthF(penStyle.width);
    setPen(pen);

    cPoint.setX(px);
    cPoint.setY(py);
    r1 = radius1;
    r2 = radius2;
    alpha = angle;
    sPoint = transformRotate(cPoint, r1, alpha);
    ePoint = transformRotate(cPoint, r2, 90 - alpha);  //// 不确定
    setRect(cPoint.rx()-r1, cPoint.ry()-r2, r1*2, r2*2);
    cFlag = true;
    r1Flag = true;
    r2Flag = true;
    update();
}

Ellipse Ellipse::ellipse()
{

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
        emit select(this);
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

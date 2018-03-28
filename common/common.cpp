#include "common.h"
#include <QDebug>

QColor intToColor(const int rgb, bool a)
{
    //将Color 从int 转换成 QColor
    int red = rgb & 255;
    int green = rgb >> 8 & 255;
    int blue = rgb >> 16 & 255;
    int alpha = 255;
    if(a){
        alpha = rgb >> 24 & 255;
    }
    return QColor(red, green, blue, alpha);
}

void drawCrossPoint(QPainter *painter, QPointF point, int offset=2, crossType type=upright)
{
    qreal x0 = point.rx();
    qreal y0 = point.ry();
    switch (type) {
    case normal:
        painter->drawLine(QPointF(x0-offset, y0+offset), QPointF(x0+offset, y0-offset));
        painter->drawLine(QPointF(x0+offset, y0+offset), QPointF(x0-offset, y0-offset));
        break;
    case upright:
        painter->drawLine(QPointF(x0, y0+offset), QPointF(x0, y0-offset));
        painter->drawLine(QPointF(x0+offset, y0), QPointF(x0-offset, y0));
        break;
    }
}

void drawNodePoint(QPainter *painter, QPointF point, int radius=2)
{
    painter->drawEllipse(point, radius, radius);
}

void drawRectPoint(QPainter *painter, QPointF point, int length=2)
{
    qreal x0 = point.rx();
    qreal y0 = point.ry();
    painter->drawRect(x0-length, y0-length, 2*length, 2*length);
}

void drawLineWithArrow(QPainter *painter, QLineF line, int offset)
{
    if(line.dx() == line.dy()){
        return;
    }
    QLineF v = line.unitVector();
    v.setLength(offset);
    v.translate(QPointF(line.dx(), line.dy()));

    QLineF n = v.normalVector();
    n.setLength(n.length() * 0.5);
    QLineF n2 = n.normalVector().normalVector();

    QPointF p1 = v.p2();
    QPointF p2 = n.p2();
    QPointF p3 = n2.p2();

    painter->drawLine(line);
    QVector<QPointF> vector;
    vector << p1 << p2 << p3 << p1;
    QPolygonF polygon(vector);
    painter->drawPolygon(polygon);
}

//QRectF getLineBoundingRect(QLineF line)
//{
//    qreal deltaX = line.p1().rx() - line.p2().rx();
//    qreal deltaY = line.p1().ry() - line.p2().ry();
//    qreal len = qSqrt(qPow(deltaX, 2) + qPow(deltaY, 2));
//    if(line.angle() >= 0 || line.angle() <= 90){
//        //setRect(line.p1().rx()-4, line.p1.ry()+4, );
//    }
//}

QPointF transformY(QPointF p)
{
    return QPointF(p.rx(), -p.ry());
}

QPointF transformRotate(QPointF o, qreal r, qreal angle)
{
    QPointF res(o.rx()+r*qCos(M_PI*angle/180),
                o.ry()+r*qSin(M_PI*angle/180));
    return res;
}
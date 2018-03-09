#include "common.h"
#include <QDebug>

QColor intToColor(int rgb, bool a)
{
    //将Color 从int 转换成 QColor
    int blue = rgb & 255;
    int green = rgb >> 8 & 255;
    int red = rgb >> 16 & 255;
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

QPointF transformRotate(QPointF p, QPointF o, qreal angle)
{
    // 绕(x2, y2)旋转
    // x=(x1-x2)cosθ-(y1-y2)sinθ+x2;
    // y=(y1-y2)cosθ+(x1-x2)sinθ+y2;
    // 首先将点转化为标准坐标系
    p = transformY(p);
    o = transformY(o);
    qreal px = p.rx();
    qreal py = p.ry();
    qreal ox = o.rx();
    qreal oy = o.ry();
    QPointF res = QPointF(((px - ox) * qCos(angle) - (py - oy) * qSin(angle)) + ox,
                          ((px - ox) * qSin(angle) + (py - oy) * qCos(angle)) + oy);
    return transformY(res);

    // 首先将点转化为标准坐标系
//    p = transformY(p);
//    qreal x = p.rx();
//    qreal y = p.ry();
//    qreal tx = -x;
//    qreal ty = -y;
//    qreal x0 = 0;
//    qreal y0 = 0;
//    qreal cosAngle = qCos(angle);
//    qreal sinAngle = qSin(angle);

//    QPointF res = QPointF(0,0);
//    return transformY(res);
}

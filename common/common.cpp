#include "common.h"
#include <QDebug>

void str2int(int &int_temp, const std::string &string_temp)
{
    std::stringstream stream(string_temp);
    stream>>int_temp;
}

void str2float(float &float_temp, const std::string &string_temp)
{
    std::stringstream stream(string_temp);
    stream>>float_temp;
}

void str2double(double &double_temp, const std::string &string_temp)
{
    std::stringstream stream(string_temp);
    stream>>double_temp;
}

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

//QRectF getLineBoundingRect(QLineF line)
//{
//    qreal deltaX = line.p1().rx() - line.p2().rx();
//    qreal deltaY = line.p1().rx() - line.p2().rx();
//    qreal len = qSqrt(qPow(deltaX, 2) + qPow(deltaY, 2));
//    if(line.angle() >= 0 || line.angle() <= 90){
//        //setRect(line.p1().rx()-4, line.p1.rx()+4, );
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

QPointF transformRotate(QPointF o, QPointF p, qreal angle)
{
    // 将点映射到普通平面直角坐标系
    qreal ox = o.rx();
    qreal oy = -o.ry();
    qreal px = p.rx();
    qreal py = -p.ry();
    qreal a = M_PI*angle/180;
    qreal rx= (px - ox)*qCos(a) - (py - oy)*qSin(a) + ox;
    qreal ry= (px - ox)*qSin(a) + (py - oy)*qCos(a) + oy;
    return QPointF(rx, -ry);
}

qreal getDistance(QPointF p1, QPointF p2)
{
    qreal px1 = p1.rx();
    qreal py1 = p1.ry();

    qreal px2 = p2.rx();
    qreal py2 = p2.ry();

    qreal mid1 = qPow((px2-px1), 2);
    qreal mid2 = qPow((py2-py1), 2);
    qreal dis = qSqrt(mid1 + mid2);

    return dis;
}

void drawCrossPoint(QPainter *painter, QPointF point, int offset=2, crossType type=upright)
{
    qreal rx = point.rx();
    qreal ry = point.ry();
    switch (type) {
    case normal:
        painter->drawLine(QPointF(rx-offset, ry+offset), QPointF(rx+offset, ry-offset));
        painter->drawLine(QPointF(rx+offset, ry+offset), QPointF(rx-offset, ry-offset));
        break;
    case upright:
        painter->drawLine(QPointF(rx, ry+offset), QPointF(rx, ry-offset));
        painter->drawLine(QPointF(rx+offset, ry), QPointF(rx-offset, ry));
        break;
    default:
        break;
    }
}

void drawNodePoint(QPainter *painter, QPointF point, int radius=2)
{
    painter->drawEllipse(point, radius, radius);
}

void drawRectPoint(QPainter *painter, QPointF point, int length=2)
{
    qreal rx = point.rx();
    qreal ry = point.ry();
    painter->drawRect(rx-length, ry-length, 2*length, 2*length);
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

void getRectBoundValue(const QRectF rect, qreal &minX, qreal &minY, qreal &maxX, qreal &maxY)
{
    QPointF topLeftPoint = rect.topLeft();  // 左上角坐标
    QPointF bottomRightPoint = rect.bottomRight();  // 右下角坐标
    // 获取边界值
    minX = topLeftPoint.rx();
    minY = topLeftPoint.ry();
    maxX = bottomRightPoint.rx();
    maxY = bottomRightPoint.ry();
}

bool boundingRectSeperate(const QRectF rect1, const QRectF rect2)
{
    qreal minX1, minX2, minY1, minY2, maxX1, maxX2, maxY1, maxY2;
    getRectBoundValue(rect1, minX1, minY1, maxX1, maxY1);
    getRectBoundValue(rect2, minX2, minY2, maxX2, maxY2);
    if(minX1 > maxX2
            || maxX1 < minX2
            || minY1 > maxY2
            || maxY1 < minY1){
        return true;
    }
    return false;
}

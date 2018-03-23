#include "painter.h"

Painter::Painter()
{

}

void Painter::drawCrossPoint(QPointF point, int offset=2, crossType type=upright)
{
    qreal x0 = point.rx();
    qreal y0 = point.ry();
    switch (type) {
    case normal:
        this->drawLine(QPointF(x0-offset, y0+offset), QPointF(x0+offset, y0-offset));
        this->drawLine(QPointF(x0+offset, y0+offset), QPointF(x0-offset, y0-offset));
        break;
    case upright:
        this->drawLine(QPointF(x0, y0+offset), QPointF(x0, y0-offset));
        this->drawLine(QPointF(x0+offset, y0), QPointF(x0-offset, y0));
        break;
    }
}

void Painter::drawNodePoint(QPointF point, int radius=2)
{
    this->drawEllipse(point, radius, radius);
}

void Painter::drawRectPoint(QPointF point, int length=2)
{
    qreal x0 = point.rx();
    qreal y0 = point.ry();
    this->drawRect(x0-length, y0-length, 2*length, 2*length);
}

void Painter::drawLineWithArrow(QLineF line, int offset)
{
    QLineF v = line.unitVector();
    v.setLength(offset);
    v.translate(QPointF(line.dx(), line.dy()));

    QLineF n = v.normalVector();
    n.setLength(n.length() * 0.5);
    QLineF n2 = n.normalVector().normalVector();

    QPointF p1 = v.p2();
    QPointF p2 = n.p2();
    QPointF p3 = n2.p2();

    this->drawLine(line);
    QVector<QPointF> vector;
    vector << p1 << p2 << p3 << p1;
    QPolygonF polygon(vector);
    this->drawPolygon(polygon);
}

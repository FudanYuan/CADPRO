#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include "shape.h"
#include <QPointF>

class Ellipse : public Shape, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    Ellipse(QGraphicsItem *parent=0);
};

#endif // ELLIPSE_H

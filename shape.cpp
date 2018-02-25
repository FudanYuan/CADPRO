#include "shape.h"

Shape::Shape() :
    scaleFactor(1),
    moveable(false),
    overFlag(false)
{
}

void Shape::setScaleFactor(qreal scaleFactor)
{
    this->scaleFactor = scaleFactor;
}

void Shape::setMoveable(bool moveable)
{
    this->moveable = moveable;
    emit sceneMoveableChanged(moveable);
}

void Shape::setShapeId(int id)
{
    this->id = id;
}

int Shape::getShapeId()
{
    return this->id;
}

void Shape::setLayer(QString layer)
{
    this->layer = layer;
}

QString Shape::getLayer()
{
    return this->layer;
}

void Shape::setShapeType(Shape::ShapeType shape)
{
    this->shape = shape;
}

Shape::ShapeType Shape::getShapeType()
{
    return this->shape;
}

void Shape::setOverFlag(bool overFlag)
{
    this->overFlag = overFlag;
}

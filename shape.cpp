#include "shape.h"

Shape::Shape() :
    scaleFactor(1),
    moveable(false),
    selectable(false),
    overFlag(false),
    selected(false)
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

void Shape::setPenStyle(PenStyle penStyle)
{
    this->penStyle = penStyle;
}

void Shape::setEntityUnderCursorStyle(PenStyle underCursorStyle)
{
    this->underCursorStyle = underCursorStyle;
}

void Shape::setSelectStyle(PenStyle selectedEntity)
{
    this->selectedEntity = selectedEntity;
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

void Shape::setSelectable(bool selectable)
{
    this->selectable = selectable;
}

bool Shape::getSelectable() const
{
    return this->selectable;
}

void Shape::setOverFlag(bool overFlag)
{
    this->overFlag = overFlag;
}

bool Shape::getOverFlag() const
{
    return this->overFlag;
}

void Shape::setSelected(bool selected)
{
    this->selected = selected;
}

bool Shape::isSelected() const
{
    return this->selected;
}

#include "shape.h"

Shape::Shape() :
    layer(""),
    name(""),
    shape(None),
    id(0),
    number(1),
    scaleFactor(1),
    moveable(false),
    selectable(false),
    overFlag(false),
    selected(false),
    showNode(false),
    editable(false),
    editOverFlag(false)
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

void Shape::setObjectSize(Configure::ObjSize objSize)
{
    this->objSize = objSize;
}

void Shape::setPenStyle(Configure::PenStyle penStyle)
{
    this->penStyle = penStyle;
}

Configure::PenStyle Shape::getPenStyle()
{
    return this->penStyle;
}

void Shape::setEntityUnderCursorStyle(Configure::PenStyle underCursorStyle)
{
    this->underCursorStyle = underCursorStyle;
}

void Shape::setSelectStyle(Configure::PenStyle selectedEntity)
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

void Shape::setName(QString name)
{
    this->name = name;
}

QString Shape::getName()
{
    return this->name;
}

void Shape::setNumber(int num)
{
    this->number = num;
}

int Shape::getNumber()
{
    return this->number;
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

bool Shape::isSelectable() const
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

void Shape::setShowNode(bool showNode)
{
    this->showNode = showNode;
}

bool Shape::isShowNode()
{
    return showNode;
}

void Shape::setEditable(bool editable)
{
    this->editable = editable;
}

bool Shape::isEditable()
{
    return this->editable;
}

void Shape::setEditOverFlag(bool editOverFlag)
{
    this->editOverFlag = editOverFlag;
}

bool Shape::getEditOverFlag() const
{
    return this->editOverFlag;
}

#include "layer.h"

Layer::Layer(QWidget *parent) : QWidget(parent)
{
    scene.setSceneRect(-300000, -300000, 600000, 600000);
}

Layer::~Layer()
{
}

MyGraphicsScene *Layer::getScene()
{
    return &(this->scene);
}

void Layer::setName(QString name)
{
    this->name = name;
}

QString Layer::getName()
{
    return this->name;
}

void Layer::setBackColor(QColor backColor)
{
    this->backColor = backColor;
}

QColor Layer::getBackColor()
{
    return this->backColor;
}

void Layer::setPenColor(QColor penColor)
{
    this->penColor = penColor;
}

QColor Layer::getPenColor()
{
    return this->penColor;
}

void Layer::setPenStyle(Qt::PenStyle penStyle)
{
    this->penColor = penStyle;
}

Qt::PenStyle Layer::getPenStyle()
{
    return this->penStyle;
}

void Layer::setPenWidth(int penWidth)
{
    this->penWidth = penWidth;
}

int Layer::getPenWidth()
{
    return this->penWidth;
}

void Layer::setBrushColor(QColor brushColor)
{
    this->brushColor = brushColor;
}

QColor Layer::getBrushColor()
{
    return this->brushColor;
}

void Layer::setCurShape(MyGraphicsItem::ShapeType curShape)
{
    this->curShape = curShape;
}

MyGraphicsItem::ShapeType Layer::getCurShape()
{
    return this->curShape;
}

void Layer::setModified(bool modified)
{
    this->modified = modified;
}

bool Layer::isModified() const
{
    return this->modified;
}

void Layer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

QColor Layer::transformIntToQColor(const int &intColor)
{
    //将Color 从int 转换成 QColor
    int blue = intColor & 255;
    int green = intColor >> 8 & 255;
    int red = intColor >> 16 & 255;
    return QColor(red, green, blue);
}

void Layer::keyPressEvent(QKeyEvent *event)
{

}

void Layer::mouseMoveEvent(QMouseEvent *event)
{

}

void Layer::mousePressEvent(QMouseEvent *event)
{

}

void Layer::mouseReleaseEvent(QMouseEvent *event)
{

}

void Layer::wheelEvent(QWheelEvent *event)
{

}


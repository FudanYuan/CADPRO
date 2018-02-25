#include "ellipse.h"

Ellipse::Ellipse(QGraphicsItem *parent) :
    QGraphicsEllipseItem(parent)
{
    setShapeType(Shape::Line);
    // 设置图元为可焦点的
    setFlag(QGraphicsItem::ItemIsFocusable);
    // 设置图元为可移动的
    setFlag(QGraphicsItem::ItemIsMovable, false);
    // 设置图元为可接受拖拽事件
    setAcceptDrops(true);
    // 设置图元为可接受hover事件
    setAcceptHoverEvents(true);
}

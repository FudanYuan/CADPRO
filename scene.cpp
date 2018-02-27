#include "scene.h"
#include <QDebug>
#include <QPainter>

Scene::Scene(QObject *parent) :
    QGraphicsScene(parent),
    name(""),
    curShape(Shape::None),
    modified(false),
    drawable(true),
    moveable(false),
    drawing(false),
    penWidth(1),
    scaleFactor(1)
{
    setSceneRect(SHRT_MIN, SHRT_MIN, SHRT_MAX * 2, SHRT_MAX * 2);
}

void Scene::setName(QString name)
{
    this->name = name;
}

QString Scene::getName()
{
    return this->name;
}

void Scene::setCurShape(Shape::ShapeType curShape)
{
    this->curShape = curShape;
}

Shape::ShapeType Scene::getCurShape()
{
    return this->curShape;
}

int Scene::getitemListLength()
{
    return this->itemList.length();
}

void Scene::setModified(bool modified)
{
    this->modified = modified;
}

bool Scene::isModified() const
{
    return this->modified;
}

void Scene::setMoveable(bool moveable)
{
    this->moveable = moveable;
}

bool Scene::isMoveable() const
{
    return this->moveable;
}

void Scene::setDrawable(bool flag)
{
    this->drawable = flag;
}

void Scene::setEntityStyle(EntityStyle eStyle)
{
    this->eStyle = eStyle;
}

EntityStyle Scene::getEntityStyle()
{
    return this->eStyle;
}

void Scene::setAxesGrid(AxesGrid axesGrid)
{
    this->axesGrid = axesGrid;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // 左键可控制 画图、选择、移动、
    if(event->button() == Qt::LeftButton){
        if(moveable){
            qDebug() << "移动...";
            // 移动图元步骤：
            // 1. 选择图元，可多选
            // 2. 移动图元
            for(int i=0; i<itemList.length();i++){
                itemList.at(i)->setMoveable(true);
            }
        } else{
            if(drawable){
                if(drawing) {
                    // 对于需要不止两个条件就可确定的图形，
                    // 还需要继续进行执行drawing函数
                    if(curItem && curItem->updateFlag(event)) {
                        drawing = false;
                        qDebug() << "结束绘图";
                    }
                } else {
                    int id = itemList.length();
                    switch(curShape) {
                    case Shape::None:
                    {
                        curItem = NULL;
                        break;
                    }
                    case Shape::Point:{
                        curItem = NULL;
                        break;
                    }
                    case Shape::Line:
                    {
                        Line *line = new Line;
                        line->setShapeId(id+1);
                        line->setPenStyle(eStyle.perimeterLine);
                        line->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                        line->setSelectStyle(eStyle.selectedEntity);
                        curItem = line;
                        addItem(line);
                        connect(line, &Shape::sceneMoveableChanged, line, &Line::onSceneMoveableChanged);
                        break;
                    }
                    case Shape::MiddleAxis:
                    {
                        Line *line = new Line;
                        line->setShapeId(id+1);
                        line->setPenStyle(eStyle.middleAxis);
                        line->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                        line->setSelectStyle(eStyle.selectedEntity);
                        curItem = line;
                        addItem(line);
                        connect(line, &Shape::sceneMoveableChanged, line, &Line::onSceneMoveableChanged);
                        break;
                    }
                    case Shape::Rectangle:
                    {
                        Rect *rect = new Rect;
                        rect->setShapeId(id+1);
                        rect->setPenStyle(eStyle.cut);
                        rect->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                        rect->setSelectStyle(eStyle.selectedEntity);
                        curItem = rect;
                        addItem(rect);
                        connect(rect, &Shape::sceneMoveableChanged, rect, &Rect::onSceneMoveableChanged);
                        break;
                    }
                    case Shape::Ellipse:
                    {
                        Ellipse *ellipse = new Ellipse;
                        ellipse->setShapeId(id+1);
                        ellipse->setPenStyle(eStyle.cut);
                        ellipse->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                        ellipse->setSelectStyle(eStyle.selectedEntity);
                        curItem = ellipse;
                        addItem(ellipse);
                        connect(ellipse, &Shape::sceneMoveableChanged, ellipse, &Ellipse::onSceneMoveableChanged);
                        break;
                    }
                    case Shape::Circle:
                    {
                        Circle *circle = new Circle;
                        circle->setShapeId(id+1);
                        circle->setPenStyle(eStyle.cut);
                        circle->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                        circle->setSelectStyle(eStyle.selectedEntity);
                        curItem = circle;
                        addItem(circle);
                        connect(circle, &Shape::sceneMoveableChanged, circle, &Circle::onSceneMoveableChanged);
                        break;
                    }
                    case Shape::Arc:
                    {
                        Arc *arc = new Arc;
                        arc->setShapeId(id+1);
                        arc->setPenStyle(eStyle.cut);
                        arc->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                        arc->setSelectStyle(eStyle.selectedEntity);
                        curItem = arc;
                        addItem(arc);
                        connect(arc, &Shape::sceneMoveableChanged, arc, &Arc::onSceneMoveableChanged);
                        break;
                    }
                    case Shape::Arc2:
                    {
                        Arc *arc = new Arc;
                        arc->setShapeId(id+1);
                        arc->setPenStyle(eStyle.cut);
                        arc->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                        arc->setSelectStyle(eStyle.selectedEntity);
                        curItem = arc;
                        addItem(arc);
                        connect(arc, &Shape::sceneMoveableChanged, arc, &Arc::onSceneMoveableChanged);
                        break;
                    }
                    }

                    if(curItem && !drawing && !moveable) {
                        itemList.append(curItem);
                        curItem->startDraw(event);
                        drawing = true;
                        modified = true;
                        emit sceneItemsChanged();
                    }
                }
            }
        }
    }

    // 右键进行 取消及恢复操作：
    // 取消作图、取消选择、取消移动
    // 恢复作图、

    if(event->button() == Qt::RightButton) {
        qDebug() << "点击右键";
        if(drawing){
            drawing = false;
        }
        if(curItem){
            curItem->setOverFlag(true);
        }
        if(moveable){
            moveable = false;
        }

        drawable = !drawable;
        for(int i=0; i<itemList.length();i++){
            itemList.at(i)->setSelectable(!drawable);
            itemList.at(i)->setSelected(false);
            itemList.at(i)->setMoveable(moveable);
        }
    }

    QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(curItem && drawing) {
        curItem->drawing(event);
    }
    QGraphicsScene::mouseMoveEvent(event);
    update();
}

void Scene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsScene::dragEnterEvent(event);
}

void Scene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsScene::dragLeaveEvent(event);
}

void Scene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsScene::dragMoveEvent(event);
}

void Scene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsScene::dropEvent(event);
}

void Scene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->save();
    eStyle.backgroundColor = Qt::white;
    painter->setBrush(eStyle.backgroundColor);
    painter->drawRect(rect);

    if(axesGrid.grid.showGrid){
        // 画网格
        // 获取到当前的线宽，这里的线宽其实还是之前设置的线宽值;
        // 比如我们之前设置线宽为 2 ，这里返回的线宽还是 2 ，但是当前的缩放比例变了；
        // 其实当前的线宽就相当于 penWidth * scaleFactor;
        // 所以如果我们想要让线宽保持不变，那就需要进行转换，即 penWidth = penWidth / scaleFactor;
        // 重新设置画笔线宽;
        QPen pen = QPen();
        pen.setWidthF(0);
        pen.setColor(axesGrid.grid.gridColor);
        painter->setPen(pen);

        const double w = sceneRect().width();
        const double h = sceneRect().height();

        for(int i=0; i<h; i+=axesGrid.grid.yStep / scaleFactor){
            painter->drawLine(QPointF(-w,i),QPointF(w,i));
            painter->drawLine(QPointF(-w,-i),QPointF(w,-i));
        }
        for(int i=0; i<w; i+=axesGrid.grid.xStep / scaleFactor)
        {
            painter->drawLine(QPointF(i,-h),QPointF(i,h));
            painter->drawLine(QPointF(-i,-h),QPointF(-i,h));
        }
    }

    if(axesGrid.axes.showAxes){
        // 画x轴
        QPen pen = QPen();
        pen.setWidthF(0);
        pen.setColor(axesGrid.axes.xAxisColor);
        painter->setPen(pen);

        QLineF lineX(QPointF(0,0),QPointF(10,0));
        painter->drawLine(lineX);
        painter->drawText(10, 0, 20, 20, Qt::AlignLeft | Qt::AlignTop, tr("x"));

        // 画y轴
        pen.setWidthF(0);
        pen.setColor(axesGrid.axes.yAxisColor);
        painter->setPen(pen);

        QLineF lineY(QPointF(0,0),QPointF(0,-10));
        painter->drawLine(lineY);
        painter->drawText(-10, -20, 20, 20, Qt::AlignLeft | Qt::AlignTop, tr("y"));
    }
    painter->restore();
}

void Scene::onViewScaleChanged(qreal scaleFactor)
{
    this->scaleFactor = scaleFactor;
    update();
}

void Scene::onAxesChanged(bool show)
{
    axesGrid.axes.showAxes = show;
    update();
}

void Scene::onGridChanged(bool show)
{
    axesGrid.grid.showGrid = show;
    update();
}

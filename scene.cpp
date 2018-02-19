#include "scene.h"
#include <QDebug>
#include <QPainter>

Scene::Scene(QObject *parent) :
    QGraphicsScene(parent),
    name(""),
    curShape(Shape::None),
    modified(false),
    drawing(false),
    moveable(false),
    startFlag(true),
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

void Scene::setEntityStyle(EntityStyle eStyle)
{
    this->eStyle = eStyle;
}

EntityStyle Scene::getEntityStyle()
{
    return this->eStyle;
}

QColor Scene::transformIntToQColor(const int &intColor)
{
    //将Color 从int 转换成 QColor
    int blue = intColor & 255;
    int green = intColor >> 8 & 255;
    int red = intColor >> 16 & 255;
    return QColor(red, green, blue);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
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
            if(startFlag){
                if(drawing) {
                    drawing = false;
                    qDebug() << "结束绘图";
                } else {
                    startFlag = true;
                    int id = itemList.length() + 1;
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
                        eStyle.perimeterLine.width /= this->scaleFactor;
                        line->setStyle(eStyle);
                        line->setShapeId(id);
                        curItem = line;
                        addItem(line);

                        connect(line, &Shape::sceneMoveableChanged, line, &Line::onSceneMoveableChanged);
                        connect(this, &Scene::sceneScaleChanged, line, &Line::onSceneScaleChanged);
                        break;
                    }
                    case Shape::Rectangle:
                    {
                        Rect *rect = new Rect;
                        rect->setShapeId(id);
                        eStyle.perimeterLine.width /= this->scaleFactor;
                        rect->setStyle(eStyle);
                        curItem = rect;
                        addItem(rect);

                        connect(rect, &Shape::sceneMoveableChanged, rect, &Rect::onSceneMoveableChanged);
                        connect(this, &Scene::sceneScaleChanged, rect, &Rect::onSceneScaleChanged);
                        break;
                    }
                    }

                    if(curItem && !drawing && !moveable) {
                        itemList.append(curItem);
                        curItem->startDraw(event);
                        drawing = true;
                        modified = true;
                    }

                    emit sceneItemsChanged();
                }
            }
        }
    }

    if(event->button() == Qt::RightButton) {
        startFlag = false;
        for(int i=0; i<itemList.length();i++){
            itemList.at(i)->setOverFlag(true);
        }
    }

    QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "Scene::mouseReleaseEvent";
    QGraphicsScene::mouseReleaseEvent(event);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "Scene::mouseMoveEvent";
    if(curItem && drawing) {
        curItem->drawing(event);
    }
    QGraphicsScene::mouseMoveEvent(event);
    update();
}

void Scene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Scene::dragEnterEvent";
    QGraphicsScene::dragEnterEvent(event);
}

void Scene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Scene::dragLeaveEvent";
    QGraphicsScene::dragLeaveEvent(event);
}

void Scene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Scene::dragMoveEvent";
    QGraphicsScene::dragMoveEvent(event);
}

void Scene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "Scene::dropEvent";
    QGraphicsScene::dropEvent(event);
}

void Scene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->save();
    painter->setBrush(Qt::white);
    painter->drawRect(rect);

    // 获取到当前的线宽，这里的线宽其实还是之前设置的线宽值;
    // 比如我们之前设置线宽为 2 ，这里返回的线宽还是 2 ，但是当前的缩放比例变了；
    // 其实当前的线宽就相当于 penWidth * scaleFactor;
    // 所以如果我们想要让线宽保持不变，那就需要进行转换，即 penWidth = penWidth / scaleFactor;
    // 重新设置画笔线宽;
    QPen pen = QPen();
    pen.setWidthF(penWidth / scaleFactor);
    pen.setColor(Qt::gray);
    painter->setPen(pen);

    const double w = sceneRect().width();
    const double h = sceneRect().height();

    // 画网格
    for(int i=0; i<h; i+=100 / scaleFactor){
        painter->drawLine(QPointF(-w,i),QPointF(w,i));
        painter->drawLine(QPointF(-w,-i),QPointF(w,-i));
    }
    for(int i=0; i<w; i+=100 / scaleFactor)
    {
        painter->drawLine(QPointF(i,-h),QPointF(i,h));
        painter->drawLine(QPointF(-i,-h),QPointF(-i,h));
    }

    // 画x轴
    pen.setWidthF(penWidth / scaleFactor);
    pen.setColor(Qt::red);
    painter->setPen(pen);

    QLineF lineX(QPointF(0,0),QPointF(10,0));
    painter->drawLine(lineX);
    painter->drawText(10, 0, 20, 20, Qt::AlignLeft | Qt::AlignTop, tr("x"));

    // 画y轴
    pen.setWidthF(penWidth / scaleFactor);
    pen.setColor(Qt::blue);
    painter->setPen(pen);

    QLineF lineY(QPointF(0,0),QPointF(0,-10));
    painter->drawLine(lineY);
    painter->drawText(-10, -20, 20, 20, Qt::AlignLeft | Qt::AlignTop, tr("y"));

    painter->restore();
}

void Scene::onViewScaleChanged(qreal scaleFactor)
{
    qDebug() << "scene on view scale changed: " << scaleFactor;
    qDebug() << "to do : 重画drawBackground";
    this->scaleFactor = scaleFactor;
    update();
    emit sceneScaleChanged(scaleFactor);
}

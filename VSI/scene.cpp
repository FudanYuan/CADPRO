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
    polygonType = 1;
    polygonLineNum = 4;
    polygonRadius = 100;
    polygonAlpha = 0;
    trapeziumH = 100;
    trapeziumToplength = 100;
    trapeziumAlpha1 = 60;
    trapeziumAlpha2 = 60;
    eyeletDialog = new Eyelet;
    textdialog = new Text;
    recttram = new Rect;
    polygontram = new Polygon;
}

Scene::~Scene()
{
#ifdef DEBUG
    qDebug() << "scene has been deleted!";
#endif
    itemList.clear();
    curItem = NULL;
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
    this->setDrawable(true);
    this->setMoveable(false);
    this->curShape = curShape;
}

Shape::ShapeType Scene::getCurShape()
{
    return this->curShape;
}

QList<Shape *> Scene::getItemList() const
{
    return this->itemList;
}

int Scene::getItemListLength() const
{
    return this->itemList.length();
}

QList<Point *> Scene::getPointList() const
{
    return this->pointList;
}

int Scene::getPointListLength() const
{
    return this->pointList.length();
}

QList<Line *> Scene::getLineList() const
{
    return this->lineList;
}

int Scene::getLineListLength() const
{
    return this->lineList.length();
}

QList<Polyline *> Scene::getPolylineList() const
{
    return this->polylineList;
}

int Scene::getPolylineListLength() const
{
    return this->polylineList.length();
}

QList<Ellipse *> Scene::getEllipseList() const
{
    return this->ellipseList;
}

int Scene::getEllipseListLength() const
{
    return this->ellipseList.length();
}

QList<Circle *> Scene::getCircleList() const
{
    return this->circleList;
}

int Scene::getCircleListLength() const
{
    return this->circleList.length();
}

QList<Arc *> Scene::getArcList() const
{
    return this->arcList;
}

int Scene::getArcListLength() const
{
    return this->arcList.length();
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

void Scene::setEntityStyle(Configure::EntityStyle eStyle)
{
    this->eStyle = eStyle;
}

Configure::EntityStyle Scene::getEntityStyle()
{
    return this->eStyle;
}

void Scene::setAxesGrid(Configure::AxesGrid axesGrid)
{
    this->axesGrid = axesGrid;
}

void Scene::addCustomPointItem(Point *point)
{
    if(itemList.contains(point)){
        return;
    }
    point->setShapeId(getItemListLength()+1);

    qDebug() << "shapeType: " << point->getShapeType();
    Configure::PenStyle pen(eStyle.referPoint.color, Qt::SolidLine, 1);
    point->setPenStyle(pen);
    point->setOffset(eStyle.referPoint.sizeInPix);
    point->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
    point->setSelectStyle(eStyle.selectedEntity);

    itemList.append(point);
    pointList.append(point);
    addItem(point);
    connect(point, &Shape::sceneMoveableChanged, point, &Point::onSceneMoveableChanged);
    connect(point, &Point::select, this, &Scene::onPointSelected);
}

void Scene::addCustomLineItem(Line *line)
{
    if(itemList.contains(line)){
        return;
    }
    line->setShapeId(getItemListLength()+1);
    switch (line->getShapeType()) {
    case Shape::Line:
        line->setPenStyle(eStyle.perimeterLine);
        line->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
        line->setSelectStyle(eStyle.selectedEntity);
        break;
    case Shape::MiddleAxis:
    case Shape::Direction:
        line->setPenStyle(eStyle.middleAxis);
        line->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
        line->setSelectStyle(eStyle.selectedEntity);
        break;
    default:
        break;
    }
    itemList.append(line);
    lineList.append(line);
    addItem(line);
    connect(line, &Shape::sceneMoveableChanged, line, &Line::onSceneMoveableChanged);
    connect(line, &Line::select, this, &Scene::onLineSelected);
}

void Scene::addCustomPolylineItem(Polyline *polyline)
{
    if(itemList.contains(polyline)){
        return;
    }
    polyline->setShapeId(getItemListLength()+1);

    polyline->setPenStyle(eStyle.perimeterLine);
    polyline->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
    polyline->setSelectStyle(eStyle.selectedEntity);

    itemList.append(polyline);
    polylineList.append(polyline);
    addItem(polyline);
    connect(polyline, &Shape::sceneMoveableChanged, polyline, &Polyline::onSceneMoveableChanged);
    connect(polyline, &Polyline::select, this, &Scene::onPolylineSelected);
}

void Scene::addCustomRectItem(Rect *rect)
{
    if(itemList.contains(rect)){
        return;
    }
    rect->setShapeId(getItemListLength()+1);

    rect->setPenStyle(eStyle.cut);
    rect->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
    rect->setSelectStyle(eStyle.selectedEntity);

    itemList.append(rect);
    addItem(rect);
    connect(rect, &Shape::sceneMoveableChanged, rect, &Rect::onSceneMoveableChanged);
}

void Scene::addCustomEllipseItem(Ellipse *ellipse)
{
    if(itemList.contains(ellipse)){
        return;
    }
    ellipse->setShapeId(getItemListLength()+1);

    ellipse->setPenStyle(eStyle.cut);
    ellipse->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
    ellipse->setSelectStyle(eStyle.selectedEntity);

    itemList.append(ellipse);
    ellipseList.append(ellipse);
    addItem(ellipse);
    connect(ellipse, &Shape::sceneMoveableChanged, ellipse, &Ellipse::onSceneMoveableChanged);
}

void Scene::addCustomCircleItem(Circle *circle)
{
    if(itemList.contains(circle)){
        return;
    }
    circle->setShapeId(getItemListLength()+1);

    switch (circle->getShapeType()) {
    case Shape::Circle:
        circle->setPenStyle(eStyle.cut);
        circle->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
        circle->setSelectStyle(eStyle.selectedEntity);
        break;
    case Shape::Hole:
        circle->setFilled(true);
        circle->setPenStyle(eStyle.cut);
        circle->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
        circle->setSelectStyle(eStyle.selectedEntity);
        break;
    default:
        break;
    }

    itemList.append(circle);
    circleList.append(circle);
    addItem(circle);
    connect(circle, &Shape::sceneMoveableChanged, circle, &Circle::onSceneMoveableChanged);
    connect(circle, &Circle::select, this, &Scene::onCircleSelected);
}

void Scene::addCustomArcItem(Arc *arc)
{
    if(itemList.contains(arc)){
        return;
    }
    arc->setShapeId(getItemListLength()+1);

    switch (arc->getShapeType()) {
    case Shape::Arc:
        arc->setPenStyle(eStyle.cut);
        arc->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
        arc->setSelectStyle(eStyle.selectedEntity);
        break;
    case Shape::Arc2:
        arc->setType(Arc::updated);
        arc->setPenStyle(eStyle.cut);
        arc->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
        arc->setSelectStyle(eStyle.selectedEntity);
        break;
    default:
        break;
    }

    itemList.append(arc);
    arcList.append(arc);
    addItem(arc);
    connect(arc, &Shape::sceneMoveableChanged, arc, &Arc::onSceneMoveableChanged);
    connect(arc, &Arc::select, this, &Scene::onArcSelected);
}

void Scene::addCustomItem()
{

}

void Scene::clearCustomItem(){
    itemList.clear();
    pointList.clear();
    lineList.clear();
    polylineList.clear();
    ellipseList.clear();
    circleList.clear();
    clear();
}

int Scene::getpolygonType() const
{
    return polygonType;
}

void Scene::setpolygonType(int value)
{
    polygonType = value;
}

int Scene::getpolygonLineNum() const
{
    return polygonLineNum;
}

void Scene::setpolygonLineNum(int value)
{
    polygonLineNum = value;
}

double Scene::getpolygonRadius() const
{
    return polygonRadius;
}

void Scene::setpolygonRadius(double value)
{
    polygonRadius = value;
}

qreal Scene::getpolygonAlpha() const
{
    return polygonAlpha;
}

void Scene::setpolygonAlpha(const qreal &value)
{
    polygonAlpha = value;
}

double Scene::gettrapeziumH() const
{
    return trapeziumH;
}

void Scene::settrapeziumH(double value)
{
    trapeziumH = value;
}

double Scene::gettrapeziumToplength() const
{
    return trapeziumToplength;
}

void Scene::settrapeziumToplength(double value)
{
    trapeziumToplength = value;
}

double Scene::gettrapeziumAlpha1() const
{
    return trapeziumAlpha1;
}

void Scene::settrapeziumAlpha1(double value)
{
    trapeziumAlpha1 = value;
}

double Scene::gettrapeziumAlpha2() const
{
    return trapeziumAlpha2;
}

void Scene::settrapeziumAlpha2(double value)
{
    trapeziumAlpha2 = value;
}

int Scene::gettrapeziumType() const
{
    return trapeziumType;
}

void Scene::settrapeziumType(int value)
{
    trapeziumType = value;
}


void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // 左键可控制 画图、选择、移动、
    if(event->button() == Qt::LeftButton){
        // 可画标识为真，表示目前为可画，而且是新添图形
        if(drawable){
            if(drawing) {
                // 对于需要不止两个条件就可确定的图形，
                // 还需要继续进行执行drawing函数
                if(curItem && curItem->updateFlag(event)) {
                    drawing = false;
                    qDebug() << "结束绘图";
                    //变化成polyline
                    switch(curShape)
                    {
                    case Shape::Rectangle:
                    {
                        //转换类型
                        Polyline *polyline = new Polyline;
                        polyline->setPoints(recttram->toPolyline());
                        polylineList.append(polyline);
                    }
                    case Shape::Polygon:
                    {
                        //转换类型
                        Polyline *polyline = new Polyline;
                        polygontram->toPolyline();
                        polygontram->setPoints(polygontram->getPoints());
                        polylineList.append(polyline);

                        polyline->setPenStyle(recttram->getPenStyle());
                        polyline->setLayer(recttram->getLayer());
//                        if(polyline->getPoints().isEmpty())
//                            qDebug()<<"没有点集";
//                        else
//                        {
//                            QPointF p;
//                            p=polyline->getPoints().at(0);
//                            qDebug()<<"点：x:"<<p.rx()<<"y:"<<p.ry();
//                            p=polyline->getPoints().at(1);
//                            qDebug()<<"点：x:"<<p.rx()<<"y:"<<p.ry();
//                            p=polyline->getPoints().at(2);
//                            qDebug()<<"点：x:"<<p.rx()<<"y:"<<p.ry();
//                            p=polyline->getPoints().at(3);
//                            qDebug()<<"点：x:"<<p.rx()<<"y:"<<p.ry();
//                            qDebug()<<"点集长度："<<polyline->getPoints().length();
//                            qDebug()<<"线类型："<<polyline->getType();
//                            qDebug()<<"不知道是啥："<<polyline->getLayer();
//                            qDebug()<<"颜色"<<polyline->getPenStyle().color;
//                            qDebug()<<"线宽"<<polyline->getPenStyle().width;
//                        }
                    }
                    default:
                        break;
                    }
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
                    Point *point = new Point;
                    point->setShapeId(id+1);
                    Configure::PenStyle pen(eStyle.referPoint.color, Qt::SolidLine, 1);
                    point->setPenStyle(pen);
                    point->setOffset(eStyle.referPoint.sizeInPix);
                    point->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                    point->setSelectStyle(eStyle.selectedEntity);
                    curItem = point;
                    addItem(point);
                    pointList.append(point);
                    connect(point, &Shape::sceneMoveableChanged, point, &Point::onSceneMoveableChanged);
                    connect(point, &Point::select, this, &Scene::onPointSelected);
                    break;
                }
                case Shape::Line:
                {
                    Line *line = new Line;
                    line->setShapeId(id+1);
                    line->setShapeType(Shape::Line);
                    line->setPenStyle(eStyle.perimeterLine);
                    line->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                    line->setSelectStyle(eStyle.selectedEntity);
                    curItem = line;
                    addItem(line);
                    lineList.append(line);
                    connect(line, &Shape::sceneMoveableChanged, line, &Line::onSceneMoveableChanged);
                    connect(line, &Line::select, this, &Scene::onLineSelected);
                    break;
                }
                case Shape::MiddleAxis:
                {
                    Line *line = new Line;
                    line->setShapeId(id+1);
                    line->setShapeType(Shape::MiddleAxis);
                    line->setPenStyle(eStyle.middleAxis);
                    line->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                    line->setSelectStyle(eStyle.selectedEntity);
                    curItem = line;
                    addItem(line);
                    lineList.append(line);
                    connect(line, &Shape::sceneMoveableChanged, line, &Line::onSceneMoveableChanged);
                    connect(line, &Line::select, this, &Scene::onLineSelected);
                    break;
                }
                case Shape::Direction:
                {
                    Line *line = new Line;
                    line->setShapeId(id+1);
                    line->setShapeType(Shape::Direction);
                    line->setPenStyle(eStyle.middleAxis);
                    line->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                    line->setSelectStyle(eStyle.selectedEntity);
                    curItem = line;
                    addItem(line);
                    lineList.append(line);
                    connect(line, &Shape::sceneMoveableChanged, line, &Line::onSceneMoveableChanged);
                    connect(line, &Line::select, this, &Scene::onLineSelected);
                    break;
                }
                case Shape::Polyline:
                {
                    Polyline *polyline = new Polyline;
//                    polyline->setType(Polyline::cubic);
                    polyline->setShapeId(id+1);
                    polyline->setPenStyle(eStyle.perimeterLine);
                    polyline->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                    polyline->setSelectStyle(eStyle.selectedEntity);
                    curItem = polyline;
                    addItem(polyline);
                    polylineList.append(polyline);
                    connect(polyline, &Shape::sceneMoveableChanged, polyline, &Polyline::onSceneMoveableChanged);
                    connect(polyline, &Polyline::select, this, &Scene::onPolylineSelected);
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
                    recttram =rect;//传递
                    addItem(rect);
                    connect(rect, &Shape::sceneMoveableChanged, rect, &Rect::onSceneMoveableChanged);
                    connect(rect, &Rect::select, this, &Scene::onRectSelected);
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
                    ellipseList.append(ellipse);
                    connect(ellipse, &Shape::sceneMoveableChanged, ellipse, &Ellipse::onSceneMoveableChanged);
                    connect(ellipse, &Ellipse::select, this, &Scene::onEllipseSelected);
                    break;
                }
                case Shape::Circle:
                {
                    Circle *circle = new Circle;
                    circle->setShapeId(id+1);
                    circle->setFilled(false);
                    circle->setPenStyle(eStyle.cut);
                    circle->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                    circle->setSelectStyle(eStyle.selectedEntity);
                    curItem = circle;
                    addItem(circle);
                    circleList.append(circle);
                    connect(circle, &Shape::sceneMoveableChanged, circle, &Circle::onSceneMoveableChanged);
                    connect(circle, &Circle::select, this, &Scene::onCircleSelected);
                    break;
                }
                case Shape::Hole:
                {
                    Circle *circle = new Circle;
                    circle->setShapeId(id+1);
                    circle->setShapeType(Shape::Hole);
                    circle->setFilled(true);
                    circle->setPenStyle(eStyle.cut);
                    circle->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                    circle->setSelectStyle(eStyle.selectedEntity);
                    curItem = circle;
                    addItem(circle);
                    circleList.append(circle);
                    connect(circle, &Shape::sceneMoveableChanged, circle, &Circle::onSceneMoveableChanged);
                    connect(circle, &Circle::select, this, &Scene::onCircleSelected);
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
                    arcList.append(arc);
                    connect(arc, &Shape::sceneMoveableChanged, arc, &Arc::onSceneMoveableChanged);
                    connect(arc, &Arc::select, this, &Scene::onArcSelected);
                    break;
                }
                case Shape::Arc2:
                {
                    Arc *arc = new Arc;
                    arc->setType(Arc::updated);
                    arc->setShapeId(id+1);
                    arc->setPenStyle(eStyle.cut);
                    arc->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                    arc->setSelectStyle(eStyle.selectedEntity);
                    curItem = arc;
                    addItem(arc);
                    arcList.append(arc);
                    connect(arc, &Shape::sceneMoveableChanged, arc, &Arc::onSceneMoveableChanged);
                    connect(arc, &Arc::select, this, &Scene::onArcSelected);
                    break;
                }
                case Shape::Polygon:
                {
                    Polygon *polygon = new Polygon;

                    polygon->setLineNum(this->getpolygonLineNum());
                    polygon->setRadius(this->getpolygonRadius());
                    polygon->setAlpha(this->getpolygonAlpha());
                    polygon->setType(this->getpolygonType());

                   /* Polygon_dialog *polygon_dialog = new Polygon_dialog;//正多边形绘制的对话框
                    polygon_dialog->exec();
                    //获得对话框中传递的数据
                    polygon->setLine_num(polygon_dialog->getLen_num());
                    polygon->setRadius(polygon_dialog->getRaduii());
                    polygon->setAlpha(polygon_dialog->getAngle());
                    */
                    polygon->setShapeId(id+1);
                    switch(this->getpolygonType()){
                        case 0:polygon->setPenStyle(eStyle.generic);break;
                        case 1:polygon->setPenStyle(eStyle.mark);break;
                        case 2:polygon->setPenStyle(eStyle.perimeterLine);break;
                        case 3:polygon->setPenStyle(eStyle.cut);break;
                        case 4:polygon->setPenStyle(eStyle.stitch);break;
                    }
                    polygon->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                    polygon->setSelectStyle(eStyle.selectedEntity);
                    curItem = polygon;
                    polygontram = polygon;
                    addItem(polygon);
                    connect(polygon, &Shape::sceneMoveableChanged, polygon, &Polygon::onSceneMoveableChanged);
                    break;
                }
                case Shape::Trapezium:
                {
                    Trapezium *trapezium = new Trapezium;

                    trapezium->setTrapeziumAlpha1(this->gettrapeziumAlpha1());
                    trapezium->setTrapeziumAlpha2(this->gettrapeziumAlpha2());
                    trapezium->setTrapeziumH(this->gettrapeziumH());
                    trapezium->setTrapeziumToplength(this->gettrapeziumToplength());
                    trapezium->setTrapeziumType(this->gettrapeziumType());

                    trapezium->setShapeId(id+1);
                    switch(this->getpolygonType()){
                        case 0:trapezium->setPenStyle(eStyle.generic);break;
                        case 1:trapezium->setPenStyle(eStyle.mark);break;
                        case 2:trapezium->setPenStyle(eStyle.perimeterLine);break;
                        case 3:trapezium->setPenStyle(eStyle.cut);break;
                        case 4:trapezium->setPenStyle(eStyle.stitch);break;
                    }
                    trapezium->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                    trapezium->setSelectStyle(eStyle.selectedEntity);
                    curItem = trapezium;
                    addItem(trapezium);
                    connect(trapezium, &Shape::sceneMoveableChanged, trapezium, &Trapezium::onSceneMoveableChanged);
                    break;
                }
                case Shape::Eyelet:
                {
                    Eyelet *eyelet = new Eyelet;

                    eyelet->setEyeletH(this->eyeletDialog->eyeletdialog->getEyeletHeight());
                    eyelet->setEyeletW(this->eyeletDialog->eyeletdialog->getEyeletWidth());

                    eyelet->setShapeId(id+1);
                    switch(this->eyeletDialog->eyeletdialog->getPenStyle()){
                        case 0:eyelet->setPenStyle(eStyle.generic);break;
                        case 1:eyelet->setPenStyle(eStyle.mark);break;
                        case 2:eyelet->setPenStyle(eStyle.perimeterLine);break;
                        case 3:eyelet->setPenStyle(eStyle.cut);break;
                        case 4:eyelet->setPenStyle(eStyle.stitch);break;
                    }
                    eyelet->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                    eyelet->setSelectStyle(eStyle.selectedEntity);
                    curItem = eyelet;
                    addItem(eyelet);
                    connect(eyelet, &Shape::sceneMoveableChanged, eyelet, &Eyelet::onSceneMoveableChanged);
                    break;
                }
                case Shape::Text:
                {
                    Text *text = new Text;

                    text->setTextcontent(this->textdialog->textdialog->getText());
                    text->setTextPixelSize(this->textdialog->textdialog->getTextsize());
                    qDebug()<<"文本内容"<<text->getTextcontent();
                    qDebug()<<"文本大小"<<text->getTextPixelSize();

                    text->setShapeId(id+1);
                    text->setEntityUnderCursorStyle(eStyle.entityUnderCursor);
                    text->setSelectStyle(eStyle.selectedEntity);
                    curItem = text;
                    addItem(text);
                    connect(text, &Shape::sceneMoveableChanged, text, &Text::onSceneMoveableChanged);
                    break;
                }
                default:
                    break;
                }

                if(curItem && !drawing && !moveable) {
                    itemList.append(curItem);
                    curItem->startDraw(event);
                    curItem->setObjectSize(eStyle.objSize);
                    if(getCurShape() == Shape::Point){
                        drawing = false;
                        update();
                    } else {
                        drawing = true;
                    }
                    modified = true;
                    emit sceneItemsChanged();
                }
            }
        }

        // 移动标识为真，表示目前可移动图元
        if(moveable){
            qDebug() << "移动...";
            // 移动图元步骤：
            // 1. 选择图元，可多选
            // 2. 移动图元
            for(int i=0; i<itemList.length();i++){
                itemList.at(i)->setMoveable(true);
            }
        }

        // 编辑标识为真，表示目前可编辑图元
        if(editable){
            if(curItem && curItem->updateFlag(event)) {
                drawing = false;
                curItem->setEditable(false);
                curItem->setEditOverFlag(true);
            }
            editable = false;
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
            if(getCurShape() == Shape::Polyline && !curItem->getOverFlag()){
                curItem->setOverFlag(true);
                return;
            } else{
                curItem->setOverFlag(true);
            }
            drawable = !drawable;
        }

        if(moveable){
            moveable = false;
        }

        for(int i=0; i<itemList.length();i++){
            itemList.at(i)->setSelectable(!drawable);
            itemList.at(i)->setSelected(false);
            itemList.at(i)->setMoveable(moveable);
        }
        clearSelection();
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
    update();
    QGraphicsScene::mouseMoveEvent(event);
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
        pen.setJoinStyle(Qt::MiterJoin);
        painter->setPen(pen);

        QBrush brush = QBrush();
        brush.setColor(axesGrid.axes.xAxisColor);
        brush.setStyle(Qt::SolidPattern);
        painter->setBrush(brush);

        QPointF xPos = QPointF(axesGrid.axes.axisSizeInPix / scaleFactor,0);
        QLineF lineX(QPointF(0,0), xPos);
        drawLineWithArrow(painter, lineX, axesGrid.axes.arrowSizeInPix / scaleFactor);
        painter->drawText(10, 0, 20, 20, Qt::AlignLeft | Qt::AlignTop, tr("x"));


        // 画y轴
        pen.setWidthF(0);
        pen.setColor(axesGrid.axes.yAxisColor);
        pen.setJoinStyle(Qt::MiterJoin);
        painter->setPen(pen);

        brush.setColor(axesGrid.axes.yAxisColor);
        brush.setStyle(Qt::SolidPattern);
        painter->setBrush(brush);

        QPointF yPos = QPointF(0,-axesGrid.axes.axisSizeInPix / scaleFactor);
        QLineF lineY(QPointF(0,0), yPos);
        drawLineWithArrow(painter, lineY, axesGrid.axes.arrowSizeInPix / scaleFactor);
        painter->drawText(-10, -20, 20, 20, Qt::AlignLeft | Qt::AlignTop, tr("y"));
    }
    painter->restore();
}

Text *Scene::getTextdialog() const
{
    return textdialog;
}

void Scene::setTextdialog(Text *value)
{
    textdialog = value;
}

Eyelet *Scene::getEyeletDialog() const
{
    return eyeletDialog;
}

void Scene::setEyeletDialog(Eyelet *value)
{
    eyeletDialog = value;
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

void Scene::onNewItem()
{
    qDebug() << "有新实体";
    emit sceneItemsChanged();
}

void Scene::onPointSelected(Point *point)
{
    emit pointSelected(point);
}

void Scene::onLineSelected(Line *line)
{
    emit lineSelected(line);
    if(line->isEditable()){
        curItem = line;
        drawing = true;
        editable = true;
    }
}

void Scene::onArcSelected(Arc *arc)
{
    emit arcSelected(arc);
}

void Scene::onEllipseSelected(Ellipse *ellipse)
{
    emit ellipseSelected(ellipse);
}

void Scene::onCircleSelected(Circle *circle)
{
    emit circleSelected(circle);
}

void Scene::onRectSelected(Rect *rect)
{
    emit rectSelected(rect);
}

void Scene::onPolylineSelected(Polyline *polyline)
{
    emit polylineSelected(polyline);
}

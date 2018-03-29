#include "project.h"
#include <QMessageBox>
#include <QDebug>

Project::Project(QObject *parent) :
QObject(parent),
type(Sketch),
name(""),
saved(false),
modified(false)
{
    
}

Project::~Project()
{
    qDebug() << "project has been deleted!";
    sceneList.clear();
    if(sceneActive){
        //free(sceneActive);
    }
}

void Project::setType(Project::Type type)
{
    this->type = type;
}

Project::Type Project::getType()
{
    return this->type;
}

QString Project::getName()
{
    return this->name;
}

void Project::setName(const QString name)
{
#ifdef DEBUG
    qDebug() << name;
#endif
    this->name = name;
}

void Project::changeName(const QString name)
{
    emit projectNameChanged(this->name, name);
    modified = true;
    setName(name);
}

void Project::addScene(Scene *scene)
{
    if(scene->getName() == ""){
        QString name = getNewSceneName();
        scene->setName(name);
    }
    this->sceneList.append(scene);
}

Scene *Project::addScene()
{
    sceneActive = new Scene(this);
    QString name = getNewSceneName();
    sceneActive->setName(name);
    this->addScene(sceneActive);
    return sceneActive;
}

void Project::setActiveScene(Scene *scene)
{
    this->sceneActive = scene;
}

QList<Scene *> Project::getSceneList()
{
    return this->sceneList;
}

Scene *Project::getScene(const int index)
{
    int length = this->sceneList.length();
    if(index >= length){
        return NULL;
    }
    return this->sceneList.at(index);
}

Scene *Project::getSceneByName(const QString name)
{
    for(int i=0;i<sceneList.length();i++){
        if(sceneList.at(i)->getName() == name){
            return sceneList.at(i);
        }
    }
    return NULL;
}

bool Project::removeScene(const int index)
{
    int length = this->sceneList.length();
    if(index > length){
        return false;
    }
    sceneList.removeAt(index);
    return true;
}

bool Project::removeSceneByName(const QString name)
{
    for(int i=0;i<sceneList.length();i++){
        if(sceneList.at(i)->getName() == name){
            sceneList.removeAt(i);
            return true;
        }
    }
    return false;
}

Scene *Project::getActiveScene()
{
    return this->sceneActive;
}

void Project::setSaved(const bool saved)
{
    this->saved = saved;
}

bool Project::isSaved()
{
    return this->saved;
}

bool Project::isModified()
{
    for(int i=0; i<this->sceneList.length();i++){
        this->modified |= this->sceneList.at(i)->isModified();
    }
    return modified;
}

bool Project::saveProject()
{
    for(int i=0; i<this->sceneList.length();i++){
        this->sceneList.at(i)->setModified(false);
    }
    qDebug() << this->name << "已保存" <<endl;
    return true;
}

QString Project::getNewSceneName()
{
    // 获取图层数目
    int len_scene = sceneList.length() + 1;
    QString str = QString::number(len_scene, 10);
    char *ch;
    QByteArray ba = str.toLatin1();
    ch = ba.data();
    return tr(ch);
}

QString Project::getSceneName(Scene *scene)
{
    return scene->getName();
}

QString Project::getSceneName(const int i)
{
    return sceneList.at(i)->getName();
}

void Project::dxfFileReader(const QString fileName)
{
    //初始化dxf文件
    if (!dxf.in(fileName.toStdString(), &dxfFilter)) {
#ifdef DXFDEBUG
        qDebug() << "无法读取DXF文件";
#endif
        throw(tr("无法读取DXF文件"));
    }else {
#ifdef DXFDEBUG
        qDebug() << "success";
#endif
        // 读取图层
        dxfLayerReader(dxfFilter);
        
        if(type == Sketch){
            // 读取point实体
            dxfPointReader(dxfFilter);
        }
        
        // 读取line实体
        dxfLineReader(dxfFilter);
        
        //  读取Arc实体
        dxfArcReader(dxfFilter);
        
        //  读取Circle实体
        dxfCircleReader(dxfFilter);
        
        //  读取Ellipse实体
        dxfEllipseReader(dxfFilter);
        
        //  读取Polyline实体
        dxfPolylineReader(dxfFilter);
        
        //  读取Text实体
        dxfTextReader(dxfFilter);
        
        // 插入新图元
        sceneActive->onNewItem();
        
        // 更新图层修改标识
        sceneActive->setModified(true);
    }
}

void Project::dxfLayerReader(const DxfFilter dxfFilter)
{
    sceneList.clear();  // 清空图层列表
    for(int i=0; i<dxfFilter.layers.length();i++){
        QString layer = dxfFilter.transformText(dxfFilter.layers.at(i).layer.name);
        bool off = dxfFilter.layers.at(i).layer.off;
        if(!off && type == Sketch){
            QString name = layer;
            Scene *scene = new Scene();
            scene->setName(name);
            sceneList.append(scene);
#ifdef DXFDEBUG
            qDebug() << "第" << i << "个图层名称：" << name;
            qDebug() << "第" << i << "个图层颜色：" << dxfFilter.layers.at(i).attribute.getColor();
#endif
        } else{
            offLayers.append(layer);
        }
    }
    if(sceneList.length()==0 && type == Sketch){
        sceneActive = new Scene;
        sceneActive->setName(getNewSceneName());
        sceneList.append(sceneActive);
    }
#ifdef DXFDEBUG
    qDebug() << "图层列表长度为：" << sceneList.length();
#endif
    
}

void Project::dxfPointReader(const DxfFilter dxfFilter)
{
    for(int i=0; i<dxfFilter.points.length();i++){
        // 判断该元素是否在不可用图层上
        DL_Attributes attr = dxfFilter.points.at(i).attribute;
        QString layer = dxfFilter.transformText(attr.getLayer());
        if(offLayers.contains(layer)){
            continue;
        }
        
        // 获取point坐标
        qreal x = dxfFilter.points.at(i).point.x;
        qreal y = dxfFilter.points.at(i).point.y;
        
        // 获取point属性
        QColor color = dxfFilter.transformColor(attr.getColor());
        Qt::PenStyle style = dxfFilter.transformStyle(attr.getLinetype());
        int width = dxfFilter.transformWidth(attr.getWidth());
        
        if(type == Sketch){
            // 根据图层名获取图层
            sceneActive = getSceneByName(layer);
            if(!sceneActive){
                sceneActive = new Scene(this);
                QString name = layer;
                sceneActive->setName(name);
                sceneList.append(sceneActive);
            }
        } else{
            // 当类型为排版时，一个图形就是一个图层
            sceneActive = new Scene(this);
            QString name = getNewSceneName();
            sceneActive->setName(name);
            sceneList.append(sceneActive);
        }
        
        // 添加点元素
        Point *point = new Point;
        Configure::PenStyle pen;
        pen.setPenStyle(color, style, width);
        point->setPenStyle(pen);
        point->setPoint(QPointF(x,y));
        
        sceneActive->addCustomPointItem(point);
        sceneList[0]->addCustomPointItem(point);
        
#ifdef DXFDEBUG
        qDebug() << "layer：" << layer;
        qDebug() << "color：" << color;
        qDebug() << "style：" << style;
        qDebug() << "width：" << width;
        qDebug() << "point：" << point->point();
#endif
    }
}

void Project::dxfLineReader(const DxfFilter dxfFilter)
{
    for(int i=0; i<dxfFilter.lines.length();i++){
        // 判断该元素是否在不可用图层上
        DL_Attributes attr = dxfFilter.lines.at(i).attribute;
        QString layer = dxfFilter.transformText(attr.getLayer());
        if(offLayers.contains(layer)){
            continue;
        }
        
        // 获取line坐标
        qreal x1 = dxfFilter.lines.at(i).line.x1;
        qreal y1 = dxfFilter.lines.at(i).line.y1;
        qreal x2 = dxfFilter.lines.at(i).line.x2;
        qreal y2 = dxfFilter.lines.at(i).line.y2;
        
        // 获取line属性
        QColor color = dxfFilter.transformColor(attr.getColor());
        Qt::PenStyle style = dxfFilter.transformStyle(attr.getLinetype());
        int width = dxfFilter.transformWidth(attr.getWidth());
        
        if(type == Sketch){
            // 根据图层名获取图层
            sceneActive = getSceneByName(layer);
            if(!sceneActive){
                sceneActive = new Scene(this);
                QString name = layer;
                sceneActive->setName(name);
                sceneList.append(sceneActive);
            }
        } else{
            // 当类型为排版时，一个图形就是一个图层
            sceneActive = new Scene(this);
            QString name = getNewSceneName();
            sceneActive->setName(name);
            sceneList.append(sceneActive);
        }
        
        // 添加直线元素
        Line *line = new Line;
        Configure::PenStyle pen;
        pen.setPenStyle(color, style, width);
        line->setPenStyle(pen);
        line->setLine(QLineF(x1, y1, x2, y2));
        
        sceneActive->addCustomLineItem(line);
        sceneList[0]->addCustomLineItem(line);
#ifdef DXFDEBUG
        qDebug() << "layer：" << layer;
        qDebug() << "color：" << color;
        qDebug() << "style：" << style;
        qDebug() << "width：" << width;
        qDebug() << "line：" << line->line();
#endif
    }
}

void Project::dxfPolylineReader(const DxfFilter dxfFilter)
{
    for(int i=0; i<dxfFilter.polylines.length();i++){
        // 判断该元素是否在不可用图层上
        DL_Attributes attr = dxfFilter.polylines.at(i).attribute;
        QString layer = dxfFilter.transformText(attr.getLayer());
        if(offLayers.contains(layer)){
            continue;
        }
        
        // 获取polyline的顶点
        QList<DL_VertexData> v = dxfFilter.polylines.at(i).vertexList;
        QList<QPointF> points;
        for(int j=0; j<v.length(); j++){
            // 获取vertex基本信息
            qreal px = v.at(j).x;
            qreal py = v.at(j).y;
            points.append(QPointF(px, py));
        }
        
        // 获取polyline属性
        int flag = dxfFilter.polylines.at(i).polyline.flags;
        double elevation = dxfFilter.polylines.at(i).polyline.elevation;
        QColor color = dxfFilter.transformColor(attr.getColor());
        Qt::PenStyle style = dxfFilter.transformStyle(attr.getLinetype());
        int width = dxfFilter.transformWidth(attr.getWidth());

        if(type == Sketch){
            // 根据图层名获取图层
            sceneActive = getSceneByName(layer);
            if(!sceneActive){
                sceneActive = new Scene(this);
                QString name = layer;
                sceneActive->setName(name);
                sceneList.append(sceneActive);
            }
        } else{
            // 当类型为排版时，一个图形就是一个图层
            sceneActive = new Scene(this);
            QString name = getNewSceneName();
            sceneActive->setName(name);
            sceneList.append(sceneActive);
        }
        
        // 添加点元素
        Polyline *polyline = new Polyline;
        Configure::PenStyle pen;
        pen.setPenStyle(color, style, width);
        polyline->setPenStyle(pen);
        polyline->setPolyline(points, flag, elevation);
        sceneActive->addCustomPolylineItem(polyline);
        sceneList[0]->addCustomPolylineItem(polyline);
#ifndef DXFDEBUG
        qDebug() << "layer：" << layer;
        qDebug() << "color：" << color;
        qDebug() << "style：" << style;
        qDebug() << "width：" << width;
        qDebug() << "polyline";
        qDebug() << "points num: " << polyline->getPoints().length();
        qDebug() << "flag: " << flag;
        qDebug() << "elevation: " << elevation;
#endif
    }
}

void Project::dxfArcReader(const DxfFilter dxfFilter)
{
    for(int i=0; i<dxfFilter.arcs.length();i++){
        // 判断该元素是否在不可用图层上
        DL_Attributes attr = dxfFilter.arcs.at(i).attribute;
        QString layer = dxfFilter.transformText(attr.getLayer());
        if(offLayers.contains(layer)){
            continue;
        }
        
        // 获取arc基本信息
        qreal cx = dxfFilter.arcs.at(i).arc.cx;
        qreal cy = dxfFilter.arcs.at(i).arc.cy;
        qreal r = dxfFilter.arcs.at(i).arc.radius;
        qreal angle1 = dxfFilter.arcs.at(i).arc.angle1;
        qreal angle2 = dxfFilter.arcs.at(i).arc.angle2;
        
        // 获取arc属性
        QColor color = dxfFilter.transformColor(attr.getColor());
        Qt::PenStyle style = dxfFilter.transformStyle(attr.getLinetype());
        int width = dxfFilter.transformWidth(attr.getWidth());
        
        if(type == Sketch){
            // 根据图层名获取图层
            sceneActive = getSceneByName(layer);
            if(!sceneActive){
                sceneActive = new Scene(this);
                QString name = layer;
                sceneActive->setName(name);
                sceneList.append(sceneActive);
            }
        } else{
            // 当类型为排版时，一个图形就是一个图层
            sceneActive = new Scene(this);
            QString name = getNewSceneName();
            sceneActive->setName(name);
            sceneList.append(sceneActive);
        }
        
        // 添加arc元素
        Arc *arc = new Arc;
        Configure::PenStyle pen;
        pen.setPenStyle(color, style, width);
        arc->setPenStyle(pen);
        arc->setArc(cx, cy, r, angle1, angle2);
        
        sceneActive->addCustomArcItem(arc);
        sceneList[0]->addCustomArcItem(arc);
#ifdef DXFDEBUG
        qDebug() << "layer：" << layer;
        qDebug() << "color：" << color;
        qDebug() << "style：" << style;
        qDebug() << "width：" << width;
        qDebug() << "arc：";
#endif
    }
}

void Project::dxfCircleReader(const DxfFilter dxfFilter)
{
    for(int i=0; i<dxfFilter.circles.length();i++){
        // 判断该元素是否在不可用图层上
        DL_Attributes attr = dxfFilter.circles.at(i).attribute;
        QString layer = dxfFilter.transformText(attr.getLayer());
        if(offLayers.contains(layer)){
            continue;
        }
        
        // 获取circle基本信息
        qreal cx = dxfFilter.circles.at(i).circle.cx;
        qreal cy = dxfFilter.circles.at(i).circle.cy;
        qreal r = dxfFilter.circles.at(i).circle.radius;
        
        // 获取circle属性
        QColor color = dxfFilter.transformColor(attr.getColor());
        Qt::PenStyle style = dxfFilter.transformStyle(attr.getLinetype());
        int width = dxfFilter.transformWidth(attr.getWidth());
        
        if(type == Sketch){
            // 根据图层名获取图层
            sceneActive = getSceneByName(layer);
            if(!sceneActive){
                sceneActive = new Scene(this);
                QString name = layer;
                sceneActive->setName(name);
                sceneList.append(sceneActive);
            }
        } else{
            // 当类型为排版时，一个图形就是一个图层
            sceneActive = new Scene(this);
            QString name = getNewSceneName();
            sceneActive->setName(name);
            sceneList.append(sceneActive);
        }
        
        // 添加circle元素
        Circle *circle = new Circle;
        Configure::PenStyle pen;
        pen.setPenStyle(color, style, width);
        circle->setPenStyle(pen);
        circle->setCircle(cx, cy, r);
        
        sceneActive->addCustomCircleItem(circle);
        sceneList[0]->addCustomCircleItem(circle);
#ifdef DXFDEBUG
        qDebug() << "layer：" << layer;
        qDebug() << "color：" << color;
        qDebug() << "style：" << style;
        qDebug() << "width：" << width;
        qDebug() << "circle：" << cx << ", " << cy << ", " << r;
#endif
    }
}

void Project::dxfEllipseReader(const DxfFilter dxfFilter)
{
    for(int i=0; i<dxfFilter.ellipses.length();i++){
        // 判断该元素是否在不可用图层上
        DL_Attributes attr = dxfFilter.ellipses.at(i).attribute;
        QString layer = dxfFilter.transformText(attr.getLayer());
        if(offLayers.contains(layer)){
            continue;
        }
        
        // 获取ellipse基本信息
        qreal cx = dxfFilter.ellipses.at(i).ellipse.cx;
        qreal cy = dxfFilter.ellipses.at(i).ellipse.cy;
        qreal r1 = dxfFilter.ellipses.at(i).ellipse.mx - cx;
        qreal r2 = dxfFilter.ellipses.at(i).ellipse.my - cy;
        qreal angle1 = dxfFilter.ellipses.at(i).ellipse.angle1;
        qreal angle2 = dxfFilter.ellipses.at(i).ellipse.angle2;
        qreal ratio = dxfFilter.ellipses.at(i).ellipse.ratio;
        
        // 获取ellipse属性
        QColor color = dxfFilter.transformColor(attr.getColor());
        Qt::PenStyle style = dxfFilter.transformStyle(attr.getLinetype());
        int width = dxfFilter.transformWidth(attr.getWidth());
        
        if(type == Sketch){
            // 根据图层名获取图层
            sceneActive = getSceneByName(layer);
            if(!sceneActive){
                sceneActive = new Scene(this);
                QString name = layer;
                sceneActive->setName(name);
                sceneList.append(sceneActive);
            }
        } else{
            // 当类型为排版时，一个图形就是一个图层
            sceneActive = new Scene(this);
            QString name = getNewSceneName();
            sceneActive->setName(name);
            sceneList.append(sceneActive);
        }
        
        // 添加ellipse元素
        Ellipse *ellipse = new Ellipse;
        Configure::PenStyle pen;
        pen.setPenStyle(color, style, width);
        ellipse->setPenStyle(pen);
        ellipse->setEllipse(cx, cy, r1, r2, angle1);
        
        sceneActive->addCustomEllipseItem(ellipse);
        sceneList[0]->addCustomEllipseItem(ellipse);
#ifdef DXFDEBUG
        qDebug() << "layer：" << layer;
        qDebug() << "color：" << color;
        qDebug() << "style：" << style;
        qDebug() << "width：" << width;
        qDebug() << "ellipse：";
#endif
    }
}

void Project::dxfTextReader(const DxfFilter dxfFilter)
{
    for(int i=0; i<dxfFilter.texts.length();i++){
        // 判断该元素是否在不可用图层上
        DL_Attributes attr = dxfFilter.texts.at(i).attribute;
        QString layer = dxfFilter.transformText(attr.getLayer());
        if(offLayers.contains(layer)){
            continue;
        }
        
        // 获取texts基本信息
        qreal ipx = dxfFilter.texts.at(i).text.ipx;
        qreal ipy = dxfFilter.texts.at(i).text.ipy;
        qreal apx = dxfFilter.texts.at(i).text.apx;
        qreal apy = dxfFilter.texts.at(i).text.apy;
        qreal xScale = dxfFilter.texts.at(i).text.xScaleFactor;
        int gFlag = dxfFilter.texts.at(i).text.textGenerationFlags;
        int hJust = dxfFilter.texts.at(i).text.hJustification;
        int vJust = dxfFilter.texts.at(i).text.vJustification;
        QString content = dxfFilter.transformText(dxfFilter.texts.at(i).text.text);
        QString font = dxfFilter.transformText(dxfFilter.texts.at(i).text.style);
        qreal angle = dxfFilter.texts.at(i).text.angle;
        
        // 获取texts属性
        QColor color = dxfFilter.transformColor(attr.getColor());
        Qt::PenStyle style = dxfFilter.transformStyle(attr.getLinetype());
        int width = dxfFilter.transformWidth(attr.getWidth());
        
        if(type == Sketch){
            // 根据图层名获取图层
            sceneActive = getSceneByName(layer);
            if(!sceneActive){
                sceneActive = new Scene(this);
                QString name = layer;
                sceneActive->setName(name);
                sceneList.append(sceneActive);
            }
        } else{
            // 当类型为排版时，一个图形就是一个图层
            sceneActive = new Scene(this);
            QString name = getNewSceneName();
            sceneActive->setName(name);
            sceneList.append(sceneActive);
        }
        
//#ifdef DXFDEBUG
#if 0
        // 添加texts元素
        Text *text = new Text;
        Configure::PenStyle pen;
        pen.setPenStyle(color, style, width);
        text->setPenStyle(pen);
        text->setText(cx, cy, r, angle1, angle2);
        
        sceneActive->addCustomTextItem(text);
        sceneList[0]->addCustomTextItem(text);
        
        qDebug() << "layer：" << layer;
        qDebug() << "color：" << color;
        qDebug() << "style：" << style;
        qDebug() << "width：" << width;
        qDebug() << "text：" << content;
#endif
    }
}

void Project::dxfFileWriter(const QString fileName)
{
    DL_Codes::version exportVersion = DL_Codes::AC1009;
    DL_WriterA* dw = dxf.out(fileName.toStdString().c_str(), exportVersion);
    if (dw==NULL) {
        throw(tr("无法打开文件进行写入操作"));
    }
    dxf.writeHeader(*dw);
    
    // 保存layers
    int numberOfLayers = sceneList.length();
    dw->tableLayers(numberOfLayers);
    for(int i=0; i<numberOfLayers; i++){
        std::string name = sceneList.at(i)->getName().toStdString();
        dxf.writeLayer(*dw,
                       DL_LayerData(name, 0),
                       DL_Attributes(
                                     std::string(""),  // leave empty
                                     DL_Codes::black,  // default color
                                     1,  // default width
                                     "CONTINUOUS", 1.0));  // default line style
        
        // save points
        dxfPointWriter(sceneList[i]->getPointList(), dxf, dw);
        
        // save lines
        dxfLineWriter(sceneList[i]->getLineList(), dxf, dw);
        
        // save polylines
        dxfPolylineWriter(sceneList[i]->getPolylineList(), dxf, dw);
        
        // save arcs
        dxfArcWriter(sceneList[i]->getArcList(), dxf, dw);
        
        // save circle
        dxfCircleWriter(sceneList[i]->getCircleList(), dxf, dw);
        
        // save ellipse
        dxfEllipseWriter(sceneList[i]->getEllipseList(), dxf, dw);
    }
    dw->dxfEOF();
    dw->close();
    delete dw;
}

void Project::dxfPointWriter(const QList<Point *> &list, DL_Dxf &dxf, DL_WriterA *dw)
{
    for(int i=0; i<list.length(); i++){
        qreal px = list[i]->point().rx();
        qreal py = list[i]->point().ry();
        qreal pz = 0.0;
        qreal offset = list[i]->getOffset();
        
        std::string layer = list[i]->getLayer().toStdString();
        int color = dxfFilter.transformColor(list[i]->getPenStyle().color);
        qreal width = list[i]->getPenStyle().width;
        
        switch (list[i]->getCrossType()) {
            case normal:
                dxf.writeLine(
                              *dw,
                              DL_LineData(px-offset, py+offset, pz, px+offset, py-offset, pz),
                              DL_Attributes(layer, color, width, "BYLAYER", 1.0));
                dxf.writeLine(
                              *dw,
                              DL_LineData(px+offset, py+offset, pz, px-offset, py-offset, pz),
                              DL_Attributes(layer, color, width, "BYLAYER", 1.0));
                break;
            case upright:
                dxf.writeLine(
                              *dw,
                              DL_LineData(px, py+offset, pz, px, py-offset, pz),
                              DL_Attributes(layer, color, width, "BYLAYER", 1.0));
                dxf.writeLine(
                              *dw,
                              DL_LineData(px+offset, py, pz, px-offset, py, pz),
                              DL_Attributes(layer, color, width, "BYLAYER", 1.0));
                break;
            case none:
                dxf.writePoint(
                               *dw,
                               DL_PointData(px, py),
                               DL_Attributes(layer, color, offset, "BYLAYER", 1.0));
            default:
                break;
        }
    }
}

void Project::dxfLineWriter(const QList<Line *> &list, DL_Dxf &dxf, DL_WriterA *dw)
{
    for(int i=0; i<list.length(); i++){
        qreal px1 = list[i]->line().p1().rx();
        qreal py1 = list[i]->line().p1().ry();
        qreal pz1 = 0.0;
        qreal px2 = list[i]->line().p2().rx();
        qreal py2 = list[i]->line().p2().ry();
        qreal pz2 = 0.0;
        
        std::string layer = list[i]->getLayer().toStdString();
        int color = dxfFilter.transformColor(list[i]->getPenStyle().color);
        qreal width = list[i]->getPenStyle().width;
        
        Shape::ShapeType shapeType = list[i]->getShapeType();
        switch (shapeType) {
            case Shape::Line:
                dxf.writeLine(
                              *dw,
                              DL_LineData(px1, py1, pz1, px2, py2, pz2),
                              DL_Attributes(layer, color, width, "BYLAYER", 1.0));
                break;
            case Shape::MiddleAxis:
                dxf.writeLine(
                              *dw,
                              DL_LineData(px1, py1, pz1, px2, py2, pz2),
                              DL_Attributes(layer, color, width, "DASH", 1.0));
                break;
            case Shape::Direction:{
                qreal offset = list[i]->getArrowSize();
                QLineF v = list[i]->line().unitVector();
                v.setLength(offset);
                v.translate(QPointF(list[i]->line().dx(), list[i]->line().dy()));
                
                QLineF n = v.normalVector();
                n.setLength(n.length() * 0.5);
                QLineF n2 = n.normalVector().normalVector();
                
                QPointF p1 = v.p2();
                QPointF p2 = n.p2();
                QPointF p3 = n2.p2();
                
                dxf.writeLine(
                              *dw,
                              DL_LineData(px1, py1, pz1, px2, py2, pz2),
                              DL_Attributes(layer, color, width, "DASH", 1.0));
                
                dxf.writePolyline(
                                  *dw,
                                  DL_PolylineData(3, 0, 0, 0, 0),
                                  DL_Attributes(layer, color, width, "BYLAYER", 1.0));
                dxf.writeVertex(
                                *dw,
                                DL_VertexData(p1.rx(), p1.ry(), 0, 0));
                dxf.writeVertex(
                                *dw,
                                DL_VertexData(p2.rx(), p2.ry(), 0, 0));
                dxf.writeVertex(
                                *dw,
                                DL_VertexData(p3.rx(), p3.ry(), 0, 0));
                dxf.writePolylineEnd(*dw);
                break;
            }
            default:
                break;
        }
        
    }
}

void Project::dxfPolylineWriter(const QList<Polyline *> &list, DL_Dxf &dxf, DL_WriterA *dw)
{
    for(int i=0; i<list.length(); i++){
        QList<QPointF> points = list[i]->getPoints();
        int pCount = points.length();
        Polyline::Type type = list[i]->getType();
        
        std::string layer = list[i]->getLayer().toStdString();
        int color = dxfFilter.transformColor(list[i]->getPenStyle().color);
        qreal width = list[i]->getPenStyle().width;
        qDebug() << "type" << type;
        dxf.writePolyline(
                          *dw,
                          DL_PolylineData(pCount, 0, 0, (int)type),
                          DL_Attributes(layer, color, width, "BYLAYER", 1.0));
        for(int j=0; j<pCount; j++){
            dxf.writeVertex(
                            *dw,
                            DL_VertexData(points[j].rx(), points[j].ry(), 0, 0));
        }
        dxf.writePolylineEnd(*dw);
    }
}

void Project::dxfArcWriter(const QList<Arc *> &list, DL_Dxf &dxf, DL_WriterA *dw)
{
    for(int i=0; i<list.length(); i++){
        QPointF cPoint = list[i]->getCPoint();
        qreal r = list[i]->getRadius();
        qreal sAngle = list[i]->getSAngle();
        qreal eAngle = list[i]->getEAngle();
        
        std::string layer = list[i]->getLayer().toStdString();
        int color = dxfFilter.transformColor(list[i]->getPenStyle().color);
        qreal width = list[i]->getPenStyle().width;
        dxf.writeArc(
                     *dw,
                     DL_ArcData(cPoint.rx(), cPoint.ry(), 0.0, r, sAngle, eAngle),
                     DL_Attributes(layer, color, width, "BYLAYER", 1.0));
    }
}

void Project::dxfCircleWriter(const QList<Circle *> &list, DL_Dxf &dxf, DL_WriterA *dw)
{
    for(int i=0; i<list.length(); i++){
        QPointF cPoint = list[i]->getCPoint();
        qreal r = list[i]->getRadius();
        
        std::string layer = list[i]->getLayer().toStdString();
        int color = dxfFilter.transformColor(list[i]->getPenStyle().color);
        qreal width = list[i]->getPenStyle().width;
        dxf.writeCircle(
                        *dw,
                        DL_CircleData(cPoint.rx(), cPoint.ry(), 0.0, r),
                        DL_Attributes(layer, color, width, "BYLAYER", 1.0));
    }
}

void Project::dxfEllipseWriter(const QList<Ellipse *> &list, DL_Dxf &dxf, DL_WriterA *dw)
{
    for(int i=0; i<list.length(); i++){
        QPointF cPoint = list[i]->getCPoint();
        qreal r1 = list[i]->getRadius1();
        qreal r2 = list[i]->getRadius1();
        
        std::string layer = list[i]->getLayer().toStdString();
        int color = dxfFilter.transformColor(list[i]->getPenStyle().color);
        qreal width = list[i]->getPenStyle().width;
        dxf.writeEllipse(
                         *dw,
                         DL_EllipseData(cPoint.rx(), cPoint.ry(), 0.0,
                                        cPoint.rx() + r1, cPoint.ry() + r2, 0.0,
                                        r2/r1, 0, 2*M_PI),
                         DL_Attributes(layer, color, width, "BYLAYER", 1.0));
    }
}


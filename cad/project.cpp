#include "project.h"
#include <QMessageBox>
#include <QDebug>

Project::Project(QObject *parent) :
QObject(parent),
type(Sketch),
name(""),
saved(false),
modified(false),
entityFlag(false)
{
    
}

Project::~Project()
{
    qDebug() << "project has been deleted!";
    sceneList.clear();
}

void Project::setType(Project::Type type)
{
    this->type = type;
}

Project::Type Project::getType()
{
    return this->type;
}

void Project::setName(const QString name)
{
    this->name = name;
}

QString Project::getName()
{
    return this->name;
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

int Project::getSceneIdByName(const QString name)
{
    for(int i=0;i<sceneList.length();i++){
        if(sceneList.at(i)->getName() == name){
            return i;
        }
    }
    return -1;
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

bool Project::changeScene(int i, int j)
{
    Scene *currentScene = sceneList[i];
    sceneList[i] = sceneList[j];
    sceneList[j] = currentScene;
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

Scene *Project::getDXFLayer(QString name)
{
    if(type == Sketch){
        // 根据图层名获取图层
        sceneActive = getSceneByName(name);
        if(!sceneActive){
            sceneActive = new Scene(this);
            sceneActive->setName(name);
            SketchConfigure config;
            sceneActive->setEntityStyle(config.eStyle);
            sceneList.append(sceneActive);
        }
    } else{
        // 当类型为排版时，一个图形就是一个图层
        sceneActive = new Scene(this);
        QString newLayer = getNewSceneName();
        sceneActive->setName(newLayer);
        SketchConfigure config;
        sceneActive->setEntityStyle(config.eStyle);
        sceneList.append(sceneActive);
    }
    return sceneActive;
}

void Project::dxfFileReader(const QString fileName)
{
    //初始化dxf文件
    if (!dxf.in(fileName.toStdString(), &dxfFilter)) {
        throw(tr("无法读取DXF文件"));
    }else {
        // 读取图层
        dxfLayerReader(dxfFilter);
        
        // 读取point实体
        dxfPointReader(dxfFilter);
        
        // 读取line实体
        dxfLineReader(dxfFilter);
        
        //  读取Polyline实体
        dxfPolylineReader(dxfFilter);

        //  读取Arc实体
        dxfArcReader(dxfFilter);
        
        //  读取Circle实体
        dxfCircleReader(dxfFilter);
        
        //  读取Ellipse实体
        dxfEllipseReader(dxfFilter);
        
        //  读取Text实体
        dxfTextReader(dxfFilter);

        if(!entityFlag){
            throw(tr("导入文件中不含有效实体！"));
        }

        // 插入新图元
        sceneActive->onNewItem();
        // 更新图层修改标识
        sceneActive->setModified(true);
    }
}

void Project::dxfLayerReader(const DxfFilter dxfFilter)
{
    sceneList.clear();  // 清空图层列表
    if(type == Sketch){
        Scene *scene = new Scene();
        scene->setName(getNewSceneName());
        SketchConfigure config;
        scene->setEntityStyle(config.eStyle);
        sceneList.append(scene);
    }
    for(int i=0; i<dxfFilter.layers.length();i++){
        QString layer = dxfFilter.transformText(dxfFilter.layers.at(i).layer.name);
        bool off = dxfFilter.layers.at(i).layer.off;
        if(!off && type == Sketch){
            QString name = layer;
            Scene *scene = new Scene();
            scene->setName(name);
            SketchConfigure config;
            scene->setEntityStyle(config.eStyle);
            sceneList.append(scene);
        } else{
            offLayers.append(layer);
        }
    }

#ifdef DXFDEBUG
    qDebug() << "图层列表长度为：" << sceneList.length();
    for(int i=0; i<sceneList.length(); i++){
        qDebug() << "第" << i << "个图层名称：" << sceneList[i]->getName();
    }
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

        // 获取图层对象
        sceneActive = getDXFLayer(layer);

        // 获取point坐标
        qreal x = dxfFilter.points.at(i).point.x;
        qreal y = dxfFilter.points.at(i).point.y;

#ifndef DISCARD_DXF_ENTITY_PROPERTITY
        // 获取point属性
        QColor color = dxfFilter.transformColor(attr.getColor());
        Qt::PenStyle style = dxfFilter.transformStyle(attr.getLinetype());
        int width = dxfFilter.transformWidth(attr.getWidth());

        // to do
        // 按dxf文件中的属性进行配置
        // ...

        qDebug() << "layer：" << layer;
        qDebug() << "color：" << color;
        qDebug() << "style：" << style;
        qDebug() << "width：" << width;
#endif

        // 添加点元素
        Point *point = new Point;
        sceneActive->addCustomPointItem(point);
        point->setPoint(QPointF(x,y));

        if(type == Sketch){
            Point *pointCopy = new Point(point);
            sceneList[0]->addCustomPointItem(pointCopy);
            pointCopy->setPoint(QPointF(x,y));
        }
        // 更新实体标识
        entityFlag = true;

#ifdef DXFDEBUG
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
        
        // 获取图层对象
        sceneActive = getDXFLayer(layer);

        // 获取line坐标
        qreal x1 = dxfFilter.lines.at(i).line.x1;
        qreal y1 = dxfFilter.lines.at(i).line.y1;
        qreal x2 = dxfFilter.lines.at(i).line.x2;
        qreal y2 = dxfFilter.lines.at(i).line.y2;

#ifndef DISCARD_DXF_ENTITY_PROPERTITY
        // 获取dxf中line属性
        QColor color = dxfFilter.transformColor(attr.getColor());
        Qt::PenStyle style = dxfFilter.transformStyle(attr.getLinetype());
        int width = dxfFilter.transformWidth(attr.getWidth());

        // to do
        // 按dxf文件中的属性进行配置
        // ...

        qDebug() << "layer：" << layer;
        qDebug() << "color：" << color;
        qDebug() << "style：" << style;
        qDebug() << "width：" << width;
#endif

        // 添加直线元素
        Line *line = new Line;
        if(attr.getLinetype().compare("LINE") == 0){
            line->setShapeType(Shape::Line);
        }
        if(attr.getLinetype().compare("DIRECTION") == 0){
            line->setShapeType(Shape::Direction);
        }
        if(attr.getLinetype().compare("MIDDLEAXIS") == 0){
            line->setShapeType(Shape::MiddleAxis);
        }
        sceneActive->addCustomLineItem(line);
        line->setCustomLine(QLineF(x1, y1, x2, y2));

        if(type == Sketch){
            Line *lineCopy = new Line(line);
            sceneList[0]->addCustomLineItem(lineCopy);
            lineCopy->setCustomLine(QLineF(x1, y1, x2, y2));
        }

        // 更新实体标识
        entityFlag = true;

#ifdef DXFDEBUG
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
        
        // 获取图层对象
        sceneActive = getDXFLayer(layer);

        // 获取polyline的基本信息
        QList<DL_VertexData> v = dxfFilter.polylines.at(i).vertexList;
        QVector<QPointF> points;
        for(int j=0; j<v.length(); j++){
            // 获取vertex基本信息
            qreal px = v.at(j).x;
            qreal py = v.at(j).y;
            points.append(QPointF(px, py));
        }
        int flag = dxfFilter.polylines.at(i).polyline.flags;
        double elevation = dxfFilter.polylines.at(i).polyline.elevation;

#ifndef DISCARD_DXF_ENTITY_PROPERTITY
        // 获取polyline属性
        QColor color = dxfFilter.transformColor(attr.getColor());
        Qt::PenStyle style = dxfFilter.transformStyle(attr.getLinetype());
        int width = dxfFilter.transformWidth(attr.getWidth());

        // to do
        // 按dxf文件中的属性进行配置
        // ...

        qDebug() << "layer：" << layer;
        qDebug() << "color：" << color;
        qDebug() << "style：" << style;
        qDebug() << "width：" << width;
#endif

        // 添加点元素
        Polyline *polyline = new Polyline;
        polyline->setShapeType(Shape::Polyline);
        sceneActive->addCustomPolylineItem(polyline);
        polyline->setPolyline(points, flag, elevation);

        if(type == Sketch){
            Polyline *polylineCopy = new Polyline(polyline);
            polylineCopy->setShapeType(Shape::Polyline);
            sceneList[0]->addCustomPolylineItem(polylineCopy);
            polylineCopy->setPolyline(points, flag, elevation);
        }

        // 更新实体标识
        entityFlag = true;

#ifdef DXFDEBUG
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

        // 获取图层对象
        sceneActive = getDXFLayer(layer);

        // 获取arc基本信息
        qreal cx = dxfFilter.arcs.at(i).arc.cx;
        qreal cy = dxfFilter.arcs.at(i).arc.cy;
        qreal r = dxfFilter.arcs.at(i).arc.radius;
        qreal angle1 = dxfFilter.arcs.at(i).arc.angle1;
        qreal angle2 = dxfFilter.arcs.at(i).arc.angle2;

#ifndef DISCARD_DXF_ENTITY_PROPERTITY
        // 获取arc属性
        QColor color = dxfFilter.transformColor(attr.getColor());
        Qt::PenStyle style = dxfFilter.transformStyle(attr.getLinetype());
        int width = dxfFilter.transformWidth(attr.getWidth());

        // to do
        // 按dxf文件中的属性进行配置
        // ...

        qDebug() << "layer：" << layer;
        qDebug() << "color：" << color;
        qDebug() << "style：" << style;
        qDebug() << "width：" << width;
#endif

        // 添加arc元素
        Arc *arc = new Arc;
        arc->setShapeType(Shape::Arc);
        sceneActive->addCustomArcItem(arc);
        arc->setArc(cx, cy, r, angle1, angle2);

        if(type == Sketch){
            Arc *arcCopy = new Arc(arc);
            arcCopy->setShapeType(Shape::Arc);
            sceneList[0]->addCustomArcItem(arcCopy);
            arcCopy->setArc(cx, cy, r, angle1, angle2);
        }

        // 更新实体标识
        entityFlag = true;

#ifdef DXFDEBUG
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
        
        // 获取图层对象
        sceneActive = getDXFLayer(layer);

        // 获取circle基本信息
        qreal cx = dxfFilter.circles.at(i).circle.cx;
        qreal cy = dxfFilter.circles.at(i).circle.cy;
        qreal r = dxfFilter.circles.at(i).circle.radius;
        
#ifndef DISCARD_DXF_ENTITY_PROPERTITY
        // 获取circle属性
        QColor color = dxfFilter.transformColor(attr.getColor());
        Qt::PenStyle style = dxfFilter.transformStyle(attr.getLinetype());
        int width = dxfFilter.transformWidth(attr.getWidth());

        // to do
        // 按dxf文件中的属性进行配置
        // ...

        qDebug() << "layer：" << layer;
        qDebug() << "color：" << color;
        qDebug() << "style：" << style;
        qDebug() << "width：" << width;
#endif

        // 添加circle元素
        Circle *circle = new Circle;
        if(attr.getLinetype().compare("CIRCLE") == 0){
            circle->setShapeType(Shape::Circle);
        }
        if(attr.getLinetype().compare("HOLE") == 0){
            circle->setShapeType(Shape::Hole);
        }
        sceneActive->addCustomCircleItem(circle);
        circle->setCircle(cx, cy, r);

        if(type == Sketch){
            Circle *circleCopy = new Circle(circle);
            circleCopy->setShapeType(circle->getShapeType());
            sceneList[0]->addCustomCircleItem(circleCopy);
            circleCopy->setCircle(cx, cy, r);
        }

        // 更新实体标识
        entityFlag = true;

#ifdef DXFDEBUG
        qDebug() << "circle：" << cx << ", "
                 << cy << ", " << r;
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
        
        // 获取图层对象
        sceneActive = getDXFLayer(layer);

        // 获取ellipse基本信息
        qreal cx = dxfFilter.ellipses.at(i).ellipse.cx;
        qreal cy = dxfFilter.ellipses.at(i).ellipse.cy;
        qreal mx = dxfFilter.ellipses.at(i).ellipse.mx;
        qreal my = dxfFilter.ellipses.at(i).ellipse.my;
        qreal ratio = dxfFilter.ellipses.at(i).ellipse.ratio;
        qreal angle1 = dxfFilter.ellipses.at(i).ellipse.angle1;
        qreal angle2 = dxfFilter.ellipses.at(i).ellipse.angle2;
        
#ifndef DISCARD_DXF_ENTITY_PROPERTITY
        // 获取ellipse属性
        QColor color = dxfFilter.transformColor(attr.getColor());
        Qt::PenStyle style = dxfFilter.transformStyle(attr.getLinetype());
        int width = dxfFilter.transformWidth(attr.getWidth());

        // to do
        // 按dxf文件中的属性进行配置
        // ...

        qDebug() << "layer：" << layer;
        qDebug() << "color：" << color;
        qDebug() << "style：" << style;
        qDebug() << "width：" << width;
#endif

        // 添加ellipse元素
        Ellipse *ellipse = new Ellipse;
        ellipse->setShapeType(Shape::Ellipse);
        sceneActive->addCustomEllipseItem(ellipse);
        ellipse->setEllipse(cx, cy, mx, my, ratio, angle1, angle2);

        if(type == Sketch){
            Ellipse *ellipseCopy = new Ellipse(ellipse);
            ellipseCopy->setShapeType(Shape::Ellipse);
            sceneList[0]->addCustomEllipseItem(ellipseCopy);
            ellipseCopy->setEllipse(cx, cy, mx, my, ratio, angle1, angle2);
        }

        // 更新实体标识
        entityFlag = true;

#ifdef DXFDEBUG
        qDebug() << "dxfFilter.ellipses.length = " << dxfFilter.ellipses.length();
        qDebug() << "ellipse：";
        qDebug() << "cx: " << cx;
        qDebug() << "cy: " << cy;
        qDebug() << "mx: " << mx;
        qDebug() << "my: " << my;
        qDebug() << "angel1: " << angle1;
        qDebug() << "angel2: " << angle2;
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

        // 获取图层对象
        sceneActive = getDXFLayer(layer);

#if 0
#ifndef DISCARD_DXF_ENTITY_PROPERTITY
        // 获取text属性
        QColor color = dxfFilter.transformColor(attr.getColor());
        Qt::PenStyle style = dxfFilter.transformStyle(attr.getLinetype());
        int width = dxfFilter.transformWidth(attr.getWidth());

        // to do
        // 按dxf文件中的属性进行配置
        // ...

        qDebug() << "layer：" << layer;
        qDebug() << "color：" << color;
        qDebug() << "style：" << style;
        qDebug() << "width：" << width;
#endif

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
        
        // 添加texts元素
        Text *text = new Text;
        text->setShapeType(Shape::Text);
        sceneActive->addCustomTextItem(text);
        text->setText(cx, cy, r, angle1, angle2);

        if(type == Sketch){
            Text *textCopy = new Text(text);
            textCopy->setShapeType(Shape::Text);
            sceneList[0]->addCustomTextItem(textCopy);
            textCopy->setText(cx, cy, r, angle1, angle2);
        }

        // 更新实体标识
        entityFlag = true;

#ifdef DXFDEBUG
        qDebug() << "text：" << content;
#endif
#endif
    }
}

void Project::dxfFileWriter(const QString fileName)
{
    DL_Codes::version exportVersion = DL_Codes::AC1015;
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
        std::string style;
        Shape::ShapeType shapeType = list[i]->getShapeType();
        switch (shapeType) {
        case Shape::Line:
            style = "LINE";
            break;
        case Shape::MiddleAxis:
            style = "MIDDLEAXIS";
            break;
        case Shape::Direction:
            style = "DIRECTION";
            break;
        default:
            style = "BYLAYER";
            break;
        }
        dxf.writeLine(
                      *dw,
                      DL_LineData(px1, py1, pz1, px2, py2, pz2),
                      DL_Attributes(layer, color, width, style, 1.0));

#ifdef DXF_WRITE_DEBUG
        qDebug() << "shapeType: " << shapeType;
        qDebug() << dxfFilter.transformText(style);
#endif
    }
}

void Project::dxfPolylineWriter(const QList<Polyline *> &list, DL_Dxf &dxf, DL_WriterA *dw)
{
    for(int i=0; i<list.length(); i++){
        QVector<QPointF> points = list[i]->getPoints();
        int pCount = points.length();
        Polyline::Type type = list[i]->getType();
        
        std::string layer = list[i]->getLayer().toStdString();
        int color = dxfFilter.transformColor(list[i]->getPenStyle().color);
        qreal width = list[i]->getPenStyle().width;
        std::string style = "BYLAYER";

        dxf.writePolyline(
                          *dw,
                          DL_PolylineData(pCount, 0, 0, (int)type),
                          DL_Attributes(layer, color, width, style, 1.0));
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
        std::string style = "BYLAYER";
        dxf.writeArc(
                     *dw,
                     DL_ArcData(cPoint.rx(), cPoint.ry(), 0.0, r, sAngle, eAngle),
                     DL_Attributes(layer, color, width, style, 1.0));
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
        std::string style;
        Shape::ShapeType shapeType = list[i]->getShapeType();
        switch (shapeType) {
        case Shape::Circle:
            style = "CIRCLE";
            break;
        case Shape::Hole:
            style = "HOLE";
            break;
        default:
            style = "BYLAYER";
            break;
        }
        dxf.writeCircle(
                        *dw,
                        DL_CircleData(cPoint.rx(), cPoint.ry(), 0.0, r),
                        DL_Attributes(layer, color, width, style, 1.0));
    }
}

void Project::dxfEllipseWriter(const QList<Ellipse *> &list, DL_Dxf &dxf, DL_WriterA *dw)
{
    for(int i=0; i<list.length(); i++){
        QPointF cPoint = list[i]->getCPoint();
        QPointF mPoint = list[i]->getMajorPoint();
        qreal ratio = list[i]->getRatio();
        qreal sAngle = list[i]->getSAngle();
        qreal eAngle = list[i]->getEAngle();

        std::string layer = list[i]->getLayer().toStdString();
        int color = dxfFilter.transformColor(list[i]->getPenStyle().color);
        qreal width = list[i]->getPenStyle().width;
        std::string style = "BYLAYER";
        dxf.writeEllipse(
                         *dw,
                         DL_EllipseData(cPoint.rx(), cPoint.ry(), 0.0,
                                        mPoint.rx(), mPoint.ry(), 0.0,
                                        ratio, sAngle, eAngle),
                         DL_Attributes(layer, color, width, style, 1.0));
#ifdef PROJECTDEBUG
        qDebug() << "Ellipse length: " << list.length() << "-" << i;
        qDebug() << "cPoint: " << cPoint;
        qDebug() << "mPoint: " << mPoint;
        qDebug() << "ratio: " << ratio;
        qDebug() << "sAngle: " << sAngle;
        qDebug() << "eAngle: " << eAngle;
#endif
    }
}

void Project::tnfFileWriter(const QString fileName,
                            const QList<Sheet*> sheetList,
                            const QList<PieceCenter> pieceCenterList,
                            const QList<PieceOffset> pieceOffsetList)
{
    NF_Writer* nw = tnf.out(fileName.toStdString().c_str());
    if (nw==NULL) {
        throw(tr("无法打开文件进行写入操作"));
    }
    nw->writeHeader("SATURNO2 ST2 CAM FILE by LEANEST (deep): CamSaturno2 V1.8.09");
    nw->writeSheet(sheetList);

    int centerCount = pieceCenterList.length();
    nw->writePieceCenterHeader(centerCount);

    nw->writePieceCenter(pieceCenterList);

    int offsetCount = pieceOffsetList.length();
    nw->writePieceOffsetHeader(offsetCount);

    nw->writePieceOffset(pieceOffsetList);
    nw->close();
    delete nw;
}


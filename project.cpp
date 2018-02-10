#include "project.h"
#include <QDebug>

Project::Project(QString name, QWidget *parent)
{
    this->name = name;
    this->saved = true;
    layer_active = new Layer(parent);
    this->addLayer(layer_active);
}

bool Project::isSaved()
{
    for(int i=0; i<this->layer.length();i++){
        this->saved &= !this->layer.at(i)->isModified();
    }
    return this->saved;
}

void Project::setSaved(bool saved)
{
    this->saved = saved;
}

QString Project::getName()
{
    return this->name;
}

void Project::setName(QString name)
{
    this->name = name;
}

void Project::addLayer(Layer *layer)
{
    this->layer.append(layer);
}

Layer * Project::addLayer()
{
    layer_active = new Layer;
    this->addLayer(layer_active);
    return layer_active;
}

QList<Layer *> Project::getLayerList()
{
    return this->layer;
}

Layer *Project::getLayer(int index)
{
    int length = this->layer.length();
    if(index > length){
        return NULL;
    }
    return this->layer.at(index);
}

Layer *Project::getActiveLayer()
{
    return this->layer_active;
}

void Project::setActiveLayer(Layer *layer)
{
    this->layer_active = layer;
}

bool Project::saveProject()
{
    for(int i=0; i<this->layer.length();i++){
        this->layer.at(i)->setModified(false);
    }
    this->saved = true;
    qDebug() << this->name << "已保存" <<endl;
    return true;
}

QString Project::getNewLayerName()
{
    // 获取图层数目
    int len_layer = layer.length() + 1;
    QString str = QString::number(len_layer, 10);
    char *ch;
    QByteArray ba = str.toLatin1();
    ch = ba.data();
    return tr(ch);
}

QString Project::getLayerName(Layer *layer)
{
    return layer->getName();
}

QString Project::getLayerName(int i)
{
    return layer.at(i)->getName();
}

void Project::dxfFileParser(QString fileName)
{
    //初始化第一个dxf文件
    if (!dxf.in(fileName.toStdString(), &dxf_filter)) {
        qDebug() << "can not read datas";
    }else {
        qDebug() << "success";
        layer.clear();
        for(int i=0; i<dxf_filter.layers.length();i++){
            bool off = dxf_filter.layers.at(i).layer.off;
            if(!off){
                QString name = QString::fromStdString(dxf_filter.layers.at(i).layer.name);
                qDebug() << name;
                qDebug() << dxf_filter.layers.at(i).attribute.getColor();
                Layer *area = new Layer();
                area->setName(name);
                layer.append(area);
            }
        }
        qDebug() << layer.length();
        layer_active = layer.at(0);
        for(int i=0; i<dxf_filter.points.length();i++){
            layer_active->setCurShape(Layer::Point);
//            int x = dxf_filter.points.at(i).point.x;
//            int y = dxf_filter.points.at(i).point.y;
//            int z = dxf_filter.points.at(i).point.z;
//            layer_active->setPoint(Layer::First, x, y, z);
//            layer_active->paint();
        }

        for(int i=0; i<dxf_filter.lines.length();i++){
            layer_active->setCurShape(Layer::Line);
            int x1 = dxf_filter.lines.at(i).line.x1;
            int y1 = dxf_filter.lines.at(i).line.y1;
            int z1 = dxf_filter.lines.at(i).line.z1;
            int x2 = dxf_filter.lines.at(i).line.x2;
            int y2 = dxf_filter.lines.at(i).line.y2;
            int z2 = dxf_filter.lines.at(i).line.z2;
//            layer_active->setPoint(Layer::First, x1, y1, z1);
//            layer_active->setPoint(Layer::Second, x2, y2, z2);

            int color = dxf_filter.lines.at(i).attribute.getColor();
            qDebug() << "color" << color;
            int width = dxf_filter.lines.at(i).attribute.getWidth();
            if(width == -1){
                width = 10;
            }
            qDebug() << "width" << width;
            layer_active->setPenColor(layer_active->transformIntToQColor(color));
            layer_active->setPenWidth(width);
//            layer_active->paint();
        }
    }
//    layer_active->initLayer();
}

//void Project::onActiveProjectChanged(Project *active_project)
//{
//    qDebug() << active_project->name;
//}

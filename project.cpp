#include "project.h"
#include <QDebug>

Project::Project(QString name, QWidget *parent)
{
    this->name = name;
    this->saved = true;
    PaintArea *area = new PaintArea;
    area->setMinimumSize(400, 400);
    area->setName(this->getNewLayerName());
    layer_active = area;
    this->addLayer(layer_active);
}

bool Project::isSaved()
{
    for(int i=0; i<this->layer.length();i++){
        this->saved &= this->layer.at(i)->isModified();
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

void Project::addLayer(PaintArea *area)
{
    this->layer.append(area);
}

PaintArea * Project::addLayer()
{
    layer_active = new PaintArea;
    layer_active->setMinimumSize(400, 400);
    layer_active->setName(this->getNewLayerName());
    this->addLayer(layer_active);
    return layer_active;
}

QList<PaintArea *> Project::getLayerList()
{
    return this->layer;
}

PaintArea *Project::getLayer(int index)
{
    int length = this->layer.length();
    if(index > length){
        return NULL;
    }
    return this->layer.at(index);
}

PaintArea *Project::getActiveLayer()
{
    return this->layer_active;
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

QString Project::getLayerName(PaintArea *layer)
{
    return layer->getName();
}

//void Project::onActiveProjectChanged(Project *active_project)
//{
//    qDebug() << active_project->name;
//}

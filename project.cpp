#include "project.h"
#include <QDebug>

Project::Project(QWidget *parent) :
    QWidget(parent),
    name(""),
    saved(false),
    modified(false)
{

}

QString Project::getName()
{
    return this->name;
}

void Project::setName(QString name)
{
    this->name = name;
}

void Project::changeName(QString name)
{
    emit projectNameChanged(this->name, name);
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

Scene * Project::addScene()
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

Scene *Project::getScene(int index)
{
    int length = this->sceneList.length();
    if(index > length){
        return NULL;
    }
    return this->sceneList.at(index);
}

Scene *Project::getSceneByName(QString name)
{
    for(int i=0;i<sceneList.length();i++){
        if(sceneList.at(i)->getName() == name){
            return sceneList.at(i);
        }
    }
    return NULL;
}

Scene *Project::getActiveScene()
{
    return this->sceneActive;
}

void Project::setSaved(bool saved)
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
        this->modified |= !this->sceneList.at(i)->isModified();
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

QString Project::getSceneName(int i)
{
    return sceneList.at(i)->getName();
}

void Project::dxfFileParser(QString fileName)
{
    //初始化dxf文件
    if (!dxf.in(fileName.toStdString(), &dxfFilter)) {
        qDebug() << "can not read datas";
    }else {
        qDebug() << "success";
        sceneList.clear();
        for(int i=0; i<dxfFilter.layers.length();i++){
            bool off = dxfFilter.layers.at(i).layer.off;
            if(!off){
                QString name = QString::fromStdString(dxfFilter.layers.at(i).layer.name);
                qDebug() << name;
                qDebug() << dxfFilter.layers.at(i).attribute.getColor();
                Scene *scene = new Scene();
                scene->setName(name);
                sceneList.append(scene);
            }
        }
        qDebug() << sceneList.length();
        sceneActive = sceneList.at(0);
        for(int i=0; i<dxfFilter.points.length();i++){
            sceneActive->setCurShape(Shape::Point);
        }

        for(int i=0; i<dxfFilter.lines.length();i++){
            sceneActive->setCurShape(Shape::Line);
            int x1 = dxfFilter.lines.at(i).line.x1;
            int y1 = dxfFilter.lines.at(i).line.y1;
            int z1 = dxfFilter.lines.at(i).line.z1;
            int x2 = dxfFilter.lines.at(i).line.x2;
            int y2 = dxfFilter.lines.at(i).line.y2;
            int z2 = dxfFilter.lines.at(i).line.z2;

            int color = dxfFilter.lines.at(i).attribute.getColor();
            qDebug() << "color" << color;
            int width = dxfFilter.lines.at(i).attribute.getWidth();
            if(width == -1){
                width = 10;
            }
            qDebug() << "width" << width;
        }
    }
//    sceneActive->initScene();
}


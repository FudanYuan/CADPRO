#ifndef PROJECT_H
#define PROJECT_H
#include "scene.h"
#include "dxffilter.h"
#include "dl_dxf.h"

#include <QWidget>
#include <QString>
#include <QList>

class Project : public QObject
{
    Q_OBJECT
public:
    explicit Project(QObject *parent = 0);

    void setName(QString name);  // 设置项目名称
    void changeName(QString name);  // 更改项目名称
    QString getName();  // 获取项目名称

    void addScene(Scene *scene);  // 添加项目图层
    Scene *addScene();  // 添加项目图层
    void setActiveScene(Scene *Scene);  // 设置活动图层

    QList<Scene *> getSceneList();  // 获取项目图层列表
    Scene* getScene(int index);  // 获取项目图层
    Scene* getSceneByName(QString name);  // 通过名称获取项目图层
    Scene* getActiveScene();  // 获取项目活动图层

    void setSaved(bool saved);  // 设置项目保存状态
    bool isSaved();  // 判断项目是否被保存过

    bool isModified();  // 判断项目是否被更改过

    bool saveProject();  // 保存项目

    QString getNewSceneName();  // 获取新图层名称
    QString getSceneName(Scene *Scene);  // 获取图层名称
    QString getSceneName(int i);  // 获取第i层图层名称

    void dxfFileParser(QString fileName);  // 解析dxf文件

private:
    QString name;  // 项目名称
    QList<Scene *> sceneList;  // 图层对象
    Scene *sceneActive;  // 活动图层
    bool saved;  // 是否保存
    bool modified;  // 是否更改

    DxfFilter dxfFilter;
    DL_Dxf dxf;
protected:

signals:
    void projectNameChanged(QString, QString);

public slots:
};

#endif // PROJECT_H

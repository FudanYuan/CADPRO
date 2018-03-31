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
    enum Type{
        Sketch,
        Nest
    };
    explicit Project(QObject *parent = 0);
    ~Project();
    void setType(Type type);  // 设置类型
    Type getType();  // 获取类型
    void setName(const QString name);  // 设置项目名称
    void changeName(const QString name);  // 更改项目名称
    QString getName();  // 获取项目名称
    void addScene(Scene *scene);  // 添加项目图层
    Scene *addScene();  // 添加项目图层
    void setActiveScene(Scene *Scene);  // 设置活动图层
    QList<Scene *> getSceneList();  // 获取项目图层列表
    Scene* getScene(const int index);  // 获取项目图层
    Scene* getSceneByName(const QString name);  // 通过名称获取项目图层
    bool removeScene(const int index);  // 删除项目图层
    bool removeSceneByName(const QString name); // 通过名称删除项目图层
    Scene* getActiveScene();  // 获取项目活动图层
    void setSaved(const bool saved);  // 设置项目保存状态
    bool isSaved();  // 判断项目是否被保存过
    bool isModified();  // 判断项目是否被更改过
    bool saveProject();  // 保存项目
    QString getNewSceneName();  // 获取新图层名称
    QString getSceneName(Scene *Scene);  // 获取图层名称
    QString getSceneName(const int i);  // 获取第i层图层名称

    // dxf 读
    Scene* getDXFLayer(QString name);  //  获取dxf图层对象
    void dxfFileReader(const QString fileName);  // 解析dxf文件
    void dxfLayerReader(const DxfFilter dxfFilter);  // 解析layer
    void dxfPointReader(const DxfFilter dxfFilter);  // 解析point实体
    void dxfLineReader(const DxfFilter dxfFilter);  // 解析line实体
    void dxfPolylineReader(const DxfFilter dxfFilter);  // 解析polyline实体
    void dxfArcReader(const DxfFilter dxfFilter);  // 解析arc实体
    void dxfCircleReader(const DxfFilter dxfFilter);  // 解析circle实体
    void dxfEllipseReader(const DxfFilter dxfFilter);  // 解析ellipse实体
    void dxfTextReader(const DxfFilter dxfFilter);  // 解析text实体

    // dxf 写
    void dxfFileWriter(const QString fileName);  // 存储dxf文件
    void dxfPointWriter(const QList<Point *> &list, DL_Dxf &dxf, DL_WriterA* dw);  // 存储点
    void dxfLineWriter(const QList<Line *> &list, DL_Dxf &dxf, DL_WriterA* dw);  // 存储直线
    void dxfPolylineWriter(const QList<Polyline *> &list, DL_Dxf &dxf, DL_WriterA* dw);  // 存储多边形
    void dxfArcWriter(const QList<Arc *> &list, DL_Dxf &dxf, DL_WriterA* dw);  // 存储圆弧
    void dxfCircleWriter(const QList<Circle *> &list, DL_Dxf &dxf, DL_WriterA* dw);  // 存储圆
    void dxfEllipseWriter(const QList<Ellipse *> &list, DL_Dxf &dxf, DL_WriterA* dw);  // 存储椭圆
//    void dxfTextWriter(const QList<Text *> &list, DL_Dxf &dxf, DL_WriterA* dw);  // 存储文本

private:
    Type type;  // 类型
    QString name;  // 项目名称
    QList<Scene *> sceneList;  // 图层对象
    Scene *sceneActive;  // 活动图层
    bool saved;  // 是否保存
    bool modified;  // 是否更改

    DxfFilter dxfFilter;  // dxf 操作
    DL_Dxf dxf;
    QList<QString> offLayers; // 关闭的layer
    bool entityFlag;  // 实体标识，有实体的话为true

signals:
    void projectNameChanged(QString, QString);
};

#endif // PROJECT_H

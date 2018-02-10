#ifndef PROJECT_H
#define PROJECT_H
#include "layer.h"
#include "dxffilter.h"
#include "dxflib/dl_dxf.h"

#include <QWidget>
#include <QString>
#include <QList>

class Project : public QWidget
{
    Q_OBJECT
public:
    explicit Project(QString name, QWidget *parent = 0);
    QString getName();                              // 获取项目名称
    void setName(QString name);                     // 设置项目名称
    void addLayer(Layer *layer);                 // 添加项目图层
    Layer *addLayer();                          // 添加项目图层
    QList<Layer *> getLayerList();              // 获取项目图层列表
    Layer* getLayer(int index);                 // 获取项目图层
    Layer* getActiveLayer();                    // 获取项目活动图层
    void setActiveLayer(Layer *layer);          // 设置活动图层
    bool isSaved();                                 // 判断项目是否被保存过
    void setSaved(bool saved);                      // 设置项目保存状态
    bool saveProject();                             // 保存项目
    QString getNewLayerName();                      // 获取新图层名称
    QString getLayerName(Layer *layer);         // 获取图层名称
    QString getLayerName(int i);                    // 获取图层名称
    void dxfFileParser(QString fileName);           // 解析dxf文件

private:
    QString name;  // 项目名称
    QList<Layer *> layer;  // 图层对象
    Layer *layer_active;  // 活动图层
    bool saved;  // 是否保存

    DxfFilter dxf_filter;
    DL_Dxf dxf;
protected:

signals:

public slots:
};

#endif // PROJECT_H

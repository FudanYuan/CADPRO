#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPaintEvent>
#include <QList>
#include "configure.h"
#include "shape.h"
#include "line.h"
#include "rect.h"
#include "ellipse.h"
#include "circle.h"
#include "arc.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(QObject *parent = 0);

    void setName(QString name);  // 设置图层名称
    QString getName();  // 获取图层名称

    void setCurShape(Shape::ShapeType curShape);  // 设置当前图形
    Shape::ShapeType getCurShape();  // 获取当前图形

    int getitemListLength();  // 获取item列表长度

    void setModified(bool modified);  // 设置画布是否被更改
    bool isModified() const;  // 返回画布内容是否被更改过

    void setMoveable(bool moveable);  // 设置图元是否可移动
    bool isMoveable() const;  // 返回图元是否可移动

    void setDrawable(bool flag);  // 设置开始标识

    void setEntityStyle(EntityStyle eStyle);  // 设置实体样式
    EntityStyle getEntityStyle();  // 获取实体样式

    void setAxesGrid(AxesGrid axesGrid);  // 设置网格坐标轴

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;
    void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;

private:
    QString name;  // 图层名称
    Shape::ShapeType preShape;  // 上一个图形
    Shape::ShapeType curShape;  // 当前图形
    QList<Shape *> itemList;  // 图元列表
    Shape *curItem;  // 设置当前图形
    bool modified;  // 标志图层是否被更改过

    bool drawable;  // 可画标识, 不可画时即可选中
    bool moveable;  // 设置图元是否可移动

    bool drawing;  // 是否在绘制特殊图形

    qreal penWidth;  // 画笔
    qreal scaleFactor;  // 缩放因子

    // 配置文件
    EntityStyle eStyle;  // 实体类型
    AxesGrid axesGrid;  // 坐标网格

signals:
    void sceneScaleChanged(qreal scaleFactor);  // scene缩放事件
    void sceneItemsChanged();  // scene图元改变
    void sceneNameChanged(QString, QString); // scene名称改变

public slots:
    void onViewScaleChanged(qreal scaleFactor);  // 响应view缩放事件
    void onAxesChanged(bool show);  // 响应坐标轴是否显示
    void onGridChanged(bool show);  // 响应网格是否显示
};

#endif // SCENE_H

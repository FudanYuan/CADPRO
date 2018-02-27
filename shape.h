#ifndef SHAPE_H
#define SHAPE_H

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include "configure.h"

class Shape : public QObject
{
    Q_OBJECT
public:
    // 枚举变量，几个图形的选择
    enum ShapeType {
        None,
        Point,
        Line,
        MiddleAxis,
        PolyLine,
        Rectangle,
        Ellipse,
        Circle,
        Arc,
        Arc2,
        Polygon
    };
    Shape();
    void setScaleFactor(qreal scaleFactor);
    void setMoveable(bool moveable);  // 设置可移动性
    virtual void startDraw(QGraphicsSceneMouseEvent * event) = 0;
    virtual void drawing(QGraphicsSceneMouseEvent * event) = 0;
    virtual bool updateFlag(QGraphicsSceneMouseEvent *event) = 0;  // 更新标识
    void setPenStyle(Configure::PenStyle penStyle);  // 设置实体类型
    void setEntityUnderCursorStyle(Configure::PenStyle underCursorStyle);  // 设置光标下类型
    void setSelectStyle(Configure::PenStyle selectedEntity);  // 选择实体类型

    void setShapeId(int id);  // 设置实体id
    int getShapeId();  // 获取实体id

    void setLayer(QString layer);  // 设置图层
    QString getLayer();  // 获取图层

    void setShapeType(ShapeType shape);  // 设置类型
    ShapeType getShapeType();  // 获取类型

    void setSelectable(bool selectable);  // 设置可选择标志
    bool getSelectable() const;  // 获取可选择标志

    void setOverFlag(bool overFlag);  // 结束标志
    bool getOverFlag() const;  // 获取结束标识

    void setSelected(bool selected);  // 设置选中
    bool isSelected() const;  // 是否选中
protected:
    qreal scaleFactor;  // 缩放因子
    bool moveable;  // 是否可移动
    bool selectable;  // 可选择标识
    bool overFlag;  // 结束标识
    bool selected;  // 选中标识

    int id;  // 编号
    Configure::PenStyle penStyle;  // 属性
    Configure::PenStyle underCursorStyle;  // 光标下属性
    Configure::PenStyle selectedEntity;  // 选中属性

    QString layer;  // 图层名称
    ShapeType shape;  // 类型
signals:
    void sceneMoveableChanged(bool moveable);  // 场景可移动性信号

public slots:
    virtual void onSceneMoveableChanged(bool moveable) = 0;  //  响应场景可移动性改变

};

#endif // SHAPE_H

#ifndef SHAPE_H
#define SHAPE_H

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPointF>
#include "common.h"
#include "sketchconfigure.h"

class Shape : public QObject
{
    Q_OBJECT
public:
    // 枚举变量，几个图形的选择
    enum ShapeType {
        None,
        Point, // 点
        Line,  // 线
        Direction,  // 方向轴
        MiddleAxis, // 中心轴
        Polyline,   // 中心轴
        Rectangle,  // 矩形
        Ellipse,    // 椭圆
        Circle,     // 圆
        Arc,        // 弧1
        Arc2,       // 弧2
        Polygon,    // 多边形
        Hole,       // 冲孔
        Eyelet,     // 鸡眼（形似操场）
        Text,       // 文本
        image,      // 图像
        Trapezium   //梯形
    };
    Shape();
    void setScaleFactor(qreal scaleFactor);
    void setMoveable(bool moveable);  // 设置可移动性

    virtual void startDraw(QGraphicsSceneMouseEvent * event) = 0;
    virtual void drawing(QGraphicsSceneMouseEvent * event) = 0;
    virtual bool updateFlag(QGraphicsSceneMouseEvent *event) = 0;  // 更新标识

    void setObjectSize(SketchConfigure::ObjSize objSize);  // 设置对象识别距离

    void setPenStyle(SketchConfigure::PenStyle penStyle);  // 设置实体类型
    SketchConfigure::PenStyle getPenStyle();  // 获取画笔属性

    void setEntityUnderCursorStyle(SketchConfigure::PenStyle underCursorStyle);  // 设置光标下类型
    void setSelectStyle(SketchConfigure::PenStyle selectedEntity);  // 选择实体类型

    void setShapeId(int id);  // 设置实体id
    int getShapeId();  // 获取实体id

    void setLayer(QString layer);  // 设置图层
    QString getLayer();  // 获取图层

    void setName(QString name);  // 设置名称
    QString getName();  // 获取名称

    void setNumber(int num);  // 设置个数
    int getNumber();  // 获取个数

    void setShapeType(ShapeType shape);  // 设置类型
    ShapeType getShapeType();  // 获取类型

    void setSelectable(bool selectable);  // 设置可选择标志
    bool isSelectable() const;  // 获取可选择标志

    void setOverFlag(bool overFlag);  // 结束标志
    bool getOverFlag() const;  // 获取结束标识

    void setSelected(bool selected);  // 设置选中
    bool isSelected() const;  // 是否选中

    void setShowNode(bool showNode);  // 设置显示端点
    bool isShowNode() const;  // 是否显示端点

    void setEditable(bool editable);  // 设置可编辑
    bool isEditable() const;  // 是否可编辑

    void setEditOverFlag(bool editOverFlag);  // 结束标志
    bool getEditOverFlag() const;  // 获取结束标识

    void setFill(bool fill);  // 设置是否填充
    bool isFill() const;  // 是否填充

    void setCollision(bool coll);  // 设置是否碰撞
    bool getCollision() const;  // 是否碰撞

protected:
    QString layer;  // 图层名称
    QString name;  // 图形名称
    ShapeType shape;  // 类型
    int id;  // 编号
    int number;  // 图形个数
    int precsion;  // 小数点位数
    qreal scaleFactor;  // 缩放因子
    bool moveable;  // 是否可移动
    bool selectable;  // 可选择标识
    bool overFlag;  // 结束标识
    bool selected;  // 选中标识
    bool showNode;  // 显示节点
    bool editable;  // 可编辑
    bool editOverFlag;  // 编辑结束
    bool fill;  // 是否填充，仅针对封闭图形
    mutable bool collision;  // 是否碰撞

    SketchConfigure::ObjSize objSize;  // 对象大小
    SketchConfigure::PenStyle penStyle;  // 属性
    SketchConfigure::PenStyle underCursorStyle;  // 光标下属性
    SketchConfigure::PenStyle selectedEntity;  // 选中属性

signals:
    void sceneMoveableChanged(bool moveable);  // 场景可移动性信号
    void shapeEdit(int id);
public slots:
    virtual void onSceneMoveableChanged(bool moveable) = 0;  // 响应场景可移动性改变
};

#endif // SHAPE_H

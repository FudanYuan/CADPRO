﻿#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPaintEvent>
#include <QList>
#include <QLine>
#include "sketchconfigure.h"
#include "nestconfigure.h"
#include "shape.h"
#include "point.h"
#include "line.h"
#include "polyline.h"
#include "polygon.h"
#include "rect.h"
#include "ellipse.h"
#include "circle.h"
#include "arc.h"
#include "trapezium.h"
#include "eyelet.h"
#include "text.h"
#include "sheet.h"


class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum Type{
        Sketch,
        Nest
    };
    Scene(QObject *parent = 0);
    ~Scene();

    void setType(Type type);  // 设置类型
    Type getType();  // 获取类型

    void setName(QString name);  // 设置图层名称
    QString getName();  // 获取图层名称

    void setCurShape(Shape::ShapeType curShape);  // 设置当前图形
    Shape::ShapeType getCurShape();  // 获取当前图形

    QList<Shape *> getItemList() const;  // 获取item列表
    int getItemListLength() const;  // 获取item列表长度

    QList<Point *> getPointList() const;  // 获取点列表
    int getPointListLength() const;  // 获取点列表长度

    QList<Line *> getLineList() const;  // 获取直线列表
    int getLineListLength() const;  // 获取直线列表长度

    QList<Polyline *> getPolylineList() const;  // 获取多边形列表
    int getPolylineListLength() const;  // 获取多边形列表长度
    void addPolylineList(Polyline *polyline);

    QList<Ellipse *> getEllipseList() const;  // 获取椭圆列表
    int getEllipseListLength() const;  // 获取椭圆列表长度

    QList<Circle *> getCircleList() const;  // 获取圆列表
    int getCircleListLength() const;  // 获取圆列表长度

    QList<Arc *> getArcList() const;  // 获取圆弧列表
    int getArcListLength() const;  // 获取圆弧列表长度

    void setModified(bool modified);  // 设置画布是否被更改
    bool isModified() const;  // 返回画布内容是否被更改过

    void setMoveable(bool moveable);  // 设置图元是否可移动
    bool isMoveable() const;  // 返回图元是否可移动

    void setDrawable(bool flag);  // 设置开始标识

    void setBackgroundColor(QColor color);  // 设置背景颜色
    QColor getBackgroundColor();  // 获取背景颜色

    void setOffset(QPointF pos);  // 设置偏移量
    QPointF getOffset();  // 获取偏移量

    void setEntityStyle(const SketchConfigure::EntityStyle &eStyle);  // 设置实体样式
    SketchConfigure::EntityStyle getEntityStyle();  // 获取实体样式

    void setAxesGrid(SketchConfigure::AxesGrid axesGrid);  // 设置网格坐标轴

    void setSheetStyle(const NestConfigure::SheetStyle &style);  // 设置材料类型
    NestConfigure::SheetStyle getSheetStyle() const;  // 获取实体样式

    void setMainGrid(const NestConfigure::Grid &grid);  // 设置主网格
    NestConfigure::Grid getMainGrid() const;  // 获取主网格

    void setSecondGrid(const NestConfigure::Grid &grid);  // 设置主网格
    NestConfigure::Grid getSecondGrid() const;  // 获取主网格

    void setSheet(const Sheet &s);  // 设置材料类型
    Sheet getSheet() const;  // 获取材料类型

    // 添加自定义图形元素
    void addCustomPointItem(Point *point);  // 添加自定义点
    void addCustomLineItem(Line *line);  // 添加自定义直线
    void addCustomPolylineItem(Polyline *polyline);  // 添加自定义直线
    void addCustomRectItem(Rect *rect);  // 添加自定义矩形
    void addCustomEllipseItem(Ellipse *ellipse);  // 添加自定义椭圆
    void addCustomCircleItem(Circle *circle);  // 添加自定义圆形
    void addCustomArcItem(Arc *arc);  // 添加自定义圆弧
    void addCustomItem();  // 添加自定义元素图形
    void clearCustomItem();  // 清空自定义图元

    //绘制正多边形传递参数
    int getpolygonType() const;
    void setpolygonType(int value);
    int getpolygonLineNum() const;
    void setpolygonLineNum(int value);
    double getpolygonRadius() const;
    void setpolygonRadius(double value);
    qreal getpolygonAlpha() const;
    void setpolygonAlpha(const qreal &value);

    //绘制梯边形的传递参数
    double gettrapeziumH() const;
    void settrapeziumH(double value);
    double gettrapeziumToplength() const;
    void settrapeziumToplength(double value);
    double gettrapeziumAlpha1() const;
    void settrapeziumAlpha1(double value);
    double gettrapeziumAlpha2() const;
    void settrapeziumAlpha2(double value);
    int gettrapeziumType() const;
    void settrapeziumType(int value);

    Eyelet *getEyeletDialog() const;
    void setEyeletDialog(Eyelet *value);

    Text *getTextdialog() const;
    void setTextdialog(Text *value);

    Scene *copy();  // 获取该对象
protected:
    int polygonType;  //绘制正多边形传递的线类型
    int polygonLineNum;//绘制正多边形传递的边数
    double polygonRadius;//绘制正多边形传递的半径
    qreal polygonAlpha;  // 绘制正多边形传递的旋转角度

    double trapeziumH;//梯形的高
    double trapeziumToplength;//梯形的上底
    double trapeziumAlpha1;//梯形的角度1
    double trapeziumAlpha2;//梯形的角度2
    int trapeziumType;//梯形的类型

    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;
    void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;

private:
    Type type;  // 角色类型：SKetch, Nest
    QString name;  // 图层名称
    Shape::ShapeType preShape;  // 上一个图形
    Shape::ShapeType curShape;  // 当前图形
    QList<Shape *> itemList;  // 图元列表
    QList<Point *> pointList;  // 点列表
    QList<Line *> lineList;  // 直线列表
    QList<Polyline *> polylineList;  // 多边形列表
    QList<Ellipse *> ellipseList;  // 椭圆列表
    QList<Circle *> circleList;  // 圆列表
    QList<Arc *> arcList;  // 圆弧列表
    Shape *curItem;  // 设置当前图形
    bool modified;  // 标志图层是否被更改过

    bool drawable;  // 可画标识, 不可画时即可选中
    bool moveable;  // 设置图元是否可移动
    bool editable;  // 表示现在可更改图元
    bool drawing;  // 是否在绘制图形

    qreal penWidth;  // 画笔
    qreal scaleFactor;  // 缩放因子
    QPointF offset;  // 偏移量
    QColor backgroundColor;  // 背景颜色

    // 绘图配置文件
    SketchConfigure::EntityStyle eStyle;  // 实体类型
    SketchConfigure::AxesGrid axesGrid;  // 坐标网格

    // 排版配置文件
    NestConfigure::SheetStyle sheetStyle;  // 材料类型
    NestConfigure::Grid mainGrid;  // 主网格
    NestConfigure::Grid secondGrid;  // 副网格
    Sheet sheet;  // 材料

    Eyelet *eyeletDialog ;//绘制鸡眼孔
    Text *textdialog;//文本的对话框传递

    Rect *recttram;//矩形变化
    Polygon *polygontram;//正多边形变化

signals:
    void sceneScaleChanged(qreal scaleFactor);  // scene缩放事件
    void sceneItemsChanged();  // scene图元改变
    void sceneNameChanged(QString, QString); // scene名称改变
    void pointSelected(Point *point);
    void lineSelected(Line *line);
    void arcSelected(Arc *arc);
    void ellipseSelected(Ellipse *ellipse);
    void circleSelected(Circle *circle);
    void rectSelected(Rect *rect);
    void polylineSelected(Polyline *polyline);

public slots:
    void onViewScaleChanged(qreal scaleFactor);  // 响应view缩放事件
    void onViewOffsetChanged(QPointF offset);  // 响应view偏移事件
    void onAxesChanged(bool show);  // 响应坐标轴是否显示
    void onGridChanged(bool show);  // 响应网格是否显示
    void onNewItem();  // 图元改变
    void onPointSelected(Point *point);
    void onLineSelected(Line *line);  // 响应图形被选中
    void onArcSelected(Arc *arc);
    void onEllipseSelected(Ellipse *ellipse);
    void onCircleSelected(Circle *circle);
    void onRectSelected(Rect *rect);
    void onPolylineSelected(Polyline *polyline);
};

#endif // SCENE_H

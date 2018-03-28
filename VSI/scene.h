#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPaintEvent>
#include <QList>
#include <QLine>
#include "configure.h"
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

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(QObject *parent = 0);
    ~Scene();

    void setName(QString name);  // 设置图层名称
    QString getName();  // 获取图层名称

    void setCurShape(Shape::ShapeType curShape);  // 设置当前图形
    Shape::ShapeType getCurShape();  // 获取当前图形

    int getitemListLength() const;  // 获取item列表长度

    void setModified(bool modified);  // 设置画布是否被更改
    bool isModified() const;  // 返回画布内容是否被更改过

    void setMoveable(bool moveable);  // 设置图元是否可移动
    bool isMoveable() const;  // 返回图元是否可移动

    void setDrawable(bool flag);  // 设置开始标识

    void setEntityStyle(Configure::EntityStyle eStyle);  // 设置实体样式
    Configure::EntityStyle getEntityStyle();  // 获取实体样式

    void setAxesGrid(Configure::AxesGrid axesGrid);  // 设置网格坐标轴

    // 添加自定义图形元素
    void addCustomPointItem(Point *point);  // 添加自定义点
    void addCustomLineItem(Line *line);  // 添加自定义直线
    void addCustomPolylineItem(Polyline *polyline);  // 添加自定义直线
    void addCustomRectItem(Rect *rect);  // 添加自定义矩形
    void addCustomEllipseItem(Ellipse *ellipse);  // 添加自定义椭圆
    void addCustomCircleItem(Circle *circle);  // 添加自定义圆形
    void addCustomArcItem(Arc *arc);  // 添加自定义圆弧

    //绘制正多边形传递参数
    int getPolygon_type() const;
    void setPolygon_type(int value);
    int getPolygon_line_num() const;
    void setPolygon_line_num(int value);
    double getPolygon_radius() const;
    void setPolygon_radius(double value);
    qreal getPolygon_alpha() const;
    void setPolygon_alpha(const qreal &value);

    //绘制梯边形的传递参数
    double getTrapezium_H() const;
    void setTrapezium_H(double value);
    double getTrapezium_toplength() const;
    void setTrapezium_toplength(double value);
    double getTrapezium_alpha1() const;
    void setTrapezium_alpha1(double value);
    double getTrapezium_alpha2() const;
    void setTrapezium_alpha2(double value);
    int getTrapezium_type() const;
    void setTrapezium_type(int value);

    Eyelet *getEyeletDialog() const;
    void setEyeletDialog(Eyelet *value);

    Text *getTextdialog() const;
    void setTextdialog(Text *value);

protected:
    int polygon_type=1;  //绘制正多边形传递的线类型
    int polygon_line_num=4;//绘制正多边形传递的边数
    double polygon_radius=100;//绘制正多边形传递的半径
    qreal polygon_alpha=0;  // 绘制正多边形传递的旋转角度

    double trapezium_H=100;//梯形的高
    double trapezium_toplength=100;//梯形的上底
    double trapezium_alpha1=60;//梯形的角度1
    double trapezium_alpha2=60;//梯形的角度2
    int trapezium_type;//梯形的类型

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
    bool editable;  // 表示现在可更改图元
    bool drawing;  // 是否在绘制特殊图形

    qreal penWidth;  // 画笔
    qreal scaleFactor;  // 缩放因子

    // 配置文件
    Configure::EntityStyle eStyle;  // 实体类型
    Configure::AxesGrid axesGrid;  // 坐标网格

    Eyelet *eyeletDialog ;//绘制鸡眼孔
    Text *textdialog;//文本的对话框传递

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

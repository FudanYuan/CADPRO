#ifndef LAYER_H
#define LAYER_H

#include <QWidget>
#include "mygraphicsview.h"
#include "mygraphicsscene.h"
#include "mygraphicsitem.h"
#include "mygraphicsitem.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

class Layer : public QWidget
{
    Q_OBJECT
public:
    explicit Layer(QWidget *parent = 0);
    ~Layer();

    MyGraphicsScene *getScene();  // 获取场景

    void setName(QString name);  // 设置图层名称
    QString getName();  // 获取图层名称

    void setBackColor(QColor backColor);  // 设置图层背景颜色
    QColor getBackColor();  // 获取图层背景颜色

    void setPenColor(QColor penColor);  // 设置画笔颜色
    QColor getPenColor();  // 获取画笔颜色

    void setPenStyle(Qt::PenStyle penStyle);  // 设置画笔风格
    Qt::PenStyle getPenStyle();  // 获取画笔风格

    void setPenWidth(int penWidth);  // 设置画笔宽度
    int getPenWidth();  // 获取画笔宽度

    void setBrushColor(QColor brushColor);  // 设置填充颜色
    QColor getBrushColor();  // 获取填充颜色

    void setCurShape(MyGraphicsItem::ShapeType curShape);  // 设置当前图形
    MyGraphicsItem::ShapeType getCurShape();  // 获取当前图形

    void setModified(bool modified);  // 设置画布是否被更改
    bool isModified() const;  // 返回画布内容是否被更改过

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QColor transformIntToQColor(const int &intColor);

protected:
    // 键盘事件（上/下/左/右键向各个方向移动、加/减键进行缩放、空格/回车键旋转）
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    // 鼠标事件（移动、按压、释放）
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    // 滚轮事件（放大/缩小）
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

private:
    MyGraphicsScene scene;  // 场景
    MyGraphicsScene seneceTemp;  // 临时绘图场景
    MyGraphicsItem *itemSelected;  // 设置选中图形

    QString name;   // 图层名称
    QColor backColor; // 背景色
    QColor penColor;   // 画笔颜色
    Qt::PenStyle penStyle; // 画笔风格
    int penWidth;   // 画笔宽度
    QColor brushColor; // 填充颜色

    QPoint sPoint, ePoint;  // 鼠标起止点

    MyGraphicsItem::ShapeType curShape;  // 当前图形
    bool isDrawing; // 是否在绘制特殊图形
    bool modified;  // 标志图层是否被更改过

signals:

public slots:

};

#endif // LAYER_H

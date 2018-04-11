#ifndef ITEMPROPERTIES_H
#define ITEMPROPERTIES_H

#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QDialog>
#include <QPen>
#include "shape.h"
#include "configure.h"
#include "customwidget.h"

class ItemProperties : public QDialog
{
    Q_OBJECT

public:
    ItemProperties();
    void initdialog();
    void initLineItemproperties();
    void initArcItemproperties();
    void initPolygonItemproperties();
    void initCircleItemproperties();

    int getShapeid() const;
    void setShapeid(int value);

    double getLength() const;
    void setLength(double value);

    bool getOk() const;
    void setOk(bool value);

    QPen getPen() const;
    void setPen(const QPen &value);

    Shape::ShapeType getCurShape() const;
    void setCurShape(const Shape::ShapeType &value);

    Configure::PenStyle getPenstyle() const;
    void setPenstyle(const Configure::PenStyle &value);

    QString getShapetype() const;
    void setShapetype(const QString &value);

    bool getIsinsertoffset() const;
    void setIsinsertoffset(bool value);

    double getOffset();
    int getPolygonEdge() const;
    void setPolygonEdge(int value);

    double getPolygonEdgeLength() const;
    void setPolygonEdgeLength(double value);

    double getPolygonRad() const;
    void setPolygonRad(double value);

private:
    QVBoxLayout *mainLayout;
    QTableWidget *tableWidget;//表格
    QTableWidgetItem *tableitem;//表格中的某格
    QComboBox *linestyle;//线类型
    QComboBox *linecolor;//线颜色
    QComboBox *lineTypeChoose;//线属性
    Configure::PenStyle penstyle;
    Configure::EntityStyle eStyle;  // 实体类型
    Shape::ShapeType curShape;  // 当前图形

    QPen pen;
    int rows;//属性中的行数
    bool ok;

    //通用的属性值
    int shapeid;
    QString shapetype;//图形类型
    bool isinsertoffset;//是否偏移
    double offset;//偏移量

    //线的属性
    double length;//线段长度

    //正多边形的属性
    int polygonEdge;//边数
    double polygonEdgeLength;//边长
    double polygonRad;//旋转度数

signals:
    void PropertiesChanged();

public slots:
    void typechanged();
    void linestylechange(QString name, int index);//线类型
    void linecolorchange(QString key, QColor color);//线颜色
    void linetypechange();//线属性
    void polygonEdgeChange(QString key);//正多边形边数改变
    void polygonEdgeLengthChange(QString key);//正多边形半径改变
    void polygonRadChange(QString key);//正多边形旋转度数改变
    //偏移
    void insertoffset(bool isoffset);
    void setOffset(QString offset);
};

#endif // ITEMPROPERTIES_H

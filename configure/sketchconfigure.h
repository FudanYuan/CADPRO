﻿#ifndef SKETCHCONFIGURE_H
#define SKETCHCONFIGURE_H

#include <QDialog>
#include <QPushButton>
#include <QTableWidget>
#include <QDialogButtonBox>
#include <QLayout>
#include <QComboBox>
#include <QColor>
#include <QSettings>
#include <QList>
#include <debug.h>

#define SKETCH_CONFG_FILE_PATH "sketch_config.ini"

// 配置类，此配置类用于整个软件的配置
class SketchConfigure : public QObject
{
    Q_OBJECT
public:
    // 内线类型
    enum InterLineStyle{mark,cut,stitch,generic};

    // 轴类型
    enum AxesType{small, large};

    // 网格类型
    enum GridType{rectangular, across, node};

    // 语言
    enum Language{Chinese, English, Spanish, Italian, French, Russian, Portuguese, Deutsch, Ukrainian, Japanese};
    // 【pyccknn】【俄语】
    // 【ykpaihcbka】【乌克兰】
    // 【espanol】【西班牙语】
    // 【italiano】【意大利语】
    // 【deutsch】【德语】
    // 【polski】【波兰】
    // 【francais】【法语】
    // 【cestina】【捷克语】
    // 【portugues】【葡萄牙语】

    // 画笔属性，凡是配置项目中关于画笔的都可使用此结构体
    struct PenStyle
    {
        // 此方法类似于类的结构体
        PenStyle() :
            color(Qt::red), style(Qt::SolidLine), width(1), brush(Qt::white)
        {

        }

        PenStyle(QColor c, Qt::PenStyle s, qreal w, QColor b=Qt::white) :
            color(c), style(s), width(w), brush(b)
        {

        }

        QColor color;  // 实体颜色, 默认为黑色
        Qt::PenStyle style;  // 实体样式，默认为实线
        qreal width;  // 实体宽度，默认为1
        QColor brush;  // 刷子颜色

        // 设置画笔属性
        void setPenStyle(QColor color, Qt::PenStyle style, double width, QColor brush = Qt::white)
        {
            this->color = color;
            this->style = style;
            this->width = width;
            this->brush = brush;
        }
    };

    //对象大小
    struct ObjSize
    {
        ObjSize() :
            knotSizeInPix(8),
            snapDistanceInPix(5)
        {

        }

        int knotSizeInPix;  // 节点大小 默认为8像素

        int snapDistanceInPix; // 捕捉距离 默认为5像素

        void setObjSize(int knotSizeInPix, int snapDistanceInPix)
        {
            this->knotSizeInPix = knotSizeInPix;
            this->snapDistanceInPix = snapDistanceInPix;
        }
    };

    //参考点
    struct ReferPoint
    {
        ReferPoint() :
            color(Qt::red),sizeInPix(20),textHeightInPix(10)
        {

        }

        QColor color; // 参考点颜色 默认为红

        int sizeInPix; // 参考点大小 默认为20像素

        int textHeightInPix; // 文字高度 默认为10像素

        void setReferPoint(QColor color,int sizeInPix,int textHeightInPix)
        {
            this->color = color;
            this->sizeInPix = sizeInPix;
            this->textHeightInPix = textHeightInPix;
        }
    };

    // 实体类型
    struct EntityStyle{
        EntityStyle() :
            backgroundColor(Qt::white),
            selectionBox(Qt::black),
            hiddenEntity(Qt::gray)
        {

        }
        // 实体
        PenStyle perimeterLine; // 周长线
        PenStyle mark;  // 标记
        PenStyle cut;  // 切割
        PenStyle stitch;  // 缝线
        PenStyle generic; // 通用
        PenStyle middleAxis; // 中心轴
        QColor notchColor; // 凹槽

        // 更多颜色
        QColor backgroundColor = Qt::white; // 背景颜色
        PenStyle selectedEntity; // 所选实体
        QColor selectionBox; // 选择框
        PenStyle entityUnderCursor; // 光标下的实体
        QColor hiddenEntity; // 隐藏实体

        ObjSize objSize; // 对象大小
        ReferPoint referPoint; // 参考点

        InterLineStyle interLineStyle;  // 内线类型

        bool lineTypes;  // 是否使用合适的曲线

        QColor projectHistory; // 项目历史
        QColor basicSize; // 基本尺码
    };

    //轴
    struct Axes
    {
        Axes() :
            showAxes(false),
            axesType(SketchConfigure::small),
            xAxisColor(Qt::blue),
            yAxisColor(Qt::red),
            arrowSizeInPix(10),
            axisSizeInPix(10)
        {

        }

        bool showAxes; // 是否显示轴
        AxesType axesType;  // 轴类型
        QColor xAxisColor; // x轴颜色
        QColor yAxisColor; // y轴颜色
        int arrowSizeInPix; // 箭头大小
        int axisSizeInPix; // 轴大小

        void setAxes(AxesType axesType, QColor xAxisColor, QColor yAxisColor, int arrowSizeInPix, int axisSizeInPix){
            this->axesType = axesType;
            this->xAxisColor = xAxisColor;
            this->yAxisColor = yAxisColor;
            this->arrowSizeInPix = arrowSizeInPix;
            this->axisSizeInPix = axisSizeInPix;
        }
    };

    //网格
    struct Grid
    {
        Grid() :
            showGrid(false),
            gridType(SketchConfigure::rectangular),
            gridColor(Qt::gray),
            xStep(100),
            yStep(100)
        {

        }
        bool showGrid; // 是否显示网格
        GridType gridType;  // 网格类型
        QColor gridColor; // 网格颜色
        double xStep; // x步长
        double yStep; // y步长

        void setGrid(GridType gridType, QColor gridColor, int xStep, int yStep){
            this->gridType = gridType;
            this->gridColor = gridColor;
            this->xStep = xStep;
            this->yStep = yStep;
        }
    };

    // 轴和网格
    struct AxesGrid{
        Axes axes;  // 轴
        Grid grid;  // 网格

        bool antiAliaising;  //  抗锯齿
    };

    //偏移配置
    struct Offset
    {
        Offset() :
            type(""),
            distance(0),
            color(Qt::black)
        {

        }
        QString type; // 偏移类型
        qreal distance; // 偏移距离
        QColor color; // 颜色

        void setOffset(QString type, qreal distance, QColor color){
            this->type = type;
            this->distance = distance;
            this->color = color;
        }
    };

    // 界面显示
    struct WorkView
    {
        WorkView() :
            gradingRules(false),
            filledPatterns(false),
            toolFindStyle(false),
            toolProject(true),
            toolProperties(true)
        {

        }
        bool gradingRules;
        bool filledPatterns;
        bool toolFindStyle;
        bool toolProject;
        bool toolProperties;
    };

    // 配置键值对
    struct KeyValue
    {
        KeyValue(QString key, QVariant value) {
            this->key = key;
            this->value = value;
        }

        QString key;
        QVariant value;

        void setValue(const QVariant &v){
            this->value = v;
        }
    };

    explicit SketchConfigure(QObject *parent=0);
    ~SketchConfigure();

    EntityStyle eStyle;  // 实体样式
    AxesGrid axesGrid;  // 轴和网格
    QList<Offset> offset;  // 偏移
    Language language;  // 语言
    WorkView view;  // 视图

    // 配置文件
    QSettings *settings;
    void writeConfig(QSettings *settings);
    void readConfig(QSettings *settings);
    void writeConfigEntityStyle(QSettings *settings);
    void readConfigEntityStyle(QSettings *settings);
    void writeConfigAxesGrid(QSettings *settings);
    void readConfigAxesGrid(QSettings *settings);
    void writeConfigOffset(QSettings *settings);
    void readConfigOffset(QSettings *settings);
    void writeConfigLanguage(QSettings *settings);
    void readConfigLanguage(QSettings *settings);
    void writeConfigView(QSettings *settings);
    void readConfigView(QSettings *settings);

    static void updateConfig(QList<KeyValue> keyValue);

signals:
    void axesChanged(bool show);
    void gridChanged(bool show);

public slots:
    void onConfigChanged(QString name, QVariant value);  // 响应配置变化
};

#endif // SKETCHCONFIGURE_H

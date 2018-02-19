#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <QWidget>
#include <QColor>
#include <QSettings>
#include <QList>

#define CONFG_FILE_PATH "config.ini"

//内线类型
enum InterLineStyle{none,mark,cut,stitch,generic};

//轴类型
enum AxesType{small,};

//网格类型
enum GridType{square};


// 画笔属性，凡是配置项目中关于画笔的都可使用此结构体
struct PenStyle
{
    // 此方法类似于类的结构体
    PenStyle() :
        color(Qt::black), style(Qt::SolidLine), width(1)
    {

    }

    QColor color;  // 实体颜色, 默认为黑色
    Qt::PenStyle style;  // 实体样式，默认为实线
    int width;  // 实体宽度，默认为1

    // 设置画笔属性
    void setPenStyle(QColor color, Qt::PenStyle style, int width)
    {
        this->color = color;
        this->style = style;
        this->width = width;
    }
};

//对象大小
struct ObjSizeStyle
{
    ObjSizeStyle() :
        knotSizeInPix(8),snapDistanceInPix(5)
    {

    }

    int knotSizeInPix;  // 节点大小 默认为8像素

    int snapDistanceInPix; // 捕捉距离 默认为5像素

    void setObjSizeStyle(int knotSizeInPix, int snapDistanceInPix)
    {
        this->knotSizeInPix = knotSizeInPix;
        this->snapDistanceInPix = snapDistanceInPix;
    }

};

//参考点
struct ReferPointStyle
{
    ReferPointStyle() :
        color(Qt::red),sizeInPix(20),textHeightInPix(10)
    {

    }

    QColor color; // 参考点颜色 默认为红

    int sizeInPix; // 参考点大小 默认为20像素

    int textHeightInPix; // 文字高度 默认为10像素

    void setReferPointStyle(QColor color,int sizeInPix,int textHeightInPix)
    {
        this->color = color;
        this->sizeInPix = sizeInPix;
        this->textHeightInPix = textHeightInPix;
    }
};


//实体
struct EntityStyle
{
    // 变量名称 原则为：栏目英文，并采用驼峰命名方法。
    // 添加注释 原则为： 栏目中文
    // 如 周长线（Perimeter Line）一项的变量为：
    // penStyle perimeterLine;  // 周长线

    PenStyle perimeterLine; // 周长线
    PenStyle mark;  // 标记
    PenStyle cut;  // 切割
    PenStyle stitch;  // 缝线
    PenStyle generic; // 通用
    PenStyle middleAxis; // 中心轴
    PenStyle notch; // 凹槽
    ObjSizeStyle objSize; // 对象大小
    ReferPointStyle referPoint; //参考点
    PenStyle selectedEntity; // 所选实体
    QColor backgroundColor; // 背景颜色
    QColor selectionBox; // 选择框
    PenStyle entityUnderCursor; //  光标下的实体
    QColor hiddenEntity; // 隐藏实体
    QColor projectHistory; // 项目历史
    QColor basicSize; // 基本尺码
    bool lineTypes;  // 是否使用合适的曲线
};

//轴和网格
struct AxesAndGrid
{
    bool showAxes; // 是否显示轴
    QColor xAxisColor; // x轴颜色
    QColor yAxisColor; // y轴颜色
    int arrowSizeInPix; // 箭头大小
    int axisSizeInPix; // 轴大小
    bool showGrid; // 是否显示网格
    QColor gridColor; // 网格颜色
    double xStep; // x步长
    double yStep; // y步长
    bool antiAliaising; // 是否抗锯齿
};

//偏移配置
struct Offset
{
    QString type; // 偏移种类
    
    qreal distance; // 偏移距离

    QColor color; // 颜色
    
};

// 配置类，此配置类用于整个软件的配置
class Configure : public QWidget
{
    Q_OBJECT
public:

    explicit Configure(QWidget *parent = 0);

    // 成员变量为需要配置的项目，如实体样式、轴和网格、偏移配置等等，
    // 其实就是将上面定义的结构体放到这里，
    // 以便在软件初始化运行时对需要配置的项目进行初始化操作
    EntityStyle eStyle;
    AxesAndGrid axesGrid;
    QList<Offset> offset;

    void writeConfigEStyle();
    void readConfigEStyle();
    void writeConfigAxesGrid();
    void readConfigAxesGrid();
    void writeConfigOffset();
    void readConfigOffset();
    // .. 将其他成员添加完整
signals:

public slots:
};

#endif // CONFIGURE_H

#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QMouseEvent>
#include <QImage>

class PaintArea : public QWidget
{
    Q_OBJECT
public:
    //枚举变量，几个图形的选择
    enum ShapeType {None,Point,Line,Rectangle,Ellipse};
    enum Points {First,Second,Third};

    explicit PaintArea(QWidget *parent = 0);
    void initPaintArea();
    bool isModified() const { return modified; }    // 判断画布内容是否被更改过
    void setModified(bool modified);                // 设置画布是否更改
    QString getName();                              // 获取画布名称
    void setName(QString name);                     // 设置画布名称
    QImage getImage();                              // 获取iamge对象
    bool saveImage(const QString &fileName, const char *fileFormat); // 保存图片
    bool openImage(const QString &fileName);        // 打开图片
    void setImageSize(int width, int height);       // 设置图像大小
    void setImageColor(QColor color);               // 设置图像背景颜色
    QSize getImageSize();                           // 获取图片大小
    void setShape(ShapeType shape);                 //设置要绘制的图形
    void setPenStyle(Qt::PenStyle style); //设置画笔风格
    void setPenWidth(int width); //设置画笔宽度
    void setPenColor(QColor color); //设置画笔颜色
    void setBrushColor(QColor color); //设置填充颜色
    void setPoint(Points point, int x, int y, int z); // 设置点的坐标
    QColor IntToQColor(const int &intColor);
public slots:
    void print(); // 打印
    void zoomIn(); //放大
    void zoomOut(); //缩小
    void zoomBack(); //还原
    void doRotate(int in_angle, int direction); //旋转
    void doShear(); //拉伸
    void doClear(); //清空
    void drawGrid(QPainter *painter); // 画网格
    void paint();
    void paint(QImage &image);

protected:
    void paintEvent(QPaintEvent *); // 重绘事件
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QString name;   // 画布名称
    bool modified;  // 标志画布是否被更改过

    QImage image;   // QImage 类对象，用于在其上绘图
    QRgb backColor; // QRgb 颜色对象，存储 image 的背景色
    QPoint firstPoint, secondPoint, thirdPoint; // 定义两个坐标对象存放鼠标指针的前后两个坐标
    qreal scale;    // 缩放量
    int angle;      // 角度
    qreal shear;    // 拉伸量

    ShapeType curShape; //当前图形

    QImage tempImage; //临时绘图区
    bool isDrawing; //是否在绘制特殊图形

    QColor penColor; //画笔颜色
    QColor brushColor; //填充颜色
    int penWidth; //画笔宽度
    Qt::PenStyle penStyle; //画笔风格

signals:

public slots:
};

#endif // PAINTAREA_H

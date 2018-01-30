#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QMouseEvent>
#include <QImage>

class PaintArea : public QWidget
{
    Q_OBJECT
public:
    explicit PaintArea(QWidget *parent = 0);
    bool isModified() const { return modified; } // 判断画布内容是否被更改过
    bool saveImage(const QString &fileName, const char *fileFormat); // 保存图片
    bool openImage(const QString &fileName); // 打开图片
    void setImageSize(int width, int height); // 设置图像大小
    void setImageColor(QColor color); // 设置图像背景颜色
    QSize getImageSize(); // 获取图片大小

    void print(); // 打印
    void zoomIn(); //放大
    void zoomOut(); //缩小
    void zoomBack(); //还原
    void doRotate(int in_angle, int direction); //旋转
    void doShear(); //拉伸
    void doClear(); //清空
    void drawGrid(QPainter *painter); // 画网格
protected:
    void paintEvent(QPaintEvent *); // 重绘事件
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paint(QImage &image);

private:
    QImage image; // QImage 类对象，用于在其上绘图
    QRgb backColor; // QRgb 颜色对象，存储 image 的背景色
    QPoint firstPoint, secondPoint, thirdPoint; // 定义两个坐标对象存放鼠标指针的前后两个坐标
    bool modified; //标志画布是否被更改过

    qreal scale; //缩放量
    int angle; //角度
    qreal shear; //拉伸量

signals:

public slots:
};

#endif // PAINTAREA_H

#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include <QKeyEvent>

class View : public QGraphicsView
{
    Q_OBJECT
public:
    View(QWidget *parent = 0);

    void setPosition(qreal x, qreal y);  // 设置鼠标坐标点
    void setPosition(QPointF pos);  // 设置鼠标坐标点
    QPoint getPosition();  // 获取鼠标坐标点

    void setZoomDelta(qreal zoomDelta);  // 设置缩放增量
    qreal getZoomDelta();  // 获取缩放增量

    void setIsTranslate(bool isTranslate);  // 设置平移标识
    bool getIsTranslate();  // 获取平移标识

    void setTranslateSpeed(qreal translateSpeed);  // 设置平移速度
    qreal getTranslateSpeed();  // 获取平移速度

    void setAngle(qreal angle);  // 设置旋转角度
    qreal getAngle();  // 获取旋转角度

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QDragLeaveEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

private:
    qreal xPos;  // x点坐标
    qreal yPos;  // y点坐标

    qreal windowScale;  // 缩放量
    qreal zoomDelta;  // 缩放的增量

    bool isTranslate;  // 平移标识
    qreal translateSpeed;  // 平移速度

    bool isRotate;  // 旋转标识
    qreal angle;      // 旋转角度

signals:
    void mousePositionChanged(QPointF pos);  // 鼠标坐标更改事件
    void viewScaleChanged(qreal scaleFactor);  // view缩放改变

public slots:
    void zoomIn();  // 放大
    void zoomOut();  // 缩小
    void zoomBack();  // 还原
    void zoom(qreal scaleFactor); // 缩放 - scaleFactor：缩放的比例因子
    void translate(QPointF delta);  // 平移
};


#endif // VIEW_H

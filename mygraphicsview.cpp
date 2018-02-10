#include "mygraphicsview.h"
#include <QDebug>

#define VIEW_CENTER viewport()->rect().center()
#define VIEW_WIDTH  viewport()->rect().width()
#define VIEW_HEIGHT viewport()->rect().height()

MyGraphicsView::MyGraphicsView(QWidget *parent)
    : QGraphicsView(parent),
      windowScale(1.0),
      zoomDelta(0.1),
      isTranslate(false),
      translateSpeed(1.0)
{
}

void MyGraphicsView::setZoomDelta(qreal zoomDelta)
{
    this->zoomDelta = zoomDelta;
}

qreal MyGraphicsView::getZoomDelta()
{
    return this->zoomDelta;
}

void MyGraphicsView::setIsTranslate(bool isTranslate)
{
    this->isTranslate = isTranslate;
}

bool MyGraphicsView::getIsTranslate()
{
    return this->isTranslate;
}

void MyGraphicsView::setTranslateSpeed(qreal translateSpeed)
{
    this->translateSpeed = translateSpeed;
}

qreal MyGraphicsView::getTranslateSpeed()
{
    return this->translateSpeed;
}

void MyGraphicsView::setAngle(qreal angle)
{
    this->angle = angle;
}

qreal MyGraphicsView::getAngle()
{
    return this->angle;
}

void MyGraphicsView::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "MyGraphicsView::keyPressEvent";
    switch (event->key()) {
    case Qt::Key_Up:
        translate(QPointF(0, -2));  // 上移
        break;
    case Qt::Key_Down:
        translate(QPointF(0, 2));  // 下移
        break;
    case Qt::Key_Left:
        translate(QPointF(-2, 0));  // 左移
        break;
    case Qt::Key_Right:
        translate(QPointF(2, 0));  // 右移
        break;
    case Qt::Key_Plus:  // 放大
        qDebug() << "in";
        zoomIn();
        break;
    case Qt::Key_Minus:  // 缩小
        qDebug() << "out";
        zoomOut();
        break;
    case Qt::Key_Space:  // 逆时针旋转
        rotate(-5);
        break;
    case Qt::Key_Enter:  // 顺时针旋转
    case Qt::Key_Return:
        rotate(5);
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }

}

void MyGraphicsView::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "MyGraphicsView::mousePressEvent";
    QGraphicsView::mousePressEvent(event);
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "MyGraphicsView::mouseMoveEvent";
    QGraphicsView::mouseMoveEvent(event);
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "MyGraphicsView::mouseReleaseEvent";
    QGraphicsView::mouseReleaseEvent(event);
}

void MyGraphicsView::paintEvent(QPaintEvent *event)
{
    qDebug() << "MyGraphicsView::paintEvent";
    QGraphicsView::paintEvent(event);
}

void MyGraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "MyGraphicsView::dragEnterEvent";
    QGraphicsView::dragEnterEvent(event);
}

void MyGraphicsView::dragLeaveEvent(QDragLeaveEvent *event)
{
    qDebug() << "MyGraphicsView::dragLeaveEvent";
    QGraphicsView::dragLeaveEvent(event);
}

void MyGraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug() << "MyGraphicsView::dragMoveEvent";
    QGraphicsView::dragMoveEvent(event);

}

void MyGraphicsView::dropEvent(QDropEvent *event)
{
    qDebug() << "MyGraphicsView::dropEvent";
    QGraphicsView::dropEvent(event);
}

void MyGraphicsView::wheelEvent(QWheelEvent *event)
{
    // 滚轮的滚动量
    QPoint scrollAmount = event->angleDelta();
    // 正值表示滚轮远离使用者（放大），负值表示朝向使用者（缩小）
    scrollAmount.y() > 0 ? zoomIn() : zoomOut();
}

void MyGraphicsView::zoomIn()
{
    this->zoom(1 + this->zoomDelta);
}

void MyGraphicsView::zoomOut()
{
    this->zoom(1 - this->zoomDelta);
}

void MyGraphicsView::zoomBack()
{
    scale(1, 1);
    this->windowScale = 1;
}

void MyGraphicsView::zoom(qreal scaleFactor)
{
    qDebug() << scaleFactor;
    // 防止过小或过大
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    qDebug() << factor;
    if (factor < 0.07 || factor > 100)
        return;
    scale(scaleFactor, scaleFactor);
    this->windowScale *= scaleFactor;
}

void MyGraphicsView::translate(QPointF delta)
{
    // 根据当前 zoom 缩放平移数
    delta *= this->windowScale;
    delta *= this->translateSpeed;
    qDebug() << "delta" << delta;
    // view 根据鼠标下的点作为锚点来定位 scene
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    QPoint newCenter(VIEW_WIDTH / 2 - delta.x(),  VIEW_HEIGHT / 2 - delta.y());
    centerOn(mapToScene(newCenter));

    // scene 在 view 的中心点作为锚点
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}



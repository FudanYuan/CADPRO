#include "view.h"
#include "scene.h"
#include <QDebug>

#define VIEW_CENTER viewport()->rect().center()
#define VIEW_WIDTH  viewport()->rect().width()
#define VIEW_HEIGHT viewport()->rect().height()

View::View(QWidget *parent)
    : QGraphicsView(parent),
      mouseFlag(true),
      wheelFlag(true),
      keyboardFlag(true),
      xPos(0),
      yPos(0),
      anchor(QPoint()),
      windowScale(1.0),
      zoomDelta(0.1),
      isTranslate(false),
      translateSpeed(10)
{
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setMouseTracking(true);

    // 去掉滚动条
    //setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::Antialiasing);

    // 设置视图中央
    centerOn(mapToScene(0, 0));
#if 0
    // 视图坐标原点(0,0)对应场景坐标（场景坐标）
    qDebug() << "mapToScene(0, 0):" << mapToScene(0, 0);

    // 场景坐标原点(0,0)对应视图坐标（视图坐标）
    qDebug() << "mapFromScene(0, 0):" << mapFromScene(0, 0);
    // 场景左上角坐标（场景坐标）
    QPointF p1 = QPointF(sceneRect().topLeft());
    qDebug() << "p1:" << p1;
    // 场景左上角对应视图坐标（视图坐标）
    qDebug() << "mapFromScene(p1.x(), p1.y())" << mapFromScene(p1.x(), p1.y());
    qDebug() << "view " << size().width() << "  " << size().height();
#endif
}

View::~View()
{
#ifdef DEBUG
    qDebug() << "view has been deleted!";
#endif
}

void View::setMouseFlag(bool flag)
{
    this->mouseFlag = flag;
}

bool View::getMouseFlag()
{
    return this->mouseFlag;
}

void View::setWheelFlag(bool flag)
{
    this->wheelFlag = flag;
}

bool View::getWheelFlag()
{
    return this->wheelFlag;
}

void View::setKeyboardFlag(bool flag)
{
    this->keyboardFlag = flag;
}

bool View::getKeyboardFlag()
{
    return this->keyboardFlag;
}

void View::setPosition(qreal x, qreal y)
{
    xPos = x;
    yPos = -y;
}

void View::setPosition(QPointF pos)
{
    xPos = pos.x();
    yPos = -pos.y();
}

QPoint View::getPosition()
{
    return QPoint(xPos, yPos);
}

void View::setZoomDelta(qreal zoomDelta)
{
    this->zoomDelta = zoomDelta;
}

qreal View::getZoomDelta()
{
    return this->zoomDelta;
}

void View::setIsTranslate(bool isTranslate)
{
    this->isTranslate = isTranslate;
}

bool View::getIsTranslate()
{
    return this->isTranslate;
}

void View::setTranslateSpeed(qreal translateSpeed)
{
    this->translateSpeed = translateSpeed;
}

qreal View::getTranslateSpeed()
{
    return this->translateSpeed;
}

void View::setAngle(qreal angle)
{
    this->angle = angle;
}

qreal View::getAngle()
{
    return this->angle;
}

QPointF View::customFitInView(const QRectF &rect, qreal rate)
{
    // 材料居中显示
    this->zoomBack();  // 视图还原

    // 获取视图大小并设置图层矩形
    qreal viewWidth = width();
    qreal viewHeight = height();
    this->setSceneRect(0, 0, viewWidth, viewHeight);

    // 获取兴趣区域大小
    qreal rectWidth = rect.width();
    qreal rectHeight = rect.height();

    // 缩放比例，长宽缩放比例不等时选择缩放比例较小的进行缩放，以确保兴趣区域全部显示
    qreal widthFactor = viewWidth / rectWidth;
    qreal heightFactor = viewHeight / rectHeight;
    qreal factor = widthFactor;
    bool flag = false;
    if(factor > heightFactor){
        factor = heightFactor;
        flag = true;
    }
    // 如果factor>1代表图层比视图小
    if(factor > 1){
        qreal newViewWidth = flag ? viewWidth / factor : rectWidth;
        qreal newViewHeight = flag ? rectHeight : viewHeight / factor;
        this->setSceneRect(0, 0, newViewWidth, newViewHeight);
    }
    this->zoom(factor * rate);  // 进行缩放，以便中心显示
    QPointF bottomRightPos = mapToScene(QPoint(viewWidth, viewHeight));  // 获取图层右下角坐标
    QPointF offset = QPointF(0, qAbs(bottomRightPos.ry() - rectHeight) / 2) +
            QPointF(qAbs(bottomRightPos.rx() - rectWidth) / 2, 0);  // 计算偏移量
    return offset;  // 返回偏移量
}

void View::keyPressEvent(QKeyEvent *event)
{
    if(!keyboardFlag){
        event->ignore();
        return;
    }
    switch (event->key()) {
    case Qt::Key_Up:
        translate_(QPointF(0, 2));  // 上移
        break;
    case Qt::Key_Down:
        translate_(QPointF(0, -2));  // 下移
        break;
    case Qt::Key_Left:
        translate_(QPointF(2, 0));  // 左移
        break;
    case Qt::Key_Right:
        translate_(QPointF(-2, 0));  // 右移
        break;
    case Qt::Key_Plus:  // 放大
        zoomIn();
        break;
    case Qt::Key_Minus:  // 缩小
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

void View::mousePressEvent(QMouseEvent *event)
{
    if(!mouseFlag){
        event->ignore();
        return;
    }
    QGraphicsView::mousePressEvent(event);
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    if(!mouseFlag){
        event->ignore();
        return;
    }
    QGraphicsView::mouseMoveEvent(event);
    setPosition(event->pos());
    emit mousePositionChanged(mapToScene(event->pos()));
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
    if(!mouseFlag){
        event->ignore();
        return;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void View::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
}

void View::dragEnterEvent(QDragEnterEvent *event)
{
    QGraphicsView::dragEnterEvent(event);
}

void View::dragLeaveEvent(QDragLeaveEvent *event)
{
    QGraphicsView::dragLeaveEvent(event);
}

void View::dragMoveEvent(QDragMoveEvent *event)
{
    QGraphicsView::dragMoveEvent(event);

}

void View::dropEvent(QDropEvent *event)
{
    QGraphicsView::dropEvent(event);
}

void View::wheelEvent(QWheelEvent *event)
{
    if(!wheelFlag){
        event->ignore();
        return;
    }
<<<<<<< HEAD
    // 以鼠标位置为中心
//    centerOn(event->posF());
    setResizeAnchor(QGraphicsView::AnchorUnderMouse); // AnchorUnderMouse
=======
    // 获取锚点
    anchor = event->pos();
>>>>>>> Jeremy
    // 滚轮的滚动量
    QPoint scrollAmount = event->angleDelta();
    // 正值表示滚轮远离使用者（放大），负值表示朝向使用者（缩小）
    scrollAmount.y() > 0 ? zoomIn() : zoomOut();
}

void View::zoomIn()
{
    this->zoom(1 + this->zoomDelta);
}

void View::zoomOut()
{
    this->zoom(1 - this->zoomDelta);
}

void View::zoomBack()
{
    this->zoom(1 / this->windowScale);
}

void View::zoom(qreal scaleFactor)
{
    // 记录锚点对应的图层坐标
//    QPointF anchorSceneBefore = mapToScene(anchor);
//    qDebug() << "变换前视图坐标" << anchor << ",锚点图层坐标：" << anchorSceneBefore;

//    QPoint viewCenter = viewport()->rect().center();
//    QPointF sceneCenter = mapToScene(viewCenter);
//    qDebug() << "变换前中心点：" << viewCenter << ", 对应图层：" << sceneCenter;
    // 防止过小或过大
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
<<<<<<< HEAD
    qDebug() << "factor " << factor;
    if (factor < 0.01 || factor > 50){
        return;
    }
    qDebug() << "scaleFactor " << scaleFactor;
    scale(scaleFactor, scaleFactor);
    this->windowScale *= scaleFactor;
=======

    if (factor < 0.01 || factor > 50){
        return;
    }
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    scale(scaleFactor, scaleFactor);
    this->windowScale *= scaleFactor;
//    qDebug() << "windowScale: " << windowScale;

//    QPointF anchorSceneNow = mapToScene(anchor);
//    qDebug() << "变换后锚点图层坐标：" << anchorSceneNow;

//    QPointF offset = anchorSceneNow - anchorSceneBefore;  // 偏移
//    qDebug() << "偏移量为：" << offset;

//    qDebug() << "变换后中心点：" << viewCenter
//             << ", 对应图层：" << mapToScene(viewCenter);
//    qDebug() << "原来点对应的view坐标：" << mapFromScene(sceneCenter);

//    centerOn(mapToScene(viewCenter)+offset);
//    qDebug() << "11变换后中心点：" << mapFromScene(mapToScene(viewCenter)+offset)
//             << ", 对应图层：" << mapToScene(viewCenter)+offset;

>>>>>>> Jeremy
    // 发送视图缩放改变的信号
    emit viewScaleChanged(this->windowScale);
    //emit viewOffsetChanged(offset);

    // 场景左上角对应视图坐标（视图坐标）
//    qDebug() << "view rect: " << sceneRect();
//    qDebug() << "view size: " << size().width() << "  " << size().height();
//    qDebug() << "view bottom right: " << sceneRect().bottomRight().toPoint();
//    qDebug() << "scene bottom right: " << mapToScene(sceneRect().bottomRight().toPoint());
}

void View::translate_(QPointF delta)
{
    // 根据当前 zoom 缩放平移数
    delta *= this->windowScale;
    delta *= this->translateSpeed;

    // view 根据鼠标下的点作为锚点来定位 scene
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
<<<<<<< HEAD
=======
    qDebug() << "中心：" << VIEW_CENTER << "，平移：" << VIEW_WIDTH / 2 - delta.x() << "  " << VIEW_HEIGHT / 2 - delta.y();
>>>>>>> Jeremy
    QPoint newCenter(VIEW_WIDTH / 2 - delta.x(),  VIEW_HEIGHT / 2 - delta.y());
    centerOn(mapToScene(newCenter));

    // scene 在 view 的中心点作为锚点
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}



#ifndef RECT_H
#define RECT_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include "shape.h"
#include <QPointF>

class Rect : public Shape, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Rect(QGraphicsItem *parent=0);
    void startDraw(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 开始绘图
    void drawing(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 绘图开始
    void setStyle(EntityStyle style) Q_DECL_OVERRIDE;  // 设置实体类型

protected:
    //鼠标事件
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    //拖拽事件
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;

private:
    QPointF topLeftPoint, bottomRightPoint;  // 顶点

public slots:
    void onSceneScaleChanged(qreal scaleFactor) Q_DECL_OVERRIDE;  // 响应view缩放事件
    void onSceneMoveableChanged(bool moveable) Q_DECL_OVERRIDE;  //  响应场景可移动性改变
};

#endif // RECT_H

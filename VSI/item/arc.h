#ifndef ARC_H
#define ARC_H

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include "shape.h"
#include <QPointF>

class Arc : public Shape, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    enum ArcType{
        normal,
        updated
    };
    Arc(QGraphicsItem *parent=0);
    void startDraw(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 开始绘图
    void drawing(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 绘图开始
    bool updateFlag(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE; // paint

    void setType(ArcType type);
    ArcType getType();
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
    QPointF cPoint;  // 圆心
    qreal r; // 半径
    qreal sAngle;  // 开始角度
    qreal eAngle;  // 结束角度
    qreal angleRange;  // 跨度
    QPointF fPoint, sPoint, tPoint; // 圆心, 第一点
    ArcType type;
    bool fFlag;  // 第一点确定
    bool sFlag;  // 第二点确定
    bool tFlag;  // 第三点确定
    bool eFlag;  // 出错，三点同直线，画直线
    QPointF getArcCenter(QPointF p1, QPointF p2, QPointF p3, qreal &r);
    bool isClockWise(QPointF p1, QPointF p2, QPointF p3);
    bool isClockWise(QPointF pc, QPointF p1, QPointF p2, QPointF p3);
    QLineF getLine(QPointF p1, QPointF p2, QPointF p0);  // 判断是否在线段内
    qreal getLineAngle(QPointF sPoint, QPointF ePoint);

signals:
    void select(Arc *arc);  // 图形被选择

public slots:
    void onSceneMoveableChanged(bool moveable) Q_DECL_OVERRIDE;  //  响应场景可移动性改变
};

#endif // ARC_H

#ifndef ARC_H
#define ARC_H

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include "shape.h"
#include <QPointF>
#include "itemproperties.h"

class Arc : public Shape, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    enum Type{
        normal,
        updated
    };
    Arc(QGraphicsItem *parent=0);
    void startDraw(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 开始绘图
    void drawing(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 绘图开始
    bool updateFlag(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE; // paint

    void setCPoint(QPointF p);  // 设置圆心
    QPointF getCPoint();  // 获取圆心

    void setRadius(qreal r);  // 设置半径
    qreal getRadius();  // 获取半径

    void setSAngle(qreal angle);  //设置开始角度
    qreal getSAngle();  // 获取开始角度

    void setEAngle(qreal angle);  //设置结束角度
    qreal getEAngle();  // 获取结束角度

    void setType(Type type);
    Type getType();  // 获取圆心点类型

    void setArc(qreal px, qreal py, qreal radius, qreal angle1, qreal angle2);  // 设置圆弧
    Arc arc();
    Arc *copy(); 
    ItemProperties *arcproperties;

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
    Type type;
    bool fFlag;  // 第一点确定
    bool sFlag;  // 第二点确定
    bool tFlag;  // 第三点确定
    bool eFlag;  // 出错，三点同直线，画直线
    QPointF getArcCenter(QPointF p1, QPointF p2, QPointF p3, qreal &r);
    bool isClockWise(QPointF p1, QPointF p2, QPointF p3);
    bool isClockWise(QPointF pc, QPointF p1, QPointF p2, QPointF p3);
    QLineF getLine(QPointF p1, QPointF p2, QPointF p0);  // 判断是否在线段内
    qreal getLineAngle(QPointF sPoint, QPointF ePoint);

    bool itemp;//属性状态改变

signals:
    void select(Arc *arc);  // 图形被选择

public slots:
    void onSceneMoveableChanged(bool moveable) Q_DECL_OVERRIDE;  //  响应场景可移动性改变
    void typechange();
};

#endif // ARC_H

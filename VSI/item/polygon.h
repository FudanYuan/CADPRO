#ifndef POLYGON_H
#define POLYGON_H

#include <QGraphicsPathItem>
#include <QGraphicsSceneMouseEvent>
#include "shape.h"
#include <QPointF>

class Polygon : public Shape, public QGraphicsPathItem
{
    Q_OBJECT
public:

    Polygon(QGraphicsItem *parent=0);
    void startDraw(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 开始绘图
    void drawing(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 绘图开始
    bool updateFlag(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE; // paint
<<<<<<< HEAD
    void toPolyline();//转变成polyline

    QList<QPointF> getPoints() const;
    void setPoints(const QList<QPointF> &value);
=======
    QVector<QPointF> toPolyline();//转变成polyline

    QVector<QPointF> getPoints() const;
    void setPoints(const QVector<QPointF> &value);
>>>>>>> Jeremy
    
	void setType(int type);
    int getType();

    int getLineNum() const;
    void setLineNum(int value);
    
	double getRadius() const;
    void setRadius(double value);
    
	qreal getAlpha() const;
    void setAlpha(const qreal &value);

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
    QPointF cPoint, sPoint;
<<<<<<< HEAD
    QList<QPointF> points;  // 各个点的坐标
=======
    QVector<QPointF> points;  // 各个点的坐标
>>>>>>> Jeremy

    qreal r; // 半径
    qreal alpha;  // 旋转角度

    int type;  //线类型
<<<<<<< HEAD
    int lineNum;//边的个数
    double radius;//半径
=======
    int lineNum;  //边的个数
    double radius;  //半径
>>>>>>> Jeremy

public slots:
    void on_commandLinkButton_2_clicked();
    void onSceneMoveableChanged(bool moveable) Q_DECL_OVERRIDE;  //  响应场景可移动性改变
};

#endif // POLYGON_H

#ifndef EYELET_H
#define EYELET_H

#include <QGraphicsPathItem>
#include <QGraphicsSceneMouseEvent>
#include "shape.h"
#include <QPointF>
#include "eyeletdialog.h"
#include "itemproperties.h"

class Eyelet : public Shape, public QGraphicsPathItem
{
    Q_OBJECT

public:

    Eyelet(QGraphicsItem *parent=0);
    void startDraw(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 开始绘图
    void drawing(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 绘图开始
    bool updateFlag(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE; // paint
    QList<QPointF> toPolyline();

    double getEyeletH() const;
    void setEyeletH(double value);

    double getEyeletW() const;
    void setEyeletW(double value);

    EyeletDialog *eyeletdialog;//鸡眼孔的对话框
    ItemProperties *eyeletproperties;

    double getEyeletAlpha() const;
    void setEyeletAlpha(double value);

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

    QPointF cPoint;

    double eyeletH ;
    double eyeletW ;
    double eyeletAlpha;

signals:
    void select(Eyelet *eyelet);//图形被选择

public slots:
    void on_commandLinkButton_2_clicked();
    void onSceneMoveableChanged(bool moveable) Q_DECL_OVERRIDE;  //  响应场景可移动性改变
    void typechange();
};

#endif // EYELET_H

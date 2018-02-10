#include "mygraphicsscene.h"
#include <QDebug>
#include <QPainter>

MyGraphicsScene::MyGraphicsScene(QObject *parent)
{

}

void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "MyGraphicsScene::mousePressEvent";
    QGraphicsScene::mousePressEvent(event);
}

void MyGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "MyGraphicsScene::mouseReleaseEvent";
    QGraphicsScene::mouseReleaseEvent(event);
}

void MyGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "MyGraphicsScene::mouseMoveEvent";
    QGraphicsScene::mouseMoveEvent(event);
    update();
}

void MyGraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    qDebug() << "MyGraphicsScene::mouseMoveEvent";
    QGraphicsScene::dragEnterEvent(event);
}

void MyGraphicsScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    qDebug() << "MyGraphicsScene::dragLeaveEvent";
    QGraphicsScene::dragLeaveEvent(event);
}

void MyGraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    qDebug() << "MyGraphicsScene::dragMoveEvent";
    QGraphicsScene::dragMoveEvent(event);
}

void MyGraphicsScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    qDebug() << "MyGraphicsScene::dropEvent";
    QGraphicsScene::dropEvent(event);
}

void MyGraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->save();
    painter->setBrush(Qt::darkCyan);
    painter->drawRect(rect);

    painter->setPen(Qt::yellow);
    const double w = sceneRect().width();
    const double h = sceneRect().height();
    QLineF line(QPointF(1,h),QPointF(w,h));
    painter->drawLine(line);
    QLineF line1(QPointF(1,h),QPointF(1,0));
    painter->drawLine(line1);
    for(int i=h; i>0; i-=100)
    {
        QLineF line(QPointF(1,i),QPointF(w,i));
        painter->drawLine(line);
    }
     for(int i=1;i<w;i+=100)
     {
         QLineF line(QPointF(i,0),QPointF(i,h));
         painter->drawLine(line);
     }
    painter->restore();
}

#include "text.h"
#include <QCursor>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <qmath.h>
#include <QDebug>
#include <QGraphicsTextItem>
#include <QFont>

Text::Text(QGraphicsItem *parent) :
    QGraphicsPathItem(parent)
{
    setShapeType(Shape::Polygon);
    // 设置图元为可焦点的
    setFlag(QGraphicsItem::ItemIsFocusable);
    // 设置图元为可移动的
    setFlag(QGraphicsItem::ItemIsMovable, false);
    // 设置图元为可接受拖拽事件
    setAcceptDrops(true);
    // 设置图元为可接受hover事件
    setAcceptHoverEvents(true);
    textdialog =new TextDialog();
}

void Text::startDraw(QGraphicsSceneMouseEvent *event)
{
    QPen pen = QPen();
    pen.setColor(penStyle.color);
    pen.setStyle(penStyle.style);
    pen.setWidthF(penStyle.width);
    setPen(pen);

    overFlag = true;
}

void Text::drawing(QGraphicsSceneMouseEvent *event)
{
    cPoint = event->scenePos();
    update();
}

bool Text::updateFlag(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    return overFlag;
}

void Text::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
        Q_UNUSED(option);
        Q_UNUSED(widget);
        scaleFactor =  painter->matrix().m11();
        // 获取到当前的线宽，这里的线宽其实还是之前设置的线宽值;
        // 比如我们之前设置线宽为 2 ，这里返回的线宽还是 2 ，但是当前的缩放比例变了；
        // 其实当前的线宽就相当于 penWidth * scaleFactor;
        // 所以如果我们想要让线宽保持不变，那就需要进行转换，即 penWidth = penWidth / scaleFactor;
        QPen pen = this->pen();
        // 重新设置画笔线宽;
        pen.setWidthF(pen.widthF() / scaleFactor);
        painter->setPen(pen);

        /*绘制文本的一种方法
        QFont *qfont =new QFont();
        qfont->setWeight(textWeight);//设置字体粗细0~100
        qfont->setPixelSize(textPixelSize);//设置字体大小
        qfont->setPointSize(textPointSize);//设置点大小
        qfont->setBold(textBold);//设置粗体
        qfont->setItalic(textItalic);//设置斜体
        qfont->setUnderline(textUnderline);//设置下划线
        painter->setFont(*qfont);
        painter->drawText(cPoint, textcontent);
        qDebug()<<"文本内容:"<<textcontent;
        qDebug()<<"字体大小:"<<textPixelSize;
        */
    //   painter->drawRoundedRect(QRect(cPoint.rx(),cPoint.ry()-50,50,50));

        //绘制文本的另一种方法
        int textsize = textPixelSize;
        QString text = textcontent;

        qDebug()<<"文本内容:"<<text;
        qDebug()<<"字体大小:"<<textsize;
        QPainterPath textPath;//字体
        QFont font("Times",textsize);
        font.setStyleStrategy(QFont::ForceOutline);//轮廓线
        textPath.addText(cPoint.rx(),cPoint.ry(),font,text);
        painter->drawPath(textPath);
        setPath(textPath);
}

void Text::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(selectable){
        selected = true;
        qDebug() << "type: " << getShapeType();
        qDebug() << "id: " << getShapeId();
        setCursor(Qt::ClosedHandCursor);
        QPen pen = QPen();
        pen.setColor(selectedEntity.color);
        pen.setStyle(selectedEntity.style);
        pen.setWidthF(selectedEntity.width);
        setPen(pen);
    }
    QGraphicsItem::mousePressEvent(event);
}

void Text::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "PolyLine::mouseMoveEvent";
    QGraphicsItem::mouseMoveEvent(event);
}

void Text::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug() << "PolyLine::mouseReleaseEvent";
    QGraphicsItem::mouseReleaseEvent(event);
}

void Text::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "PolyLine::dragEnterEvent";
    QGraphicsItem::dragEnterEvent(event);
}

void Text::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "PolyLine::dragLeaveEvent";
    QGraphicsItem::dragLeaveEvent(event);
}

void Text::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "PolyLine::dragMoveEvent";
    QGraphicsItem::dragMoveEvent(event);
}

void Text::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    // qDebug() << "PolyLine::dropEvent";
    QGraphicsItem::dropEvent(event);
}

void Text::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if(selectable){
        QPen pen = QPen();
        if(!selected){
            pen.setColor(underCursorStyle.color);
            pen.setStyle(underCursorStyle.style);
            pen.setWidthF(underCursorStyle.width);
        } else{
            pen.setColor(selectedEntity.color);
            pen.setStyle(selectedEntity.style);
            pen.setWidthF(selectedEntity.width);
        }
        setPen(pen);
        setCursor(Qt::OpenHandCursor);
        QGraphicsItem::hoverEnterEvent(event);
    }
}

void Text::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(selectable){
        setCursor(Qt::OpenHandCursor);
        QGraphicsItem::hoverMoveEvent(event);
    }
}

void Text::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if(selectable){
        QPen pen = QPen();
        if(!selected){
            pen.setColor(penStyle.color);
            pen.setStyle(penStyle.style);
            pen.setWidthF(penStyle.width);
        } else{
            pen.setColor(selectedEntity.color);
            pen.setStyle(selectedEntity.style);
            pen.setWidthF(selectedEntity.width);
        }
        setPen(pen);
        QGraphicsItem::hoverLeaveEvent(event);
    }
}

int Text::getTextPixelSize() const
{
    return textPixelSize;
}

void Text::setTextPixelSize(int value)
{
    textPixelSize = value;
}

QString Text::getTextcontent() const
{
    return textcontent;
}

void Text::setTextcontent(const QString &value)
{
    textcontent = value;
}

void Text::onSceneMoveableChanged(bool moveable)
{
    this->moveable = moveable;
    setFlag(QGraphicsItem::ItemIsMovable, moveable);
}

void Text::on_commandLinkButton_2_clicked()
{

}

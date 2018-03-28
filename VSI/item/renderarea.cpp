#include "renderarea.h"

#include <QPainter>
#include <QPaintEvent>
#include <qmath.h>

//! [0]
RenderArea::RenderArea(QWidget *parent,int value,int tra_dialog_type)
    : QWidget(parent)
{
    dialog_type = value;
    trapezium_dialog_type = tra_dialog_type;

    QFont newFont = font();
    newFont.setPixelSize(12);
    setFont(newFont);
}

RenderArea::RenderArea(QWidget *parent, int lin, double alp,int typ,int value)
    : QWidget(parent)
{
    line_num=lin;
    alpha=alp;
    type=typ;
    dialog_type = value;
    QFont newFont = font();
    newFont.setPixelSize(12);
    setFont(newFont);
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(182, 182);
}

QSize RenderArea::sizeHint() const
{
    return QSize(232, 232);
}

void RenderArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), QBrush(Qt::white));
    drawCoordinates(painter);
}

void RenderArea::drawpolygon(QPainter &painter)
{
    int num = line_num;
    double angle=alpha;
    int r=100;
//    qDebug()<<"边数"<<num;
//    qDebug()<<"角度"<<angle;
//    qDebug()<<"类型"<<type;
    /*
    int num = 4;
    double angle=0;
    */
    QPointF cpoint;cpoint.setX(120);cpoint.setY(120);//显示的中点
    QPainterPath path;

    QPointF sPoint(120+r*qCos(M_PI*angle/180), 120+r*qSin(M_PI*angle/180));
    path.moveTo(sPoint);
    for(int i=1;i < num;i++){
       QPointF p(120+r*qCos(2*M_PI/num * i+M_PI*angle/180), 120+r*qSin(2*M_PI/num * i+M_PI*angle/180));
       path.lineTo(p);
    }
    path.lineTo(sPoint);
    painter.drawPath(path);
    painter.drawPoint(120,120);

    painter.setPen(Qt::DashLine);
    if(type==1)
        painter.drawEllipse(cpoint,r,r);//外切圆
    if(type==2)
        painter.drawEllipse(cpoint,r*qSin(M_PI/2-M_PI/num),r*qSin(M_PI/2-M_PI/num));//内切圆
    return;
}

void RenderArea::drawtrapezium(QPainter &painter)
{
    int alpha1=60;//角度1
    int alpha2=60;//角度2
    int H=100;//高
    int top_length=100;//上底

    //画出整个梯形
    if(trapezium_dialog_type == 2){

        QPointF cpoint;cpoint.setX(100);cpoint.setY(160);//显示的中点
        QPainterPath path;

        path.moveTo(cpoint);
        QPointF sPoint(cpoint.rx()+top_length+H/(qTan(M_PI*alpha1/180)), cpoint.ry());
        path.lineTo(sPoint);
        sPoint.setX(cpoint.rx()+top_length);sPoint.setY(cpoint.ry()-H);
        path.lineTo(sPoint);
        sPoint.setX(cpoint.rx());sPoint.setY(cpoint.ry()-H);
        path.lineTo(sPoint);

        painter.drawPath(path);
        painter.setPen(Qt::DashLine);
        painter.drawPoint(cpoint);

        path.lineTo(cpoint);
        painter.drawPath(path);
        painter.setPen(Qt::DotLine);
    }
    //画出一半的梯形
    if(trapezium_dialog_type == 1){
        QPointF cpoint;cpoint.setX(150);cpoint.setY(160);//显示的中点
        QPainterPath path;

        path.moveTo(cpoint);
        QPointF sPoint(cpoint.rx()+top_length/2+H/(qTan(M_PI*alpha1/180)), cpoint.ry());
        path.lineTo(sPoint);
        sPoint.setX(cpoint.rx()+top_length/2);sPoint.setY(cpoint.ry()-H);
        path.lineTo(sPoint);
        sPoint.setX(cpoint.rx());sPoint.setY(cpoint.ry()-H);
        path.lineTo(sPoint);
        sPoint.setX(cpoint.rx());sPoint.setY(cpoint.ry()-H);
        path.lineTo(sPoint);
        sPoint.setX(cpoint.rx()-top_length/2);sPoint.setY(cpoint.ry()-H);
        path.lineTo(sPoint);
        sPoint.setX(cpoint.rx()-top_length/2-H/(qTan(M_PI*alpha2/180)));sPoint.setY(cpoint.ry());
        path.lineTo(sPoint);
        path.lineTo(cpoint);
        painter.drawPath(path);
        painter.drawPoint(cpoint);
        painter.setPen(Qt::DashLine);

        sPoint.setX(cpoint.rx());sPoint.setY(cpoint.ry()-H);
        path.lineTo(sPoint);
        painter.drawPath(path);
        painter.setPen(Qt::DotLine);
    }
}

void RenderArea::draweyelet(QPainter &painter)
{
    QPointF cPoint;cPoint.setX(80);cPoint.setY(160);//显示的中点

    int width = 150;//鸡眼孔宽度
    int height = 100;//鸡眼孔高度
    QPointF sPoint;
    QPainterPath path;

    sPoint.setX(cPoint.rx()+width);sPoint.setY(cPoint.ry());
    path.moveTo(cPoint);
    path.lineTo(sPoint);
    path.arcTo(cPoint.rx()+width-height/2,cPoint.ry()-height,height,height,-90,180);//图形左上角为点，宽和高，以某个角度开始，划过多少度
    sPoint.setX(cPoint.rx());sPoint.setY(cPoint.ry()-height);
    path.lineTo(sPoint);
    path.arcTo(cPoint.rx()-height/2,cPoint.ry()-height,height,height,90,180);
    painter.setPen(Qt::SolidLine);
    painter.drawPath(path);

    path.lineTo(cPoint.rx(),cPoint.ry()-height);
    path.moveTo(cPoint.rx()+width,cPoint.ry());
    path.lineTo(cPoint.rx()+width,cPoint.ry()-height);
    painter.setPen(Qt::DotLine);
    painter.drawPath(path);
}

int RenderArea::getTrapezium_dialog_type() const
{
    return trapezium_dialog_type;
}

void RenderArea::setTrapezium_dialog_type(int value)
{
    trapezium_dialog_type = value;
    update();
}

int RenderArea::getDialog_type() const
{
    return dialog_type;
}

void RenderArea::setDialog_type(int value)
{
    dialog_type = value;
}

double RenderArea::getAlpha() const
{
    return alpha;
}

void RenderArea::setAlpha(double value)
{
    alpha = value;
    update();
}

int RenderArea::getType() const
{
    return type;
}

void RenderArea::setType(int value)
{
    type = value;
    update();
}

int RenderArea::getLine_num() const
{
    return line_num;
}

void RenderArea::setLine_num(int value)
{
    line_num = value;
    update();
}

void RenderArea::drawCoordinates(QPainter &painter)
{
    //绘制正多边形的区域
    if(dialog_type == 1){
        drawpolygon(painter);
        }

    //绘制梯形的区域
    if(dialog_type == 2){
        drawtrapezium(painter);
        }       
    if(dialog_type == 3){
        draweyelet(painter);
       }
    return;
}


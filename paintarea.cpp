#include "paintarea.h"
#include <QPainter>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QDebug>

PaintArea::PaintArea(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
    name = tr("");
    image = QImage(this->width(),this->height(),QImage::Format_RGB32); // 画布的初始化大小设为 400*300，使用 32 位颜色
    tempImage = QImage(this->width(),this->height(),QImage::Format_RGB32); // 画布的初始化大小设为 400*300，使用 32 位颜色
    backColor = qRgb(255,255,255); //画布初始化背景色使用白色
    image.fill(backColor);
    tempImage.fill(backColor);
    modified = false;
    curShape = None;
    penColor = Qt::black;
    brushColor = Qt::black;
    penWidth = 1;
    penStyle = Qt::SolidLine;
    isDrawing = false;
    scale = 1;
    angle = 0;
    shear = 0;
}

void PaintArea::initPaintArea()
{
    curShape = None;
    penColor = Qt::black;
    brushColor = Qt::black;
    penWidth = 1;
    penStyle = Qt::SolidLine;
    isDrawing = false;
}

bool PaintArea::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = image;
    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    } else {
        return false;
    }
}

bool PaintArea::openImage(const QString &fileName)
{
    QImage loadedImage;
    if (!loadedImage.load(fileName)) return false;
    QSize newSize = loadedImage.size();
    setImageSize(newSize.width(), newSize.height());
    image = loadedImage;
    modified = false;
    update();
    return true;
}

QSize PaintArea::getImageSize()
{
    return image.size()*scale;
}

void PaintArea::setModified(bool modified)
{
    this->modified = modified;
}

QString PaintArea::getName()
{
    return this->name;
}

void PaintArea::setName(QString name)
{
    this->name = name;
}

QImage PaintArea::getImage()
{
    return this->image;
}

void PaintArea::print()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *printDialog = new QPrintDialog(&printer, this);
    if (printDialog->exec() == QDialog::Accepted)
    {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
}

void PaintArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.scale(scale,scale);
    if(isDrawing) {
        painter.drawImage(0,0,tempImage);
    }
    else {
        if(angle) {
            QImage copyImage = image;
            QPainter pp(&copyImage);
            QPointF center(copyImage.width()/2.0,copyImage.height()/2.0);
            pp.translate(center);
            pp.rotate(angle);
            pp.translate(-center);
            pp.drawImage(0,0,image);
            image = copyImage; //只会复制图片上的内容，不会复制坐标系统
            angle = 0; //完成旋转后将角度值重新设为 0
        }
        if(shear) {
            QImage copyImage = image;
            QPainter pp(&copyImage);
            pp.shear(shear,shear);
            pp.drawImage(0,0,image);
            image = copyImage;
            shear = 0;
        }
        painter.drawImage(0,0,image);
    }
    //int side = qMin(width(), height());
    //painter.setRenderHint(QPainter::Antialiasing,true);                //开启抗锯齿
    //painter.translate(width() / 2, height() / 2);                      //坐标系统平移变换，把原点平移到窗口中心
    //painter.scale(side / 300.0, side / 300.0);            //坐标系统比例变换，使绘制的图形随窗口的放大而放大
    //ainter.scale(1, -1);                                          //Y轴向上翻转，翻转成正常平面直角坐标系
    //painter.setPen(QPen(Qt::black, height() / 600));
    //painter.drawLine(-2000,0,2000,0);
    //painter.drawLine(0,1500,0,-1500);
    //drawGrid(&painter);
}

void PaintArea::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {     //当鼠标左键按下
        firstPoint = event->pos();              //获得鼠标指针的当前坐标作为起始坐标
        isDrawing = true;
    }
}

void PaintArea::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << event->globalPos().x() << " " << event->globalPos().y();
    qDebug() << event->pos().x() << " " << event->pos().y();
    if(event->buttons()&Qt::LeftButton) {       //如果鼠标左键按着的同时移动鼠标
        secondPoint = event->pos();             //获得鼠标指针的当前坐标作为终止坐标
        if(curShape == None) //如果不进行特殊图形绘制，则直接在 image 上绘制
        {
            isDrawing = false;
            paint(image);
        }
        else //如果绘制特殊图形，则在临时绘图区 tempImage 上绘制
        {
            tempImage = image; //每次绘制tempImage前用上一次image中的图片对其进行填充
            paint(tempImage);
        }
    }
}

void PaintArea::mouseReleaseEvent(QMouseEvent *event)
{
    if(isDrawing){
        image = tempImage;
        isDrawing = false;
    }
    if(event->buttons() == Qt::NoButton) {
        qDebug() << "no button";
    }
    if(event->buttons() == Qt::LeftButton) {
        secondPoint = event->pos();
        isDrawing = false;
        image = tempImage;
        update();
    }
}

void PaintArea::setShape(ShapeType shape)
{
    curShape = shape;
}

void PaintArea::setPenStyle(Qt::PenStyle style)
{
    penStyle = style;
}

void PaintArea::setPenWidth(int width)
{
    penWidth = width;
}

void PaintArea::setPenColor(QColor color)
{
    penColor = color;
}

void PaintArea::setBrushColor(QColor color)
{
    brushColor = color;
}

void PaintArea::setPoint(PaintArea::Points point, int x, int y, int z)
{
    switch (point) {
    case First:
        firstPoint = QPoint(x, y);
        break;
    case Second:
        secondPoint = QPoint(x, y);
    case Third:
        thirdPoint = QPoint(x, y);
    default:
        break;
    }
}

QColor PaintArea::IntToQColor(const int &intColor)
{
    //将Color 从int 转换成 QColor
    int blue = intColor & 255;
    int green = intColor >> 8 & 255;
    int red = intColor >> 16 & 255;
    return QColor(red, green, blue);
}

void PaintArea::paint(QImage &image)
{
    QPainter pp(&image); //在 the Image 上绘图
    QPen pen = QPen();
    pen.setColor(penColor);
    pen.setStyle(penStyle);
    pen.setWidth(penWidth);
    QBrush brush = QBrush(brushColor);
    pp.setPen(pen);
    pp.setBrush(brush);

    int x,y,w,h;
    x = firstPoint.x()/scale;
    y = firstPoint.y()/scale;
    w = secondPoint.x()/scale - x;
    h = secondPoint.y()/scale - y;

//    if(isDrawing){
//        image = tempImage;
//    }

    switch(curShape) {
    case None: //不绘制特殊图形
    {
        pp.drawLine(firstPoint/scale,secondPoint/scale); //由起始坐标和终止坐标绘制直线
        firstPoint = secondPoint; //让终止坐标变为起始坐标
        break;
    }
    case Point:
    {
        pp.drawPoint(x, y);
        break;
    }
    case Line: //绘制直线
    {
        pp.drawLine(firstPoint/scale,secondPoint/scale);
        break;
    }
    case Rectangle: //绘制矩形
    {
        pp.drawRect(x,y,w,h);
        break;
    }
    case Ellipse: //绘制椭圆
    {
        pp.drawEllipse(x,y,w,h);
        break;
    }
    }
    update(); //进行更新界面显示，可引起窗口重绘事件，重绘窗口
    modified = true;
}

void PaintArea::setImageSize(int width, int height)
{
    QImage newImage(width, height, QImage::Format_RGB32);
    image = newImage;
    update();
}

void PaintArea::setImageColor(QColor color)
{
    backColor = color.rgb();        //因为 image 的背景色要用 QRgb 类型的颜色，所以这里进行了一下转换
    image.fill(backColor);
    update();
}

void PaintArea::zoomIn() {
    scale*=1.2;
    update();
}

void PaintArea::zoomOut() {
    scale/=1.2;
    update();
}

void PaintArea::zoomBack() {
    scale = 1;
    update();
}

void PaintArea::doRotate(int in_angle, int direction) {
    angle += direction * in_angle;
    update();
}

void PaintArea::doShear() {
    shear = 0.2;
    update();
}

void PaintArea::doClear() {
    image.fill(backColor);
    update();
}

void PaintArea::drawGrid(QPainter *painter)
{
    painter->drawLine(0,0,100,100);
    int Margin=40;//边缘
    QRect rect;
    //取得绘图区域，大小要减去旁白
    //rect=QRect(Margin+300,Margin+300,width()-Margin-700,height()-Margin-500);
    rect=QRect(Margin+25,Margin,width()-2*Margin-10,height()-2*Margin);
    for(int i=0;i<=20;i++)
    {
    int x=rect.left()+(i*(rect.width()-1)/20);
    painter->drawLine(x,rect.top(),x,rect.bottom());
    }
    for(int j=0;j<=10;j++)
    {
        int y=rect.bottom()-(j*(rect.height()-1)/10);
        painter->drawLine(rect.left()-5,y,rect.right(),y);
    }
}

void PaintArea::paint()
{
    QPainter pp(&image); //在 theImage 上绘图
    QPen pen = QPen();

    pen.setColor(penColor);
    pen.setStyle(penStyle);
    pen.setWidth(penWidth);
    QBrush brush = QBrush(brushColor);
    pp.setPen(pen);
    pp.setBrush(brush);

    int x,y,w,h;
    x = firstPoint.x()/scale;
    y = firstPoint.y()/scale;
    w = secondPoint.x()/scale - x;
    h = secondPoint.y()/scale - y;
    switch(curShape) {
    case None: //不绘制特殊图形
    {
        pp.drawLine(firstPoint/scale,secondPoint/scale); //由起始坐标和终止坐标绘制直线
        firstPoint = secondPoint; //让终止坐标变为起始坐标
        break;
    }
    case Point:
    {
        pp.drawPoint(x, y);
        break;
    }
    case Line: //绘制直线
    {
        pp.drawLine(firstPoint/scale,secondPoint/scale);
        break;
    }
    case Rectangle: //绘制矩形
    {
        pp.drawRect(x,y,w,h);
        break;
    }
    case Ellipse: //绘制椭圆
    {
        pp.drawEllipse(x,y,w,h);
        break;
    }
    }
    update(); //进行更新界面显示，可引起窗口重绘事件，重绘窗口
    modified = true;
}

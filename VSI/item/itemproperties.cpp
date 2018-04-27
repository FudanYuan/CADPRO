
#include <QDebug>
#include <QComboBox>

#include "itemproperties.h"

ItemProperties::ItemProperties():
    shapeid(0),
    length(0)
{
    rows = 0;
    polygonEdge = 4;
    polygonEdgeLength = 50;
    polygonRad = 0;
    offset = 50;
}

//属性中通用的
void ItemProperties::initdialog()
{
    mainLayout = new QVBoxLayout;

    tableWidget =new QTableWidget();

    tableWidget->setColumnCount(2);
    tableWidget->setFrameShape(QFrame::Box);//设置边框
    tableWidget->setShowGrid(true); //设置显示格子线
    //table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    //表头内容
    QStringList m_Header;
    m_Header<<tr("属性")<<tr("值");
    tableWidget->setHorizontalHeaderLabels(m_Header);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //设置表头字体加粗
    QFont font;
    font.setBold(true);
    tableWidget->horizontalHeader()->setFont(font);
    //设置表格内容

    //显示图形的ShapeId
    rows = 0;//列表第一行
    tableWidget->insertRow(rows);
    tableitem= new QTableWidgetItem(tr("ID"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    QString string = QString::number(shapeid, 10);//将int变成QString类
    tableitem = new QTableWidgetItem(string);
    tableWidget->setItem(rows,1,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);

    //图形类型
    tableWidget->insertRow(++rows);//加一行
    tableitem = new QTableWidgetItem(tr("类型"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    tableitem = new QTableWidgetItem(this->getShapetype());
    tableWidget->setItem(rows,1,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);

    //显示线形
    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("线形"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    lineStyleComboBox *itemlinestyle = new lineStyleComboBox(tr("属性框中的线形"),this);
    tableWidget->setCellWidget(rows,1,itemlinestyle);
    QObject::connect(itemlinestyle,&lineStyleComboBox::customActivated,this,linestylechange);

    //显示颜色
    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("颜色"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    ColorComboBox *itemcolor = new ColorComboBox(tr("12332"),this);
    tableWidget->setCellWidget(rows,1,itemcolor);
    QObject::connect(itemcolor,&ColorComboBox::colorChanged,this,linecolorchange);

    //属性
    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("属性"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    lineTypeChoose =new QComboBox;
    lineTypeChoose->addItem(tr("通用"));
    lineTypeChoose->addItem(tr("标记"));
    lineTypeChoose->addItem(tr("周长"));
    lineTypeChoose->addItem(tr("切割"));
    lineTypeChoose->addItem(tr("缝线"));
    tableWidget->setCellWidget(rows,1,lineTypeChoose);
    QObject::connect(lineTypeChoose,SIGNAL(activated(QString)),this,SLOT(linetypechange()));

    switch (this->getCurShape())
    {
        case Shape::Line:
            initLineItemproperties();
            break;
        case Shape::Arc:
            initArcItemproperties();
            break;
        case Shape::Polygon:
            initPolygonItemproperties();
            break;
        case Shape::Circle:
            initCircleItemproperties();
            break;
        case Shape::Rectangle:
            initRectItemproperties();
            break;
        case Shape::Ellipse:
            initEllipseItemproperties();
            break;
        case Shape::Trapezium:
            initTrapeziumItemproperties();
            break;
        case Shape::Eyelet:
            initEyeletItemproperties();
            break;
        default:
            break;
    }

    mainLayout->addWidget(tableWidget);
    setLayout(mainLayout);
}
//设置直线中一些特殊的属性
void ItemProperties::initLineItemproperties()
{
    //显示线段长度
    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("长度"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    QString string = QString::number(length,'g',6);
    tableitem = new QTableWidgetItem(string);
    tableWidget->setItem(rows,1,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    //QObject::connect(tableitem,signal())
}
//设置圆弧的一些特殊属性
void ItemProperties::initArcItemproperties()
{

}
//设置正多边形的特殊属性
void ItemProperties::initPolygonItemproperties()
{
    //显示正多边形边数
    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("边数"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    QString string = QString::number(polygonEdge,'g',6);
    QLineEdit *edge =new QLineEdit(string);
    edge->setAlignment(Qt::AlignCenter);
    tableWidget->setCellWidget(rows,1,edge);
    connect(edge,&QLineEdit::textEdited,this,polygonEdgeChange);

    //显示正多边形边长
    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("半径"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    string = QString::number(polygonEdgeLength,'g',6);
    QLineEdit *edgelength =new QLineEdit(string);
    edgelength->setAlignment(Qt::AlignCenter);
    tableWidget->setCellWidget(rows,1,edgelength);
    connect(edgelength,&QLineEdit::textEdited,this,polygonEdgeLengthChange);

    //显示正多边形旋转
    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("旋转度数"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    string = QString::number(polygonRad,'g',6);
    QLineEdit *rad =new QLineEdit(string);
    rad->setAlignment(Qt::AlignCenter);
    tableWidget->setCellWidget(rows,1,rad);
    connect(rad,&QLineEdit::textEdited,this,polygonRadChange);

    //偏移
    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("偏移"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    QCheckBox *isoffset = new QCheckBox(tr("选择"));
    tableWidget->setCellWidget(rows,1,isoffset);
    connect(isoffset,&QCheckBox::clicked,this,insertoffset);

    tableWidget->insertRow(++rows);
    tableitem= new QTableWidgetItem(tr("偏移量"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    QLineEdit *offset =new QLineEdit(tr("50"));
    offset->setAlignment(Qt::AlignCenter);
    connect(offset,&QLineEdit::textEdited,this,setOffset);
    tableWidget->setCellWidget(rows,1,offset);
}
//设置圆的特殊属性
void ItemProperties::initCircleItemproperties()
{
    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("半径"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    QString string = QString::number(circleR,'g',6);
    QLineEdit *edgelength =new QLineEdit(string);
    edgelength->setAlignment(Qt::AlignCenter);
    tableWidget->setCellWidget(rows,1,edgelength);
    connect(edgelength,&QLineEdit::textEdited,this,polygonEdgeLengthChange);

    //偏移
    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("偏移"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    QCheckBox *isoffset = new QCheckBox(tr("选择"));
    tableWidget->setCellWidget(rows,1,isoffset);
    connect(isoffset,&QCheckBox::clicked,this,insertoffset);

    tableWidget->insertRow(++rows);
    tableitem= new QTableWidgetItem(tr("偏移量"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    QLineEdit *offset =new QLineEdit(tr("50"));
    offset->setAlignment(Qt::AlignCenter);
    connect(offset,&QLineEdit::textEdited,this,setOffset);
    tableWidget->setCellWidget(rows,1,offset);
}
//设置矩形的特殊属性
void ItemProperties::initRectItemproperties()
{
    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("长"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    QString string = QString::number(rectLength,'g',6);
    QLineEdit *edge =new QLineEdit(string);
    edge->setAlignment(Qt::AlignCenter);
    tableWidget->setCellWidget(rows,1,edge);
    connect(edge,&QLineEdit::textEdited,this,rectLengthChange);

    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("宽"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    string = QString::number(rectHeigth,'g',6);
    QLineEdit *edgelength =new QLineEdit(string);
    edgelength->setAlignment(Qt::AlignCenter);
    tableWidget->setCellWidget(rows,1,edgelength);
    connect(edgelength,&QLineEdit::textEdited,this,rectHeigthChange);

    //偏移
    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("偏移"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    QCheckBox *isoffset = new QCheckBox(tr("选择"));
    tableWidget->setCellWidget(rows,1,isoffset);
    connect(isoffset,&QCheckBox::clicked,this,insertoffset);

    tableWidget->insertRow(++rows);
    tableitem= new QTableWidgetItem(tr("偏移量"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    QLineEdit *offset =new QLineEdit(tr("50"));
    offset->setAlignment(Qt::AlignCenter);
    connect(offset,&QLineEdit::textEdited,this,setOffset);
    tableWidget->setCellWidget(rows,1,offset);

}
//椭圆
void ItemProperties::initEllipseItemproperties()
{
    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("长轴长"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    QString string = QString::number(ellipseR1,'g',6);
    QLineEdit *r1 =new QLineEdit(string);
    r1->setAlignment(Qt::AlignCenter);
    tableWidget->setCellWidget(rows,1,r1);
    connect(r1,&QLineEdit::textEdited,this,ellipseR1Change);

    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("短轴长"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    string = QString::number(ellipseR2,'g',6);
    QLineEdit *r2 =new QLineEdit(string);
    r2->setAlignment(Qt::AlignCenter);
    tableWidget->setCellWidget(rows,1,r2);
    connect(r2,&QLineEdit::textEdited,this,ellipseR2Change);

    //显示正多边形旋转
    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("旋转度数"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    string = QString::number(ellipseAlpha,'g',6);
    QLineEdit *rad =new QLineEdit(string);
    rad->setAlignment(Qt::AlignCenter);
    tableWidget->setCellWidget(rows,1,rad);
    connect(rad,&QLineEdit::textEdited,this,ellipseAlphaChange);
}
//梯形
void ItemProperties::initTrapeziumItemproperties()
{
    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("上底"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    QString string = QString::number(trapeziumTop,'g',6);
    QLineEdit *top =new QLineEdit(string);
    top->setAlignment(Qt::AlignCenter);
    tableWidget->setCellWidget(rows,1,top);
    connect(top,&QLineEdit::textEdited,this,trapeziumTopChange);

    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("高"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    string = QString::number(trapeziumHeigth,'g',6);
    QLineEdit *heigth =new QLineEdit(string);
    heigth->setAlignment(Qt::AlignCenter);
    tableWidget->setCellWidget(rows,1,heigth);
    connect(heigth,&QLineEdit::textEdited,this,trapeziumHeigthChange);

    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("角度1"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    string = QString::number(trapeziumAlpha1,'g',6);
    QLineEdit *alpha1 =new QLineEdit(string);
    alpha1->setAlignment(Qt::AlignCenter);
    tableWidget->setCellWidget(rows,1,alpha1);
    connect(alpha1,&QLineEdit::textEdited,this,trapeziumAlpha1Change);

    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("角度2"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    string = QString::number(trapeziumAlpha2,'g',6);
    QLineEdit *alpha2 =new QLineEdit(string);
    alpha2->setAlignment(Qt::AlignCenter);
    tableWidget->setCellWidget(rows,1,alpha2);
    connect(alpha2,&QLineEdit::textEdited,this,trapeziumAlpha2Change);

    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("偏移"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    QCheckBox *isoffset = new QCheckBox(tr("选择"));
    tableWidget->setCellWidget(rows,1,isoffset);
    connect(isoffset,&QCheckBox::clicked,this,insertoffset);

    tableWidget->insertRow(++rows);
    tableitem= new QTableWidgetItem(tr("偏移量"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    QLineEdit *offset =new QLineEdit(tr("50"));
    offset->setAlignment(Qt::AlignCenter);
    connect(offset,&QLineEdit::textEdited,this,setOffset);
    tableWidget->setCellWidget(rows,1,offset);
}

void ItemProperties::initEyeletItemproperties()
{
    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("长"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    QString string = QString::number(rectLength,'g',6);
    QLineEdit *edge =new QLineEdit(string);
    edge->setAlignment(Qt::AlignCenter);
    tableWidget->setCellWidget(rows,1,edge);
    connect(edge,&QLineEdit::textEdited,this,rectLengthChange);

    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("宽"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    string = QString::number(rectHeigth,'g',6);
    QLineEdit *edgelength =new QLineEdit(string);
    edgelength->setAlignment(Qt::AlignCenter);
    tableWidget->setCellWidget(rows,1,edgelength);
    connect(edgelength,&QLineEdit::textEdited,this,rectHeigthChange);

    tableWidget->insertRow(++rows);
    tableitem = new QTableWidgetItem(tr("旋转度数"));
    tableWidget->setItem(rows,0,tableitem);
    tableitem->setTextAlignment(Qt::AlignCenter);
    QLineEdit *rad =new QLineEdit(tr("0"));
    rad->setAlignment(Qt::AlignCenter);
    tableWidget->setCellWidget(rows,1,rad);
    connect(rad,&QLineEdit::textEdited,this,ellipseAlphaChange);
}

int ItemProperties::getShapeid() const
{
    return shapeid;
}

void ItemProperties::setShapeid(int value)
{
    shapeid = value;
}

double ItemProperties::getLength() const
{
    return length;
}

void ItemProperties::setLength(double value)
{
    length = value;
}

bool ItemProperties::getOk() const
{
    return ok;
}

void ItemProperties::setOk(bool value)
{
    ok = value;
    this->initdialog();
}

QPen ItemProperties::getPen() const
{
    return pen;
}

void ItemProperties::setPen(const QPen &value)
{
    pen = value;
}

Shape::ShapeType ItemProperties::getCurShape() const
{
    return curShape;
}

void ItemProperties::setCurShape(const Shape::ShapeType &value)
{
    curShape = value;
}

Configure::PenStyle ItemProperties::getPenstyle() const
{
    return penstyle;
}

void ItemProperties::setPenstyle(const Configure::PenStyle &value)
{
    penstyle = value;
}

QString ItemProperties::getShapetype() const
{
    return shapetype;
}

void ItemProperties::setShapetype(const QString &value)
{
    shapetype = value;
}

bool ItemProperties::getIsinsertoffset() const
{
    return isinsertoffset;
}

void ItemProperties::setIsinsertoffset(bool value)
{
    isinsertoffset = value;
}

double ItemProperties::getOffset()
{
    return this->offset;
}

int ItemProperties::getPolygonEdge() const
{
    return polygonEdge;
}

void ItemProperties::setPolygonEdge(int value)
{
    polygonEdge = value;
}

double ItemProperties::getPolygonEdgeLength() const
{
    return polygonEdgeLength;
}

void ItemProperties::setPolygonEdgeLength(double value)
{
    polygonEdgeLength = value;
}

double ItemProperties::getPolygonRad() const
{
    return polygonRad;
}

void ItemProperties::setPolygonRad(double value)
{
    polygonRad = value;
}

double ItemProperties::getRectLength() const
{
    return rectLength;
}

void ItemProperties::setRectLength(double value)
{
    rectLength = value;
}

double ItemProperties::getRectHeigth() const
{
    return rectHeigth;
}

void ItemProperties::setRectHeigth(double value)
{
    rectHeigth = value;
}

double ItemProperties::getCircleR() const
{
    return circleR;
}

void ItemProperties::setCircleR(double value)
{
    circleR = value;
}

double ItemProperties::getEllipseR2() const
{
    return ellipseR2;
}

void ItemProperties::setEllipseR2(double value)
{
    ellipseR2 = value;
}

double ItemProperties::getEllipseR1() const
{
    return ellipseR1;
}

void ItemProperties::setEllipseR1(double value)
{
    ellipseR1 = value;
}

double ItemProperties::getEllipseAlpha() const
{
    return ellipseAlpha;
}

void ItemProperties::setEllipseAlpha(double value)
{
    ellipseAlpha = value;
}

double ItemProperties::getTrapeziumTop() const
{
    return trapeziumTop;
}

void ItemProperties::setTrapeziumTop(double value)
{
    trapeziumTop = value;
}

double ItemProperties::getTrapeziumHeigth() const
{
    return trapeziumHeigth;
}

void ItemProperties::setTrapeziumHeigth(double value)
{
    trapeziumHeigth = value;
}

double ItemProperties::getTrapeziumAlpha1() const
{
    return trapeziumAlpha1;
}

void ItemProperties::setTrapeziumAlpha1(double value)
{
    trapeziumAlpha1 = value;
}

double ItemProperties::getTrapeziumAlpha2() const
{
    return trapeziumAlpha2;
}

void ItemProperties::setTrapeziumAlpha2(double value)
{
    trapeziumAlpha2 = value;
}

void ItemProperties::typechanged()
{
    emit this->PropertiesChanged();
    //    qDebug()<<"发出信号";
}

void ItemProperties::linestylechange(QString name, int index)
{
    switch (index)
    {
        case 0:
            pen.setStyle(Qt::CustomDashLine);
            break;
        case 1:
            pen.setStyle(Qt::SolidLine);
            break;
        case 2:
            pen.setStyle(Qt::DashLine);
            break;
        case 3:
            pen.setStyle(Qt::DotLine);
            break;
        case 4:
            pen.setStyle(Qt::DashDotLine);
            break;
        case 5:
            pen.setStyle(Qt::DashDotDotLine);
            break;
        default:
            break;
    }
    typechanged();
}

void ItemProperties::linecolorchange(QString key, QColor color)
{
    //qDebug() << "颜色：" << key << " " << color.rgba();
    pen.setColor(color);
    typechanged();
}

void ItemProperties::linetypechange()
{
    switch (lineTypeChoose->currentIndex())
    {
        case 0:
            this->setPenstyle(eStyle.generic);
            break;
        case 1:
            this->setPenstyle(eStyle.mark);
            break;
        case 2:
            this->setPenstyle(eStyle.perimeterLine);
            break;
        case 3:
            this->setPenstyle(eStyle.cut);
            break;
        case 4:
            this->setPenstyle(eStyle.stitch);
            break;
        default:
            break;
    }
    typechanged();
}

void ItemProperties::polygonEdgeChange(QString key)
{
    this->setPolygonEdge(key.toInt());
    typechanged();
}

void ItemProperties::polygonEdgeLengthChange(QString key)
{
    this->setPolygonEdgeLength(key.toDouble());
    typechanged();
}

void ItemProperties::polygonRadChange(QString key)
{
    this->setPolygonRad(key.toDouble());
    typechanged();
}

void ItemProperties::rectLengthChange(QString key)
{
    this->rectLength = key.toDouble();
    typechanged();
}

void ItemProperties::rectHeigthChange(QString key)
{
    this->rectHeigth = key.toDouble();
    typechanged();
}

void ItemProperties::ellipseR1Change(QString key)
{
    this->ellipseR1 = key.toDouble();
    typechanged();
}

void ItemProperties::ellipseR2Change(QString key)
{
    this->ellipseR2 = key.toDouble();
    typechanged();
}

void ItemProperties::ellipseAlphaChange(QString key)
{
    this->ellipseAlpha = key.toDouble();
    typechanged();
}

void ItemProperties::trapeziumTopChange(QString key)
{
    this->trapeziumTop = key.toDouble();
    typechanged();
}

void ItemProperties::trapeziumHeigthChange(QString key)
{
    this->trapeziumHeigth = key.toDouble();
    typechanged();
}

void ItemProperties::trapeziumAlpha1Change(QString key)
{
    this->trapeziumAlpha1 = key.toDouble();
    typechanged();
}

void ItemProperties::trapeziumAlpha2Change(QString key)
{
    this->trapeziumAlpha2 = key.toDouble();
    typechanged();
}

void ItemProperties::insertoffset(bool isoffset)
{
    this->setIsinsertoffset(isoffset);
    typechanged();
}

void ItemProperties::setOffset(QString offset)
{
    this->offset = offset.toDouble();
    typechanged();
}

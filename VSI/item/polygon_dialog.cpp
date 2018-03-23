#include <QtWidgets>

#include "polygon_dialog.h"
#include <QGraphicsView>


//! [0]
Polygon_dialog::Polygon_dialog()
{
    len_num=4;
    raduii=100;
    angle=0;
    penstyle=1;
    initdialog();
}
void Polygon_dialog::createtopGroupBox(){
    topGroupBox = new QGroupBox;
    QGridLayout *layout = new QGridLayout;
    topGroupBox->setMaximumSize(200,230);
    topGroupBox->setMinimumSize(200,230);


    len = new QLabel(tr("边数"));
    rad = new QLabel(tr("半径"));
    angel = new QLabel(tr("旋转"));
    len_edit =new QLineEdit;
    len_edit->setText("4");//设置初始边数
    //len_edit->setValidator(new QIntValidator(3,20,this));
    len->setBuddy(len_edit);

    rad_edit =new QLineEdit;
    rad->setBuddy(rad_edit);
    rad_edit->setText("100");//设置初始半径
    //rad_edit->setValidator(new QDoubleValidator(0,1000,this));

    angel_edit =new QLineEdit;
    angel->setBuddy(angel_edit);
    angel_edit->setText("0.00");//设置初始的旋转角度
    //angel_edit->setValidator(new QDoubleValidator(0,360,this));

    line_type = new QLabel(tr("线类型"));
    line_type_choose =new QComboBox;
    line_type->setBuddy(line_type_choose);
    line_type_choose->addItem(tr("通用"));
    line_type_choose->addItem(tr("标记"));
    line_type_choose->addItem(tr("周长"));
    line_type_choose->addItem(tr("切割"));
    line_type_choose->addItem(tr("缝线"));

    layout->addWidget(len,0,0,1,1);
    layout->addWidget(rad,1,0,1,1);
    layout->addWidget(angel,2,0,1,1);
    layout->addWidget(line_type,3,0,1,1);
    layout->addWidget(len_edit,0,1,1,1);
    layout->addWidget(rad_edit,1,1,1,1);
    layout->addWidget(angel_edit,2,1,1,1);
    layout->addWidget(line_type_choose,3,1,1,1);
    layout->setColumnStretch(0,10);
    layout->setColumnStretch(1,10);


    topGroupBox->setLayout(layout);

    QObject::connect(len_edit,SIGNAL(textChanged(QString)),this,SLOT(len_numchanged()));
    QObject::connect(rad_edit,SIGNAL(textChanged(QString)),this,SLOT(radchanged()));
    QObject::connect(angel_edit,SIGNAL(textChanged(QString)),this,SLOT(angelchanged()));
    QObject::connect(line_type_choose,SIGNAL(activated(QString)),this,SLOT(typechanged()));


}

void Polygon_dialog::createtop_insideGroupBox(){
    top_insideGroupBox = new QGroupBox(tr("模式"));
    QGridLayout *layout = new QGridLayout;

    radio1 = new QRadioButton(tr("在某个图形内"));
    radio1->setChecked(true);
    radio2 = new QRadioButton(tr("内接（边线群组）"));
    layout->addWidget(radio1,0,0,1,1);
    layout->addWidget(radio2,1,0,1,1);

    QObject::connect(radio1,SIGNAL(toggled(bool)),this,SLOT(modelchanged()));
    QObject::connect(radio2,SIGNAL(toggled(bool)),this,SLOT(modelchanged()));

    top_insideGroupBox->setLayout(layout);
}

void Polygon_dialog::initdialog(){
    createtopGroupBox();
    createtop_insideGroupBox();

    QVBoxLayout *mainLayout = new QVBoxLayout;

    QPushButton *ok = new QPushButton(tr("确定"));
    QPushButton *cancel = new QPushButton(tr("取消"));

    connect(ok,SIGNAL(clicked()),this,SLOT(onclickedok()));
    connect(cancel,SIGNAL(clicked()),this,SLOT(onclickedcancel()));

    QHBoxLayout *layout1 = new QHBoxLayout();

    originalRenderArea = new RenderArea(this,len_num,angle,penstyle,1);//显示绘图区域,1表示绘制正多边形的框

    layout1->addWidget(originalRenderArea);
    layout1->addWidget(topGroupBox);
    layout1->addWidget(top_insideGroupBox);

    mainLayout->addLayout(layout1);

    QHBoxLayout *layout2 = new QHBoxLayout();
    layout2->addWidget(cancel);
    layout2->addStretch(0);
    layout2->addWidget(ok);

    mainLayout->addLayout(layout2);

    setMaximumSize(600,280);
    setMinimumSize(600,280);
    setLayout(mainLayout);

    setWindowTitle(tr("正多边形绘制"));
}

int Polygon_dialog::getPenstyle() const
{
    return penstyle;
}

void Polygon_dialog::setPenstyle(int &value)
{
    penstyle = value;
}

bool Polygon_dialog::getOk() const
{
    return ok;
}

void Polygon_dialog::setOk(bool value)
{
    ok = value;
}

double Polygon_dialog::getAngle() const
{
    return angle;
}

void Polygon_dialog::setAngle(double value)
{
    angle = value;
}

double Polygon_dialog::getRaduii() const
{
    return raduii;
}

void Polygon_dialog::setRaduii(double value)
{
    raduii = value;
}

int Polygon_dialog::getLen_num() const
{
    return len_num;
}

void Polygon_dialog::setLen_num(int value)
{
    len_num = value;
}

void Polygon_dialog::onclickedok(){
    setOk(true);
    this->accept();
}
void Polygon_dialog::onclickedcancel(){
    setOk(false);
    this->reject();
}
void Polygon_dialog::len_numchanged(){
    this->setLen_num(len_edit->text().toInt());
    len_num=len_edit->text().toInt();
    originalRenderArea->setLine_num(len_num);
//    qDebug()<<"输出边数"<<len_edit->text().toInt();
}
void Polygon_dialog::radchanged(){
    this->setRaduii(rad_edit->text().toDouble());
}
void Polygon_dialog::angelchanged(){
    this->setAngle(angel_edit->text().toDouble());
    angle=angel_edit->text().toDouble();
    originalRenderArea->setAlpha(angle);

}

//获取线对象
void Polygon_dialog::typechanged(){
    int line=line_type_choose->currentIndex();
    this->setPenstyle(line);
    qDebug()<<"线类型："<<line_type_choose->currentIndex();
 /*   switch (this->line_type_choose->currentIndex()){

    case 0:
        this->setPenstyle(Configure::PenStyle);
        break;
    case 1:
        this->setPenstyle(Configure::EntityStyle::mark);
        break;
    case 2:
        this->setPenstyle(Configure::EntityStyle::perimeterLine);
        break;
    case 3:
        this->setPenstyle(Configure::EntityStyle::cut);
        break;
    case 4:
        this->setPenstyle(Configure::EntityStyle::stitch);
        break;
    }
    */
}

void Polygon_dialog::modelchanged()
{
    if(radio1->isChecked())
        originalRenderArea->setType(1);
    if(radio2->isChecked())
        originalRenderArea->setType(2);
}

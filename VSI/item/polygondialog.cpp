#include <QtWidgets>

#include "polygondialog.h"
#include <QGraphicsView>


//! [0]
PolygonDialog::PolygonDialog()
{
    lenNum=4;
    raduii=100;
    angle=0;
    penstyle=1;
    initdialog();
}
void PolygonDialog::createtopGroupBox(){
    topGroupBox = new QGroupBox;
    QGridLayout *layout = new QGridLayout;
    topGroupBox->setMaximumSize(200,230);
    topGroupBox->setMinimumSize(200,230);


    len = new QLabel(tr("边数"));
    rad = new QLabel(tr("半径"));
    angel = new QLabel(tr("旋转"));
    lenEdit =new QLineEdit;
    lenEdit->setText("4");//设置初始边数
    //len_edit->setValidator(new QIntValidator(3,20,this));
    len->setBuddy(lenEdit);

    radEdit =new QLineEdit;
    rad->setBuddy(radEdit);
    radEdit->setText("100");//设置初始半径
    //rad_edit->setValidator(new QDoubleValidator(0,1000,this));

    angelEdit =new QLineEdit;
    angel->setBuddy(angelEdit);
    angelEdit->setText("0.00");//设置初始的旋转角度
    //angel_edit->setValidator(new QDoubleValidator(0,360,this));

    line_type = new QLabel(tr("线类型"));
    lineTypeChoose =new QComboBox;
    line_type->setBuddy(lineTypeChoose);
    lineTypeChoose->addItem(tr("通用"));
    lineTypeChoose->addItem(tr("标记"));
    lineTypeChoose->addItem(tr("周长"));
    lineTypeChoose->addItem(tr("切割"));
    lineTypeChoose->addItem(tr("缝线"));

    layout->addWidget(len,0,0,1,1);
    layout->addWidget(rad,1,0,1,1);
    layout->addWidget(angel,2,0,1,1);
    layout->addWidget(line_type,3,0,1,1);
    layout->addWidget(lenEdit,0,1,1,1);
    layout->addWidget(radEdit,1,1,1,1);
    layout->addWidget(angelEdit,2,1,1,1);
    layout->addWidget(lineTypeChoose,3,1,1,1);
    layout->setColumnStretch(0,10);
    layout->setColumnStretch(1,10);


    topGroupBox->setLayout(layout);

    QObject::connect(lenEdit,SIGNAL(textChanged(QString)),this,SLOT(lenNumchanged()));
    QObject::connect(radEdit,SIGNAL(textChanged(QString)),this,SLOT(radchanged()));
    QObject::connect(angelEdit,SIGNAL(textChanged(QString)),this,SLOT(angelchanged()));
    QObject::connect(lineTypeChoose,SIGNAL(activated(QString)),this,SLOT(typechanged()));


}

void PolygonDialog::createtop_insideGroupBox(){
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

void PolygonDialog::initdialog(){
    createtopGroupBox();
    createtop_insideGroupBox();

    QVBoxLayout *mainLayout = new QVBoxLayout;

    QPushButton *ok = new QPushButton(tr("确定"));
    QPushButton *cancel = new QPushButton(tr("取消"));

    connect(ok,SIGNAL(clicked()),this,SLOT(onclickedok()));
    connect(cancel,SIGNAL(clicked()),this,SLOT(onclickedcancel()));

    QHBoxLayout *layout1 = new QHBoxLayout();

    originalRenderArea = new RenderArea(this,lenNum,angle,penstyle,1);//显示绘图区域,1表示绘制正多边形的框

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

int PolygonDialog::getPenstyle() const
{
    return penstyle;
}

void PolygonDialog::setPenstyle(int &value)
{
    penstyle = value;
}

bool PolygonDialog::getOk() const
{
    return ok;
}

void PolygonDialog::setOk(bool value)
{
    ok = value;
}

double PolygonDialog::getAngle() const
{
    return angle;
}

void PolygonDialog::setAngle(double value)
{
    angle = value;
}

double PolygonDialog::getRaduii() const
{
    return raduii;
}

void PolygonDialog::setRaduii(double value)
{
    raduii = value;
}

int PolygonDialog::getLenNum() const
{
    return lenNum;
}

void PolygonDialog::setLenNum(int value)
{
    lenNum = value;
}

void PolygonDialog::onclickedok(){
    setOk(true);
    this->accept();
}
void PolygonDialog::onclickedcancel(){
    setOk(false);
    this->reject();
}
void PolygonDialog::lenNumchanged(){
    this->setLenNum(lenEdit->text().toInt());
    lenNum=lenEdit->text().toInt();
    originalRenderArea->setLineNum(lenNum);
//    qDebug()<<"输出边数"<<len_edit->text().toInt();
}
void PolygonDialog::radchanged(){
    this->setRaduii(radEdit->text().toDouble());
}

void PolygonDialog::angelchanged(){
    this->setAngle(angelEdit->text().toDouble());
    angle=angelEdit->text().toDouble();
    originalRenderArea->setAlpha(angle);
}

//获取线对象
void PolygonDialog::typechanged(){
    int line=lineTypeChoose->currentIndex();
    this->setPenstyle(line);
    qDebug()<<"线类型："<<lineTypeChoose->currentIndex();
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

void PolygonDialog::modelchanged()
{
    if(radio1->isChecked())
        originalRenderArea->setType(1);
    if(radio2->isChecked())
        originalRenderArea->setType(2);
}

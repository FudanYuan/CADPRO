#include <QtWidgets>

#include "eyeletdialog.h"
#include <QGraphicsView>


//! [0]
EyeletDialog::EyeletDialog()
{
    eyeleWidth =100;//鸡眼孔的宽
    eyeletHeight =100;//鸡眼孔的高
    penStyle = 0;//线条类型
    intidialog();
}

void EyeletDialog::intidialog()
{
    mainLayout = new QVBoxLayout;//主框架
    layout1 = new QHBoxLayout();//水平框架
    layout2 = new QHBoxLayout();

    topGroupBox = new QGroupBox;
    layout3 = new QGridLayout;//表格布局,用于输入框等的布局
    creatlayout3();

    //将确定取消的按键加入布局
    QPushButton *ok = new QPushButton(tr("确定"));
    QPushButton *cancel = new QPushButton(tr("取消"));
    layout2->addWidget(cancel);
    layout2->addStretch(0);
    layout2->addWidget(ok);
    connect(ok,SIGNAL(clicked()),this,SLOT(onclickedok()));
    connect(cancel,SIGNAL(clicked()),this,SLOT(onclickedcancel()));


    //将绘图区域和各种信息输入框加入布局
    paintPlace = new RenderArea(this,3,0);//显示绘图区域
    layout1->addWidget(paintPlace);
    layout1->addWidget(topGroupBox);


    mainLayout->addLayout(layout1);
    mainLayout->addLayout(layout2);


    //主对话框的大小、名称
    setMaximumSize(600,280);
    setMinimumSize(600,280);
    setLayout(mainLayout);
    setWindowTitle(tr("梯形绘制"));
}

void EyeletDialog::creatlayout3()
{
     topGroupBox->setMaximumSize(250,230);
     topGroupBox->setMinimumSize(250,230);

     H = new QLabel(tr("鸡眼孔高度"));
     HEdit = new QLineEdit;
     HEdit->setText(tr("100"));
     W = new QLabel(tr("鸡眼孔的宽"));
     WEdit = new QLineEdit;
     WEdit->setText(tr("100"));

     lineType = new QLabel(tr("线类型"));
     lineTypeChoose =new QComboBox;
     lineType->setBuddy(lineTypeChoose);
     lineTypeChoose->addItem(tr("通用"));
     lineTypeChoose->addItem(tr("标记"));
     lineTypeChoose->addItem(tr("周长"));
     lineTypeChoose->addItem(tr("切割"));
     lineTypeChoose->addItem(tr("缝线"));

     QObject::connect(lineTypeChoose,SIGNAL(activated(QString)),this,SLOT(onlineTypechange()));
     QObject::connect(HEdit,SIGNAL(textChanged(QString)),this,SLOT(onHchange()));
     QObject::connect(WEdit,SIGNAL(textChanged(QString)),this,SLOT(onWchange()));

     layout3->addWidget(H,0,0,1,1);
     layout3->addWidget(HEdit,0,1,1,1);
     layout3->addWidget(W,1,0,1,1);
     layout3->addWidget(WEdit,1,1,1,1);
     layout3->addWidget(lineType,2,0,1,1);
     layout3->addWidget(lineTypeChoose,2,1,1,2);

     layout3->setColumnStretch(0,10);
     layout3->setColumnStretch(1,10);
     layout3->setColumnStretch(2,10);
     layout3->setColumnStretch(3,10);

     topGroupBox->setLayout(layout3);
}

bool EyeletDialog::getOk() const
{
    return ok;
}

void EyeletDialog::setOk(bool value)
{
    ok = value;
}

int EyeletDialog::getPenStyle() const
{
    return penStyle;
}

void EyeletDialog::setPenStyle(int value)
{
    penStyle = value;
}

int EyeletDialog::getEyeletWidth() const
{
    return eyeleWidth;
}

void EyeletDialog::setEyeletWidth(int value)
{
    eyeleWidth = value;
}

int EyeletDialog::getEyeletHeight() const
{
    return eyeletHeight;
}

void EyeletDialog::setEyeletHeight(int value)
{
    eyeletHeight = value;
}

void EyeletDialog::onclickedok()
{
    this->setOk(true);
    this->accept();
}

void EyeletDialog::onclickedcancel()
{
    this->setOk(false);
    this->reject();
}

void EyeletDialog::onHchange()
{
    this->setEyeletHeight(HEdit->text().toInt());
}

void EyeletDialog::onWchange()
{
    this->setEyeletWidth(WEdit->text().toInt());
}

void EyeletDialog::onlineTypechange()
{
    int line=lineTypeChoose->currentIndex();
    this->setPenStyle(line);
}

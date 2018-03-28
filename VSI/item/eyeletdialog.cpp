#include <QtWidgets>

#include "eyeletdialog.h"
#include <QGraphicsView>


//! [0]
EyeletDialog::EyeletDialog()
{
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
    paint_place = new RenderArea(this,3,0);//显示绘图区域
    layout1->addWidget(paint_place);
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
     H_edit = new QLineEdit;
     H_edit->setText(tr("100"));
     W = new QLabel(tr("鸡眼孔的宽"));
     W_edit = new QLineEdit;
     W_edit->setText(tr("100"));

     line_type = new QLabel(tr("线类型"));
     line_type_choose =new QComboBox;
     line_type->setBuddy(line_type_choose);
     line_type_choose->addItem(tr("通用"));
     line_type_choose->addItem(tr("标记"));
     line_type_choose->addItem(tr("周长"));
     line_type_choose->addItem(tr("切割"));
     line_type_choose->addItem(tr("缝线"));

     QObject::connect(line_type_choose,SIGNAL(activated(QString)),this,SLOT(onlineTypechange()));
     QObject::connect(H_edit,SIGNAL(textChanged(QString)),this,SLOT(onHchange()));
     QObject::connect(W_edit,SIGNAL(textChanged(QString)),this,SLOT(onWchange()));

     layout3->addWidget(H,0,0,1,1);
     layout3->addWidget(H_edit,0,1,1,1);
     layout3->addWidget(W,1,0,1,1);
     layout3->addWidget(W_edit,1,1,1,1);
     layout3->addWidget(line_type,2,0,1,1);
     layout3->addWidget(line_type_choose,2,1,1,2);

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

int EyeletDialog::getPenstyle() const
{
    return penstyle;
}

void EyeletDialog::setPenstyle(int value)
{
    penstyle = value;
}

int EyeletDialog::getEyeletwidth() const
{
    return eyeletwidth;
}

void EyeletDialog::setEyeletwidth(int value)
{
    eyeletwidth = value;
}

int EyeletDialog::getEyeletheight() const
{
    return eyeletheight;
}

void EyeletDialog::setEyeletheight(int value)
{
    eyeletheight = value;
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
    this->setEyeletheight(H_edit->text().toInt());
}

void EyeletDialog::onWchange()
{
    this->setEyeletwidth(W_edit->text().toInt());
}

void EyeletDialog::onlineTypechange()
{
    int line=line_type_choose->currentIndex();
    this->setPenstyle(line);
}

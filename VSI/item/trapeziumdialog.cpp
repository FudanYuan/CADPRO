#include <QtWidgets>

#include "trapeziumdialog.h"
#include <QGraphicsView>


//! [0]
TrapeziumDialog::TrapeziumDialog()
{
    alp1=60;
    alp2=60;
    HSize=50;
    topLengthSize=50;
    trapeziumDialogType=1;//没选择插入一半
    intidialog();
}

void TrapeziumDialog::intidialog()
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
    paintPlace = new RenderArea(this,2,getTrapeziumDialogType());//显示绘图区域
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

void TrapeziumDialog::creatlayout3()
{
     topGroupBox->setMaximumSize(250,230);
     topGroupBox->setMinimumSize(250,230);

     alpha1 = new QLabel(tr("角度1"));//梯形的第一个角度
     alpha1Edit = new QLineEdit;
     alpha1Edit->setText(tr("60"));

     alpha2 = new QLabel(tr("角度2"));;//梯形的第二个角度
     alpha2Edit = new QLineEdit;
     alpha2Edit->setText(tr("60"));
     H = new QLabel(tr("梯形高度"));//梯形的高度H
     HEdit = new QLineEdit;
     HEdit->setText(tr("100"));
     topLength = new QLabel(tr("梯形上底"));//梯形的上底
     topLengthEdit = new QLineEdit;
     topLengthEdit->setText(tr("100"));

     chooseCheck = new QCheckBox(tr("插入一半"));

     lineType = new QLabel(tr("线类型"));
     lineTypeChoose =new QComboBox;
     lineType->setBuddy(lineTypeChoose);
     lineTypeChoose->addItem(tr("通用"));
     lineTypeChoose->addItem(tr("标记"));
     lineTypeChoose->addItem(tr("周长"));
     lineTypeChoose->addItem(tr("切割"));
     lineTypeChoose->addItem(tr("缝线"));

     QObject::connect(alpha1Edit,SIGNAL(textChanged(QString)),this,SLOT(onalpha1change()));
     QObject::connect(alpha2Edit,SIGNAL(textChanged(QString)),this,SLOT(onalpha2change()));
     QObject::connect(HEdit,SIGNAL(textChanged(QString)),this,SLOT(onHchange()));
     QObject::connect(topLengthEdit,SIGNAL(textChanged(QString)),this,SLOT(ontopLengthchange()));
     QObject::connect(lineTypeChoose,SIGNAL(activated(QString)),this,SLOT(onlineTypechange()));
     QObject::connect(chooseCheck,SIGNAL(stateChanged (int)),this,SLOT(onchooseCheckchange()));

     layout3->addWidget(alpha1,0,0,1,1);
     layout3->addWidget(alpha1Edit,0,1,1,1);
     layout3->addWidget(alpha2,0,2,1,1);
     layout3->addWidget(alpha2Edit,0,3,1,1);
     layout3->addWidget(H,1,0,1,1);
     layout3->addWidget(HEdit,1,1,1,1);
     layout3->addWidget(topLength,2,0,1,1);
     layout3->addWidget(topLengthEdit,2,1,1,1);
     layout3->addWidget(chooseCheck,2,2,1,3);
     layout3->addWidget(lineType,3,0,1,1);
     layout3->addWidget(lineTypeChoose,3,1,1,2);

     layout3->setColumnStretch(0,10);
     layout3->setColumnStretch(1,10);
     layout3->setColumnStretch(2,10);
     layout3->setColumnStretch(3,10);

     topGroupBox->setLayout(layout3);
}

double TrapeziumDialog::getAlp1() const
{
    return alp1;
}

void TrapeziumDialog::setAlp1(double value)
{
    alp1 = value;
}

double TrapeziumDialog::getAlp2() const
{
    return alp2;
}

void TrapeziumDialog::setAlp2(double value)
{
    alp2 = value;
}

double TrapeziumDialog::getHSize() const
{
    return HSize;
}

void TrapeziumDialog::setHSize(double value)
{
    HSize = value;
}

double TrapeziumDialog::getTopLengthSize() const
{
    return topLengthSize;
}

void TrapeziumDialog::setTopLengthSize(double value)
{
    topLengthSize = value;
}

bool TrapeziumDialog::getOk() const
{
    return ok;
}

void TrapeziumDialog::setOk(bool value)
{
    ok = value;
}

int TrapeziumDialog::getTrapeziumDialogType() const
{
    return trapeziumDialogType;
}

void TrapeziumDialog::setTrapeziumDialogType(int value)
{
    trapeziumDialogType = value;
}

int TrapeziumDialog::getPenstyle() const
{
    return penstyle;
}

void TrapeziumDialog::setPenstyle(int value)
{
    penstyle = value;
}

void TrapeziumDialog::onclickedok()
{
    this->setOk(true);
    this->accept();
}

void TrapeziumDialog::onclickedcancel()
{
    this->setOk(false);
    this->reject();
}

void TrapeziumDialog::onalpha1change()
{
    this->setAlp1(alpha1Edit->text().toDouble());
//    qDebug()<<"角度1:"<<alpha1_edit->text().toDouble();
}

void TrapeziumDialog::onalpha2change()
{
    this->setAlp2(alpha2Edit->text().toDouble());
//    qDebug()<<"角度2:"<<alpha2_edit->text().toDouble();
}

void TrapeziumDialog::onHchange()
{
    this->setHSize(HEdit->text().toDouble());
//    qDebug()<<"高:"<<H_edit->text().toDouble();
}

void TrapeziumDialog::ontopLengthchange()
{
    this->setTopLengthSize(topLengthEdit->text().toDouble());
//    qDebug()<<"上底:"<<top_length_edit->text().toDouble();
}

void TrapeziumDialog::onlineTypechange()
{
    int line=lineTypeChoose->currentIndex();
    this->setPenstyle(line);
}

void TrapeziumDialog::onchooseCheckchange()
{
    if(chooseCheck->isChecked())
    {
        paintPlace->setTrapeziumDialogType(2);
        this->setTrapeziumDialogType(2);
    }
    else
    {
        paintPlace->setTrapeziumDialogType(1);
        this->setTrapeziumDialogType(1);
    }
}

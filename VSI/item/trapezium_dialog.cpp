#include <QtWidgets>

#include "trapezium_dialog.h"
#include <QGraphicsView>


//! [0]
Trapezium_dialog::Trapezium_dialog()
{
    intidialog();
}

void Trapezium_dialog::intidialog()
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
    paint_place = new RenderArea(this,2,getTrapezium_dialog_type());//显示绘图区域
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

void Trapezium_dialog::creatlayout3()
{
     topGroupBox->setMaximumSize(250,230);
     topGroupBox->setMinimumSize(250,230);

     alpha1 = new QLabel(tr("角度1"));//梯形的第一个角度
     alpha1_edit = new QLineEdit;
     alpha1_edit->setText(tr("60"));

     alpha2 = new QLabel(tr("角度2"));;//梯形的第二个角度
     alpha2_edit = new QLineEdit;
     alpha2_edit->setText(tr("60"));
     H = new QLabel(tr("梯形高度"));//梯形的高度H
     H_edit = new QLineEdit;
     H_edit->setText(tr("100"));
     top_length = new QLabel(tr("梯形上底"));//梯形的上底
     top_length_edit = new QLineEdit;
     top_length_edit->setText(tr("100"));

     choose_check = new QCheckBox(tr("插入一半"));

     line_type = new QLabel(tr("线类型"));
     line_type_choose =new QComboBox;
     line_type->setBuddy(line_type_choose);
     line_type_choose->addItem(tr("通用"));
     line_type_choose->addItem(tr("标记"));
     line_type_choose->addItem(tr("周长"));
     line_type_choose->addItem(tr("切割"));
     line_type_choose->addItem(tr("缝线"));

     QObject::connect(alpha1_edit,SIGNAL(textChanged(QString)),this,SLOT(onalpha1change()));
     QObject::connect(alpha2_edit,SIGNAL(textChanged(QString)),this,SLOT(onalpha2change()));
     QObject::connect(H_edit,SIGNAL(textChanged(QString)),this,SLOT(onHchange()));
     QObject::connect(top_length_edit,SIGNAL(textChanged(QString)),this,SLOT(ontop_lengthchange()));
     QObject::connect(line_type_choose,SIGNAL(activated(QString)),this,SLOT(online_typechange()));
     QObject::connect(choose_check,SIGNAL(stateChanged (int)),this,SLOT(onchoose_checkchange()));

     layout3->addWidget(alpha1,0,0,1,1);
     layout3->addWidget(alpha1_edit,0,1,1,1);
     layout3->addWidget(alpha2,0,2,1,1);
     layout3->addWidget(alpha2_edit,0,3,1,1);
     layout3->addWidget(H,1,0,1,1);
     layout3->addWidget(H_edit,1,1,1,1);
     layout3->addWidget(top_length,2,0,1,1);
     layout3->addWidget(top_length_edit,2,1,1,1);
     layout3->addWidget(choose_check,2,2,1,3);
     layout3->addWidget(line_type,3,0,1,1);
     layout3->addWidget(line_type_choose,3,1,1,2);

     layout3->setColumnStretch(0,10);
     layout3->setColumnStretch(1,10);
     layout3->setColumnStretch(2,10);
     layout3->setColumnStretch(3,10);

     topGroupBox->setLayout(layout3);
}

double Trapezium_dialog::getAlp1() const
{
    return alp1;
}

void Trapezium_dialog::setAlp1(double value)
{
    alp1 = value;
}

double Trapezium_dialog::getAlp2() const
{
    return alp2;
}

void Trapezium_dialog::setAlp2(double value)
{
    alp2 = value;
}

double Trapezium_dialog::getH_size() const
{
    return H_size;
}

void Trapezium_dialog::setH_size(double value)
{
    H_size = value;
}

double Trapezium_dialog::getTop_length_size() const
{
    return top_length_size;
}

void Trapezium_dialog::setTop_length_size(double value)
{
    top_length_size = value;
}

bool Trapezium_dialog::getOk() const
{
    return ok;
}

void Trapezium_dialog::setOk(bool value)
{
    ok = value;
}

int Trapezium_dialog::getTrapezium_dialog_type() const
{
    return trapezium_dialog_type;
}

void Trapezium_dialog::setTrapezium_dialog_type(int value)
{
    trapezium_dialog_type = value;
}

int Trapezium_dialog::getPenstyle() const
{
    return penstyle;
}

void Trapezium_dialog::setPenstyle(int value)
{
    penstyle = value;
}

void Trapezium_dialog::onclickedok()
{
    this->setOk(true);
    this->accept();
}

void Trapezium_dialog::onclickedcancel()
{
    this->setOk(false);
    this->reject();
}

void Trapezium_dialog::onalpha1change()
{
    this->setAlp1(alpha1_edit->text().toDouble());
//    qDebug()<<"角度1:"<<alpha1_edit->text().toDouble();
}

void Trapezium_dialog::onalpha2change()
{
    this->setAlp2(alpha2_edit->text().toDouble());
//    qDebug()<<"角度2:"<<alpha2_edit->text().toDouble();
}

void Trapezium_dialog::onHchange()
{
    this->setH_size(H_edit->text().toDouble());
//    qDebug()<<"高:"<<H_edit->text().toDouble();
}

void Trapezium_dialog::ontop_lengthchange()
{
    this->setTop_length_size(top_length_edit->text().toDouble());
//    qDebug()<<"上底:"<<top_length_edit->text().toDouble();
}

void Trapezium_dialog::online_typechange()
{
    int line=line_type_choose->currentIndex();
    this->setPenstyle(line);
}

void Trapezium_dialog::onchoose_checkchange()
{
    if(choose_check->isChecked())
    {
        paint_place->setTrapezium_dialog_type(2);
        this->setTrapezium_dialog_type(2);
    }
    else
    {
        paint_place->setTrapezium_dialog_type(1);
        this->setTrapezium_dialog_type(1);
    }
}

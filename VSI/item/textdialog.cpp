#include "textdialog.h"
#include <QTextEdit>
#include <QDebug>

TextDialog::TextDialog():
    text(tr("hello")),
    textsize(50)
{
    ok=false;
    this->initDialog();
    textcolor = Qt::red;
}

void TextDialog::initDialog()
{
    mainLayout = new QVBoxLayout();//主框架
    layout1 = new QHBoxLayout();//水平框架
    layout2 = new QHBoxLayout();
    layout3 = new QGridLayout();//表格布局,用于输入框等的布局
    layout4 = new QGridLayout();

    initlayout3();
    initlayout4();

    QLabel *textcontent =new QLabel(tr("文本"));
    textcontent1 =new QLineEdit(tr("hello"));

    connect(textcontent1,SIGNAL(textChanged(QString)),this,SLOT(textchanged()));

    layout1->addWidget(textcontent);
    layout1->addWidget(textcontent1);
    layout1->setStretch(0,10);
    layout1->setStretch(1,20);

    //将确定取消的按键加入布局
    QPushButton *ok = new QPushButton(tr("确定"));
    QPushButton *cancel = new QPushButton(tr("取消"));
    layout2->addWidget(cancel);
    layout2->addStretch(0);
    layout2->addWidget(ok);
    connect(ok,SIGNAL(clicked()),this,SLOT(onclickedok()));
    connect(cancel,SIGNAL(clicked()),this,SLOT(onclickedcancel()));

    mainLayout->addLayout(layout1);
    mainLayout->addWidget(middle);
    mainLayout->addWidget(bottom);
    mainLayout->addLayout(layout2);

    setLayout(mainLayout);
    setWindowTitle(tr("文本编辑"));
}

void TextDialog::initlayout3()
{
    middle =new QGroupBox(tr("文本选项"));
    QLabel *textfont =new QLabel(tr("字体"));
    QComboBox *textfont1 =new QComboBox;

    QLabel *textcolor =new QLabel(tr("字体颜色"));
    ColorComboBox *textcolor1 =new ColorComboBox(tr("text color"), this);
    connect(textcolor1,&ColorComboBox::colorChanged,this,onColorChanged);

    QLabel *textheight =new QLabel(tr("文字高度"));
    textheight1 =new QLineEdit(tr("50"));
    QLabel *textheight2 =new QLabel(tr("mn"));
    layout3->addWidget(textfont,0,0,1,1);
    layout3->addWidget(textfont1,0,1,1,2);
    layout3->addWidget(textcolor,1,0,1,1);
    layout3->addWidget(textcolor1,1,1,1,2);
    layout3->addWidget(textheight,2,0,1,1);
    layout3->addWidget(textheight1,2,1,1,1);
    layout3->addWidget(textheight2,2,2,1,1);
    layout3->setColumnStretch(0,10);
    layout3->setColumnStretch(1,10);
    layout3->setColumnStretch(2,10);
    layout3->setColumnStretch(3,10);

    connect(textheight1,SIGNAL(textChanged(QString)),this,SLOT(textsizechanged()));


    middle->setLayout(layout3);
}

void TextDialog::initlayout4()
{
    bottom =new QGroupBox(tr("字段"));
    QRadioButton *radio1 =new QRadioButton(tr("项目"));
    QRadioButton *radio2 =new QRadioButton(tr("图样"));
    QRadioButton *radio3 =new QRadioButton(tr("其他"));
    QTextEdit *blank =new QTextEdit(tr("备注"));
    layout4->addWidget(radio1,0,0,1,1);
    layout4->addWidget(radio2,1,0,1,1);
    layout4->addWidget(radio3,2,0,1,1);
    layout4->addWidget(blank,0,1,3,1);
    layout3->setColumnStretch(0,10);
    layout3->setColumnStretch(1,10);
    layout3->setColumnStretch(2,10);
    bottom->setLayout(layout4);
}

int TextDialog::getTextsize() const
{
    return textsize;
}

void TextDialog::setTextsize(int value)
{
    textsize = value;
}

QString TextDialog::getText() const
{
    return text;
}

void TextDialog::setText(const QString &value)
{
    text = value;
}

bool TextDialog::getOk() const
{
    return ok;
}

void TextDialog::setOk(bool value)
{
    ok = value;
}

QColor TextDialog::getTextcolor() const
{
    return textcolor;
}

void TextDialog::setTextcolor(const QColor &value)
{
    textcolor = value;
}

void TextDialog::onclickedok()
{
    this->setOk(true);
    this->accept();
}

void TextDialog::onclickedcancel()
{
    this->setOk(false);
    this->reject();
}

void TextDialog::textsizechanged()
{
    this->setTextsize(textheight1->text().toInt());
//   qDebug()<<"+++"<<textheight1->text().toInt();
}

void TextDialog::textchanged()
{
    this->setText(textcontent1->text());
    //    qDebug()<<"+++"<<textcontent1->text();
}

void TextDialog::onColorChanged(QString string,QColor color)
{
    //this->setTextcolor(textcolor1->);
    this->setTextcolor(color);
}

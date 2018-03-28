#ifndef EYELETDIALOG_H
#define EYELETDIALOG_H


#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QRadioButton>
#include <QLayout>
#include <QPushButton>
#include <QCheckBox>
#include "renderarea.h"

class EyeletDialog : public QDialog
{
    Q_OBJECT

public:

    EyeletDialog();
    void intidialog();//初始化对话框
    void creatlayout3();

    bool getOk() const;
    void setOk(bool value);

    int getPenstyle() const;
    void setPenstyle(int value);

    int getEyeletwidth() const;
    void setEyeletwidth(int value);

    int getEyeletheight() const;
    void setEyeletheight(int value);

private:
    //设置对话框的布局
    QVBoxLayout *mainLayout;//主框架
    QHBoxLayout *layout1;//水平框架
    QHBoxLayout *layout2;//放置确定取消按钮
    QGridLayout *layout3;//表格布局,用于输入框等的布局
    QGroupBox *topGroupBox;

    QLabel  *H;//鸡眼孔的高度H
    QLineEdit *H_edit;
    QLabel  *W;//梯形的宽
    QLineEdit *W_edit;

    QLabel  *line_type;//线属性
    QComboBox *line_type_choose;

    RenderArea *paint_place;//显示区域

    int eyeletwidth =100;//鸡眼孔的宽
    int eyeletheight =100;//鸡眼孔的高
    int penstyle = 0;//线条类型
    bool ok;//是否做好选择



public slots:
    void onclickedok();
    void onclickedcancel();
    void onHchange();
    void onWchange();
    void onlineTypechange();

};

#endif // EYELETDIALOG_H


//绘制梯形的对话框;
#ifndef TRAPEZIUM_DIALOG_H
#define TRAPEZIUM_DIALOG_H

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

class TrapeziumDialog : public QDialog
{
    Q_OBJECT

public:

    TrapeziumDialog();
    void intidialog();//初始化对话框
    void creatlayout3();

    double getAlp1() const;
    void setAlp1(double value);
    double getAlp2() const;
    void setAlp2(double value);
    double getH_size() const;
    void setH_size(double value);
    double getTop_length_size() const;
    void setTop_length_size(double value);
    bool getOk() const;
    void setOk(bool value);

    int getTrapezium_dialog_type() const;
    void setTrapezium_dialog_type(int value);

    int getPenstyle() const;
    void setPenstyle(int value);

private:
    //设置对话框的布局
    QVBoxLayout *mainLayout;//主框架
    QHBoxLayout *layout1;//水平框架
    QHBoxLayout *layout2;//放置确定取消按钮
    QGridLayout *layout3;//表格布局,用于输入框等的布局
    QGroupBox *topGroupBox;

    QLabel *alpha1;//梯形的第一个角度
    QLineEdit *alpha1_edit;
    QLabel *alpha2;//梯形的第二个角度
    QLineEdit *alpha2_edit;
    QLabel  *H;//梯形的高度H
    QLineEdit *H_edit;
    QLabel  *top_length;//梯形的上底
    QLineEdit *top_length_edit;

    QCheckBox *choose_check;//选择是否插入一半

    QLabel  *line_type;//线属性
    QComboBox *line_type_choose;

    RenderArea *paint_place;//显示区域

    double alp1=30;
    double alp2=30;
    double H_size=50;
    double top_length_size=50;
    int trapezium_dialog_type=1;//没选择插入一半
    int penstyle;//线条类型
    bool ok;//是否做好选择



public slots:
    void onclickedok();
    void onclickedcancel();
    void onalpha1change();
    void onalpha2change();
    void onHchange();
    void ontop_lengthchange();
    void online_typechange();
    void onchoose_checkchange();

};


#endif // TRAPEZIUM_DIALOG_H

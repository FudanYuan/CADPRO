#ifndef POLYGONDIALOG_H
#define POLYGONDIALOG_H

#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QRadioButton>
#include <QLayout>
#include <QPushButton>

#include "renderarea.h"

class PolygonDialog : public QDialog
{
    Q_OBJECT

public:
    enum line_type{
        normal,
        upright
    };
    PolygonDialog();
    void createtopGroupBox();
    void createtop_insideGroupBox();
    void initdialog();

    int getLen_num() const;
    void setLen_num(int value);
    double getRaduii() const;
    void setRaduii(double value);
    double getAngle() const;
    void setAngle(double value);
    int getPenstyle() const;
    void setPenstyle(int &value);

    bool getOk() const;
    void setOk(bool value);

private:
    //设置对话框的布局

    QGroupBox *topGroupBox;
    QGroupBox *top_insideGroupBox;

    RenderArea *originalRenderArea;//显示区域

    QLabel *len;
    QLineEdit *len_edit;//边数
    QLabel *rad;
    QLineEdit *rad_edit;//半径
    QLabel *angel;
    QLineEdit *angel_edit;//旋转角度
    QLabel *line_type;
    QComboBox *line_type_choose;//线类型

    QRadioButton *radio1;//模式的两种选项
    QRadioButton *radio2;

    int len_num;//边数
    double raduii;//半径
    double angle;//旋转角度
    int penstyle;//线条类型
    bool ok;//确定是否选择

public slots:
    void onclickedok();
    void onclickedcancel();
    void len_numchanged();
    void radchanged();
    void angelchanged();
    void typechanged();//线类型改变的事件
    //void lenchanged(int newlen);//边数改变的事件
    //void radchanged(double newrad);//半径改变的事件
    //void anglechanged(double newangle);//旋转角度改变的事件
    void modelchanged();//显示模式改变的事件

};


#endif // POLYGONDIALOG_H

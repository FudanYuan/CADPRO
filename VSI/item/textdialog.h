#ifndef TEXTDIALOG_H
#define TEXTDIALOG_H

#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QRadioButton>
#include <QLayout>
#include <QPushButton>
#include <QCheckBox>
#include "customwidget.h"

class TextDialog:public QDialog
{
    Q_OBJECT

public:
    TextDialog();
    void initDialog();
    void initlayout3();
    void initlayout4();

    int getTextsize() const;
    void setTextsize(int value);

    QString getText() const;
    void setText(const QString &value);

    bool getOk() const;
    void setOk(bool value);

    QColor getTextcolor() const;
    void setTextcolor(const QColor &value);

private:
    //设置对话框的布局
    QVBoxLayout *mainLayout;//主框架
    QHBoxLayout *layout1;//水平框架
    QHBoxLayout *layout2;//放置确定取消按钮
    QGridLayout *layout3;//表格布局,用于输入框等的布局
    QGridLayout *layout4;//表格布局,用于输入框等的布局
    QGroupBox *middle;
    QGroupBox *bottom;
    QLineEdit *textheight1;
    QLineEdit *textcontent1;

    QString text;//文本内容
    QColor textcolor;//文本颜色
    int textsize;//字体大小
    bool ok;

public slots:
    void onclickedok();
    void onclickedcancel();
    void textsizechanged();
    void textchanged();
    void onColorChanged(QString string, QColor color);
};
#endif // TEXTDIALOG_H

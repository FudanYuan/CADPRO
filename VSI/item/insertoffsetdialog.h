#ifndef INSERTOFFSETDIALOG_H
#define INSERTOFFSETDIALOG_H

#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QDesktopWidget>
#include <QApplication>

class InsertOffsetDialog : public QDialog
{

public:
    InsertOffsetDialog();
    void intidialog();//初始化对话框

private:
    //设置对话框的布局
    QHBoxLayout *mainLayout;//主框架
    QVBoxLayout *layout1;//水平框架
    QVBoxLayout *layout2;//放置确定取消按钮

signals:

public slots:


};

#endif // INSERTOFFSETDIALOG_H

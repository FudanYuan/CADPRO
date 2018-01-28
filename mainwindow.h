#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "paintarea.h"
#include <QScrollArea>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void doNew();               // 新建文件操作
    void doOpen();              // 打开文件操作
    bool doFileSave();          // 保存文件操作
    bool doFileSaveAs();        // 文件另存为操作
    void doPrint();             // 打印
    void zoomIn();              // 放大
    void zoomOut();             // 缩小
    void zoomBack();            // 还原
    void doRotate();            // 旋转
    void doShear();             // 拉伸
    void doClear();             // 清空

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;

    bool isSaved;               // 标志文件是否保存过
    QString curFile;            // 保存当前文件的路径
    bool maybeSave();           // 是否保存文件
    bool saveFile(QString fileName); //实现文件的存储

    PaintArea *area;            // 画布(工作区)
    QScrollArea *scrollArea;    // 滚动栏
};

#endif // MAINWINDOW_H

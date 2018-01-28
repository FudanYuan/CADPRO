#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QDebug>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);

    QMenu *menu_File = ui->menuBar->addMenu(tr("文件(&F)"));
    QAction *action_new = new QAction(tr("&新建"), this);
    action_new->setShortcut(QKeySequence::New);
    action_new->setStatusTip(tr("新建一个项目"));
    menu_File->addAction(action_new);

    QAction *action_open = new QAction(tr("&打开"), this);
    action_open->setShortcut(QKeySequence::Open);
    action_open->setStatusTip(tr("打开一个已存的项目"));
    menu_File->addAction(action_open);

    QAction *action_save = new QAction(tr("&保存"), this);
    action_save->setShortcut(QKeySequence::Save);
    action_save->setStatusTip(tr("保存当前项目"));
    menu_File->addAction(action_save);

    QAction *action_saveAs = new QAction(tr("&另存为"), this);
    action_saveAs->setShortcut(QKeySequence::SaveAs);
    action_saveAs->setStatusTip(tr("另存当前项目"));
    menu_File->addAction(action_saveAs);

    QAction *action_saveAll = new QAction(tr("&保存全部"), this);
    action_saveAll->setStatusTip(tr("保存所有打开的项目"));
    menu_File->addAction(action_saveAll);
    menu_File->addSeparator();

    QAction *action_print = new QAction(tr("&打印"), this);
    action_print->setShortcut(QKeySequence::Print);
    action_print->setStatusTip(tr("打印当前视图"));
    menu_File->addAction(action_print);
    menu_File->addSeparator();

    QAction *action_close = new QAction(tr("&退出"), this);
    action_close->setShortcut(QKeySequence::Quit);
    action_close->setStatusTip(tr("退出应用程序；提示保存项目"));
    menu_File->addAction(action_close);

    QMenu *menu_Draw = ui->menuBar->addMenu(tr("绘图(&D)"));
    QAction *action_line = new QAction(tr("&线条"), this);
    action_line->setStatusTip(tr("绘制直线"));
    menu_Draw->addAction(action_line);

    QAction *action_axle = new QAction(tr("&中轴线"), this);
    action_axle->setStatusTip(tr("绘制中轴线"));
    menu_Draw->addAction(action_axle);

    QAction *action_broke_line = new QAction(tr("&曲折线"), this);
    action_broke_line->setStatusTip(tr("绘制曲折线"));
    menu_Draw->addAction(action_broke_line);

    QAction *action_circle = new QAction(tr("&圆"), this);
    action_circle->setStatusTip(tr("按中心和半径绘制圆"));
    menu_Draw->addAction(action_circle);

    QAction *action_ellipse = new QAction(tr("&椭圆"), this);
    action_ellipse->setStatusTip(tr("按中心和半径绘制椭圆"));
    menu_Draw->addAction(action_ellipse);

    QAction *action_arc = new QAction(tr("&3点圆弧"), this);
    action_arc->setStatusTip(tr("绘制3点椭圆(起点，终点，中心点)"));
    menu_Draw->addAction(action_arc);

    QAction *action_arc2 = new QAction(tr("&3点圆弧2"), this);
    action_arc2->setStatusTip(tr("绘制3点椭圆(起点，终点，中心点)"));
    menu_Draw->addAction(action_arc2);

    QAction *action_rectangle = new QAction(tr("&矩形"), this);
    action_rectangle->setStatusTip(tr("绘制矩形"));
    menu_Draw->addAction(action_rectangle);

    QAction *action_ladder = new QAction(tr("&梯形"), this);
    action_ladder->setStatusTip(tr("绘制矩形"));
    menu_Draw->addAction(action_ladder);

    QAction *action_regular_polygon = new QAction(tr("&正多边形"), this);
    action_regular_polygon->setStatusTip(tr("绘制正多边形"));
    menu_Draw->addAction(action_regular_polygon);

    QAction *action_round = new QAction(tr("&冲孔"), this);
    action_round->setStatusTip(tr("绘制冲孔"));
    menu_Draw->addAction(action_round);

    QAction *action_reference_point = new QAction(tr("&参考点"), this);
    action_reference_point->setStatusTip(tr("绘制参考点"));
    menu_Draw->addAction(action_reference_point);

    QAction *action_rounded_rectangle = new QAction(tr("&鸡眼孔"), this);
    action_rounded_rectangle->setStatusTip(tr("在当前图案上插入鸡眼孔"));
    menu_Draw->addAction(action_rounded_rectangle);

    QAction *action_direction_axis = new QAction(tr("&图案方向"), this);
    action_direction_axis->setStatusTip(tr("插入方向轴"));
    menu_Draw->addAction(action_direction_axis);

    QAction *action_plug_line = new QAction(tr("&上插线"), this);
    action_plug_line->setStatusTip(tr("创建上插线"));
    menu_Draw->addAction(action_plug_line);
    menu_Draw->addSeparator();

    QAction *action_vertical_line = new QAction(tr("&垂直线"), this);
    action_vertical_line->setStatusTip(tr("绘制垂直线"));
    menu_Draw->addAction(action_vertical_line);
    menu_Draw->addSeparator();

    QMenu *menu_image = menu_Draw->addMenu(tr("图像"));
    QAction *action_get_image = new QAction(tr("&获得"), this);
    action_get_image->setStatusTip(tr("从扫描仪获取图像"));
    menu_image->addAction(action_get_image);

    QAction *action_insert_image = new QAction(tr("&插入"), this);
    action_insert_image->setStatusTip(tr("在图样中插入图片"));
    menu_image->addAction(action_insert_image);

    QAction *action_delete_image = new QAction(tr("&删除"), this);
    action_delete_image->setStatusTip(tr("从图样中删除图片"));
    menu_image->addAction(action_delete_image);

    QAction *action_tailor_image = new QAction(tr("&裁剪"), this);
    action_tailor_image->setStatusTip(tr("裁剪图样图片"));
    menu_image->addAction(action_tailor_image);

    QAction *action_correct_size_image = new QAction(tr("&校准/划刻度"), this);
    action_correct_size_image->setStatusTip(tr("校准图像大小"));
    menu_image->addAction(action_correct_size_image);

    QAction *action_adjust_image = new QAction(tr("&图像调整"), this);
    action_adjust_image->setStatusTip(tr("校准图像大小"));
    menu_image->addAction(action_adjust_image);

    menu_Draw->addMenu(menu_image);

    QMenu *menu_Insert = ui->menuBar->addMenu(tr("插入(&I)"));
    QAction *action_offset = new QAction(tr("&偏移"), this);
    action_offset->setStatusTip(tr("偏移"));
    menu_Insert->addAction(action_offset);

    QMenu *menu_Edit = ui->menuBar->addMenu(tr("修改(&E)"));

    QAction *action_undo = new QAction(tr("&撤销"), this);
    action_undo->setShortcut(QKeySequence::Undo);
    action_undo->setStatusTip(tr("撤销上一个操作"));
    menu_Edit->addAction(action_undo);

    QAction *action_redo = new QAction(tr("&重做"), this);
    action_redo->setShortcut(QKeySequence::Redo);
    action_redo->setStatusTip(tr("重做撤销的操作"));
    menu_Edit->addAction(action_redo);

    QAction *action_clear = new QAction(tr("&清空"), this);
    action_clear->setStatusTip(tr("清空画板"));
    menu_Edit->addAction(action_clear);
    menu_Edit->addSeparator();

    QMenu *menu_transform = menu_Edit->addMenu(tr("转换"));
    QAction *action_rotate = new QAction(tr("&旋转"), this);
    action_rotate->setStatusTip(tr("旋转实体"));
    menu_transform->addAction(action_rotate);

    QAction *action_shear = new QAction(tr("&拉伸"), this);
    action_shear->setStatusTip(tr("拉伸画板"));
    menu_transform->addAction(action_shear);
    menu_Edit->addMenu(menu_transform);

    QMenu *menu_View = ui->menuBar->addMenu(tr("查看(&L)"));
    QAction *action_zoomIn = new QAction(tr("&放大"), this);
    action_zoomIn->setShortcut(QKeySequence::ZoomIn);
    action_zoomIn->setStatusTip(tr("放大"));
    menu_View->addAction(action_zoomIn);

    QAction *action_zoomOut = new QAction(tr("&缩小"), this);
    action_zoomOut->setShortcut(QKeySequence::ZoomOut);
    action_zoomOut->setStatusTip(tr("缩小"));
    menu_View->addAction(action_zoomOut);

    QAction *action_zoomBack = new QAction(tr("&还原"), this);
    action_zoomBack->setStatusTip(tr("还原"));
    menu_View->addAction(action_zoomBack);
    menu_View->addSeparator();

//    QAction *action_RotateLeft = new QAction(tr("&向左旋转90"), this);
//    action_RotateLeft->setStatusTip(tr("向左旋转90"));
//    sub_menu_Rotate->addAction(action_RotateLeft);
//    QAction *action_RotateRight = new QAction(tr("&向右旋转90"), this);
//    action_RotateRight->setStatusTip(tr("向右旋转90"));
//    sub_menu_Rotate->addAction(action_RotateRight);
//    QAction *action_RotateCustom = new QAction(tr("&自定义"), this);
//    action_RotateCustom->setStatusTip(tr("自定义旋转角度"));
//    sub_menu_Rotate->addAction(action_RotateCustom);

//    menu_View->addMenu(sub_menu_Rotate);

    connect(action_new, &QAction::triggered, this, &MainWindow::doNew);
    connect(action_open, &QAction::triggered, this, &MainWindow::doOpen);
    connect(action_save, &QAction::triggered, this, &MainWindow::doFileSave);
    connect(action_saveAs, &QAction::triggered, this, &MainWindow::doFileSaveAs);
    connect(action_print, &QAction::triggered, this, &MainWindow::doPrint);
    connect(action_zoomIn, &QAction::triggered, this, &MainWindow::zoomIn);
    connect(action_zoomOut, &QAction::triggered, this, &MainWindow::zoomOut);
    connect(action_zoomBack, &QAction::triggered, this, &MainWindow::zoomBack);
    connect(action_rotate, &QAction::triggered, this, &MainWindow::doRotate);
    connect(action_shear, &QAction::triggered, this, &MainWindow::doShear);
    connect(action_clear, &QAction::triggered, this, &MainWindow::doClear);

//    QMenu *file = menuBar()->addMenu(tr("&File"));
//    file->addAction(openAction);

//    QToolBar *toolBar = addToolBar(tr("&File"));
//    toolBar->addAction(openAction);

//    statusBar() ;

    area = new PaintArea;
    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(area); //将画布添加到 scrollArea 中
    scrollArea->widget()->setMinimumSize(800,600); //scrollArea 初始化大小设为800*600
    setCentralWidget(scrollArea); //将 scrollArea 加入到主窗口的中心区

    isSaved = false;
    curFile = tr("未命名.png");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::doNew()
{
    if(maybeSave()) {
        //DoNewDialog dlg;
        //if(dlg.exec() == QDialog::Accepted) {
            int width = 600; // dlg.getWidth();
            int height = 800; // dlg.getHeight();
            area->setImageSize(width,height);
            scrollArea->widget()->resize(width,height);
            area->setImageColor(Qt::white);
            isSaved = false;
        //}
    }
}

void MainWindow::doOpen()
{
    if(maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"), QDir::currentPath());
        if (!fileName.isEmpty()) {
            area->openImage(fileName);
            scrollArea->widget()->resize(area->getImageSize()); //获得图片的大小，然后更改 scrollArea 的大小 isSaved = true;
            curFile = fileName;
        }
    }
}

bool MainWindow::doFileSave()
{
    if(isSaved) {
        return saveFile(curFile);
    } else
        return doFileSaveAs();
}

bool MainWindow::doFileSaveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("另存为"),curFile);
    if(fileName.isEmpty()) {
        return false;
    } else
        return saveFile(fileName);
}

void MainWindow::doPrint()
{
    area->print();
}

void MainWindow::zoomIn()
{
    area->zoomIn();
    scrollArea->widget()->resize(area->getImageSize());
    //获得图片的大小，然后更改 scrollArea 的大小
}

void MainWindow::zoomOut()
{
    area->zoomOut();
    scrollArea->widget()->resize(area->getImageSize());
    //获得图片的大小，然后更改 scrollArea 的大小
}

void MainWindow::zoomBack()
{
    area->zoomBack();
}

void MainWindow::doRotate()
{
    area->doRotate(90, -1);
}

void MainWindow::doShear()
{
    area->doShear();
}

void MainWindow::doClear()
{
    area->doClear();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(maybeSave()) qApp->quit();
    else event->ignore();
}

bool MainWindow::saveFile(QString fileName)
{
    if(area->saveImage(fileName, "png")) {
        isSaved = true;

        return true;
    } else
        return false;
}

bool MainWindow::maybeSave()
{
    if(area->isModified()) {
        QMessageBox::StandardButton box;
        box = QMessageBox::warning(this,tr("保存文件"),tr("图片已经改变，是否保存?"),
        QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if(box == QMessageBox::Yes)
        {
            return doFileSave();
        }
        else if(box == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

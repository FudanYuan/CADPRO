#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mydocktitlebar.h"

#include <QDockWidget>
#include <QToolButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMdiSubWindow>
#include <QFrame>

#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QBitmap>

#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>

#include <QDebug>

#define TILE_SIZE 100

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("CADPRO");
    setWindowState(Qt::WindowMaximized);
    setMouseTracking(true);     // 开启鼠标追踪

    // 系统初始化
    initActions();      // 初始化动作
    initMenuBar();      // 初始化菜单栏
    initToolBar();      // 初始化工具栏
    initStatusBar();    // 初始化状态栏
    initDockWidget();   // 初始化窗口
    initProject();      // 初始化项目
    readConfiguration();// 初始化配置文件
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onActionFileNew()
{
    QString name_project_new = getNewProjectName();
    project_active = new Project(name_project_new);
    layer_active = project_active->getActiveLayer();
    QString name_layer_new = project_active->getLayerName(0);
    layer_active->setName(name_layer_new);
    list_project.append(project_active);

    updateLayer();

    QTreeWidgetItem *item_project = new QTreeWidgetItem(tree_project,QStringList(name_project_new));
    QTreeWidgetItem *item_layer = new QTreeWidgetItem(item_project,QStringList(name_layer_new)); //子节点1
    item_project->addChild(item_layer); //添加子节点
    item_layer->setCheckState(0, Qt::Checked);
}

void MainWindow::onActionFileOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"), QDir::currentPath());
    if (!fileName.isEmpty()) {
//        layer_active->openImage(fileName);
//        project_active->setName(fileName);

        QString name_project_new = getNewProjectName();
        project_active = new Project(name_project_new);
        QString name_layer_new = project_active->getNewLayerName();
        list_project.append(project_active);

        updateLayer();

        QTreeWidgetItem *item_project = new QTreeWidgetItem(tree_project,QStringList(name_project_new));
        QTreeWidgetItem *item_layer = new QTreeWidgetItem(item_project,QStringList(name_layer_new)); //子节点1
        item_project->addChild(item_layer); //添加子节点
        item_layer->setCheckState(0, Qt::Checked);
    }
}

bool MainWindow::onActionFileSave()
{
    if(project_active->isSaved()) {
        return saveFile(project_active->getName());
    } else
        return onActionFileSaveAs();
}

bool MainWindow::onActionFileSaveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("另存为"),project_active->getName());
    if(fileName.isEmpty()) {
        return false;
    } else
        return saveFile(fileName);
}

void MainWindow::onActionFileImportDXF()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开DXF文件"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        QFileInfo new_project = QFileInfo(fileName);
        QString name_project_new = new_project.fileName();
        project_active = new Project(name_project_new);
        list_project.append(project_active);

        project_active->dxfFileParser(fileName);
        QTreeWidgetItem *item_project = new QTreeWidgetItem(tree_project,QStringList(name_project_new));

        int count = project_active->getLayerList().length();

        for(int i=0;i<count;i++){
            QString name_layer_new = project_active->getLayerName(i);
            QTreeWidgetItem *item_layer = new QTreeWidgetItem(item_project,QStringList(name_layer_new)); //子节点1
            item_project->addChild(item_layer); //添加子节点
            item_layer->setCheckState(0, Qt::Checked);
        }
        layer_active = project_active->getLayer(0);
        project_active->setActiveLayer(layer_active);
        updateLayer();
    }
}

void MainWindow::onActionFileConfiguration()
{
    qDebug() << "系统配置中···";
}

void MainWindow::onActionDrawLine()
{
    qDebug() << "line";
    layer_active->setCurShape(Layer::Line);
//    layer_active->paint();
}

void MainWindow::onActionViewXYAxes(bool toggled)
{
    layer_active->setCurShape(Layer::None);
    // 是否显示xy轴
    QSettings settings(CONFG_FILE_PATH,QSettings::IniFormat);
    qDebug() << "xy axes about to " << toggled;
    settings.setValue("view/view_xy_axes", QVariant(toggled));
}

void MainWindow::onActionViewGrid(bool toggled)
{
    QSettings settings(CONFG_FILE_PATH,QSettings::IniFormat);
    qDebug() << "grid axes about to " << toggled;
    settings.setValue("view/view_grid", QVariant(toggled));
}

void MainWindow::onActionViewGradingRules(bool toggled)
{
    QSettings settings(CONFG_FILE_PATH,QSettings::IniFormat);
    qDebug() << "grading rules axes about to " << toggled;
    settings.setValue("view/view_grading_rules", QVariant(toggled));
}

void MainWindow::onActionViewFilledPatterns(bool toggled)
{
    QSettings settings(CONFG_FILE_PATH,QSettings::IniFormat);
    qDebug() << "filled patterns about to " << toggled;
    settings.setValue("view/view_filled_patterns", QVariant(toggled));
}

void MainWindow::onActionViewZoomWindow()
{
    qDebug() << "选择视图放大";
}

void MainWindow::onActionViewZoomAll()
{
    qDebug() << "全局最大";
}

void MainWindow::onActionViewZoomIn()
{
    view->zoomIn();
}

void MainWindow::onActionViewZoomOut()
{
    view->zoomOut();
}

void MainWindow::onActionViewZoomBack()
{
    view->zoomBack();
}

void MainWindow::onActionViewToolFindStyleToggled(bool toggled)
{
    onDockFindStyleVisibilityChanged(toggled);
    QSettings settings(CONFG_FILE_PATH,QSettings::IniFormat);
    qDebug() << "find style about to " << toggled;
    settings.setValue("view/view_tool_find_style", QVariant(toggled));
}

void MainWindow::onActionViewToolProjectToggled(bool toggled)
{
    onDockProjectVisibilityChanged(toggled);
    QSettings settings(CONFG_FILE_PATH,QSettings::IniFormat);
    qDebug() << "project about to " << toggled;
    settings.setValue("view/view_tool_project", QVariant(toggled));
}

void MainWindow::onActionViewToolPropertiesToggled(bool toggled)
{
    onDockPropertiesVisibilityChanged(toggled);
    QSettings settings(CONFG_FILE_PATH,QSettings::IniFormat);
    qDebug() << "propertities about to " << toggled;
    settings.setValue("view/view_tool_properties", QVariant(toggled));
}

void MainWindow::doPrint()
{
//    layer_active->print();
}

void MainWindow::zoomIn()
{
//    layer_active->zoomIn();
}

void MainWindow::zoomOut()
{
//    layer_active->zoomOut();
}

void MainWindow::zoomBack()
{
//    layer_active->zoomBack();
}

void MainWindow::doRotate()
{
//    layer_active->doRotate(90, -1);
}

void MainWindow::slideFindStyleToggle()
{
    dock_find_style->setVisible(!dock_find_style->isVisible());
    onToolSlideChanged();
}

void MainWindow::slideProjectToggle()
{
    dock_project->setVisible(!dock_project->isVisible());
    onToolSlideChanged();
}

void MainWindow::slidePropertiesToggle()
{
    dock_properties->setVisible(!dock_properties->isVisible());
    onToolSlideChanged();
}

void MainWindow::onActionTreeExpandAll()
{
    tree_project->expandAll();
}

void MainWindow::onActionTreeFoldAll()
{
    tree_project->collapseAll();
}

void MainWindow::onTreeProjectItemDoubleClicked(QTreeWidgetItem *item, int i)
{
    QTreeWidgetItem *parent=item->parent();//获得父节点
    if(NULL == parent) {
        // 根据项目名称获取项目指针
        QString project_name = item->text(0);
        project_active = getProjectByName(project_name);
        layer_active = project_active->getLayer(0);
    } else{
        QString project_name = parent->text(0);
        project_active = getProjectByName(project_name);
        layer_active = getLayerByName(project_active, item->text(0));
        project_active->setActiveLayer(layer_active);
    }
    updateLayer();
}

void MainWindow::onActionTreeProjectAddLayer()
{
    // 根据项目名称获取项目指针
    QString project_name = tree_project_active_item->text(0);
    project_active = getProjectByName(project_name);
    layer_active = project_active->addLayer();
    QString name_layer_new = project_active->getLayerName(layer_active);

    QTreeWidgetItem *item_layer = new QTreeWidgetItem(tree_project_active_item,QStringList(name_layer_new)); //子节点1
    tree_project_active_item->addChild(item_layer);
    item_layer->setCheckState(0, Qt::Checked);

    updateLayer();
}

void MainWindow::onActionTreeProjectSave()
{
    qDebug() << tree_project_active_item->text(0) << "保存项目";
}

void MainWindow::onActionTreeProjectSaveAs()
{
    qDebug() << tree_project_active_item->text(0) << "另存为项目";
}

void MainWindow::onActionTreeProjectRename()
{
    // 根据项目名称获取项目指针
    Project *project = getProjectByName(tree_project_active_item->text(0));
    // 获取新的项目名称
    bool ok;
    QString text = QInputDialog::getText(this, tr("项目重命名"),
                                         tr("项目名称:"), QLineEdit::Normal,
                                         project->getName(), &ok);
    if (ok && !text.isEmpty()){
        tree_project_active_item->setText(0, text);
        project->setName(text);
    }
}

void MainWindow::onActionTreeProjectClose()
{
    qDebug() << tree_project_active_item->text(0) << "关闭项目";
}

void MainWindow::onActionTreeProjectLayerChangeTo()
{
    qDebug() << tree_project_layer_active_item->text(0) << "切换至";
}

void MainWindow::onActionTreeProjectLayerMoveUpOne()
{
    qDebug() << tree_project_layer_active_item->text(0) << "上移一层";
}

void MainWindow::onActionTreeProjectLayerMoveUpTop()
{
    qDebug() << tree_project_layer_active_item->text(0) << "上移至顶";
}

void MainWindow::onActionTreeProjectLayerMoveDownOne()
{
    qDebug() << tree_project_layer_active_item->text(0) << "下移一层";
}

void MainWindow::onActionTreeProjectLayerMoveDownBottom()
{
    qDebug() << tree_project_layer_active_item->text(0) << "下移至底";
}

void MainWindow::onActionTreeProjectLayerRename()
{
    // 根据图层名称获取图层指针
    Project *project = getProjectByName(tree_project_active_item->text(0));
    Layer *layer = getLayerByName(project, tree_project_layer_active_item->text(0));

    // 获取新的图层名称
    bool ok;
    QString text = QInputDialog::getText(this, tr("图层重命名"),
                                         tr("新图层名称:"), QLineEdit::Normal,
                                         layer->getName(), &ok);
    if (ok && !text.isEmpty()){
        tree_project_layer_active_item->setText(0, text);
        layer->setName(text);
    }
}

void MainWindow::onActionTreeProjectLayerDelete()
{
    qDebug() << tree_project_layer_active_item->text(0) << "删除";
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(maybeSave()) qApp->quit();
    else event->ignore();
}

void MainWindow::updateLayer()
{
    setWindowTitle("CADPRO-<" + project_active->getName() + "-" + layer_active->getName() + ">");
    dock_paint_area->setWidget(project_active->getActiveLayer());
}

bool MainWindow::saveFile(QString fileName)
{
//    if(layer_active->saveImage(fileName, "png")) {
//        project_active->setSaved(true);
//        return true;
//    } else
//        return false;
}

void MainWindow::initActions()
{
// ![1] 文件
    action_file_new = new QAction(tr("&新建"), this);
    action_file_new->setShortcut(QKeySequence::New);
    action_file_new->setStatusTip(tr("新建一个项目"));
    connect(action_file_new, &QAction::triggered, this, &MainWindow::onActionFileNew);

    action_file_open = new QAction(tr("&打开"), this);
    action_file_open->setShortcut(QKeySequence::Open);
    action_file_open->setStatusTip(tr("打开一个已存的项目"));
    connect(action_file_open, &QAction::triggered, this, &MainWindow::onActionFileOpen);

    action_file_save = new QAction(tr("&保存"), this);
    action_file_save->setShortcut(QKeySequence::Save);
    action_file_save->setStatusTip(tr("保存当前项目"));

    action_file_save_as = new QAction(tr("&另存为"), this);
    action_file_save_as->setShortcut(QKeySequence::SaveAs);
    action_file_save_as->setStatusTip(tr("另存当前项目"));

    action_file_save_all = new QAction(tr("&保存全部"), this);
    action_file_save_all->setStatusTip(tr("保存所有打开的项目"));

    action_file_print = new QAction(tr("&打印"), this);
    action_file_print->setShortcut(QKeySequence::Print);
    action_file_print->setStatusTip(tr("打印当前视图"));

    action_file_print_preview = new QAction(tr("&打印预览"), this);
    action_file_print_preview->setStatusTip(tr("显示当前项目打印预览"));
    action_file_print_preview->setDisabled(true);

    action_file_print_setup = new QAction(tr("&打印设置"), this);
    action_file_print_setup->setStatusTip(tr("更改打印机和打印选项"));
    action_file_print_setup->setDisabled(true);

    action_file_import_DXF = new QAction(tr("&DXF"), this);
    action_file_import_DXF->setStatusTip(tr("导入DXF文件"));
    connect(action_file_import_DXF, &QAction::triggered, this, &MainWindow::onActionFileImportDXF);

    action_file_import_DVS = new QAction(tr("&DVS"), this);
    action_file_import_DVS->setStatusTip(tr("导入DVS文件"));

    action_file_import_TEF = new QAction(tr("&TEF"), this);
    action_file_import_TEF->setStatusTip(tr("导入TEF文件"));

    action_file_export_DXF = new QAction(tr("&DXF"), this);
    action_file_export_DXF->setStatusTip(tr("导出DXF文件"));

    action_file_export_DVS = new QAction(tr("&DVS"), this);
    action_file_export_DVS->setStatusTip(tr("导出DVS文件"));

    action_file_export_TEF = new QAction(tr("&TEF"), this);
    action_file_export_TEF->setStatusTip(tr("导出TEF文件"));

    action_file_configuration = new QAction(tr("&配置"), this);
    action_file_configuration->setStatusTip(tr("修改cadPro配置"));
    connect(action_file_configuration, &QAction::triggered, this, &MainWindow::onActionFileConfiguration);

    action_file_exit = new QAction(tr("&退出"), this);
    action_file_exit->setShortcut(QKeySequence::Quit);
    action_file_exit->setStatusTip(tr("退出应用程序；提示保存项目"));

//    connect(action_file_save, &QAction::triggered, this, &MainWindow::onActionFileSave);
//    connect(action_file_save_as, &QAction::triggered, this, &MainWindow::onActionFileSaveAs);
//    connect(action_file_print, &QAction::triggered, this, &MainWindow::doPrint);
//    connect(action_view_zoom_in, &QAction::triggered, this, &MainWindow::zoomIn);
//    connect(action_view_zoom_out, &QAction::triggered, this, &MainWindow::zoomOut);
//    connect(action_view_zoom_back, &QAction::triggered, this, &MainWindow::zoomBack);
//    connect(action_modify_transform_rotate, &QAction::triggered, this, &MainWindow::doRotate);
//    connect(action_modify_clear, &QAction::triggered, this, &MainWindow::doClear);

// ![1] 文件

// ![2] 绘图
    action_draw_line = new QAction(tr("&线条"), this);
    action_draw_line->setStatusTip(tr("绘制直线"));
    connect(action_draw_line, &QAction::triggered, this, &MainWindow::onActionDrawLine);

    action_draw_middle_axis = new QAction(tr("&中轴线"), this);
    action_draw_middle_axis->setStatusTip(tr("绘制中轴线"));

    action_draw_polyline = new QAction(tr("&曲折线"), this);
    action_draw_polyline->setStatusTip(tr("绘制曲折线"));

    action_draw_circle = new QAction(tr("&圆"), this);
    action_draw_circle->setStatusTip(tr("按中心和半径绘制圆"));

    action_draw_ellipse = new QAction(tr("&椭圆"), this);
    action_draw_ellipse->setStatusTip(tr("按中心和半径绘制椭圆"));

    action_draw_arc_by_3_points = new QAction(tr("&3点圆弧"), this);
    action_draw_arc_by_3_points->setStatusTip(tr("绘制3点椭圆(起点，终点，中心点)"));

    action_draw_arc_by_3_points_2 = new QAction(tr("&3点圆弧2"), this);
    action_draw_arc_by_3_points_2->setStatusTip(tr("绘制3点椭圆(起点，终点，中心点)"));

    action_draw_rectangle = new QAction(tr("&矩形"), this);
    action_draw_rectangle->setStatusTip(tr("绘制矩形"));

    action_draw_trapezium = new QAction(tr("&梯形"), this);
    action_draw_trapezium->setStatusTip(tr("绘制矩形"));

    action_draw_polygon = new QAction(tr("&正多边形"), this);
    action_draw_polygon->setStatusTip(tr("绘制正多边形"));

    action_draw_stab_hole = new QAction(tr("&冲孔"), this);
    action_draw_stab_hole->setStatusTip(tr("绘制冲孔"));

    action_draw_reference = new QAction(tr("&参考点"), this);
    action_draw_reference->setStatusTip(tr("绘制参考点"));

    action_draw_eyelet = new QAction(tr("&鸡眼孔"), this);
    action_draw_eyelet->setStatusTip(tr("在当前图案上插入鸡眼孔"));

    action_draw_pattern_direction = new QAction(tr("&图案方向"), this);
    action_draw_pattern_direction->setStatusTip(tr("插入方向轴"));

    action_draw_shank_line = new QAction(tr("&上插线"), this);
    action_draw_shank_line->setStatusTip(tr("创建上插线"));

    action_draw_perpendicular = new QAction(tr("&垂直线"), this);
    action_draw_perpendicular->setStatusTip(tr("绘制垂直线"));
    action_draw_perpendicular->setDisabled(true);

    action_draw_image = new QAction(tr("&获得"), this);
    action_draw_image->setStatusTip(tr("从扫描仪获取图像"));

    action_draw_image_insert = new QAction(tr("&插入"), this);
    action_draw_image_insert->setStatusTip(tr("在图样中插入图片"));

    action_draw_image_delete = new QAction(tr("&删除"), this);
    action_draw_image_delete->setStatusTip(tr("从图样中删除图片"));
    action_draw_image_delete->setDisabled(true);

    action_draw_image_crop = new QAction(tr("&裁剪"), this);
    action_draw_image_crop->setStatusTip(tr("裁剪图样图片"));
    action_draw_image_crop->setDisabled(true);

    action_draw_image_calibrate = new QAction(tr("&校准/划刻度"), this);
    action_draw_image_calibrate->setStatusTip(tr("校准图像大小"));
    action_draw_image_calibrate->setDisabled(true);

    action_draw_image_agjustement = new QAction(tr("&图像调整"), this);
    action_draw_image_agjustement->setStatusTip(tr("校准图像大小"));
    action_draw_image_agjustement->setDisabled(true);

    action_draw_digitizer_activate = new QAction(tr("&启用"), this);
    action_draw_digitizer_activate->setStatusTip(tr("启用/禁用数字化仪"));
    action_draw_digitizer_activate->setDisabled(true);

    action_draw_digitizer_set_position = new QAction(tr("&设置shell位置"), this);
    action_draw_digitizer_set_position->setStatusTip(tr("设置shell位置"));
    action_draw_digitizer_set_position->setDisabled(true);

    action_draw_digitizer_configure = new QAction(tr("&配置"), this);
    action_draw_digitizer_configure->setStatusTip(tr("配置数字化仪大小"));
    action_draw_digitizer_configure->setDisabled(true);

    action_draw_scanner_vectorize_from_scanner = new QAction(tr("&扫描矢量"), this);
    action_draw_scanner_vectorize_from_scanner->setStatusTip(tr("扫描和矢量化一个图样"));

    action_draw_scanner_select_source = new QAction(tr("&选择源"), this);
    action_draw_scanner_select_source->setStatusTip(tr("选择扫描源"));

    action_draw_scanner_vectorize_image = new QAction(tr("&矢量图像"), this);
    action_draw_scanner_vectorize_image->setStatusTip(tr("矢量化扫描图像"));
// ![2] 绘图

// ![3] 插入
    action_insert_offset = new QAction(tr("&偏移"), this);
    action_insert_offset->setStatusTip(tr("创建偏移"));
    action_insert_offset->setDisabled(true);

    action_insert_advanced_offset = new QAction(tr("&高级偏移"), this);
    action_insert_advanced_offset->setStatusTip(tr("创建高级偏移"));
    action_insert_advanced_offset->setDisabled(true);

    action_insert_text = new QAction(tr("&文本"), this);
    action_insert_text->setStatusTip(tr("插入文本"));

    action_insert_fillet = new QAction(tr("&倒圆角"), this);
    action_insert_fillet->setStatusTip(tr("倒圆角两条线"));
    action_insert_fillet->setDisabled(true);

    action_insert_notch = new QAction(tr("&凹槽"), this);
    action_insert_notch->setStatusTip(tr("插入凹槽"));
    action_insert_notch->setDisabled(true);

    action_insert_notches_group = new QAction(tr("&凹槽组"), this);
    action_insert_notches_group->setStatusTip(tr("插入凹槽"));
    action_insert_notches_group->setDisabled(true);

    action_insert_notches_axis_tied_group = new QAction(tr("&凹槽轴绑定群组"), this);
    action_insert_notches_axis_tied_group->setStatusTip(tr("插入一个与轴绑定的凹槽群组"));
    action_insert_notches_axis_tied_group->setDisabled(true);


    action_insert_size_notch = new QAction(tr("&凹槽尺寸编号"), this);
    action_insert_size_notch->setStatusTip(tr("插入凹槽尺寸编号"));
    action_insert_size_notch->setDisabled(true);

    action_insert_remark = new QAction(tr("&备注"), this);
    action_insert_remark->setStatusTip(tr("插入备注"));
    action_insert_remark->setDisabled(true);

    action_insert_smooth = new QAction(tr("&平滑"), this);
    action_insert_smooth->setStatusTip(tr("角度平滑"));
    action_insert_smooth->setDisabled(true);

    action_insert_margin = new QAction(tr("&边距/余量"), this);
    action_insert_margin->setStatusTip(tr("插入边距或余量"));
    action_insert_margin->setDisabled(true);

    action_insert_canal = new QAction(tr("&管道"), this);
    action_insert_canal->setStatusTip(tr("插入管道"));
    action_insert_canal->setDisabled(true);

    action_insert_block = new QAction(tr("&块"), this);
    action_insert_block->setStatusTip(tr("在当前图样插入块"));
    action_insert_block->setDisabled(true);

    action_insert_holes_over_a_line = new QAction(tr("&沿线孔"), this);
    action_insert_holes_over_a_line->setStatusTip(tr("插入沿线孔"));
    action_insert_holes_over_a_line->setDisabled(true);

    action_insert_array_rectangular = new QAction(tr("&矩形"), this);
    action_insert_array_rectangular->setStatusTip(tr("创建矩形阵列"));
    action_insert_array_rectangular->setDisabled(true);

    action_insert_array_circular = new QAction(tr("&圆形"), this);
    action_insert_array_circular->setStatusTip(tr("创建圆形阵列"));
    action_insert_array_circular->setDisabled(true);
// ![3] 插入

// ![4] 修改
    action_modify_undo = new QAction(tr("&撤销"), this);
    action_modify_undo->setShortcut(QKeySequence::Undo);
    action_modify_undo->setStatusTip(tr("撤销上一个操作"));
    action_modify_undo->setDisabled(true);

    action_modify_redo = new QAction(tr("&重做"), this);
    action_modify_redo->setShortcut(QKeySequence::Redo);
    action_modify_redo->setDisabled(true);
    action_modify_redo->setStatusTip(tr("重做撤销的操作"));

    action_modify_clear = new QAction(tr("&清空"), this);
    action_modify_clear->setStatusTip(tr("清空画板"));
    action_modify_clear->setDisabled(true);

    action_modify_delete = new QAction(tr("&删除"), this);
    action_modify_delete->setShortcut(QKeySequence::Delete);
    action_modify_delete->setStatusTip(tr("删除实体"));
    action_modify_delete->setDisabled(true);

    action_modify_cut = new QAction(tr("&剪切"), this);
    action_modify_cut->setShortcut(QKeySequence::Cut);
    action_modify_cut->setStatusTip(tr("剪切选择并将其放在剪贴板上"));
    action_modify_cut->setDisabled(true);

    action_modify_copy = new QAction(tr("&复制"), this);
    action_modify_copy->setShortcut(QKeySequence::Copy);
    action_modify_copy->setStatusTip(tr("将所选线复制到粘贴板"));
    action_modify_copy->setDisabled(true);

    action_modify_paste = new QAction(tr("&粘贴"), this);
    action_modify_paste->setShortcut(QKeySequence::Paste);
    action_modify_paste->setStatusTip(tr("粘贴先前复制的实体"));
    action_modify_paste->setDisabled(true);

    action_modify_paste_as_slave = new QAction(tr("&粘贴为副本"), this);
    action_modify_paste_as_slave->setStatusTip(tr("粘贴为副本"));
    action_modify_paste_as_slave->setDisabled(true);

    action_modify_transform_move = new QAction(tr("&移动"), this);
    action_modify_transform_move->setDisabled(true);

    action_modify_transform_rotate = new QAction(tr("&旋转"), this);
    action_modify_transform_rotate->setStatusTip(tr("旋转实体"));
    action_modify_transform_rotate->setDisabled(true);

    action_modify_transform_rotate_knots_continuely = new QAction(tr("&连续旋转节点"), this);
    action_modify_transform_rotate_knots_continuely->setDisabled(true);

    action_modify_transform_rotate_knots = new QAction(tr("&旋转节点"), this);
    action_modify_transform_rotate_knots->setDisabled(true);

    action_modify_transform_scale = new QAction(tr("&比例"), this);
    action_modify_transform_scale->setDisabled(true);

    action_modify_transform_mirror = new QAction(tr("&镜像"), this);
    action_modify_transform_mirror->setDisabled(true);

    action_modify_transform_strech = new QAction(tr("&拉伸"), this);
    action_modify_transform_strech->setDisabled(true);

    action_modify_transform_explode = new QAction(tr("&爆炸"), this);
    action_modify_transform_explode->setDisabled(true);

    action_modify_transform_join = new QAction(tr("&联接"), this);
    action_modify_transform_join->setDisabled(true);

    action_modify_transform_break = new QAction(tr("&断开"), this);
    action_modify_transform_break->setDisabled(true);

    action_modify_transform_fillet = new QAction(tr("&倒圆角"), this);
    action_modify_transform_fillet->setDisabled(true);

    action_modify_transform_V_cut = new QAction(tr("&'V'切"), this);
    action_modify_transform_V_cut->setDisabled(true);

    action_modify_transform_spline = new QAction(tr("&创建样条"), this);
    action_modify_transform_spline->setDisabled(true);

//    action_shear = new QAction(tr("&拉伸"), this);
//    action_shear->setStatusTip(tr("拉伸画板"));

    action_modify_trim = new QAction(tr("&修剪"), this);
    action_modify_trim->setDisabled(true);

    action_modify_extend = new QAction(tr("&延伸"), this);
    action_modify_extend->setDisabled(true);

    action_modify_modify_endpoint = new QAction(tr("&修改端点"), this);
    action_modify_modify_endpoint->setDisabled(true);

    action_modify_adjust_polyline = new QAction(tr("&调整折线"), this);
    action_modify_adjust_polyline->setDisabled(true);

    action_modify_start_point = new QAction(tr("&起点"), this);
    action_modify_start_point->setDisabled(true);

    action_modify_soften_up = new QAction(tr("&软化"), this);
    action_modify_soften_up->setDisabled(true);

    action_modify_warp = new QAction(tr("&弯曲"), this);
    action_modify_warp->setDisabled(true);

    action_modify_knots_reduction = new QAction(tr("&减少节点"), this);
    action_modify_knots_reduction->setDisabled(true);

    action_modify_replace_part = new QAction(tr("&更换部分"), this);
    action_modify_replace_part->setDisabled(true);

    action_modify_change_to = new QAction(tr("&更改至"), this);
    action_modify_change_to->setDisabled(true);

    action_modify_make_master_line = new QAction(tr("&制作主线"), this);

    action_modify_style = new QAction(tr("&实体线样式"), this);
    action_modify_style->setDisabled(true);

    action_modify_springing = new QAction(tr("&Springing弹回"), this);
// ![4] 修改

// ![5] 图样
    action_pattern_select = new QAction(tr("&选择"), this);
    action_pattern_select->setDisabled(true);

    action_pattern_new = new QAction(tr("&新建"), this);

    action_pattern_clone = new QAction(tr("&克隆"), this);

    action_pattern_delete = new QAction(tr("&删除"), this);
    action_pattern_delete->setDisabled(true);

    action_pattern_rotate = new QAction(tr("&旋转"), this);

    action_pattern_make_symmetic = new QAction(tr("&使对称"), this);
    action_pattern_make_symmetic->setDisabled(true);

    action_pattern_position = new QAction(tr("&位置"), this);
    action_pattern_position->setDisabled(true);

    action_pattern_extract_auto = new QAction(tr("&提取(自动)"), this);
    action_pattern_extract_auto->setDisabled(true);

    action_pattern_extract_manual = new QAction(tr("&提取(手动)"), this);
    action_pattern_extract_manual->setDisabled(true);

    action_pattern_find_perimeter = new QAction(tr("&查找周长"), this);
    action_pattern_find_perimeter->setDisabled(true);

    action_pattern_close_trim = new QAction(tr("&关闭和修剪"), this);
    action_pattern_close_trim->setDisabled(true);

    action_pattern_check = new QAction(tr("&检查"), this);
    action_pattern_check->setDisabled(true);

    action_pattern_devide = new QAction(tr("&分裂/瓜分"), this);
    action_pattern_devide->setDisabled(true);

    action_pattern_save_as_weft = new QAction(tr("&另存为纬线"), this);
    action_pattern_save_as_weft->setDisabled(true);
// ![5] 图样

// ![6] 查看
    action_view_xy_axes = new QAction(tr("&XY轴"), this);
    action_view_xy_axes->setCheckable(true);
    action_view_xy_axes->setStatusTip(tr("显示/隐藏xy轴"));
    connect(action_view_xy_axes, &QAction::toggled, this, &MainWindow::onActionViewXYAxes);

    action_view_grid = new QAction(tr("&网格"), this);
    action_view_grid->setCheckable(true);
    action_view_grid->setStatusTip(tr("显示/隐藏网格"));
    connect(action_view_grid, &QAction::toggled, this, &MainWindow::onActionViewGrid);

    action_view_knots = new QAction(tr("&节点"), this);
    action_view_knots->setCheckable(true);
    action_view_knots->setStatusTip(tr("显示/隐藏节点"));
    action_view_knots->setDisabled(true);

    action_view_image = new QAction(tr("&图片"), this);
    action_view_image->setCheckable(true);
    action_view_image->setStatusTip(tr("显示/隐藏图片"));
    action_view_image->setDisabled(true);

    action_view_design_rules = new QAction(tr("&设计尺"), this);
    action_view_design_rules->setCheckable(true);
    action_view_design_rules->setStatusTip(tr("显示/隐藏绘图规则"));
    action_view_design_rules->setDisabled(true);

    action_view_grading_rules = new QAction(tr("&级放规则"), this);
    action_view_grading_rules->setCheckable(true);
    action_view_grading_rules->setStatusTip(tr("显示/隐藏级放规则"));
    connect(action_view_grading_rules, &QAction::toggled, this, &MainWindow::onActionViewGradingRules);

    action_view_filled_patterns = new QAction(tr("&填充图样"), this);
    action_view_filled_patterns->setCheckable(true);
    action_view_filled_patterns->setStatusTip(tr("显示/隐藏填充图样"));
    connect(action_view_filled_patterns, &QAction::toggled, this, &MainWindow::onActionViewFilledPatterns);

    action_view_zoom_window = new QAction(tr("&缩放窗口"), this);
    action_view_zoom_window->setStatusTip(tr("&缩放窗口"));
    connect(action_view_zoom_window, &QAction::triggered, this, &MainWindow::onActionViewZoomWindow);

    action_view_zoom_all = new QAction(tr("&全部缩放"), this);
    action_view_zoom_all->setStatusTip(tr("&全部缩放"));
    connect(action_view_zoom_all, &QAction::triggered, this, &MainWindow::onActionViewZoomAll);

    action_view_zoom_in = new QAction(tr("&放大"), this);
    action_view_zoom_in->setShortcut(QKeySequence::ZoomIn);
    action_view_zoom_in->setStatusTip(tr("放大"));
    connect(action_view_zoom_in, &QAction::triggered, this, &MainWindow::onActionViewZoomIn);

    action_view_zoom_out = new QAction(tr("&缩小"), this);
    action_view_zoom_out->setShortcut(QKeySequence::ZoomOut);
    action_view_zoom_out->setStatusTip(tr("缩小"));
    connect(action_view_zoom_out, &QAction::triggered, this, &MainWindow::onActionViewZoomOut);

    action_view_zoom_back = new QAction(tr("&还原"), this);
    action_view_zoom_back->setStatusTip(tr("还原"));

    action_view_voerlap_patterns = new QAction(tr("&覆盖图样"), this);

    action_view_hide = new QAction(tr("&隐藏"), this);

    action_view_show = new QAction(tr("&显示"), this);
    action_view_show->setDisabled(true);

    action_view_clean = new QAction(tr("&清除"), this);
    action_view_clean->setDisabled(true);

    action_view_set_reference = new QAction(tr("&设置引用参考"), this);
    action_view_set_reference->setDisabled(true);

    action_view_lock_layout = new QAction(tr("&锁定布局"), this);
    action_view_lock_layout->setCheckable(true);

    action_view_tool_find_style = new QAction(tr("&寻找款型"), this);
    action_view_tool_find_style->setCheckable(true);
    connect(action_view_tool_find_style, &QAction::toggled, this, &MainWindow::onActionViewToolFindStyleToggled);

    action_view_tool_project = new QAction(tr("&项目"), this);
    action_view_tool_project->setCheckable(true);
    connect(action_view_tool_project, &QAction::toggled, this, &MainWindow::onActionViewToolProjectToggled);

    action_view_tool_properties = new QAction(tr("&属性"), this);
    action_view_tool_properties->setCheckable(true);
    connect(action_view_tool_properties, &QAction::toggled, this, &MainWindow::onActionViewToolPropertiesToggled);

    action_view_tool_slide = new QActionGroup(this);
    action_view_tool_slide->setExclusive(false);

    action_view_tool_slide->addAction(action_view_tool_find_style);
    action_view_tool_slide->addAction(action_view_tool_project);
    action_view_tool_slide->addAction(action_view_tool_properties);

    connect(action_view_lock_layout, &QAction::toggled, action_view_tool_slide, &QActionGroup::setDisabled);

    action_view_tool_customize = new QAction(tr("&自定义"));

    action_view_status_bar = new QAction(tr("&状态栏"), this);
    action_view_status_bar->setCheckable(true);
    action_view_status_bar->setChecked(true);
// ![6] 查看

// ![7] 效用
    action_utility_measure_distance = new QAction(tr("&测量距离"), this);

    action_utility_measure_line = new QAction(tr("&测量线长"), this);

    action_create_pattern_card = new QAction(tr("&创建图样卡"), this);
    action_create_pattern_card->setDisabled(true);

    action_utility_quick_nesting = new QAction(tr("&快速排版"), this);

    action_utility_nest_by_job_card = new QAction(tr("&转到排版"), this);

    action_utility_heel_cover = new QAction(tr("&后跟..."), this);

    action_utility_pattern_search = new QAction(tr("&搜索图样"), this);
    action_utility_pattern_search->setDisabled(true);
// ![7] 效用

// ![8] 级放
    action_grading_parameters = new QAction(tr("&图样"), this);

    action_grading_grade = new QAction(tr("&级放"), this);
    action_grading_grade->setDisabled(true);

    action_grading_delete_graded_patterns = new QAction(tr("&删除级放图样"), this);
    action_grading_delete_graded_patterns->setDisabled(true);

    action_grading_reference_insert = new QAction(tr("插入"), this);
    action_grading_reference_insert->setDisabled(true);

    action_grading_keep_margin = new QAction(tr("&保留 边距/余量"), this);
    action_grading_keep_margin->setDisabled(true);

    action_grading_delete_margin = new QAction(tr("&删除 边距/余量"), this);
    action_grading_delete_margin->setDisabled(true);

    action_grading_update_grading = new QAction(tr("&更新级放"), this);
    action_grading_update_grading->setDisabled(true);

    action_grading_XY_lock = new QAction(tr("&XY 锁定"), this);
    action_grading_XY_lock->setDisabled(true);

    action_grading_delete_XY_lock = new QAction(tr("&删除XY 锁定 "), this);
    action_grading_delete_XY_lock->setDisabled(true);

    action_grading_horizontal_zone = new QAction(tr("&水平区"), this);

    action_grading_vertical_zone = new QAction(tr("&垂直区"), this);

    action_grading_delete_zone = new QAction(tr("&删除区域"), this);

// ![8] 级放

// ![10] 帮助
    action_help_help = new QAction(tr("&帮助(&H)"), this);
    action_help_help->setShortcut(QKeySequence::HelpContents);
    action_help_license = new QAction(tr("&许可(&L)"), this);
    action_help_about = new QAction(tr("&关于"), this);
// ![10] 帮助

// ![11] 边栏
    action_tool_slide = new QActionGroup(this);

    action_tool_slide_find_style = new QAction(tr("&寻找款型"), this);
    connect(action_tool_slide_find_style, &QAction::triggered, this, &MainWindow::slideFindStyleToggle);

    action_tool_slide_project = new QAction(tr("&项目"), this);
    connect(action_tool_slide_project, &QAction::triggered, this, &MainWindow::slideProjectToggle);

    action_tool_slide_properties = new QAction(tr("&属性"), this);
    connect(action_tool_slide_properties, &QAction::triggered, this, &MainWindow::slidePropertiesToggle);

    action_tool_slide->addAction(action_tool_slide_find_style);
    action_tool_slide->addAction(action_tool_slide_project);
    action_tool_slide->addAction(action_tool_slide_properties);

    action_tool_slide->setVisible(false);
// ![11] 边栏

// ![12] 对象捕捉
    action_object_snap_end_point = new QAction(tr("&实体端点"));
    action_object_snap_end_point->setCheckable(true);
    action_object_snap_end_point->setChecked(false);
    action_object_snap_end_point->setStatusTip(tr("实体端点"));

    action_object_snap_center = new QAction(tr("&实体中心"));
    action_object_snap_center->setCheckable(true);
    action_object_snap_center->setChecked(false);
    action_object_snap_center->setStatusTip(tr("实体中心"));

    action_object_snap_intersection = new QAction(tr("&交叉点"));
    action_object_snap_intersection->setCheckable(true);
    action_object_snap_intersection->setChecked(false);
    action_object_snap_intersection->setStatusTip(tr("交叉点"));

    action_object_snap_knot = new QAction(tr("&节点"));
    action_object_snap_knot->setCheckable(true);
    action_object_snap_knot->setChecked(false);
    action_object_snap_knot->setStatusTip(tr("节点"));

    action_object_snap_ortho = new QAction(tr("&直角正交"));
    action_object_snap_ortho->setCheckable(true);
    action_object_snap_ortho->setChecked(false);
    action_object_snap_ortho->setStatusTip(tr("直角正交"));

    action_object_snap_enter_coordinates = new QAction(tr("&输入坐标"));
    action_object_snap_enter_coordinates->setCheckable(true);
    action_object_snap_enter_coordinates->setChecked(false);
    action_object_snap_enter_coordinates->setStatusTip(tr("输入坐标"));
// ![12] 对象捕捉
}

void MainWindow::initMenuBar()
{
// ![1] 文件栏
    menu_file = ui->menuBar->addMenu(tr("文件(&F)"));
    menu_file->addAction(action_file_new);
    menu_file->addAction(action_file_open);
    menu_file->addAction(action_file_save);
    menu_file->addAction(action_file_save_as);
    menu_file->addAction(action_file_save_all);
    menu_file->addSeparator();
    menu_file->addAction(action_file_print);
    menu_file->addAction(action_file_print_preview);
    menu_file->addAction(action_file_print_setup);
    menu_file->addSeparator();
    menu_file_import = menu_file->addMenu(tr("导入"));
    menu_file_import->addAction(action_file_import_DXF);
    menu_file_import->addAction(action_file_import_DVS);
    menu_file_import->addAction(action_file_import_TEF);
    menu_file_export = menu_file->addMenu(tr("导出"));
    menu_file_export->addAction(action_file_export_DXF);
    menu_file_export->addAction(action_file_export_DVS);
    menu_file_export->addAction(action_file_export_TEF);
    menu_file->addSeparator();
    menu_file->addAction(action_file_configuration);
    menu_file->addAction(action_file_exit);
// ![1] 文件栏

// ![2] 绘图栏
    menu_draw = ui->menuBar->addMenu(tr("绘图(&D)"));
    menu_draw->addAction(action_draw_line);
    menu_draw->addAction(action_draw_middle_axis);
    menu_draw->addAction(action_draw_polyline);
    menu_draw->addAction(action_draw_circle);
    menu_draw->addAction(action_draw_ellipse);
    menu_draw->addAction(action_draw_arc_by_3_points);
    menu_draw->addAction(action_draw_arc_by_3_points_2);
    menu_draw->addAction(action_draw_rectangle);
    menu_draw->addAction(action_draw_trapezium);
    menu_draw->addAction(action_draw_polygon);
    menu_draw->addAction(action_draw_stab_hole);
    menu_draw->addAction(action_draw_reference);
    menu_draw->addAction(action_draw_eyelet);
    menu_draw->addAction(action_draw_pattern_direction);
    menu_draw->addAction(action_draw_shank_line);
    menu_draw->addSeparator();
    menu_draw->addAction(action_draw_perpendicular);
    menu_draw->addSeparator();

    menu_image = menu_draw->addMenu(tr("图像"));
    menu_image->addAction(action_draw_image);
    menu_image->addAction(action_draw_image_insert);
    menu_image->addAction(action_draw_image_delete);
    menu_image->addAction(action_draw_image_crop);
    menu_image->addAction(action_draw_image_calibrate);
    menu_image->addAction(action_draw_image_calibrate);

    menu_digitizer = menu_draw->addMenu(tr("数字化仪"));
    menu_digitizer->addAction(action_draw_digitizer_activate);
    menu_digitizer->addAction(action_draw_digitizer_set_position);
    menu_digitizer->addAction(action_draw_digitizer_configure);

    menu_scanner = menu_draw->addMenu(tr("扫描仪"));
    menu_scanner->addAction(action_draw_scanner_vectorize_from_scanner);
    menu_scanner->addAction(action_draw_scanner_select_source);
    menu_scanner->addAction(action_draw_scanner_vectorize_image);
// ![2] 绘图栏

// ![3] 插入栏
    menu_insert = ui->menuBar->addMenu(tr("插入(&I)"));
    menu_insert->addAction(action_insert_offset);
    menu_insert->addAction(action_insert_advanced_offset);
    menu_insert->addAction(action_insert_text);
    menu_insert->addAction(action_insert_fillet);
    menu_insert->addAction(action_insert_notch);
    menu_insert->addAction(action_insert_notches_group);
    menu_insert->addAction(action_insert_notches_axis_tied_group);
    menu_insert->addAction(action_insert_size_notch);
    menu_insert->addAction(action_insert_remark);
    menu_insert->addAction(action_insert_smooth);
    menu_insert->addSeparator();
    menu_insert->addAction(action_insert_margin);
    menu_insert->addAction(action_insert_canal);
    menu_insert->addSeparator();
    menu_insert->addAction(action_insert_block);
    menu_insert->addSeparator();
    menu_insert->addAction(action_insert_holes_over_a_line);
    menu_insert->addSeparator();
    menu_insert_array = menu_insert->addMenu("阵列");
    menu_insert_array->addAction(action_insert_array_rectangular);
    menu_insert_array->addAction(action_insert_array_circular);
// ![3] 插入栏

// ![4] 修改栏
    menu_modify = ui->menuBar->addMenu(tr("修改(&M)"));
    menu_modify->addAction(action_modify_undo);
    menu_modify->addAction(action_modify_redo);
    menu_modify->addAction(action_modify_clear);
    menu_modify->addSeparator();
    menu_modify->addAction(action_modify_delete);
    menu_modify->addAction(action_modify_copy);
    menu_modify->addAction(action_modify_paste);
    menu_modify->addAction(action_modify_paste_as_slave);
    menu_modify->addSeparator();

    menu_modify_transform = menu_modify->addMenu(tr("转换"));
    menu_modify_transform->addAction(action_modify_transform_move);
    menu_modify_transform->addAction(action_modify_transform_rotate);
    menu_modify_transform->addAction(action_modify_transform_rotate_knots_continuely);
    menu_modify_transform->addAction(action_modify_transform_rotate_knots);
    menu_modify_transform->addAction(action_modify_transform_scale);
    menu_modify_transform->addAction(action_modify_transform_mirror);
    menu_modify_transform->addAction(action_modify_transform_strech);
    menu_modify_transform->addAction(action_modify_transform_explode);
    menu_modify_transform->addAction(action_modify_transform_join);
    menu_modify_transform->addAction(action_modify_transform_break);
    menu_modify_transform->addAction(action_modify_transform_fillet);
    menu_modify_transform->addAction(action_modify_transform_V_cut);
    menu_modify_transform->addSeparator();
    menu_modify_transform->addAction(action_modify_transform_spline);
//    menu_modify_transform->addAction(action_shear);
//    menu_modify->addMenu(menu_modify_transform);
    menu_modify->addAction(action_modify_trim);
    menu_modify->addAction(action_modify_extend);
    menu_modify->addAction(action_modify_modify_endpoint);
    menu_modify->addAction(action_modify_adjust_polyline);
    menu_modify->addAction(action_modify_start_point);
    menu_modify->addAction(action_modify_soften_up);
    menu_modify->addAction(action_modify_warp);
    menu_modify->addAction(action_modify_knots_reduction);
    menu_modify->addAction(action_modify_replace_part);
    menu_modify->addSeparator();

    menu_modify->addAction(action_modify_change_to);
    menu_modify->addAction(action_modify_make_master_line);
    menu_modify->addAction(action_modify_style);
    menu_modify->addSeparator();
    menu_modify->addAction(action_modify_springing);
// ![4] 修改栏

// ![5] 图样栏
    menu_pattern = ui->menuBar->addMenu(tr("图样(&P)"));
    menu_pattern->addAction(action_pattern_select);
    menu_pattern->addAction(action_pattern_new);
    menu_pattern->addAction(action_pattern_clone);
    menu_pattern->addAction(action_pattern_delete);
    menu_pattern->addAction(action_pattern_rotate);
    menu_pattern->addAction(action_pattern_make_symmetic);
    menu_pattern->addSeparator();
    menu_pattern->addAction(action_pattern_position);
    menu_pattern->addSeparator();
    menu_pattern->addAction(action_pattern_extract_auto);
    menu_pattern->addAction(action_pattern_extract_manual);
    menu_pattern->addAction(action_pattern_find_perimeter);
    menu_pattern->addAction(action_pattern_close_trim);
    menu_pattern->addSeparator();
    menu_pattern->addAction(action_pattern_check);
    menu_pattern->addAction(action_pattern_devide);
    menu_pattern->addSeparator();
    menu_pattern->addAction(action_pattern_save_as_weft);
// ![5] 图样栏

// ![6] 查看栏
    menu_view = ui->menuBar->addMenu(tr("查看(&V)"));
    menu_view->addAction(action_view_xy_axes);
    menu_view->addAction(action_view_grid);
    menu_view->addAction(action_view_knots);
    menu_view->addAction(action_view_image);
    menu_view->addAction(action_view_design_rules);
    menu_view->addAction(action_view_grading_rules);
    menu_view->addAction(action_view_filled_patterns);
    menu_view->addSeparator();
    menu_view->addAction(action_view_zoom_window);
    menu_view->addAction(action_view_zoom_all);
    menu_view->addAction(action_view_zoom_in);
    menu_view->addAction(action_view_zoom_out);
    menu_view->addAction(action_view_zoom_back);
    menu_view->addSeparator();
    menu_view->addAction(action_view_voerlap_patterns);
    menu_view->addSeparator();
    menu_view->addAction(action_view_hide);
    menu_view->addAction(action_view_show);
    menu_view->addAction(action_view_clean);
    menu_view->addAction(action_view_set_reference);
    menu_view->addSeparator();
    menu_view->addAction(action_view_lock_layout);

    menu_view_tool_bar = new QMenu(tr("&工具栏"));
    menu_view_tool_bar->addActions(action_view_tool_slide->actions());
    menu_view_tool_bar->addSeparator();
    menu_view_tool_bar->addAction(action_view_tool_customize);
    menu_view->addMenu(menu_view_tool_bar);
    menu_view->addAction(action_view_status_bar);
    menu_view->addSeparator();
// ![6] 查看栏

// ![7] 效用栏
    menu_utility = ui->menuBar->addMenu(tr("效用(&U)"));
    menu_utility->addAction(action_utility_measure_distance);
    menu_utility->addAction(action_utility_measure_line);
    menu_utility->addSeparator();
    menu_utility->addAction(action_create_pattern_card);
    menu_utility->addAction(action_utility_quick_nesting);
    menu_utility->addAction(action_utility_nest_by_job_card);
    menu_utility->addSeparator();
    menu_utility->addAction(action_utility_heel_cover);
    menu_utility->addSeparator();
    menu_utility->addAction(action_utility_pattern_search);
// ![7] 效用栏

// ![8] 级放
    menu_grading = ui->menuBar->addMenu(tr("级放"));
    menu_grading->addAction(action_grading_parameters);
    menu_grading->addAction(action_grading_grade);
    menu_grading->addAction(action_grading_delete_graded_patterns);

    menu_grading_reference = menu_grading->addMenu(tr("级放参考"));
    menu_grading_reference->addAction(action_grading_reference_insert);
    menu_grading->addSeparator();
    menu_grading->addAction(action_grading_keep_margin);
    menu_grading->addAction(action_grading_delete_margin);
    menu_grading->addSeparator();
    menu_grading->addAction(action_grading_update_grading);
    menu_grading->addSeparator();
    menu_grading->addAction(action_grading_XY_lock);
    menu_grading->addAction(action_grading_delete_XY_lock);
    menu_grading->addSeparator();
    menu_grading->addAction(action_grading_horizontal_zone);
    menu_grading->addAction(action_grading_vertical_zone);
    menu_grading->addAction(action_grading_delete_zone);
// ![8] 级放

// ![9] 常用操作
    menu_operation = ui->menuBar->addMenu(tr("常用操作"));
// ![9] 常用操作

// ![10] 帮助
    menu_help = ui->menuBar->addMenu(tr("帮助"));
    menu_help->addAction(action_help_help);
    menu_help->addSeparator();
    menu_help->addAction(action_help_license);
    menu_help->addSeparator();
    menu_help->addAction(action_help_about);
// ![10] 帮助
    connect(menu_view, &QMenu::aboutToShow, this, &MainWindow::updateViewMenu);
}

void MainWindow::initToolBar()
{
    if(ui->mainToolBar) delete ui->mainToolBar;
// ![1] 边栏工具栏
    tool_slide = new QToolBar(tr("边栏"));
    addToolBar(Qt::LeftToolBarArea, tool_slide);
    tool_slide->setMaximumWidth(20);
    tool_slide->setOrientation(Qt::Vertical);
    tool_slide->setAllowedAreas(Qt::LeftToolBarArea);
    tool_slide->setFloatable(false);
    tool_slide->setMovable(false);
    tool_slide->addActions(action_tool_slide->actions());
    onToolSlideChanged();
// ![1] 边栏工具栏

// ![2] 标准工具栏
    tool_standard = new QToolBar(tr("标准"));
    tool_standard->setOrientation(Qt::Horizontal);
    tool_standard->setAllowedAreas(Qt::AllToolBarAreas);

    tool_standard->addAction(action_file_new);
    tool_standard->addAction(action_file_open);
    tool_standard->addAction(action_file_save);
    tool_standard->addSeparator();
    tool_standard->addAction(action_modify_cut);
    tool_standard->addAction(action_modify_copy);
    tool_standard->addAction(action_modify_paste);
    tool_standard->addAction(action_modify_paste_as_slave);
    tool_standard->addAction(action_file_print);
    tool_standard->addAction(action_help_about);
    tool_standard->addAction(action_help_help);
    tool_standard->addSeparator();
    tool_standard->addAction(action_modify_undo);
    tool_standard->addAction(action_modify_redo);
// ![3] 标准工具栏

// ![4] 草图工具栏
    tool_sketch = new QToolBar(tr("草图"));
    tool_sketch->setOrientation(Qt::Horizontal);
    tool_sketch->setAllowedAreas(Qt::AllToolBarAreas);

    tool_sketch->addAction(action_draw_line);
    tool_sketch->addAction(action_draw_middle_axis);
    tool_sketch->addAction(action_draw_polyline);
    tool_sketch->addSeparator();
    tool_sketch->addAction(action_draw_circle);
    tool_sketch->addAction(action_draw_ellipse);
    tool_sketch->addAction(action_draw_arc_by_3_points);
    tool_sketch->addAction(action_draw_arc_by_3_points_2);
    tool_sketch->addSeparator();
    tool_sketch->addAction(action_draw_rectangle);
    tool_sketch->addAction(action_draw_trapezium);
    tool_sketch->addSeparator();
    tool_sketch->addAction(action_draw_stab_hole);
    tool_sketch->addAction(action_draw_reference);
    tool_sketch->addSeparator();
    tool_sketch->addAction(action_draw_perpendicular);
// ![4] 草图工具栏

// ![5] 对象捕捉
    tool_object_snap = new QToolBar(tr("对象捕捉"));
    tool_object_snap->setOrientation(Qt::Horizontal);
    tool_object_snap->setAllowedAreas(Qt::AllToolBarAreas);

    tool_object_snap->addAction(action_object_snap_end_point);
    tool_object_snap->addAction(action_object_snap_center);
    tool_object_snap->addAction(action_object_snap_intersection);
    tool_object_snap->addAction(action_object_snap_knot);
    tool_object_snap->addSeparator();
    tool_object_snap->addAction(action_object_snap_ortho);
    tool_object_snap->addAction(action_object_snap_enter_coordinates);
// ![5] 对象捕捉

// ![6] 缩放
    tool_zoom = new QToolBar(tr("缩放"));
    tool_zoom->setOrientation(Qt::Horizontal);
    tool_zoom->setAllowedAreas(Qt::AllToolBarAreas);

    tool_zoom->addAction(action_view_zoom_in);
    tool_zoom->addAction(action_view_zoom_out);
    tool_zoom->addAction(action_view_zoom_window);
    tool_zoom->addAction(action_view_zoom_all);
    tool_zoom->addSeparator();
    tool_zoom->addAction(action_view_xy_axes);
    tool_zoom->addAction(action_view_grid);
    tool_zoom->addAction(action_view_image);
    tool_zoom->addAction(action_view_knots);
    tool_zoom->addAction(action_view_filled_patterns);
    tool_zoom->addSeparator();
    tool_zoom->addAction(action_view_voerlap_patterns);
    tool_zoom->addAction(action_view_design_rules);
    tool_zoom->addAction(action_view_grading_rules);
// ![6] 缩放

// ![7] 修改
    tool_modify = new QToolBar(tr("修改"));
    tool_modify->setOrientation(Qt::Horizontal);
    tool_modify->setAllowedAreas(Qt::AllToolBarAreas);

    tool_modify->addAction(action_modify_delete);
    tool_modify->addAction(action_modify_transform_move);
    tool_modify->addAction(action_modify_transform_rotate);
    tool_modify->addAction(action_modify_transform_rotate_knots_continuely);
    tool_modify->addAction(action_modify_transform_rotate_knots);
    tool_modify->addAction(action_modify_transform_scale);
    tool_modify->addAction(action_modify_transform_mirror);
    tool_modify->addSeparator();
    tool_modify->addAction(action_modify_transform_strech);
    tool_modify->addAction(action_modify_trim);
    tool_modify->addAction(action_modify_extend);
    tool_modify->addAction(action_modify_modify_endpoint);
    tool_modify->addAction(action_modify_change_to);
    tool_modify->addSeparator();
    tool_modify->addAction(action_modify_transform_explode);
    tool_modify->addAction(action_modify_transform_join);
    tool_modify->addAction(action_modify_transform_break);
    tool_modify->addAction(action_modify_transform_fillet);
    tool_modify->addSeparator();
    tool_modify->addAction(action_modify_transform_V_cut);
    tool_modify->addAction(action_modify_adjust_polyline);
    tool_modify->addAction(action_modify_make_master_line);
    tool_modify->addSeparator();
    tool_modify->addAction(action_modify_style);
    tool_modify->addAction(action_modify_springing);
    tool_modify->addAction(action_modify_warp);
    tool_modify->addAction(action_modify_replace_part);
// ![7] 修改

// ![8] 图片
    tool_image = new QToolBar(tr("图片"));
    tool_image->setOrientation(Qt::Horizontal);
    tool_image->setAllowedAreas(Qt::AllToolBarAreas);

    tool_image->addAction(action_draw_image_insert);
    tool_image->addAction(action_draw_image_delete);
    tool_image->addAction(action_draw_image_crop);
    tool_image->addAction(action_draw_image_calibrate);
    tool_image->addAction(action_draw_image_agjustement);
// ![8] 图片

// ![9] 数字化仪
    tool_digitizer = new QToolBar(tr("数字化仪"));
    tool_digitizer->setOrientation(Qt::Horizontal);
    tool_digitizer->setAllowedAreas(Qt::AllToolBarAreas);

    tool_digitizer->addAction(action_draw_digitizer_activate);
    tool_digitizer->addAction(action_draw_digitizer_set_position);
    tool_digitizer->addAction(action_draw_digitizer_configure);
// ![9] 数字化仪

// ![10] 级放
    tool_grading = new QToolBar(tr("级放"));
    tool_grading->setOrientation(Qt::Horizontal);
    tool_grading->setAllowedAreas(Qt::AllToolBarAreas);

    tool_grading->addAction(action_grading_parameters);
    tool_grading->addAction(action_grading_grade);
    tool_grading->addAction(action_grading_delete_graded_patterns);
    tool_grading->addAction(action_grading_reference_insert);
    tool_grading->addAction(action_grading_keep_margin);
    tool_grading->addAction(action_grading_delete_margin);
    tool_grading->addAction(action_grading_update_grading);
    tool_grading->addAction(action_grading_XY_lock);
    tool_grading->addAction(action_grading_delete_XY_lock);
    tool_grading->addSeparator();
    tool_grading->addAction(action_grading_horizontal_zone);
    tool_grading->addAction(action_grading_vertical_zone);
    tool_grading->addAction(action_grading_delete_zone);
// ![10] 级放

// ![11] 扫描仪
    tool_scanner = new QToolBar(tr("扫描仪"));
    tool_scanner->setOrientation(Qt::Horizontal);
    tool_scanner->setAllowedAreas(Qt::AllToolBarAreas);

    tool_scanner->addAction(action_draw_scanner_vectorize_from_scanner);
    tool_scanner->addAction(action_draw_scanner_select_source);
    tool_scanner->addAction(action_draw_scanner_vectorize_image);
// ![11] 扫描仪

// ![12] 插入
    tool_insert = new QToolBar(tr("插入"));
    tool_insert->setOrientation(Qt::Horizontal);
    tool_insert->setAllowedAreas(Qt::AllToolBarAreas);

    tool_insert->addAction(action_insert_offset);
    tool_insert->addAction(action_insert_advanced_offset);
    tool_insert->addAction(action_insert_notch);
    tool_insert->addAction(action_insert_notches_group);
    tool_insert->addAction(action_insert_notches_axis_tied_group);
    tool_insert->addAction(action_insert_size_notch);
    tool_insert->addAction(action_insert_size_notch);
    tool_insert->addAction(action_insert_text);
    tool_insert->addAction(action_insert_smooth);
    tool_insert->addSeparator();
    tool_insert->addAction(action_insert_margin);
    tool_insert->addAction(action_insert_canal);
// ![12] 插入

// ![13] 图样
    tool_pattern = new QToolBar(tr("图样"));
    tool_pattern->setOrientation(Qt::Horizontal);
    tool_pattern->setAllowedAreas(Qt::AllToolBarAreas);

    tool_pattern->addAction(action_pattern_select);
    tool_pattern->addAction(action_pattern_check);
    tool_pattern->addAction(action_pattern_new);
    tool_pattern->addAction(action_pattern_clone);
    tool_pattern->addAction(action_pattern_delete);
    tool_pattern->addAction(action_pattern_make_symmetic);
    tool_pattern->addAction(action_pattern_position);
    tool_pattern->addAction(action_pattern_extract_auto);
    tool_pattern->addAction(action_pattern_find_perimeter);
    tool_pattern->addAction(action_pattern_close_trim);
    tool_pattern->addSeparator();
    tool_pattern->addAction(action_pattern_extract_manual);
// ![13] 图样

    addToolBar(Qt::TopToolBarArea, tool_standard);
    addToolBar(Qt::TopToolBarArea, tool_sketch);
    addToolBar(Qt::TopToolBarArea, tool_digitizer);
    this->addToolBarBreak();
    addToolBar(Qt::TopToolBarArea, tool_zoom);
    addToolBar(Qt::TopToolBarArea, tool_object_snap);
    addToolBar(Qt::TopToolBarArea, tool_image);
    this->addToolBarBreak();
    addToolBar(Qt::TopToolBarArea, tool_modify);
    this->addToolBarBreak();
    addToolBar(Qt::TopToolBarArea, tool_grading);
    addToolBar(Qt::TopToolBarArea, tool_scanner);
    this->addToolBarBreak();
    addToolBar(Qt::TopToolBarArea, tool_insert);
    addToolBar(Qt::TopToolBarArea, tool_pattern);
}

void MainWindow::initStatusBar()
{
    if(ui->statusBar) delete ui->statusBar;
    statusBar();
}

void MainWindow::initDockWidget()
{
    //删除中央窗体
    QWidget* p = takeCentralWidget();
    if(p) delete p;

    // 设置dock窗口嵌套
    setDockNestingEnabled(DockNestingEnabled);

    // 新建dock窗口
    dock_find_style = new QDockWidget(tr("查找款型"));     // 添加find_style浮动窗口
    dock_project = new QDockWidget(tr("项目"));        // 添加project浮动窗口
    dock_properties = new QDockWidget(tr("属性"));     // 添加properties浮动窗口
    dock_paint_area= new QDockWidget(this);      // 添加绘图区

    dock_find_style->setHidden(!action_view_tool_find_style->isChecked());
    dock_project->setHidden(!action_view_tool_project->isChecked());
    dock_properties->setHidden(!action_view_tool_properties->isChecked());

//    connect(dock_find_style, &QDockWidget::visibilityChanged, this, &MainWindow::onDockFindStyleVisibilityChanged);
//    connect(dock_project, &QDockWidget::visibilityChanged, this, &MainWindow::onDockProjectVisibilityChanged);
//    connect(dock_properties, &QDockWidget::visibilityChanged, this, &MainWindow::onDockPropertiesVisibilityChanged);

    // 绘图区隐藏标题栏
    QWidget * qw=new QWidget(this);
    dock_paint_area->setTitleBarWidget(qw);

    // 绘图区不可移动、不可关闭、不可最小化
    dock_paint_area->setFeatures(QDockWidget::NoDockWidgetFeatures);

    // 将窗口添加至主窗口
    addDockWidget(Qt::LeftDockWidgetArea,dock_find_style);
    splitDockWidget(dock_find_style,dock_project,Qt::Horizontal);
    splitDockWidget(dock_project, dock_paint_area, Qt::Horizontal);
    splitDockWidget(dock_project,dock_properties,Qt::Vertical);
}

void MainWindow::initProject()
{
    // 初始化view
    view = new MyGraphicsView(this);

    // 初始化项目
    QString name_project_new = getNewProjectName();
    project_active = new Project(name_project_new, this);
    QString name_layer_new = project_active->getLayerName(project_active->getActiveLayer());
    list_project.append(project_active);
    layer_active = project_active->getActiveLayer();
    view->setScene(layer_active->getScene());
    dock_paint_area->setWidget(view);

    tree_project = new QTreeWidget();
    tree_project->setColumnCount(1); //设置列数
    tree_project->setHeaderLabel(tr("项目列表")); //设置头的标题
    tree_project->setContextMenuPolicy(Qt::CustomContextMenu);//右键 不可少否则右键无反应
    connect(tree_project, &QTreeWidget::customContextMenuRequested, this, &MainWindow::showTreeMenu);
    connect(tree_project, &QTreeWidget::itemDoubleClicked, this, &MainWindow::onTreeProjectItemDoubleClicked);

    QTreeWidgetItem *item_project = new QTreeWidgetItem(tree_project,QStringList(name_project_new));
    QTreeWidgetItem *item_layer = new QTreeWidgetItem(item_project,QStringList(name_layer_new)); //子节点1

    item_layer->setCheckState(0, Qt::Checked);
    item_project->addChild(item_layer); //添加子节点
    tree_project->expandAll(); //结点全部展开
    dock_project->setWidget(tree_project);
}

void MainWindow::writeConfiguration()
{
    QSettings settings(CONFG_FILE_PATH,QSettings::IniFormat);
    settings.beginGroup("view");
    settings.setValue("view_xy_axes", QVariant(action_view_xy_axes->isChecked()));
    settings.setValue("view_grid", QVariant(action_view_grid->isChecked()));
    settings.setValue("view_grading_rules", QVariant(action_view_grading_rules->isChecked()));
    settings.setValue("view_filled_patterns", QVariant(action_view_filled_patterns->isChecked()));
    settings.setValue("view_tool_find_style", QVariant(action_view_tool_find_style->isChecked()));
    settings.setValue("view_tool_project", QVariant(action_view_tool_project->isChecked()));
    settings.setValue("view_tool_properties",QVariant( action_view_tool_properties->isChecked()));
    settings.endGroup();
}

void MainWindow::readConfiguration()
{
    QFileInfo file(CONFG_FILE_PATH);
    // 若配置文件不存在，初始化系统配置
    if(!file.exists()){
        qDebug() << "配置文件不存在";
        writeConfiguration();
        return;
    }
    // 若配置文件存在，读入配置
    qDebug() << "配置文件已存在";
    QSettings settings(CONFG_FILE_PATH,QSettings::IniFormat);
    settings.beginGroup("view");
    bool view_xy_axes_on = settings.value("view_xy_axes").toBool();
    bool view_grid_on = settings.value("view_grid").toBool();
    bool view_grading_rules_on = settings.value("view_grading_rules").toBool();
    bool view_filled_patterns_on = settings.value("view_filled_patterns").toBool();
    bool view_tool_find_style_on = settings.value("view_tool_find_style").toBool();
    bool view_tool_project_on = settings.value("view_tool_project").toBool();
    bool view_tool_properties_on = settings.value("view_tool_properties").toBool();
    qDebug() << view_xy_axes_on;
    qDebug() << view_grid_on;
    qDebug() << view_grading_rules_on;
    qDebug() << view_filled_patterns_on;
    qDebug() << view_tool_find_style_on;
    qDebug() << view_tool_project_on;
    qDebug() << view_tool_properties_on;
    action_view_xy_axes->setChecked(view_xy_axes_on);
    action_view_grid->setChecked(view_grid_on);
    action_view_grid->setChecked(view_grid_on);
    action_view_grading_rules->setChecked(view_grading_rules_on);
    action_view_filled_patterns->setChecked(view_filled_patterns_on);
    action_view_tool_project->setChecked(view_tool_project_on);
    action_view_tool_properties->setChecked(view_tool_properties_on);
    settings.endGroup();
}

QString MainWindow::getNewProjectName()
{
    // 获取项目数目
    int len_project = list_project.length() + 1;
    QString str = QString::number(len_project, 10);
    char *ch;
    QByteArray ba = str.toLatin1();
    ch = ba.data();
    return tr("新项目") + tr(ch);
}

Project *MainWindow::getProjectByName(QString project_name)
{
    for(int i=0;i<list_project.length();i++){
        if(list_project.at(i)->getName() == project_name){
            return list_project.at(i);
        }
    }
}

Layer *MainWindow::getLayerByName(Project *project, QString layer_name)
{
    for(int i=0;i<project->getLayerList().length();i++){
        if(project->getLayer(i)->getName() == layer_name){
            return project->getLayer(i);
         }
    }
}

void MainWindow::showTreeMenu(QPoint pos)
{
    tree_project_layer_active_item = tree_project->itemAt(pos); //可得到右键条目
    if(tree_project_layer_active_item == NULL){  //这种情况是右键的位置不在treeItem的范围内,即在空白位置右击,则显示
        menu_tree = new QMenu(tree_project);
        action_tree_expand_all = new QAction(tr("展开全部"));
        action_tree_fold_all = new QAction(tr("折叠全部"));
        connect(action_tree_expand_all, &QAction::triggered, this, &MainWindow::onActionTreeExpandAll);
        connect(action_tree_fold_all, &QAction::triggered, this, &MainWindow::onActionTreeFoldAll);
        menu_tree->addAction(action_tree_expand_all);
        menu_tree->addAction(action_tree_fold_all);
        menu_tree->exec(QCursor::pos());  //在当前鼠标位置显示
        return;
    }
    tree_project_active_item = tree_project_layer_active_item->parent();
    if(tree_project_active_item == NULL){     // 项目栏
        tree_project_active_item = tree_project_layer_active_item;
        tree_project_layer_active_item = NULL;
        menu_tree_project = new QMenu(tree_project);
        action_tree_project_add_layer = new QAction(tr("添加图层"));
        action_tree_project_save = new QAction(tr("保存"));
        action_tree_project_save_as = new QAction(tr("另存为"));
        action_tree_project_rename = new QAction(tr("重命名"));
        action_tree_project_close = new QAction(tr("关闭"));

        connect(action_tree_project_add_layer, &QAction::triggered, this, &MainWindow::onActionTreeProjectAddLayer);
        connect(action_tree_project_save, &QAction::triggered, this, &MainWindow::onActionTreeProjectSave);
        connect(action_tree_project_save_as, &QAction::triggered, this, &MainWindow::onActionTreeProjectSaveAs);
        connect(action_tree_project_rename, &QAction::triggered, this, &MainWindow::onActionTreeProjectRename);
        connect(action_tree_project_close, &QAction::triggered, this, &MainWindow::onActionTreeProjectClose);

        menu_tree_project->addAction(action_tree_project_rename);
        menu_tree_project->addAction(action_tree_project_add_layer);
        menu_tree_project->addAction(action_tree_project_save);
        menu_tree_project->addAction(action_tree_project_save_as);
        menu_tree_project->addAction(action_tree_project_close);
        menu_tree_project->exec(QCursor::pos());  //在当前鼠标位置显示
    } else{                 // 图层栏
        menu_tree_project_layer = new QMenu(tree_project);
        action_tree_project_layer_change_to = new QAction(tr("切换至该图层"));

        menu_tree_project_layer_move_up = new QMenu(tr("上移"));

        action_tree_project_layer_move_up_one = new QAction(tr("上移一层"));
        action_tree_project_layer_move_up_top = new QAction(tr("移至顶部"));
        menu_tree_project_layer_move_up->addAction(action_tree_project_layer_move_up_one);
        menu_tree_project_layer_move_up->addAction(action_tree_project_layer_move_up_top);

        menu_tree_project_layer_move_down = new QMenu(tr("下移"));
        action_tree_project_layer_move_down_one = new QAction(tr("下移一层"));
        action_tree_project_layer_move_down_bottom = new QAction(tr("移至底层"));
        menu_tree_project_layer_move_down->addAction(action_tree_project_layer_move_down_one);
        menu_tree_project_layer_move_down->addAction(action_tree_project_layer_move_down_bottom);

        int layer_length = tree_project_layer_active_item->parent()->childCount();
        if(layer_length <= 1){
            action_tree_project_layer_move_up_one->setEnabled(false);
            action_tree_project_layer_move_up_top->setEnabled(false);
            action_tree_project_layer_move_down_one->setEnabled(false);
            action_tree_project_layer_move_down_bottom->setEnabled(false);
        }

        action_tree_project_layer_rename = new QAction(tr("重命名"));
        action_tree_project_layer_delete = new QAction(tr("删除"));

        connect(action_tree_project_layer_change_to, &QAction::triggered, this, &MainWindow::onActionTreeProjectLayerChangeTo);
        connect(action_tree_project_layer_move_up_one, &QAction::triggered, this, &MainWindow::onActionTreeProjectLayerMoveUpOne);
        connect(action_tree_project_layer_move_up_top, &QAction::triggered, this, &MainWindow::onActionTreeProjectLayerMoveUpTop);
        connect(action_tree_project_layer_move_down_one, &QAction::triggered, this, &MainWindow::onActionTreeProjectLayerMoveDownOne);
        connect(action_tree_project_layer_move_down_bottom, &QAction::triggered, this, &MainWindow::onActionTreeProjectLayerMoveDownBottom);
        connect(action_tree_project_layer_rename, &QAction::triggered, this, &MainWindow::onActionTreeProjectLayerRename);
        connect(action_tree_project_layer_delete, &QAction::triggered, this, &MainWindow::onActionTreeProjectLayerDelete);

        menu_tree_project_layer->addAction(action_tree_project_layer_change_to);
        menu_tree_project_layer->addMenu(menu_tree_project_layer_move_up);
        menu_tree_project_layer->addMenu(menu_tree_project_layer_move_down);
        menu_tree_project_layer->addAction(action_tree_project_layer_rename);
        menu_tree_project_layer->addAction(action_tree_project_layer_delete);
        menu_tree_project_layer->exec(QCursor::pos());  //在当前鼠标位置显示
    }
}

void MainWindow::updateViewMenu()
{
    bool tool_slide_enable = action_view_tool_slide->isEnabled();
    if(tool_slide_enable){
        action_view_tool_find_style->setEnabled(tool_slide_enable);
        action_view_tool_project->setEnabled(tool_slide_enable);
        action_view_tool_properties->setEnabled(tool_slide_enable);
    }
}

void MainWindow::onToolSlideChanged()
{
    if(!action_tool_slide_find_style->isVisible()
            & !action_tool_slide_project->isVisible()
            & !action_tool_slide_properties->isVisible()){
        tool_slide->setVisible(false);
    } else{
        tool_slide->setVisible(true);
    }
}

void MainWindow::onDockFindStyleVisibilityChanged(bool visible)
{
    action_tool_slide_find_style->setVisible(!visible);
    action_view_tool_find_style->setChecked(visible);
    dock_find_style->setHidden(!visible);
    onToolSlideChanged();
}

void MainWindow::onDockProjectVisibilityChanged(bool visible)
{
    action_tool_slide_project->setVisible(!visible);
    action_tool_slide_project->setChecked(visible);
    dock_project->setHidden(!visible);
    onToolSlideChanged();
}

void MainWindow::onDockPropertiesVisibilityChanged(bool visible)
{
    action_tool_slide_properties->setVisible(!visible);
    action_tool_slide_properties->setChecked(visible);
    dock_properties->setHidden(!visible);
    onToolSlideChanged();
}

bool MainWindow::maybeSave()
{
    for(int i=0; i<list_project.length();i++){
        if(!list_project.at(i)->isSaved()){
            QMessageBox::StandardButton box;
            box = QMessageBox::warning(this,tr("保存项目"),tr("您要存储对“") + project_active->getName() + tr("”所做的更改吗？"),
            QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
            if(box == QMessageBox::Yes)
            {
                return onActionFileSave();
            }
            else if(box == QMessageBox::Cancel) {
                return false;
            }
        }
    }
    return true;
}

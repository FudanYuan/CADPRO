#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mydocktitlebar.h"

#include "configuredialog.h"

#include <QDockWidget>
#include <QToolButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFrame>

#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QBitmap>

#include <QMessageBox>
#include <QFileDialog>

#include <QDebug>

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
    initDockWidget();   // 初始化窗口
    initConfiguration();
    initProjectView();  // 初始化项目
    initStatusBar();    // 初始化状态栏
}

MainWindow::~MainWindow()
{
    delete ui;
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
    connect(action_file_save, &QAction::triggered, this, &MainWindow::onActionFileSave);

    action_file_save_as = new QAction(tr("&另存为"), this);
    action_file_save_as->setShortcut(QKeySequence::SaveAs);
    action_file_save_as->setStatusTip(tr("另存当前项目"));
    connect(action_file_save_as, &QAction::triggered, this, &MainWindow::onActionFileSaveAs);

    action_file_save_all = new QAction(tr("&保存全部"), this);
    action_file_save_all->setStatusTip(tr("保存所有打开的项目"));
    connect(action_file_save_all, &QAction::triggered, this, &MainWindow::onActionFileSaveAll);

    action_file_print = new QAction(tr("&打印"), this);
    action_file_print->setShortcut(QKeySequence::Print);
    action_file_print->setStatusTip(tr("打印当前视图"));
    connect(action_file_print, &QAction::triggered, this, &MainWindow::onActionFilePrint);

    action_file_print_preview = new QAction(tr("&打印预览"), this);
    action_file_print_preview->setStatusTip(tr("显示当前项目打印预览"));
    action_file_print_preview->setDisabled(true);
    connect(action_file_print_preview, &QAction::triggered, this, &MainWindow::onActionFilePrintPreview);

    action_file_print_setup = new QAction(tr("&打印设置"), this);
    action_file_print_setup->setStatusTip(tr("更改打印机和打印选项"));
    action_file_print_setup->setDisabled(true);
    connect(action_file_print_setup, &QAction::triggered, this, &MainWindow::onActionFilePrintSetup);

    action_file_import_DXF = new QAction(tr("&DXF"), this);
    action_file_import_DXF->setStatusTip(tr("导入DXF文件"));
    connect(action_file_import_DXF, &QAction::triggered, this, &MainWindow::onActionFileImportDXF);

    action_file_import_DVS = new QAction(tr("&DVS"), this);
    action_file_import_DVS->setStatusTip(tr("导入DVS文件"));
    connect(action_file_import_DVS, &QAction::triggered, this, &MainWindow::onActionFileImportDVS);

    action_file_import_TEF = new QAction(tr("&TEF"), this);
    action_file_import_TEF->setStatusTip(tr("导入TEF文件"));
    connect(action_file_import_TEF, &QAction::triggered, this, &MainWindow::onActionFileImportTEF);

    action_file_export_DXF = new QAction(tr("&DXF"), this);
    action_file_export_DXF->setStatusTip(tr("导出DXF文件"));
    connect(action_file_export_DXF, &QAction::triggered, this, &MainWindow::onActionFileExportDXF);

    action_file_export_DVS = new QAction(tr("&DVS"), this);
    action_file_export_DVS->setStatusTip(tr("导出DVS文件"));
    connect(action_file_export_DVS, &QAction::triggered, this, &MainWindow::onActionFileExportDVS);

    action_file_export_TEF = new QAction(tr("&TEF"), this);
    action_file_export_TEF->setStatusTip(tr("导出TEF文件"));
    connect(action_file_export_TEF, &QAction::triggered, this, &MainWindow::onActionFileExportTEF);

    action_file_configuration = new QAction(tr("&配置"), this);
    action_file_configuration->setStatusTip(tr("修改cadPro配置"));
    connect(action_file_configuration, &QAction::triggered, this, &MainWindow::onActionFileConfiguration);

    action_file_exit = new QAction(tr("&退出"), this);
    action_file_exit->setShortcut(QKeySequence::Quit);
    action_file_exit->setStatusTip(tr("退出应用程序；提示保存项目"));
    connect(action_file_exit, &QAction::triggered, this, &MainWindow::onActionFileExit);


// ![1] 文件

// ![2] 绘图
    action_draw_line = new QAction(tr("&线条"), this);
    action_draw_line->setStatusTip(tr("绘制直线"));
    connect(action_draw_line, &QAction::triggered, this, &MainWindow::onActionDrawLine);

    action_draw_middle_axis = new QAction(tr("&中轴线"), this);
    action_draw_middle_axis->setStatusTip(tr("绘制中轴线"));
    connect(action_draw_middle_axis, &QAction::triggered, this, &MainWindow::onActionDrawMiddleAxis);

    action_draw_polyline = new QAction(tr("&曲折线"), this);
    action_draw_polyline->setStatusTip(tr("绘制曲折线"));
    connect(action_draw_polyline, &QAction::triggered, this, &MainWindow::onActionDrawPolyline);


    action_draw_circle = new QAction(tr("&圆"), this);
    action_draw_circle->setStatusTip(tr("按中心和半径绘制圆"));
    connect(action_draw_circle, &QAction::triggered, this, &MainWindow::onActionDrawCircle);

    action_draw_ellipse = new QAction(tr("&椭圆"), this);
    action_draw_ellipse->setStatusTip(tr("按中心和半径绘制椭圆"));
    connect(action_draw_ellipse,&QAction::triggered,this,&MainWindow::onActionDrawEllipse);

    action_draw_arc_by_3_points = new QAction(tr("&3点圆弧"), this);
    action_draw_arc_by_3_points->setStatusTip(tr("绘制3点椭圆(起点，终点，中心点)"));
    connect(action_draw_arc_by_3_points, &QAction::triggered, this, &MainWindow::onActionDrawArcBy3Pnts);

    action_draw_arc_by_3_points_2 = new QAction(tr("&3点圆弧2"), this);
    action_draw_arc_by_3_points_2->setStatusTip(tr("绘制3点椭圆(起点，终点，中心点)"));
    connect(action_draw_arc_by_3_points_2, &QAction::triggered, this, &MainWindow::onActionDrawArcBy3Pnts2);

    action_draw_rectangle = new QAction(tr("&矩形"), this);
    action_draw_rectangle->setStatusTip(tr("绘制矩形"));
    connect(action_draw_rectangle,&QAction::triggered,this,&MainWindow::onActionDrawRect);

    action_draw_trapezium = new QAction(tr("&梯形"), this);
    action_draw_trapezium->setStatusTip(tr("绘制梯形"));
    connect(action_draw_trapezium, &QAction::triggered, this, &MainWindow::onActionDrawTrapezium);

    action_draw_polygon = new QAction(tr("&正多边形"), this);
    action_draw_polygon->setStatusTip(tr("绘制正多边形"));
    connect(action_draw_polygon, &QAction::triggered, this, &MainWindow::onActionDrawPolygon);

    action_draw_stab_hole = new QAction(tr("&冲孔"), this);
    action_draw_stab_hole->setStatusTip(tr("绘制冲孔"));
    connect(action_draw_stab_hole, &QAction::triggered, this, &MainWindow::onActionDrawStabHole);

    action_draw_reference = new QAction(tr("&参考点"), this);
    action_draw_reference->setStatusTip(tr("绘制参考点"));
    connect(action_draw_reference, &QAction::triggered, this, &MainWindow::onActionDrawReference);

    action_draw_eyelet = new QAction(tr("&鸡眼孔"), this);
    action_draw_eyelet->setStatusTip(tr("在当前图案上插入鸡眼孔"));
    connect(action_draw_eyelet, &QAction::triggered, this, &MainWindow::onActionDrawEyelet);

    action_draw_pattern_direction = new QAction(tr("&图案方向"), this);
    action_draw_pattern_direction->setStatusTip(tr("插入方向轴"));
    connect(action_draw_pattern_direction, &QAction::triggered, this, &MainWindow::onActionDrawPatternDirection);

    action_draw_shank_line = new QAction(tr("&上插线"), this);
    action_draw_shank_line->setStatusTip(tr("创建上插线"));
    connect(action_draw_shank_line, &QAction::triggered, this, &MainWindow::onActionDrawShankLine);

    action_draw_perpendicular = new QAction(tr("&垂直线"), this);
    action_draw_perpendicular->setStatusTip(tr("绘制垂直线"));
    action_draw_perpendicular->setDisabled(true);
    connect(action_draw_perpendicular, &QAction::triggered, this, &MainWindow::onActionDrawPerpendicular);

    action_draw_image = new QAction(tr("&获得"), this);
    action_draw_image->setStatusTip(tr("从扫描仪获取图像"));
    connect(action_draw_image, &QAction::triggered, this, &MainWindow::onActionDrawImage);

    action_draw_image_insert = new QAction(tr("&插入"), this);
    action_draw_image_insert->setStatusTip(tr("在图样中插入图片"));
    connect(action_draw_image_insert, &QAction::triggered, this, &MainWindow::onActionDrawImageInsert);

    action_draw_image_delete = new QAction(tr("&删除"), this);
    action_draw_image_delete->setStatusTip(tr("从图样中删除图片"));
    action_draw_image_delete->setDisabled(true);
    connect(action_draw_image_delete, &QAction::triggered, this, &MainWindow::onActionDrawImageDelete);

    action_draw_image_crop = new QAction(tr("&裁剪"), this);
    action_draw_image_crop->setStatusTip(tr("裁剪图样图片"));
    action_draw_image_crop->setDisabled(true);
    connect(action_draw_image_crop, &QAction::triggered, this, &MainWindow::onActionDrawImageCrop);

    action_draw_image_calibrate = new QAction(tr("&校准/划刻度"), this);
    action_draw_image_calibrate->setStatusTip(tr("校准图像大小"));
    action_draw_image_calibrate->setDisabled(true);
    connect(action_draw_image_calibrate, &QAction::triggered, this, &MainWindow::onActionDrawImageCalibrate);

    action_draw_image_agjustement = new QAction(tr("&图像调整"), this);
    action_draw_image_agjustement->setStatusTip(tr("校准图像大小"));
    action_draw_image_agjustement->setDisabled(true);
    connect(action_draw_image_agjustement, &QAction::triggered, this, &MainWindow::onActionDrawImageAdjustment);

    action_draw_digitizer_activate = new QAction(tr("&启用"), this);
    action_draw_digitizer_activate->setStatusTip(tr("启用/禁用数字化仪"));
    action_draw_digitizer_activate->setDisabled(true);
    connect(action_draw_digitizer_activate, &QAction::triggered, this, &MainWindow::onActionDrawDigitizerActivate);

    action_draw_digitizer_set_position = new QAction(tr("&设置shell位置"), this);
    action_draw_digitizer_set_position->setStatusTip(tr("设置shell位置"));
    action_draw_digitizer_set_position->setDisabled(true);
    connect(action_draw_digitizer_set_position, &QAction::triggered, this, &MainWindow::onActionDrawDigitizerSetPosition);

    action_draw_digitizer_configure = new QAction(tr("&配置"), this);
    action_draw_digitizer_configure->setStatusTip(tr("配置数字化仪大小"));
    action_draw_digitizer_configure->setDisabled(true);
    connect(action_draw_digitizer_configure, &QAction::triggered, this, &MainWindow::onActionDrawDigitizerConfigue);

    action_draw_scanner_vectorize_from_scanner = new QAction(tr("&扫描矢量"), this);
    action_draw_scanner_vectorize_from_scanner->setStatusTip(tr("扫描和矢量化一个图样"));
    connect(action_draw_scanner_vectorize_from_scanner, &QAction::triggered, this, &MainWindow::onActionDrawScannerVectorizeFromScanner);

    action_draw_scanner_select_source = new QAction(tr("&选择源"), this);
    action_draw_scanner_select_source->setStatusTip(tr("选择扫描源"));
    connect(action_draw_scanner_select_source, &QAction::triggered, this, &MainWindow::onActionDrawScannerSelectSource);


    action_draw_scanner_vectorize_image = new QAction(tr("&矢量图像"), this);
    action_draw_scanner_vectorize_image->setStatusTip(tr("矢量化扫描图像"));
    connect(action_draw_scanner_vectorize_image, &QAction::triggered, this, &MainWindow::onActionDrawScannerVectorizeImage);
// ![2] 绘图

// ![3] 插入
    action_insert_offset = new QAction(tr("&偏移"), this);
    action_insert_offset->setStatusTip(tr("创建偏移"));
    action_insert_offset->setDisabled(true);
    connect(action_insert_offset, &QAction::triggered, this, &MainWindow::onActionInsertOffset);

    action_insert_advanced_offset = new QAction(tr("&高级偏移"), this);
    action_insert_advanced_offset->setStatusTip(tr("创建高级偏移"));
    action_insert_advanced_offset->setDisabled(true);
    connect(action_insert_advanced_offset, &QAction::triggered, this, &MainWindow::onActionInsertAdvancedOffset);

    action_insert_text = new QAction(tr("&文本"), this);
    action_insert_text->setStatusTip(tr("插入文本"));
    connect(action_insert_text, &QAction::triggered, this, &MainWindow::onActionInsertText);

    action_insert_fillet = new QAction(tr("&倒圆角"), this);
    action_insert_fillet->setStatusTip(tr("倒圆角两条线"));
    action_insert_fillet->setDisabled(true);
    connect(action_insert_fillet, &QAction::triggered, this, &MainWindow::onActionInsertFillet);

    action_insert_notch = new QAction(tr("&凹槽"), this);
    action_insert_notch->setStatusTip(tr("插入凹槽"));
    action_insert_notch->setDisabled(true);
    connect(action_insert_notch, &QAction::triggered, this, &MainWindow::onActionInsertNotch);

    action_insert_notches_group = new QAction(tr("&凹槽组"), this);
    action_insert_notches_group->setStatusTip(tr("插入凹槽"));
    action_insert_notches_group->setDisabled(true);
    connect(action_insert_notches_group, &QAction::triggered, this, &MainWindow::onActionInsertNotchesGroup);

    action_insert_notches_axis_tied_group = new QAction(tr("&凹槽轴绑定群组"), this);
    action_insert_notches_axis_tied_group->setStatusTip(tr("插入一个与轴绑定的凹槽群组"));
    action_insert_notches_axis_tied_group->setDisabled(true);
    connect(action_insert_notches_axis_tied_group, &QAction::triggered, this, &MainWindow::onActionInsertNotchesAxisTiedGroup);


    action_insert_size_notch = new QAction(tr("&凹槽尺寸编号"), this);
    action_insert_size_notch->setStatusTip(tr("插入凹槽尺寸编号"));
    action_insert_size_notch->setDisabled(true);
    connect(action_insert_size_notch, &QAction::triggered, this, &MainWindow::onActionInsertSizeNotch);

    action_insert_remark = new QAction(tr("&备注"), this);
    action_insert_remark->setStatusTip(tr("插入备注"));
    action_insert_remark->setDisabled(true);
    connect(action_insert_remark, &QAction::triggered, this, &MainWindow::onActionInsertRemark);

    action_insert_smooth = new QAction(tr("&平滑"), this);
    action_insert_smooth->setStatusTip(tr("角度平滑"));
    action_insert_smooth->setDisabled(true);
    connect(action_insert_smooth, &QAction::triggered, this, &MainWindow::onActionInsertSmooth);

    action_insert_margin = new QAction(tr("&边距/余量"), this);
    action_insert_margin->setStatusTip(tr("插入边距或余量"));
    action_insert_margin->setDisabled(true);
    connect(action_insert_margin, &QAction::triggered, this, &MainWindow::onActionInsertMargin);

    action_insert_canal = new QAction(tr("&管道"), this);
    action_insert_canal->setStatusTip(tr("插入管道"));
    action_insert_canal->setDisabled(true);
    connect(action_insert_canal, &QAction::triggered, this, &MainWindow::onActionInsertCanel);

    action_insert_block = new QAction(tr("&块"), this);
    action_insert_block->setStatusTip(tr("在当前图样插入块"));
    action_insert_block->setDisabled(true);
    connect(action_insert_block, &QAction::triggered, this, &MainWindow::onActionInsertBlock);

    action_insert_holes_over_a_line = new QAction(tr("&沿线孔"), this);
    action_insert_holes_over_a_line->setStatusTip(tr("插入沿线孔"));
    action_insert_holes_over_a_line->setDisabled(true);
    connect(action_insert_holes_over_a_line, &QAction::triggered, this, &MainWindow::onActionInsertHolesOverALine);

    action_insert_array_rectangular = new QAction(tr("&矩形"), this);
    action_insert_array_rectangular->setStatusTip(tr("创建矩形阵列"));
    action_insert_array_rectangular->setDisabled(true);
    connect(action_insert_array_rectangular, &QAction::triggered, this, &MainWindow::onActionInsertArrayRectangle);

    action_insert_array_circular = new QAction(tr("&圆形"), this);
    action_insert_array_circular->setStatusTip(tr("创建圆形阵列"));
    action_insert_array_circular->setDisabled(true);
    connect(action_insert_array_circular, &QAction::triggered, this, &MainWindow::onActionInsertArrayCircular);
// ![3] 插入

// ![4] 修改
    action_modify_undo = new QAction(tr("&撤销"), this);
    action_modify_undo->setShortcut(QKeySequence::Undo);
    action_modify_undo->setStatusTip(tr("撤销上一个操作"));
    action_modify_undo->setDisabled(true);
    connect(action_modify_undo, &QAction::triggered, this, &MainWindow::onActionModifyUndo);

    action_modify_redo = new QAction(tr("&重做"), this);
    action_modify_redo->setShortcut(QKeySequence::Redo);
    action_modify_redo->setDisabled(true);
    action_modify_redo->setStatusTip(tr("重做撤销的操作"));
    connect(action_modify_redo, &QAction::triggered, this, &MainWindow::onActionModifyRedo);

    action_modify_clear = new QAction(tr("&清空"), this);
    action_modify_clear->setStatusTip(tr("清空画板"));
    action_modify_clear->setDisabled(true);
    connect(action_modify_clear, &QAction::triggered, this, &MainWindow::onActionModifyClear);

    action_modify_delete = new QAction(tr("&删除"), this);
    action_modify_delete->setShortcut(QKeySequence::Delete);
    action_modify_delete->setStatusTip(tr("删除实体"));
    action_modify_delete->setDisabled(true);
    connect(action_modify_delete, &QAction::triggered, this, &MainWindow::onActionModifyDelete);

    action_modify_cut = new QAction(tr("&剪切"), this);
    action_modify_cut->setShortcut(QKeySequence::Cut);
    action_modify_cut->setStatusTip(tr("剪切选择并将其放在剪贴板上"));
    action_modify_cut->setDisabled(true);
    connect(action_modify_cut, &QAction::triggered, this, &MainWindow::onActionModifyCut);

    action_modify_copy = new QAction(tr("&复制"), this);
    action_modify_copy->setShortcut(QKeySequence::Copy);
    action_modify_copy->setStatusTip(tr("将所选线复制到粘贴板"));
    action_modify_copy->setDisabled(true);
    connect(action_modify_copy, &QAction::triggered, this, &MainWindow::onActionModifyCopy);

    action_modify_paste = new QAction(tr("&粘贴"), this);
    action_modify_paste->setShortcut(QKeySequence::Paste);
    action_modify_paste->setStatusTip(tr("粘贴先前复制的实体"));
    action_modify_paste->setDisabled(true);
    connect(action_modify_paste, &QAction::triggered, this, &MainWindow::onActionModifyPaste);

    action_modify_paste_as_slave = new QAction(tr("&粘贴为副本"), this);
    action_modify_paste_as_slave->setStatusTip(tr("粘贴为副本"));
    action_modify_paste_as_slave->setDisabled(true);
    connect(action_modify_paste_as_slave, &QAction::triggered, this, &MainWindow::onActionModifyPasteAsSlave);

    action_modify_transform_move = new QAction(tr("&移动"), this);
    action_modify_transform_move->setStatusTip(tr("移动实体"));
    action_modify_transform_move->setDisabled(true);
    connect(action_modify_transform_move, &QAction::triggered, this, &MainWindow::onActionModifyTransformMove);

    action_modify_transform_rotate = new QAction(tr("&旋转"), this);
    action_modify_transform_rotate->setStatusTip(tr("旋转实体"));
    action_modify_transform_rotate->setDisabled(true);
    connect(action_modify_transform_rotate, &QAction::triggered, this, &MainWindow::onActionModifyTransformRotate);

    action_modify_transform_rotate_knots_continuely = new QAction(tr("&连续旋转节点"), this);
    action_modify_transform_rotate_knots_continuely->setDisabled(true);
    connect(action_modify_transform_rotate_knots_continuely, &QAction::triggered, this, &MainWindow::onActionModifyTransformRotateKnotsContinuely);

    action_modify_transform_rotate_knots = new QAction(tr("&旋转节点"), this);
    action_modify_transform_rotate_knots->setDisabled(true);
    connect(action_modify_transform_rotate_knots, &QAction::triggered, this, &MainWindow::onActionModifyTransformRotateKnots);

    action_modify_transform_scale = new QAction(tr("&比例"), this);
    action_modify_transform_scale->setDisabled(true);
    connect(action_modify_transform_scale, &QAction::triggered, this, &MainWindow::onActionModifyTransformScale);

    action_modify_transform_mirror = new QAction(tr("&镜像"), this);
    action_modify_transform_mirror->setDisabled(true);
    connect(action_modify_transform_mirror, &QAction::triggered, this, &MainWindow::onActionModifyTransformMirror);

    action_modify_transform_strech = new QAction(tr("&拉伸"), this);
    action_modify_transform_strech->setDisabled(true);
    connect(action_modify_transform_strech, &QAction::triggered, this, &MainWindow::onActionModifyTransformStrech);

    action_modify_transform_explode = new QAction(tr("&爆炸"), this);
    action_modify_transform_explode->setDisabled(true);
    connect(action_modify_transform_explode, &QAction::triggered, this, &MainWindow::onActionModifyTransformExplode);

    action_modify_transform_join = new QAction(tr("&联接"), this);
    action_modify_transform_join->setDisabled(true);
    connect(action_modify_transform_join, &QAction::triggered, this, &MainWindow::onActionModifyTransformJoin);

    action_modify_transform_break = new QAction(tr("&断开"), this);
    action_modify_transform_break->setDisabled(true);
    connect(action_modify_transform_break, &QAction::triggered, this, &MainWindow::onActionModifyTransformBreak);

    action_modify_transform_fillet = new QAction(tr("&倒圆角"), this);
    action_modify_transform_fillet->setDisabled(true);
    connect(action_modify_transform_fillet, &QAction::triggered, this, &MainWindow::onActionModifyTransformfillet);

    action_modify_transform_V_cut = new QAction(tr("&'V'切"), this);
    action_modify_transform_V_cut->setDisabled(true);
    connect(action_modify_transform_V_cut, &QAction::triggered, this, &MainWindow::onActionModifyTransformVCut);

    action_modify_transform_spline = new QAction(tr("&创建样条"), this);
    action_modify_transform_spline->setDisabled(true);
    connect(action_modify_transform_spline, &QAction::triggered, this, &MainWindow::onActionModifyTransformSpline);

//    action_shear = new QAction(tr("&拉伸"), this);
//    action_shear->setStatusTip(tr("拉伸画板"));

    action_modify_trim = new QAction(tr("&修剪"), this);
    action_modify_trim->setDisabled(true);
    connect(action_modify_trim, &QAction::triggered, this, &MainWindow::onActionModifyTrim);

    action_modify_extend = new QAction(tr("&延伸"), this);
    action_modify_extend->setDisabled(true);
    connect(action_modify_extend, &QAction::triggered, this, &MainWindow::onActionModifyExtend);

    action_modify_modify_endpoint = new QAction(tr("&修改端点"), this);
    action_modify_modify_endpoint->setDisabled(true);
    connect(action_modify_modify_endpoint, &QAction::triggered, this, &MainWindow::onActionModifyEndpoint);

    action_modify_adjust_polyline = new QAction(tr("&调整折线"), this);
    action_modify_adjust_polyline->setDisabled(true);
    connect(action_modify_adjust_polyline, &QAction::triggered, this, &MainWindow::onActionModifyAdjustPolyLine);

    action_modify_start_point = new QAction(tr("&起点"), this);
    action_modify_start_point->setDisabled(true);
    connect(action_modify_start_point, &QAction::triggered, this, &MainWindow::onActionModifyStartpoint);

    action_modify_soften_up = new QAction(tr("&软化"), this);
    action_modify_soften_up->setDisabled(true);
    connect(action_modify_soften_up, &QAction::triggered, this, &MainWindow::onActionModifySoftenup);

    action_modify_warp = new QAction(tr("&弯曲"), this);
    action_modify_warp->setDisabled(true);
    connect(action_modify_warp, &QAction::triggered, this, &MainWindow::onActionModifyWarp);

    action_modify_knots_reduction = new QAction(tr("&减少节点"), this);
    action_modify_knots_reduction->setDisabled(true);
    connect(action_modify_knots_reduction, &QAction::triggered, this, &MainWindow::onActionModifyKnotsReduction);

    action_modify_replace_part = new QAction(tr("&更换部分"), this);
    action_modify_replace_part->setDisabled(true);
    connect(action_modify_replace_part, &QAction::triggered, this, &MainWindow::onActionModifyReplacePart);

    action_modify_change_to = new QAction(tr("&更改至"), this);
    action_modify_change_to->setDisabled(true);
    connect(action_modify_change_to, &QAction::triggered, this, &MainWindow::onActionModifyChangeTo);

    action_modify_make_master_line = new QAction(tr("&制作主线"), this);
    connect(action_modify_make_master_line, &QAction::triggered, this, &MainWindow::onActionModifyMakeMasterLine);

    action_modify_style = new QAction(tr("&实体线样式"), this);
    action_modify_style->setDisabled(true);
    connect(action_modify_style, &QAction::triggered, this, &MainWindow::onActionModifyStyle);


    action_modify_springing = new QAction(tr("&Springing弹回"), this);
    connect(action_modify_springing, &QAction::triggered, this, &MainWindow::onActionModifySpringing);
// ![4] 修改

// ![5] 图样
    action_pattern_select = new QAction(tr("&选择"), this);
    action_pattern_select->setDisabled(true);
    connect(action_pattern_select, &QAction::triggered, this, &MainWindow::onActionPatternSelect);

    action_pattern_new = new QAction(tr("&新建"), this);
    connect(action_pattern_new, &QAction::triggered, this, &MainWindow::onActionPatternNew);

    action_pattern_clone = new QAction(tr("&克隆"), this);
    connect(action_pattern_clone, &QAction::triggered, this, &MainWindow::onActionPatternClone);

    action_pattern_delete = new QAction(tr("&删除"), this);
    action_pattern_delete->setDisabled(true);
    connect(action_pattern_delete, &QAction::triggered, this, &MainWindow::onActionPatternDelete);

    action_pattern_rotate = new QAction(tr("&旋转"), this);
    connect(action_pattern_rotate, &QAction::triggered, this, &MainWindow::onActionPatternRotate);

    action_pattern_make_symmetic = new QAction(tr("&使对称"), this);
    action_pattern_make_symmetic->setDisabled(true);
    connect(action_pattern_make_symmetic, &QAction::triggered, this, &MainWindow::onActionPatternMakeSymmetic);

    action_pattern_position = new QAction(tr("&位置"), this);
    action_pattern_position->setDisabled(true);
    connect(action_pattern_position, &QAction::triggered, this, &MainWindow::onActionPatternPosition);

    action_pattern_extract_auto = new QAction(tr("&提取(自动)"), this);
    action_pattern_extract_auto->setDisabled(true);
    connect(action_pattern_extract_auto, &QAction::triggered, this, &MainWindow::onActionPatternExtractAuto);

    action_pattern_extract_manual = new QAction(tr("&提取(手动)"), this);
    action_pattern_extract_manual->setDisabled(true);
    connect(action_pattern_extract_manual, &QAction::triggered, this, &MainWindow::onActionPatternExtractManual);

    action_pattern_find_perimeter = new QAction(tr("&查找周长"), this);
    action_pattern_find_perimeter->setDisabled(true);
    connect(action_pattern_find_perimeter, &QAction::triggered, this, &MainWindow::onActionPatternPerimeter);

    action_pattern_close_trim = new QAction(tr("&关闭和修剪"), this);
    action_pattern_close_trim->setDisabled(true);
    connect(action_pattern_close_trim, &QAction::triggered, this, &MainWindow::onActionPatternTrim);

    action_pattern_check = new QAction(tr("&检查"), this);
    action_pattern_check->setDisabled(true);
    connect(action_pattern_check, &QAction::triggered, this, &MainWindow::onActionPatternCheck);

    action_pattern_devide = new QAction(tr("&分裂/瓜分"), this);
    action_pattern_devide->setDisabled(true);
    connect(action_pattern_devide, &QAction::triggered, this, &MainWindow::onActionPatternDevide);

    action_pattern_save_as_weft = new QAction(tr("&另存为纬线"), this);
    action_pattern_save_as_weft->setDisabled(true);
    connect(action_pattern_save_as_weft, &QAction::triggered, this, &MainWindow::onActionPatternSaveAsWeft);
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
    connect(action_view_knots, &QAction::toggled, this, &MainWindow::onActionViewKnots);

    action_view_image = new QAction(tr("&图片"), this);
    action_view_image->setCheckable(true);
    action_view_image->setStatusTip(tr("显示/隐藏图片"));
    action_view_image->setDisabled(true);
    connect(action_view_image, &QAction::toggled, this, &MainWindow::onActionViewImage);

    action_view_design_rules = new QAction(tr("&设计尺"), this);
    action_view_design_rules->setCheckable(true);
    action_view_design_rules->setStatusTip(tr("显示/隐藏绘图规则"));
    action_view_design_rules->setDisabled(true);
    connect(action_view_design_rules, &QAction::toggled, this, &MainWindow::onActionViewDesignRules);

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
    connect(action_view_zoom_back, &QAction::triggered, this, &MainWindow::onActionViewZoomBack);

    action_view_voerlap_patterns = new QAction(tr("&覆盖图样"), this);
    connect(action_view_voerlap_patterns, &QAction::triggered, this, &MainWindow::onActionViewVoerlapPatterns);

    action_view_hide = new QAction(tr("&隐藏"), this);
    connect(action_view_hide, &QAction::triggered, this, &MainWindow::onActionViewHide);

    action_view_show = new QAction(tr("&显示"), this);
    action_view_show->setDisabled(true);
    connect(action_view_show, &QAction::triggered, this, &MainWindow::onActionViewShow);

    action_view_clean = new QAction(tr("&清除"), this);
    action_view_clean->setDisabled(true);
    connect(action_view_clean, &QAction::triggered, this, &MainWindow::onActionViewClean);

    action_view_set_reference = new QAction(tr("&设置引用参考"), this);
    action_view_set_reference->setDisabled(true);
    connect(action_view_set_reference, &QAction::triggered, this, &MainWindow::onActionViewSetReference);

    action_view_lock_layout = new QAction(tr("&锁定布局"), this);
    action_view_lock_layout->setCheckable(true);
    connect(action_view_lock_layout, &QAction::toggled, this, &MainWindow::onActionViewLockLayout);

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

    action_view_tool_customize = new QAction(tr("&自定义"),this);
    connect(action_view_tool_customize, &QAction::toggled, this, &MainWindow::onActionViewToolCustomizeToggled);

    action_view_status_bar = new QAction(tr("&状态栏"), this);
    action_view_status_bar->setCheckable(true);
    action_view_status_bar->setChecked(true);
    connect(action_view_status_bar, &QAction::toggled, this, &MainWindow::onActionViewStatusBar);
// ![6] 查看

// ![7] 效用
    action_utility_measure_distance = new QAction(tr("&测量距离"), this);
    connect(action_utility_measure_distance, &QAction::triggered, this, &MainWindow::onActionUtilityMeasureDis);

    action_utility_measure_line = new QAction(tr("&测量线长"), this);
    connect(action_utility_measure_line, &QAction::triggered, this, &MainWindow::onActionUtilityMeasureLine);

    action_create_pattern_card = new QAction(tr("&创建图样卡"), this);
    action_create_pattern_card->setDisabled(true);
    connect(action_create_pattern_card, &QAction::triggered, this, &MainWindow::onActionCreatorPatternCard);

    action_utility_quick_nesting = new QAction(tr("&快速排版"), this);
    connect(action_utility_quick_nesting, &QAction::triggered, this, &MainWindow::onActionUtilityQuickNesting);

    action_utility_nest_by_job_card = new QAction(tr("&转到排版"), this);
    connect(action_utility_nest_by_job_card, &QAction::triggered, this, &MainWindow::onActionUtilityNestByJobCard);

    action_utility_heel_cover = new QAction(tr("&后跟..."), this);
    connect(action_utility_heel_cover, &QAction::triggered, this, &MainWindow::onActionUtilityHeelCover);

    action_utility_pattern_search = new QAction(tr("&搜索图样"), this);
    action_utility_pattern_search->setDisabled(true);
    connect(action_utility_pattern_search, &QAction::triggered, this, &MainWindow::onActionUtilityPatternSearch);

    // ![7] 效用

// ![8] 级放
    action_grading_parameters = new QAction(tr("&图样"), this);
    connect(action_grading_parameters, &QAction::triggered, this, &MainWindow::onActionGradingParameters);

    action_grading_grade = new QAction(tr("&级放"), this);
    action_grading_grade->setDisabled(true);
    connect(action_grading_grade, &QAction::triggered, this, &MainWindow::onActionGradingGrade);

    action_grading_delete_graded_patterns = new QAction(tr("&删除级放图样"), this);
    action_grading_delete_graded_patterns->setDisabled(true);
    connect(action_grading_delete_graded_patterns, &QAction::triggered, this, &MainWindow::onActionGradingDeletedGradedPatterns);

    action_grading_reference_insert = new QAction(tr("插入"), this);
    action_grading_reference_insert->setDisabled(true);
    connect(action_grading_reference_insert, &QAction::triggered, this, &MainWindow::onActionGradingReferenceInsert);

    action_grading_keep_margin = new QAction(tr("&保留 边距/余量"), this);
    action_grading_keep_margin->setDisabled(true);
    connect(action_grading_keep_margin, &QAction::triggered, this, &MainWindow::onActionGradingKeepMargin);

    action_grading_delete_margin = new QAction(tr("&删除 边距/余量"), this);
    action_grading_delete_margin->setDisabled(true);
    connect(action_grading_delete_margin, &QAction::triggered, this, &MainWindow::onActionGradingDeleteMargin);

    action_grading_update_grading = new QAction(tr("&更新级放"), this);
    action_grading_update_grading->setDisabled(true);
    connect(action_grading_update_grading, &QAction::triggered, this, &MainWindow::onActionGradingUpdate);

    action_grading_XY_lock = new QAction(tr("&XY 锁定"), this);
    action_grading_XY_lock->setDisabled(true);
    connect(action_grading_XY_lock, &QAction::triggered, this, &MainWindow::onActionGradingXYLock);

    action_grading_delete_XY_lock = new QAction(tr("&删除XY 锁定 "), this);
    action_grading_delete_XY_lock->setDisabled(true);
    connect(action_grading_delete_XY_lock, &QAction::triggered, this, &MainWindow::onActionGradingDeleteXYLock);

    action_grading_horizontal_zone = new QAction(tr("&水平区"), this);
    connect(action_grading_horizontal_zone, &QAction::triggered, this, &MainWindow::onActionGradingHorizontalZone);

    action_grading_vertical_zone = new QAction(tr("&垂直区"), this);
    connect(action_grading_vertical_zone, &QAction::triggered, this, &MainWindow::onActionGradingVerticalZone);

    action_grading_delete_zone = new QAction(tr("&删除区域"), this);
    connect(action_grading_delete_zone, &QAction::triggered, this, &MainWindow::onActionGradingDeleteZone);

// ![8] 级放

// ![10] 帮助
    action_help_help = new QAction(tr("&帮助(&H)"), this);
    action_help_help->setShortcut(QKeySequence::HelpContents);
    connect(action_help_help, &QAction::triggered, this, &MainWindow::onActionHelpHelp);

    action_help_license = new QAction(tr("&许可(&L)"), this);
    connect(action_help_license, &QAction::triggered, this, &MainWindow::onActionHelpLicense);

    action_help_about = new QAction(tr("&关于"), this);
    connect(action_help_about, &QAction::triggered, this, &MainWindow::onActionHelpAbout);
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
    action_object_snap_end_point = new QAction(tr("&实体端点"),this);
    action_object_snap_end_point->setCheckable(true);
    action_object_snap_end_point->setChecked(false);
    action_object_snap_end_point->setStatusTip(tr("实体端点"));
    connect(action_object_snap_end_point, &QAction::triggered, this, &MainWindow::onActionObjectSnapEndpoint);

    action_object_snap_center = new QAction(tr("&实体中心"),this);
    action_object_snap_center->setCheckable(true);
    action_object_snap_center->setChecked(false);
    action_object_snap_center->setStatusTip(tr("实体中心"));
    connect(action_object_snap_center, &QAction::triggered, this, &MainWindow::onActionObjectSnapCenter);


    action_object_snap_intersection = new QAction(tr("&交叉点"),this);
    action_object_snap_intersection->setCheckable(true);
    action_object_snap_intersection->setChecked(false);
    action_object_snap_intersection->setStatusTip(tr("交叉点"));
    connect(action_object_snap_intersection, &QAction::triggered, this, &MainWindow::onActionObjectSnapInterSection);


    action_object_snap_knot = new QAction(tr("&节点"),this);
    action_object_snap_knot->setCheckable(true);
    action_object_snap_knot->setChecked(false);
    action_object_snap_knot->setStatusTip(tr("节点"));
    connect(action_object_snap_knot, &QAction::triggered, this, &MainWindow::onActionObjectSnapKnot);


    action_object_snap_ortho = new QAction(tr("&直角正交"),this);
    action_object_snap_ortho->setCheckable(true);
    action_object_snap_ortho->setChecked(false);
    action_object_snap_ortho->setStatusTip(tr("直角正交"));
    connect(action_object_snap_ortho, &QAction::triggered, this, &MainWindow::onActionObjectSnapOrtho);


    action_object_snap_enter_coordinates = new QAction(tr("&输入坐标"),this);
    action_object_snap_enter_coordinates->setCheckable(true);
    action_object_snap_enter_coordinates->setChecked(false);
    action_object_snap_enter_coordinates->setStatusTip(tr("输入坐标"));
    connect(action_object_snap_enter_coordinates, &QAction::triggered, this, &MainWindow::onActionObjectSnapEnterCoordinates);

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

    menu_view_tool_bar = new QMenu(tr("&工具栏"), this);
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
    tool_slide = new QToolBar(tr("边栏"), this);
    addToolBar(Qt::LeftToolBarArea, tool_slide);
    tool_slide->setMaximumWidth(50);
    tool_slide->setOrientation(Qt::Vertical);
    tool_slide->setAllowedAreas(Qt::LeftToolBarArea);
    tool_slide->setFloatable(false);
    tool_slide->setMovable(false);
    tool_slide->addActions(action_tool_slide->actions());
    onToolSlideChanged();
// ![1] 边栏工具栏

// ![2] 标准工具栏
    tool_standard = new QToolBar(tr("标准"), this);
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
    tool_sketch = new QToolBar(tr("草图"), this);
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
    tool_object_snap = new QToolBar(tr("对象捕捉"), this);
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
    tool_zoom = new QToolBar(tr("缩放"), this);
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
    tool_modify = new QToolBar(tr("修改"), this);
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
    tool_image = new QToolBar(tr("图片"), this);
    tool_image->setOrientation(Qt::Horizontal);
    tool_image->setAllowedAreas(Qt::AllToolBarAreas);

    tool_image->addAction(action_draw_image_insert);
    tool_image->addAction(action_draw_image_delete);
    tool_image->addAction(action_draw_image_crop);
    tool_image->addAction(action_draw_image_calibrate);
    tool_image->addAction(action_draw_image_agjustement);
// ![8] 图片

// ![9] 数字化仪
    tool_digitizer = new QToolBar(tr("数字化仪"), this);
    tool_digitizer->setOrientation(Qt::Horizontal);
    tool_digitizer->setAllowedAreas(Qt::AllToolBarAreas);

    tool_digitizer->addAction(action_draw_digitizer_activate);
    tool_digitizer->addAction(action_draw_digitizer_set_position);
    tool_digitizer->addAction(action_draw_digitizer_configure);
// ![9] 数字化仪

// ![10] 级放
    tool_grading = new QToolBar(tr("级放"), this);
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
    tool_scanner = new QToolBar(tr("扫描仪"), this);
    tool_scanner->setOrientation(Qt::Horizontal);
    tool_scanner->setAllowedAreas(Qt::AllToolBarAreas);

    tool_scanner->addAction(action_draw_scanner_vectorize_from_scanner);
    tool_scanner->addAction(action_draw_scanner_select_source);
    tool_scanner->addAction(action_draw_scanner_vectorize_image);
// ![11] 扫描仪

// ![12] 插入
    tool_insert = new QToolBar(tr("插入"), this);
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
    tool_pattern = new QToolBar(tr("图样"), this);
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
    mousePositionLabel = new QLabel(tr(""), this);
    mousePositionLabel->setMargin(1);
    statusBar()->addPermanentWidget(mousePositionLabel);
}

void MainWindow::initDockWidget()
{
    //删除中央窗体
    QWidget* p = takeCentralWidget();
    if(p) delete p;

    // 设置dock窗口嵌套
    setDockNestingEnabled(DockNestingEnabled);

    // 新建dock窗口
    dock_find_style = new QDockWidget(tr("查找款型"), this);     // 添加find_style浮动窗口
    dock_project = new QDockWidget(tr("项目"), this);        // 添加project浮动窗口
    dock_properties = new QDockWidget(tr("属性"), this);     // 添加properties浮动窗口
    dock_scene= new QDockWidget(this);      // 添加绘图区

    dock_find_style->setHidden(!action_view_tool_find_style->isChecked());
    dock_project->setHidden(!action_view_tool_project->isChecked());
    dock_properties->setHidden(!action_view_tool_properties->isChecked());

//    connect(dock_find_style, &QDockWidget::visibilityChanged, this, &MainWindow::onDockFindStyleVisibilityChanged);
//    connect(dock_project, &QDockWidget::visibilityChanged, this, &MainWindow::onDockProjectVisibilityChanged);
//    connect(dock_properties, &QDockWidget::visibilityChanged, this, &MainWindow::onDockPropertiesVisibilityChanged);

    // 绘图区隐藏标题栏
    QWidget * qw=new QWidget(this);
    dock_scene->setTitleBarWidget(qw);

    // 绘图区不可移动、不可关闭、不可最小化
    dock_scene->setFeatures(QDockWidget::NoDockWidgetFeatures);

    // 将窗口添加至主窗口
    addDockWidget(Qt::LeftDockWidgetArea,dock_find_style);
    splitDockWidget(dock_find_style,dock_project,Qt::Horizontal);
    splitDockWidget(dock_project, dock_scene, Qt::Horizontal);
    splitDockWidget(dock_project,dock_properties,Qt::Vertical);
}

void MainWindow::initConfiguration()
{
    qDebug() << "初始化配置文件";
//    if(configCopy) delete configCopy;
    configCopy = new Configure(this);
    connect(this, &MainWindow::configChanged, configCopy, &Configure::onConfigChanged);
    action_view_xy_axes->setChecked(configCopy->axesGrid.axes.showAxes);
    action_view_grid->setChecked(configCopy->axesGrid.grid.showGrid);
    action_view_grading_rules->setChecked(configCopy->view.gradingRules);
    action_view_filled_patterns->setChecked(configCopy->view.filledPatterns);
    action_view_tool_find_style->setChecked(configCopy->view.toolFindStyle);
    action_view_tool_project->setChecked(configCopy->view.toolProject);
    action_view_tool_properties->setChecked(configCopy->view.toolProperties);

    for(int i=0; i<project_list.length();i++){
        for(int j=0; j<project_list.at(i)->getSceneList().length();j++){
            project_list.at(i)->getScene(j)->setCurShape(Shape::None);
            project_list.at(i)->getScene(j)->setEntityStyle(configCopy->eStyle);
            project_list.at(i)->getScene(j)->setAxesGrid(configCopy->axesGrid);
        }
    }
}

void MainWindow::initProjectView()
{
    view = new View(this);    // 初始化view
    connect(view, &View::mousePositionChanged, this, &MainWindow::onMousePositionChanged);

    dock_scene->setWidget(view);    // 将给视图加入到dock_scene

    tree_project = new QTreeWidget(this);
    tree_project->setColumnCount(1); //设置列数
    tree_project->setHeaderLabel(tr("项目列表")); //设置头的标题
    tree_project->setContextMenuPolicy(Qt::CustomContextMenu);//右键 不可少否则右键无反应
    connect(tree_project, &QTreeWidget::customContextMenuRequested, this, &MainWindow::showTreeMenu);
    connect(tree_project, &QTreeWidget::itemDoubleClicked, this, &MainWindow::onTreeProjectItemDoubleClicked);

    dock_project->setWidget(tree_project);

    addProject();  // 添加项目
}

void MainWindow::addProject()
{
    // 初始化项目名称
    QString name_project_new = getNewProjectName();
    project_active = new Project(this);
    project_active->setName(name_project_new);
    // 在项目列表中加入该项目
    project_list.append(project_active);

    // 初始化图层名称
    QString name_scene_new = project_active->getNewSceneName();
    scene_active = project_active->addScene();
    scene_active->setName(name_scene_new);

    connect(scene_active, &Scene::sceneItemsChanged, this, &MainWindow::onSceneItemsChanged);

    QTreeWidgetItem *item_project = new QTreeWidgetItem(tree_project,QStringList(name_project_new));
    QTreeWidgetItem *item_scene = new QTreeWidgetItem(item_project,QStringList(name_scene_new)); //子节点1
    tree_project_item_list.append(item_project);

    connect(project_active, &Project::projectNameChanged, this, &MainWindow::onProjectNameChanged);

    item_scene->setCheckState(0, Qt::Checked);
    item_project->addChild(item_scene); //添加子节点
    tree_project->expandAll(); //结点全部展开
    updateScene();
}

QString MainWindow::getNewProjectName()
{
    // 获取项目数目
    int len_project = project_list.length() + 1;
    QString str = QString::number(len_project, 10);
    char *ch;
    QByteArray ba = str.toLatin1();
    ch = ba.data();
    return tr("新项目") + tr(ch);
}

Project *MainWindow::getProjectByName(QString project_name)
{
    for(int i=0;i<project_list.length();i++){
        if(project_list.at(i)->getName() == project_name){
            return project_list.at(i);
        }
    }
    return NULL;
}

void MainWindow::showTreeMenu(QPoint pos)
{
    tree_project_scene_active_item = tree_project->itemAt(pos); //可得到右键条目
    if(tree_project_scene_active_item == NULL){  //这种情况是右键的位置不在treeItem的范围内,即在空白位置右击,则显示
        menu_tree = new QMenu(tree_project);
        action_tree_expand_all = new QAction(tr("展开全部"), tree_project);
        action_tree_fold_all = new QAction(tr("折叠全部"), tree_project);
        connect(action_tree_expand_all, &QAction::triggered, this, &MainWindow::onActionTreeExpandAll);
        connect(action_tree_fold_all, &QAction::triggered, this, &MainWindow::onActionTreeFoldAll);
        menu_tree->addAction(action_tree_expand_all);
        menu_tree->addAction(action_tree_fold_all);
        menu_tree->exec(QCursor::pos());  //在当前鼠标位置显示
        return;
    }
    tree_project_active_item = tree_project_scene_active_item->parent();
    if(tree_project_active_item == NULL){     // 项目栏
        tree_project_active_item = tree_project_scene_active_item;
        tree_project_scene_active_item = NULL;
        menu_tree_project = new QMenu(tree_project);
        action_tree_project_add_scene = new QAction(tr("添加图层"), tree_project);
        action_tree_project_save = new QAction(tr("保存"), tree_project);
        action_tree_project_save_as = new QAction(tr("另存为"), tree_project);
        action_tree_project_rename = new QAction(tr("重命名"), tree_project);
        action_tree_project_close = new QAction(tr("关闭"), tree_project);

        connect(action_tree_project_add_scene, &QAction::triggered, this, &MainWindow::onActionTreeProjectAddScene);
        connect(action_tree_project_save, &QAction::triggered, this, &MainWindow::onActionTreeProjectSave);
        connect(action_tree_project_save_as, &QAction::triggered, this, &MainWindow::onActionTreeProjectSaveAs);
        connect(action_tree_project_rename, &QAction::triggered, this, &MainWindow::onActionTreeProjectRename);
        connect(action_tree_project_close, &QAction::triggered, this, &MainWindow::onActionTreeProjectClose);

        menu_tree_project->addAction(action_tree_project_rename);
        menu_tree_project->addAction(action_tree_project_add_scene);
        menu_tree_project->addAction(action_tree_project_save);
        menu_tree_project->addAction(action_tree_project_save_as);
        menu_tree_project->addAction(action_tree_project_close);
        menu_tree_project->exec(QCursor::pos());  //在当前鼠标位置显示
    } else{                 // 图层栏
        menu_tree_project_scene = new QMenu(tree_project);
        action_tree_project_scene_change_to = new QAction(tr("切换至该图层"), tree_project);

        menu_tree_project_scene_move_up = new QMenu(tr("上移"), tree_project);

        action_tree_project_scene_move_up_one = new QAction(tr("上移一层"), tree_project);
        action_tree_project_scene_move_up_top = new QAction(tr("移至顶部"), tree_project);
        menu_tree_project_scene_move_up->addAction(action_tree_project_scene_move_up_one);
        menu_tree_project_scene_move_up->addAction(action_tree_project_scene_move_up_top);

        menu_tree_project_scene_move_down = new QMenu(tr("下移"));
        action_tree_project_scene_move_down_one = new QAction(tr("下移一层"), tree_project);
        action_tree_project_scene_move_down_bottom = new QAction(tr("移至底层"), tree_project);
        menu_tree_project_scene_move_down->addAction(action_tree_project_scene_move_down_one);
        menu_tree_project_scene_move_down->addAction(action_tree_project_scene_move_down_bottom);

        int scene_length = tree_project_scene_active_item->parent()->childCount();
        if(scene_length <= 1){
            action_tree_project_scene_move_up_one->setEnabled(false);
            action_tree_project_scene_move_up_top->setEnabled(false);
            action_tree_project_scene_move_down_one->setEnabled(false);
            action_tree_project_scene_move_down_bottom->setEnabled(false);
        }

        action_tree_project_scene_rename = new QAction(tr("重命名"), tree_project);
        action_tree_project_scene_delete = new QAction(tr("删除"), tree_project);

        connect(action_tree_project_scene_change_to, &QAction::triggered, this, &MainWindow::onActionTreeProjectSceneChangeTo);
        connect(action_tree_project_scene_move_up_one, &QAction::triggered, this, &MainWindow::onActionTreeProjectSceneMoveUpOne);
        connect(action_tree_project_scene_move_up_top, &QAction::triggered, this, &MainWindow::onActionTreeProjectSceneMoveUpTop);
        connect(action_tree_project_scene_move_down_one, &QAction::triggered, this, &MainWindow::onActionTreeProjectSceneMoveDownOne);
        connect(action_tree_project_scene_move_down_bottom, &QAction::triggered, this, &MainWindow::onActionTreeProjectSceneMoveDownBottom);
        connect(action_tree_project_scene_rename, &QAction::triggered, this, &MainWindow::onActionTreeProjectSceneRename);
        connect(action_tree_project_scene_delete, &QAction::triggered, this, &MainWindow::onActionTreeProjectSceneDelete);

        menu_tree_project_scene->addAction(action_tree_project_scene_change_to);
        menu_tree_project_scene->addMenu(menu_tree_project_scene_move_up);
        menu_tree_project_scene->addMenu(menu_tree_project_scene_move_down);
        menu_tree_project_scene->addAction(action_tree_project_scene_rename);
        menu_tree_project_scene->addAction(action_tree_project_scene_delete);
        menu_tree_project_scene->exec(QCursor::pos());  //在当前鼠标位置显示
    }
}

void MainWindow::updateScene()
{
    // 设置图层样式
    scene_active->setEntityStyle(configCopy->eStyle);
    scene_active->setAxesGrid(configCopy->axesGrid);

    // 更新窗口名称
    setWindowTitle("CADPRO-<" + project_active->getName() + "-" + scene_active->getName() + ">");
    // 在视图中更新图层
    connect(view, &View::viewScaleChanged, scene_active, &Scene::onViewScaleChanged);
    view->setScene(project_active->getActiveScene());
}

bool MainWindow::maybeSave()
{
    while(project_list.length() != 0){
        project_active = project_list.front();
        if(project_active->isModified()){
            QMessageBox::StandardButton box;
            box = QMessageBox::warning(this,tr("保存项目"),tr("您要存储对“") + project_active->getName() + tr("”所做的更改吗？"),
            QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
            if(box == QMessageBox::Yes) {
                if(onActionFileSave()) {
                    project_list.pop_front();
                }
                return maybeSave();
            } else if(box == QMessageBox::No) {
                project_list.pop_front();
            } else if(box == QMessageBox::Cancel) {
                return false;
            }
        }else{
            project_list.pop_front();
        }
    }
    return true;
}

bool MainWindow::saveFile(QString fileName)
{
    qDebug() << "save file " << fileName;
    return project_active->saveProject();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(maybeSave()) qApp->quit();
    else event->ignore();
}

void MainWindow::onActionFileNew()
{
    addProject();
}

void MainWindow::onActionFileOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        qDebug() << "open file";
    }
}

bool MainWindow::onActionFileSave()
{
    // 如果项目保存过，不再弹出保存框
    if(project_active->isSaved()) {
        // 如果项目被修改过，直接保存修改后的项目
        if(project_active->isModified()){
            return saveFile(project_active->getName());
        }
    } else{
        return onActionFileSaveAs();
    }
    return true;
}

bool MainWindow::onActionFileSaveAs()
{
    qDebug() << "saving files as ...";
    QString fileName = QFileDialog::getSaveFileName(this,tr("保存为"),project_active->getName());
    if(fileName.isEmpty()) {
        return false;
    } else{
        if(fileName != project_active->getName()){
            project_active->changeName(fileName);
        }
        // 保存逻辑
        project_active->saveProject();
        bool res = saveFile(fileName);
        project_active->setSaved(res);
        return res;
    }
}

bool MainWindow::onActionFileSaveAll()
{
    qDebug() << "saving all files";
}

void MainWindow::onActionFilePrint()
{
    qDebug() << "print";
}

void MainWindow::onActionFilePrintPreview()
{
    qDebug() << "file preview";
}

void MainWindow::onActionFilePrintSetup()
{
    qDebug() << "print setup";
}

void MainWindow::onActionFileImportDXF()
{
    qDebug() << "import dxf files";
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开DXF文件"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        QFileInfo new_project = QFileInfo(fileName);
        QString name_project_new = new_project.fileName();
        project_active = new Project(this);
        project_active->setName(name_project_new);
        project_list.append(project_active);

        project_active->dxfFileParser(fileName);
        QTreeWidgetItem *item_project = new QTreeWidgetItem(tree_project, QStringList(name_project_new));
        tree_project_item_list.append(item_project);
        int count = project_active->getSceneList().length();

        for(int i=0;i<count;i++){
            QString name_scene_new = project_active->getSceneName(i);
            QTreeWidgetItem *item_scene = new QTreeWidgetItem(item_project, QStringList(name_scene_new)); //子节点1
            item_project->addChild(item_scene); //添加子节点
            item_scene->setCheckState(0, Qt::Checked);
        }
        scene_active = project_active->getScene(0);

        project_active->setActiveScene(scene_active);
        updateScene();
    }
}

void MainWindow::onActionFileImportDVS()
{
    qDebug() << "export dvs files";
}

void MainWindow::onActionFileImportTEF()
{
    qDebug() << "export tef files";
}

void MainWindow::onActionFileExportDXF()
{
    qDebug() << "export dxf files";
}

void MainWindow::onActionFileExportDVS()
{
    qDebug() << "export dvs files";
}

void MainWindow::onActionFileExportTEF()
{
    qDebug() << "export tef files";
}

void MainWindow::onActionFileConfiguration()
{
    ConfigureDialog configDialog(configCopy);
    configDialog.exec();

    initConfiguration();
}

void MainWindow::onActionFileExit()
{
    qDebug() << "退出提示保存文件";
}

void MainWindow::onActionDrawLine()
{
    qDebug() << "drawing a line";
    scene_active->setDrawable(true);
    scene_active->setCurShape(Shape::Line);
}

void MainWindow::onActionDrawEllipse()
{
    qDebug() << "drawing an ellipse";
    scene_active->setDrawable(true);
    scene_active->setCurShape(Shape::Ellipse);
}

void MainWindow::onActionDrawRect()
{
    qDebug() << "drawing a rectangle";
    scene_active->setDrawable(true);
    scene_active->setCurShape(Shape::Rectangle);
}

void MainWindow::onActionDrawMiddleAxis()
{
    qDebug() << "draw  middle axis";
    scene_active->setDrawable(true);
    scene_active->setCurShape(Shape::MiddleAxis);
}

void MainWindow::onActionDrawCircle()
{
    qDebug() << "draw a circle";
    scene_active->setDrawable(true);
    scene_active->setCurShape(Shape::Circle);
}

void MainWindow::onActionDrawPolyline()
{
    qDebug() << "draw a polyline";
    scene_active->setDrawable(true);
}

void MainWindow::onActionDrawArcBy3Pnts()
{
    qDebug() << "draw a tripoint arc ";
    scene_active->setDrawable(true);
    scene_active->setCurShape(Shape::Arc);
}

void MainWindow::onActionDrawArcBy3Pnts2()
{
    qDebug() << "draw tripoin arc 2";
    scene_active->setDrawable(true);
//    scene_active->setCurShape(Shape::Arc2);
}

void MainWindow::onActionDrawTrapezium()
{
    qDebug() << "draw a trapezium";
    scene_active->setDrawable(true);
}

void MainWindow::onActionDrawPolygon()
{
    qDebug() << "draw a polygon";
    scene_active->setDrawable(true);
}

void MainWindow::onActionDrawStabHole()
{
    qDebug() << "draw a stabhole";
    scene_active->setDrawable(true);
}

void MainWindow::onActionDrawReference()
{
    qDebug() << "draw a refer point";
    scene_active->setDrawable(true);
}

void MainWindow::onActionDrawEyelet()
{
    qDebug() << "draw an eyelet";
    scene_active->setDrawable(true);
}

void MainWindow::onActionDrawPatternDirection()
{
    qDebug() << "draw pattern direction";
    scene_active->setDrawable(true);
}

void MainWindow::onActionDrawShankLine()
{
    qDebug() << "上插线";
}

void MainWindow::onActionDrawPerpendicular()
{
    qDebug() << "垂直线";
}

void MainWindow::onActionDrawImage()
{
    qDebug() << "获取图像";
}

void MainWindow::onActionDrawImageInsert()
{
    qDebug() << "插入图像";
}

void MainWindow::onActionDrawImageDelete()
{
    qDebug() << "删除图像";
}

void MainWindow::onActionDrawImageCrop()
{
    qDebug() << "裁剪图像";
}

void MainWindow::onActionDrawImageCalibrate()
{
    qDebug() << "校准刻度";
}

void MainWindow::onActionDrawImageAdjustment()
{
    qDebug() << "调整图片";
}

void MainWindow::onActionDrawDigitizerActivate()
{
    qDebug() << "启用数字化仪";
}

void MainWindow::onActionDrawDigitizerSetPosition()
{
    qDebug() << "设置shell位置";
}

void MainWindow::onActionDrawDigitizerConfigue()
{
    qDebug() << "配置数字化仪";
}

void MainWindow::onActionDrawScannerVectorizeFromScanner()
{
    qDebug() << "扫描矢量";
}

void MainWindow::onActionDrawScannerSelectSource()
{
    qDebug() << "选择源";
}

void MainWindow::onActionDrawScannerVectorizeImage()
{
    qDebug() << "矢量图";
}

void MainWindow::onActionInsertOffset()
{
    qDebug() << "创建偏移";
}

void MainWindow::onActionInsertAdvancedOffset()
{
    qDebug() << "创建高级偏移";
}

void MainWindow::onActionInsertText()
{
    qDebug() << "插入文本";
}

void MainWindow::onActionInsertFillet()
{
    qDebug() << "倒圆角";
}

void MainWindow::onActionInsertNotch()
{
    qDebug() << "插入凹槽";
}

void MainWindow::onActionInsertNotchesGroup()
{
    qDebug() << "插入凹槽组";
}

void MainWindow::onActionInsertNotchesAxisTiedGroup()
{
    qDebug() << "凹槽绑定群";
}

void MainWindow::onActionInsertSizeNotch()
{
    qDebug() << "凹槽尺寸编号";
}

void MainWindow::onActionInsertRemark()
{
    qDebug() << "插入备注";
}

void MainWindow::onActionInsertSmooth()
{
    qDebug() << "角度平滑";
}

void MainWindow::onActionInsertMargin()
{
    qDebug() << "插入边距/余量";
}

void MainWindow::onActionInsertCanel()
{
    qDebug() << "插入管道";
}

void MainWindow::onActionInsertBlock()
{
    qDebug() << "插入块";
}

void MainWindow::onActionInsertHolesOverALine()
{
    qDebug() << "插入沿线孔";
}

void MainWindow::onActionInsertArrayRectangle()
{
    qDebug() << "创建矩形阵列";
}

void MainWindow::onActionInsertArrayCircular()
{
    qDebug() << "创建圆形阵列";
}

void MainWindow::onActionModifyUndo()
{
    qDebug() << "撤销上个操作";
}

void MainWindow::onActionModifyRedo()
{
    qDebug() << "重做撤销操作";
}


void MainWindow::onActionModifyClear()
{
    qDebug() << "清空";
}

void MainWindow::onActionModifyDelete()
{
    qDebug() << "删除";
}

void MainWindow::onActionModifyCut()
{
    qDebug() << "剪切";
}

void MainWindow::onActionModifyCopy()
{
    qDebug() << "复制";
}

void MainWindow::onActionModifyPaste()
{
    qDebug() << "粘贴";
}

void MainWindow::onActionModifyPasteAsSlave()
{
    qDebug() << "粘贴为副本";
}

void MainWindow::onActionModifyTransformMove()
{
    qDebug() << "移动";
    scene_active->setMoveable(true);
}

void MainWindow::onActionModifyTransformRotate()
{
    qDebug() << "旋转";
}

void MainWindow::onActionModifyTransformRotateKnotsContinuely()
{
    qDebug() << "连续旋转";
}

void MainWindow::onActionModifyTransformRotateKnots()
{
    qDebug() << "旋转节点";
}

void MainWindow::onActionModifyTransformScale()
{
    qDebug() << "比例";
}

void MainWindow::onActionModifyTransformMirror()
{
    qDebug() << "镜像";
}

void MainWindow::onActionModifyTransformStrech()
{
    qDebug() << "拉伸";
}


void MainWindow::onActionModifyTransformExplode()
{
    qDebug() << "爆炸";
}

void MainWindow::onActionModifyTransformJoin()
{
    qDebug() << "连接";
}

void MainWindow::onActionModifyTransformBreak()
{
    qDebug() << "断开";
}

void MainWindow::onActionModifyTransformfillet()
{
    qDebug() << "倒圆角";
}

void MainWindow::onActionModifyTransformVCut()
{
    qDebug() << "V切";
}

void MainWindow::onActionModifyTransformSpline()
{
    qDebug() << "创建样条";
}

void MainWindow::onActionModifyTrim()
{
    qDebug() << "修剪";
}

void MainWindow::onActionModifyExtend()
{
    qDebug() << "延伸";
}

void MainWindow::onActionModifyEndpoint()
{
    qDebug() << "修改端点";
}

void MainWindow::onActionModifyAdjustPolyLine()
{
    qDebug() << "调整折线";
}

void MainWindow::onActionModifyStartpoint()
{
    qDebug() << "起点";
}

void MainWindow::onActionModifySoftenup()
{
    qDebug() << "软化";
}

void MainWindow::onActionModifyWarp()
{
    qDebug() << "弯曲";
}

void MainWindow::onActionModifyKnotsReduction()
{
    qDebug() << "减少节点";
}

void MainWindow::onActionModifyReplacePart()
{
    qDebug() << "更换部分";
}

void MainWindow::onActionModifyChangeTo()
{
    qDebug() << "更改至";
}

void MainWindow::onActionModifyMakeMasterLine()
{
    qDebug() << "制作主线";
}

void MainWindow::onActionModifyStyle()
{
    qDebug() << "实体线样式";
}

void MainWindow::onActionModifySpringing()
{
    qDebug() << "弹回";
}

void MainWindow::onActionPatternSelect()
{
    qDebug() << "选择";
}

void MainWindow::onActionPatternNew()
{
    qDebug() << "新建";
}

void MainWindow::onActionPatternClone()
{
    qDebug() << "克隆";
}

void MainWindow::onActionPatternDelete()
{
    qDebug() << "删除";
}

void MainWindow::onActionPatternRotate()
{
    qDebug() << "旋转";
}

void MainWindow::onActionPatternMakeSymmetic()
{
    qDebug() << "使对称";
}

void MainWindow::onActionPatternPosition()
{
    qDebug() << "位置";
}

void MainWindow::onActionPatternExtractAuto()
{
    qDebug() << "自动提取";
}

void MainWindow::onActionPatternExtractManual()
{
    qDebug() << "手动提取";
}

void MainWindow::onActionPatternPerimeter()
{
    qDebug() << "周长";
}

void MainWindow::onActionPatternTrim()
{
    qDebug() << "关闭修剪";
}

void MainWindow::onActionPatternCheck()
{
    qDebug() << "检查";
}

void MainWindow::onActionPatternDevide()
{
    qDebug() << "瓜分";
}

void MainWindow::onActionPatternSaveAsWeft()
{
    qDebug() << "另存为纬线";
}

void MainWindow::onActionViewXYAxes(bool toggled)
{
    // 更新配置文件
    configCopy->axesGrid.axes.showAxes = toggled;
    emit configChanged("axesGrid/axesGrid_showAxes", QVariant(toggled));
    // 显示坐标
    for(int i=0; i<project_list.length();i++){
        for(int j=0; j<project_list.at(i)->getSceneList().length();j++){
            project_list.at(i)->getScene(j)->onAxesChanged(toggled);
        }
    }
}

void MainWindow::onActionViewGrid(bool toggled)
{
    configCopy->axesGrid.grid.showGrid = toggled;
    emit configChanged("axesGrid/axesGrid_showGrid", QVariant(toggled));
    // 显示网格
    for(int i=0; i<project_list.length();i++){
        for(int j=0; j<project_list.at(i)->getSceneList().length();j++){
            project_list.at(i)->getScene(j)->onGridChanged(toggled);
        }
    }
}

void MainWindow::onActionViewKnots(bool toggled)
{
    qDebug() << "knots axes about to " << toggled;
}

void MainWindow::onActionViewImage(bool toggled)
{
    qDebug() << "Image axes about to " << toggled;
}

void MainWindow::onActionViewDesignRules(bool toggled)
{
    qDebug() << "design rules axes about to " << toggled;
}

void MainWindow::onActionViewGradingRules(bool toggled)
{
    configCopy->view.gradingRules = toggled;
    emit configChanged("view/view_grading_rules", QVariant(toggled));
}

void MainWindow::onActionViewFilledPatterns(bool toggled)
{
    configCopy->view.filledPatterns = toggled;
    emit configChanged("view/view_filled_patterns", QVariant(toggled));
}

void MainWindow::onActionViewZoomWindow()
{
    qDebug() << "缩放窗口";
}

void MainWindow::onActionViewZoomAll()
{
    qDebug() << "全部缩放";
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

void MainWindow::onActionViewVoerlapPatterns()
{
    qDebug() << "覆盖图样";
}

void MainWindow::onActionViewHide(bool toggled)
{
    qDebug() << "hide" << toggled;
}

void MainWindow::onActionViewShow(bool toggled)
{
    qDebug() << "show" << toggled;
}

void MainWindow::onActionViewClean(bool toggled)
{
    qDebug() << "clean" << toggled;
}

void MainWindow::onActionViewSetReference()
{
    qDebug() << "设置参考";
}

void MainWindow::onActionViewLockLayout(bool toggled)
{
    qDebug() << "锁定布局" << toggled;
}

void MainWindow::onActionViewToolFindStyleToggled(bool toggled)
{
    onDockFindStyleVisibilityChanged(toggled);
    configCopy->view.toolFindStyle = toggled;
    emit configChanged("view/view_tool_find_style", QVariant(toggled));
}

void MainWindow::onActionViewToolProjectToggled(bool toggled)
{
    onDockProjectVisibilityChanged(toggled);
    configCopy->view.toolProject = toggled;
    emit configChanged("view/view_tool_project", QVariant(toggled));
}

void MainWindow::onActionViewToolPropertiesToggled(bool toggled)
{
    onDockPropertiesVisibilityChanged(toggled);
    configCopy->view.toolProperties = toggled;
    emit configChanged("view/view_tool_properties", QVariant(toggled));
}


void MainWindow::onActionViewToolSlideToggled(bool toggled)
{
    qDebug() << "slide about to " << toggled;
}

void MainWindow::onActionViewToolCustomizeToggled(bool toggled)
{
    qDebug() << "customize about to " << toggled;
}

void MainWindow::onActionViewStatusBar(bool toggled)
{
    qDebug() << "status_bar about to " << toggled;
}


void MainWindow::onActionUtilityMeasureDis()
{
    qDebug() << "测量距离" ;
}

void MainWindow::onActionUtilityMeasureLine()
{
    qDebug() << "测量线长" ;
}

void MainWindow::onActionCreatorPatternCard()
{
    qDebug() << "创建图样卡" ;
}

void MainWindow::onActionUtilityQuickNesting()
{
    qDebug() << "快速排版" ;
}

void MainWindow::onActionUtilityNestByJobCard()
{
    qDebug() << "转到排版" ;
}

void MainWindow::onActionUtilityHeelCover()
{
    qDebug() << "后跟" ;
}

void MainWindow::onActionUtilityPatternSearch()
{
    qDebug() << "搜索图样" ;
}

void MainWindow::onActionGradingParameters()
{
    qDebug() << "图样";
}

void MainWindow::onActionGradingGrade()
{
    qDebug() << "级放";
}

void MainWindow::onActionGradingDeletedGradedPatterns()
{
    qDebug() << "删除级放";
}

void MainWindow::onActionGradingReferenceInsert()
{
    qDebug() << "插入";
}

void MainWindow::onActionGradingKeepMargin()
{
    qDebug() << "保留边距";
}

void MainWindow::onActionGradingDeleteMargin()
{
    qDebug() << "删除边距";
}

void MainWindow::onActionGradingUpdate()
{
    qDebug() << "更新级放";
}

void MainWindow::onActionGradingXYLock()
{
    qDebug() << "XY锁定";
}

void MainWindow::onActionGradingDeleteXYLock()
{
    qDebug() << "解除XY锁定";
}

void MainWindow::onActionGradingHorizontalZone()
{
    qDebug() << "水平区";
}

void MainWindow::onActionGradingVerticalZone()
{
    qDebug() << "垂直区";
}

void MainWindow::onActionGradingDeleteZone()
{
    qDebug() << "删除区";
}


void MainWindow::onActionHelpHelp()
{
    qDebug() << "帮助";
}

void MainWindow::onActionHelpLicense()
{
    qDebug() << "许可";
}

void MainWindow::onActionHelpAbout()
{
    QMessageBox::about(this, tr("关于CADPRO"),
        tr("The <b>Style Sheet</b> example shows how widgets can be styled "
           "using <a href=\"http://doc.qt.io/qt-5/stylesheet.html\">Qt "
           "Style Sheets</a>. Click <b>File|Edit Style Sheet</b> to pop up the "
           "style editor, and either choose an existing style sheet or design "
           "your own."));
}

void MainWindow::onActionObjectSnapEndpoint()
{
    qDebug() << "实体端点";
}

void MainWindow::onActionObjectSnapCenter()
{
    qDebug() << "实体中心";
}

void MainWindow::onActionObjectSnapInterSection()
{
    qDebug() << "交叉点";
}

void MainWindow::onActionObjectSnapKnot()
{
    qDebug() << "节点";
}

void MainWindow::onActionObjectSnapOrtho()
{
    qDebug() << "直角正交";
}

void MainWindow::onActionObjectSnapEnterCoordinates()
{
    qDebug() << "输入坐标";
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
        scene_active = project_active->getScene(0);
    } else{
        QString project_name = parent->text(0);
        project_active = getProjectByName(project_name);
        scene_active = project_active->getSceneByName(item->text(0));
        project_active->setActiveScene(scene_active);
    }
    updateScene();
}

void MainWindow::onActionTreeProjectAddScene()
{
    // 根据项目名称获取项目指针
    QString project_name = tree_project_active_item->text(0);
    project_active = getProjectByName(project_name);
    scene_active = project_active->addScene();
    QString name_scene_new = project_active->getSceneName(scene_active);

    QTreeWidgetItem *item_scene = new QTreeWidgetItem(tree_project_active_item,QStringList(name_scene_new)); //子节点1
    tree_project_active_item->addChild(item_scene);
    item_scene->setCheckState(0, Qt::Checked);

    updateScene();
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
        project->changeName(text);
        updateScene();
    }
}

void MainWindow::onActionTreeProjectClose()
{
    qDebug() << tree_project_active_item->text(0) << "关闭项目";
}

void MainWindow::onActionTreeProjectSceneChangeTo()
{
    qDebug() << tree_project_scene_active_item->text(0) << "切换至";
}

void MainWindow::onActionTreeProjectSceneMoveUpOne()
{
    qDebug() << tree_project_scene_active_item->text(0) << "上移一层";
}

void MainWindow::onActionTreeProjectSceneMoveUpTop()
{
    qDebug() << tree_project_scene_active_item->text(0) << "上移至顶";
}

void MainWindow::onActionTreeProjectSceneMoveDownOne()
{
    qDebug() << tree_project_scene_active_item->text(0) << "下移一层";
}

void MainWindow::onActionTreeProjectSceneMoveDownBottom()
{
    qDebug() << tree_project_scene_active_item->text(0) << "下移至底";
}

void MainWindow::onActionTreeProjectSceneRename()
{
    // 根据图层名称获取图层指针
    Project *project = getProjectByName(tree_project_active_item->text(0));
    Scene *scene = project->getSceneByName(tree_project_scene_active_item->text(0));

    // 获取新的图层名称
    bool ok;
    QString text = QInputDialog::getText(this, tr("图层重命名"),
                                         tr("新图层名称:"), QLineEdit::Normal,
                                         scene->getName(), &ok);
    if (ok && !text.isEmpty()){
        tree_project_scene_active_item->setText(0, text);
        scene->setName(text);
        updateScene();
    }
}

void MainWindow::onActionTreeProjectSceneDelete()
{
    qDebug() << tree_project_scene_active_item->text(0) << "删除";
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

void MainWindow::updateViewMenu()
{
    bool tool_slide_enable = action_view_tool_slide->isEnabled();
    if(tool_slide_enable){
        action_view_tool_find_style->setEnabled(tool_slide_enable);
        action_view_tool_project->setEnabled(tool_slide_enable);
        action_view_tool_properties->setEnabled(tool_slide_enable);
    }
}

void MainWindow::onProjectNameChanged(QString lastName, QString presentName)
{
    for(int i = 0;i<tree_project_item_list.length();i++){
        if(tree_project_item_list.at(i)->text(0) == lastName){
            tree_project_item_list.at(i)->setText(0, presentName);
            break;
        }
    }
}

void MainWindow::onMousePositionChanged(QPointF pos)
{
    mousePositionLabel->setText(tr("X=") + QString::number(pos.rx()) + " Y=" + QString::number(-pos.ry()));
}

void MainWindow::onSceneItemsChanged()
{
    bool flag = true;
    if(scene_active->getitemListLength() > 0){
        flag = false;
    }
    action_modify_redo->setDisabled(flag);
    action_modify_undo->setDisabled(flag);
    action_modify_redo->setDisabled(flag);
    action_modify_clear->setDisabled(flag);
    action_modify_delete->setDisabled(flag);
    action_modify_cut->setDisabled(flag);
    action_modify_copy->setDisabled(flag);
    action_modify_paste->setDisabled(flag);
    action_modify_paste_as_slave->setDisabled(flag);

    action_modify_transform_move->setDisabled(flag);
    action_modify_transform_rotate->setDisabled(flag);
    action_modify_transform_rotate_knots_continuely->setDisabled(flag);
    action_modify_transform_rotate_knots->setDisabled(flag);
    action_modify_transform_scale->setDisabled(flag);
    action_modify_transform_mirror->setDisabled(flag);
    action_modify_transform_strech->setDisabled(flag);
    action_modify_transform_explode->setDisabled(flag);
    action_modify_transform_join->setDisabled(flag);
    action_modify_transform_break->setDisabled(flag);
    action_modify_transform_fillet->setDisabled(flag);
    action_modify_transform_V_cut->setDisabled(flag);
    action_modify_transform_spline->setDisabled(flag);
    action_modify_trim->setDisabled(flag);
    action_modify_extend->setDisabled(flag);
    action_modify_modify_endpoint->setDisabled(flag);
    action_modify_adjust_polyline->setDisabled(flag);
    action_modify_start_point->setDisabled(flag);
    action_modify_soften_up->setDisabled(flag);
    action_modify_warp->setDisabled(flag);
    action_modify_knots_reduction->setDisabled(flag);
    action_modify_replace_part->setDisabled(flag);
    action_modify_change_to->setDisabled(flag);
    action_modify_make_master_line->setDisabled(flag);
    action_modify_style->setDisabled(flag);
}



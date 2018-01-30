#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("CADPRO");
    setWindowState(Qt::WindowMaximized);
// ![1] 文件栏
    QMenu *menu_File = ui->menuBar->addMenu(tr("文件(&F)"));
    QAction *action_file_new = new QAction(tr("&新建"), this);
    action_file_new->setShortcut(QKeySequence::New);
    action_file_new->setStatusTip(tr("新建一个项目"));
    menu_File->addAction(action_file_new);

    QAction *action_file_open = new QAction(tr("&打开"), this);
    action_file_open->setShortcut(QKeySequence::Open);
    action_file_open->setStatusTip(tr("打开一个已存的项目"));
    menu_File->addAction(action_file_open);

    QAction *action_file_save = new QAction(tr("&保存"), this);
    action_file_save->setShortcut(QKeySequence::Save);
    action_file_save->setStatusTip(tr("保存当前项目"));
    menu_File->addAction(action_file_save);

    QAction *action_file_save_as = new QAction(tr("&另存为"), this);
    action_file_save_as->setShortcut(QKeySequence::SaveAs);
    action_file_save_as->setStatusTip(tr("另存当前项目"));
    menu_File->addAction(action_file_save_as);

    QAction *action_file_save_all = new QAction(tr("&保存全部"), this);
    action_file_save_all->setStatusTip(tr("保存所有打开的项目"));
    menu_File->addAction(action_file_save_all);
    menu_File->addSeparator();

    QAction *action_file_print = new QAction(tr("&打印"), this);
    action_file_print->setShortcut(QKeySequence::Print);
    action_file_print->setStatusTip(tr("打印当前视图"));
    menu_File->addAction(action_file_print);

    QAction *action_file_print_preview = new QAction(tr("&打印预览"), this);
    action_file_print_preview->setStatusTip(tr("显示当前项目打印预览"));
    action_file_print_preview->setDisabled(true);
    menu_File->addAction(action_file_print_preview);

    QAction *action_file_print_setup = new QAction(tr("&打印设置"), this);
    action_file_print_setup->setStatusTip(tr("更改打印机和打印选项"));
    action_file_print_setup->setDisabled(true);
    menu_File->addAction(action_file_print_setup);
    menu_File->addSeparator();

    QMenu *menu_file_import = menu_File->addMenu(tr("导入"));
    QAction *action_file_import_DXF = new QAction(tr("&DXF"), this);
    action_file_import_DXF->setStatusTip(tr("导入DXF文件"));
    menu_file_import->addAction(action_file_import_DXF);

    QAction *action_file_import_DVS = new QAction(tr("&DVS"), this);
    action_file_import_DVS->setStatusTip(tr("导入DVS文件"));
    menu_file_import->addAction(action_file_import_DVS);

    QAction *action_file_import_TEF = new QAction(tr("&TEF"), this);
    action_file_import_TEF->setStatusTip(tr("导入TEF文件"));
    menu_file_import->addAction(action_file_import_TEF);

    QMenu *menu_file_export = menu_File->addMenu(tr("导出"));
    QAction *action_file_export_DXF = new QAction(tr("&DXF"), this);
    action_file_export_DXF->setStatusTip(tr("导出DXF文件"));
    menu_file_export->addAction(action_file_export_DXF);

    QAction *action_file_export_DVS = new QAction(tr("&DVS"), this);
    action_file_export_DVS->setStatusTip(tr("导出DVS文件"));
    menu_file_export->addAction(action_file_export_DVS);

    QAction *action_file_export_TEF = new QAction(tr("&TEF"), this);
    action_file_export_TEF->setStatusTip(tr("导出TEF文件"));
    menu_file_export->addAction(action_file_export_TEF);
    menu_File->addSeparator();

    QAction *action_file_configuration = new QAction(tr("&配置"), this);
    action_file_configuration->setStatusTip(tr("修改cadPro配置"));
    menu_File->addAction(action_file_configuration);

    QAction *action_file_exit = new QAction(tr("&退出"), this);
    action_file_exit->setShortcut(QKeySequence::Quit);
    action_file_exit->setStatusTip(tr("退出应用程序；提示保存项目"));
    menu_File->addAction(action_file_exit);
// ![1] 文件栏

// ![2] 绘图栏
    QMenu *menu_Draw = ui->menuBar->addMenu(tr("绘图(&D)"));
    QAction *action_draw_line = new QAction(tr("&线条"), this);
    action_draw_line->setStatusTip(tr("绘制直线"));
    menu_Draw->addAction(action_draw_line);

    QAction *action_draw_middle_axis = new QAction(tr("&中轴线"), this);
    action_draw_middle_axis->setStatusTip(tr("绘制中轴线"));
    menu_Draw->addAction(action_draw_middle_axis);

    QAction *action_draw_polyline = new QAction(tr("&曲折线"), this);
    action_draw_polyline->setStatusTip(tr("绘制曲折线"));
    menu_Draw->addAction(action_draw_polyline);

    QAction *action_draw_circle = new QAction(tr("&圆"), this);
    action_draw_circle->setStatusTip(tr("按中心和半径绘制圆"));
    menu_Draw->addAction(action_draw_circle);

    QAction *action_draw_ellipse = new QAction(tr("&椭圆"), this);
    action_draw_ellipse->setStatusTip(tr("按中心和半径绘制椭圆"));
    menu_Draw->addAction(action_draw_ellipse);

    QAction *action_draw_arc_by_3_points = new QAction(tr("&3点圆弧"), this);
    action_draw_arc_by_3_points->setStatusTip(tr("绘制3点椭圆(起点，终点，中心点)"));
    menu_Draw->addAction(action_draw_arc_by_3_points);

    QAction *action_draw_arc_by_3_points_2 = new QAction(tr("&3点圆弧2"), this);
    action_draw_arc_by_3_points_2->setStatusTip(tr("绘制3点椭圆(起点，终点，中心点)"));
    menu_Draw->addAction(action_draw_arc_by_3_points_2);

    QAction *action_draw_rectangle = new QAction(tr("&矩形"), this);
    action_draw_rectangle->setStatusTip(tr("绘制矩形"));
    menu_Draw->addAction(action_draw_rectangle);

    QAction *action_draw_trapezium = new QAction(tr("&梯形"), this);
    action_draw_trapezium->setStatusTip(tr("绘制矩形"));
    menu_Draw->addAction(action_draw_trapezium);

    QAction *action_draw_polygon = new QAction(tr("&正多边形"), this);
    action_draw_polygon->setStatusTip(tr("绘制正多边形"));
    menu_Draw->addAction(action_draw_polygon);

    QAction *action_draw_stab_hole = new QAction(tr("&冲孔"), this);
    action_draw_stab_hole->setStatusTip(tr("绘制冲孔"));
    menu_Draw->addAction(action_draw_stab_hole);

    QAction *action_draw_reference = new QAction(tr("&参考点"), this);
    action_draw_reference->setStatusTip(tr("绘制参考点"));
    menu_Draw->addAction(action_draw_reference);

    QAction *action_draw_eyelet = new QAction(tr("&鸡眼孔"), this);
    action_draw_eyelet->setStatusTip(tr("在当前图案上插入鸡眼孔"));
    menu_Draw->addAction(action_draw_eyelet);

    QAction *action_draw_pattern_direction = new QAction(tr("&图案方向"), this);
    action_draw_pattern_direction->setStatusTip(tr("插入方向轴"));
    menu_Draw->addAction(action_draw_pattern_direction);

    QAction *action_draw_shank_line = new QAction(tr("&上插线"), this);
    action_draw_shank_line->setStatusTip(tr("创建上插线"));
    menu_Draw->addAction(action_draw_shank_line);
    menu_Draw->addSeparator();

    QAction *action_draw_perpendicular = new QAction(tr("&垂直线"), this);
    action_draw_perpendicular->setStatusTip(tr("绘制垂直线"));
    action_draw_perpendicular->setDisabled(true);
    menu_Draw->addAction(action_draw_perpendicular);
    menu_Draw->addSeparator();

    QMenu *menu_image = menu_Draw->addMenu(tr("图像"));
    QAction *action_draw_image = new QAction(tr("&获得"), this);
    action_draw_image->setStatusTip(tr("从扫描仪获取图像"));
    menu_image->addAction(action_draw_image);

    QAction *action_draw_image_insert = new QAction(tr("&插入"), this);
    action_draw_image_insert->setStatusTip(tr("在图样中插入图片"));
    menu_image->addAction(action_draw_image_insert);

    QAction *action_draw_image_delete = new QAction(tr("&删除"), this);
    action_draw_image_delete->setStatusTip(tr("从图样中删除图片"));
    action_draw_image_delete->setDisabled(true);
    menu_image->addAction(action_draw_image_delete);

    QAction *action_draw_image_crop = new QAction(tr("&裁剪"), this);
    action_draw_image_crop->setStatusTip(tr("裁剪图样图片"));
    action_draw_image_crop->setDisabled(true);
    menu_image->addAction(action_draw_image_crop);

    QAction *action_draw_image_calibrate = new QAction(tr("&校准/划刻度"), this);
    action_draw_image_calibrate->setStatusTip(tr("校准图像大小"));
    action_draw_image_calibrate->setDisabled(true);
    menu_image->addAction(action_draw_image_calibrate);

    QAction *action_adjust_image_image_agjustement = new QAction(tr("&图像调整"), this);
    action_adjust_image_image_agjustement->setStatusTip(tr("校准图像大小"));
    action_adjust_image_image_agjustement->setDisabled(true);
    menu_image->addAction(action_draw_image_calibrate);

    QMenu *menu_digitizer = menu_Draw->addMenu(tr("数字化仪"));
    QAction *action_draw_digitizer_activate = new QAction(tr("&启用"), this);
    action_draw_digitizer_activate->setStatusTip(tr("启用/禁用数字化仪"));
    action_draw_digitizer_activate->setDisabled(true);
    menu_digitizer->addAction(action_draw_digitizer_activate);

    QAction *action_draw_digitizer_set_position = new QAction(tr("&设置shell位置"), this);
    action_draw_digitizer_set_position->setStatusTip(tr("设置shell位置"));
    action_draw_digitizer_set_position->setDisabled(true);
    menu_digitizer->addAction(action_draw_digitizer_set_position);

    QAction *action_draw_digitizer_configure = new QAction(tr("&配置"), this);
    action_draw_digitizer_configure->setStatusTip(tr("配置数字化仪大小"));
    action_draw_digitizer_configure->setDisabled(true);
    menu_digitizer->addAction(action_draw_digitizer_configure);

    QMenu *menu_scanner = menu_Draw->addMenu(tr("扫描仪"));
    QAction *action_draw_scanner_vectorize__from_scanner = new QAction(tr("&扫描矢量"), this);
    action_draw_scanner_vectorize__from_scanner->setStatusTip(tr("扫描和矢量化一个图样"));
    menu_scanner->addAction(action_draw_scanner_vectorize__from_scanner);

    QAction *action_draw_scanner_select_source = new QAction(tr("&选择源"), this);
    action_draw_scanner_select_source->setStatusTip(tr("选择扫描源"));
    menu_scanner->addAction(action_draw_scanner_select_source);

    QAction *action_draw_scanner_vectorize_image = new QAction(tr("&矢量图像"), this);
    action_draw_scanner_vectorize_image->setStatusTip(tr("矢量化扫描图像"));
    menu_scanner->addAction(action_draw_scanner_vectorize_image);
// ![2] 绘图栏

// ![3] 插入栏
    QMenu *menu_Insert = ui->menuBar->addMenu(tr("插入(&I)"));
    QAction *action_insert_offset = new QAction(tr("&偏移"), this);
    action_insert_offset->setStatusTip(tr("创建偏移"));
    action_insert_offset->setDisabled(true);
    menu_Insert->addAction(action_insert_offset);

    QAction *action_insert_advanced_offset = new QAction(tr("&高级偏移"), this);
    action_insert_advanced_offset->setStatusTip(tr("创建高级偏移"));
    action_insert_advanced_offset->setDisabled(true);
    menu_Insert->addAction(action_insert_advanced_offset);

    QAction *action_insert_text = new QAction(tr("&文本"), this);
    action_insert_text->setStatusTip(tr("插入文本"));
    menu_Insert->addAction(action_insert_text);

    QAction *action_insert_fillet = new QAction(tr("&倒圆角"), this);
    action_insert_fillet->setStatusTip(tr("倒圆角两条线"));
    action_insert_fillet->setDisabled(true);
    menu_Insert->addAction(action_insert_fillet);

    QAction *action_insert_notch = new QAction(tr("&凹槽"), this);
    action_insert_notch->setStatusTip(tr("插入凹槽"));
    action_insert_notch->setDisabled(true);
    menu_Insert->addAction(action_insert_notch);

    QAction *action_insert_notches_group = new QAction(tr("&凹槽组"), this);
    action_insert_notches_group->setStatusTip(tr("插入凹槽"));
    action_insert_notches_group->setDisabled(true);
    menu_Insert->addAction(action_insert_notches_group);

    QAction *action_insert_notches_axis_tied_group = new QAction(tr("&凹槽轴绑定群组"), this);
    action_insert_notches_axis_tied_group->setStatusTip(tr("插入一个与轴绑定的凹槽群组"));
    action_insert_notches_axis_tied_group->setDisabled(true);
    menu_Insert->addAction(action_insert_notches_axis_tied_group);

    QAction *action_insert_size_notch = new QAction(tr("&凹槽尺寸编号"), this);
    action_insert_size_notch->setStatusTip(tr("插入凹槽尺寸编号"));
    action_insert_size_notch->setDisabled(true);
    menu_Insert->addAction(action_insert_size_notch);

    QAction *action_insert_remark = new QAction(tr("&备注"), this);
    action_insert_remark->setStatusTip(tr("插入备注"));
    action_insert_remark->setDisabled(true);
    menu_Insert->addAction(action_insert_remark);

    QAction *action_insert_smooth = new QAction(tr("&平滑"), this);
    action_insert_smooth->setStatusTip(tr("角度平滑"));
    action_insert_smooth->setDisabled(true);
    menu_Insert->addAction(action_insert_smooth);

    menu_Insert->addSeparator();

    QAction *action_insert_margin = new QAction(tr("&边距/余量"), this);
    action_insert_margin->setStatusTip(tr("插入边距或余量"));
    action_insert_margin->setDisabled(true);
    menu_Insert->addAction(action_insert_margin);

    menu_Insert->addSeparator();

    QAction *action_insert_block = new QAction(tr("&块"), this);
    action_insert_block->setStatusTip(tr("在当前图样插入块"));
    action_insert_block->setDisabled(true);
    menu_Insert->addAction(action_insert_block);

    menu_Insert->addSeparator();

    QAction *action_insert_holes_over_a_line = new QAction(tr("&沿线孔"), this);
    action_insert_holes_over_a_line->setStatusTip(tr("插入沿线孔"));
    action_insert_holes_over_a_line->setDisabled(true);
    menu_Insert->addAction(action_insert_holes_over_a_line);

    menu_Insert->addSeparator();

    QMenu *menu_insert_array = menu_Insert->addMenu("阵列");

    QAction *action_insert_array_rectangular = new QAction(tr("&矩形"), this);
    action_insert_array_rectangular->setStatusTip(tr("创建矩形阵列"));
    action_insert_array_rectangular->setDisabled(true);
    menu_insert_array->addAction(action_insert_array_rectangular);

    QAction *action_insert_array_circular = new QAction(tr("&圆形"), this);
    action_insert_array_circular->setStatusTip(tr("创建圆形阵列"));
    action_insert_array_circular->setDisabled(true);
    menu_insert_array->addAction(action_insert_array_circular);
// ![3] 插入栏

// ![4] 修改栏
    QMenu *menu_Modify = ui->menuBar->addMenu(tr("修改(&M)"));

    QAction *action_modify_undo = new QAction(tr("&撤销"), this);
    action_modify_undo->setShortcut(QKeySequence::Undo);
    action_modify_undo->setStatusTip(tr("撤销上一个操作"));
    action_modify_undo->setDisabled(true);
    menu_Modify->addAction(action_modify_undo);

    QAction *action_modify_redo = new QAction(tr("&重做"), this);
    action_modify_redo->setShortcut(QKeySequence::Redo);
    action_modify_redo->setDisabled(true);
    action_modify_redo->setStatusTip(tr("重做撤销的操作"));
    menu_Modify->addAction(action_modify_redo);

    QAction *action_modify_clear = new QAction(tr("&清空"), this);
    action_modify_clear->setStatusTip(tr("清空画板"));
    action_modify_clear->setDisabled(true);
    menu_Modify->addAction(action_modify_clear);
    menu_Modify->addSeparator();

    QAction *action_modify_delete = new QAction(tr("&删除"), this);
    action_modify_delete->setShortcut(QKeySequence::Delete);
    action_modify_delete->setStatusTip(tr("删除实体"));
    action_modify_delete->setDisabled(true);
    menu_Modify->addAction(action_modify_delete);

    QAction *action_modify_copy = new QAction(tr("&复制"), this);
    action_modify_copy->setShortcut(QKeySequence::Copy);
    action_modify_copy->setStatusTip(tr("将所选线复制到粘贴板"));
    action_modify_copy->setDisabled(true);
    menu_Modify->addAction(action_modify_copy);

    QAction *action_modify_paste = new QAction(tr("&粘贴"), this);
    action_modify_paste->setShortcut(QKeySequence::Paste);
    action_modify_paste->setStatusTip(tr("粘贴先前复制的实体"));
    action_modify_paste->setDisabled(true);
    menu_Modify->addAction(action_modify_paste);

    QAction *action_modify_paste_as_slave = new QAction(tr("&粘贴为副本"), this);
    action_modify_paste_as_slave->setStatusTip(tr("粘贴为副本"));
    action_modify_paste_as_slave->setDisabled(true);
    menu_Modify->addAction(action_modify_paste_as_slave);
    menu_Modify->addSeparator();

    QMenu *menu_modify_transform = menu_Modify->addMenu(tr("转换"));
    QAction *action_modify_transform_move = new QAction(tr("&移动"), this);
    action_modify_transform_move->setDisabled(true);
    menu_modify_transform->addAction(action_modify_transform_move);

    QAction *action_modify_transform_rotate = new QAction(tr("&旋转"), this);
    action_modify_transform_rotate->setStatusTip(tr("旋转实体"));
    action_modify_transform_rotate->setDisabled(true);
    menu_modify_transform->addAction(action_modify_transform_rotate);

    QAction *action_modify_transform_rotate_knots_continuely = new QAction(tr("&连续旋转节点"), this);
    action_modify_transform_rotate_knots_continuely->setDisabled(true);
    menu_modify_transform->addAction(action_modify_transform_rotate_knots_continuely);

    QAction *action_modify_transform_rotate_knots = new QAction(tr("&旋转节点"), this);
    action_modify_transform_rotate_knots->setDisabled(true);
    menu_modify_transform->addAction(action_modify_transform_rotate_knots);

    QAction *action_modify_transform_scale = new QAction(tr("&比例"), this);
    action_modify_transform_scale->setDisabled(true);
    menu_modify_transform->addAction(action_modify_transform_scale);

    QAction *action_modify_transform_mirror = new QAction(tr("&镜像"), this);
    action_modify_transform_mirror->setDisabled(true);
    menu_modify_transform->addAction(action_modify_transform_mirror);

    QAction *action_modify_transform_strech = new QAction(tr("&拉伸"), this);
    action_modify_transform_strech->setDisabled(true);
    menu_modify_transform->addAction(action_modify_transform_strech);

    QAction *action_modify_transform_explode = new QAction(tr("&爆炸"), this);
    action_modify_transform_explode->setDisabled(true);
    menu_modify_transform->addAction(action_modify_transform_explode);

    QAction *action_modify_transform_join = new QAction(tr("&联接"), this);
    action_modify_transform_join->setDisabled(true);
    menu_modify_transform->addAction(action_modify_transform_join);

    QAction *action_modify_transform_break = new QAction(tr("&断开"), this);
    action_modify_transform_break->setDisabled(true);
    menu_modify_transform->addAction(action_modify_transform_break);

    QAction *action_modify_transform_fillet = new QAction(tr("&倒圆角"), this);
    action_modify_transform_fillet->setDisabled(true);
    menu_modify_transform->addAction(action_modify_transform_fillet);

    QAction *action_modify_transform_V_cut = new QAction(tr("&'V'切"), this);
    menu_modify_transform->addAction(action_modify_transform_V_cut);
    action_modify_transform_V_cut->setDisabled(true);
    menu_modify_transform->addSeparator();

    QAction *action_modify_transform_spline = new QAction(tr("&创建样条"), this);
    action_modify_transform_spline->setDisabled(true);
    menu_modify_transform->addAction(action_modify_transform_spline);

//    QAction *action_shear = new QAction(tr("&拉伸"), this);
//    action_shear->setStatusTip(tr("拉伸画板"));
//    menu_modify_transform->addAction(action_shear);
//    menu_Modify->addMenu(menu_modify_transform);

    QAction *action_modify_trim = new QAction(tr("&修剪"), this);
    action_modify_trim->setDisabled(true);
    menu_Modify->addAction(action_modify_trim);

    QAction *action_modify_extend = new QAction(tr("&延伸"), this);
    action_modify_extend->setDisabled(true);
    menu_Modify->addAction(action_modify_extend);

    QAction *action_modify_modify_endpoint = new QAction(tr("&修改端点"), this);
    action_modify_modify_endpoint->setDisabled(true);
    menu_Modify->addAction(action_modify_modify_endpoint);

    QAction *action_modify_adjust_polyline = new QAction(tr("&调整折线"), this);
    action_modify_adjust_polyline->setDisabled(true);
    menu_Modify->addAction(action_modify_adjust_polyline);

    QAction *action_modify_start_point = new QAction(tr("&起点"), this);
    action_modify_start_point->setDisabled(true);
    menu_Modify->addAction(action_modify_start_point);

    QAction *action_modify_soften_up = new QAction(tr("&软化"), this);
    action_modify_soften_up->setDisabled(true);
    menu_Modify->addAction(action_modify_soften_up);

    QAction *action_modify_warp = new QAction(tr("&弯曲"), this);
    action_modify_warp->setDisabled(true);
    menu_Modify->addAction(action_modify_warp);

    QAction *action_modify_knots_reduction = new QAction(tr("&减少节点"), this);
    action_modify_knots_reduction->setDisabled(true);
    menu_Modify->addAction(action_modify_knots_reduction);

    QAction *action_modify_replace_part = new QAction(tr("&更换部分"), this);
    action_modify_replace_part->setDisabled(true);
    menu_Modify->addAction(action_modify_replace_part);

    menu_Modify->addSeparator();

    QAction *action_modify_change_to = new QAction(tr("&更改至"), this);
    action_modify_change_to->setDisabled(true);
    menu_Modify->addAction(action_modify_change_to);

    QAction *action_modify_make_master_line = new QAction(tr("&制作主线"), this);
    menu_Modify->addAction(action_modify_make_master_line);

    QAction *action_modify_style = new QAction(tr("&实体线样式"), this);
    action_modify_style->setDisabled(true);
    menu_Modify->addAction(action_modify_style);

    menu_Modify->addSeparator();

    QAction *action_modify_springing = new QAction(tr("&Springing弹回"), this);
    menu_Modify->addAction(action_modify_springing);

// ![4] 修改栏

// ![5] 图样栏
    QMenu *menu_Pattern = ui->menuBar->addMenu(tr("图样(&P)"));
    QAction *action_pattern_select = new QAction(tr("&选择"), this);
    action_pattern_select->setDisabled(true);
    menu_Pattern->addAction(action_pattern_select);

    QAction *action_pattern_new = new QAction(tr("&新建"), this);
    menu_Pattern->addAction(action_pattern_new);

    QAction *action_pattern_copy = new QAction(tr("&复制"), this);
    menu_Pattern->addAction(action_pattern_copy);

    QAction *action_pattern_delete = new QAction(tr("&删除"), this);
    action_pattern_delete->setDisabled(true);
    menu_Pattern->addAction(action_pattern_delete);

    QAction *action_pattern_rotate = new QAction(tr("&旋转"), this);
    menu_Pattern->addAction(action_pattern_rotate);

    QAction *action_pattern_make_symmetic = new QAction(tr("&使对称"), this);
    action_pattern_make_symmetic->setDisabled(true);
    menu_Pattern->addAction(action_pattern_make_symmetic);

    menu_Pattern->addSeparator();

    QAction *action_pattern_position = new QAction(tr("&位置"), this);
    action_pattern_position->setDisabled(true);
    menu_Pattern->addAction(action_pattern_position);

    menu_Pattern->addSeparator();

    QAction *action_pattern_extract_auto = new QAction(tr("&提取(自动)"), this);
    action_pattern_extract_auto->setDisabled(true);
    menu_Pattern->addAction(action_pattern_extract_auto);

    QAction *action_pattern_extract_manual = new QAction(tr("&提取(手动)"), this);
    action_pattern_extract_manual->setDisabled(true);
    menu_Pattern->addAction(action_pattern_extract_manual);

    QAction *action_pattern_find_perimeter = new QAction(tr("&查找周长"), this);
    action_pattern_find_perimeter->setDisabled(true);
    menu_Pattern->addAction(action_pattern_find_perimeter);

    QAction *action_pattern_close_trim = new QAction(tr("&关闭和修剪"), this);
    action_pattern_close_trim->setDisabled(true);
    menu_Pattern->addAction(action_pattern_close_trim);

    menu_Pattern->addSeparator();

    QAction *action_pattern_check = new QAction(tr("&检查"), this);
    action_pattern_check->setDisabled(true);
    menu_Pattern->addAction(action_pattern_check);

    QAction *action_pattern_devide = new QAction(tr("&分裂/瓜分"), this);
    action_pattern_devide->setDisabled(true);
    menu_Pattern->addAction(action_pattern_devide);

    menu_Pattern->addSeparator();

    QAction *action_pattern_save_as_weft = new QAction(tr("&另存为纬线"), this);
    action_pattern_save_as_weft->setDisabled(true);
    menu_Pattern->addAction(action_pattern_save_as_weft);
// ![5] 图样栏

// ![6] 查看栏
    QMenu *menu_View = ui->menuBar->addMenu(tr("查看(&V)"));
    QAction *action_view_xy_axes = new QAction(tr("&XY轴"), this);
    menu_View->addAction(action_view_xy_axes);

    QAction *action_view_grid = new QAction(tr("&网格"), this);
    menu_View->addAction(action_view_grid);

    QAction *action_view_knots = new QAction(tr("&节点"), this);
    action_view_knots->setDisabled(true);
    menu_View->addAction(action_view_knots);

    QAction *action_view_image = new QAction(tr("&图片"), this);
    action_view_image->setDisabled(true);
    menu_View->addAction(action_view_image);

    QAction *action_view_design_rules = new QAction(tr("&设计尺"), this);
    action_view_design_rules->setDisabled(true);
    menu_View->addAction(action_view_design_rules);

    QAction *action_view_grading_rules = new QAction(tr("&级放规则"), this);
    menu_View->addAction(action_view_grading_rules);

    QAction *action_view_filled_patterns = new QAction(tr("&填充图样"), this);
    menu_View->addAction(action_view_filled_patterns);

    menu_View->addSeparator();

    QAction *action_view_zoom_window = new QAction(tr("&缩放窗口"), this);
    menu_View->addAction(action_view_zoom_window);

    QAction *action_view_zoom_all = new QAction(tr("&全部缩放"), this);
    menu_View->addAction(action_view_zoom_all);

    QAction *action_view_zoom_in = new QAction(tr("&放大"), this);
    action_view_zoom_in->setShortcut(QKeySequence::ZoomIn);
    action_view_zoom_in->setStatusTip(tr("放大"));
    menu_View->addAction(action_view_zoom_in);

    QAction *action_view_zoom_out = new QAction(tr("&缩小"), this);
    action_view_zoom_out->setShortcut(QKeySequence::ZoomOut);
    action_view_zoom_out->setStatusTip(tr("缩小"));
    menu_View->addAction(action_view_zoom_out);

    QAction *action_view_zoom_back = new QAction(tr("&还原"), this);
    action_view_zoom_back->setStatusTip(tr("还原"));
    menu_View->addAction(action_view_zoom_back);
    menu_View->addSeparator();

    QAction *action_view_voerlap_patterns = new QAction(tr("&覆盖图样"), this);
    menu_View->addAction(action_view_voerlap_patterns);
    menu_View->addSeparator();

    QAction *action_view_hide = new QAction(tr("&隐藏"), this);
    menu_View->addAction(action_view_hide);

    QAction *action_view_show = new QAction(tr("&显示"), this);
    action_view_show->setDisabled(true);
    menu_View->addAction(action_view_show);

    QAction *action_view_clean = new QAction(tr("&清除"), this);
    action_view_clean->setDisabled(true);
    menu_View->addAction(action_view_clean);

    QAction *action_view_set_reference = new QAction(tr("&设置引用参考"), this);
    action_view_set_reference->setDisabled(true);
    menu_View->addAction(action_view_set_reference);
    menu_View->addSeparator();

    QAction *action_view_lock_layout = new QAction(tr("&锁定布局"), this);
    action_view_lock_layout->setCheckable(true);
    action_view_lock_layout->setChecked(true);
    menu_View->addAction(action_view_lock_layout);

    QAction *action_view_tool_bar = new QAction(tr("&工具栏"), this);
    menu_View->addAction(action_view_tool_bar);

    QAction *action_view_status_bar = new QAction(tr("&状态栏"), this);
    action_view_status_bar->setCheckable(true);
    action_view_status_bar->setChecked(true);
    menu_View->addAction(action_view_status_bar);

    menu_View->addSeparator();

// ![6] 查看栏

// ![7] 效用栏
    QMenu *menu_Utility = ui->menuBar->addMenu(tr("效用(&U)"));
    QAction *action_utility_measure_distance = new QAction(tr("&测量距离"), this);
    menu_Utility->addAction(action_utility_measure_distance);

    QAction *action_utility_measure_line = new QAction(tr("&测量线长"), this);
    menu_Utility->addAction(action_utility_measure_line);

    menu_Utility->addSeparator();

    QAction *action_create_pattern_card = new QAction(tr("&创建图样卡"), this);
    action_create_pattern_card->setDisabled(true);
    menu_Utility->addAction(action_create_pattern_card);

    QAction *action_utility_quick_nesting = new QAction(tr("&快速排版"), this);
    menu_Utility->addAction(action_utility_quick_nesting);

    QAction *action_utility_nest_by_job_card = new QAction(tr("&转到排版"), this);
    menu_Utility->addAction(action_utility_nest_by_job_card);

    menu_Utility->addSeparator();

    QAction *action_utility_heel_cover = new QAction(tr("&后跟..."), this);
    menu_Utility->addAction(action_utility_heel_cover);

    menu_Utility->addSeparator();

    QAction *action_utility_pattern_search = new QAction(tr("&搜索图样"), this);
    action_utility_pattern_search->setDisabled(true);
    menu_Utility->addAction(action_utility_pattern_search);
// ![7] 效用栏

// ![8] 级放
    QMenu *menu_Grading = ui->menuBar->addMenu(tr("级放"));
    QAction *action_grading_parameters = new QAction(tr("&图样"), this);
    menu_Grading->addAction(action_grading_parameters);

    QAction *action_grading_grade = new QAction(tr("&级放"), this);
    action_grading_grade->setDisabled(true);
    menu_Grading->addAction(action_grading_grade);

    QAction *action_grading_delete_graded_patterns = new QAction(tr("&删除级放图样"), this);
    action_grading_delete_graded_patterns->setDisabled(true);
    menu_Grading->addAction(action_grading_delete_graded_patterns);

    QMenu *menu_Grading_reference = menu_Grading->addMenu(tr("级放参考"));
    QAction *action_grading_reference_insert = new QAction(tr("插入"), this);
    action_grading_reference_insert->setDisabled(true);
    menu_Grading_reference->addAction(action_grading_reference_insert);

    menu_Grading->addSeparator();

    QAction *action_grading_keep_margin = new QAction(tr("&保留 边距/余量"), this);
    action_grading_keep_margin->setDisabled(true);
    menu_Grading->addAction(action_grading_keep_margin);

    QAction *action_grading_delete_margin = new QAction(tr("&删除 边距/余量"), this);
    action_grading_delete_margin->setDisabled(true);
    menu_Grading->addAction(action_grading_delete_margin);

    menu_Grading->addSeparator();

    QAction *action_grading_update_grading = new QAction(tr("&更新级放"), this);
    action_grading_update_grading->setDisabled(true);
    menu_Grading->addAction(action_grading_update_grading);

    menu_Grading->addSeparator();

    QAction *action_grading_XY_lock = new QAction(tr("&XY 锁定"), this);
    action_grading_XY_lock->setDisabled(true);
    menu_Grading->addAction(action_grading_XY_lock);

    QAction *action_grading_delete_XY_lock = new QAction(tr("&删除XY 锁定 "), this);
    action_grading_delete_XY_lock->setDisabled(true);
    menu_Grading->addAction(action_grading_delete_XY_lock);

    menu_Grading->addSeparator();

    QAction *action_grading_horizontal = new QAction(tr("&水平区"), this);
    menu_Grading->addAction(action_grading_horizontal);

    QAction *action_grading_vertical = new QAction(tr("&垂直区"), this);
    menu_Grading->addAction(action_grading_vertical);

    QAction *action_grading_delete_zone = new QAction(tr("&删除区域"), this);
    menu_Grading->addAction(action_grading_delete_zone);
// ![8] 级放

// ![9] 常用操作
    QMenu *menu_Operation = ui->menuBar->addMenu(tr("常用操作"));
// ![9] 常用操作

// ![10] 帮助
    QMenu *menu_Help = ui->menuBar->addMenu(tr("帮助"));
    QAction *action_help_help = new QAction(tr("&帮助(&H)"), this);
    action_help_help->setShortcut(QKeySequence::HelpContents);
    menu_Help->addAction(action_help_help);

    menu_Help->addSeparator();

    QAction *action_help_license = new QAction(tr("&许可(&L)"), this);
    menu_Help->addAction(action_help_license);

    menu_Help->addSeparator();

    QAction *action_help_about = new QAction(tr("&关于"), this);
    menu_Help->addAction(action_help_about);
// ![10] 帮助

    connect(action_file_new, &QAction::triggered, this, &MainWindow::doNew);
    connect(action_file_open, &QAction::triggered, this, &MainWindow::doOpen);
    connect(action_file_save, &QAction::triggered, this, &MainWindow::doFileSave);
    connect(action_file_save_as, &QAction::triggered, this, &MainWindow::doFileSaveAs);
    connect(action_file_print, &QAction::triggered, this, &MainWindow::doPrint);
    connect(action_view_zoom_in, &QAction::triggered, this, &MainWindow::zoomIn);
    connect(action_view_zoom_out, &QAction::triggered, this, &MainWindow::zoomOut);
    connect(action_view_zoom_back, &QAction::triggered, this, &MainWindow::zoomBack);
    connect(action_modify_transform_rotate, &QAction::triggered, this, &MainWindow::doRotate);
    connect(action_modify_clear, &QAction::triggered, this, &MainWindow::doClear);
//    QMenu *file = menuBar()->addMenu(tr("&File"));
//    file->addAction(openAction);

    //QToolBar *toolBar = addToolBar(tr("&File"));

//    statusBar() ;

    area = new PaintArea;
    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(area); //将画布添加到 scrollArea 中
    scrollArea->widget()->setMinimumSize(2560,1600); //scrollArea 初始化大小设为800*600
    setCentralWidget(scrollArea); //将 scrollArea 加入到主窗口的中心区

    //浮动窗口
    QDockWidget *dock = new QDockWidget(this);//浮动窗口组件QDockWidget
    addDockWidget(Qt::LeftDockWidgetArea,dock);//浮动窗口里添加其他组件
    QDockWidget *dock2 = new QDockWidget(this);//浮动窗口组件QDockWidget
    addDockWidget(Qt::LeftDockWidgetArea,dock2);//浮动窗口里添加其他组件

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

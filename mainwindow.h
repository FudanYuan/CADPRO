#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "paintarea.h"
#include "project.h"
#include <QList>
#include <QActionGroup>
#include <QTreeWidget>
#include <QInputDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
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

    QMenu *menu_file;
    QAction *action_file_new;
    QAction *action_file_open;
    QAction *action_file_save;
    QAction *action_file_save_as;
    QAction *action_file_save_all;
    QAction *action_file_print;
    QAction *action_file_print_preview;
    QAction *action_file_print_setup;
    QMenu *menu_file_import;
    QAction *action_file_import_DXF;
    QAction *action_file_import_DVS;
    QAction *action_file_import_TEF;
    QMenu *menu_file_export;
    QAction *action_file_export_DXF;
    QAction *action_file_export_DVS;
    QAction *action_file_export_TEF;
    QAction *action_file_configuration;
    QAction *action_file_exit;

    QMenu *menu_draw;
    QAction *action_draw_line;
    QAction *action_draw_middle_axis;
    QAction *action_draw_polyline;
    QAction *action_draw_circle;
    QAction *action_draw_ellipse;
    QAction *action_draw_arc_by_3_points;
    QAction *action_draw_arc_by_3_points_2;
    QAction *action_draw_rectangle;
    QAction *action_draw_trapezium;
    QAction *action_draw_polygon;
    QAction *action_draw_stab_hole;
    QAction *action_draw_reference;
    QAction *action_draw_eyelet;
    QAction *action_draw_pattern_direction;
    QAction *action_draw_shank_line;
    QAction *action_draw_perpendicular;
    QMenu *menu_image;
    QAction *action_draw_image;
    QAction *action_draw_image_insert;
    QAction *action_draw_image_delete;
    QAction *action_draw_image_crop;
    QAction *action_draw_image_calibrate;
    QAction *action_draw_image_agjustement;
    QMenu *menu_digitizer;
    QAction *action_draw_digitizer_activate;
    QAction *action_draw_digitizer_set_position;
    QAction *action_draw_digitizer_configure;
    QMenu *menu_scanner;
    QAction *action_draw_scanner_vectorize_from_scanner;
    QAction *action_draw_scanner_select_source;
    QAction *action_draw_scanner_vectorize_image;

    QMenu *menu_insert;
    QAction *action_insert_offset;
    QAction *action_insert_advanced_offset;
    QAction *action_insert_text;
    QAction *action_insert_fillet;
    QAction *action_insert_notch;
    QAction *action_insert_notches_group;
    QAction *action_insert_notches_axis_tied_group;
    QAction *action_insert_size_notch;
    QAction *action_insert_remark;
    QAction *action_insert_smooth;
    QAction *action_insert_margin;
    QAction *action_insert_canal;
    QAction *action_insert_block;
    QAction *action_insert_holes_over_a_line;
    QMenu *menu_insert_array;
    QAction *action_insert_array_rectangular;
    QAction *action_insert_array_circular;

    QMenu *menu_modify;
    QAction *action_modify_undo;
    QAction *action_modify_redo;
    QAction *action_modify_clear;
    QAction *action_modify_delete;
    QAction *action_modify_cut;
    QAction *action_modify_copy;
    QAction *action_modify_paste;
    QAction *action_modify_paste_as_slave;
    QMenu *menu_modify_transform;
    QAction *action_modify_transform_move;
    QAction *action_modify_transform_rotate;
    QAction *action_modify_transform_rotate_knots_continuely;
    QAction *action_modify_transform_rotate_knots;
    QAction *action_modify_transform_scale;
    QAction *action_modify_transform_mirror;
    QAction *action_modify_transform_strech;
    QAction *action_modify_transform_explode;
    QAction *action_modify_transform_join;
    QAction *action_modify_transform_break;
    QAction *action_modify_transform_fillet;
    QAction *action_modify_transform_V_cut;
    QAction *action_modify_transform_spline;
    QAction *action_modify_trim;
    QAction *action_modify_extend;
    QAction *action_modify_modify_endpoint;
    QAction *action_modify_adjust_polyline;
    QAction *action_modify_start_point;
    QAction *action_modify_soften_up;
    QAction *action_modify_warp;
    QAction *action_modify_knots_reduction;
    QAction *action_modify_replace_part;
    QAction *action_modify_change_to;
    QAction *action_modify_make_master_line;
    QAction *action_modify_style;
    QAction *action_modify_springing;

    QMenu *menu_pattern;
    QAction *action_pattern_select;
    QAction *action_pattern_new;
    QAction *action_pattern_clone;
    QAction *action_pattern_delete;
    QAction *action_pattern_rotate;
    QAction *action_pattern_make_symmetic;
    QAction *action_pattern_position;
    QAction *action_pattern_extract_auto;
    QAction *action_pattern_extract_manual;
    QAction *action_pattern_find_perimeter;
    QAction *action_pattern_close_trim;
    QAction *action_pattern_check;
    QAction *action_pattern_devide;
    QAction *action_pattern_save_as_weft;

    QMenu *menu_view;
    QAction *action_view_xy_axes;
    QAction *action_view_grid;
    QAction *action_view_knots;
    QAction *action_view_image;
    QAction *action_view_design_rules;
    QAction *action_view_grading_rules;
    QAction *action_view_filled_patterns;
    QAction *action_view_zoom_window;
    QAction *action_view_zoom_all;
    QAction *action_view_zoom_in;
    QAction *action_view_zoom_out;
    QAction *action_view_zoom_back;
    QAction *action_view_voerlap_patterns;
    QAction *action_view_hide;
    QAction *action_view_show;
    QAction *action_view_clean;
    QAction *action_view_set_reference;
    QAction *action_view_lock_layout;
    QMenu *menu_view_tool_bar;
    QActionGroup *action_view_tool_slide;
    QAction *action_view_tool_find_style;
    QAction *action_view_tool_project;
    QAction *action_view_tool_properties;
    QAction *action_view_tool_customize;
    QAction *action_view_status_bar;

    QMenu *menu_utility;
    QAction *action_utility_measure_distance;
    QAction *action_utility_measure_line;
    QAction *action_create_pattern_card;
    QAction *action_utility_quick_nesting;
    QAction *action_utility_nest_by_job_card;
    QAction *action_utility_heel_cover;
    QAction *action_utility_pattern_search;

    QMenu *menu_grading;
    QAction *action_grading_parameters;
    QAction *action_grading_grade;
    QAction *action_grading_delete_graded_patterns;
    QMenu *menu_grading_reference;
    QAction *action_grading_reference_insert;
    QAction *action_grading_keep_margin;
    QAction *action_grading_delete_margin;
    QAction *action_grading_update_grading;
    QAction *action_grading_XY_lock;
    QAction *action_grading_delete_XY_lock;
    QAction *action_grading_horizontal_zone;
    QAction *action_grading_vertical_zone;
    QAction *action_grading_delete_zone;

    QMenu *menu_operation;

    QMenu *menu_help;
    QAction *action_help_help;
    QAction *action_help_license;
    QAction *action_help_about;

    QAction *action_object_snap_end_point;
    QAction *action_object_snap_center;
    QAction *action_object_snap_intersection;
    QAction *action_object_snap_knot;
    QAction *action_object_snap_ortho;
    QAction *action_object_snap_enter_coordinates;

    QToolBar *tool_slide;                    // 边栏
    QActionGroup *action_tool_slide;
    QAction *action_tool_slide_find_style;   // 寻找款型边栏
    QAction *action_tool_slide_project;      // 项目边栏
    QAction *action_tool_slide_properties;   // 属性边栏

    QMenu *menu_tree;
    QAction *action_tree_expand_all;
    QAction *action_tree_fold_all;

    QMenu *menu_tree_project;
    QAction *action_tree_project_rename;
    QAction *action_tree_project_add_layer;
    QAction *action_tree_project_save;
    QAction *action_tree_project_save_as;
    QAction *action_tree_project_close;

    QMenu *menu_tree_project_layer;
    QAction *action_tree_project_layer_rename;
    QAction *action_tree_project_layer_change_to;
    QAction *action_tree_project_layer_copy;
    QAction *action_tree_project_layer_paste;
    QMenu *menu_tree_project_layer_move_up;
    QAction *action_tree_project_layer_move_up_one;
    QAction *action_tree_project_layer_move_up_top;
    QMenu *menu_tree_project_layer_move_down;
    QAction *action_tree_project_layer_move_down_one;
    QAction *action_tree_project_layer_move_down_bottom;
    QAction *action_tree_project_layer_delete;

    QToolBar *tool_standard;
    QToolBar *tool_sketch;
    QToolBar *tool_object_snap;
    QToolBar *tool_zoom;
    QToolBar *tool_modify;
    QToolBar *tool_image;
    QToolBar *tool_digitizer;
    QToolBar *tool_grading;
    QToolBar *tool_scanner;
    QToolBar *tool_insert;
    QToolBar *tool_pattern;

    bool DockNestingEnabled = true;
    QDockWidget *dock_find_style;    // 浮动窗口1
    QDockWidget *dock_project;       // 浮动窗口2
    QDockWidget *dock_properties;    // 浮动窗口3
    QDockWidget *dock_paint_area;    // 主窗口

    QTreeWidget *tree_project;       // 项目树
    QTreeWidgetItem *tree_project_active_item; // 弹出菜单属于的那个项目
    QTreeWidgetItem *tree_project_layer_active_item; // 弹出菜单属于的那个项目

    Project *project_active;            // 活动项目
    PaintArea *area_active;             // 活动图层
    QList<Project *> list_project;      // 项目列表

    void initActions();         // 初始化action
    void initMenuBar();         // 初始化菜单栏
    void initToolBar();         // 初始化工具栏
    void initStatusBar();       // 初始化状态栏
    void initDockWidget();      // 初始化dock widget
    void initProject();         // 初始化项目
    QString getNewProjectName();    // 获取新项目名称

    Project *getProjectByName(QString project_name);
    PaintArea *getLayerByName(Project* project, QString layer_name);

    void showTreeMenu(QPoint pos);
    void updatePaintArea();             // 更新绘图区域
    bool maybeSave();                   // 是否保存项目
    bool saveFile(QString fileName);    // 实现文件的存储

private slots:
    void onActionFileNew();             // 新建项目操作
    void onActionFileOpen();            // 打开文件操作
    bool onActionFileSave();            // 保存文件操作
    bool onActionFileSaveAs();          // 文件另存为操作

    void onActionFileImportDXF();       // DXF文件导入操作
    void updateViewMenu();

    void onToolSlideChanged();

    void onDockFindStyleVisibilityChanged(bool visible);
    void onDockProjectVisibilityChanged(bool visible);
    void onDockPropertiesVisibilityChanged(bool visible);

    void slideFindStyleToggle();    // 边栏寻找款式
    void slideProjectToggle();      // 边栏项目
    void slidePropertiesToggle();   // 边栏属性

    void onActionTreeExpandAll();
    void onActionTreeFoldAll();

    void onTreeProjectItemDoubleClicked(QTreeWidgetItem *item, int i);
    void onActionTreeProjectAddLayer(); // 项目添加图层
    void onActionTreeProjectSave();
    void onActionTreeProjectSaveAs();
    void onActionTreeProjectRename();
    void onActionTreeProjectClose();

    void onActionTreeProjectLayerChangeTo();
    void onActionTreeProjectLayerMoveUpOne();
    void onActionTreeProjectLayerMoveUpTop();
    void onActionTreeProjectLayerMoveDownOne();
    void onActionTreeProjectLayerMoveDownBottom();
    void onActionTreeProjectLayerRename();
    void onActionTreeProjectLayerDelete();
};

#endif // MAINWINDOW_H

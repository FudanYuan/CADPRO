#ifndef SKETCH_H
#define SKETCH_H

#include <QMainWindow>
#include <QList>
#include <QActionGroup>
#include <QTreeWidget>
#include <QInputDialog>
#include <QSettings>
#include <QLabel>

#include "configure.h"
#include "project.h"
#include "view.h"
#include "scene.h"
#include "shape.h"


namespace Ui {
class Sketch;
}

class Sketch : public QMainWindow
{
    Q_OBJECT

public:
    explicit Sketch(QWidget *parent = 0);
    ~Sketch();

    void initActions();         // 初始化action
    void initMenuBar();         // 初始化菜单栏
    void initToolBar();         // 初始化工具栏
    void initStatusBar();       // 初始化状态栏
    void initDockWidget();      // 初始化dock widget
    void initConfiguration();   // 初始化配置
    void initProjectView();     // 初始化项目
    void addProject();          // 添加项目
    QString getNewProjectName();// 获取新项目名称
    Project *getProjectByName(QString project_name);  // 根据项目获取对象

    void showTreeMenu(QPoint pos);      // 显示项目栏菜单
    void updateScene();             // 更新绘图区域
    bool maybeSave();                   // 是否保存项目
    bool saveFile(QString fileName);    // 实现文件的存储
    void saveLayout();  // 保存窗口布局
    void loadLayout();  // 保存窗口布局
protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::Sketch *ui;

    Configure *configCopy;               // 配置

    View *view;                         // 视图

    Project *project_active;            // 活动项目
    Scene *scene_active;                // 活动图层
    QList<Project *> project_list;      // 项目列表

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
    QAction *action_tree_project_add_scene;
    QAction *action_tree_project_save;
    QAction *action_tree_project_save_as;
    QAction *action_tree_project_close;

    QMenu *menu_tree_project_scene;
    QAction *action_tree_project_scene_rename;
    QAction *action_tree_project_scene_change_to;
    QAction *action_tree_project_scene_copy;
    QAction *action_tree_project_scene_paste;
    QMenu *menu_tree_project_scene_move_up;
    QAction *action_tree_project_scene_move_up_one;
    QAction *action_tree_project_scene_move_up_top;
    QMenu *menu_tree_project_scene_move_down;
    QAction *action_tree_project_scene_move_down_one;
    QAction *action_tree_project_scene_move_down_bottom;
    QAction *action_tree_project_scene_delete;

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

    QLabel *mousePositionLabel;  //  鼠标坐标
    bool DockNestingEnabled;
    QDockWidget *dock_find_style;    // 浮动窗口1
    QDockWidget *dock_project;       // 浮动窗口2
    QDockWidget *dock_properties;    // 浮动窗口3
    QDockWidget *dock_scene;    // 主窗口

    QTreeWidget *tree_project;       // 项目树
    QList<QTreeWidgetItem *> tree_project_item_list; // 项目树列表
    QTreeWidgetItem *tree_project_active_item; // 弹出菜单属于的那个项目
    QTreeWidgetItem *tree_project_scene_active_item; // 弹出菜单属于的那个项目的图层

private slots:
    // FILE
    void onActionFileNew();             // 新建项目操作
    void onActionFileOpen();            // 打开文件操作
    bool onActionFileSave();            // 保存文件操作
    bool onActionFileSaveAs();          // 文件另存为操作
    bool onActionFileSaveAll();         // 文件全部保存
    void onActionFilePrint();           //打印文件
    void onActionFilePrintPreview();    //打印预览
    void onActionFilePrintSetup();      //打印设置
    void onActionFileImportDXF();       // 导入DXF
    void onActionFileImportDVS();       // 导入DVS
    void onActionFileImportTEF();       // 导入TEF
    void onActionFileExportDXF();       // 导出DXF
    void onActionFileExportDVS();       // 导出DVS
    void onActionFileExportTEF();       // 导出TEF
    void onActionFileConfiguration();   // 系统配置操作
    void onActionFileExit();            // 退出提示保存
    // FILE

    // DRAW
    void onActionDrawLine();            // 画直线
    void onActionDrawEllipse();         // 画椭圆
    void onActionDrawRect();            // 画矩形
    void onActionDrawMiddleAxis();      // 画中轴线
    void onActionDrawPolyline();        // 画曲折线
    void onActionDrawCircle();          // 画圆
    void onActionDrawArcBy3Pnts();      // 画三点圆弧
    void onActionDrawArcBy3Pnts2();     // 画三点圆弧2
    void onActionDrawTrapezium();       // 画梯形
    void onActionDrawPolygon();         // 画正多边形
    void onActionDrawStabHole();        // 画冲孔
    void onActionDrawReference();       // 画参考点
    void onActionDrawEyelet();          // 画鸡眼孔

    void onActionDrawPatternDirection();// 图案方向
    void onActionDrawShankLine();       // 上插线
    void onActionDrawPerpendicular();   // 垂直线

    void onActionDrawImage();           // 获取图像
    void onActionDrawImageInsert();     // 插入图片
    void onActionDrawImageDelete();     // 删除图片
    void onActionDrawImageCrop();       // 裁剪图片
    void onActionDrawImageCalibrate();  // 校准刻度
    void onActionDrawImageAdjustment(); // 调整图片

    void onActionDrawDigitizerActivate(); // 启用数字化仪
    void onActionDrawDigitizerSetPosition(); // 设置shell位置
    void onActionDrawDigitizerConfigue(); // 配置数字化仪

    void onActionDrawScannerVectorizeFromScanner(); // 扫描矢量
    void onActionDrawScannerSelectSource();         // 选择源
    void onActionDrawScannerVectorizeImage();       // 矢量图
    // DRAW

    // INSERT
    void onActionInsertOffset();                // 创建偏移
    void onActionInsertAdvancedOffset();        // 创建高级偏移
    void onActionInsertText();                  // 插入文本
    void onActionInsertFillet();                // 倒圆角
    void onActionInsertNotch();                 // 插入凹槽
    void onActionInsertNotchesGroup();          // 插入凹槽组
    void onActionInsertNotchesAxisTiedGroup();  // 凹槽绑定群
    void onActionInsertSizeNotch();             // 凹槽尺寸编号
    void onActionInsertRemark();                // 插入备注
    void onActionInsertSmooth();                // 角度平滑
    void onActionInsertMargin();                // 插入边距/余量
    void onActionInsertCanel();                 // 插入管道
    void onActionInsertBlock();                 // 插入块
    void onActionInsertHolesOverALine();        // 插入沿线孔
    void onActionInsertArrayRectangle();        // 创建矩形阵列
    void onActionInsertArrayCircular();         // 创建圆形阵列
    // INSERT


    // MODIFY
    void onActionModifyUndo();                  // 撤销上个操作
    void onActionModifyRedo();                  // 重做撤销操作
    void onActionModifyClear();                 // 清空
    void onActionModifyDelete();                // 删除
    void onActionModifyCut();                   // 剪切
    void onActionModifyCopy();                  // 复制
    void onActionModifyPaste();                 // 粘贴
    void onActionModifyPasteAsSlave();          // 粘贴为副本
    void onActionModifyTransformMove();                     // 移动
    void onActionModifyTransformRotate();                   // 旋转
    void onActionModifyTransformRotateKnotsContinuely();    // 连续旋转
    void onActionModifyTransformRotateKnots();              // 旋转节点
    void onActionModifyTransformScale();                    // 比例
    void onActionModifyTransformMirror();                   // 镜像
    void onActionModifyTransformStrech();                   // 拉伸
    void onActionModifyTransformExplode();                  // 爆炸
    void onActionModifyTransformJoin();                     // 连接
    void onActionModifyTransformBreak();                    // 断开
    void onActionModifyTransformfillet();                   // 倒圆角
    void onActionModifyTransformVCut();                     // V切
    void onActionModifyTransformSpline();                   // 创建样条
    void onActionModifyTrim();                              // 修剪
    void onActionModifyExtend();                    // 延伸
    void onActionModifyEndpoint();                  // 修改端点
    void onActionModifyAdjustPolyLine();            // 调整折线
    void onActionModifyStartpoint();                // 起点
    void onActionModifySoftenup();                  // 软化
    void onActionModifyWarp();                      // 弯曲
    void onActionModifyKnotsReduction();            // 减少节点
    void onActionModifyReplacePart();               // 更换部分
    void onActionModifyChangeTo();                  // 更改至
    void onActionModifyMakeMasterLine();            //制作主线
    void onActionModifyStyle();                     // 实体线样式
    void onActionModifySpringing();                 // 弹回
    // MODIFY


    // PATTERN
    void onActionPatternSelect();           //选择
    void onActionPatternNew();              //新建
    void onActionPatternClone();            //克隆
    void onActionPatternDelete();           //删除
    void onActionPatternRotate();           //旋转
    void onActionPatternMakeSymmetic();     //使对称
    void onActionPatternPosition();         //位置
    void onActionPatternExtractAuto();      //自动提取
    void onActionPatternExtractManual();    //手动提取
    void onActionPatternPerimeter();        //周长
    void onActionPatternTrim();             //关闭修剪
    void onActionPatternCheck();            //检查
    void onActionPatternDevide();           //瓜分
    void onActionPatternSaveAsWeft();       //另存为纬线
    // PATTERN

    // VIEW
    void onActionViewXYAxes(bool toggled);
    void onActionViewGrid(bool toggled);
    void onActionViewKnots(bool toggled);
    void onActionViewImage(bool toggled);
    void onActionViewDesignRules(bool toggled);
    void onActionViewGradingRules(bool toggled);
    void onActionViewFilledPatterns(bool toggled);

    void onActionViewZoomWindow();  // 选择区域放大
    void onActionViewZoomAll();     // 全部图形居中最大
    void onActionViewZoomIn();      // 放大
    void onActionViewZoomOut();     // 缩小
    void onActionViewZoomBack();    // 缩小
    void onActionViewVoerlapPatterns();

    void onActionViewHide(bool toggled);
    void onActionViewShow(bool toggled);
    void onActionViewClean(bool toggled);
    void onActionViewSetReference();

    void onActionViewLockLayout(bool toggled);
    void onActionViewToolFindStyleToggled(bool toggled);
    void onActionViewToolProjectToggled(bool toggled);
    void onActionViewToolPropertiesToggled(bool toggled);
    void onActionViewToolSlideToggled(bool toggled);
    void onActionViewToolCustomizeToggled(bool toggled);
    void onActionViewStatusBar(bool toggled);
    // VIEW


    // UTILITY
    void onActionUtilityMeasureDis();        	//测量距离
    void onActionUtilityMeasureLine();        	//测量线长
    void onActionCreatorPatternCard();        	//创建图样卡
    void onActionUtilityQuickNesting();        	//快速排版
    void onActionUtilityNestByJobCard();        //转到排版
    void onActionUtilityHeelCover();        	//后跟
    void onActionUtilityPatternSearch();        //搜索图样
    // Utility


    // GRID
    void onActionGradingParameters();           //图样
    void onActionGradingGrade();                //级放
    void onActionGradingDeletedGradedPatterns();//删除级放
    void onActionGradingReferenceInsert();     	//插入
    void onActionGradingKeepMargin();           //保留边距
    void onActionGradingDeleteMargin();     	//删除边距
    void onActionGradingUpdate();               //更新级放
    void onActionGradingXYLock();               //XY锁定
    void onActionGradingDeleteXYLock();     	//解除XY锁定
    void onActionGradingHorizontalZone();     	//水平区
    void onActionGradingVerticalZone();     	//垂直区
    void onActionGradingDeleteZone();           //删除区域
    // GRID


    // HELP
    void onActionHelpHelp();             //帮助
    void onActionHelpLicense();          //许可
    void onActionHelpAbout();            //关于
    // HELP


    void onActionObjectSnapEndpoint();  	//实体端点
    void onActionObjectSnapCenter();  		//实体中心
    void onActionObjectSnapInterSection();  	//交叉点
    void onActionObjectSnapKnot();  		//节点
    void onActionObjectSnapOrtho();  		//直角正交
    void onActionObjectSnapEnterCoordinates();  //输入坐标


    // TOOL SLIDE
    void onToolSlideChanged();

    void onDockFindStyleVisibilityChanged(bool visible);
    void onDockProjectVisibilityChanged(bool visible);
    void onDockPropertiesVisibilityChanged(bool visible);

    void slideFindStyleToggle();    // 边栏寻找款式
    void slideProjectToggle();      // 边栏项目
    void slidePropertiesToggle();   // 边栏属性

    void onActionTreeExpandAll();
    void onActionTreeFoldAll();

    void onTreeProjectItemDoubleClicked(QTreeWidgetItem *item);
    void onActionTreeProjectAddScene(); // 项目添加图层
    void onActionTreeProjectSave();
    void onActionTreeProjectSaveAs();
    void onActionTreeProjectRename();
    void onActionTreeProjectClose();

    void onActionTreeProjectSceneChangeTo();
    void onActionTreeProjectSceneMoveUpOne();
    void onActionTreeProjectSceneMoveUpTop();
    void onActionTreeProjectSceneMoveDownOne();
    void onActionTreeProjectSceneMoveDownBottom();
    void onActionTreeProjectSceneRename();
    void onActionTreeProjectSceneDelete();
    // TOOL SLIDE

    void updateViewMenu();

signals:
    void configChanged(QString name, QVariant value);

public slots:
    void onProjectNameChanged(QString lastName, QString presentName);

    void onMousePositionChanged(QPointF pos);  // 鼠标位置更新

    void onSceneItemsChanged();  // 响应场景图元改变
    void onPointSelected(Point *point);
    void onLineSelected(Line *line);  // 响应图形被选中
//    void onArcSelected(Arc *arc);
//    void onEllipseSelected(Ellipse *ellipse);
    void onCircleSelected(Circle *circle);
    void onRectSelected(Rect *rect);
    void onPolyLineSelected(PolyLine *polyline);
};

#endif // SKETCH_H

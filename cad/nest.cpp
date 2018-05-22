#include "nest.h"
#include "ui_nest.h"
#include "rectnestengine.h"
#include <QDockWidget>
#include <QTreeWidgetItem>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QRegExp>
#include <QValidator>
#include <QDebug>

#include "piece.h"
#include "packpointnestengine.h"
#include "queue"
#include "quadtreenode.h"
#include <sys/time.h>

#define COUNT 20

// 初始化矩形排版引擎
QList<Nest::Component> RectNestEngine::components;  // 零件
QList<RectNestEngine::MinRect> RectNestEngine::compMinRects;  // 零件的最小矩形
QList<RectNestEngine::EmptyRectArea> RectNestEngine::emptyRectArea;  // 空白矩形
double RectNestEngine::mWidth = 0;  // 材料宽度
double RectNestEngine::mHeight = 0;  // 材料高度
double RectNestEngine::maxHight = 0;  // 最大高度值
long RectNestEngine::allRectsArea = 0; // 矩形零件面积
long RectNestEngine::minRectsArea = LONG_MAX; // 矩形零件面积

Nest::Nest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Nest)
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
    initConfiguration();// 初始化配置
    initProjectView();  // 初始化项目视图
    initPieceView();  // 初始化切割件视图
    initNestView();  // 初始化排版视图
    initStatusBar();    // 初始化状态栏
    initConnect();  // 初始化信号和槽函数
}

Nest::~Nest()
{
    delete ui;
}

void Nest::initActions()
{
// ![1] 文件
    action_file_new = new QAction(tr("&新建"), this);
    action_file_new->setShortcut(QKeySequence::New);
    action_file_new->setStatusTip(tr("新建一个项目"));
    connect(action_file_new, &QAction::triggered, this, &Nest::onActionFileNew);

    action_file_open = new QAction(tr("&打开"), this);
    action_file_open->setShortcut(QKeySequence::Open);
    action_file_open->setStatusTip(tr("打开一个已存的项目"));
    connect(action_file_open, &QAction::triggered, this, &Nest::onActionFileOpen);

    action_file_save = new QAction(tr("&保存"), this);
    action_file_save->setShortcut(QKeySequence::Save);
    action_file_save->setStatusTip(tr("保存当前项目"));
    connect(action_file_save, &QAction::triggered, this, &Nest::onActionFileSave);

    action_file_save_as = new QAction(tr("&另存为"), this);
    action_file_save_as->setShortcut(QKeySequence::SaveAs);
    action_file_save_as->setStatusTip(tr("另存当前项目"));
    connect(action_file_save_as, &QAction::triggered, this, &Nest::onActionFileSaveAs);

    action_file_save_all = new QAction(tr("&保存全部"), this);
    action_file_save_all->setStatusTip(tr("保存所有打开的项目"));
    connect(action_file_save_all, &QAction::triggered, this, &Nest::onActionFileSaveAll);

    action_file_print = new QAction(tr("&打印"), this);
    action_file_print->setShortcut(QKeySequence::Print);
    action_file_print->setStatusTip(tr("打印当前视图"));
    connect(action_file_print, &QAction::triggered, this, &Nest::onActionFilePrint);

    action_file_print_preview = new QAction(tr("&打印预览"), this);
    action_file_print_preview->setStatusTip(tr("显示当前项目打印预览"));
    action_file_print_preview->setDisabled(true);
    connect(action_file_print_preview, &QAction::triggered, this, &Nest::onActionFilePrintPreview);

    action_file_print_setup = new QAction(tr("&打印设置"), this);
    action_file_print_setup->setStatusTip(tr("更改打印机和打印选项"));
    action_file_print_setup->setDisabled(true);
    connect(action_file_print_setup, &QAction::triggered, this, &Nest::onActionFilePrintSetup);

    action_file_configuration = new QAction(tr("&配置"), this);
    action_file_configuration->setStatusTip(tr("修改cadPro配置"));
    connect(action_file_configuration, &QAction::triggered, this, &Nest::onActionFileConfiguration);

    action_file_exit = new QAction(tr("&退出"), this);
    action_file_exit->setShortcut(QKeySequence::Quit);
    action_file_exit->setStatusTip(tr("退出应用程序；提示保存项目"));
    connect(action_file_exit, &QAction::triggered, this, &Nest::onActionFileExit);
// ![1] 文件

// ![2] 编辑
    action_edit_undo = new QAction(tr("&撤销"), this);
    action_edit_undo->setShortcut(QKeySequence::Undo);
    action_edit_undo->setStatusTip(tr("撤销上一个操作"));
    //action_edit_undo->setDisabled(true);
    connect(action_edit_undo, &QAction::triggered, this, &Nest::onActionEditUndo);

    action_edit_redo = new QAction(tr("&重做"), this);
    action_edit_redo->setShortcut(QKeySequence::Redo);
    action_edit_redo->setDisabled(true);
    action_edit_redo->setStatusTip(tr("重做撤销的操作"));
    connect(action_edit_redo, &QAction::triggered, this, &Nest::onActionEditRedo);

    action_edit_clear = new QAction(tr("&清空"), this);
    action_edit_clear->setStatusTip(tr("清空排版区"));
    action_edit_clear->setDisabled(true);
    connect(action_edit_clear, &QAction::triggered, this, &Nest::onActionEditClear);

    action_edit_delete = new QAction(tr("&删除"), this);
    action_edit_delete->setShortcut(QKeySequence::Delete);
    action_edit_delete->setStatusTip(tr("删除实体"));
    action_edit_delete->setDisabled(true);
    connect(action_edit_delete, &QAction::triggered, this, &Nest::onActionEditDelete);

    action_edit_cut = new QAction(tr("&剪切"), this);
    action_edit_cut->setShortcut(QKeySequence::Cut);
    action_edit_cut->setStatusTip(tr("剪切选择并将其放在剪贴板上"));
    action_edit_cut->setDisabled(true);
    connect(action_edit_cut, &QAction::triggered, this, &Nest::onActionEditCut);

    action_edit_copy = new QAction(tr("&复制"), this);
    action_edit_copy->setShortcut(QKeySequence::Copy);
    action_edit_copy->setStatusTip(tr("将所选线复制到粘贴板"));
    action_edit_copy->setDisabled(true);
    connect(action_edit_copy, &QAction::triggered, this, &Nest::onActionEditCopy);

    action_edit_paste = new QAction(tr("&粘贴"), this);
    action_edit_paste->setShortcut(QKeySequence::Paste);
    action_edit_paste->setStatusTip(tr("粘贴先前复制的实体"));
    action_edit_paste->setDisabled(true);
    connect(action_edit_paste, &QAction::triggered, this, &Nest::onActionEditPaste);
// ![2] 编辑

// ![3] 排版
    action_nest_config = new QAction(tr("自动排版配置"));
    action_nest_config->setStatusTip(tr("自动排版配置"));
    connect(action_nest_config, &QAction::triggered, this, &Nest::onActionNestConfig);

    action_nest_side_left = new QAction(tr("左靠边"));
    action_nest_side_left->setStatusTip(tr("零件紧靠材料左边"));
    action_nest_side_left->setDisabled(true);
    connect(action_nest_side_left, &QAction::triggered, this, &Nest::onActionNestSideLeft);

    action_nest_side_right = new QAction(tr("右靠边"));
    action_nest_side_right->setStatusTip(tr("零件紧靠材料右边"));
    action_nest_side_right->setDisabled(true);
    connect(action_nest_side_right, &QAction::triggered, this, &Nest::onActionNestSideRight);

    action_nest_side_top = new QAction(tr("顶靠边"));
    action_nest_side_top->setStatusTip(tr("零件紧靠材料顶部"));
    action_nest_side_top->setDisabled(true);
    connect(action_nest_side_top, &QAction::triggered, this, &Nest::onActionNestSideTop);

    action_nest_side_bottom = new QAction(tr("底靠边"));
    action_nest_side_bottom->setStatusTip(tr("零件紧靠材料底部"));
    action_nest_side_bottom->setDisabled(true);
    connect(action_nest_side_bottom, &QAction::triggered, this, &Nest::onActionNestSideBottom);

    action_nest_direction_horizontal = new QAction(tr("横向"));
    action_nest_direction_horizontal->setStatusTip(tr("横向排版，即自左至右"));
    action_nest_direction_horizontal->setDisabled(true);
    connect(action_nest_direction_horizontal, &QAction::triggered, this, &Nest::onActionNestDirectionHorizontal);

    action_nest_direction_vertical = new QAction(tr("纵向"));
    action_nest_direction_vertical->setStatusTip(tr("纵向排版，即自上而下"));
    action_nest_direction_vertical->setDisabled(true);
    connect(action_nest_direction_vertical, &QAction::triggered, this, &Nest::onActionNestSideDirectionVertical);
// ![3] 排版

// ![4] 材料
    action_sheet_manager = new QAction(tr("材料管理"));
    action_sheet_manager->setStatusTip(tr("创建或编辑材料"));
    connect(action_sheet_manager, &QAction::triggered, this, &Nest::onActionSheetManager);

    action_sheet_add = new QAction(tr("增加材料"));
    action_sheet_add->setStatusTip(tr("增加新的材料"));
    action_sheet_add->setDisabled(true);
    connect(action_sheet_add, &QAction::triggered, this, &Nest::onActionSheetAdd);

    action_sheet_remove = new QAction(tr("删除材料"));
    action_sheet_remove->setStatusTip(tr("删除当前材料"));
    action_sheet_remove->setDisabled(true);
    connect(action_sheet_remove, &QAction::triggered, this, &Nest::onActionSheetRemove);

    action_sheet_duplicate = new QAction(tr("重复材料"));
    action_sheet_duplicate->setStatusTip(tr("重复当前材料"));
    action_sheet_duplicate->setDisabled(true);
    connect(action_sheet_duplicate, &QAction::triggered, this, &Nest::onActionSheetDuplicate);

    action_sheet_auto_duplicate = new QAction(tr("自动重复材料"));
    action_sheet_auto_duplicate->setStatusTip(tr("自动重复当前材料"));
    action_sheet_auto_duplicate->setDisabled(true);
    connect(action_sheet_auto_duplicate, &QAction::triggered, this, &Nest::onActionSheetAutoDuplicate);

    action_sheet_previous = new QAction(tr("上一张"));
    action_sheet_previous->setStatusTip(tr("转到上一张材料"));
    action_sheet_previous->setDisabled(true);
    connect(action_sheet_previous, &QAction::triggered, this, &Nest::onActionSheetPrevious);

    action_sheet_next = new QAction(tr("下一张"));
    action_sheet_next->setStatusTip(tr("转到下一张材料"));
    action_sheet_next->setDisabled(true);
    connect(action_sheet_next, &QAction::triggered, this, &Nest::onActionSheetNext);

    action_sheet_sheet_number = new QAction(tr("输入材料序号"));
    action_sheet_sheet_number->setStatusTip(tr("跳转至排版中的材料序号"));
    action_sheet_sheet_number->setDisabled(true);
    connect(action_sheet_sheet_number, &QAction::triggered, this, &Nest::onActionSheetSheetNumber);

    action_sheet_use_last_sheet = new QAction(tr("使用最后的材料"));
    action_sheet_use_last_sheet->setStatusTip(tr("重复使用最后一张已切割过的材料"));
    action_sheet_use_last_sheet->setDisabled(true);
    connect(action_sheet_use_last_sheet, &QAction::triggered, this, &Nest::onActionSheetUseLastSheet);

    action_sheet_sheet_property = new QAction(tr("属性"));
    action_sheet_sheet_property->setStatusTip(tr("更改材料&余良/边距"));
    action_sheet_sheet_property->setDisabled(true);
    connect(action_sheet_sheet_property, &QAction::triggered, this, &Nest::onActionSheetProperty);
// ![4] 材料

// ![5] 视图
    action_view_grid = new QAction(tr("&网格"), this);
    action_view_grid->setCheckable(true);
    action_view_grid->setStatusTip(tr("显示/隐藏网格"));
    connect(action_view_grid, &QAction::toggled, this, &Nest::onActionViewGrid);

    action_view_zoom_window = new QAction(tr("&缩放窗口"), this);
    action_view_zoom_window->setStatusTip(tr("&缩放窗口"));
    connect(action_view_zoom_window, &QAction::triggered, this, &Nest::onActionViewZoomWindow);

    action_view_zoom_all = new QAction(tr("&全部缩放"), this);
    action_view_zoom_all->setStatusTip(tr("&全部缩放"));
    connect(action_view_zoom_all, &QAction::triggered, this, &Nest::onActionViewZoomAll);

    action_view_zoom_in = new QAction(tr("&放大"), this);
    action_view_zoom_in->setShortcut(QKeySequence::ZoomIn);
    action_view_zoom_in->setStatusTip(tr("放大"));
    connect(action_view_zoom_in, &QAction::triggered, this, &Nest::onActionViewZoomIn);

    action_view_zoom_out = new QAction(tr("&缩小"), this);
    action_view_zoom_out->setShortcut(QKeySequence::ZoomOut);
    action_view_zoom_out->setStatusTip(tr("缩小"));
    connect(action_view_zoom_out, &QAction::triggered, this, &Nest::onActionViewZoomOut);

    action_view_zoom_back = new QAction(tr("&还原"), this);
    action_view_zoom_back->setStatusTip(tr("还原"));
    connect(action_view_zoom_back, &QAction::triggered, this, &Nest::onActionViewZoomBack);

    action_view_lock_layout = new QAction(tr("&锁定布局"), this);
    action_view_lock_layout->setCheckable(true);
    connect(action_view_lock_layout, &QAction::toggled, this, &Nest::onActionViewLockLayout);

    action_view_tool_projects = new QAction(tr("&项目列表"), this);
    action_view_tool_projects->setCheckable(true);
    connect(action_view_tool_projects, &QAction::toggled, this, &Nest::onActionViewToolProjectsToggled);

    action_view_tool_pieces = new QAction(tr("&零件列表"), this);
    action_view_tool_pieces->setCheckable(true);
    connect(action_view_tool_pieces, &QAction::toggled, this, &Nest::onActionViewToolPiecesToggled);

    action_view_tool_sheets = new QAction(tr("&材料列表"), this);
    action_view_tool_sheets->setCheckable(true);
    connect(action_view_tool_sheets, &QAction::toggled, this, &Nest::onActionViewToolSheetsToggled);

    action_view_tool_slide = new QActionGroup(this);
    action_view_tool_slide->setExclusive(false);

    action_view_tool_slide->addAction(action_view_tool_projects);
    action_view_tool_slide->addAction(action_view_tool_pieces);
    action_view_tool_slide->addAction(action_view_tool_sheets);

    connect(action_view_lock_layout, &QAction::toggled, action_view_tool_slide, &QActionGroup::setDisabled);

    action_view_tool_customize = new QAction(tr("&自定义"),this);
    connect(action_view_tool_customize, &QAction::toggled, this, &Nest::onActionViewToolCustomizeToggled);

    action_view_status_bar = new QAction(tr("&状态栏"), this);
    action_view_status_bar->setCheckable(true);
    action_view_status_bar->setChecked(true);
    connect(action_view_status_bar, &QAction::toggled, this, &Nest::onActionViewStatusBar);
// ![5] 视图

// ![6] 实用
    action_utility_measure_distance = new QAction(tr("&测量距离"), this);
    connect(action_utility_measure_distance, &QAction::triggered, this, &Nest::onActionUtilityMeasureDis);
// ![6] 实用

// ![7] 帮助
    action_help_help = new QAction(tr("&帮助(&H)"), this);
    action_help_help->setShortcut(QKeySequence::HelpContents);
    connect(action_help_help, &QAction::triggered, this, &Nest::onActionHelpHelp);

    action_help_license = new QAction(tr("&许可(&L)"), this);
    connect(action_help_license, &QAction::triggered, this, &Nest::onActionHelpLicense);

    action_help_about = new QAction(tr("&关于"), this);
    connect(action_help_about, &QAction::triggered, this, &Nest::onActionHelpAbout);
// ![7] 帮助
}

void Nest::initMenuBar()
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
    menu_file->addAction(action_file_configuration);
    menu_file->addAction(action_file_exit);
// ![1] 文件栏

// ![2] 编辑栏
    menu_edit = ui->menuBar->addMenu(tr("编辑(&E)"));
    menu_edit->addAction(action_edit_undo);
    menu_edit->addAction(action_edit_redo);
    menu_edit->addAction(action_edit_clear);
    menu_edit->addSeparator();
    menu_edit->addAction(action_edit_delete);
    menu_edit->addAction(action_edit_copy);
    menu_edit->addAction(action_edit_paste);
// ![2] 编辑栏

// ![3] 排版栏
    menu_nest = ui->menuBar->addMenu(tr("排版"));
    menu_nest->addAction(action_nest_config);
    menu_nest->addSeparator();
    menu_action_nest_side = menu_nest->addMenu(tr("靠边"));
    menu_action_nest_side->addAction(action_nest_side_left);
    menu_action_nest_side->addAction(action_nest_side_right);
    menu_action_nest_side->addAction(action_nest_side_top);
    menu_action_nest_side->addAction(action_nest_side_bottom);
    menu_nest->addMenu(menu_action_nest_side);
    menu_nest->addSeparator();
    menu_action_nest_direction = menu_nest->addMenu(tr("方向"));
    menu_action_nest_direction->addAction(action_nest_direction_horizontal);
    menu_action_nest_direction->addAction(action_nest_direction_vertical);
// ![3] 排版栏

// ![4] 材料栏
    menu_sheet = ui->menuBar->addMenu(tr("材料(&M)"));
    menu_sheet->addAction(action_sheet_manager);
    menu_sheet->addSeparator();
    menu_sheet->addAction(action_sheet_add);
    menu_sheet->addAction(action_sheet_remove);
    menu_sheet->addAction(action_sheet_duplicate);
    menu_sheet->addAction(action_sheet_auto_duplicate);
    menu_sheet->addSeparator();
    menu_sheet->addAction(action_sheet_previous);
    menu_sheet->addAction(action_sheet_next);
    menu_sheet->addAction(action_sheet_sheet_number);
    menu_sheet->addSeparator();
    menu_sheet->addAction(action_sheet_use_last_sheet);
    menu_sheet->addSeparator();
    menu_sheet->addAction(action_sheet_sheet_property);
// ![4] 材料栏

// ![5] 查看
    menu_view = ui->menuBar->addMenu(tr("查看(&V)"));
    menu_view->addAction(action_view_grid);
    menu_view->addSeparator();
    menu_view->addAction(action_view_zoom_window);
    menu_view->addAction(action_view_zoom_all);
    menu_view->addAction(action_view_zoom_in);
    menu_view->addAction(action_view_zoom_out);
    menu_view->addAction(action_view_zoom_back);
    menu_view->addSeparator();
    menu_view->addAction(action_view_lock_layout);
    menu_view_tool_bar = new QMenu(tr("&工具栏"), this);
    menu_view_tool_bar->addActions(action_view_tool_slide->actions());
    menu_view_tool_bar->addSeparator();
    menu_view_tool_bar->addAction(action_view_tool_customize);
    menu_view->addMenu(menu_view_tool_bar);
    menu_view->addAction(action_view_status_bar);
    menu_view->addSeparator();
// ![5] 查看

// ![6] 实用
    menu_utility = ui->menuBar->addMenu(tr("效用(&U)"));
    menu_utility->addAction(action_utility_measure_distance);
// ![6] 实用

// ![7] 帮助
    menu_help = ui->menuBar->addMenu(tr("帮助"));
    menu_help->addAction(action_help_help);
    menu_help->addSeparator();
    menu_help->addAction(action_help_license);
    menu_help->addSeparator();
    menu_help->addAction(action_help_about);
// ![7] 帮助
}

void Nest::initToolBar()
{
    if(ui->toolBar) delete ui->toolBar;
// ![1] 边栏工具栏
//    tool_slide = new QToolBar(tr("边栏"), this);
//    addToolBar(Qt::LeftToolBarArea, tool_slide);
//    tool_slide->setOrientation(Qt::Vertical);
//    tool_slide->setAllowedAreas(Qt::LeftToolBarArea);
//    tool_slide->setFloatable(false);
//    tool_slide->setMovable(false);
//    tool_slide->addActions(action_tool_slide->actions());
//    onToolSlideChanged();
// ![1] 边栏工具栏

// ![2] 标准工具栏
    tool_standard = new QToolBar(tr("标准"), this);
    tool_standard->setOrientation(Qt::Horizontal);
    tool_standard->setAllowedAreas(Qt::AllToolBarAreas);

    tool_standard->addAction(action_file_new);
    tool_standard->addAction(action_file_open);
    tool_standard->addAction(action_file_save);
    tool_standard->addSeparator();
    tool_standard->addAction(action_edit_cut);
    tool_standard->addAction(action_edit_copy);
    tool_standard->addAction(action_edit_paste);
    tool_standard->addAction(action_file_print);
    tool_standard->addAction(action_help_about);
    tool_standard->addAction(action_help_help);
    tool_standard->addSeparator();
    tool_standard->addAction(action_edit_undo);
    tool_standard->addAction(action_edit_redo);
// ![2] 标准工具栏

// ![4] 视图工具栏
    tool_view = new QToolBar(tr("视图"), this);
    tool_view->setOrientation(Qt::Horizontal);
    tool_view->setAllowedAreas(Qt::AllToolBarAreas);

    tool_view->addAction(action_view_zoom_window);
    tool_view->addAction(action_view_zoom_in);
    tool_view->addAction(action_view_zoom_out);
    tool_view->addAction(action_view_zoom_all);
    tool_view->addSeparator();
    tool_view->addAction(action_view_grid);
// ![4] 视图工具栏

// ![5] 排版工具栏
    tool_nest = new QToolBar(tr("排版"), this);
    tool_nest->setOrientation(Qt::Horizontal);
    tool_nest->setAllowedAreas(Qt::AllToolBarAreas);

    tool_nest->addAction(action_nest_config);
    tool_nest->addSeparator();
    tool_nest->addAction(action_nest_side_left);
    tool_nest->addAction(action_nest_side_right);
    tool_nest->addAction(action_nest_side_top);
    tool_nest->addAction(action_nest_side_bottom);
    tool_nest->addSeparator();
    tool_nest->addAction(action_nest_direction_horizontal);
    tool_nest->addAction(action_nest_direction_vertical);
// ![5] 排版工具栏

// ![6] 材料工具栏
    tool_sheet = new QToolBar(tr("材料"), this);
    tool_sheet->setOrientation(Qt::Horizontal);
    tool_sheet->setAllowedAreas(Qt::AllToolBarAreas);

    tool_sheet->addAction(action_sheet_add);
    tool_sheet->addAction(action_sheet_remove);
    tool_sheet->addAction(action_sheet_duplicate);
    tool_sheet->addAction(action_sheet_auto_duplicate);
    tool_sheet->addAction(action_sheet_previous);
    tool_sheet->addAction(action_sheet_next);
    tool_sheet->addAction(action_sheet_sheet_number);
    tool_sheet->addAction(action_sheet_use_last_sheet);
// ![6] 材料工具栏

    addToolBar(Qt::TopToolBarArea, tool_standard);
    addToolBar(Qt::TopToolBarArea, tool_view);
    addToolBar(Qt::TopToolBarArea, tool_nest);
    addToolBar(Qt::TopToolBarArea, tool_sheet);
}

void Nest::initStatusBar()
{
    if(ui->statusBar) delete ui->statusBar;
    statusBar();
    mousePositionLabel = new QLabel(tr(""), this);
    mousePositionLabel->setMargin(1);
    statusBar()->addPermanentWidget(mousePositionLabel);
}

void Nest::initDockWidget()
{
    // 设置dock窗口嵌套
    setDockNestingEnabled(true);

    // 新建dock窗口
    dock_project = new QDockWidget(tr("项目"), this);  // 添加project浮动窗口
    dock_piece = new QDockWidget(tr("切割件"), this);  // 添加pieces浮动窗口
    dock_sheet = new QDockWidget(tr("材料"), this);  // 添加sheets浮动窗口
    dock_nest = new QDockWidget(tr("排版区"), this);  // 添加排版区

    // 绘图区隐藏标题栏
    QWidget * qw=new QWidget(this);
    dock_nest->setTitleBarWidget(qw);

    // 绘图区不可移动、不可关闭、不可最小化
    dock_nest->setFeatures(QDockWidget::NoDockWidgetFeatures);
    // 绘图区的最小宽度
    dock_nest->setMinimumWidth(1000);
    // 将窗口添加至主窗口
    addDockWidget(Qt::LeftDockWidgetArea, dock_project);
    splitDockWidget(dock_project, dock_nest, Qt::Horizontal);
    splitDockWidget(dock_project, dock_piece, Qt::Vertical);
    splitDockWidget(dock_nest, dock_sheet, Qt::Vertical);
}

void Nest::initConfiguration()
{

}

void Nest::initNestView()
{
    //删除中央窗体
    QWidget *p = takeCentralWidget();
    if(p) {
        delete p;
        p = NULL;
    }

    nestView = new View(dock_nest);  // 初始化nest view
    dock_nest->setWidget(nestView);  // 将该视图加入到dock_nest
    connect(nestView, &View::mousePositionChanged, this, &Nest::onMousePositionChanged);

    // 初始化排版图层，无项目情况下的默认图层
    nestScene = new Scene(nestView);
    nestView->setScene(nestScene);
}

void Nest::updateNestView()
{
    // 如果活动项目为空，则返回
    if(!projectActive){
        QMessageBox::warning(this, tr("错误"), tr(""));
        return;
    }
    // 更新活动项目Map
    QString pName = projectActive->getName();
    if(proSceneListMap.contains(pName)){
        qDeleteAll(proSceneListMap[pName]);
        proSceneListMap[pName].clear();
    }

    // 按材料更新nest Scene的背景
    if(curSheet){
        Rect *rect = new Rect;
        SketchConfigure::PenStyle pen;
        pen.setPenStyle(Qt::black, Qt::SolidLine, 2);
        rect->setPenStyle(pen);
        rect->setRect(curSheet->layoutRect());
        nestScene->addCustomRectItem(rect);
    }
    nestView->setScene(nestScene);
    nestView->centerOn(nestView->mapFromScene(0,0));
}

void Nest::initProjectView()
{
    // 初始化project窗口
    tree_project = new QTreeWidget(dock_project);
    tree_project->setColumnCount(1); //设置列数
    tree_project->setHeaderLabel(tr("切割件列表")); //设置头的标题
    tree_project->setContextMenuPolicy(Qt::CustomContextMenu);//右键 不可少否则右键无反应
    connect(tree_project, &QTreeWidget::customContextMenuRequested, this, &Nest::showTreeMenu);
    connect(tree_project, &QTreeWidget::itemDoubleClicked, this, &Nest::onTreeProjectItemDoubleClicked);

    dock_project->setWidget(tree_project);
}

void Nest::initPieceView()
{
    widget = new QWidget(this);

    pieceView = new View(widget);  // 初始化piece view
    pieceView->setMouseFlag(false);  // 忽略鼠标事件
    pieceView->setWheelFlag(false);  // 忽略滚轮事件
    pieceView->setKeyboardFlag(false);  // 忽略键盘事件

    label = new QLabel(tr("设置切割件个数"), widget);
    lineEdit = new QLineEdit(tr("1"), widget);
    lineEdit->setValidator(new QIntValidator(1, 100, lineEdit));
    connect(lineEdit, &QLineEdit::textChanged, this, &Nest::onPieceNumChanged);

    lastButton = new QPushButton(tr("上一个"), widget);
    lastButton->setStatusTip(tr("上一个切割件"));

    nestButton = new QPushButton(tr("下一个"), widget);
    nestButton->setStatusTip(tr("下一个切割件"));

    QHBoxLayout *subLayout = new QHBoxLayout();
    subLayout->addWidget(lastButton);
    subLayout->addWidget(nestButton);
    widget->setDisabled(true);

    // 设置布局
    QGridLayout *layout = new QGridLayout(widget);
    layout->addWidget(pieceView, 0, 0, 10, 2);
    layout->addWidget(label, 10, 0, 1, 1);
    layout->addWidget(lineEdit, 10, 1, 1, 1);
    layout->addLayout(subLayout, 11, 0, 1, 2);
    widget->setLayout(layout);

    // 添加widget
    dock_piece->setWidget(widget);  // 将该视图加入到dock_pieces
}

void Nest::initConnect()
{
    connect(this, &Nest::nestProjectChange, this, &Nest::onNestProjectChanged);
}

void Nest::addProject()
{
    // 初始化项目名称
    QString name_project_new = getNewProjectName();
    projectActive = new Project(this);
    projectActive->setType(Project::Nest);
    projectActive->setName(name_project_new);
    // 在项目列表中加入该项目
    projectList.append(projectActive);

    // 更新piece视图
    pieceScene = new Scene(pieceView);
    pieceScene->setSceneRect(0, 0, dock_piece->width(), dock_piece->height());
    pieceView->setScene(pieceScene);
    pieceView->centerOn(pieceView->mapFromScene(0,0));

    // 初始化图层名称
    QString name_scene_new = tr("切割件列表-空");
    pieceScene->setName(name_scene_new);
    projectActive->addScene(pieceScene);
    projectActive->setActiveScene(pieceScene);

    QTreeWidgetItem *item_project = new QTreeWidgetItem(tree_project,QStringList(name_project_new));
    QTreeWidgetItem *item_scene = new QTreeWidgetItem(item_project,QStringList(name_scene_new)); //子节点1
    tree_project_item_list.append(item_project);

    connect(projectActive, &Project::projectNameChanged, this, &Nest::onProjectNameChanged);

    item_scene->setCheckState(0, Qt::Checked);
    item_project->addChild(item_scene); //添加子节点
    tree_project->expandAll(); //结点全部展开

    // 选择材料
    initSheet();

    // 发送排版项目改变信号
    emit nestProjectChange(projectActive);
}

void Nest::initSheet()
{
    if(!projectActive){  // 如果活动项目指针为空，返回
        QMessageBox::warning(this, tr("错误"), tr("未选择任何项目！"));
        return;
    }

    SheetDialog mDialog;
    mDialog.setDialogRole(SheetDialog::Nest);
    mDialog.exec();
    curSheet = mDialog.getSheetActive();

    if(!curSheet){  // 如果选择的材料为空，则
        QMessageBox::warning(this, tr("错误"), tr("未选择材料！"));
        return;
    }

    QString pName = projectActive->getName();
    // 添加proSheetInfoMap
    if(proSheetInfoMap.contains(pName)){
        ProSheetInfo *map = proSheetInfoMap[pName];
        map->sheetList.append(curSheet);
        map->usageList.append(0.0);
        map->pieceNumList.append(0);
        map->curSheetID = map->sheetList.length() - 1;  // 当前材料为最后一张
    } else{
        ProSheetInfo *map = new ProSheetInfo(pName);
        map->sheetList.append(curSheet);
        map->usageList.append(0.0);
        map->pieceNumList.append(0);
        map->curSheetID = 0;    // 当前材料为第一张
        proSheetInfoMap.insert(pName, map);
    }

    Scene *scene = new Scene;
    Line *line = new Line();
    line->setLine(0, 0 , 100, 100);
    scene->addCustomLineItem(line);
    // 将该图层加入proSceneListMap
    if(proSceneListMap.contains(pName)){
        proSceneListMap[pName].append(scene);
    } else{
        QList<Scene *> sList;
        sList.append(scene);
        proSceneListMap.insert(pName, sList);
    }
}

void Nest::updateSheetTree()
{
    // 首先，获取当前项目的材料信息
    QString pName = projectActive->getName();
    if(!proSheetInfoMap.contains(pName)){
        qDebug() << "当前项目的材料信息为空";
        return;
    }

    QList<Sheet *> sheetList = proSheetInfoMap[pName]->sheetList;
    QList<qreal> pieceNumList = proSheetInfoMap[pName]->pieceNumList;
    QList<qreal> usageList = proSheetInfoMap[pName]->usageList;
    int sheetNum = sheetList.length();
    qreal rateTotal = 0.0;

    // 更新sheet窗口
    if(!tree_sheet){
        tree_sheet = new QTreeWidget(dock_project);
    } else{
        tree_sheet->clear();
    }
    tree_sheet->setColumnCount(1); //设置列数

    for(int i=0;i<sheetNum;i++){
        QString sheetName = sheetList[i]->name;
        qreal width = sheetList[i]->width;
        qreal height = sheetList[i]->height;
        int pieceNum = pieceNumList[i];
        qreal rate = usageList[i];
        rateTotal += rate;

        QStringList sList1;
        sList1 << tr("材料") + QString("%1").arg(i+1) + tr("：") + sheetName;
        QStringList sList2;
        sList2 << tr("尺寸：") + QString("%1").arg(width) + " X " + QString("%1").arg(height);
        QStringList sList3;
        sList3 << tr("切割件：") + QString("%1").arg(pieceNum);
        QStringList sList4;
        sList4 << tr("产能（利用率）：") + QString("%1").arg(rate) + tr("%");

        QTreeWidgetItem *item_sheet = new QTreeWidgetItem(tree_sheet, sList1);
        QTreeWidgetItem *item_sheet_size = new QTreeWidgetItem(item_sheet, sList2); //子节点1
        QTreeWidgetItem *item_sheet_piece = new QTreeWidgetItem(item_sheet, sList3); //子节点2
        QTreeWidgetItem *item_sheet_rate = new QTreeWidgetItem(item_sheet, sList4); //子节点3

        item_sheet->addChild(item_sheet_size);
        item_sheet->addChild(item_sheet_piece);
        item_sheet->addChild(item_sheet_rate);
    }

    const char *ch1 = "排版信息（共";
    const char *ch2 = "张板材料，总产能（利用率）：";
    const char *ch3 = "%）";
    char *buf = new char[strlen(ch1) + strlen(ch2) + strlen(ch3) + sizeof(sheetNum) + sizeof(rateTotal)];
    sprintf(buf, "%s%d%s%.2f%s", ch1, sheetNum, ch2, rateTotal, ch3);
    tree_sheet->setHeaderLabel(tr(buf)); //设置头的标题

    dock_sheet->setWidget(tree_sheet);
}

void Nest::initRectNestEngine()
{
    double len = RectNestEngine::components.length();
    if(len <= 0){
        QMessageBox::warning(this, tr("错误"), tr("请添加切割件！"));
        return;
    }

    // 将零件列表转化为最小矩形列表
    int index = 0;
    for(int i=0; i<len; i++){
        Component com = RectNestEngine::components[i];
        for(int j=0; j<com.count; j++){
            // 更新矩形件总面积和最小面积
            long area = com.rect.width() * com.rect.height();
            RectNestEngine::allRectsArea += area;
            if(area < RectNestEngine::minRectsArea){
                RectNestEngine::minRectsArea = area;
            }
            RectNestEngine::MinRect minRect(com.rect.width(),
                                            com.rect.height(),
                                            index++,
                                            false,
                                            i);
            RectNestEngine::compMinRects.append(minRect);
        }
    }

    // 矩形列表的长度即为基因数
    int totalNum = RectNestEngine::compMinRects.length();

    // 初始化空白矩形集合
    QRectF r = curSheet->layoutRect();
    RectNestEngine::mWidth = r.width();
    RectNestEngine::mHeight = r.height();

    // 使用遗传算法进行求解最优解
    GA g(COUNT, totalNum, 3, 20, 0.1, RectNestEngine::fitnessFunc, 0.97, 1, 0.3);
    g.initPopulation();
    int n = 50;
    while(n-- > 0 && !g.isStop()){
        // 增加进度条
        g.getNewGeneration();
    }
    // 进化结束后，在新的种群中选择最优个体
    g.evaluateFitness();

    // 重置所有参数
    RectNestEngine::resetAllParameter();

    // 排版算法
    RectNestEngine::layoutAlg(g.getFittestGenome().getGenome());

    // 保存该项目的原始图形，即 offset
    QList<PieceOffset> pieceOffsetList;
    for(int i=0; i<RectNestEngine::components.length(); i++){
        Polyline *polyline = RectNestEngine::components[i].polyline;
        // 将该图形移动至原点位置
        QPointF currentCenter = polyline->boundingRect().center();
        QPointF originCenter = QPointF(polyline->boundingRect().width()/2,
                                       polyline->boundingRect().height()/2);

        QPointF offset = currentCenter - originCenter;
        QList<PiecePointsList> pList;
        for(int j=0;j<polyline->getPoints().length();j++){
            QPointF oldPoint = polyline->getPoints()[j];
            QPointF newPoint = oldPoint-offset;
            pList.append(PiecePointsList(newPoint.rx(),
                                                    newPoint.ry(),
                                                    RESERVE_DOUBLE));
        }

        PieceOffset pieceOffset(i, RESERVE_DOUBLE,
                                           RESERVE_DOUBLE, RESERVE_DOUBLE,
                                           RESERVE_STRING, RESERVE_DOUBLE,
                                           RESERVE_INT, pList.length(), pList);
        pieceOffsetList.append(pieceOffset);
    }

    QString pName = projectActive->getName();
    if(!pieceOffsetMap.contains(pName)){
        pieceOffsetMap.insert(pName, pieceOffsetList);
    } else{
        pieceOffsetMap[pName] = pieceOffsetList;
    }

    int unLay = 0;
    QList<PieceCenter> pieceCenterList;
    for(int i=0;i<g.getFittestGenome().getGenome().length();i++){
        int index = qAbs(g.getFittestGenome().getGenome()[i]) - 1;  // 得到矩形的序号
        RectNestEngine::MinRect *currentRect = &RectNestEngine::compMinRects[index]; // 得到矩形指针
        if(!currentRect->layFlag){
            unLay++;
            continue;
        }
        int page = currentRect->page;
        bool rotate = g.getFittestGenome().getGenome()[i] < 0; // 如果基因为负值，则需要旋转90*
        currentRect->setRotate(rotate);
        int id = currentRect->getComponentId();
        QPointF cp = currentRect->position;
        qDebug() << "页数：" << page << "  位置: "<< cp;

        // 获取该最小矩形对应的原图形
        Polyline *polyline = RectNestEngine::components[id].polyline;
        // 将该图形移动至原点位置
        QPointF currentCenter = polyline->boundingRect().center();

        QPointF originCenter;
        if(rotate){
           originCenter = QPointF(polyline->boundingRect().height()/2,
                                           polyline->boundingRect().width()/2);
        } else{
            originCenter = QPointF(polyline->boundingRect().width()/2,
                                           polyline->boundingRect().height()/2);
        }
        QPointF offset = currentCenter - originCenter;

        // 构造新图形
        QVector<QPointF> pList;
        for(int j=0;j<polyline->getPoints().length();j++){
            QPointF oldPoint = polyline->getPoints()[j];
            QPointF newPoint = oldPoint-offset+cp;
            pList.append(newPoint);
        }
        Polyline *p = new Polyline();
        SketchConfigure::PenStyle pen;
        pen.setPenStyle(Qt::blue, Qt::SolidLine, 1);
        p->setPenStyle(pen);
        p->setPolyline(pList, polyline->getType(), polyline->getElevation(), rotate ? 90.000 : 0.000);
        nestScene->addCustomPolylineItem(p);

        PieceCenter pieceCenter(i, page, id, rotate ? 90.000 : 0.000, RESERVE_INT,
                                           cp.rx() + polyline->boundingRect().width(),
                                           cp.ry() + polyline->boundingRect().height());
        pieceCenterList.append(pieceCenter);
    }

    if(!pieceCenterMap.contains(pName)){
        pieceCenterMap.insert(pName, pieceCenterList);
    } else{
        pieceCenterMap[pName] = pieceCenterList;
    }

    if(unLay != 0){
        qDebug() << "有" << unLay << "个未排放";
    }
    // 更新场景和视图
    nestView->setScene(nestScene); // nestView->setScene(proSceneListMap[name].);
    nestScene->setMoveable(true);
    QString name = projectActive->getName();
    if(proSceneListMap.contains(name)){
        proSceneListMap[name].append(nestScene);
    }

    if(proSheetInfoMap.contains(name)){
        proSheetInfoMap[name]->usageList[0] = g.getFittestGenome().getFitness() * 100.0;
        proSheetInfoMap[name]->pieceNumList[0] = totalNum;
        updateSheetTree();
    }
    qDebug() << "材料使用率： " << g.getFittestGenome().getFitness();
}

void Nest::showNestResult()
{
}

QString Nest::getNewProjectName()
{
    // 获取项目数目
    int len_project = projectList.length() + 1;
    QString str = QString::number(len_project, 10);
    char *ch;
    QByteArray ba = str.toLatin1();
    ch = ba.data();
    return tr("新项目") + tr(ch);
}

Project *Nest::getProjectByName(QString project_name)
{
    for(int i=0;i<projectList.length();i++){
        if(projectList.at(i)->getName() == project_name){
            return projectList.at(i);
        }
    }
    return NULL;
}

void Nest::removeProjectByName(QString project_name)
{
    QMutableListIterator<Project *> i(projectList);
    while (i.hasNext()) {
        if(i.next()->getName() == project_name){
            Project *pro = i.value();
            i.remove();
            delete pro;
            pro = NULL;
        }
    }
}

void Nest::showTreeMenu(QPoint pos)
{
    tree_project_scene_active_item = tree_project->itemAt(pos); //可得到右键条目
    if(tree_project_scene_active_item == NULL){  //这种情况是右键的位置不在treeItem的范围内,即在空白位置右击,则显示
        menu_tree = new QMenu(tree_project);
        action_tree_expand_all = new QAction(tr("展开全部"), tree_project);
        action_tree_fold_all = new QAction(tr("折叠全部"), tree_project);
        connect(action_tree_expand_all, &QAction::triggered, this, &Nest::onActionTreeExpandAll);
        connect(action_tree_fold_all, &QAction::triggered, this, &Nest::onActionTreeFoldAll);
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
        action_tree_project_nest_scene = new QAction(tr("排版"), tree_project);
        action_tree_project_add_scene = new QAction(tr("添加切割件"), tree_project);
        action_tree_project_save = new QAction(tr("保存"), tree_project);
        action_tree_project_save_as = new QAction(tr("另存为"), tree_project);
        action_tree_project_rename = new QAction(tr("重命名"), tree_project);
        action_tree_project_close = new QAction(tr("关闭"), tree_project);

        connect(action_tree_project_nest_scene, &QAction::triggered, this, &Nest::onActionTreeProjectNestScene);
        connect(action_tree_project_add_scene, &QAction::triggered, this, &Nest::onActionTreeProjectAddScene);
        connect(action_tree_project_save, &QAction::triggered, this, &Nest::onActionTreeProjectSave);
        connect(action_tree_project_save_as, &QAction::triggered, this, &Nest::onActionTreeProjectSaveAs);
        connect(action_tree_project_rename, &QAction::triggered, this, &Nest::onActionTreeProjectRename);
        connect(action_tree_project_close, &QAction::triggered, this, &Nest::onActionTreeProjectClose);

        menu_tree_project->addAction(action_tree_project_nest_scene);
        menu_tree_project->addAction(action_tree_project_add_scene);
        menu_tree_project->addAction(action_tree_project_save);
        menu_tree_project->addAction(action_tree_project_save_as);
        menu_tree_project->addAction(action_tree_project_rename);
        menu_tree_project->addAction(action_tree_project_close);
        menu_tree_project->exec(QCursor::pos());  //在当前鼠标位置显示
    } else{  // 图层栏
        menu_tree_project_scene = new QMenu(tree_project);
        action_tree_project_scene_change_to = new QAction(tr("切换至该切割件"), tree_project);

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

        connect(action_tree_project_scene_change_to, &QAction::triggered, this, &Nest::onActionTreeProjectSceneChangeTo);
        connect(action_tree_project_scene_move_up_one, &QAction::triggered, this, &Nest::onActionTreeProjectSceneMoveUpOne);
        connect(action_tree_project_scene_move_up_top, &QAction::triggered, this, &Nest::onActionTreeProjectSceneMoveUpTop);
        connect(action_tree_project_scene_move_down_one, &QAction::triggered, this, &Nest::onActionTreeProjectSceneMoveDownOne);
        connect(action_tree_project_scene_move_down_bottom, &QAction::triggered, this, &Nest::onActionTreeProjectSceneMoveDownBottom);
        connect(action_tree_project_scene_rename, &QAction::triggered, this, &Nest::onActionTreeProjectSceneRename);
        connect(action_tree_project_scene_delete, &QAction::triggered, this, &Nest::onActionTreeProjectSceneDelete);

        menu_tree_project_scene->addAction(action_tree_project_scene_change_to);
        menu_tree_project_scene->addMenu(menu_tree_project_scene_move_up);
        menu_tree_project_scene->addMenu(menu_tree_project_scene_move_down);
        menu_tree_project_scene->addAction(action_tree_project_scene_rename);
        menu_tree_project_scene->addAction(action_tree_project_scene_delete);
        menu_tree_project_scene->exec(QCursor::pos());  //在当前鼠标位置显示
    }
}

void Nest::updateAll()
{
    // 如果活动项目指针为空，
    if(!projectActive){
        setWindowTitle("CADPRO-<空项目>");
        dock_piece->setWindowTitle("<空项目>");
        widget->setDisabled(true);
        lineEdit->setText("1");

        tree_project_active_item = NULL;
        tree_project_scene_active_item = NULL;
        pieceView->setScene(NULL);

        updateNestView();
        return;
    }

    // 更新piece图层
    pieceScene = projectActive->getActiveScene();
    QString pName = projectActive->getName();
    QString sName = pieceScene->getName();

    // 更新piece视图
    widget->setDisabled(false);
    QString name = pName + "_" + sName;
    if(nestNum.contains(name)){
        lineEdit->setText(QString::number(nestNum[name], 10));
    } else{
        lineEdit->setText("1");
    }
    pieceView->setScene(pieceScene);

    // 更新nest图层
    if(proSceneListMap.contains(pName)){
        if(proSceneListMap[pName].length() != 0){
            nestScene = proSceneListMap[pName][0];
            nestView->setScene(nestScene);
        } else{
            updateNestView();
        }
    } else{
        updateNestView();
    }

    // 更新材料部分
    updateSheetTree();

    // 更新窗口名称
    setWindowTitle("CADPRO-<" + pName + ">");
    dock_piece->setWindowTitle("<" + pName + "-" + sName + ">");
    dock_sheet->setWindowTitle("<" + pName + ">");
}

bool Nest::maybeSave()
{
    while(projectList.length() != 0){
        projectActive = projectList.front();
        if(projectActive->isModified()){
            QMessageBox::StandardButton box;
            box = QMessageBox::warning(this,tr("保存项目"),tr("您要存储对“") + projectActive->getName() + tr("”所做的更改吗？"),
            QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
            if(box == QMessageBox::Yes) {
                if(onActionFileSave()) {
                    projectList.pop_front();
                }
                return maybeSave();
            } else if(box == QMessageBox::No) {
                projectList.pop_front();
            } else if(box == QMessageBox::Cancel) {
                return false;
            }
        }else{
            projectList.pop_front();
        }
    }
    return true;
}

bool Nest::saveFile(QString fileName)
{
    if(fileName.isEmpty()){
        return false;
    }

    try{
        QString pName = projectActive->getName();
        if(!proSceneListMap.contains(pName)
                || !proSheetInfoMap.contains(pName)
                || !pieceCenterMap.contains(pName)
                || !pieceOffsetMap.contains(pName)){
            QMessageBox::warning(this, tr("错误"), tr("该项目还未进行排版!"));
            return false;
        }
        Project *pro = new Project(this);
        QList<Scene *> sList = proSceneListMap[pName];
        for(int i=0; i<sList.length(); i++){
            pro->addScene(sList[i]);
        }

        int centerCount = 0;
        for(int i=0; i < proSheetInfoMap[pName]->pieceNumList.length(); i++){
            centerCount += proSheetInfoMap[pName]->pieceNumList[i];
        }

        pro->tnfFileWriter(fileName,
                           proSheetInfoMap[pName]->sheetList,
                           pieceCenterMap[pName],
                           pieceOffsetMap[pName]);
        delete pro;
        pro = NULL;
    } catch(QString exception){
        QMessageBox::warning(this, tr("错误"), exception);
        return false;
    }

    return true;
}

void Nest::setNestActionDisabled(bool flag)
{
    action_nest_side_left->setDisabled(flag);
    action_nest_side_right->setDisabled(flag);
    action_nest_side_top->setDisabled(flag);
    action_nest_side_left->setDisabled(flag);
    action_nest_side_bottom->setDisabled(flag);
    action_nest_direction_horizontal->setDisabled(flag);
    action_nest_direction_vertical->setDisabled(flag);
    action_sheet_add->setDisabled(flag);
    action_sheet_remove->setDisabled(flag);
    action_sheet_duplicate->setDisabled(flag);
    action_sheet_auto_duplicate->setDisabled(flag);
    action_sheet_previous->setDisabled(flag);
    action_sheet_next->setDisabled(flag);
    action_sheet_sheet_number->setDisabled(flag);
    action_sheet_use_last_sheet->setDisabled(flag);
    action_sheet_sheet_property->setDisabled(flag);
}

void Nest::onProjectNameChanged(QString lastName, QString presentName)
{
    for(int i=0; i<tree_project_item_list.length(); i++){
        if(tree_project_item_list.at(i)->text(0) == lastName){
            // 改变项目名称
            tree_project_item_list.at(i)->setText(0, presentName);
            // 改变项目对应的材料信息
            if(proSheetInfoMap.contains(lastName)){
                proSheetInfoMap.insert(presentName, proSheetInfoMap[lastName]);
                proSheetInfoMap.remove(lastName);
            }

            // 改变项目对应的图层
            if(proSceneListMap.contains(lastName)){
                proSceneListMap.insert(presentName, proSceneListMap[lastName]);
                proSceneListMap.remove(lastName);
            }
            break;
        }
    }
    // 改变树枝结构的名称
    tree_project_active_item->setText(0, presentName);
}

void Nest::onMousePositionChanged(QPointF pos)
{
    mousePositionLabel->setText(tr("X=") + QString::number(pos.rx()) + " Y=" + QString::number(-pos.ry()));
}

void Nest::onNestProjectChanged(Project *curProject)
{
    // 首先判断当前项目是否为空
    if(!curProject){
        setNestActionDisabled(true);
        return;
    }
    qDebug() << "当前项目为：" << curProject->getName();
    // 使能排版相关的action
    setNestActionDisabled(false);
    // 更新全部组件
    updateAll();
}

void Nest::closeEvent(QCloseEvent *event)
{
    if(maybeSave()) {
        qApp->quit();
    }
    else {
        event->ignore();
    }
}

void Nest::onActionFileNew()
{
    qDebug() << "新建一个项目，即导入一个dxf文件"
                "，对其中的对象进行扫描加入到对象列表中来";
    addProject();
}

void Nest::onActionFileOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        qDebug() << "open file";
    }
}

bool Nest::onActionFileSave()
{
    // 如果项目保存过，不再弹出保存框
    if(projectActive->isSaved()) {
        // 如果项目被修改过，直接保存修改后的项目
        if(projectActive->isModified()){
            return saveFile(projectActive->getName());
        }
    } else{
        return onActionFileSaveAs();
    }
    return true;
}

bool Nest::onActionFileSaveAs()
{
    qDebug() << "save dxf files";
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("导出DXF文件"),
                                                    tr("export.tnf"));
    if(fileName.isEmpty()){
        return false;
    }
    if(fileName != projectActive->getName()){
        //projectActive->changeName(fileName);
    }
    // 保存逻辑
    projectActive->saveProject();
    bool res = saveFile(fileName);
    projectActive->setSaved(res);
    if(res){
        QMessageBox::information(this, tr("通知"), projectActive->getName() + tr("保存成功"));
    }
    return res;
}

bool Nest::onActionFileSaveAll()
{
    return true;
}

void Nest::onActionFilePrint()
{

}

void Nest::onActionFilePrintPreview()
{

}

void Nest::onActionFilePrintSetup()
{

}

void Nest::onActionFileConfiguration()
{

}

void Nest::onActionFileExit()
{
    if(maybeSave()) {
        qApp->quit();
    }
}

void Nest::onActionEditUndo()
{
    qDebug() << "撤销上个操作";
}

void Nest::onActionEditRedo()
{
    qDebug() << "重做撤销操作";
}

void Nest::onActionEditClear()
{
    qDebug() << "清空";
}

void Nest::onActionEditDelete()
{
    qDebug() << "删除";
}

void Nest::onActionEditCut()
{
    qDebug() << "剪切";
}

void Nest::onActionEditCopy()
{
    qDebug() << "复制";
}

void Nest::onActionEditPaste()
{
    qDebug() << "粘贴";
}

void Nest::onActionNestConfig()
{
    qDebug() << "自动排版配置";
}

void Nest::onActionNestSideLeft()
{
    qDebug() << "左靠边";
}

void Nest::onActionNestSideRight()
{
    qDebug() << "右靠边";
}

void Nest::onActionNestSideTop()
{
    qDebug() << "顶靠边";
}

void Nest::onActionNestSideBottom()
{
    qDebug() << "底靠边";
}

void Nest::onActionNestDirectionHorizontal()
{
    qDebug() << "横向";
}

void Nest::onActionNestSideDirectionVertical()
{
    qDebug() << "纵向";
}

void Nest::onActionSheetManager()
{
    qDebug() << "创建或编辑材料";
    SheetDialog mDialog;
    mDialog.exec();
}

void Nest::onActionSheetAdd()
{
    qDebug() << "增加新的材料";
    // Step1: 获取当前项目
    // Step2: 获取排版引擎
    // Step3: 选择材料
    // Step4: 将选中的材料加入排版引擎列表
    initSheet();  // 添加材料
    updateSheetTree();  // 更新材料树
}

void Nest::onActionSheetRemove()
{
    qDebug() << "删除当前材料";
    if(!projectActive){  // 如果活动项目指针为空，返回
        QMessageBox::warning(this, tr("错误"), tr("未选择任何项目！"));
        return;
    }
    QString pName = projectActive->getName();  // 获取活动项目名称
    if(!proSheetInfoMap.contains(pName) || !proSceneListMap.contains(pName)){  // 如果项目-材料Map中不存在该名称的项目，返回
        QMessageBox::warning(this, tr("错误"), tr("不存在该名称的项目！"));
        return;
    }
    ProSheetInfo *map = proSheetInfoMap[pName];  // 获取该项目的项目-材料指针
    if(map->sheetList.length() < 1){  // 如果该项目的材料列表为空，返回
        QMessageBox::warning(this, tr("错误"), tr("材料列表已空！"));
        return;
    }
    int id = map->curSheetID;  // 获取当前材料id
    Sheet *s = map->sheetList[id];   // 在列表删除该材料
    map->sheetList.removeAt(id);
    delete s;  // 释放内存，防止内存泄漏
    s = NULL;
    if(id > map->sheetList.length() - 1){  // 如果id大于列表长度，则为最后一张
        map->curSheetID = map->sheetList.length() - 1;  // 更新当前材料id
    }
    qDebug() << "删除之后：" << map->curSheetID;
    updateSheetTree();  // 更新材料列表
}

void Nest::onActionSheetDuplicate()
{
    qDebug() << "重复当前材料";
    if(!projectActive){  // 如果活动项目指针为空，返回
        QMessageBox::warning(this, tr("错误"), tr("未选择任何项目！"));
        return;
    }
    QString pName = projectActive->getName();  // 获取活动项目名称
    if(!proSheetInfoMap.contains(pName) || !proSceneListMap.contains(pName)){  // 如果项目-材料Map中不存在该名称的项目，返回
        QMessageBox::warning(this, tr("错误"), tr("不存在该名称的项目！"));
        return;
    }
    ProSheetInfo *map = proSheetInfoMap[pName];  // 获取该项目的项目-材料指针
    if(map->sheetList.length() < 1){  // 如果材料列表为空，返回
        QMessageBox::warning(this, tr("错误"), tr("材料列表为空"));
        return;
    }
    int id = map->curSheetID;  // 获取当前材料id
    Sheet *sheet = map->sheetList[id]->sheet();  // 获取当前材料指针
    map->sheetList.append(sheet);  // 将该材料指针加入材料列表
    map->pieceNumList.append(0);
    map->usageList.append(0.0);
    map->curSheetID = map->curSheetID + 1;

    // 添加图层
    Scene *scene = new Scene;
    Line *line = new Line;
    line->setLine(0, 0 , 100, 100);
    scene->addCustomLineItem(line);
    // 将该图层加入proSceneListMap
    if(proSceneListMap.contains(pName)){
        proSceneListMap[pName].append(scene);
    } else{
        QList<Scene *> sList;
        sList.append(scene);
        proSceneListMap.insert(pName, sList);
    }
    updateSheetTree();  // 更新材料列表
}

void Nest::onActionSheetAutoDuplicate()
{
    qDebug() << "自动重复当前材料";
}

void Nest::onActionSheetPrevious()
{
    qDebug() << "转到上一张材料";
    // 如果活动项目为空 或 该项目未选择材料，则返回
    if(!projectActive){
        QMessageBox::warning(this, tr("错误"), tr("未选择任何项目！"));
        return;
    }

    QString pName = projectActive->getName();
    if(!proSheetInfoMap.contains(pName) || !proSceneListMap.contains(pName)){
        QMessageBox::warning(this, tr("错误"), tr("不存在该名称的项目！"));
        return;
    }
    ProSheetInfo *map = proSheetInfoMap[pName];
    if(map->sheetList.length() < 1){  // 如果材料列表为空，返回
        QMessageBox::warning(this, tr("错误"), tr("材料列表为空"));
        return;
    }
    int id = map->curSheetID;  // 获取当前sheetID
    if(id == 0){
        QMessageBox::warning(this, tr("通知"), tr("目前已经是第一张材料"));
        return;
    }
    nestScene = proSceneListMap[pName][id-1];
    map->curSheetID = id - 1;

    // 按材料更新nest Scene的背景
    nestView->setScene(nestScene);
    nestView->centerOn(nestView->mapFromScene(0,0));
}

void Nest::onActionSheetNext()
{
    qDebug() << "转到下一张材料";
    // 如果活动项目为空 或 该项目未选择材料，则返回
    if(!projectActive){
        QMessageBox::warning(this, tr("错误"), tr("未选择任何项目！"));
        return;
    }

    QString pName = projectActive->getName();
    if(!proSheetInfoMap.contains(pName) || !proSceneListMap.contains(pName)){
        QMessageBox::warning(this, tr("错误"), tr("不存在该名称的项目！"));
        return;
    }
    ProSheetInfo *map = proSheetInfoMap[pName];
    if(map->sheetList.length() < 1){  // 如果材料列表为空，返回
        QMessageBox::warning(this, tr("错误"), tr("材料列表为空"));
        return;
    }
    int id = map->curSheetID;  // 获取当前sheetID
    if(id == map->sheetList.length()-1){
        QMessageBox::warning(this, tr("通知"), tr("目前已经是最后一张材料"));
        return;
    }
    nestScene = proSceneListMap[pName][id+1];
    map->curSheetID = id + 1;

    // 按材料更新nest Scene的背景
    nestView->setScene(nestScene);
    nestView->centerOn(nestView->mapFromScene(0,0));
}

void Nest::onActionSheetSheetNumber()
{
    qDebug() << "跳转至排版中的材料序号";
    QString pName = projectActive->getName();
    if(!proSheetInfoMap.contains(pName) || !proSceneListMap.contains(pName)){
        QMessageBox::warning(this, tr("错误"), tr("不存在该名称的项目！"));
        return;
    }
    ProSheetInfo *map = proSheetInfoMap[pName];
    if(map->sheetList.length() < 1){  // 如果材料列表为空，返回
        QMessageBox::warning(this, tr("错误"), tr("材料列表为空"));
        return;
    }
    int maxId = map->sheetList.length();  // 获取当前最大的ID
    bool ok;
    int id = QInputDialog::getInt(this, tr("跳转至"),
                                  tr("请输入材料序号:"), 1, 1, maxId, 1,
                                  &ok);
    if(ok){
        nestScene = proSceneListMap[pName][id-1];
        map->curSheetID = id - 1;

        // 按材料更新nest Scene的背景
        nestView->setScene(nestScene);
        nestView->centerOn(nestView->mapFromScene(0,0));
    }
}

void Nest::onActionSheetUseLastSheet()
{
    // Step1：获取当前项目的排版情况
    qDebug() << "重复使用最后一张已切割过的材料";
    if(!projectActive){ // 如果活动项目为空，则返回
        QMessageBox::warning(this, tr("错误"), tr("未选择任何项目！"));
        return;
    }

    QString pName = projectActive->getName();
    if(!proSheetInfoMap.contains(pName) || !proSceneListMap.contains(pName)){
        QMessageBox::warning(this, tr("错误"), tr("不存在该名称的项目！"));
        return;
    }
    ProSheetInfo *map = proSheetInfoMap[pName];
    if(map->sheetList.length() < 1){  // 如果材料列表为空，返回
        QMessageBox::warning(this, tr("错误"), tr("材料列表为空"));
        return;
    }

    // to do
}

void Nest::onActionSheetProperty()
{
    qDebug() << "更改材料&余量/边距";
}

void Nest::onActionViewGrid(bool toggled)
{
    // 显示网格
    for(int i=0; i<projectList.length();i++){
        QString pName = projectList[i]->getName();
        if(proSceneListMap.contains(pName)){
            QList<Scene*> sceneList = proSceneListMap[pName];
            for(int j=0; j<sceneList.length();j++){
                sceneList[j]->onGridChanged(toggled);
            }
        } else{
            if(nestScene){
                nestScene->onGridChanged(toggled);
            }
        }
    }
}

void Nest::onActionViewZoomWindow()
{
    qDebug() << "缩放窗口";
}

void Nest::onActionViewZoomAll()
{
    qDebug() << "全部缩放";
}

void Nest::onActionViewZoomIn()
{
    nestView->zoomIn();
}

void Nest::onActionViewZoomOut()
{
    nestView->zoomOut();
}

void Nest::onActionViewZoomBack()
{
    nestView->zoomBack();
}

void Nest::onActionViewLockLayout(bool toggled)
{
    qDebug() << "锁定布局" << toggled;
}

void Nest::onActionViewToolProjectsToggled(bool toggled)
{
    qDebug() << "项目视图" << toggled;
}

void Nest::onActionViewToolPiecesToggled(bool toggled)
{
    qDebug() << "零件视图" << toggled;
}

void Nest::onActionViewToolSheetsToggled(bool toggled)
{
    qDebug() << "材料视图" << toggled;
}

void Nest::onActionViewToolSlideToggled(bool toggled)
{
    qDebug() << "slide about to " << toggled;
}

void Nest::onActionViewToolCustomizeToggled(bool toggled)
{
    qDebug() << "customize about to " << toggled;
}

void Nest::onActionViewStatusBar(bool toggled)
{
    qDebug() << "status_bar about to " << toggled;
}

void Nest::onActionUtilityMeasureDis()
{
    qDebug() << "测量距离" ;
    if(nestView){
        nestView = new View(dock_nest);
    }
    if(nestScene == NULL){
        nestScene = new Scene(nestView);
    }
    nestScene->setCurShape(Shape::MiddleAxis);
}

void Nest::onActionHelpHelp()
{
    qDebug() << "帮助";
    QMessageBox::about(this, tr("帮助"),
        tr("暂不支持帮助！"));
}

void Nest::onActionHelpLicense()
{
    qDebug() << "许可";
    QMessageBox::about(this, tr("许可"),
        tr("许可证书：CADPRO1234567890"));
}

void Nest::onActionHelpAbout()
{
    QMessageBox::about(this, tr("关于CADPRO"),
        tr("这是一款排版软件测试版<br/>作者：Jeremy<br/>开发时间：2018年4月"));
}

void Nest::onActionTreeExpandAll()
{
    tree_project->expandAll();
}

void Nest::onActionTreeFoldAll()
{
    tree_project->collapseAll();
}

void Nest::onTreeProjectItemDoubleClicked(QTreeWidgetItem *item)
{
    QTreeWidgetItem *parent = item->parent();//获得父节点
    if(NULL == parent) {
        tree_project_active_item = item;
        // 根据项目名称获取项目指针
        QString project_name = item->text(0);
        projectActive = getProjectByName(project_name);
        pieceScene = projectActive->getScene(0);
    } else{
        tree_project_active_item = parent;
        tree_project_scene_active_item = item;
        QString project_name = parent->text(0);
        projectActive = getProjectByName(project_name);
        pieceScene = projectActive->getSceneByName(item->text(0));
        projectActive->setActiveScene(pieceScene);
    }
    // 发送排版项目改变信号
    emit nestProjectChange(projectActive);
}

void Nest::onActionTreeProjectNestScene()
{
    if(!curSheet){
        QMessageBox::warning(this, tr("警告"), tr("未设置切割件所用材料!"));
        return;
    }
    if(nestNum.count() == 0){
        QMessageBox::warning(this, tr("警告"), tr("未设置切割件排版个数!"));
        return;
    }
    RectNestEngine::components.clear();
    RectNestEngine::compMinRects.clear();
    updateNestView();  // 更新排版视图
    for(int i=0;i<projectActive->getSceneList().length();i++){
        Scene *s = projectActive->getSceneList().at(i);
        int count = 0;
        QString name = projectActive->getName() + "_" + s->getName();
        if(!nestNum.contains(name)){
            /*
            bool ok;
            int res = QMessageBox::question(this, tr("警告"), tr("未设置该切割件排版个数现在设置？"), tr("取消"), tr("设置"));
            if(res == 0){
                return;
            }
            int number = QInputDialog::getInt(this, tr("设置该切割件个数"),
                                                 tr("设置切割件个数:"), 1,
                                                 0, 100, 1, &ok);
            if (ok && number<0){
                nestNum.insert(name, number);
            }
            */
            int number = 1;
            nestNum[name] = number;
            count = number;
        } else{
            count = nestNum[name];
        }
        qDebug() << "count= " << s->getPolylineList().length();
        for(int j=0; j<s->getPolylineList().length(); j++){
            Polyline *p = s->getPolylineList()[j];
            RectNestEngine::components.append(Component(p, count));
        }
    }
    initRectNestEngine(); // 初始化矩形排版引擎
}

void Nest::onActionTreeProjectAddScene()
{
    qDebug() << "import dxf files";
    QString fileName;// = QFileDialog::getOpenFileName(this, tr("打开DXF文件"), QDir::currentPath());
    fileName = "/Users/Jeremy/Qt5.10.0/Projects/build-CADPRO-Desktop_Qt_5_10_0_clang_64bit-Debug/CADPRO.app/Contents/MacOS/toNest.dxf";
    if (!fileName.isEmpty()) {
        if(!projectActive){
            QString name_project_new = getNewProjectName();
            projectActive = new Project(this);
            projectActive->setType(Project::Nest);
            projectActive->setName(name_project_new);
            projectList.append(projectActive);

            tree_project_active_item = new QTreeWidgetItem(tree_project, QStringList(name_project_new));
            tree_project_item_list.append(tree_project_active_item);
        }

        // 将空列表去掉
        if(projectActive->getScene(0) && projectActive->getScene(0)->getName() == tr("切割件列表-空")){
            projectActive->getSceneList().removeFirst();
            tree_project_active_item->removeChild(tree_project_active_item->child(0));
        }

        // 读取dxf文件
        try{
            projectActive->dxfFileReader(fileName);
        } catch (QString exception){
            QMessageBox::warning(this, tr("错误"), exception);
        }

        QList<Scene*> sList = projectActive->getSceneList();
        int count = sList.length();

        for(int i=0;i<count;i++){
            QString name_scene_new = sList[i]->getName();
            QTreeWidgetItem *item_scene = new QTreeWidgetItem(tree_project_active_item, QStringList(name_scene_new)); //子节点1
            tree_project_active_item->addChild(item_scene); //添加子节点
            item_scene->setCheckState(0, Qt::Checked);
        }

        if(projectActive->getScene(0)){
            if(tree_project_active_item->childCount() != 0){
                tree_project_scene_active_item = tree_project_active_item->child(0);
            }
            projectActive->setActiveScene(projectActive->getScene(0));
            emit nestProjectChange(projectActive);
        }
    }
}

void Nest::onActionTreeProjectSave()
{
    qDebug() << tree_project_active_item->text(0) << "保存项目";
}

void Nest::onActionTreeProjectSaveAs()
{
    qDebug() << tree_project_active_item->text(0) << "另存为项目";
}

void Nest::onActionTreeProjectRename()
{
    // 根据项目名称获取项目指针
    QString pName = tree_project_active_item->text(0);
    Project *project = getProjectByName(pName);

    // 获取新的项目名称
    bool ok;
    QString text = QInputDialog::getText(this, tr("项目重命名"),
                                         tr("项目名称:"), QLineEdit::Normal,
                                         pName, &ok);
    if (ok && !text.isEmpty()){
        // 优化此部分至槽函数
        for(int i=0;i<project->getSceneList().length();i++){
            QString sName = project->getScene(i)->getName();
            QString key = pName + "_" + sName;
            if(nestNum.contains(key)){
                int num = nestNum[key];
                nestNum.remove(key);
                QString newKey = text + "_" + sName;
                nestNum.insert(newKey, num);
            }
        }
        project->changeName(text);
        emit nestProjectChange(project);
    }
}

void Nest::onActionTreeProjectClose()
{
    qDebug() << tree_project_active_item->text(0) << "关闭项目";

    // 获取项目名称
    QString pName = tree_project_active_item->text(0);

    // 关闭project
    //projectList.removeOne(getProjectByName(pName));
    removeProjectByName(pName);

    // 先删除子树
    for(int i=0;i<tree_project_active_item->childCount();i++){
        QString sName = tree_project_active_item->child(i)->text(0);
        QString key = pName + "_" + sName;
        if(nestNum.contains(key)){
            nestNum.remove(key);
        }
        QTreeWidgetItem *ch = tree_project_active_item->child(i);
        tree_project_active_item->removeChild(ch);
        // ****非常重要，不然会出现内存泄漏
        delete ch;
        ch = NULL;
    }

    // 删除树根
    tree_project->takeTopLevelItem(tree_project_item_list.lastIndexOf(tree_project_active_item));

    // 列表中删除该节点
    tree_project_item_list.removeOne(tree_project_active_item);

    // 定位到新的节点
    if(projectList.length() !=0 && tree_project_item_list.length() != 0){
        projectActive = projectList[0];
        tree_project_active_item = tree_project_item_list[0];
        if(tree_project_active_item->childCount() != 0){
            tree_project_scene_active_item = tree_project_active_item->child(0);
        }
        pieceScene = projectActive->getActiveScene();
    } else{
        projectActive = NULL;
    }

    // 删除项目对应的材料信息
    if(proSheetInfoMap.contains(pName)){
        // 清空所有列表
        delete proSheetInfoMap[pName];
        proSheetInfoMap[pName] = NULL;
        proSheetInfoMap.remove(pName);
    }

    // 删除项目对应的图层信息
    if(proSceneListMap.contains(pName)){
        // 清空列表
        qDeleteAll(proSceneListMap[pName]);
        proSceneListMap[pName].clear();
        // 删除该项目的图层Map
        proSceneListMap.remove(pName);
    }

    emit nestProjectChange(projectActive);
}

void Nest::onActionTreeProjectSceneChangeTo()
{

}

void Nest::onActionTreeProjectSceneMoveUpOne()
{

}

void Nest::onActionTreeProjectSceneMoveUpTop()
{

}

void Nest::onActionTreeProjectSceneMoveDownOne()
{

}

void Nest::onActionTreeProjectSceneMoveDownBottom()
{

}

void Nest::onActionTreeProjectSceneRename()
{
    // 根据图层名称获取图层指针
    QString pName = tree_project_active_item->text(0);
    QString sName = tree_project_scene_active_item->text(0);
    Project *project = getProjectByName(pName);
    Scene *scene = project->getSceneByName(sName);

    // 获取新的图层名称
    bool ok;
    QString text = QInputDialog::getText(this, tr("切割件重命名"),
                                         tr("新切割件名称:"), QLineEdit::Normal,
                                         sName, &ok);
    if (ok && !text.isEmpty()){
        // 优化此部分至槽函数
        QString key = pName + "_" + sName;
        if(nestNum.contains(key)){
            int num = nestNum[key];
            nestNum.remove(key);
            QString newKey = pName + "_" + text;
            nestNum.insert(newKey, num);
        }
        tree_project_scene_active_item->setText(0, text);
        scene->setName(text);
        emit nestProjectChange(project);
    }
}

void Nest::onActionTreeProjectSceneDelete()
{
    qDebug() << tree_project_scene_active_item->text(0) << "删除";
    QString name = tree_project_scene_active_item->text(0);
    bool ret = projectActive->removeSceneByName(name);
    qDebug() << "删除之后的长度： " << projectActive->getSceneList().length();
    if(ret){
        // 优化此部分至槽函数
        QString key = tree_project_active_item->text(0) + "_" + name;
        if(nestNum.contains(key)){
            nestNum.remove(key);
        }
        tree_project_active_item->removeChild(tree_project_scene_active_item);
        //QMessageBox::information(this, tr("通知"), tr("删除成功！"));
        if(projectActive->getScene(0)){
            projectActive->setActiveScene(projectActive->getScene(0));
            emit nestProjectChange(projectActive);
        }
    } else{
        QMessageBox::warning(this, tr("警告"), tr("删除失败！"));
    }
}

void Nest::onPieceNumChanged(const QString &num)
{
    if(projectList.length() == 0 || tree_project_item_list.length() == 0){
        return;
    }
    if(!tree_project_active_item){
        tree_project_active_item = tree_project->topLevelItem(0);
    }

    if(projectActive->getSceneList().length() == 0 || tree_project_active_item->childCount() == 0){
        return;
    }

    if(!tree_project_scene_active_item){
        tree_project_scene_active_item = tree_project_active_item->child(0);
    }

    int n = num.toInt();
    QString name = tree_project_active_item->text(0) + "_" +
            tree_project_scene_active_item->text(0);
    if(!nestNum.contains(name)){
        nestNum.insert(name, n);
    } else {
        nestNum[name] = n;
    }

    qDebug() << n << " " << name;
}

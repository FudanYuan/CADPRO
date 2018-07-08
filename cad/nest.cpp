#include "nest.h"
#include "ui_nest.h"
#include <QDockWidget>
#include <QTreeWidgetItem>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QRegExp>
#include <QValidator>
#include <QMetaType>
#include <nestconfiguredialog.h>
#include "nest.h"
#include "rectnestengine.h"
#include "packpointnestengine.h"
#include "nestengineconfiguredialog.h"
#include <sys/time.h>

#include <QDebug>

#define COUNT 20

// 初始化矩形排版引擎
QList<Nest::Component> RectNestEngine::components;  // 切割件
QList<RectNestEngine::MinRect> RectNestEngine::compMinRects;  // 切割件的最小矩形
QList<RectNestEngine::EmptyRectArea> RectNestEngine::emptyRectArea;  // 空白矩形
double RectNestEngine::mWidth = 0;  // 材料宽度
double RectNestEngine::mHeight = 0;  // 材料高度
double RectNestEngine::maxHight = 0;  // 最大高度值
long RectNestEngine::allRectsArea = 0; // 矩形切割件面积
long RectNestEngine::minRectsArea = LONG_MAX; // 矩形切割件面积

Nest::Nest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Nest)
{
    ui->setupUi(this);
    qRegisterMetaType<QVector<NestEngine::NestPiece>>("QVector<NestEngine::NestPiece>");
    setWindowTitle("CADPRO");
    setWindowState(Qt::WindowMaximized);
    setMouseTracking(true);     // 开启鼠标追踪

    // 系统初始化
    initAllPointers();  // 初始化所有指针
    initActions();      // 初始化动作
    initMenuBar();      // 初始化菜单栏
    initToolBar();      // 初始化工具栏
    initStatusBar();    // 初始化状态栏
    initDockWidget();   // 初始化窗口
    initProjectView();  // 初始化项目视图
    initPieceView();    // 初始化切割件视图
    initSheetView();    // 初始化材料视图
    initNestView();     // 初始化排版视图
    initConfiguration();// 初始化配置
    initConnect();      // 初始化信号和槽函数
}

Nest::~Nest()
{
    if(nestThread)
    {
        nestThread->quit();
        nestThread->wait();
    }
    qDebug() << "end destroy nest";
    delete ui;
}

void Nest::initAllPointers()
{
    config = NULL;
    nestView = NULL;
    projectActive = NULL;
    nestScene = NULL;
    pieceView = NULL;
    pieceScene = NULL;
    curSheet = NULL;
    nestThread = NULL;
    nestEngine = NULL;
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
    action_nest_start = new QAction(tr("排版"));
    action_nest_start->setStatusTip(tr("开始排版"));
    action_nest_start->setDisabled(true);
    connect(action_nest_start, &QAction::triggered, this, &Nest::onActionNestStart);

    action_nest_config = new QAction(tr("自动排版配置"));
    action_nest_config->setStatusTip(tr("自动排版配置"));
    connect(action_nest_config, &QAction::triggered, this, &Nest::onActionNestEngineConfig);

    action_nest_side_left = new QAction(tr("左靠边"));
    action_nest_side_left->setStatusTip(tr("切割件紧靠材料左边"));
    action_nest_side_left->setDisabled(true);
    connect(action_nest_side_left, &QAction::triggered, this, &Nest::onActionNestSideLeft);

    action_nest_side_right = new QAction(tr("右靠边"));
    action_nest_side_right->setStatusTip(tr("切割件紧靠材料右边"));
    action_nest_side_right->setDisabled(true);
    connect(action_nest_side_right, &QAction::triggered, this, &Nest::onActionNestSideRight);

    action_nest_side_top = new QAction(tr("顶靠边"));
    action_nest_side_top->setStatusTip(tr("切割件紧靠材料顶部"));
    action_nest_side_top->setDisabled(true);
    connect(action_nest_side_top, &QAction::triggered, this, &Nest::onActionNestSideTop);

    action_nest_side_bottom = new QAction(tr("底靠边"));
    action_nest_side_bottom->setStatusTip(tr("切割件紧靠材料底部"));
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

    action_view_tool_pieces = new QAction(tr("&切割件列表"), this);
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
    menu_nest->addAction(action_nest_start);
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
    tool_nest->addAction(action_nest_start);
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

    nestProgressBar = new QProgressBar();
    nestProgressBar->setRange(0,100);
    nestProgressBar->setValue(0);
    nestProgressLabel= new QLabel(tr(""), this);
    nestProgressLabel->setMargin(1);
    mousePositionLabel = new QLabel(tr(""), this);
    mousePositionLabel->setMargin(1);
    statusBar()->addPermanentWidget(nestProgressBar);//添加到状态栏的右边
    statusBar()->addPermanentWidget(nestProgressLabel);
    statusBar()->addPermanentWidget(mousePositionLabel);
}

void Nest::initDockWidget()
{
    // 设置dock窗口嵌套
    setDockNestingEnabled(true);

    // 新建dock窗口
    dock_project = new QDockWidget(tr("项目-<空项目>"), this);  // 添加project浮动窗口
    dock_piece = new QDockWidget(tr("切割件-<空项目>"), this);  // 添加pieces浮动窗口
    dock_sheet = new QDockWidget(tr("材料-<空项目>"), this);  // 添加sheets浮动窗口
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

void Nest::initProjectView()
{
    // 初始化project窗口
    tree_project = new QTreeWidget(dock_project);
    tree_project->setColumnCount(1); //设置列数
    tree_project->setHeaderLabel(tr("切割件列表")); //设置头的标题
    tree_project->setContextMenuPolicy(Qt::CustomContextMenu);//右键 不可少否则右键无反应
    connect(tree_project, &QTreeWidget::customContextMenuRequested, this, &Nest::showTreeMenu);
    //connect(tree_project, &QTreeWidget::itemClicked, this, &Nest::onProjectTreeItemClicked);
    connect(tree_project, &QTreeWidget::itemDoubleClicked, this, &Nest::onTreeProjectItemDoubleClicked);

    dock_project->setWidget(tree_project);
}

void Nest::updateProjectView()
{
    if(!projectActive){  // 活动项目为空，返回
        tree_project_active_item = NULL;
        tree_project_scene_active_item = NULL;
        setWindowTitle("CADPRO-<空项目>");
        dock_project->setWindowTitle("项目-<空项目>");
        return;
    }
    QString pName = projectActive->getName();  // 获取活动项目名称
    // 如果活动项目的图层列表为空，即没有添加切割件，则初始化为空。
    if(projectActive->getSceneList().length() < 1){
        // 初始化图层名称
        QString name_scene_new = tr("切割件列表-空");
        pieceScene = projectActive->addScene();
        pieceScene->setName(name_scene_new);

        bool flag = false;  // 是否含有此项目树枝
        foreach (QTreeWidgetItem *proItem, tree_project_item_list) {
            if(proItem->text(0) == pName){
                flag = true;
            }
        }
        if(!flag){  // 如果不含此项目树，则添加该项目树
            QTreeWidgetItem *item_project = new QTreeWidgetItem(tree_project, QStringList(pName));
            tree_project_item_list.append(item_project);
            tree_project_active_item = item_project;
        }
        // 添加图层节点
        QTreeWidgetItem *item_scene = new QTreeWidgetItem(tree_project_active_item,QStringList(name_scene_new)); //子节点1
        item_scene->setCheckState(0, Qt::Checked);
        tree_project_active_item->addChild(item_scene); //添加子节点
        tree_project->expandAll(); //结点全部展开
    }
    // 更新窗口名称
    setWindowTitle("CADPRO-<" + pName + ">");
    dock_project->setWindowTitle("项目-<" + pName + ">");
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

    firstPieceButton = new QPushButton(tr("第一个"), widget);
    firstPieceButton->setStatusTip(tr("第一个切割件"));
    connect(firstPieceButton, &QPushButton::clicked, this, &Nest::onActionFirstPiece);

    previousPieceButton = new QPushButton(tr("上一个"), widget);
    previousPieceButton->setStatusTip(tr("上一个切割件"));
    connect(previousPieceButton, &QPushButton::clicked, this, &Nest::onActionPreviousPiece);

    nextPieceButton = new QPushButton(tr("下一个"), widget);
    nextPieceButton->setStatusTip(tr("下一个切割件"));
    connect(nextPieceButton, &QPushButton::clicked, this, &Nest::onActionNextPiece);

    lastPieceButton = new QPushButton(tr("最后一个"), widget);
    lastPieceButton->setStatusTip(tr("最后一个切割件"));
    connect(lastPieceButton, &QPushButton::clicked, this, &Nest::onActionLastPiece);

    QHBoxLayout *subLayout = new QHBoxLayout();
    subLayout->addWidget(firstPieceButton);
    subLayout->addWidget(previousPieceButton);
    subLayout->addWidget(nextPieceButton);
    subLayout->addWidget(lastPieceButton);
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

void Nest::updatePieceView()
{
    if(!projectActive){  // 活动项目为空，返回
        dock_piece->setWindowTitle("切割件-<空项目>");
        widget->setDisabled(true);
        lineEdit->setText("1");
        pieceView->setScene(NULL);
        return;
    }
    // 更新piece图层
    pieceScene = projectActive->getActiveScene();
    if(!pieceScene){  // 如果活动图层为空，返回
        QMessageBox::warning(this, tr("错误"), tr("当前项目切割件列表为空！"));
        return;
    }
    // 更新piece视图
    widget->setDisabled(false);
    pieceView->setScene(pieceScene);
    // pieceView->centerOn();  //优化方案：以切割件的中心为视图中心

    // 更新项目-切割件
    QString pName = projectActive->getName();
    qDebug() << "debug: " << pName;
    if(!proPieceInfoMap.contains(pName)){  // 项目-切割件信息为空，返回
        QMessageBox::warning(this, tr("错误"), tr("当前项目切割件列表为空！"));
        return;
    }
    int index = proPieceInfoMap[pName]->curPieceID;
    if(index != -1){  // 如果切割件列表为空时，文本框中默认为1
        int count = proPieceInfoMap[pName]->pieceList[index]->getCount();
        qDebug() << "保存个数为: " << count;
        lineEdit->setText(QString::number(count));
    } else{
        lineEdit->setText("1");
    }
    // 更新窗口名称
    QString sName = pieceScene->getName();
    dock_piece->setWindowTitle("切割件-<" + pName + "-" + sName + ">");
}

void Nest::initSheetView()
{
    tree_sheet = new QTreeWidget(dock_project);
    tree_sheet->setColumnCount(1); //设置列数
    tree_sheet->setHeaderLabel(tr("材料信息")); //设置头的标题
    dock_sheet->setWidget(tree_sheet);
}

void Nest::updateSheetView()
{
    tree_sheet->clear();  // 清空材料树

    if(!projectActive){  // 如果活动项目为空
        tree_sheet->setHeaderLabel(tr("材料信息")); //设置头的标题
        dock_sheet->setWindowTitle("材料-<空项目>");
        return;
    }

    // 获取当前项目的材料信息
    QString pName = projectActive->getName();
    if(!proSheetInfoMap.contains(pName)){
        QMessageBox::warning(this, tr("错误"), tr("当前项目的材料信息为空！"));
        return;
    }
    QList<Sheet *> sheetList = proSheetInfoMap[pName]->sheetList;
    QList<qreal> pieceNumList = proSheetInfoMap[pName]->pieceNumList;
    QList<qreal> usageList = proSheetInfoMap[pName]->usageList;
    int sheetNum = sheetList.length();
    qreal rateTotal = 0.0;

    // 更新材料树
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

    const char *ch1 = "材料信息（共";
    const char *ch2 = "张板材料，总产能（利用率）：";
    const char *ch3 = "%）";
    char *buf = new char[strlen(ch1) + strlen(ch2) + strlen(ch3) + sizeof(sheetNum) + sizeof(rateTotal)];
    sprintf(buf, "%s%d%s%.2f%s", ch1, sheetNum, ch2, rateTotal, ch3);
    tree_sheet->setHeaderLabel(tr(buf)); //设置头的标题

    // 更新标题
    dock_sheet->setWindowTitle("材料-<" + pName + ">");
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
    if(!projectActive){  // 如果活动项目为空，则返回
        nestView->setScene(NULL);
        return;
    }

    QString pName = projectActive->getName();
    if(!proSceneListMap.contains(pName)){
        QMessageBox::warning(this, tr("错误"), tr("当前项目为初始化排版视图"));
        return;
    }
    if(proSceneListMap[pName].length() != 0){
        ProSheetInfo *proSheetInfo = proSheetInfoMap[pName];  // 获取该项目的项目-材料指针
        int id = proSheetInfo->curSheetID;  // 获取当前材料id
        nestScene = proSceneListMap[pName][id];
        nestView->setScene(nestScene);
        nestView->centerOn(nestView->mapFromScene(0,0));
    }
}

void Nest::updateAll()
{
    updateProjectView();  // 更新项目视图
    updatePieceView();  // 更新切割件视图
    updateSheetView();  // 更新材料视图
    updateNestView();  // 更新排版视图
}

void Nest::initConfiguration()
{
    qDebug() << "显示配置界面";
    if(config) {
        delete config;
        config = NULL;
    }
    config = new NestConfigure(this);
    connect(this, &Nest::nestConfigChanged, config, &NestConfigure::onConfigChanged);
}

void Nest::initConnect()
{
    connect(this, &Nest::nestProjectChange, this, &Nest::onNestProjectChanged);
}

void Nest::addProject()
{
    // 初始化项目名称
    QString pName = getNewProjectName();
    // 判断该名称是否存在
    foreach(Project *project, projectList){
        if(pName == project->getName()){
            pName += "-2";
        }
    }
    projectActive = new Project(this);
    projectActive->setType(Project::Nest);
    projectActive->setName(pName);
    connect(projectActive, &Project::projectNameChanged, this, &Nest::onProjectNameChanged);
    projectList.append(projectActive);  // 在项目列表中加入该项目

    // 更新piece视图
    pieceScene = new Scene(pieceView);
    pieceScene->setSceneRect(0, 0, dock_piece->width(), dock_piece->height());
    pieceView->setScene(pieceScene);
    pieceView->centerOn(pieceView->mapFromScene(0,0));

    // 初始化材料
    initSheet();

    // 初始化排版引擎
    initNestEngine();

    // 初始化项目-切割件信息
    if(!proPieceInfoMap.contains(pName)){  // 如果不存在该名称，则添加
        ProPieceInfo *proPieceInfo = new ProPieceInfo(pName, -1);
        proPieceInfoMap.insert(pName, proPieceInfo);
    }

    // 发送排版项目改变信号
    emit nestProjectChange(projectActive);
}

void Nest::initSheet()
{
    if(!projectActive){  // 如果活动项目指针为空，返回
        QMessageBox::warning(this, tr("错误"), tr("未选择任何项目！"));
        return;
    }

    // 判断该项目是否为新建项目，
    // 如果是，则自由选择材料类型；
    // 如果否，则只能选择该项目的材料类型
    Sheet::SheetType sheetType = Sheet::None;
    QString pName = projectActive->getName();
    if(proSheetInfoMap.contains(pName)){
        sheetType = proSheetInfoMap[pName]->sheetType;
        qDebug() << "项目已选择材料，材料类型为：" << sheetType;
    }
    SheetDialog mDialog(sheetType);
    mDialog.setDialogRole(SheetDialog::Nest);
    mDialog.exec();
    curSheet = mDialog.getSheetActive();

    if(!curSheet){  // 如果选择的材料为空，则返回
        QMessageBox::warning(this, tr("错误"), tr("未选择材料！"));
        return;
    }

    // 添加项目-材料信息
    if(proSheetInfoMap.contains(pName)){
        ProSheetInfo *proSheetInfo = proSheetInfoMap[pName];
        if(proSheetInfo->sheetType != curSheet->type){  // 如果选择材料类型不一样，返回
             QMessageBox::warning(this, tr("错误"), tr("材料类型不一致！"));
             return;
        }
        proSheetInfo->sheetList.append(curSheet);
        proSheetInfo->usageList.append(0.0);
        proSheetInfo->pieceNumList.append(0);
        proSheetInfo->curSheetID = proSheetInfo->sheetList.length() - 1;  // 当前材料为最后一张
    } else{
        ProSheetInfo *proSheetInfo = new ProSheetInfo(pName, curSheet->type);
        proSheetInfo->sheetList.append(curSheet);
        proSheetInfo->usageList.append(0.0);
        proSheetInfo->pieceNumList.append(0);
        proSheetInfo->curSheetID = 0;    // 当前材料为第一张
        proSheetInfoMap.insert(pName, proSheetInfo);
    }

    // 添加项目-排版图层信息
    if(!proSceneListMap.contains(pName)){  // 如果没有，则初始化
        QList<Scene *> sList;
        proSceneListMap.insert(pName, sList);
    }

    // 构建一个新的图层
    Scene *scene = new Scene(nestView);
    scene->setType(Scene::Nest);  // 设置图层类型
    scene->setSheetStyle(config->sheetStyle);
    scene->setMainGrid(config->mainGrid);
    scene->setSecondGrid(config->secondGrid);
    scene->setSheet(*curSheet);
    proSceneListMap[pName].append(scene);
}

void Nest::initNestEngine()
{
    qDebug() << "初始化排版引擎";
    // 获取该项目的材料类型
    QString pName = projectActive->getName();
    if(!proSheetInfoMap.contains(pName)){
        QMessageBox::warning(this, tr("警告"), tr("材料初始化出现错误！"));
        return;
    }

    NestEngineConfigure *nestEngineConfig = new NestEngineConfigure;  // 实例化时都要做配置文件操作
    Sheet::SheetType type = proSheetInfoMap[pName]->sheetType;
    NestEngineConfigureDialog nestEngineconfigDialog(nestEngineConfig);
    nestEngineconfigDialog.setDialogRole(NestEngineConfigureDialog::Nest);
    nestEngineconfigDialog.onTabChanged(type);
    nestEngineconfigDialog.exec();

    if(!proNestEngineConfigMap.contains(pName)){  // 保存项目配置
        NestEngineConfigure *config = new NestEngineConfigure();
        proNestEngineConfigMap.insert(pName, config);
    }
    NestEngineConfigure *config = proNestEngineConfigMap[pName];  // 获取项目排版引擎配置指针
    // 获取通过这个对话框设置的排版配置
    if(type == Sheet::Whole){
        NestEngineConfigure::WholeSheetNest *curWholeConfig = nestEngineconfigDialog.getCurWholeConfig();
        if(curWholeConfig == NULL){return;}
        config->setWholeSheetNest(*curWholeConfig);
        qDebug() <<"whole";
        qDebug() << "混合方式"<<curWholeConfig->wholemixing;
        qDebug() <<"排版"<<curWholeConfig->wholeorientation;
        qDebug() <<"旋转角度"<<curWholeConfig->wholedegree;
    }
    if(type == Sheet::Strip){
        NestEngineConfigure::StripSheetNest *curStripConfig = nestEngineconfigDialog.getCurStripConfig();
        if(curStripConfig == NULL){return;}
        config->setStripSheetNest(*curStripConfig);
        qDebug() <<"strip";
        qDebug() <<"排版方式"<<curStripConfig->strategy;
        qDebug() <<"适应方式"<<curStripConfig->stripadaptive;
        qDebug() <<"混合方式"<<curStripConfig->stripmixing;
    }
    if(type == Sheet::Package){
        NestEngineConfigure::PackageSheetNest *curPackageConfig = nestEngineconfigDialog.getCurPackageConfig();
        if(curPackageConfig == NULL){return;}
        config->setPackageSheetNest(*curPackageConfig);
        qDebug() <<"package";
        qDebug() << "混合方式"<<curPackageConfig->packagemixing;
        qDebug() <<"排版"<<curPackageConfig->packageorientation;
        qDebug() <<"旋转角度"<<curPackageConfig->packagedegree;
    }
}

/*矩形排版策略*/
void Nest::initRectNestEngine()
{
    double len = RectNestEngine::components.length();
    if(len <= 0){
        QMessageBox::warning(this, tr("错误"), tr("请添加切割件！"));
        return;
    }

    // 将切割件列表转化为最小矩形列表
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
        QVector<PiecePoint> pList;
        for(int j=0;j<polyline->getPoints().length();j++){
            QPointF oldPoint = polyline->getPoints()[j];
            QPointF newPoint = oldPoint-offset;
            PiecePoint piecePoint(newPoint.rx(), newPoint.ry(), RESERVE_DOUBLE);
            pList.append(piecePoint);
        }

        PieceOffset pieceOffset(i, RESERVE_DOUBLE,
                                           RESERVE_DOUBLE, RESERVE_DOUBLE,
                                           RESERVE_STRING, RESERVE_DOUBLE,
                                           RESERVE_INT, pList.length(), pList);
        pieceOffsetList.append(pieceOffset);
    }

    QString pName = projectActive->getName();
    if(!proPieceOffsetMap.contains(pName)){
        proPieceOffsetMap.insert(pName, pieceOffsetList);
    } else{
        proPieceOffsetMap[pName] = pieceOffsetList;
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

    if(!proPieceCenterMap.contains(pName)){
        proPieceCenterMap.insert(pName, pieceCenterList);
    } else{
        proPieceCenterMap[pName] = pieceCenterList;
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
        updateSheetView();
    }
    qDebug() << "材料使用率： " << g.getFittestGenome().getFitness();
}

void Nest::showNestResult()
{
    QString pName = projectActive->getName();  // 获取项目名称
    QPointF sheetCenter;
    if(proSceneListMap.contains(pName)){
        if(proSceneListMap[pName].length() > 0){
            nestScene = proSceneListMap[pName][0];  // 默认图层为第一个图层
        }
    }
    if(proSheetInfoMap.contains(pName)){
        if(proSheetInfoMap[pName]->sheetList.length() > 0){
            proSheetInfoMap[pName]->curSheetID = 0;  // 转到第一张材料
            sheetCenter = proSheetInfoMap[pName]->sheetList[0]->layoutRect().center();
        }
    }
    nestView->setScene(nestScene);  // 更新排版视图
    nestView->centerOn(nestView->mapFromScene(sheetCenter));
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
        if(tree_project_active_item->childCount() > 0){
            tree_project_scene_active_item = tree_project_active_item->child(0);
        } else{
            tree_project_scene_active_item = NULL;
        }
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
        QString sName = tree_project_scene_active_item->text(0);  // 获取图层名称
        if(sName == tr("切割件列表-空")){  // 如果是默认分支，则不显示菜单
            return;
        }
        menu_tree_project_scene = new QMenu(tree_project);
        action_tree_project_scene_change_to = new QAction(tr("切换至该切割件"), tree_project);
        action_tree_project_scene_rename = new QAction(tr("重命名"), tree_project);
        action_tree_project_scene_delete = new QAction(tr("删除"), tree_project);

        connect(action_tree_project_scene_change_to, &QAction::triggered, this, &Nest::onActionTreeProjectSceneChangeTo);
        connect(action_tree_project_scene_rename, &QAction::triggered, this, &Nest::onActionTreeProjectSceneRename);
        connect(action_tree_project_scene_delete, &QAction::triggered, this, &Nest::onActionTreeProjectSceneDelete);

        menu_tree_project_scene->addAction(action_tree_project_scene_change_to);
        menu_tree_project_scene->addAction(action_tree_project_scene_rename);
        menu_tree_project_scene->addAction(action_tree_project_scene_delete);
        menu_tree_project_scene->exec(QCursor::pos());  //在当前鼠标位置显示
    }
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

    if(!projectActive){
        QMessageBox::warning(this, tr("错误"), tr("未选择任何项目！"));
        return false;
    }

    QString pName = projectActive->getName();
    if(!proPieceCenterMap.contains(pName)
            || !proPieceOffsetMap.contains(pName)){
        QMessageBox::warning(this, tr("错误"), tr("该项目还未排版!"));
        return false;
    }

    try{
        int centerCount = 0;
        for(int i=0; i < proSheetInfoMap[pName]->pieceNumList.length(); i++){
            centerCount += proSheetInfoMap[pName]->pieceNumList[i];
        }

        projectActive->tnfFileWriter(fileName,
                           proSheetInfoMap[pName]->sheetList,
                           proPieceCenterMap[pName],
                           proPieceOffsetMap[pName]);
    } catch(QString exception){
        QMessageBox::warning(this, tr("错误"), exception);
        return false;
    }

    return true;
}

void Nest::setNestActionDisabled(bool flag)
{
    action_nest_start->setDisabled(flag);
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

void Nest::onMousePositionChanged(QPointF pos)
{
    mousePositionLabel->setText(tr("X=") + QString::number(pos.rx()) + " Y=" + QString::number(-pos.ry()));
}

void Nest::onNestProjectChanged(Project *curProject)
{
    setNestActionDisabled(!curProject);  // 去能排版相关的action
    updateAll();  // 更新全部组件
}

void Nest::onProjectNameChanged(QString lastName, QString presentName)
{
    for(int i=0; i<tree_project_item_list.length(); i++){
        if(tree_project_item_list.at(i)->text(0) == lastName){
            // 改变项目名称
            tree_project_item_list.at(i)->setText(0, presentName);
            // 改变项目对应的切割件信息
            if(proPieceInfoMap.contains(lastName)){
                proPieceInfoMap[lastName]->projectName = presentName;
                proPieceInfoMap.insert(presentName, proPieceInfoMap[lastName]);
                proPieceInfoMap.remove(lastName);
            }

            // 改变项目对应的材料信息
            if(proSheetInfoMap.contains(lastName)){
                proSheetInfoMap[lastName]->projectName = presentName;
                proSheetInfoMap.insert(presentName, proSheetInfoMap[lastName]);
                proSheetInfoMap.remove(lastName);
            }

            // 改变项目对应的图层
            if(proSceneListMap.contains(lastName)){
                proSceneListMap.insert(presentName, proSceneListMap[lastName]);
                proSceneListMap.remove(lastName);
            }

            // 改变项目对应的排版引擎配置
            if(proNestEngineConfigMap.contains(lastName)){
                proNestEngineConfigMap.insert(presentName, proNestEngineConfigMap[lastName]);
                proNestEngineConfigMap.remove(lastName);
            }
            break;
        }
    }
    // 改变树枝结构的名称
    tree_project_active_item->setText(0, presentName);

    /*
     *
    // 更新活动项目Map
    QString pName = projectActive->getName();
    if(proSceneListMap.contains(pName)){
        qDeleteAll(proSceneListMap[pName]);
        proSceneListMap[pName].clear();
    }
     */
}

void Nest::onNestProgressChanged(int i)
{
    qDebug() << "已处理" << i << "%";
    nestProgressBar->setValue(i);
    nestProgressLabel->setText(QString("%1%").arg(i));
    if(i == 100){
        nestProgressLabel->setText("排版结束");
        QMessageBox::information(this, tr("通知"), tr("排版结束"));
    }
}

void Nest::onNestFinished(QVector<NestEngine::NestPiece> nestPieceList)
{
    qDebug() << "排版结束";
    if(nestThread){
        // 退出排版线程
        nestThread->quit();
        nestThread->wait();
    }
    if(!nestThread){
        qDebug() << "真正结束";
    }
    // 保存排版结果
    QString pName = projectActive->getName();
    if(!proPieceCenterMap.contains(pName)){  // 如果项目-零件中心信息不存在，代表该项目未排版
        QList<PieceCenter> pieceCenterList;  // 零件中心列表，用来保存排版后的零件中心
        proPieceCenterMap.insert(pName, pieceCenterList);
    } else{
        proPieceCenterMap[pName].clear();  // 如果已排，则清空旧的排版信息
    }
    if(!proPieceOffsetMap.contains(pName)){  // 如果项目-零件中心信息不存在，代表该项目未排版
        QList<PieceOffset> pieceOffsetList;  // 零件中心列表，用来保存排版后的零件中心
        proPieceOffsetMap.insert(pName, pieceOffsetList);
    } else{
        proPieceOffsetMap[pName].clear();  // 如果已排，则清空旧的排版信息
    }

    if(proSceneListMap.contains(pName) && proPieceInfoMap.contains(pName)){
        QList<Scene*> sceneList = proSceneListMap[pName];
        QVector<Piece*> pieceList = proPieceInfoMap[pName]->pieceList.toVector();
        for(int i=0; i<pieceList.length(); i++) {  // 遍历零件列表，保存偏移点
            QVector<PiecePoint> pointsList;  // 用来保存零件点列表
            QVector<QPointF> offsetList = pieceList[i]->getOffset();
            for(int j=0;j<offsetList.length();j++){
                PiecePoint piecePoint(offsetList[j].rx(), offsetList[j].ry(), RESERVE_DOUBLE);
                pointsList.append(piecePoint);
            }
            PieceOffset pieceOffset(i, RESERVE_DOUBLE,
                                    RESERVE_DOUBLE, RESERVE_DOUBLE,
                                    RESERVE_STRING, RESERVE_DOUBLE,
                                    RESERVE_INT, offsetList.length(), pointsList);
            proPieceOffsetMap[pName].append(pieceOffset);
        }
        for(int i=0; i<nestPieceList.length(); i++){  // 保存
            Polyline *p = new Polyline;
            int typeID = nestPieceList[i].typeID;  // 切割件ID
            int sheetID = nestPieceList[i].sheetID;
            QPointF pos = nestPieceList[i].position;
            qreal angle = nestPieceList[i].alpha;
            Piece piece = *pieceList[typeID];
            piece.moveTo(pos); // 移动
            piece.rotate(piece.getMinBoundingRect().center(), angle);  // 旋转
            p->setPolyline(piece.getPointsList(), Polyline::line);
            sceneList[sheetID]->addCustomPolylineItem(p);  // 将多边形加入该图层

            // 保存零件中心点
            PieceCenter pieceCenter(i, sheetID, typeID, angle, RESERVE_INT, pos.rx(), pos.ry());
            proPieceCenterMap[pName].append(pieceCenter);
        }
    }

    // 显示排版结果
    showNestResult();
//    for(int i=0; i<nestEngine->getSheetList().length(); i++){
//        for(int j=0; j<nestEngine->sheetPackPointPositionMap[i].size(); j++){
//            PackPointNestEngine::PackPoint packPoint = nestEngine->sheetPackPointPositionMap[i].value(j);
//            if(!packPoint.coverd){
//                Point *p = new Point;
//                QPointF point = packPoint.position + i * QPointF(1005, 0);
//                p->setPoint(point);
//                nestScene->addCustomPointItem(p);
//            }
//        }
    //    }
}

void Nest::onNestThreadFinished()
{
    qDebug() << "线程结束";
    delete nestThread;
    nestThread = NULL;
}

void Nest::closeEvent(QCloseEvent *event)
{
    if(nestThread){
        qDebug() << "线程还未结束";
        return;
    }
    if(!maybeSave()) {
        event->ignore();
        return;
    }
    qApp->quit();
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
    // 保存逻辑
    bool res = saveFile(fileName);
    projectActive->setSaved(res);
    if(res){
        projectActive->saveProject();
        QMessageBox::information(this, tr("通知"), projectActive->getName() + tr("保存成功"));
    }
    return res;
}

bool Nest::onActionFileSaveAll()
{
    foreach (Project *project, projectList) {
        projectActive = project;
        onActionFileSave();
    }
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
    NestConfigureDialog configDialog(config);
    configDialog.exec();
    initConfiguration();
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

void Nest::onActionNestStart()
{
    // 这里需要开一个次线程来开始排版任务，否则会造成GUI假死
    if(nestThread)
    {
        QMessageBox::warning(this, tr("警告"), tr("正在排版，请稍候或结束该进程！"));
        return;
    }
    qDebug() << "开始排版";
    if(!projectActive){  // 如果活动项目为空，则返回
        QMessageBox::warning(this, tr("错误"), tr("未选择任何项目！"));
        return;
    }

    QString pName = projectActive->getName();
    qDebug() << "1: " << proPieceInfoMap.contains(pName);
    qDebug() << "2: " << proSheetInfoMap.contains(pName);
    qDebug() << "3: " << proSceneListMap.contains(pName);
    qDebug() << "4: " << proNestEngineConfigMap.contains(pName);
    if(!proPieceInfoMap.contains(pName)
            || !proSheetInfoMap.contains(pName)
            || !proSceneListMap.contains(pName)
            || !proNestEngineConfigMap.contains(pName)){  // 如果map中不包含该项目名称，返回
        QMessageBox::warning(this, tr("错误"), tr("不存在该名称的项目！"));
        return;
    }

    ProPieceInfo *proPieceInfo = proPieceInfoMap[pName];
    if(proPieceInfo->pieceList.length() < 1){  // 如果切割件信息为空，返回
        QMessageBox::warning(this, tr("错误"), tr("切割件列表为空！"));
        return;
    }

    ProSheetInfo *proSheetInfo = proSheetInfoMap[pName];
    if(proSheetInfo->sheetList.length() < 1){  // 如果材料列表为空，返回
        QMessageBox::warning(this, tr("错误"), tr("材料列表为空！"));
        return;
    }

    // 按照配置初始化排版引擎
    QVector<Piece> pieceList;  // 初始化排样切割件
    foreach (Piece *piece, proPieceInfoMap[pName]->pieceList) {
        qDebug() << "hah: " << piece->getCount();
        pieceList.append(*piece);
    }

    QVector<Sheet> sheetList;  // 初始化排样材料
    foreach (Sheet *sheet, proSheetInfoMap[pName]->sheetList) {
        sheetList.append(*sheet);
    }
    // 创建线程
    nestThread = new QThread();
    // 创建排版引擎
    nestEngine = new PackPointNestEngine(pieceList, sheetList, 50, 1);
    nestEngine->setCompactStep(5);
    nestEngine->setCompactAccuracy(1);
    nestEngine->setCutStep(300);
    nestEngine->setAutoRepeatLastSheet(false);
    NestEngineConfigure *proConfig = proNestEngineConfigMap[pName];
    nestEngine->initNestEngineConfig(proSheetInfo->sheetType, proConfig);  // 初始化排版配置
    // 将排版引擎移至线程
    nestEngine->moveToThread(nestThread);
    connect(nestThread, &QThread::finished, nestThread, &QObject::deleteLater);
    connect(nestThread, &QThread::finished, nestEngine, &QObject::deleteLater);
    connect(nestThread, &QThread::finished, this, &Nest::onNestThreadFinished);
    connect(this, &Nest::nestStart, nestEngine, &NestEngine::onNestStart);
    connect(nestEngine, &NestEngine::progress, this, &Nest::onNestProgressChanged);
    connect(nestEngine, &NestEngine::nestFinished, this, &Nest::onNestFinished);
    emit nestStart();  // 发送排版信号
    nestThread->start();
}

void Nest::onActionNestEngineConfig()
{
    qDebug() << "自动排版配置";
    NestEngineConfigure *nestEngineConfig = new NestEngineConfigure;  // 实例化时要做都配置文件操作
    NestEngineConfigureDialog nestEngineconfigDialog(nestEngineConfig);
    nestEngineconfigDialog.setDialogRole(NestEngineConfigureDialog::Manager);
    nestEngineconfigDialog.exec();
    return;
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
    updateSheetView();  // 更新视图
    updateNestView();  // 更新排版视图
}

void Nest::onActionSheetRemove()
{
    // 删除材料不仅要删除材料信息，还要删除对应的图层信息
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
    ProSheetInfo *proSheetInfo = proSheetInfoMap[pName];  // 获取该项目的项目-材料指针
    if(proSheetInfo->sheetList.length() < 1){  // 如果该项目的材料列表为空，返回
        QMessageBox::warning(this, tr("错误"), tr("材料列表已空！"));
        return;
    }
    int id = proSheetInfo->curSheetID;  // 获取当前材料id
    // 在列表删除该材料
    Sheet *s = proSheetInfo->sheetList[id];
    proSheetInfo->sheetList.removeAt(id);
    delete s;  // 释放内存，防止内存泄漏
    s = NULL;

    // 在列表删除该材料对应的图层
    Scene *scene = proSceneListMap[pName][id];
    proSceneListMap[pName].removeAt(id);
    delete scene;
    scene = NULL;

    if(id > proSheetInfo->sheetList.length() - 1){  // 如果id大于列表长度，则为最后一张
        proSheetInfo->curSheetID = proSheetInfo->sheetList.length() - 1;  // 更新当前材料id
    }
    qDebug() << "删除之后：" << proSheetInfo->curSheetID;
    updateSheetView();  // 更新视图
    updateNestView();  // 更新排版视图
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
    ProSheetInfo *proSheetInfo = proSheetInfoMap[pName];  // 获取该项目的项目-材料指针
    if(proSheetInfo->sheetList.length() < 1){  // 如果材料列表为空，返回
        QMessageBox::warning(this, tr("错误"), tr("材料列表为空！"));
        return;
    }
    int id = proSheetInfo->curSheetID;  // 获取当前材料id
    Sheet *sheet = proSheetInfo->sheetList[id]->sheet();  // 获取当前材料指针
    proSheetInfo->sheetList.append(sheet);  // 将该材料指针加入材料列表
    proSheetInfo->pieceNumList.append(0);
    proSheetInfo->usageList.append(0.0);
    proSheetInfo->curSheetID = proSheetInfo->curSheetID + 1;

    // 添加图层
    Scene *scene = new Scene(nestView);
    scene->setType(Scene::Nest);  // 设置图层类型
    scene->setSheetStyle(config->sheetStyle);
    scene->setMainGrid(config->mainGrid);
    scene->setSecondGrid(config->secondGrid);

    // 将该图层加入proSceneListMap
    if(proSceneListMap.contains(pName)){
        proSceneListMap[pName].append(scene);
    } else{
        QList<Scene *> sList;
        sList.append(scene);
        proSceneListMap.insert(pName, sList);
    }
    updateSheetView();  // 更新视图
    updateNestView();  // 更新排版视图
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
    ProSheetInfo *proSheetInfo = proSheetInfoMap[pName];
    if(proSheetInfo->sheetList.length() < 1){  // 如果材料列表为空，返回
        QMessageBox::warning(this, tr("错误"), tr("材料列表为空！"));
        return;
    }
    int id = proSheetInfo->curSheetID;  // 获取当前sheetID
    if(id == 0){
        QMessageBox::warning(this, tr("警告"), tr("目前已经是第一张材料！"));
        return;
    }
    nestScene = proSceneListMap[pName][id-1];
    proSheetInfo->curSheetID = id - 1;

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
    ProSheetInfo *proSheetInfo = proSheetInfoMap[pName];
    if(proSheetInfo->sheetList.length() < 1){  // 如果材料列表为空，返回
        QMessageBox::warning(this, tr("错误"), tr("材料列表为空！"));
        return;
    }
    int id = proSheetInfo->curSheetID;  // 获取当前sheetID
    if(id == proSheetInfo->sheetList.length()-1){
        QMessageBox::warning(this, tr("警告"), tr("目前已经是最后一张材料！"));
        return;
    }
    nestScene = proSceneListMap[pName][id+1];
    proSheetInfo->curSheetID = id + 1;

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
    ProSheetInfo *proSheetInfo = proSheetInfoMap[pName];
    if(proSheetInfo->sheetList.length() < 1){  // 如果材料列表为空，返回
        QMessageBox::warning(this, tr("错误"), tr("材料列表为空！"));
        return;
    }
    int maxId = proSheetInfo->sheetList.length();  // 获取当前最大的ID
    bool ok;
    int id = QInputDialog::getInt(this, tr("跳转至"),
                                  tr("请输入材料序号:"), 1, 1, maxId, 1,
                                  &ok);
    if(ok){
        nestScene = proSceneListMap[pName][id-1];
        proSheetInfo->curSheetID = id - 1;

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
    ProSheetInfo *proSheetInfo = proSheetInfoMap[pName];
    if(proSheetInfo->sheetList.length() < 1){  // 如果材料列表为空，返回
        QMessageBox::warning(this, tr("错误"), tr("材料列表为空！"));
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
    qDebug() << "切割件视图" << toggled;
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

void Nest::onProjectTreeItemClicked(QTreeWidgetItem *item, int column)
{
    qDebug() << "点击： " << item->text(0) << "  " << column;
    QTreeWidgetItem *parent = item->parent();  //获得父节点
    if(NULL == parent) {  // 项目树
        tree_project_active_item = item;
        tree_project_scene_active_item = tree_project_active_item->child(0);
        // 根据项目名称获取项目指针
        QString pName = item->text(0);
        projectActive = getProjectByName(pName);  // 更新活动项目
        // 如果活动项目的图层列表不为空，则设置该项目第一个图层为活动图层
        if(projectActive->getSceneList().length() > 0){
            projectActive->setActiveScene(projectActive->getScene(0));  // 设置该项目第一个图层为活动图层
            QString sName = projectActive->getActiveScene()->getName();  // 获取图层名称
            if(proPieceInfoMap.contains(pName) && sName != tr("切割件列表-空")){  // 更新当前切割件ID
                proPieceInfoMap[pName]->curPieceID = 0;
            } else{
                proPieceInfoMap[pName]->curPieceID = -1;
            }
        }
    } else{  // 图层树
        tree_project_active_item = parent;
        tree_project_scene_active_item = item;
        QString pName = parent->text(0);
        QString sName = item->text(0);
        projectActive = getProjectByName(pName);  // 更新活动项目
        projectActive->setActiveScene(projectActive->getSceneByName(sName));  // 更新活动图层
        if(proPieceInfoMap.contains(pName) && sName != tr("切割件列表-空")){  // 更新当前切割件ID
            int index = tree_project_active_item->indexOfChild(tree_project_scene_active_item);
            proPieceInfoMap[pName]->curPieceID = index;
        }
    }
    // 发送排版项目改变信号
    emit nestProjectChange(projectActive);
}

void Nest::onTreeProjectItemDoubleClicked(QTreeWidgetItem *item)
{
    QTreeWidgetItem *parent = item->parent();  //获得父节点
    if(NULL == parent) {  // 项目树
        tree_project_active_item = item;
        tree_project_scene_active_item = tree_project_active_item->child(0);
        // 根据项目名称获取项目指针
        QString pName = item->text(0);
        projectActive = getProjectByName(pName);  // 更新活动项目
        // 如果活动项目的图层列表不为空，则设置该项目第一个图层为活动图层
        if(projectActive->getSceneList().length() > 0){
            projectActive->setActiveScene(projectActive->getScene(0));  // 设置该项目第一个图层为活动图层
            QString sName = projectActive->getActiveScene()->getName();  // 获取图层名称
            if(proPieceInfoMap.contains(pName) && sName != tr("切割件列表-空")){  // 更新当前切割件ID
                proPieceInfoMap[pName]->curPieceID = 0;
            } else{
                proPieceInfoMap[pName]->curPieceID = -1;
            }
        }
    } else{  // 图层树
        tree_project_active_item = parent;
        tree_project_scene_active_item = item;
        QString pName = parent->text(0);
        QString sName = item->text(0);
        projectActive = getProjectByName(pName);  // 更新活动项目
        projectActive->setActiveScene(projectActive->getSceneByName(sName));  // 更新活动图层
        if(proPieceInfoMap.contains(pName) && sName != tr("切割件列表-空")){  // 更新当前切割件ID
            int index = tree_project_active_item->indexOfChild(tree_project_scene_active_item);
            proPieceInfoMap[pName]->curPieceID = index;
        }
    }
    // 发送排版项目改变信号
    emit nestProjectChange(projectActive);
}

void Nest::onActionTreeProjectNestScene()
{
    QString pName = tree_project_active_item->text(0);  // 获取项目名称
    projectActive = getProjectByName(pName);  // 更新活动的项目
    if(projectActive->getSceneList().length() > 0){
        projectActive->setActiveScene(projectActive->getScene(0));
    }
    emit nestProjectChange(projectActive);   // 发送项目改变信号
    onActionNestStart();  // 开始排版

    return;

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
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开DXF文件"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        QString pName = tree_project_active_item->text(0);  // 获取当前项目
        Project *project = getProjectByName(pName);
        if(!project){
            QMessageBox::warning(this, tr("错误"), tr("无法获取当前项目"));
            return;
        }
        if(project->getSceneList().length() < 1){  // 如果项目图层为空，则返回
            emit nestProjectChange(project);
            return;
        }

        QString firSceName = project->getScene(0)->getName();
        if(firSceName == tr("切割件列表-空")){  // 如果第一个图层为默认图层，则删除该图层
            project->removeSceneByName(firSceName);
            tree_project_active_item->removeChild(tree_project_active_item->child(0));
        }

        // 读取dxf文件
        Project *projectTmp = new Project(this); // 局部变量，用来记录导入的dxf文件
        try{
            projectTmp->setType(Project::Nest);
            projectTmp->dxfFileReader(fileName);
        } catch (QString exception){
            QMessageBox::warning(this, tr("错误"), exception);
            emit nestProjectChange(project);
            delete projectTmp;
            projectTmp = NULL;
            return;
        }

        ProPieceInfo *proPieceInfo = proPieceInfoMap[pName];  // 获取切割件信息

        // 将读入的切割件保存到活动项目中
        QList<Scene*> sList;
        int oldLen = project->getSceneList().length();  // 获取活动图层列表长度
        int count = 0;
        foreach (Scene* s, projectTmp->getSceneList()) {// 获取读入的图层列表
            Scene *scene = s->copy();
            // 更新项目-切割件信息，注意：图层要确保是多变形才加入图层
            foreach (Polyline *polyline, scene->getPolylineList()) {
                QString name = scene->getName();
                int index = name.toInt();
                scene->setName(QString::number(oldLen+index));
                sList.append(scene);
                Piece *piece = new Piece(polyline);  // 切割件个数默认为1，默认精确到6位
                proPieceInfo->insertPiece(piece);
                count++;
            }
        }
        project->insertScene(sList);  // 加入图层列表

        // 更新项目树
        for(int i=0; i<count; i++){
            QString name_scene_new = project->getScene(oldLen+i)->getName();
            QTreeWidgetItem *item_scene = new QTreeWidgetItem(tree_project_active_item, QStringList(name_scene_new)); //子节点1
            tree_project_active_item->addChild(item_scene); //添加子节点
            item_scene->setCheckState(0, Qt::Checked);
        }

        // 设置活动图层
        int lastIndex = oldLen + count - 1;
        if(project->getScene(lastIndex) && tree_project_active_item->child(lastIndex)){  // 设置加入的后一个图层为活动图层
            project->setActiveScene(project->getScene(lastIndex));
            tree_project_scene_active_item = tree_project_active_item->child(lastIndex);
            proPieceInfo->curPieceID = lastIndex;  // 更新项目-切割件信息的当前切割件ID
        } else{  // 如果项目树子节点个数大于0，则为设置活动图层为第一个
            if(project->getScene(0) && tree_project_active_item->child(0)){
                project->setActiveScene(project->getScene(0));
                tree_project_scene_active_item = tree_project_active_item->child(0);
                proPieceInfo->curPieceID = 0;  // 更新项目-切割件信息的当前切割件ID
            }
        }

        emit nestProjectChange(project);
    }
}

void Nest::onActionTreeProjectSave()
{
    qDebug() << tree_project_active_item->text(0) << "保存项目";
    Project *project = projectActive;  // 保存目前的项目状态
    QString pName = tree_project_active_item->text(0);  // 获取当前项目
    projectActive = getProjectByName(pName);
    if(!projectActive){
        QMessageBox::warning(this, tr("错误"), tr("无法获取当前项目"));
        return;
    }
    onActionFileSave();  // 保存项目
    projectActive = project;  // 恢复之前的状态
}

void Nest::onActionTreeProjectSaveAs()
{
    qDebug() << tree_project_active_item->text(0) << "另存为项目";
    Project *project = projectActive;  // 保存目前的项目状态
    QString pName = tree_project_active_item->text(0);  // 获取当前项目
    projectActive = getProjectByName(pName);
    if(!projectActive){
        QMessageBox::warning(this, tr("错误"), tr("无法获取当前项目"));
        return;
    }
    onActionFileSaveAs();  // 另存为项目
    projectActive = project;  // 恢复之前的状态
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
        // 判断名称是否重复
        foreach(Project *project, projectList){
            if(text == project->getName()){
                QMessageBox::warning(this, tr("警告"), tr("该项目已存在！"));
                return;
            }
        }
        project->changeName(text);
        emit nestProjectChange(project);
    }
}

void Nest::onActionTreeProjectClose()
{
    qDebug() << tree_project_active_item->text(0) << "关闭项目";
    QString pName = tree_project_active_item->text(0);  // 获取项目名称
    // 删除项目
    removeProjectByName(pName);

    // 删除项目对应的切割件信息
    if(proPieceInfoMap.contains(pName)){
        // 释放内存
        delete proPieceInfoMap[pName];
        proPieceInfoMap[pName] = NULL;
        proPieceInfoMap.remove(pName);
    }

    // 删除项目对应的材料信息
    if(proSheetInfoMap.contains(pName)){
        // 释放内存
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

    // 删除项目对应的排版引擎配置信息
    if(proNestEngineConfigMap.contains(pName)){
        // 释放配置信息内存
        delete proNestEngineConfigMap[pName];
        proNestEngineConfigMap[pName] = NULL;
        // 删除该项目的图层Map
        proNestEngineConfigMap.remove(pName);
    }

    // 先删除子树
    for(int i=0; i<tree_project_active_item->childCount(); i++){
        QTreeWidgetItem *ch = tree_project_active_item->child(i);
        tree_project_active_item->removeChild(ch);
        // ****非常重要，不然会出现内存泄漏****
        delete ch;
        ch = NULL;
    }

    // 再删除树根
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
        if(!projectActive->getActiveScene()  && projectActive->getSceneList().length() > 0){
            projectActive->setActiveScene(projectActive->getScene(0));
        }
    } else{
        projectActive = NULL;
    }

    emit nestProjectChange(projectActive);
}

void Nest::onActionTreeProjectSceneChangeTo()
{
    QString pName = tree_project_active_item->text(0);  // 获取项目名称
    QString sName = tree_project_scene_active_item->text(0);  // 获取图层名称
    projectActive = getProjectByName(pName);  // 更新活动项目
    projectActive->setActiveScene(projectActive->getSceneByName(sName));  // 更新活动图层
    if(proPieceInfoMap.contains(pName)){  // 更新当前切割件ID
        int index = tree_project_active_item->indexOfChild(tree_project_scene_active_item);
        proPieceInfoMap[pName]->curPieceID = index;
    }
    emit nestProjectChange(projectActive);  // 发送项目更新信息
}

void Nest::onActionFirstPiece()
{
    qDebug() << "第一个切割件";
    int index = tree_project_active_item->indexOfChild(tree_project_scene_active_item);  // 获取活动分支的索引
    if(index == 0){
        QMessageBox::warning(this, tr("警告"), tr("目前已经是第一个切割件！"));
        return;
    }
    QString pName = tree_project_active_item->text(0);  // 获取项目名称
    tree_project_scene_active_item = tree_project_active_item->child(0);  // 更新活动图层分支
    QString sName = tree_project_scene_active_item->text(0);  // 获取图层名称
    projectActive = getProjectByName(pName);  // 更新活动项目
    projectActive->setActiveScene(projectActive->getSceneByName(sName));  // 更新活动图层
    tree_project_active_item->child(index)->setSelected(false);
    tree_project_scene_active_item->setSelected(true);
    if(proPieceInfoMap.contains(pName)){  // 更新当前切割件ID
        proPieceInfoMap[pName]->curPieceID = 0;
    }
    emit nestProjectChange(projectActive);  // 发送项目更新信息
}

void Nest::onActionPreviousPiece()
{
    qDebug() << "上一个切割件";
    int index = tree_project_active_item->indexOfChild(tree_project_scene_active_item);  // 获取活动分支的索引
    if(index == 0){
        QMessageBox::warning(this, tr("警告"), tr("目前已经是第一个切割件！"));
        return;
    }
    QString pName = tree_project_active_item->text(0);  // 获取项目名称
    tree_project_scene_active_item = tree_project_active_item->child(index-1);  // 更新活动图层分支
    QString sName = tree_project_scene_active_item->text(0);  // 获取图层名称
    projectActive = getProjectByName(pName);  // 更新活动项目
    projectActive->setActiveScene(projectActive->getSceneByName(sName));  // 更新活动图层
    tree_project_active_item->child(index)->setSelected(false);
    tree_project_scene_active_item->setSelected(true);
    if(proPieceInfoMap.contains(pName)){  // 更新当前切割件ID
        proPieceInfoMap[pName]->curPieceID = index-1;
    }
    emit nestProjectChange(projectActive);  // 发送项目更新信息
}

void Nest::onActionNextPiece()
{
    qDebug() << "下一个切割件";
    int index = tree_project_active_item->indexOfChild(tree_project_scene_active_item);  // 获取活动分支的索引
    if(index == tree_project_active_item->childCount() - 1){
        QMessageBox::warning(this, tr("警告"), tr("目前已经是最后一个切割件！"));
        return;
    }
    QString pName = tree_project_active_item->text(0);  // 获取项目名称
    tree_project_scene_active_item = tree_project_active_item->child(index+1);  // 更新活动图层分支
    QString sName = tree_project_scene_active_item->text(0);  // 获取图层名称
    projectActive = getProjectByName(pName);  // 更新活动项目
    projectActive->setActiveScene(projectActive->getSceneByName(sName));  // 更新活动图层
    tree_project_active_item->child(index)->setSelected(false);
    tree_project_scene_active_item->setSelected(true);
    if(proPieceInfoMap.contains(pName)){  // 更新当前切割件ID
        proPieceInfoMap[pName]->curPieceID = index+1;
    }
    emit nestProjectChange(projectActive);  // 发送项目更新信息
}

void Nest::onActionLastPiece()
{
    qDebug() << "最后一个切割件";
    int index = tree_project_active_item->indexOfChild(tree_project_scene_active_item);  // 获取活动分支的索引
    int lastChildIndex = tree_project_active_item->childCount() - 1;  // 最后子节点的序号
    if(index == lastChildIndex){
        QMessageBox::warning(this, tr("警告"), tr("目前已经是最后一个切割件！"));
        return;
    }
    QString pName = tree_project_active_item->text(0);  // 获取项目名称
    tree_project_scene_active_item = tree_project_active_item->child(lastChildIndex);  // 更新活动图层分支
    QString sName = tree_project_scene_active_item->text(0);  // 获取图层名称
    projectActive = getProjectByName(pName);  // 更新活动项目
    projectActive->setActiveScene(projectActive->getSceneByName(sName));  // 更新活动图层
    tree_project_active_item->child(index)->setSelected(false);
    tree_project_scene_active_item->setSelected(true);
    if(proPieceInfoMap.contains(pName)){  // 更新当前切割件ID
        proPieceInfoMap[pName]->curPieceID = lastChildIndex;
    }
    emit nestProjectChange(projectActive);  // 发送项目更新信息
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
        // 判断名称是否重复
        foreach(Scene *scene, project->getSceneList()){
            if(text == scene->getName()){
                QMessageBox::warning(this, tr("警告"), tr("该图层已存在！"));
                return;
            }
        }
        tree_project_scene_active_item->setText(0, text);
        scene->setName(text);
        emit nestProjectChange(project);
    }
}

void Nest::onActionTreeProjectSceneDelete()
{
    QString pName = tree_project_active_item->text(0);  // 获得该节点的项目名称
    QString sName = tree_project_scene_active_item->text(0);  // 获得该节点的图层名称
    qDebug() << "删除：" << pName << "-" << sName;
    Project *project = getProjectByName(pName);  // 获取活动项目
    bool ret = project->removeSceneByName(sName);  // 删除该项目的图层
    if(!ret){  // 删除失败，返回
        QMessageBox::warning(this, tr("警告"), tr("删除失败！"));
        return;
    }
    QTreeWidgetItem *item = tree_project_scene_active_item;
    int index = tree_project_active_item->indexOfChild(item);  // 该分支的位置
    tree_project_active_item->removeChild(item);
    delete item;  // 释放内存
    item = NULL;
    qDebug() << "删除后长度：" << tree_project_active_item->childCount();

    // 更新项目-切割件信息
    if(proPieceInfoMap.contains(pName)){
        qDebug() << "更新：删去第"<< index << "个切割件";
        if(proPieceInfoMap[pName]->pieceList.length() > 0){
            Piece *piece = proPieceInfoMap[pName]->pieceList[index];
            proPieceInfoMap[pName]->pieceList.removeAt(index);
            delete piece;
            piece = NULL;
        }

        // 更新图层分支及当前的零件ID
        int childeCount = tree_project_active_item->childCount();  // 删除之后的子节点个数
        int lastIndex = childeCount - 1;  // 获取项目的最后图层分支的序号
        if(index > lastIndex){  // 如果删除的序号大于更新后的最后图层分支的序号，则将活动图层分支更新为最后一个
            tree_project_scene_active_item = tree_project_active_item->child(lastIndex);
            index = lastIndex;
        }
        proPieceInfoMap[pName]->curPieceID = index;
    }

    // 更新活动图层
    if(!project->getActiveScene() && project->getSceneList().length() > 0){
        project->setActiveScene(project->getScene(0));
    }

    emit nestProjectChange(project);
}

void Nest::onPieceNumChanged(const QString &num)
{
    if(projectList.length() == 0
            || tree_project_item_list.length() == 0){  // 如果项目列表为空或项目树为空，则返回
        return;
    }
    if(!tree_project_active_item){  // 如果活动项目树枝为空，则默认为第一个项目
        tree_project_active_item = tree_project->topLevelItem(0);
    }

    QString pName = tree_project_active_item->text(0);  // 获取项目名称
    Project *project = getProjectByName(pName);  // 获取项目指针
    if(project->getSceneList().length() == 0
            || tree_project_active_item->childCount() == 0){  // 如果该项目下图层为空或项目树子节点为空，则返回
        return;
    }

    if(tree_project_scene_active_item->parent()->text(0) != pName){
        tree_project_scene_active_item = tree_project_active_item->child(0);
    }

    qDebug() << pName << " " << tree_project_scene_active_item->text(0);
    if(tree_project_scene_active_item->text(0) == tr("切割件列表-空")){
        QMessageBox::warning(this, tr("错误"), tr("当前项目切割件列表为空！"));
        return;
    }
    int index = tree_project_active_item->indexOfChild(tree_project_scene_active_item);  // 获取图层节点的序号
    int n = num.toInt();
    if(proPieceInfoMap.contains(pName)){
        proPieceInfoMap[pName]->curPieceID = index;
        qDebug() << proPieceInfoMap[pName]->pieceList.length();
        proPieceInfoMap[pName]->pieceList[index]->setCount(n);
        qDebug() << proPieceInfoMap[pName]->pieceList[index]->getCount();
    }

    qDebug() << index << " " << n << " " << pName;
}

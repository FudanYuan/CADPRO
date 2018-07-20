#ifndef NEST_H
#define NEST_H

#include <QMainWindow>
#include <QActionGroup>
#include <QTreeWidget>
#include <QLabel>
#include <QProgressBar>
#include <QRectF>
#include <QVector>
#include <QMap>
#include <QThread>
#include "sheet.h"
#include "GA.h"
#include "sketchconfigure.h"
#include "project.h"
#include "view.h"
#include "scene.h"
#include "shape.h"
#include "binarytree.h"
#include "nestconfigure.h"
#include "nestengineconfiguredialog.h"
#include "piece.h"
#include <QDebug>
#include <QTimer>

namespace Ui {
class Nest;
}

class RectNestEngine;

// 排样界面
class Nest : public QMainWindow
{
    Q_OBJECT
public:
    // 图层类型
    enum SceneType{
        NestScene,  // 排版图层
        PieceScene  // 零件图层
    };

    // 零件
    struct Component
    {
        Component() :
            rect(QRectF()),
            count(0)
        {}

        Component(QRectF r, int n){
            rect = r;
            count = n;
        }
        Component(Polyline *p, int n){
            polyline = p;
            rect = p->boundingRect();  // 改成最小包络矩形
            count = n;
            qreal w = rect.width();
            qreal h = rect.height();
            qDebug() << "边缘矩形: " << w << ", " << h << ", " << n;
        }
        QRectF rect;  // 零件对应的最小包络矩形
        Polyline *polyline;  // 零件形状
        int count;  // 零件个数
    };

    // 项目-材料信息
    struct ProSheetInfo
    {
        ProSheetInfo(QString name, Sheet::SheetType type) :
            projectName(name),
            sheetType(type),
            curSheetID(-1)
        {

        }

        ~ProSheetInfo() {
            projectName = "";
            qDeleteAll(sheetList);
            sheetList.clear();
            pieceNumList.clear();
            usageList.clear();
            qDebug() << "delete proSheetMap";
        }

        QString projectName; // 项目名称
        Sheet::SheetType sheetType;  // 材料类型
        QList<Sheet*> sheetList;  // 材料列表
        QList<qreal> pieceNumList;  // 切割件数量
        QList<qreal> usageList;  // 材料使用率列表
        int curSheetID;  // 当前sheet的ID
    };

    // 项目-零件信息
    struct  ProPieceInfo {
        ProPieceInfo() :
            projectName(""),
            curPieceID(-1)
        {}

        ProPieceInfo(QString name, int curId) :
            projectName(name),
            curPieceID(curId)
        {}

        ~ProPieceInfo(){
            qDebug() << "delete ProPieceInfo";
            projectName = "";
            qDeleteAll(pieceList);
            pieceList.clear();
            curPieceID = -1;
        }

        void insertPiece(Piece *piece){
            pieceList.append(piece);
        }

        QString projectName;  // 项目名称
        QList<Piece*> pieceList;  // 零件列表
        int curPieceID;  // 当前零件的ID
    };

    explicit Nest(QWidget *parent = 0);
    ~Nest();

    void initAllPointers();  // 初始化指针
    void initActions();  // 初始化action
    void initMenuBar();  // 初始化菜单栏
    void initToolBar();  // 初始化工具栏
    void initStatusBar();  // 初始化状态栏
    void initDockWidget();  // 初始化dock widget
    void initProjectView();  // 初始化项目视图
    void updateProjectView();  // 更新项目视图
    void initPieceView();  // 初始化切割件视图
    void updatePieceView();  // 更新切割件视图
    void initSheetView();  // 初始化材料视图
    void updateSheetView();  // 更新材料视图
    void initNestView();  // 初始化排版视图
    void updateNestView();  // 更新排版视图
    void updateAll();  // 更新所有组件

    void initConfiguration();  // 初始化配置
    void initConnect();  // 初始化信号和槽的链接

    void addProject();  // 添加项目
    bool initSheet();  // 初始化材料

    bool initNestEngine();  // 初始化排版引擎
    void initRectNestEngine();  // 初始化矩形排版引擎
    void showNestResult();  // 显示排版结果

    QString getNewProjectName();  // 获取新项目名称
    Project *getProjectByName(QString project_name);  // 根据项目获取对象
    void removeProjectByName(QString project_name);  // 根据项目名称删除对象
    void showTreeMenu(QPoint pos);  // 显示项目栏菜单
    bool maybeSave();  // 是否保存项目
    bool saveFile(QString fileName);  // 实现文件的存储
    void setNestActionDisabled(bool flag);  // 使除能与Nest相关的action

    void setSceneStyle(Scene *scene, SceneType type, NestConfigure *config);  // 设置图层样式
private:
    QString fName;  // debug 文件名称
    int counter;  // debug 计数器
    QTimer *timer;  // debug
    qreal minArea;  // debug，最小面积
    qreal al;
    Ui::Nest *ui;
    NestConfigure *config;  // 排版配置
    View *nestView;  // 排版视图
    QList<Project *> projectList; // 项目列表
    Project *projectActive;  // 活动项目
    Scene *nestScene;  // 排版图层

    View *pieceView;  // 切割件视图
    Scene *pieceScene;   // 切割件图层
    Sheet* curSheet;  // 当前使用材料

    QMap<QString, int> nestNum; // 每个图形的个数
    QMap<QString, ProPieceInfo*> proPieceInfoMap;  // 零件列表<项目名称，零件列表>
    QMap<QString, ProSheetInfo*> proSheetInfoMap;  // 使用材料列表
    QMap<QString, QList<Scene *>> proSceneListMap;  // 保存对象<项目名称，图层列表>
    QMap<QString, QList<PieceCenter>> proPieceCenterMap; // 保存项目的零件中心图
    QMap<QString, QList<PieceOffset>> proPieceOffsetMap; // 保存项目的零件偏移图
    QMap<QString, NestEngineConfigure*> proNestEngineConfigMap;  // 排版引擎

    QThread *nestThread;  // 排版线程
    NestEngine *nestEngine;  // 排版引擎

    QWidget *widget;
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *firstPieceButton;
    QPushButton *previousPieceButton;
    QPushButton *nextPieceButton;
    QPushButton *lastPieceButton;

    QMenu *menu_file;  // 文件
    QAction *action_file_new;
    QAction *action_file_open;
    QAction *action_file_save;
    QAction *action_file_save_as;
    QAction *action_file_save_all;
    QAction *action_file_print;
    QAction *action_file_print_preview;
    QAction *action_file_print_setup;
    QAction *action_file_configuration;
    QAction *action_file_exit;

    QMenu *menu_edit;  // 编辑
    QAction *action_edit_undo;
    QAction *action_edit_redo;
    QAction *action_edit_clear;
    QAction *action_edit_delete;
    QAction *action_edit_cut;
    QAction *action_edit_copy;
    QAction *action_edit_paste;

    QMenu *menu_nest;  // 排版
    QAction *action_nest_start;  // 排版
    QAction *action_nest_config;
    QMenu *menu_action_nest_side;  // 排版靠边
    QAction *action_nest_side_left;
    QAction *action_nest_side_right;
    QAction *action_nest_side_top;
    QAction *action_nest_side_bottom;
    QMenu *menu_action_nest_direction;  // 排版方向
    QAction *action_nest_direction_horizontal;
    QAction *action_nest_direction_vertical;

    QMenu *menu_sheet;  // 表单，即材料
    QAction *action_sheet_manager;
    QAction *action_sheet_add;
    QAction *action_sheet_remove;
    QAction *action_sheet_duplicate;
    QAction *action_sheet_auto_duplicate;
    QAction *action_sheet_previous;
    QAction *action_sheet_next;
    QAction *action_sheet_sheet_number;
    QAction *action_sheet_use_last_sheet;
    QAction *action_sheet_sheet_property;

    QMenu *menu_cam;  // cam 配置
    QAction *action_cam_send_to_cut;
    QAction *action_cam_configure;

    QMenu *menu_utility;  // 测量
    QAction *action_utility_measure_distance;

    QMenu *menu_view;  // 视图
    QAction *action_view_grid;
    QAction *action_view_zoom_window;
    QAction *action_view_zoom_all;
    QAction *action_view_zoom_in;
    QAction *action_view_zoom_out;
    QAction *action_view_zoom_back;
    QAction *action_view_lock_layout;
    QMenu *menu_view_tool_bar;
    QActionGroup *action_view_tool_slide;
    QAction *action_view_tool_projects;
    QAction *action_view_tool_pieces;
    QAction *action_view_tool_sheets;
    QAction *action_view_tool_customize;
    QAction *action_view_status_bar;

    QMenu *menu_help;
    QAction *action_help_help;
    QAction *action_help_license;
    QAction *action_help_about;

    QToolBar *tool_standard;
    QToolBar *tool_view;
    QToolBar *tool_nest;
    QToolBar *tool_sheet;

    QProgressBar *nestProgressBar;  // 排版进度条
    QLabel *nestProgressLabel;  // 排版进度
    QLabel *mousePositionLabel;  //  鼠标坐标
    CustomDockWidget *dock_piece;  // 浮动窗口1
    CustomDockWidget *dock_project;  // 浮动窗口2
    CustomDockWidget *dock_sheet;  // 浮动窗口3
    CustomDockWidget *dock_nest;   // 主窗口

    QTreeWidget *tree_project;       // 项目树
    QList<QTreeWidgetItem *> tree_project_item_list; // 项目树列表
    QTreeWidgetItem *tree_project_active_item; // 弹出菜单属于的那个项目
    QTreeWidgetItem *tree_project_scene_active_item; // 弹出菜单属于的那个项目的图层

    QTreeWidget *tree_sheet;       // 材料树
    QList<QTreeWidgetItem *> tree_sheet_item_list; // 材料树列表

    QMenu *menu_tree;
    QAction *action_tree_expand_all;
    QAction *action_tree_fold_all;

    QMenu *menu_tree_project;
    QAction *action_tree_project_nest_scene;
    QAction *action_tree_project_add_scene;
    QAction *action_tree_project_save;
    QAction *action_tree_project_save_as;
    QAction *action_tree_project_rename;
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

signals:
    void nestConfigChanged(QString name, QVariant value);
    void nestEngineConfigChange(int i);
    void nestStart();  // 开始排版
    void nestProjectChange(Project *curProject);  // 排版项目改变信号

public slots:
    void onMousePositionChanged(QPointF pos);  // 鼠标位置更新
    void onNestProjectChanged(Project *curProject);  // 响应排版项目改变
    void onProjectNameChanged(QString lastName, QString presentName);  // 响应项目名称改变
    void onNestProgressChanged(int i);  // 响应排版进度变化
    void onNestFinished(QVector<NestEngine::NestPiece> nestPieceList);  // 响应排版结束
    void onNestInterrupted(int remainNum);  // 响应排版中断
    void onAutoRepeatedLastSheet(Sheet sheet);  // 响应排版自动重复了最后一张材料
    void onNestThreadFinished();

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

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
    void onActionFileConfiguration();   // 系统配置操作
    void onActionFileExit();            // 退出提示保存

    void onActionEditUndo();            // 撤销上个操作
    void onActionEditRedo();            // 重做撤销操作
    void onActionEditClear();           // 清空
    void onActionEditDelete();          // 删除
    void onActionEditCut();             // 剪切
    void onActionEditCopy();            // 复制
    void onActionEditPaste();           // 粘贴

    void onActionNestStart();           // 开始排版
    void onActionNestEngineConfig();          // 自动排版配置
    void onActionNestSideLeft();        // 左靠边
    void onActionNestSideRight();       // 右靠边
    void onActionNestSideTop();         // 顶靠边
    void onActionNestSideBottom();      // 底靠边
    void onActionNestDirectionHorizontal(); // 横向排版
    void onActionNestSideDirectionVertical(); // 纵向排版

    void onActionSheetManager();        // 材料管理
    void onActionSheetAdd();            // 增加材料
    void onActionSheetRemove();         // 删除材料
    void onActionSheetDuplicate();      // 重复材料
    void onActionSheetAutoDuplicate();  // 自动重复材料
    void onActionSheetPrevious();       // 上一张材料
    void onActionSheetNext();           // 下一张材料
    void onActionSheetSheetNumber();    // 输入材料序号
    void onActionSheetUseLastSheet();   // 使用上一张材料
    void onActionSheetProperty();       // 材料属性

    void onActionViewGrid(bool toggled);
    void onActionViewZoomWindow();  // 选择区域放大
    void onActionViewZoomAll();     // 全部图形居中最大
    void onActionViewZoomIn();      // 放大
    void onActionViewZoomOut();     // 缩小
    void onActionViewZoomBack();    // 缩小
    void onActionViewLockLayout(bool toggled);
    void onActionViewToolProjectsToggled(bool toggled);
    void onActionViewToolPiecesToggled(bool toggled);
    void onActionViewToolSheetsToggled(bool toggled);
    void onActionViewToolSlideToggled(bool toggled);
    void onActionViewToolCustomizeToggled(bool toggled);
    void onActionViewStatusBar(bool toggled);

    void onActionUtilityMeasureDis();        	//测量距离

    // HELP
    void onActionHelpHelp();             //帮助
    void onActionHelpLicense();          //许可
    void onActionHelpAbout();            //关于
    // HELP

    void onActionFirstPiece();  // 第一张切割件
    void onActionPreviousPiece();  // 上一张切割件
    void onActionNextPiece();  // 下一张切割件
    void onActionLastPiece();  // 最后一张切割件

    void onActionTreeExpandAll();
    void onActionTreeFoldAll();

    void onProjectTreeItemClicked(QTreeWidgetItem *item, int column);  // 响应项目树选中
    void onTreeProjectItemDoubleClicked(QTreeWidgetItem *item);
    void onActionTreeProjectNestScene();  // 排版
    void onActionTreeProjectAddScene(); // 项目添加图层
    void onActionTreeProjectSave();
    void onActionTreeProjectSaveAs();
    void onActionTreeProjectRename();
    void onActionTreeProjectClose();

    void onActionTreeProjectSceneChangeTo();
    void onActionTreeProjectSceneRename();
    void onActionTreeProjectSceneDelete();

    void onPieceNumChanged(const QString &num);

    void onDockNestSizeChanged();
    void onDockPieceSizeChanged();
};

#endif // NEST_H

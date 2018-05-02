#ifndef NEST_H
#define NEST_H

#include <QMainWindow>
#include <QActionGroup>
#include <QTreeWidget>
#include <QLabel>
#include <QRectF>
#include <QVector>
#include <QMap>
#include "sheet.h"
#include "GA.h"
#include "configure.h"
#include "project.h"
#include "view.h"
#include "scene.h"
#include "shape.h"
#include "binarytree.h"
#include <QDebug>

namespace Ui {
class Nest;
}

class RectNestEngine;

// 排样界面
class Nest : public QMainWindow
{
    Q_OBJECT
public:
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

    struct ProSheetMap
    {
        ProSheetMap(QString name) :
            projectName(name)
        {

        }
        QString projectName; // 项目名称
        QList<Sheet*> sheetList;  // 材料列表
        QList<qreal> pieceNumList;  // 切割件数量
        QList<qreal> usageList;  // 材料使用率列表
    };

    explicit Nest(QWidget *parent = 0);
    ~Nest();

    void initActions();  // 初始化action
    void initMenuBar();  // 初始化菜单栏
    void initToolBar();  // 初始化工具栏
    void initStatusBar();  // 初始化状态栏
    void initDockWidget();  // 初始化dock widget
    void initConfiguration();  // 初始化配置
    void initNestView();  // 初始化排版视图
    void updateNestView();  // 更新排版视图
    void initProjectView();  // 初始化项目视图
    void initPieceView();  // 初始化切割件视图
    void addProject();  // 添加项目
    void initSheet();  // 初始化材料
    void updateSheetTree();  // 更新材料树
    void initRectNestEngine();  // 初始化矩形排版引擎
    void showNestResult();  // 显示排版结果
    QString getNewProjectName();  // 获取新项目名称
    Project *getProjectByName(QString project_name);  // 根据项目获取对象

    void showTreeMenu(QPoint pos);  // 显示项目栏菜单
    void updateAll();  // 更新整个软件
    bool maybeSave();  // 是否保存项目
    bool saveFile(QString fileName);  // 实现文件的存储

private:
    Ui::Nest *ui;
    View *nestView;  // 排版视图
    QList<Project *> projectList; // 项目列表
    Project *projectActive;  // 活动项目
    Scene *nestScene;  // 排版图层
    QMap<QString, QList<Scene *>> outMap;  // 保存对象<项目名称，图层列表>
    QMap<QString, QList<PieceCenter>> pieceCenterMap; // 保存项目的零件中心图
    QMap<QString, QList<PieceOffset>> pieceOffsetMap; // 保存项目的零件中心图

    QMap<QString, int> nestNum; // 每个图形的个数
    View *pieceView;  // 切割件视图
    Scene *pieceScene;   // 切割件图层
    QMap<QString, ProSheetMap*> proSheetMap;  // 使用材料列表
    Sheet* curSheet;  // 当前使用材料

    QWidget *widget;
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *lastButton;
    QPushButton *nestButton;

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

    QMenu *menu_cam;  // 测量
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
    QAction *action_view_tool_pieces;
    QAction *action_view_tool_sheets;
    QAction *action_view_tool_customize;
    QAction *action_view_status_bar;

    QLabel *mousePositionLabel;  //  鼠标坐标
    QDockWidget *dock_piece;  // 浮动窗口1
    QDockWidget *dock_project;  // 浮动窗口2
    QDockWidget *dock_sheet;  // 浮动窗口3
    QDockWidget *dock_nest;   // 主窗口

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

public slots:
    void onProjectNameChanged(QString lastName, QString presentName);  // 响应项目名称改变
    void onMousePositionChanged(QPointF pos);  // 鼠标位置更新

protected:
    void closeEvent(QCloseEvent *event);

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

    void onActionTreeExpandAll();
    void onActionTreeFoldAll();

    void onTreeProjectItemDoubleClicked(QTreeWidgetItem *item);
    void onActionTreeProjectNestScene();  // 排版
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

    void onPieceNumChanged(const QString &num);
};

#endif // NEST_H

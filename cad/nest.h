#ifndef NEST_H
#define NEST_H

#include <QMainWindow>
#include <QActionGroup>
#include <QTreeWidget>
#include <QLabel>
#include <QRectF>
#include <QVector>
#include <QMap>
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
    // 材料
    struct Material
    {
        enum MaterialType{
            Whole = 1,
            Strip = 2
        };
        Material() :
            name(""),
            type(Whole),
            constitute(""),
            width(1000),
            height(INT_MAX),
            area(INT_MAX),
            componentGap(0),
            topMargin(0),
            rightMargin(0),
            bottomMargin(0),
            leftMargin(0),
            layers(1),
            margin(0),
            doubleStrip(false),
            cutPaneSize(INT_MAX),
            layoutRect(QRectF(leftMargin, topMargin,
                        (width - leftMargin - rightMargin),
                        (height - topMargin - bottomMargin)))
        {}
        QString name;  // 材料名称
        MaterialType type;  // 材料类型
        QString constitute; // 材料构成，非必填

        qreal width; // 宽度
        qreal height; // 长度
        qreal area;  // 面积

        qreal componentGap; // 零件之间的间距
        qreal topMargin; // 与上边框的间距
        qreal rightMargin; // 与右边框的间距
        qreal bottomMargin; // 与下边框的间距
        qreal leftMargin; // 与左边框的间距

        int layers;  // 切割层数

        // 以下在type为Strip时使用
        qreal margin;  // 上插补强边距余量
        bool doubleStrip;  // 双上插条板
        double cutPaneSize;  // 切割平面尺寸

        // 材料的有效区域
        QRectF layoutRect;
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
            rect = p->boundingRect();
            count = n;
            qreal w = rect.width();
            qreal h = rect.height();
            qDebug() << "边缘矩形: " << w << ", " << h << ", " << n;
        }
        QRectF rect;  // 零件对应的最小矩形
        Polyline *polyline;  // 零件原形状
        int count;  // 零件个数
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
    void initMaterial();  // 初始化材料
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
    QMap<QString, int> nestNum; // 每个图形的个数
    View *pieceView;  // 切割件视图
    Scene *pieceScene;   // 切割件图层
    Material material;  // 当前使用材料

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

// 矩形排版引擎
// BL -> TL
class RectNestEngine{
public:
    // 最小矩形件
    struct MinRect
    {
        MinRect() :
            page(0),
            position(QPointF(0, 0)),
            width(0),
            height(0),
            area(0.0),
            ratio(0.0),
            index(-1),
            rotate(false)
        {}

        MinRect(qreal w, qreal h, int i, bool r, int id) :
            position(QPointF(0, 0))
        {
            width = w;
            height = h;
            area = w * h;
            ratio = qMax(h/w, w/h);
            index = i;
            rotate = r;
            componentId = id;
        }

        QLineF upper(){
            double w = width;
            double h = height;
            if(rotate){
                double temp = w;
                w = h;
                h = temp;
            }
            return QLineF(position + QPointF(0, h), position + QPointF(w, h));
        }

        QLineF lower(){
            double w = width;
            double h = height;
            if(rotate){
                double temp = w;
                w = h;
                h = temp;
            }
            return QLineF(position, position + QPointF(w, 0));
        }

        QRectF rect(){
            double w = width;
            double h = height;
            if(rotate){
                double temp = w;
                w = h;
                h = temp;
            }
            return QRectF(position.rx(), position.ry(), w, h);
        }

        qreal getWidth(){
            double w = width;
            if(rotate){
                w = height;
            }
            return w;
        }

        qreal getHeight(){
            double h = height;
            if(rotate){
                h = width;
            }
            return h;
        }

        // 设置旋转
        void setRotate(bool r) {
            rotate = r;
        }

        // 交换位置
        void exchange(MinRect &rect){
            QPointF pos = rect.position;
            rect.position = position;
            position = pos;
        }

        // 设置对应切割件的id
        void setComponentId(int id){
            componentId = id;
        }

        // 获取对应切割件的id
        int getComponentId(){
            return componentId;
        }
        int page;  // 页数
        QPointF position;  // 位置，指的是右上角坐标
        qreal width;  // 宽
        qreal height;  // 高
        qreal area;  // 面积
        qreal ratio;  // 长宽比
        int index;  // 矩形序号
        bool rotate;  // 是否旋转90*，统一逆时针

        int componentId;  // 对应的切割件的id
    };

    // 空白区域
    struct EmptyRectArea
    {
        EmptyRectArea() :
            px1(0),
            py1(0),
            px2(0),
            py2(0)
        {}

        EmptyRectArea(QRectF rect) :
            px1(rect.bottomLeft().rx()),
            py1(rect.bottomLeft().ry()),
            px2(rect.topRight().rx()),
            py2(rect.topRight().ry())
        {}

        EmptyRectArea(MinRect rect) :
            px1(rect.position.rx()),
            py1(rect.position.ry())
        {
            px2 = rect.position.rx() + (rect.rotate ? rect.height : rect.width);
            py2 = rect.position.ry() + (rect.rotate ? rect.width : rect.height);
        }

        EmptyRectArea(QPointF lb, QPointF rt) :
            px1(lb.rx()),
            py1(lb.ry()),
            px2(rt.rx()),
            py2(rt.ry())
        {}

        EmptyRectArea(QPointF p, qreal w, qreal h) :
            px1(p.rx()),
            py1(p.ry()),
            px2(p.rx()+w),
            py2(p.ry()+h)
        {}

        EmptyRectArea(qreal x1, qreal y1, qreal x2, qreal y2) :
            px1(x1),
            py1(y1),
            px2(x2),
            py2(y2)
        {}

        qreal px1, py1, px2, py2;   // 顶点坐标

        // 左下角的点
        QPointF  leftBottom()
        {
            return QPointF(px1, py1);
        }

        // 右上角的点
        QPointF  rightTop()
        {
            return QPointF(px2, py2);
        }

        // 宽度
        qreal width(){
            return px2 - px1;
        }

        // 高度
        qreal height(){
            return py2 - py1;
        }

        // 面积
        qreal area(){
            return (px2-px1)*(py2-py1);
        }

        // 是否相交（包含）
        bool intersect(const EmptyRectArea rect, EmptyRectArea &interRect){
            qreal left = 0;
            qreal bottom = 0;
            qreal right = 0;
            qreal top = 0;

            // 计算两矩形可能相交矩形的边界
            left = px1 >= rect.px1 ? px1 : rect.px1;
            bottom = py1 >= rect.py1 ? py1 : rect.py1;
            right = px2 <= rect.px2 ? px2 : rect.px2;
            top = py2 <= rect.py2 ? py2 : rect.py2;
            // 判断是否相交
            if (left > right || bottom > top)
            {
                return false;
            }
            else
            {
                interRect.px1 = left;
                interRect.py1 = bottom;
                interRect.px2 = right;
                interRect.py2 = top;
                return true;
            }
        }

        // 是否包含（不包括相等）
        bool contains(const EmptyRectArea rect){
            if(*this == rect){
                return false;
            }
            if(px1 <= rect.px1 && px2 >= rect.px2 &&
                    py1 <= rect.py1 && py2 >= rect.py2){
                return true;
            }
            else
            {
                return false;
            }
        }

        // 做差集
        QList<EmptyRectArea> subtraction(const EmptyRectArea rect){
            QList<EmptyRectArea> res;
            EmptyRectArea intersectRect; // 相交矩形
            // 如果两矩形不相交 或 相邻，则返回原矩形
            if(!intersect(rect, intersectRect) || intersectRect.area() == 0){
                res.append(*this);
                return res;
            }
            // 如果矩形包含 或 等于 被减矩形，则返回为空
            EmptyRectArea r = rect;
            if(r.contains(*this) || r == *this){
                return res;
            }

            EmptyRectArea rect1(px1, py1, px2, intersectRect.py1);
            if(rect1.area() != 0) res.append(rect1);

            EmptyRectArea rect2(px1, py1, intersectRect.px1, py2);
            if(rect2.area() != 0) res.append(rect2);

            EmptyRectArea rect3(px1, intersectRect.py2, px2, py2);
            if(rect3.area() != 0) res.append(rect3);

            EmptyRectArea rect4(intersectRect.px2, py1, px2, py2);
            if(rect4.area() != 0) res.append(rect4);
#ifdef NESTDEBUG
            qDebug() << "差集为：" << res.length();
            for(int i=0;i<res.length();i++){
                qDebug() << res[i].leftBottom() << "   " << res[i].rightTop();
            }
#endif
            return res;
        }
        // 运算符重载
        friend bool operator>(const EmptyRectArea &rect1 , const EmptyRectArea &rect2){
            if(rect1.py1 > rect2.py1){
                return true;
            }
            if(rect1.py1 == rect2.py1){
                if(rect1.px1 > rect2.px1){
                    return true;
                }
            }
            return false;
        }
        friend bool operator>=(const EmptyRectArea &rect1 , const EmptyRectArea &rect2){
            if(rect1.py1 >= rect2.py1){
                return true;
            }
            if(rect1.py1 == rect2.py1){
                if(rect1.px1 >= rect2.px1){
                    return true;
                }
            }
            return false;
        }
        friend bool operator==(const EmptyRectArea &rect1 , const EmptyRectArea &rect2){
            if(rect1.px1 == rect2.px1 &&
                    rect1.py1 == rect2.py1 &&
                    rect1.px2 == rect2.px2 &&
                    rect1.py2 == rect2.py2){
                return true;
            }
            return false;
        }
        friend bool operator!=(const EmptyRectArea &rect1 , const EmptyRectArea &rect2){
            if(rect1.px1 == rect2.px1 &&
                    rect1.py1 == rect2.py1 &&
                    rect1.px2 == rect2.px2 &&
                    rect1.py2 == rect2.py2){
                return false;
            }
            return true;
        }
        friend bool operator<(const EmptyRectArea &rect1 , const EmptyRectArea &rect2){
            if(rect1.py1 < rect2.py1){
                return true;
            }
            if(rect1.py1 == rect2.py1){
                if(rect1.px1 < rect2.px1){
                    return true;
                }
            }
            return false;
        }
        friend bool operator<=(const EmptyRectArea &rect1 , const EmptyRectArea &rect2){
            if(rect1.py1 <= rect2.py1){
                return true;
            }
            if(rect1.py1 == rect2.py1){
                if(rect1.px1 <= rect2.px1){
                    return true;
                }
            }
            return false;
        }
    };

    friend class Nest;
    RectNestEngine();
    ~RectNestEngine();

    static void quickSort(QList<EmptyRectArea> &list, int l, int r);
    static void updateEmptyRectArea(EmptyRectArea *eRect);  // 更新空白矩形区域
    static void resetAllParameter();  // 重置参数
    static double fitnessFunc(Genome &g);  // 适应度函数,根据“高度调整填充法”计算
    static void layoutAlg(QVector<double> gVector);  // 排版算法
    static double calculateScore();  // 计算评分

private:
    static QList<Nest::Component> components;  // 零件
    static QList<MinRect> compMinRects;  // 零件的最小矩形
    static QList<EmptyRectArea> emptyRectArea;  // 空白矩形

    static double mWidth;  // 材料宽度
    static double mHeight;  // 材料高度
    static double maxHight;  // 最大高度值
    static long allRectsArea; // 矩形零件面积
    static long minRectsArea; // 最小矩形零件面积
};

#endif // NEST_H

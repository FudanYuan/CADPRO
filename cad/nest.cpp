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
#include <QDebug>

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
    initMaterial();  // 初始化材料
    initNestView();  // 初始化排版视图
    addProject();   // 添加项目
    initStatusBar();    // 初始化状态栏
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
}

void Nest::initToolBar()
{

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

    // 该部分要移至材料选择结束后
    nestScene = new Scene(nestView);
    nestScene->setSceneRect(-10, -10,
                            material.layoutRect.width(),
                            material.layoutRect.height());

    // 画出边缘
    Rect *rect = new Rect;
    Configure::PenStyle pen;
    pen.setPenStyle(Qt::black, Qt::SolidLine, 2);
    rect->setPenStyle(pen);
    rect->setRect(material.layoutRect);
    nestScene->addCustomRectItem(rect);

    nestView->setScene(nestScene);
    nestView->centerOn(nestView->mapFromScene(0,0));
}

void Nest::updateNestView()
{
    nestScene = new Scene(nestView);
    nestScene->setSceneRect(-10, -10,
                            material.layoutRect.width(),
                            material.layoutRect.height());

    // 画出边缘
    Rect *rect = new Rect;
    Configure::PenStyle pen;
    pen.setPenStyle(Qt::black, Qt::SolidLine, 2);
    rect->setPenStyle(pen);
    rect->setRect(material.layoutRect);
    nestScene->addCustomRectItem(rect);

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
//    tree_project->expandAll(); //结点全部展开
    updateAll();
}

void Nest::initMaterial()
{
    material.leftMargin = 2;
    material.topMargin = 2;
    material.rightMargin = 2;
}

void Nest::initRectNestEngine()
{
    // 首先构造10个矩形对象作为零件，
    // 后期要通过可视化进行添加
    // 即通过界面化操作将图形元素或个数添加入零件列表
//    for(int i = 0; i < COUNT; i++){
//        int w = GA::randT<int>(10, 100);
//        int h = GA::randT<int>(10, 100);
//        int n = GA::randT<int>(1, 5);
//        RectNestEngine::components.append(Component(QRectF(0, 0, w, h),
//                                                   n));
//    }

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
    QRectF r = material.layoutRect;
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

    for(int i=0;i<g.getFittestGenome().getGenome().length();i++){
        int index = qAbs(g.getFittestGenome().getGenome()[i]) - 1;  // 得到矩形的序号
        RectNestEngine::MinRect *currentRect = &RectNestEngine::compMinRects[index]; // 得到矩形指针
        bool rotate = g.getFittestGenome().getGenome()[i] < 0; // 如果基因为负值，则需要旋转90*
        currentRect->setRotate(rotate);
        int id = currentRect->getComponentId();
        QPointF cp = currentRect->position;
        qDebug() << "位置: "<< cp;

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
        QList<QPointF> pList;
        for(int j=0;j<polyline->getPoints().length();j++){
            QPointF oldPoint = polyline->getPoints()[j];
            QPointF newPoint = oldPoint-offset+cp;
            pList.append(newPoint);
        }
        Polyline *p = new Polyline();
        Configure::PenStyle pen;
        pen.setPenStyle(Qt::red, Qt::SolidLine, 1);
        p->setPenStyle(pen);
        p->setPolyline(pList, polyline->getType(), polyline->getElevation(), rotate ? 90 : 0);
        nestScene->addCustomPolylineItem(p);
    }
    nestScene->setMoveable(true);
    nestView->setScene(nestScene);
    QString name = projectActive->getName();
    if(outMap.contains(name)){
        outMap[name].append(nestScene);
    } else{
        QList<Scene *> sList;
        sList.append(nestScene);
        outMap.insert(name, sList);
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
    if(!projectActive){
        setWindowTitle("CADPRO-<空项目>");
        dock_piece->setWindowTitle("<空项目>");
        widget->setDisabled(true);
        lineEdit->setText("1");

        tree_project_active_item = NULL;
        tree_project_scene_active_item = NULL;
        pieceView->setScene(NULL);
        lineEdit->setText("1");

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
    if(outMap.contains(pName)){
        qDebug() << "存在" << outMap[pName].length();
        if(outMap[pName].length() != 0){
            nestScene = outMap[pName][0];
            nestView->setScene(nestScene);
        } else{
            updateNestView();
        }
    } else{
        qDebug() << "不存在";
        updateNestView();
    }

    // 更新窗口名称
    setWindowTitle("CADPRO-<" + pName + ">");
    dock_piece->setWindowTitle(pName + "-" + sName);

    // 在视图中更新图层
    //connect(nestView, &View::viewScaleChanged, nestScene, &Scene::onViewScaleChanged);
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
        Project *pro = new Project(this);
        if(!outMap.contains(pName)){
            QMessageBox::warning(this, tr("错误"), tr("该项目还未进行排版!"));
            return false;
        }
        QList<Scene *> sList = outMap[pName];
        for(int i=0; i<sList.length(); i++){
            pro->addScene(sList[i]);
        }
        pro->dxfFileWriter(fileName);

    } catch(QString exception){
        QMessageBox::warning(this, tr("错误"), exception);
        return false;
    }

    return true;
}

void Nest::onProjectNameChanged(QString lastName, QString presentName)
{
    for(int i = 0;i<tree_project_item_list.length();i++){
        if(tree_project_item_list.at(i)->text(0) == lastName){
            tree_project_item_list.at(i)->setText(0, presentName);
            break;
        }
    }
}

void Nest::onMousePositionChanged(QPointF pos)
{
    mousePositionLabel->setText(tr("X=") + QString::number(pos.rx()) + " Y=" + QString::number(-pos.ry()));
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
                                                    tr("export.dxf"));
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
    QTreeWidgetItem *parent=item->parent();//获得父节点
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
    updateAll();
}

void Nest::onActionTreeProjectNestScene()
{
    if(nestNum.count() == 0){
        QMessageBox::warning(this, tr("警告"), tr("未设置切割件排版个数!"));
        return;
    }
    RectNestEngine::components.clear();
    RectNestEngine::compMinRects.clear();
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
        qDebug() << "count= " << count;
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
    QString fileName ;//= QFileDialog::getOpenFileName(this, tr("打开DXF文件"), QDir::currentPath());
    fileName = "/Users/Jeremy/Desktop/项目/梁叔项目/画图+排版/素材/单个.dxf";
    if (!fileName.isEmpty()) {
        if(!projectActive){
            QString name_project_new = getNewProjectName();
            projectActive = new Project(this);
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
            updateAll();
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
        tree_project_active_item->setText(0, text);
        project->changeName(text);
        updateAll();
    }
}

void Nest::onActionTreeProjectClose()
{
    qDebug() << tree_project_active_item->text(0) << "关闭项目";

    QString pName = tree_project_active_item->text(0);

    // 关闭project
    projectList.removeOne(getProjectByName(tree_project_active_item->text(0)));

    // 先删除子树
    for(int i=0;i<tree_project_active_item->childCount();i++){
        QString sName = tree_project_active_item->child(i)->text(0);
        QString key = pName + "_" + sName;
        if(nestNum.contains(key)){
            nestNum.remove(key);
        }
        tree_project_active_item->removeChild(tree_project_active_item->child(i));
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
    updateAll();
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
        updateAll();
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


/*
 * RectNestEngine: 矩形排版引擎
*/
RectNestEngine::RectNestEngine()
{
}

RectNestEngine::~RectNestEngine()
{

}

void RectNestEngine::quickSort(QList<RectNestEngine::EmptyRectArea> &s, int l, int r)
{
    if (l< r)
    {
        int i = l, j = r;
        EmptyRectArea x = s[l];
        while (i < j)
        {
            while(i < j && s[j]> x) // 从右向左找第一个小于x的数
                j--;
            if(i < j)
                s[i++] = s[j];
            while(i < j && s[i]< x) // 从左向右找第一个大于等于x的数
                i++;
            if(i < j)
                s[j--] = s[i];
        }
        s[i] = x;
        quickSort(s, l, i - 1); // 递归调用
        quickSort(s, i + 1, r);
    }
}

void RectNestEngine::updateEmptyRectArea(RectNestEngine::EmptyRectArea *eRect)
{
    // 遍历整个二叉树，做差集运算
    // 如果二叉树根节点为空，则根节点为插入值
    // 否则，二叉树各节点减去该矩形元素

    // 最后，对该树进行整理：
    // 去掉面积为零的或已无法排下所剩的任何一个矩形件的剩余矩形；
    // 把具有完全包含关系的剩余矩形中面积小的矩形去除、有相交关系的矩形全部保留。
    if(emptyRectArea.length() == 0){
        emptyRectArea.append(*eRect);
        return;
    }

    QList<EmptyRectArea> newList;  // 保存作差之后新生成的矩形
    for(int i=0;i<emptyRectArea.length();i++){
        // 每个矩形都减去该矩形集合
        QList<EmptyRectArea> area = emptyRectArea[i].subtraction(*eRect);
        int num = area.length();

        // 若两矩形相交，则最多产生四个新矩形
        // 通过与生成的矩形列表比较，判断该矩形是否需要加入新生成矩形列表
        for(int j=0; j<num; j++){
            // 若生成矩形的面积小于矩形件中的最小面积，直接忽略
            if(area[j].area() < minRectsArea){
                continue;
            }
            int len = newList.length();
            bool continueFlag = false;
            for(int m=0; m<len; m++){
                // 如果新生成的矩形中包含该生成的矩形，则不加入新生成矩形列表
                if(newList[m].contains(area[j])){
#ifdef NESTDEBUG
                    qDebug() << "----- start";
                    qDebug() << newList[m].leftBottom() << "  " << newList[m].rightTop();
                    qDebug() << area[j].leftBottom() << "  " << area[j].rightTop();
                    qDebug() << "新生成的矩形中包含该生成的矩形，则不加入新生成矩形列表";
                    qDebug() << "----- end";
#endif
                    continueFlag = true;
                    break;
                }
                // 如果该生成的矩形中包含新生成的矩形，则将原矩形替换
                if(area[j].contains(newList[m])){
#ifdef NESTDEBUG
                    qDebug() << "----- start";
                    qDebug() << newList[m].leftBottom() << "  " << newList[m].rightTop();
                    qDebug() << "该生成的矩形中包含新生成的矩形，则将原矩形替换";
                    qDebug() << area[j].leftBottom() << "  " << area[j].rightTop();
                    qDebug() << "----- end";
#endif
                    newList[m] = area[j];
                    continueFlag = true;
                    break;
                }
                // 如果两矩形相等，则不加入新生成矩形列表
                if(area[j] == newList[m]){
#ifdef NESTDEBUG
                    qDebug() << newList[m].leftBottom() << "  " << newList[m].rightTop();
                    qDebug() << area[j].leftBottom() << "  " << area[j].rightTop();
                    qDebug() << "两矩形相等，则不加入新生成矩形列表";
#endif
                    continueFlag = true;
                    break;
                }
            }

            // 如果矩形不需要加入，继续下次循环
            if(continueFlag){
                continue;
            }
            // 直接添加该区域
            newList.append(area[j]);
        }
    }

    // 更新剩余矩形集合
    emptyRectArea.clear();
    emptyRectArea.append(newList);

    int len = emptyRectArea.length();
    quickSort(emptyRectArea,0,len-1);

#ifdef NESTDEBUG
    qDebug() << "-------------空白矩形集合------------- start";
    for(int i=0;i<len;i++){
        qDebug() << i << "  " <<  emptyRectArea[i].leftBottom() << emptyRectArea[i].rightTop();
    }
    qDebug() << "-------------空白矩形集合------------- end";
#endif
}

void RectNestEngine::resetAllParameter()
{
    // 重置矩形集合及最大高度
    maxHight = 0;
    emptyRectArea.clear();
    EmptyRectArea a(0,0, mWidth, mHeight);
    updateEmptyRectArea(&a);
}

double RectNestEngine::fitnessFunc(Genome &g)
{
    // 根据“高度调整填充法”计算适应度
    //!
    //! 第二个是放入的矩形面积占待放入的矩形面积的百分比，
    //! 第二个是当前排入的矩形的最大高度，
    //! 第三个是排入矩形的整齐程度（高的种类个数）
    //!
    // 对基因进行解码
    QVector<double> gVector = g.getGenome();

    // 重置所有参数
    resetAllParameter();

    // 排版算法
    layoutAlg(gVector);

    // 计算适应度评分
    g.setFitness(calculateScore());

    return g.getFitness();
}

void RectNestEngine::layoutAlg(QVector<double> gVector)
{
    QList<MinRect> layRects;  // 存放已排矩形
    for(int i=0;i<gVector.length();i++){
        // 解码
        int index = qAbs(gVector[i]) - 1;  // 得到矩形的序号
        MinRect *currentRect = &RectNestEngine::compMinRects[index]; // 得到矩形指针
        currentRect->setRotate(gVector[i] < 0);  // 如果基因为负值，则需要旋转90*
        double rectWidth = currentRect->getWidth();  // 得到矩形宽度
        double rectHight = currentRect->getHeight();  // 得到矩形长度

        // 下面对该零件进行排版，即确定每个矩形的位置
        /*
        //!
        //! "高度调整法"算法描述：
        //! Step1: 设置初始最高轮廓线为板材的最上边的边。
        //! Step2: 每当要排入一个零件时，在最高轮廓线集中选取最低的一段水平线，
        //! 如有数段，则选取选取最左边的一段，
        //! 测试该线段的宽度是否大于或等于要排入零件的宽度：
        //!     a. 如果该线段的宽度大于排入零件的宽度，
        //! 则将该零件在此位置排放，零件排放之后，测试整体排样高度的变化。
        //! 如果整体排样高度增加（甚至超出了板材边界），
        //! 那么就从前面已排的零件中寻找宽度适合（零件宽度和排放零件处的
        //! 线段的宽度均适合）的零件：
        //!     如果没有，则按原先排样序列进行排样，更新零件最高轮廓线后
        //! 转到Step2排放下一个零件；
        //!     如有数个，则先选取最左边的一个，相互交换排样位置，
        //! 并测试交换前后的整体排样高度。然后依次选取另外几个零件，进行相同操作；
        //!     a1: 如果其中部分交换操作降低了整体排样高度，
        //! 则选取整体排样高度最低的进行交换，并更新排样序列和零件的最高轮廓线。
        //! 而其他的零件排样位置保持不变。
        //!     a2: 如果都没有降低整体排样高度，则不交换排样位置。
        //! 按原排样序列进行排样。更新零件最高轮廓线。
        //!     b. 否则，查询与最低水平线相邻的左右两段水平线，
        //! 将最低水平线提升与高度较低的一段平齐，更新零件最高轮廓线。
        //!
        //! 重复Step2，直至能排入该零件。
        //! 重复上述过程，直至所有零件排放完毕。
        //!
        //
        */
        // 寻找最低水平线集合
        /*
//        BBSTNode<BaseLine *> *minNode = highestProfileLine.findMinAVL(highestProfileLine.root);
//        double currentMaxHight = maxHight;
//        bool lay = false;  // 排放标识
//        while(minNode){
//            bool breakFlag = false;
//            BaseLine *lowestLine = minNode->data;
//            ////qDebug() << "最低水平线：" << lowestLine->line() << "  width: " << lowestLine->width() << "  最大高度: " << maxHight;
//            // 遍历最低水平线集合
//            while(lowestLine){
//                if(lowestLine->width() >= rectWidth){
//                    lay = true;  // 更新排放标识，表明已排放
//                    // 如果该线段的宽度大于排入零件的宽度，则可排放该零件
//                    // 将该零件排放在此位置
//                    currentRect->position = lowestLine->left();

//                    // 计算排放后的最大排样高度
//                    currentMaxHight = qMax(maxHight,
//                                           lowestLine->left().ry() + rectHight);
//                    //qDebug() << "存在一个位置排放该矩形, 排放在此位置: " << currentRect->position;
//                    //qDebug() << "排放后的最大排样高度: " << currentMaxHight;
//                    // 如果整体排样高度增加（甚至超出了板材边界），
//                    // 那么就从前面已排的零件中寻找宽度适合（零件宽度和排放零件处的
//                    // 线段的宽度均适合）的零件：
//                    if(currentMaxHight > maxHight || currentMaxHight > mHeight){
//                        //qDebug() << "整体高度增加，需要调换";
//                        // 如果已排零件集合不为空
//                        if(!layRects.isEmpty()){
//                            //qDebug() << "已排零件集合不为空";
//                            //qDebug() << "原始摆放位置： " << currentRect->position;
//                            // 用map来记录交换时的高度变化
//                            // <double hight, int n>
//                            // <交换之后排样高度, 已排矩形列表中的序号>
//                            QMap<double, int> map;
//                            // 遍历已排矩形列表
//                            for(int n=0; n<layRects.length();n++){
//                                // 判断两宽度是否合适，如果合适，尝试调换位置
//                                if(lowestLine->width() >= layRects[n].width && layRects[n].width >= rectWidth){
//                                    // 计算调换位置之后的最大高度
//                                    double heightTemp1 = lowestLine->left().ry() + layRects[n].height;
//                                    double heightTemp2 = layRects[n].position.ry() + rectHight;
//                                    double maxHightTemp = qMax(heightTemp1, heightTemp2);
//                                    // 若最大高度变小，则将调换记录保存
//                                    if(maxHightTemp < currentMaxHight){
//                                        //qDebug() << "最大高度变小，则将调换记录保存";
//                                        // 如果高度值不相同，直接保存
//                                        if(!map.contains(maxHightTemp)){
//                                            //qDebug() << "高度值不相同，直接保存maxHeightTemp: "<<maxHightTemp;
//                                            map.insert(maxHightTemp, n);
//                                        } else {// 如果高度值相同，则保存宽度较大的，这样可提高材料利用率
//                                            //qDebug() <<"高度值相同,存在map[maxHightTemp]:\t " << maxHightTemp << "  " << map[maxHightTemp];
//                                            if(layRects[n].width > map[maxHightTemp]){
//                                                //qDebug() << "替换map[maxHightTemp]为: " << n;
//                                                map[maxHightTemp] = n;
//                                            }
//                                        }
//                                    }
//                                }
//                            }
//                            QMap<double, int>::iterator it;
//                            for(it=map.begin(); it!=map.end(); it++){
//                                //qDebug() << it.key() <<"\t" << it.value();
//                            }

//                            ////qDebug() << "map count: " << map.count();
//                            // 如果map不为空
//                            if(map.count()){
//                                // 比较调换记录中最大高度值，取最小者进行调换
//                                // 由于map具有对key自动排序，所以取map的首记录
//                                int exchangeIndex = map.begin().value();

//                                // 还原该位置
//                                //qDebug() <<  index << "与第" << exchangeIndex << "个矩形(" << layRects[exchangeIndex].position << ")交换";
//                                updateHightProfileLine(new BaseLine(layRects[exchangeIndex].upper()));
//                                updateHightProfileLine(new BaseLine(layRects[exchangeIndex].lower()));

//                                // 调换位置，直接改变两矩形的位置坐标即可
//                                layRects[exchangeIndex].exchange(*currentRect);
//                                //qDebug() << "更换后该矩形位置： " << currentRect->position;
//                                //qDebug() << "更换后交换矩形位置： " << layRects[exchangeIndex].position;

//                                // 更新最大高度及最高轮廓线
//                                currentMaxHight = map.begin().key();
//                                updateHightProfileLine(new BaseLine(layRects[exchangeIndex].upper()));
//                                updateHightProfileLine(new BaseLine(layRects[exchangeIndex].lower()));
//                            }
//                            else{
//                                //qDebug() << "该矩形不用更换位置";
//                            }
//                        }
//                        else{
//                            //qDebug() << "已排零件集合为空, 直接排放";
//                        }
//                    }

//                    breakFlag = true;
//                    break;
//                }
//                lowestLine = lowestLine->next;
//            }
//            if(breakFlag){
//                break;
//            }
//            // 寻找比最低水平线高的最近的水平线集合
//            qDebug() << "-------寻找比最低水平线高的最近的水平线集合 start------";
//            minNode = NULL;//highestProfileLine.findMinLargerAVL(minNode);
//            qDebug() << "-------寻找比最低水平线高的最近的水平线集合 end------";
//            qDebug() << "";
//            if(minNode){
//                qDebug() << "该线段的宽度小于排入零件的宽度,寻找比最低水平线高的最近的水平线集合: " << minNode->data->line();
//            } else{
//                qDebug() << "寻找比最低水平线高的最近的水平线集合---------为空";
//            }
//        }

//        // 如果未找到合适位置，即未排放，则需要在某高度上
//        if(!lay){
//            qDebug() << "未找到该位置";
////            continue;
//        }
//        // 将当前矩形加入到已排矩形集合中
//        layRects.append(*currentRect);
//        // 更新最大高度及最高轮廓线
//        //qDebug() << "更新最大高度及最高轮廓线";
//        //qDebug() << "最大高度：" << currentMaxHight;
//        //qDebug() << "矩形上边界线：" << currentRect->upper();
//        //qDebug() << "矩形下边界线：" << currentRect->lower();
//        qDebug() << "-------------更新最大高度及最高轮廓线 start-------------";
//        maxHight = currentMaxHight;
//        updateHightProfileLine(new BaseLine(currentRect->upper()));
//        updateHightProfileLine(new BaseLine(currentRect->lower()));
//        qDebug() << "-------------更新最大高度及最高轮廓线 start-------------";
//        qDebug() << "";
        */

        bool lay = false;  // 排放标识
        double currentMaxHight = maxHight;  // 记录最高高度
        for(int j=0; j<emptyRectArea.length();j++){
            EmptyRectArea lowestRect = emptyRectArea[j];
            if(lowestRect.width() >= rectWidth && lowestRect.height() >= rectHight){
                // 如果该线段的宽度大于排入零件的宽度，则可排放该零件
                // 更新排放标识，表明已排放
                lay = true;

                // 将该零件排放在此位置
                currentRect->position = lowestRect.leftBottom();

                // 计算排放后的最大排样高度
                currentMaxHight = qMax(maxHight,
                                       lowestRect.leftBottom().ry() + rectHight);
                // 如果整体排样高度增加（甚至超出了板材边界），
                // 那么就从前面已排的零件中寻找宽度适合（零件宽度和排放零件处的
                // 线段的宽度均适合）的零件：
                // ......

                updateEmptyRectArea(new EmptyRectArea(*currentRect));
                break;
            }
        }

        // 如果未找到合适位置，即未排放，则需要在某高度上
        if(!lay){
            qDebug() << "未找到该位置";
        }

        maxHight = currentMaxHight;  // 更新最大高度
        layRects.append(*currentRect);  // 将当前矩形加入到已排矩形集合中
    }

#ifdef NESTDEBUG
    qDebug() << layRects.length();
    for(int i=0;i<layRects.length();i++){
        int index = layRects[i].index;
        QPointF position = layRects[i].position;
        bool rotate = layRects[i].rotate;
        qDebug() << index << "号矩形" << "第" << i << "个排放,  排放位置：" << position << "  ，旋转" << rotate;
    }
    qDebug() << "";
#endif
}

double RectNestEngine::calculateScore()
{
    // 计算适应度评分
    double mArea = mWidth * maxHight;
    double rate = allRectsArea / mArea;
#ifdef NESTDEBUG
    qDebug() << "最大高度： " << maxHight << "\t有效面积：" << mArea << "\t矩形件总面积：" << allRectsArea << "\t适应度评分值：" << rate;
#endif
    return rate;
}


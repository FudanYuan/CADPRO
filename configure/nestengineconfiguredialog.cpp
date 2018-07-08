#include "nestengineconfiguredialog.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QSizePolicy>
#include <QList>
#include <QMessageBox>

#include <QDebug>
NestEngineConfigureDialog::NestEngineConfigureDialog(NestEngineConfigure *config) :
    role(Manager),
    tabType(Default)
{
    setWindowTitle(tr("排版引擎配置"));

    // 适应屏幕大小
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    setFixedSize(screenRect.width() - 400, screenRect.height() - 450);
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));

    // 读入配置
    this->dataMap = config->LoadConfigureXml();

    // 添加组件
    tabWidget = new QTabWidget(this);
    connect(tabWidget, &QTabWidget::currentChanged, this, &NestEngineConfigureDialog::onTabChanged);

    wSheetTab = new WholeSheetConfigTab(this,this->dataMap[0]);
    tabWidget->addTab(wSheetTab, tr("整体材料"));
    connect(wSheetTab,&WholeSheetConfigTab::wDataChanged,this,&NestEngineConfigureDialog::onDataChanged);

    sSheetTab = new StripSheetConfigTab(this,this->dataMap[1]);
    tabWidget->addTab(sSheetTab, tr("条板材料"));
    connect(sSheetTab,&StripSheetConfigTab::sDataChanged,this,&NestEngineConfigureDialog::onDataChanged);

    pSheetTab = new PackageSheetConfig(this,this->dataMap[2]);
    tabWidget->addTab(pSheetTab, tr("卷装材料"));
    connect(pSheetTab,&PackageSheetConfig::pDataChanged,this,&NestEngineConfigureDialog::onDataChanged);

    // 将确定取消的按键加入布局
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::clicked, this, &NestEngineConfigureDialog::onDialogButtonCancelClicked);

    QVBoxLayout *mainLayout = new QVBoxLayout(this); // 主布局
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    mainLayout->addItem(new QSpacerItem(this->width(),
                                        10,
                                        QSizePolicy::Expanding,
                                        QSizePolicy::Expanding));
}

void NestEngineConfigureDialog::setDialogRole(NestEngineConfigureDialog::RoleType role)
{
    this->role = role;
}

NestEngineConfigureDialog::RoleType NestEngineConfigureDialog::getDialogRole()
{
    return role;
}

NestEngineConfigure::StripSheetNest *NestEngineConfigureDialog::getCurStripConfig()
{
    return curStripConfig;
}
NestEngineConfigure::WholeSheetNest *NestEngineConfigureDialog::getCurWholeConfig()
{
    return curWholeConfig;
}
NestEngineConfigure::PackageSheetNest *NestEngineConfigureDialog::getCurPackageConfig()
{
    return curPackageConfig;
}

void NestEngineConfigureDialog::onTabChanged(int i)
{
    tabType = (TabType)i;
    switch (i) {
    case 0:
        tabWidget->setCurrentIndex(0);
        if(role == Manager){
            return;
        }
        tabWidget->setTabEnabled(1, false);
        tabWidget->setTabEnabled(2, false);
        break;
    case 1:
        tabWidget->setCurrentIndex(1);
        if(role == Manager){
            return;
        }
        tabWidget->setTabEnabled(0, false);
        tabWidget->setTabEnabled(2, false);
        break;
    case 2:
        tabWidget->setCurrentIndex(2);
        if(role == Manager){
            return;
        }
        tabWidget->setTabEnabled(0, false);
        tabWidget->setTabEnabled(1, false);
        break;
    default:
        break;
    }
}

void NestEngineConfigureDialog::onDialogButtonClicked(QAbstractButton *button)
{
    if(button->text() == "OK"){
        onDialogButtonOkClicked();
    } else if(button->text() == "Cancel"){
        onDialogButtonCancelClicked();
    }
}

void NestEngineConfigureDialog::onDialogButtonOkClicked()
{
    switch (tabType) {
    case Whole:
        curWholeConfig = new NestEngineConfigure::WholeSheetNest();
        //方向排版
        curWholeConfig->wholedegree=wSheetTab->degree->text().toInt();
        if(wSheetTab->HorizontalNest->isChecked())
        {
            curWholeConfig->wholeorientation=NestEngine::HorizontalNest;
        }
        else if(wSheetTab->VerticalNest->isChecked())
        {
            curWholeConfig->wholeorientation=NestEngine::VerticalNest;
        }
        //混合方式
        if(wSheetTab->AllMixing->isChecked())
        {
            curWholeConfig->wholemixing = NestEngine::AllMixing;
        }
        else if(wSheetTab->TailPieceMixing->isChecked() && wSheetTab->TailLineMixing->isChecked()
                &&!wSheetTab->SameTypeSizeMixing->isChecked())
        {
            curWholeConfig->wholemixing =NestEngine::TailPieceMixing|NestEngine::TailLineMixing;
        }
        else if(wSheetTab->TailPieceMixing->isChecked() && !wSheetTab->TailLineMixing->isChecked()
                &&wSheetTab->SameTypeSizeMixing->isChecked())
        {
            curWholeConfig->wholemixing =NestEngine::TailPieceMixing|NestEngine::SameTypeSizeMixing;
        }
        else if(!wSheetTab->TailPieceMixing->isChecked() && wSheetTab->TailLineMixing->isChecked()
                &&wSheetTab->SameTypeSizeMixing->isChecked())
        {
            curWholeConfig->wholemixing =NestEngine::TailLineMixing|NestEngine::SameTypeSizeMixing;
        }
        else if(wSheetTab->TailPieceMixing->isChecked() && !wSheetTab->TailLineMixing->isChecked()
                &&!wSheetTab->SameTypeSizeMixing->isChecked())
        {
            curWholeConfig->wholemixing = NestEngine::TailPieceMixing;
        }
        else if(!wSheetTab->TailPieceMixing->isChecked() && wSheetTab->TailLineMixing->isChecked()
                &&!wSheetTab->SameTypeSizeMixing->isChecked())
        {
            curWholeConfig->wholemixing = NestEngine::TailLineMixing;
        }
        else if(!wSheetTab->TailPieceMixing->isChecked() && !wSheetTab->TailLineMixing->isChecked()
                &&wSheetTab->SameTypeSizeMixing->isChecked())
        {
            curWholeConfig->wholemixing = NestEngine::SameTypeSizeMixing;
        }
        else
        {
            curWholeConfig->wholemixing = NestEngine::NoMixing;
        }
        break;
    case Strip:
        // to do
        curStripConfig = new NestEngineConfigure::StripSheetNest();
        if(sSheetTab->leftRightTurnCBox->isChecked() && sSheetTab->sizeDownCBox->isChecked()){
            curStripConfig->strategy = NestEngine::AllStrategys;
        }
        else if(sSheetTab->leftRightTurnCBox->isChecked()){
            curStripConfig->strategy = NestEngine::LeftRightTurn;
        }
        else if(sSheetTab->sizeDownCBox->isChecked()){
            curStripConfig->strategy = NestEngine::SizeDown;
        } else {
            curStripConfig->strategy = NestEngine::NoStrategy;
        }
        if(sSheetTab->HorizontalAdaptiveSpacing->isChecked())
        {
            curStripConfig->stripadaptive=NestEngine::HorizontalAdaptiveSpacing;
        }
        else if(sSheetTab->TailPieceMixing->isChecked())
        {
            curStripConfig->stripmixing=NestEngine::NoMixing;
        }
        break;
    case Package:
        curPackageConfig = new NestEngineConfigure::PackageSheetNest();
        //方向排版
        curPackageConfig->packagedegree=pSheetTab->degree->text().toInt();
        if(pSheetTab->HorizontalNest->isChecked())
        {
            curPackageConfig->packageorientation=NestEngine::HorizontalNest;
        }
        else if(pSheetTab->VerticalNest->isChecked())
        {
            curPackageConfig->packageorientation=NestEngine::VerticalNest;
        }
        //混合方式
        if(pSheetTab->AllMixing->isChecked())
        {
            curPackageConfig->packagemixing = NestEngine::AllMixing;
        }
        else if(pSheetTab->TailPieceMixing->isChecked() && pSheetTab->TailLineMixing->isChecked()
                &&!pSheetTab->SameTypeSizeMixing->isChecked())
        {
            curPackageConfig->packagemixing =NestEngine::TailPieceMixing|NestEngine::TailLineMixing;
        }
        else if(pSheetTab->TailPieceMixing->isChecked() && !pSheetTab->TailLineMixing->isChecked()
                &&pSheetTab->SameTypeSizeMixing->isChecked())
        {
            qDebug()<<"运行到这里";
            curPackageConfig->packagemixing =NestEngine::TailPieceMixing|NestEngine::SameTypeSizeMixing;
        }
        else if(!pSheetTab->TailPieceMixing->isChecked() && pSheetTab->TailLineMixing->isChecked()
                &&pSheetTab->SameTypeSizeMixing->isChecked())
        {
            curPackageConfig->packagemixing =NestEngine::TailLineMixing|NestEngine::SameTypeSizeMixing;
        }
        else if(pSheetTab->TailPieceMixing->isChecked() && !pSheetTab->TailLineMixing->isChecked()
                &&!pSheetTab->SameTypeSizeMixing->isChecked())
        {
            curPackageConfig->packagemixing = NestEngine::TailPieceMixing;
        }
        else if(!pSheetTab->TailPieceMixing->isChecked() && pSheetTab->TailLineMixing->isChecked()
                &&!pSheetTab->SameTypeSizeMixing->isChecked())
        {
            curPackageConfig->packagemixing = NestEngine::TailLineMixing;
        }
        else if(!pSheetTab->TailPieceMixing->isChecked() && !pSheetTab->TailLineMixing->isChecked()
                &&pSheetTab->SameTypeSizeMixing->isChecked())
        {
            curPackageConfig->packagemixing = NestEngine::SameTypeSizeMixing;
        }

        else
        {
            curPackageConfig->packagemixing = NestEngine::NoMixing;
        }
        break;
    default:
        break;
    }
    NestEngineConfigure * configure = new NestEngineConfigure();
    configure->WriteConfigureXml(this->dataMap);
    QDialog::accept();
}

void NestEngineConfigureDialog::onDialogButtonCancelClicked()
{
    if(role == Manager){
        QDialog::accept();
        return;
    }
    switch (tabType) {
    case Whole:
        curWholeConfig = new NestEngineConfigure::WholeSheetNest();
        //方向排版
        curWholeConfig->wholedegree=wSheetTab->degree->text().toInt();
        if(wSheetTab->HorizontalNest->isChecked())
        {
            curWholeConfig->wholeorientation=NestEngine::HorizontalNest;
        }
        else if(wSheetTab->VerticalNest->isChecked())
        {
            curWholeConfig->wholeorientation=NestEngine::VerticalNest;
        }
        //混合方式
        if(wSheetTab->AllMixing->isChecked())
        {
            curWholeConfig->wholemixing = NestEngine::AllMixing;
        }
        else if(wSheetTab->TailPieceMixing->isChecked() && wSheetTab->TailLineMixing->isChecked()
                &&!wSheetTab->SameTypeSizeMixing->isChecked())
        {
            curWholeConfig->wholemixing =NestEngine::TailPieceMixing|NestEngine::TailLineMixing;
        }
        else if(wSheetTab->TailPieceMixing->isChecked() && !wSheetTab->TailLineMixing->isChecked()
                &&wSheetTab->SameTypeSizeMixing->isChecked())
        {
            curWholeConfig->wholemixing =NestEngine::TailPieceMixing|NestEngine::SameTypeSizeMixing;
        }
        else if(!wSheetTab->TailPieceMixing->isChecked() && wSheetTab->TailLineMixing->isChecked()
                &&wSheetTab->SameTypeSizeMixing->isChecked())
        {
            curWholeConfig->wholemixing =NestEngine::TailLineMixing|NestEngine::SameTypeSizeMixing;
        }
        else if(wSheetTab->TailPieceMixing->isChecked() && !wSheetTab->TailLineMixing->isChecked()
                &&!wSheetTab->SameTypeSizeMixing->isChecked())
        {
            curWholeConfig->wholemixing = NestEngine::TailPieceMixing;
        }
        else if(!wSheetTab->TailPieceMixing->isChecked() && wSheetTab->TailLineMixing->isChecked()
                &&!wSheetTab->SameTypeSizeMixing->isChecked())
        {
            curWholeConfig->wholemixing = NestEngine::TailLineMixing;
        }
        else if(!wSheetTab->TailPieceMixing->isChecked() && !wSheetTab->TailLineMixing->isChecked()
                &&wSheetTab->SameTypeSizeMixing->isChecked())
        {
            curWholeConfig->wholemixing = NestEngine::SameTypeSizeMixing;
        }
        else
        {
            curWholeConfig->wholemixing = NestEngine::NoMixing;
        }
        break;
    case Strip:
        // to do
        curStripConfig = new NestEngineConfigure::StripSheetNest();
        if(sSheetTab->leftRightTurnCBox->isChecked() && sSheetTab->sizeDownCBox->isChecked()){
            curStripConfig->strategy = NestEngine::AllStrategys;
        }
        else if(sSheetTab->leftRightTurnCBox->isChecked()){
            curStripConfig->strategy = NestEngine::LeftRightTurn;
        }
        else if(sSheetTab->sizeDownCBox->isChecked()){
            curStripConfig->strategy = NestEngine::SizeDown;
        } else {
            curStripConfig->strategy = NestEngine::NoStrategy;
        }
        if(sSheetTab->HorizontalAdaptiveSpacing->isChecked())
        {
            curStripConfig->stripadaptive=NestEngine::HorizontalAdaptiveSpacing;
        }
        else if(sSheetTab->TailPieceMixing->isChecked())
        {
            curStripConfig->stripmixing=NestEngine::NoMixing;
        }
        break;
    case Package:
        curPackageConfig = new NestEngineConfigure::PackageSheetNest();
        //方向排版
        curPackageConfig->packagedegree=pSheetTab->degree->text().toInt();
        if(pSheetTab->HorizontalNest->isChecked())
        {
            curPackageConfig->packageorientation=NestEngine::HorizontalNest;
        }
        else if(pSheetTab->VerticalNest->isChecked())
        {
            curPackageConfig->packageorientation=NestEngine::VerticalNest;
        }
        //混合方式
        if(pSheetTab->AllMixing->isChecked())
        {
            curPackageConfig->packagemixing = NestEngine::AllMixing;
        }
        else if(pSheetTab->TailPieceMixing->isChecked() && pSheetTab->TailLineMixing->isChecked()
                &&!pSheetTab->SameTypeSizeMixing->isChecked())
        {
            curPackageConfig->packagemixing =NestEngine::TailPieceMixing|NestEngine::TailLineMixing;
        }
        else if(pSheetTab->TailPieceMixing->isChecked() && !pSheetTab->TailLineMixing->isChecked()
                &&pSheetTab->SameTypeSizeMixing->isChecked())
        {
            qDebug()<<"运行到这里";
            curPackageConfig->packagemixing =NestEngine::TailPieceMixing|NestEngine::SameTypeSizeMixing;
        }
        else if(!pSheetTab->TailPieceMixing->isChecked() && pSheetTab->TailLineMixing->isChecked()
                &&pSheetTab->SameTypeSizeMixing->isChecked())
        {
            curPackageConfig->packagemixing =NestEngine::TailLineMixing|NestEngine::SameTypeSizeMixing;
        }
        else if(pSheetTab->TailPieceMixing->isChecked() && !pSheetTab->TailLineMixing->isChecked()
                &&!pSheetTab->SameTypeSizeMixing->isChecked())
        {
            curPackageConfig->packagemixing = NestEngine::TailPieceMixing;
        }
        else if(!pSheetTab->TailPieceMixing->isChecked() && pSheetTab->TailLineMixing->isChecked()
                &&!pSheetTab->SameTypeSizeMixing->isChecked())
        {
            curPackageConfig->packagemixing = NestEngine::TailLineMixing;
        }
        else if(!pSheetTab->TailPieceMixing->isChecked() && !pSheetTab->TailLineMixing->isChecked()
                &&pSheetTab->SameTypeSizeMixing->isChecked())
        {
            curPackageConfig->packagemixing = NestEngine::SameTypeSizeMixing;
        }

        else
        {
            curPackageConfig->packagemixing = NestEngine::NoMixing;
        }
        break;
    default:
        break;
    }

    QMessageBox::warning(this, tr("警告"), tr("将使用默认配置数据！"));
    QDialog::accept();

}

void NestEngineConfigureDialog::onDataChanged(int index, QList<QList<int> > changedData)
{
    this->dataMap[index] = changedData;
}

void NestEngineConfigureDialog::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton button;
    if(role == Manager){
        event->accept();  //接受退出信号，程序退出
        return;
    }
    button = QMessageBox::question(this, tr("警告"),
                                   QString(tr("确认关闭配置窗口?")),
                                   QMessageBox::Yes | QMessageBox::No);
    if (button == QMessageBox::No) {
         event->ignore();  //忽略退出信号，程序继续运行
    } else if (button == QMessageBox::Yes) {
        onDialogButtonCancelClicked();
        event->accept();  //接受退出信号，程序退出
    }
}

WholeSheetConfigTab::WholeSheetConfigTab(QWidget *parent,QList<QList<int>> dataList) :
    QWidget(parent)
{
    setWindowTitle(tr("排版设置"));
    this->dataList = dataList;
    QHBoxLayout *mainlayout=new QHBoxLayout();
    QVBoxLayout *vLayout1 = new QVBoxLayout();
    QVBoxLayout *vLayout2 = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();
    QHBoxLayout *hLayout1 = new QHBoxLayout();

    QLabel *label1 = new QLabel(tr("摆动角度"), this);
    degree =new QLineEdit;
    degree->setText(tr("0"));
    degree->setValidator(new QIntValidator(0, 90, this));//限制只能输入数字0~90
    QLabel *label2 = new QLabel(tr("度"), this);
    HorizontalNest = new QRadioButton(tr("横向"), this);
    VerticalNest = new QRadioButton(tr("纵向"), this);
    HorizontalNest->setChecked(true);

    hLayout->addWidget(label1);
    hLayout->addWidget(degree);
    hLayout->addWidget(label2);
    hLayout->addWidget(HorizontalNest);
    hLayout->addWidget(VerticalNest);

    TailPieceMixing = new QCheckBox(tr("尾只混合"), this);
    TailLineMixing = new QCheckBox(tr("尾行优化，允许随意角度"));
    SameTypeSizeMixing = new QCheckBox(tr("同型体内尺码混合"));
    AllMixing = new QCheckBox(tr("全混合"));

    tableWidget = new QTableWidget(0, 2);
    tableWidget->setShowGrid(true);
    QStringList m_Header;
    m_Header<<tr("类型")<<tr("名称");
    tableWidget->setHorizontalHeaderLabels(m_Header);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setFrameShape(QFrame::Box);//设置边框
    tableWidget->setShowGrid(true); //设置显示格子线
    vLayout2->addWidget(tableWidget);
    button1 =new QPushButton(tr("保存"));
    button2 =new QPushButton(tr("编辑"));
    button3 =new QPushButton(tr("删除"));
    button4 = new QPushButton(tr("增加"));
    button1->setEnabled(false);
    button2->setEnabled(false);
    button3->setEnabled(false);
    this->setTableWidget();
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
    tableWidget->show();

    hLayout1->addWidget(button4);
    hLayout1->addWidget(button2);
    hLayout1->addWidget(button3);
    hLayout1->addWidget(button1);
    vLayout2->addLayout(hLayout1);

    vLayout1->addLayout(hLayout);
    vLayout1->addWidget(TailPieceMixing);
    vLayout1->addWidget(TailLineMixing);
    vLayout1->addWidget(SameTypeSizeMixing);
    vLayout1->addWidget(AllMixing);

    mainlayout->addLayout(vLayout1);
    mainlayout->addLayout(vLayout2);
    mainlayout->setStretch(0, 10);
    mainlayout->setStretch(1, 25);
    mainlayout->addItem(new QSpacerItem(this->width(),
                                        10,
                                        QSizePolicy::Expanding,
                                        QSizePolicy::Expanding));
    setLayout(mainlayout);

    connect(AllMixing, &QCheckBox::toggled, this, &WholeSheetConfigTab::onQCheckBoxChanged);
    connect(TailPieceMixing, &QCheckBox::toggled, this, &WholeSheetConfigTab::onQCheckBoxChanged1);
    connect(TailLineMixing, &QCheckBox::toggled, this, &WholeSheetConfigTab::onQCheckBoxChanged2);
    connect(SameTypeSizeMixing, &QCheckBox::toggled, this, &WholeSheetConfigTab::onQCheckBoxChanged3);
    connect(button1,&QPushButton::clicked,this,&WholeSheetConfigTab::onConfigureButtonClicked);
    connect(button2,&QPushButton::clicked,this,&WholeSheetConfigTab::onEditButtonClicked);
    connect(button3,&QPushButton::clicked,this,&WholeSheetConfigTab::onDeleteButtonClicked);
    connect(button4,&QPushButton::clicked,this,&WholeSheetConfigTab::onNewButtonClicked);
    connect(tableWidget,&QTableWidget::currentItemChanged,this,&WholeSheetConfigTab::onItemChanged);
}

void WholeSheetConfigTab::setTableWidget()
{
    //tableWidget->clear();
    for(int row = 0; row<this->dataList.length(); row++){
        tableWidget->insertRow(row);
        QTableWidgetItem * type = new QTableWidgetItem(tr("Whole"));
        type->setFlags(type->flags()&(~Qt::ItemIsEditable));
        tableWidget->setItem(row,0,type);
        QString  name = QString("%1").arg(row+1);
        name = "材料"+name;
        tableWidget->setItem(row,1,new QTableWidgetItem(name));
    }
}
void WholeSheetConfigTab::onConfigureButtonClicked()
{
    emit wDataChanged(0,this->dataList);
    this->tableWidget->clearSelection();
    //确认之后，所有组建回归初始化状态
    this->degree->setText(QString("%1").arg(0));
    this->HorizontalNest->setChecked(true);
    this->VerticalNest->setChecked(false);
    this->TailPieceMixing->setChecked(false);
    this->TailLineMixing->setChecked(false);
    this->SameTypeSizeMixing->setChecked(false);
    this->AllMixing->setChecked(false);

    //这部分代码用来处理 edit
    this->button1->setEnabled(false);
    this->button2->setEnabled(false);
    this->button3->setEnabled(false);
    this->button4->setEnabled(true);
//    this->degree->setEnabled(false);
//    this->HorizontalNest->setEnabled(false);
//    this->VerticalNest->setEnabled(false);
//    this->TailPieceMixing->setEnabled(false);
//    this->TailLineMixing->setEnabled(false);
//    this->SameTypeSizeMixing->setEnabled(false);
//    this->AllMixing->setEnabled(false);
    if(!this->tableWidget->currentItem())
        return;
    int index = this->tableWidget->currentItem()->row();

    this->dataList[index][0] = this->degree->text().toInt();

    if(this->VerticalNest->isChecked()){
        this->dataList[index][1] = 1;
    }
    else{
        this->dataList[index][2] = 1;
    }

    if(this->AllMixing->isChecked()){
        this->dataList[index][3] = 1;
        this->dataList[index][4] = 1;
        this->dataList[index][5] = 1;
        this->dataList[index][6] = 1;
    }

    else if(this->TailPieceMixing->isChecked()&&!this->TailLineMixing->isChecked()&&
            !this->SameTypeSizeMixing->isChecked()){
        this->dataList[index][3] = 1;
        this->dataList[index][4] = 0;
        this->dataList[index][5] = 0;
        this->dataList[index][6] = 0;
    }

    else if(!this->TailPieceMixing->isChecked()&&this->TailLineMixing->isChecked()&&
            !this->SameTypeSizeMixing->isChecked()){
        this->dataList[index][3] = 0;
        this->dataList[index][4] = 1;
        this->dataList[index][5] = 0;
        this->dataList[index][6] = 0;
    }

    else if(!this->TailPieceMixing->isChecked()&&!this->TailLineMixing->isChecked()&&
            this->SameTypeSizeMixing->isChecked()){
        this->dataList[index][3] = 0;
        this->dataList[index][4] = 0;
        this->dataList[index][5] = 1;
        this->dataList[index][6] = 0;
    }

    else if(this->TailPieceMixing->isChecked()&&this->TailLineMixing->isChecked()&&
            !this->SameTypeSizeMixing->isChecked()){
        this->dataList[index][3] = 1;
        this->dataList[index][4] = 1;
        this->dataList[index][5] = 0;
        this->dataList[index][6] = 0;
    }

    else if(!this->TailPieceMixing->isChecked()&&this->TailLineMixing->isChecked()&&
            !this->SameTypeSizeMixing->isChecked()){
        this->dataList[index][3] = 0;
        this->dataList[index][4] = 1;
        this->dataList[index][5] = 1;
        this->dataList[index][6] = 0;
    }

    else if(this->TailPieceMixing->isChecked()&&!this->TailLineMixing->isChecked()&&
            this->SameTypeSizeMixing->isChecked()){
        this->dataList[index][3] = 1;
        this->dataList[index][4] = 0;
        this->dataList[index][5] = 1;
        this->dataList[index][6] = 0;
    }
    emit wDataChanged(0,this->dataList);

    this->tableWidget->clearSelection();

}

void WholeSheetConfigTab::onEditButtonClicked()
{
    //this->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
    this->degree->setEnabled(true);
    this->HorizontalNest->setEnabled(true);
    this->VerticalNest->setEnabled(true);
    this->TailPieceMixing->setEnabled(true);
    this->TailLineMixing->setEnabled(true);
    this->SameTypeSizeMixing->setEnabled(true);
    this->AllMixing->setEnabled(true);

    this->button1->setEnabled(true);
    this->button3->setEnabled(false);
    this->button4->setEnabled(false);
}

void WholeSheetConfigTab::onDeleteButtonClicked()
{
    if(this->tableWidget->currentItem()!=NULL)
    {
        int row = this->tableWidget->currentItem()->row();
        //this->dataMap->value(0).removeAt(row);
        //this->setTableWidget();

        this->dataList.removeAt(row);
        emit wDataChanged(0,this->dataList);

        qDebug()<<"here we are in del w"<<row;

        //bug是不能一开始就删除第一个！！！
        this->tableWidget->removeRow(row);
    }

    this->button1->setEnabled(true);
    this->button2->setEnabled(false);
    this->button3->setEnabled(false);
    //所有选项不可编辑
    this->degree->setText(QString("%1").arg(0));


    this->HorizontalNest->setChecked(false);

    this->VerticalNest->setChecked(false);


    this->TailPieceMixing->setChecked(false);

    this->TailLineMixing->setChecked(false);

    this->SameTypeSizeMixing->setChecked(false);

    this->AllMixing->setChecked(false);
}

void WholeSheetConfigTab::onNewButtonClicked()
{
    QList<int> newConfigureList;
    newConfigureList.append(this->degree->text().toInt());
    if(this->HorizontalNest->isChecked()){
        newConfigureList.append(1);
        newConfigureList.append(0);
    }
    else{
        newConfigureList.append(0);
        newConfigureList.append(1);
    }
    if(this->AllMixing->isChecked()){
        newConfigureList.append(1);
        newConfigureList.append(1);
        newConfigureList.append(1);
        newConfigureList.append(1);
    }
    else if(this->TailPieceMixing->isChecked()&&!this->TailLineMixing->isChecked()
            &&!this->SameTypeSizeMixing->isChecked()){
        newConfigureList.append(1);
        newConfigureList.append(0);
        newConfigureList.append(0);
        newConfigureList.append(0);
    }
    else if(!this->TailPieceMixing->isChecked()&&this->TailLineMixing->isChecked()
            &&!this->SameTypeSizeMixing->isChecked()){
        newConfigureList.append(0);
        newConfigureList.append(1);
        newConfigureList.append(0);
        newConfigureList.append(0);

    }
    else if(!this->TailPieceMixing->isChecked()&&!this->TailLineMixing->isChecked()
            &&this->SameTypeSizeMixing->isChecked()){
        newConfigureList.append(0);
        newConfigureList.append(0);
        newConfigureList.append(1);
        newConfigureList.append(0);

    }
    else if(this->TailPieceMixing->isChecked()&&this->TailLineMixing->isChecked()
            &&!this->SameTypeSizeMixing->isChecked()){
        newConfigureList.append(1);
        newConfigureList.append(1);
        newConfigureList.append(0);
        newConfigureList.append(0);

    }
    else if(this->TailPieceMixing->isChecked()&&!this->TailLineMixing->isChecked()
            &&this->SameTypeSizeMixing->isChecked()){
        newConfigureList.append(1);
        newConfigureList.append(0);
        newConfigureList.append(1);
        newConfigureList.append(0);

    }
    else if(!this->TailPieceMixing->isChecked()&&this->TailLineMixing->isChecked()
            &&this->SameTypeSizeMixing->isChecked()){
        newConfigureList.append(0);
        newConfigureList.append(1);
        newConfigureList.append(1);
        newConfigureList.append(0);

    }
    else{
        newConfigureList.append(0);
        newConfigureList.append(0);
        newConfigureList.append(0);
        newConfigureList.append(0);
    }
    this->dataList.append(newConfigureList);

    int row = this->tableWidget->rowCount();
    this->tableWidget->insertRow(row);
    QTableWidgetItem * type = new QTableWidgetItem(tr("Whole"));
    type->setFlags(type->flags()&(~Qt::ItemIsEditable));
    tableWidget->setItem(row,0,type);
    QString  name = QString("%1").arg(row+1);
    name = "材料"+name;
    tableWidget->setItem(row,1,new QTableWidgetItem(name));
    this->tableWidget->clearSelection();
    //this->tableWidget

    this->button2->setEnabled(false);
    this->button3->setEnabled(false);
    this->button1->setEnabled(!this->button1->isChecked());
}

void WholeSheetConfigTab::onItemChanged()
{
    if(this->dataList.length() == 0)
        return;

//    bool flag = false;
//    if(this->tableWidget!=NULL){
//        for(int i = 0; i<this->tableWidget->rowCount(); i++){
//            QTableWidgetItem * item = tableWidget->item(i,0);
//            if(item->isSelected()){
//                flag = true;
//                break;
//            }
//        }
//    }
//    if(!flag){
//        this->TailPieceMixing->setChecked(false);
//        this->TailLineMixing->setChecked(false);
//        this->SameTypeSizeMixing->setChecked(false);
//        this->AllMixing->setChecked(false);
//        return;
//    }

    int row = this->tableWidget->currentItem()->row();
    //qDebug()<<"row::222"<<row;
    //this->button1->setEnabled(true);
    this->button4->setEnabled(false);
    this->button2->setEnabled(true);
    this->button3->setEnabled(true);
    QList<int> configurelist = this->dataList[row];
   // qDebug()<<"row::233"<<configurelist.length();
    this->degree->setText(QString("%1").arg(configurelist[0]));
    if(configurelist[1] == 1)
        this->HorizontalNest->setChecked(true);
    else
        this->VerticalNest->setChecked(true);
    if(configurelist[6] == 1){
        this->TailPieceMixing->setChecked(true);
        this->TailLineMixing->setChecked(true);
        this->SameTypeSizeMixing->setChecked(true);
        this->AllMixing->setChecked(true);
    }
    else if(configurelist[3] == 1&&configurelist[4] == 0&&configurelist[5]==0){
        this->TailPieceMixing->setChecked(true);
        this->TailLineMixing->setChecked(false);
        this->SameTypeSizeMixing->setChecked(false);
        this->AllMixing->setChecked(false);
    }
    else if(configurelist[3] == 0&&configurelist[4] == 1&&configurelist[5]==0){
        this->TailPieceMixing->setChecked(false);
        this->TailLineMixing->setChecked(true);
        this->SameTypeSizeMixing->setChecked(false);
        this->AllMixing->setChecked(false);
    }
    else if(configurelist[3] == 0&&configurelist[4] == 0&&configurelist[5]==1){
        this->TailPieceMixing->setChecked(false);
        this->TailLineMixing->setChecked(false);
        this->SameTypeSizeMixing->setChecked(true);
        this->AllMixing->setChecked(false);
    }
    else if(configurelist[3] == 1&&configurelist[4] == 1&&configurelist[5]==0){
        this->TailPieceMixing->setChecked(true);
        this->TailLineMixing->setChecked(true);
        this->SameTypeSizeMixing->setChecked(false);
        this->AllMixing->setChecked(false);
    }
    else if(configurelist[3] == 1&&configurelist[4] == 0&&configurelist[5]==1){
        this->TailPieceMixing->setChecked(true);
        this->TailLineMixing->setChecked(false);
        this->SameTypeSizeMixing->setChecked(true);
        this->AllMixing->setChecked(false);
    }
    else if(configurelist[3] == 0&&configurelist[4] == 1&&configurelist[5]==1){
        this->TailPieceMixing->setChecked(false);
        this->TailLineMixing->setChecked(true);
        this->SameTypeSizeMixing->setChecked(true);
        this->AllMixing->setChecked(false);
    }
}

StripSheetConfigTab::StripSheetConfigTab(QWidget *parent,QList<QList<int>> dataList) :
    QWidget(parent)
{
     setWindowTitle(tr("条板设置"));
     this->dataList = dataList;
     QHBoxLayout *mainlayout = new QHBoxLayout();
     QVBoxLayout *layout = new QVBoxLayout();
     QVBoxLayout *layout1 = new QVBoxLayout();
     QHBoxLayout *hLayout1= new QHBoxLayout();
     leftRightTurnCBox = new QCheckBox(tr("左右交替"));
     sizeDownCBox = new QCheckBox(tr("尺码大到小"));
     HorizontalAdaptiveSpacing = new QCheckBox(tr("横向自适应间距"));
     TailPieceMixing = new QCheckBox(tr("尾只混合"));

     layout->addWidget(leftRightTurnCBox);
     layout->addWidget(sizeDownCBox);
     layout->addWidget(HorizontalAdaptiveSpacing);
     layout->addWidget(TailPieceMixing);
     mainlayout->addLayout(layout);
     tableWidget = new QTableWidget(0, 2);
     tableWidget->setShowGrid(true);
     QStringList m_Header;
     m_Header<<tr("类型")<<tr("名称");
     tableWidget->setHorizontalHeaderLabels(m_Header);

     this->setTabWidget();

     tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
     tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
     layout1->addWidget(tableWidget);
     button1 =new QPushButton(tr("保存"));
     button2 =new QPushButton(tr("编辑"));
     button3 =new QPushButton(tr("删除"));
     button4 = new QPushButton(tr("增加"));

     button1->setEnabled(false);
     button2->setEnabled(false);
     button3->setEnabled(false);
     button4->setEnabled(true);

     hLayout1->addWidget(button4);
     hLayout1->addWidget(button2);
     hLayout1->addWidget(button3);
     hLayout1->addWidget(button1);
     layout1->addLayout(hLayout1);
     mainlayout->addLayout(layout1);
     setLayout(mainlayout);
     mainlayout->setStretch(0, 8);
     mainlayout->setStretch(1, 25);

     connect(button1,&QPushButton::clicked,this,&StripSheetConfigTab::onConfigureButtonClicked);
     connect(button2,&QPushButton::clicked,this,&StripSheetConfigTab::onEditButtonClicked);
     connect(button3,&QPushButton::clicked,this,&StripSheetConfigTab::onDeleteButtonClicked);
     connect(button4,&QPushButton::clicked,this,&StripSheetConfigTab::onNewButtonClicked);
     connect(tableWidget,&QTableWidget::currentItemChanged,this,&StripSheetConfigTab::onItemChanged);

}

void StripSheetConfigTab::setTabWidget()
{
    for(int row = 0; row<this->dataList.length(); row++){
        tableWidget->insertRow(row);
        QTableWidgetItem * type = new QTableWidgetItem(tr("Strip"));
        type->setFlags(type->flags()&(~Qt::ItemIsEditable));
        tableWidget->setItem(row,0,type);
        QString  name = QString("%1").arg(row+1);
        name = "材料"+name;
        tableWidget->setItem(row,1,new QTableWidgetItem(name));
    }
}

void StripSheetConfigTab::onConfigureButtonClicked()
{
    emit sDataChanged(1,this->dataList);

    this->button1->setEnabled(false);
    this->button2->setEnabled(false);
    this->button3->setEnabled(false);
    this->button4->setEnabled(true);

    //所有组件回归初始化状态
    this->leftRightTurnCBox->setChecked(false);
    this->sizeDownCBox->setChecked(false);
    this->HorizontalAdaptiveSpacing->setChecked(false);
    this->TailPieceMixing->setChecked(false);

    if(!this->tableWidget->currentItem()){
        qDebug()<<"here";
        return;
    }

    int index = this->tableWidget->currentItem()->row();
    if(this->leftRightTurnCBox->isChecked()){
        this->dataList[index][0] = 1;
    }
    else{
        this->dataList[index][0] = 0;
    }
    if(this->sizeDownCBox->isChecked()){
        this->dataList[index][1] = 1;
    }
    else{
        this->dataList[index][1] = 0;
    }
    if(this->HorizontalAdaptiveSpacing->isChecked()){
        this->dataList[index][2] = 1;
    }
    else{
        this->dataList[index][2] = 0;
    }
    if(this->TailPieceMixing->isChecked()){
        this->dataList[index][3] = 1;
    }
    else{
        this->dataList[index][3] = 0;
    }
    emit sDataChanged(1,this->dataList);
}

void StripSheetConfigTab::onEditButtonClicked()
{
    this->leftRightTurnCBox->setEnabled(true);
    this->sizeDownCBox->setEnabled(true);
    this->HorizontalAdaptiveSpacing->setEnabled(true);
    this->TailPieceMixing->setEnabled(true);
    this->button1->setEnabled(true);
    this->button3->setEnabled(false);
}

void StripSheetConfigTab::onDeleteButtonClicked()
{
    if(this->tableWidget->currentItem()!=NULL)
    {
        int row = this->tableWidget->currentItem()->row();
        this->dataList.removeAt(row);
        emit sDataChanged(1,this->dataList);
        //bug是不能一开始就删除第一个！！！
        this->tableWidget->removeRow(row);
    }

    this->button1->setEnabled(true);
    this->button2->setEnabled(false);
    this->button3->setEnabled(false);

    this->leftRightTurnCBox->setChecked(false);
    this->sizeDownCBox->setChecked(false);
    this->HorizontalAdaptiveSpacing->setChecked(false);
    this->TailPieceMixing->setChecked(false);
}

void StripSheetConfigTab::onNewButtonClicked()
{
    QList<int> newConfigureList;
    if(this->leftRightTurnCBox->isChecked()){
        newConfigureList.append(1);
    }
    else{
        newConfigureList.append(0);
    }
    if(this->sizeDownCBox->isChecked()){
        newConfigureList.append(1);
    }
    else{
        newConfigureList.append(0);
    }
    if(this->HorizontalAdaptiveSpacing->isChecked()){
        newConfigureList.append(1);
    }
    else{
        newConfigureList.append(0);
    }
    if(this->TailPieceMixing->isChecked()){
        newConfigureList.append(1);
    }
    else{
        newConfigureList.append(0);
    }

    this->dataList.append(newConfigureList);

    int row = this->tableWidget->rowCount();
    this->tableWidget->insertRow(row);
    QTableWidgetItem * type = new QTableWidgetItem(tr("Strip"));
    type->setFlags(type->flags()&(~Qt::ItemIsEditable));
    tableWidget->setItem(row,0,type);
    QString  name = QString("%1").arg(row+1);
    name = "材料"+name;
    tableWidget->setItem(row,1,new QTableWidgetItem(name));
    this->tableWidget->clearSelection();

    this->button2->setEnabled(false);
    this->button3->setEnabled(false);
    this->button1->setEnabled(!this->button1->isChecked());
}

void StripSheetConfigTab::onItemChanged()
{
    if(this->dataList.length() == 0)
        return;
    int row = this->tableWidget->currentItem()->row();
    this->button2->setEnabled(true);
    this->button3->setEnabled(true);
    this->button4->setEnabled(false);
    QList<int> configurelist = this->dataList[row];

    if(configurelist[0] == 1){
        this->leftRightTurnCBox->setChecked(true);
    }
    else{
        this->leftRightTurnCBox->setChecked(false);
    }
    if(configurelist[1] == 1){
        this->sizeDownCBox->setChecked(true);
    }
    else{
        this->sizeDownCBox->setChecked(false);
    }
    if(configurelist[2] == 1){
        this->HorizontalAdaptiveSpacing->setChecked(true);
    }
    else{
        this->HorizontalAdaptiveSpacing->setChecked(false);
    }
    if(configurelist[3] == 1){
        this->TailPieceMixing->setChecked(true);
    }
    else{
        this->TailPieceMixing->setChecked(false);
    }
}

PackageSheetConfig::PackageSheetConfig(QWidget *parent,QList<QList<int>> dataList) :
    QWidget(parent)
{
    this->dataList = dataList;
    setWindowTitle(tr("排版设置"));
    QHBoxLayout *mainlayout=new QHBoxLayout();
    QVBoxLayout *vLayout1 = new QVBoxLayout();
    QVBoxLayout *vLayout2 = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();
    QHBoxLayout *hLayout1 = new QHBoxLayout();
    QLabel *label1 =new QLabel(tr("摆动角度"));
    degree =new QLineEdit;
    degree->setText(tr("0"));
    degree->setValidator(new QIntValidator(0, 90, this));//限制只能输入数字0~90
    QLabel *label2 = new QLabel(tr("度"), this);
    HorizontalNest = new QRadioButton(tr("横向"));
    VerticalNest = new QRadioButton(tr("纵向"));
    HorizontalNest->setChecked(true);
    hLayout->addWidget(label1);
    hLayout->addWidget(degree);
    hLayout->addWidget(label2);
    hLayout->addWidget(HorizontalNest);
    hLayout->addWidget(VerticalNest);
    vLayout1->addLayout(hLayout);

    TailPieceMixing = new QCheckBox(tr("尾只混合"), this);
    TailLineMixing = new QCheckBox(tr("尾行优化，允许随意角度"));
    SameTypeSizeMixing = new QCheckBox(tr("同型体内尺码混合"));
    AllMixing = new QCheckBox(tr("全混合"));

    tableWidget = new QTableWidget(0, 2);
    tableWidget->setShowGrid(true);
    QStringList m_Header;
    m_Header<<tr("类型")<<tr("名称");
    tableWidget->setHorizontalHeaderLabels(m_Header);

    this->setTableWidget();

    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    vLayout2->addWidget(tableWidget);
    button1 =new QPushButton(tr("保存"));
    button2 =new QPushButton(tr("编辑"));
    button3 =new QPushButton(tr("删除"));
    button4 =new QPushButton(tr("增加"));
    button1->setEnabled(false);
    button2->setEnabled(false);
    button3->setEnabled(false);
    button4->setEnabled(true);

    hLayout1->addWidget(button4);
    hLayout1->addWidget(button2);
    hLayout1->addWidget(button3);
    hLayout1->addWidget(button1);
    vLayout2->addLayout(hLayout1);

    vLayout1->addWidget(TailPieceMixing);
    vLayout1->addWidget(TailLineMixing);
    vLayout1->addWidget(SameTypeSizeMixing);
    vLayout1->addWidget(AllMixing);
    mainlayout->addLayout(vLayout1);
    mainlayout->addLayout(vLayout2);
    mainlayout->setStretch(0, 10);
    mainlayout->setStretch(1, 25);
    setLayout(mainlayout);

    connect(AllMixing, &QCheckBox::toggled, this, &PackageSheetConfig::onQCheckBoxChanged11);
    connect(TailPieceMixing, &QCheckBox::toggled, this, &PackageSheetConfig::onQCheckBoxChanged12);
    connect(TailLineMixing, &QCheckBox::toggled, this, &PackageSheetConfig::onQCheckBoxChanged13);
    connect(SameTypeSizeMixing, &QCheckBox::toggled, this, &PackageSheetConfig::onQCheckBoxChanged14);
    connect(button1,&QPushButton::clicked,this,&PackageSheetConfig::onConfigureButtonClicked);
    connect(button2,&QPushButton::clicked,this,&PackageSheetConfig::onEditButtonClicked);
    connect(button3,&QPushButton::clicked,this,&PackageSheetConfig::onDeleteButtonClicked);
    connect(button4,&QPushButton::clicked,this,&PackageSheetConfig::onNewButtonClicked);
    connect(tableWidget,&QTableWidget::currentItemChanged,this,&PackageSheetConfig::onItemChanged);
}
void WholeSheetConfigTab::onQCheckBoxChanged(bool checked)
{
    if(!checked) return;
        TailPieceMixing->setChecked(checked);
        TailLineMixing->setChecked(checked);
        SameTypeSizeMixing->setChecked(checked);
}
void WholeSheetConfigTab::onQCheckBoxChanged1(bool checked)
{
    if(!checked)
    {
       AllMixing->setChecked(checked);
    }
}
void WholeSheetConfigTab::onQCheckBoxChanged2(bool checked)
{
    if(!checked)
    {
       AllMixing->setChecked(checked);
    }
}
void WholeSheetConfigTab::onQCheckBoxChanged3(bool checked)
{
    if(!checked)
    {
       AllMixing->setChecked(checked);
    }
}

void PackageSheetConfig::setTableWidget()
{
    //tableWidget->clear();
    for(int row = 0; row<this->dataList.length(); row++){
        tableWidget->insertRow(row);
        QTableWidgetItem * type = new QTableWidgetItem(tr("Package"));
        type->setFlags(type->flags()&(~Qt::ItemIsEditable));
        tableWidget->setItem(row,0,type);
        QString  name = QString("%1").arg(row+1);
        name = "材料"+name;
        tableWidget->setItem(row,1,new QTableWidgetItem(name));
    }
}

void PackageSheetConfig::onConfigureButtonClicked()
{
    emit pDataChanged(2,this->dataList);
    this->tableWidget->clearSelection();
    this->degree->setText(QString("%1").arg(0));
    this->HorizontalNest->setChecked(true);
    this->VerticalNest->setChecked(false);
    this->TailPieceMixing->setChecked(false);
    this->TailLineMixing->setChecked(false);
    this->SameTypeSizeMixing->setChecked(false);
    this->AllMixing->setChecked(false);

    //这部分代码用来处理 edit
    this->button1->setEnabled(false);
    this->button2->setEnabled(false);
    this->button3->setEnabled(false);
    this->button4->setEnabled(true);

    if(!this->tableWidget->currentItem())
        return;
    int index = this->tableWidget->currentItem()->row();

    this->dataList[index][0] = this->degree->text().toInt();

    if(this->VerticalNest->isChecked()){
        this->dataList[index][1] = 1;
    }
    else{
        this->dataList[index][2] = 1;
    }

    if(this->AllMixing->isChecked()){
        this->dataList[index][3] = 1;
        this->dataList[index][4] = 1;
        this->dataList[index][5] = 1;
        this->dataList[index][6] = 1;
    }
    else if(this->TailPieceMixing->isChecked()&&!this->TailLineMixing->isChecked()&&
            !this->SameTypeSizeMixing->isChecked()){
        this->dataList[index][3] = 1;
        this->dataList[index][4] = 0;
        this->dataList[index][5] = 0;
        this->dataList[index][6] = 0;
    }

    else if(!this->TailPieceMixing->isChecked()&&this->TailLineMixing->isChecked()&&
            !this->SameTypeSizeMixing->isChecked()){
        this->dataList[index][3] = 0;
        this->dataList[index][4] = 1;
        this->dataList[index][5] = 0;
        this->dataList[index][6] = 0;
    }

    else if(!this->TailPieceMixing->isChecked()&&!this->TailLineMixing->isChecked()&&
            this->SameTypeSizeMixing->isChecked()){
        this->dataList[index][3] = 0;
        this->dataList[index][4] = 0;
        this->dataList[index][5] = 1;
        this->dataList[index][6] = 0;
    }

    else if(this->TailPieceMixing->isChecked()&&this->TailLineMixing->isChecked()&&
            !this->SameTypeSizeMixing->isChecked()){
        this->dataList[index][3] = 1;
        this->dataList[index][4] = 1;
        this->dataList[index][5] = 0;
        this->dataList[index][6] = 0;
    }

    else if(!this->TailPieceMixing->isChecked()&&this->TailLineMixing->isChecked()&&
            !this->SameTypeSizeMixing->isChecked()){
        this->dataList[index][3] = 0;
        this->dataList[index][4] = 1;
        this->dataList[index][5] = 1;
        this->dataList[index][6] = 0;
    }

    else if(this->TailPieceMixing->isChecked()&&!this->TailLineMixing->isChecked()&&
            this->SameTypeSizeMixing->isChecked()){
        this->dataList[index][3] = 1;
        this->dataList[index][4] = 0;
        this->dataList[index][5] = 1;
        this->dataList[index][6] = 0;
    }
    emit pDataChanged(2,this->dataList);

}

void PackageSheetConfig::onEditButtonClicked()
{
    //this->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
    this->degree->setEnabled(true);
    this->HorizontalNest->setEnabled(true);
    this->VerticalNest->setEnabled(true);
    this->TailPieceMixing->setEnabled(true);
    this->TailLineMixing->setEnabled(true);
    this->SameTypeSizeMixing->setEnabled(true);
    this->AllMixing->setEnabled(true);

    this->button1->setEnabled(true);
    this->button3->setEnabled(false);
    this->button4->setEnabled(false);
}

void PackageSheetConfig::onDeleteButtonClicked()
{
    if(this->tableWidget->currentItem()!=NULL)
    {
        int row = this->tableWidget->currentItem()->row();
        this->dataList.removeAt(row);
        emit pDataChanged(2, this->dataList);
        //bug是不能一开始就删除第一个！！！
        this->tableWidget->removeRow(row);
    }

    this->button1->setEnabled(true);
    this->button2->setEnabled(false);
    this->button3->setEnabled(false);
    //所有选项不可编辑
    this->degree->setText(QString("%1").arg(0));

    this->HorizontalNest->setChecked(false);
    this->VerticalNest->setChecked(false);

    this->TailPieceMixing->setChecked(false);
    this->TailLineMixing->setChecked(false);
    this->SameTypeSizeMixing->setChecked(false);
    this->AllMixing->setChecked(false);
}

void PackageSheetConfig::onNewButtonClicked()
{
    QList<int> newConfigureList;
    newConfigureList.append(this->degree->text().toInt());
    if(this->HorizontalNest->isChecked()){
        newConfigureList.append(1);
        newConfigureList.append(0);
    }
    else{
        newConfigureList.append(0);
        newConfigureList.append(1);
    }
    if(this->AllMixing->isChecked()){
        newConfigureList.append(1);
        newConfigureList.append(1);
        newConfigureList.append(1);
        newConfigureList.append(1);
    }
    else if(this->TailPieceMixing->isChecked()&&!this->TailLineMixing->isChecked()
            &&!this->SameTypeSizeMixing->isChecked()){
        newConfigureList.append(1);
        newConfigureList.append(0);
        newConfigureList.append(0);
        newConfigureList.append(0);
    }
    else if(!this->TailPieceMixing->isChecked()&&this->TailLineMixing->isChecked()
            &&!this->SameTypeSizeMixing->isChecked()){
        newConfigureList.append(0);
        newConfigureList.append(1);
        newConfigureList.append(0);
        newConfigureList.append(0);

    }
    else if(!this->TailPieceMixing->isChecked()&&!this->TailLineMixing->isChecked()
            &&this->SameTypeSizeMixing->isChecked()){
        newConfigureList.append(0);
        newConfigureList.append(0);
        newConfigureList.append(1);
        newConfigureList.append(0);

    }
    else if(this->TailPieceMixing->isChecked()&&this->TailLineMixing->isChecked()
            &&!this->SameTypeSizeMixing->isChecked()){
        newConfigureList.append(1);
        newConfigureList.append(1);
        newConfigureList.append(0);
        newConfigureList.append(0);

    }
    else if(this->TailPieceMixing->isChecked()&&!this->TailLineMixing->isChecked()
            &&this->SameTypeSizeMixing->isChecked()){
        newConfigureList.append(1);
        newConfigureList.append(0);
        newConfigureList.append(1);
        newConfigureList.append(0);

    }
    else if(!this->TailPieceMixing->isChecked()&&this->TailLineMixing->isChecked()
            &&this->SameTypeSizeMixing->isChecked()){
        newConfigureList.append(0);
        newConfigureList.append(1);
        newConfigureList.append(1);
        newConfigureList.append(0);

    }
    else{
        newConfigureList.append(0);
        newConfigureList.append(0);
        newConfigureList.append(0);
        newConfigureList.append(0);
    }
    this->dataList.append(newConfigureList);

    int row = this->tableWidget->rowCount();
    this->tableWidget->insertRow(row);
    QTableWidgetItem * type = new QTableWidgetItem(tr("Package"));
    type->setFlags(type->flags()&(~Qt::ItemIsEditable));
    tableWidget->setItem(row,0,type);
    QString  name = QString("%1").arg(row+1);
    name = "材料"+name;
    tableWidget->setItem(row,1,new QTableWidgetItem(name));
    this->tableWidget->clearSelection();
    //this->tableWidget

    this->button2->setEnabled(false);
    this->button3->setEnabled(false);
    this->button1->setEnabled(!this->button1->isChecked());
}

void PackageSheetConfig::onItemChanged()
{
    if(this->dataList.length() == 0)
        return;

    int row = this->tableWidget->currentItem()->row();
    qDebug()<<"row::"<<row;
    //this->button1->setEnabled(true);
    this->button4->setEnabled(false);
    this->button2->setEnabled(true);
    this->button3->setEnabled(true);
    QList<int> configurelist = this->dataList[row];
    this->degree->setText(QString("%1").arg(configurelist[0]));
    if(configurelist[1] == 1)
        this->HorizontalNest->setChecked(true);
    else
        this->VerticalNest->setChecked(true);
    if(configurelist[6] == 1){
        this->TailPieceMixing->setChecked(true);
        this->TailLineMixing->setChecked(true);
        this->SameTypeSizeMixing->setChecked(true);
        this->AllMixing->setChecked(true);
    }
    else if(configurelist[3] == 1&&configurelist[4] == 0&&configurelist[5]==0){
        this->TailPieceMixing->setChecked(true);
        this->TailLineMixing->setChecked(false);
        this->SameTypeSizeMixing->setChecked(false);
        this->AllMixing->setChecked(false);
    }
    else if(configurelist[3] == 0&&configurelist[4] == 1&&configurelist[5]==0){
        this->TailPieceMixing->setChecked(false);
        this->TailLineMixing->setChecked(true);
        this->SameTypeSizeMixing->setChecked(false);
        this->AllMixing->setChecked(false);
    }
    else if(configurelist[3] == 0&&configurelist[4] == 0&&configurelist[5]==1){
        this->TailPieceMixing->setChecked(false);
        this->TailLineMixing->setChecked(false);
        this->SameTypeSizeMixing->setChecked(true);
        this->AllMixing->setChecked(false);
    }
    else if(configurelist[3] == 1&&configurelist[4] == 1&&configurelist[5]==0){
        this->TailPieceMixing->setChecked(true);
        this->TailLineMixing->setChecked(true);
        this->SameTypeSizeMixing->setChecked(false);
        this->AllMixing->setChecked(false);
    }
    else if(configurelist[3] == 1&&configurelist[4] == 0&&configurelist[5]==1){
        this->TailPieceMixing->setChecked(true);
        this->TailLineMixing->setChecked(false);
        this->SameTypeSizeMixing->setChecked(true);
        this->AllMixing->setChecked(false);
    }
    else if(configurelist[3] == 0&&configurelist[4] == 1&&configurelist[5]==1){
        this->TailPieceMixing->setChecked(false);
        this->TailLineMixing->setChecked(true);
        this->SameTypeSizeMixing->setChecked(true);
        this->AllMixing->setChecked(false);
    }
}

void PackageSheetConfig::onQCheckBoxChanged11(bool checked)
{
    if(!checked) return;
        TailPieceMixing->setChecked(checked);
        TailLineMixing->setChecked(checked);
        SameTypeSizeMixing->setChecked(checked);
}
void PackageSheetConfig::onQCheckBoxChanged12(bool checked)
{
    if(!checked)
    {
       AllMixing->setChecked(checked);
    }
}
void PackageSheetConfig::onQCheckBoxChanged13(bool checked)
{
    if(!checked)
    {
       AllMixing->setChecked(checked);
    }
}
void PackageSheetConfig::onQCheckBoxChanged14(bool checked)
{
    if(!checked)
    {
       AllMixing->setChecked(checked);
    }
}

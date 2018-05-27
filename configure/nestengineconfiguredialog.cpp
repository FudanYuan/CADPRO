#include "nestengineconfiguredialog.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QSizePolicy>
#include <QList>

#include <QDebug>
NestEngineConfigureDialog::NestEngineConfigureDialog(NestEngineConfigure *config) :
    tabType(Default)
{
    setWindowTitle(tr("排版引擎配置"));

    // 适应屏幕大小
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    setFixedSize(screenRect.width() - 750, screenRect.height() - 450);
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));

    this->dataMap = config->LoadConfigureXml();



    foreach(QList<QList<int>> test, this->dataMap.values()){
        int i,j;
        for(i = 0; i<test.length(); i++){
            for(j = 0; j<test[i].length(); j++){
                qDebug()<<"after read";
                qDebug()<<test[i][j];
            }
        }
    }

    // 添加组件
    tabWidget = new QTabWidget(this);
    connect(tabWidget, &QTabWidget::currentChanged, this, &NestEngineConfigureDialog::onTabChanged);

    wSheetTab = new WholeSheetConfigTab(this);
    tabWidget->addTab(wSheetTab, tr("整体材料"));
    sSheetTab = new StripSheetConfigTab(this);
    tabWidget->addTab(sSheetTab, tr("条板材料"));

    pSheetTab = new PackageSheetConfig(this);
    tabWidget->addTab(pSheetTab, tr("卷装材料"));

    // 将确定取消的按键加入布局
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok,this);
    connect(buttonBox, &QDialogButtonBox::clicked, this, &NestEngineConfigureDialog::onDialogButtonClicked);

}

NestEngineConfigureDialog::NestEngineConfigureDialog(NestEngineConfigure *config, TabType type) :
    NestEngineConfigureDialog(config)

{
    //this->dataMap = config->LoadConfigureXml();


    onTabChanged(type);
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

QTableWidget *NestEngineConfigureDialog::inittableWIdget()
{
    QTableWidget * widget = new QTableWidget();
    return widget;
}
void NestEngineConfigureDialog::onTabChanged(int i)
{
    tabType = (TabType)i;
    switch (i) {
    case 0:
        tabWidget->setCurrentIndex(0);
        tabWidget->setTabEnabled(1, false);
        tabWidget->setTabEnabled(2, false);
        break;
    case 1:
        tabWidget->setCurrentIndex(1);
        tabWidget->setTabEnabled(0, false);
        tabWidget->setTabEnabled(2, false);
        break;
    case 2:
        tabWidget->setCurrentIndex(2);
        tabWidget->setTabEnabled(0, false);
        tabWidget->setTabEnabled(1, false);
        break;
    default:
        break;
    }
}

void NestEngineConfigureDialog::onDialogButtonClicked(QAbstractButton *button)
{
    qDebug() << button->text();
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
        else if(sSheetTab->VerticalAdaptiveSpacing->isChecked())
        {
            curStripConfig->stripmixing=NestEngine::NoMixing;
        }
        break;
    case Package:
        curPackageConfig = new NestEngineConfigure::PackageSheetNest();
        //方向排版
        curPackageConfig->packagedegree=pSheetTab->degree1->text().toInt();
        if(pSheetTab->HorizontalNest1->isChecked())
        {
            curPackageConfig->packageorientation=NestEngine::HorizontalNest;
        }
        else if(pSheetTab->VerticalNest1->isChecked())
        {
            curPackageConfig->packageorientation=NestEngine::VerticalNest;
        }
        //混合方式
        if(pSheetTab->AllMixing1->isChecked())
        {
            curPackageConfig->packagemixing = NestEngine::AllMixing;
        }
        else if(pSheetTab->TailPieceMixing1->isChecked() && pSheetTab->TailLineMixing1->isChecked()
                &&!pSheetTab->SameTypeSizeMixing1->isChecked())
        {
            curPackageConfig->packagemixing =NestEngine::TailPieceMixing|NestEngine::TailLineMixing;
        }
        else if(pSheetTab->TailPieceMixing1->isChecked() && !pSheetTab->TailLineMixing1->isChecked()
                &&pSheetTab->SameTypeSizeMixing1->isChecked())
        {
            qDebug()<<"运行到这里";
            curPackageConfig->packagemixing =NestEngine::TailPieceMixing|NestEngine::SameTypeSizeMixing;
        }
        else if(!pSheetTab->TailPieceMixing1->isChecked() && pSheetTab->TailLineMixing1->isChecked()
                &&pSheetTab->SameTypeSizeMixing1->isChecked())
        {
            curPackageConfig->packagemixing =NestEngine::TailLineMixing|NestEngine::SameTypeSizeMixing;
        }
        else if(pSheetTab->TailPieceMixing1->isChecked() && !pSheetTab->TailLineMixing1->isChecked()
                &&!pSheetTab->SameTypeSizeMixing1->isChecked())
        {
            curPackageConfig->packagemixing = NestEngine::TailPieceMixing;
        }
        else if(!pSheetTab->TailPieceMixing1->isChecked() && pSheetTab->TailLineMixing1->isChecked()
                &&!pSheetTab->SameTypeSizeMixing1->isChecked())
        {
            curPackageConfig->packagemixing = NestEngine::TailLineMixing;
        }
        else if(!pSheetTab->TailPieceMixing1->isChecked() && !pSheetTab->TailLineMixing1->isChecked()
                &&pSheetTab->SameTypeSizeMixing1->isChecked())
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

WholeSheetConfigTab::WholeSheetConfigTab(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle(tr("排版设置"));
    QGridLayout *mainlayout1=new QGridLayout();
    QHBoxLayout *mainlayout=new QHBoxLayout();
    QVBoxLayout *vLayout1 = new QVBoxLayout();
    QVBoxLayout *vLayout2 = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();
    QHBoxLayout *hLayout1 = new QHBoxLayout();

    QLabel *label1 =new QLabel(tr("摆动角度"));
    degree =new QLineEdit;
    degree->setText(tr("0"));
    degree->setValidator(new QIntValidator(0,90, this));//限制只能输入数字0~90
    QLabel *label2 =new QLabel(tr("度"));

    HorizontalNest = new QRadioButton(tr("横向"));
    VerticalNest = new QRadioButton(tr("纵向"));
    HorizontalNest->setChecked(true);



    hLayout->addWidget(label1);
    hLayout->addWidget(degree);
    hLayout->addWidget(HorizontalNest);
    hLayout->addWidget(VerticalNest);

    vLayout1->addLayout(hLayout);
    vLayout1->addStretch(0.6);
    TailPieceMixing = new QCheckBox(tr("尾只混合"), this);
    TailLineMixing = new QCheckBox(tr("尾行优化，允许随意角度"));
    SameTypeSizeMixing = new QCheckBox(tr("同型体内尺码混合"));
    AllMixing = new QCheckBox(tr("全混合"));

    tableWidget = new QTableWidget(0,2);
    tableWidget->setShowGrid(true);
    QStringList m_Header;
    m_Header<<tr("类型")<<tr("名称");
    tableWidget->setHorizontalHeaderLabels(m_Header);

    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    tableWidget->setFrameShape(QFrame::Box);//设置边框
    tableWidget->setShowGrid(true); //设置显示格子线
    vLayout2->addWidget(tableWidget);
    button1 =new QPushButton(tr("确定"));
    button2 =new QPushButton(tr("编辑"));
    button3 =new QPushButton(tr("删除"));


    tableWidget->setItem(0,0,new QTableWidgetItem("Whole"));
    tableWidget->setItem(0,1,new QTableWidgetItem("1"));



//    foreach(int index,mapData.keys()){
//        switch (index) {
//        case 0:
//            for(int i = 0; i<mapData[0].length(); i++){
//                tableWidget->setItem(i,0,new QTableWidgetItem("Whole"));
//                tableWidget->setItem(i,1,new QTableWidgetItem(i));
//            }
//            break;
//        default:
//            break;
//        }
//    }
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//默认不可编辑
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
    tableWidget->show();

    hLayout1->addWidget(button1);
    hLayout1->addWidget(button2);
    hLayout1->addWidget(button3);
    vLayout2->addLayout(hLayout1);
    vLayout1->addWidget(TailPieceMixing);
    vLayout1->addStretch(0.6);
    vLayout1->addWidget(TailLineMixing);
    vLayout1->addStretch(0.6);
    vLayout1->addWidget(SameTypeSizeMixing);
    vLayout1->addStretch(0.6);
    vLayout1->addWidget(AllMixing);
    vLayout1->addStretch(2.5);
    mainlayout->addLayout(vLayout1);
    mainlayout->addLayout(vLayout2);
    mainlayout->setStretch(0,8);
    mainlayout->setStretch(1,25);
    setLayout(mainlayout);
    connect(AllMixing, &QCheckBox::toggled, this, &WholeSheetConfigTab::onQCheckBoxChanged);
    connect(TailPieceMixing, &QCheckBox::toggled, this, &WholeSheetConfigTab::onQCheckBoxChanged1);
    connect(TailLineMixing, &QCheckBox::toggled, this, &WholeSheetConfigTab::onQCheckBoxChanged2);
    connect(SameTypeSizeMixing, &QCheckBox::toggled, this, &WholeSheetConfigTab::onQCheckBoxChanged3);
}

StripSheetConfigTab::StripSheetConfigTab(QWidget *parent) :
    QWidget(parent)
{
     setWindowTitle(tr("条板设置"));
     QHBoxLayout *mainlayout = new QHBoxLayout();
     QVBoxLayout *layout = new QVBoxLayout();
     QVBoxLayout *layout1 = new QVBoxLayout();
     QHBoxLayout *hLayout1= new QHBoxLayout();
     QLabel *label1 =new QLabel(tr("排版设置"));
     QLabel *label2 =new QLabel(tr("排版方式"));
     leftRightTurnCBox = new QCheckBox(tr("左右交替"));
     sizeDownCBox = new QCheckBox(tr("尺码大到小"));
     HorizontalAdaptiveSpacing = new QRadioButton(tr("横向自适应间距"));
     VerticalAdaptiveSpacing = new QRadioButton(tr("尾只混合"));
     HorizontalAdaptiveSpacing->setChecked(true);
     layout->addWidget(label1);
     layout->addStretch(0.6);
     layout->addWidget(label2);
     layout->addStretch(0.6);
     layout->addWidget(leftRightTurnCBox);
     layout->addStretch(0.6);
     layout->addWidget(sizeDownCBox);
     layout->addStretch(0.6);
     layout->addWidget(HorizontalAdaptiveSpacing);
     layout->addStretch(0.6);
     layout->addWidget(VerticalAdaptiveSpacing);
     layout->addStretch(2.5);
     mainlayout->addLayout(layout);
     tableWidget = new QTableWidget(0,3);
     tableWidget->setShowGrid(true);
     QStringList m_Header;
     m_Header<<tr("1")<<tr("2")<<tr("3");
     tableWidget->setHorizontalHeaderLabels(m_Header);
     tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
     tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
     layout1->addWidget(tableWidget);
     button1 =new QPushButton(tr("确定"));
     button2 =new QPushButton(tr("编辑"));
     button3 =new QPushButton(tr("删除"));
     hLayout1->addWidget(button1);
     hLayout1->addWidget(button2);
     hLayout1->addWidget(button3);
     layout1->addLayout(hLayout1);
     mainlayout->addLayout(layout1);
     setLayout(mainlayout);
     mainlayout->setStretch(0,13);
     mainlayout->setStretch(1,13);
}

PackageSheetConfig::PackageSheetConfig(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle(tr("排版设置"));
    QHBoxLayout *mainlayout=new QHBoxLayout();
    QVBoxLayout *vLayout1 = new QVBoxLayout();
    QVBoxLayout *vLayout2 = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();
    QHBoxLayout *hLayout1 = new QHBoxLayout();
    QLabel *label1 =new QLabel(tr("摆动角度"));
    degree1 =new QLineEdit;
    degree1->setText(tr("0"));
    degree1->setValidator(new QIntValidator(0,90, this));//限制只能输入数字0~90
    QLabel *label2 =new QLabel(tr("度"));

    HorizontalNest1 = new QRadioButton(tr("横向"));
    VerticalNest1 = new QRadioButton(tr("纵向"));
    HorizontalNest1->setChecked(true);
    hLayout->addWidget(label1);
    hLayout->addWidget(degree1);
    hLayout->addWidget(HorizontalNest1);
    hLayout->addWidget(VerticalNest1);

    vLayout1->addLayout(hLayout);
    vLayout1->addStretch(0.6);
    TailPieceMixing1 = new QCheckBox(tr("尾只混合"), this);
    TailLineMixing1 = new QCheckBox(tr("尾行优化，允许随意角度"));
    SameTypeSizeMixing1 = new QCheckBox(tr("同型体内尺码混合"));
    AllMixing1 = new QCheckBox(tr("全混合"));

    tableWidget1 = new QTableWidget(0,3);
    tableWidget1->setShowGrid(true);
    QStringList m_Header;
    m_Header<<tr("1")<<tr("2")<<tr("3");
    tableWidget1->setHorizontalHeaderLabels(m_Header);
    tableWidget1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget1->setSelectionBehavior(QAbstractItemView::SelectRows);
    vLayout2->addWidget(tableWidget1);
    button1 =new QPushButton(tr("确定"));
    button2 =new QPushButton(tr("编辑"));
    button3 =new QPushButton(tr("删除"));
    hLayout1->addWidget(button1);
    hLayout1->addWidget(button2);
    hLayout1->addWidget(button3);
    vLayout2->addLayout(hLayout1);
    vLayout1->addWidget(TailPieceMixing1);
    vLayout1->addStretch(0.6);
    vLayout1->addWidget(TailLineMixing1);
    vLayout1->addStretch(0.6);
    vLayout1->addWidget(SameTypeSizeMixing1);
    vLayout1->addStretch(0.6);
    vLayout1->addWidget(AllMixing1);
    vLayout1->addStretch(2.5);
    mainlayout->addLayout(vLayout1);
    mainlayout->addLayout(vLayout2);
    mainlayout->setStretch(0,13);
    mainlayout->setStretch(1,13);
    setLayout(mainlayout);
    connect(AllMixing1, &QCheckBox::toggled, this, &PackageSheetConfig::onQCheckBoxChanged11);
    connect(TailPieceMixing1, &QCheckBox::toggled, this, &PackageSheetConfig::onQCheckBoxChanged12);
    connect(TailLineMixing1, &QCheckBox::toggled, this, &PackageSheetConfig::onQCheckBoxChanged13);
    connect(SameTypeSizeMixing1, &QCheckBox::toggled, this, &PackageSheetConfig::onQCheckBoxChanged14);
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
void PackageSheetConfig::onQCheckBoxChanged11(bool checked)
{
    if(!checked) return;
        TailPieceMixing1->setChecked(checked);
        TailLineMixing1->setChecked(checked);
        SameTypeSizeMixing1->setChecked(checked);
}
void PackageSheetConfig::onQCheckBoxChanged12(bool checked)
{
    if(!checked)
    {
       AllMixing1->setChecked(checked);
    }
}
void PackageSheetConfig::onQCheckBoxChanged13(bool checked)
{
    if(!checked)
    {
       AllMixing1->setChecked(checked);
    }
}
void PackageSheetConfig::onQCheckBoxChanged14(bool checked)
{
    if(!checked)
    {
       AllMixing1->setChecked(checked);
    }
}

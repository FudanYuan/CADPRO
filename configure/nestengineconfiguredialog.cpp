#include "nestengineconfiguredialog.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QSizePolicy>

#include <QDebug>
#include <QList>
NestEngineConfigureDialog::NestEngineConfigureDialog(NestEngineConfigure *config) :
    tabType(Default)
{
    setWindowTitle(tr("排版引擎配置"));

    // 适应屏幕大小
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    setFixedSize(screenRect.width() - 800, screenRect.height() - 400);
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));

    // 添加组件
    tabWidget = new QTabWidget(this);
    connect(tabWidget, &QTabWidget::currentChanged, this, &NestEngineConfigureDialog::onTabChanged);

    wSheetTab = new WholeSheetConfigTab(config->getWholeSheetNest(),this);
    tabWidget->addTab(wSheetTab, tr("整体材料"));

    sSheetTab = new StripSheetConfigTab(this);
    tabWidget->addTab(sSheetTab, tr("条板材料"));

    pSheetTab = new PackageSheetConfig(this);
    tabWidget->addTab(pSheetTab, tr("卷装材料"));

    // 将确定取消的按键加入布局
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Apply
                                     | QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel, this);
//    buttonBox->button(QDialogButtonBox::Apply)->setDisabled(true);
//    buttonBox->button(QDialogButtonBox::Cancel)->setDisabled(true);
    connect(buttonBox, &QDialogButtonBox::clicked, this, &NestEngineConfigureDialog::onDialogButtonClicked);

}

NestEngineConfigureDialog::NestEngineConfigureDialog(NestEngineConfigure *config, TabType type) :
    NestEngineConfigureDialog(config)

{
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
    QList<int> configureList;
    switch (tabType) {
    case Whole:
    {
        curWholeConfig = new NestEngineConfigure::WholeSheetNest();
        configureList.append(45);
        configureList.append(0);
        configureList.append(0);
        if(wSheetTab == NULL){
            qDebug() << "wSheetTab == NULL";
            return;
        }
        if(wSheetTab->TailLineMixing == NULL){
            qDebug() << "TailLineMixing == NULL";
            return;
        }
        //qDebug()<<wSheetTab->TailPieceMixing->isChecked()<<"test";
        if((wSheetTab->TailPieceMixing->isChecked()&&
                wSheetTab->TailLineMixing->isChecked()&&
                wSheetTab->SameTypeSizeMixing->isChecked()&&
                (!wSheetTab->AllMixing->isChecked()))||(wSheetTab->AllMixing->isChecked()))
        {
            curWholeConfig->mixtype = NestEngine::AllMixing;
            configureList.append(1);
            configureList.append(1);
            configureList.append(1);
            configureList.append(1);
        }
        else if(wSheetTab->TailPieceMixing->isChecked() && !wSheetTab->TailLineMixing->isChecked()
                &&!wSheetTab->SameTypeSizeMixing->isChecked()&&!wSheetTab->AllMixing->isChecked())
        {
            curWholeConfig->mixtype = NestEngine::TailPieceMixing;
            configureList.append(1);
            configureList.append(0);
            configureList.append(0);
            configureList.append(0);
        }
        else if(!wSheetTab->TailPieceMixing->isChecked() && wSheetTab->TailLineMixing->isChecked()
                &&!wSheetTab->SameTypeSizeMixing->isChecked()&&!wSheetTab->AllMixing->isChecked())
        {
            curWholeConfig->mixtype = NestEngine::TailLineMixing;
            configureList.append(0);
            configureList.append(1);
            configureList.append(0);
            configureList.append(0);
        }
        else if(!wSheetTab->TailPieceMixing->isChecked() && !wSheetTab->TailLineMixing->isChecked()
                &&wSheetTab->SameTypeSizeMixing->isChecked()&&!wSheetTab->AllMixing->isChecked())
        {
            curWholeConfig->mixtype = NestEngine::SameTypeSizeMixing;
            configureList.append(0);
            configureList.append(0);
            configureList.append(0);
            configureList.append(1);
        }
        else if(!wSheetTab->TailPieceMixing->isChecked() && !wSheetTab->TailLineMixing->isChecked()
                &&!wSheetTab->SameTypeSizeMixing->isChecked()&&!wSheetTab->AllMixing->isChecked())
        {
            curWholeConfig->mixtype = NestEngine::NoMixing;
            configureList.append(0);
            configureList.append(0);
            configureList.append(0);
            configureList.append(0);
        }
        else if(wSheetTab->TailPieceMixing->isChecked() && wSheetTab->TailLineMixing->isChecked()
                &&!wSheetTab->SameTypeSizeMixing->isChecked()&&!wSheetTab->AllMixing->isChecked()){
            curWholeConfig->mixtype = NestEngine::TailPieceMixing|NestEngine::TailLineMixing;
            configureList.append(1);
            configureList.append(1);
            configureList.append(0);
            configureList.append(0);
        }
        else if(wSheetTab->TailPieceMixing->isChecked() && !wSheetTab->TailLineMixing->isChecked()
                &&wSheetTab->SameTypeSizeMixing->isChecked()&&!wSheetTab->AllMixing->isChecked()){
            curWholeConfig->mixtype = NestEngine::TailPieceMixing |NestEngine::SameTypeSizeMixing;
            configureList.append(1);
            configureList.append(0);
            configureList.append(1);
            configureList.append(0);
        }
        else if(!wSheetTab->TailPieceMixing->isChecked() && wSheetTab->TailLineMixing->isChecked()
                &&wSheetTab->SameTypeSizeMixing->isChecked()&&!wSheetTab->AllMixing->isChecked()){
            curWholeConfig->mixtype = NestEngine::TailLineMixing | NestEngine::SameTypeSizeMixing;
            configureList.append(0);
            configureList.append(1);
            configureList.append(1);
            configureList.append(0);
        }
        NestEngineConfigure *configure = new NestEngineConfigure();
        configure->WriteConfigureXml(0,configureList);
        break;
    }
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
        break;
    case Package:

        break;
    default:
        break;
    }
    QDialog::accept();
}

WholeSheetConfigTab::WholeSheetConfigTab(NestEngineConfigure::WholeSheetNest wholesheetnest, QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle(tr("排版设置"));
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QHBoxLayout *hLayout = new QHBoxLayout();
    QLabel *label1 =new QLabel(tr("摆动角度"));
    QLineEdit *edit1 =new QLineEdit;
    edit1->setText(tr("0"));
    edit1->setValidator(new QIntValidator(0,90, this));//限制只能输入数字0~90
    QLabel *label2 =new QLabel(tr("度"));

    QRadioButton *radio1 = new QRadioButton(tr("纵向"));
    QRadioButton *radio2 = new QRadioButton(tr("横向"));

    hLayout->addWidget(label1);
    hLayout->addWidget(edit1);
    hLayout->addWidget(radio1);
    hLayout->addWidget(radio2);

    mainLayout->addLayout(hLayout);
    /*
    leftRightTurnCBox = new QCheckBox(tr("左右交替"));
    sizeDownCBox = new QCheckBox(tr("尺码大到小"));*/
    TailPieceMixing = new QCheckBox(tr("尾只混合"), this);
    TailLineMixing = new QCheckBox(tr("尾行优化，允许随意角度"));
    SameTypeSizeMixing = new QCheckBox(tr("同型体内尺码混合"));
    AllMixing = new QCheckBox(tr("全混合"));
    //if(wholesheetnest.mixtype == NestEngine::NoMixing){
       // qDebug()<<"No Mixing!";
    //}
    //qDebug()<<wholesheetnest.mixtype<<"r";
    //qDebug()<<(NestEngine::AllMixing)<<"ALL";
    //if(NestEngine::AllMixing ==
           // (NestEngine::TailLineMixing|NestEngine::SameTypeSizeMixing)){
        //qDebug()<<"equal";
    //}
    //qDebug()<<(NestEngine::TailLineMixing|NestEngine::SameTypeSizeMixing)<<"TS";
    if(wholesheetnest.mixtype == NestEngine::TailPieceMixing){
        TailPieceMixing->setChecked(true);
    }
    else if(wholesheetnest.mixtype == NestEngine::TailLineMixing){
        TailLineMixing->setChecked(true);
    }
    else if(wholesheetnest.mixtype == NestEngine::SameTypeSizeMixing){
        SameTypeSizeMixing->setChecked(true);
    }
    else if(wholesheetnest.mixtype == (NestEngine::TailPieceMixing|NestEngine::TailLineMixing)){
        TailPieceMixing->setChecked(true);
        TailLineMixing->setChecked(true);
    }
    else if(wholesheetnest.mixtype == (NestEngine::TailPieceMixing|NestEngine::SameTypeSizeMixing)){
        TailPieceMixing->setChecked(true);
        SameTypeSizeMixing->setChecked(true);
    }
    else if(wholesheetnest.mixtype == (NestEngine::TailLineMixing|NestEngine::SameTypeSizeMixing)){
        TailLineMixing->setChecked(true);
        SameTypeSizeMixing->setChecked(true);
        //qDebug()<<"23";
    }
    else if(wholesheetnest.mixtype == NestEngine::AllMixing){
        TailPieceMixing->setChecked(true);
        TailLineMixing->setChecked(true);
        //qDebug()<<"4";
        SameTypeSizeMixing->setChecked(true);
        AllMixing->setChecked(true);
    }

    mainLayout->addWidget(TailPieceMixing);
    mainLayout->addWidget(TailLineMixing);
    mainLayout->addWidget(SameTypeSizeMixing);
    mainLayout->addWidget(AllMixing);
    setLayout(mainLayout);
//    QVBoxLayout *layout = new QVBoxLayout();
//    QLabel *label1 =new QLabel(tr("排版设置"));
//    QLabel *label2 =new QLabel(tr("排版方式"));
//    QLineEdit *edit2 =new QLineEdit;
//    edit2->setText(tr("左右交替/尺码大到小"));
//    QCheckBox *checkBox1 = new QCheckBox(tr("1"));
//    QCheckBox *checkBox2 = new QCheckBox(tr("2"));

//    QRadioButton *radio1 = new QRadioButton(tr("横向自适应间距"));
//    QRadioButton *radio2 = new QRadioButton(tr("位置混合"));
//    layout->addWidget(label1);
//    layout->addWidget(label2);
//    layout->addWidget(checkBox1);
//    layout->addWidget(checkBox2);
//    layout->addWidget(radio1);
//    layout->addWidget(radio2);
//    setLayout(layout);
    //init();
}

StripSheetConfigTab::StripSheetConfigTab(QWidget *parent) :
    QWidget(parent)
{
     setWindowTitle(tr("条板设置"));

     QVBoxLayout *layout = new QVBoxLayout();
     QLabel *label1 =new QLabel(tr("排版设置"));
     QLabel *label2 =new QLabel(tr("排版方式"));
     QLineEdit *edit2 =new QLineEdit;
     edit2->setText(tr("左右交替/尺码大到小"));
     leftRightTurnCBox = new QCheckBox(tr("左右交替"));
     sizeDownCBox = new QCheckBox(tr("尺码大到小"));

     QRadioButton *radio1 = new QRadioButton(tr("横向自适应间距"));
     QRadioButton *radio2 = new QRadioButton(tr("尾只混合"));
     layout->addWidget(label1);
     layout->addWidget(label2);
     layout->addWidget(leftRightTurnCBox);
     layout->addWidget(sizeDownCBox);
     layout->addWidget(radio1);
     layout->addWidget(radio2);
     setLayout(layout);
}

PackageSheetConfig::PackageSheetConfig(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle(tr("卷装设置"));
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QHBoxLayout *hLayout = new QHBoxLayout();
    QLabel *label1 =new QLabel(tr("摆动角度"));
    QLineEdit *edit1 =new QLineEdit;
    edit1->setText(tr("0"));
    edit1->setValidator(new QIntValidator(0,90, this));//限制只能输入数字0~90
    QLabel *label2 =new QLabel(tr("度"));

    QRadioButton *radio1 = new QRadioButton(tr("纵向"));
    QRadioButton *radio2 = new QRadioButton(tr("横向"));

    hLayout->addWidget(label1);
    hLayout->addWidget(edit1);
    hLayout->addWidget(radio1);
    hLayout->addWidget(radio2);

    mainLayout->addLayout(hLayout);

    QCheckBox *checkbox1 = new QCheckBox(tr("尾只混合"));
    QCheckBox *checkbox2 = new QCheckBox(tr("尾行优化，允许随意角度"));
    QCheckBox *checkbox3 = new QCheckBox(tr("同型体内尺码混合"));
    QCheckBox *checkbox4 = new QCheckBox(tr("全混合"));

    //QDialogButtonBox *okButton = new QDialogButtonBox(QDialogButtonBox::Ok);
    //connect(okButton, &QDialogButtonBox::accepted, this, &QDialog::accept);

    mainLayout->addWidget(checkbox1);
    mainLayout->addWidget(checkbox2);
    mainLayout->addWidget(checkbox3);
    mainLayout->addWidget(checkbox4);
//    mainLayout->addWidget(okButton);
    //mainLayout->setStretch(0,10);
    setLayout(mainLayout);
//    QVBoxLayout *layout = new QVBoxLayout();
//    QLabel *label1 =new QLabel(tr("排版设置"));
//    QLabel *label2 =new QLabel(tr("排版方式"));
//    QLineEdit *edit2 =new QLineEdit;
//    edit2->setText(tr("左右交替/尺码大到小"));
//    QCheckBox *checkBox1 = new QCheckBox(tr("1"));
//    QCheckBox *checkBox2 = new QCheckBox(tr("2"));

//    QRadioButton *radio1 = new QRadioButton(tr("横向自适应间距"));
//    QRadioButton *radio2 = new QRadioButton(tr("位置混合"));
//    layout->addWidget(label1);
//    layout->addWidget(label2);
//    layout->addWidget(checkBox1);
//    layout->addWidget(checkBox2);
//    layout->addWidget(radio1);
//    layout->addWidget(radio2);
//    setLayout(layout);
}

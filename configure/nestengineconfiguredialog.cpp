#include "nestengineconfiguredialog.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QSizePolicy>

#include <QDebug>
NestEngineConfigureDialog::NestEngineConfigureDialog()
{
    setWindowTitle(tr("排版引擎配置"));

    // 适应屏幕大小
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    setFixedSize(screenRect.width() - 800, screenRect.height()-400);
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));

    // 添加组件
    tabWidget = new QTabWidget(this);
    WholeSheetConfigTab *wSheetTab = new WholeSheetConfigTab(this);
    tabWidget->addTab(wSheetTab, tr("整体材料"));

    StripSheetConfigTab *sSheetTab = new StripSheetConfigTab(this);
    tabWidget->addTab(sSheetTab, tr("条板材料"));

    PackageSheetConfig *pSheetTab = new PackageSheetConfig(this);
    tabWidget->addTab(pSheetTab, tr("卷装材料"));
    //    connect(wSheetTab, &WholeSheetConfigTab::tabChanged, this, &NestEngineConfigureDialog::onChanged);
    onNestEngingeConfig(2);
}

void NestEngineConfigureDialog::onNestEngingeConfig(int i)
{
    qDebug() << "fjkldsfla" << i;
    switch (i) {
    case 0:
        tabWidget->setCurrentIndex(0);

        break;
    case 1:
        tabWidget->setCurrentIndex(1);
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

WholeSheetConfigTab::WholeSheetConfigTab(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle(tr("排版设置"));
    QVBoxLayout *layout = new QVBoxLayout();
    QLabel *label1 =new QLabel(tr("排版设置"));
    QLabel *label2 =new QLabel(tr("排版方式"));
    QLineEdit *edit2 =new QLineEdit;
    edit2->setText(tr("左右交替/尺码大到小"));
    QCheckBox *checkBox1 = new QCheckBox(tr("1"));
    QCheckBox *checkBox2 = new QCheckBox(tr("2"));

    QRadioButton *radio1 = new QRadioButton(tr("横向自适应间距"));
    QRadioButton *radio2 = new QRadioButton(tr("位置混合"));
    layout->addWidget(label1);
    layout->addWidget(label2);
    layout->addWidget(checkBox1);
    layout->addWidget(checkBox2);
    layout->addWidget(radio1);
    layout->addWidget(radio2);
    setLayout(layout);
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
    QCheckBox *checkBox1 = new QCheckBox(tr("1"));
    QCheckBox *checkBox2 = new QCheckBox(tr("2"));

    QRadioButton *radio1 = new QRadioButton(tr("横向自适应间距"));
    QRadioButton *radio2 = new QRadioButton(tr("位置混合"));
    layout->addWidget(label1);
    layout->addWidget(label2);
    layout->addWidget(checkBox1);
    layout->addWidget(checkBox2);
    layout->addWidget(radio1);
    layout->addWidget(radio2);
    setLayout(layout);
}

PackageSheetConfig::PackageSheetConfig(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle(tr("卷装设置"));
    QVBoxLayout *layout = new QVBoxLayout();
    QLabel *label1 =new QLabel(tr("排版设置"));
    QLabel *label2 =new QLabel(tr("排版方式"));
    QLineEdit *edit2 =new QLineEdit;
    edit2->setText(tr("左右交替/尺码大到小"));
    QCheckBox *checkBox1 = new QCheckBox(tr("1"));
    QCheckBox *checkBox2 = new QCheckBox(tr("2"));

    QRadioButton *radio1 = new QRadioButton(tr("横向自适应间距"));
    QRadioButton *radio2 = new QRadioButton(tr("位置混合"));
    layout->addWidget(label1);
    layout->addWidget(label2);
    layout->addWidget(checkBox1);
    layout->addWidget(checkBox2);
    layout->addWidget(radio1);
    layout->addWidget(radio2);
    setLayout(layout);
}

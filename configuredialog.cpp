#include "configuredialog.h"
#include "ui_configuredialog.h"
#include <QDesktopWidget>
#include <QLabel>
#include <QStringList>
#include <QString>
#include <colorcombobox.h>
#include <linestylecombobox.h>
#include <QGroupBox>
#include <QLineEdit>
#include <QDebug>

ConfigureDialog::ConfigureDialog(Configure *config, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigureDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("CADPRO配置"));

    // 适应屏幕大小
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    resize(screenRect.width() - 400, screenRect.height()-200);

    // 添加组件
    tabWidget = new QTabWidget;
    EntityStyleTab *eStyleTab = new EntityStyleTab(config->eStyle);
    tabWidget->addTab(eStyleTab, tr("实体类型"));
    connect(eStyleTab, &EntityStyleTab::entityStyleChanged, config, &Configure::onConfigChanged);
    tabWidget->addTab(new AxesGridTab(), tr("轴和网格"));
    tabWidget->addTab(new OffsetTab(), tr("偏移配置"));
    tabWidget->addTab(new LanguageTab(), tr("系统语言"));

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
}

ConfigureDialog::~ConfigureDialog()
{
    qDebug() << "delete configure dialog";
    delete ui;
    delete tabWidget;
    delete buttonBox;
}

EntityStyleTab::EntityStyleTab(EntityStyle &eStyle, QWidget *parent) :
    QWidget(parent)
{
    // group box
    QGroupBox *entityGroupBox = new QGroupBox(tr("实体"), this);
    QGroupBox *moreColorGroupBox = new QGroupBox(tr("更多颜色"), this);
    QGroupBox *objectSizeGroupBox = new QGroupBox(tr("对象大小"), this);
    QGroupBox *referencePointGroupBox = new QGroupBox(tr("参考点"), this);
    QGroupBox *findPerimeterLineGroupBox = new QGroupBox(tr("查找周长线"), this);
    QGroupBox *lineStyleGroupBox = new QGroupBox(tr("线型"), this);
    QGroupBox *projectWindowGroupBox = new QGroupBox(tr("项目窗口颜色"), this);

    // grid layout
    QGridLayout *entityLayout = new QGridLayout;

    QLabel *label = new QLabel(tr(""));
    QLabel *labelColor = new QLabel(tr("颜色"));
    QLabel *labelStyle = new QLabel(tr("样式"));
    QLabel *labelWidth = new QLabel(tr("宽度"));

    QLabel *labelPerimeterLine = new QLabel(tr("周长线"));
    QLabel *labelMark = new QLabel(tr("标记"));
    QLabel *labelCut = new QLabel(tr("切割"));
    QLabel *labelStitch = new QLabel(tr("缝线"));
    QLabel *labelGeneric = new QLabel(tr("通用"));
    QLabel *labelMiddleAxis = new QLabel(tr("中心轴"));
    QLabel *labelNotchColor = new QLabel(tr("凹槽"));

    ColorComboBox *perimeterLineColor = new ColorComboBox(tr("eStyle/eStyle_permeterLine_color"), this);
    connect(perimeterLineColor, &ColorComboBox::colorChanged, this, &EntityStyleTab::onColorChanged);
    qDebug() << eStyle.perimeterLine.color;
    perimeterLineColor->setColor(eStyle.perimeterLine.color);
    ColorComboBox *markColor = new ColorComboBox(tr("eStyle/eStyle_permeterLine_color"), this);
    markColor->setColor(eStyle.mark.color);
    ColorComboBox *cutColor = new ColorComboBox(tr("eStyle/eStyle_permeterLine_color"), this);
    cutColor->setColor(eStyle.cut.color);
    ColorComboBox *stitchColor = new ColorComboBox(tr("eStyle/eStyle_permeterLine_color"), this);
    stitchColor->setColor(eStyle.stitch.color);
    ColorComboBox *genericColor = new ColorComboBox(tr("eStyle/eStyle_permeterLine_color"), this);
    genericColor->setColor(eStyle.generic.color);
    ColorComboBox *middleAxisColor = new ColorComboBox(tr("eStyle/eStyle_permeterLine_color"), this);
    middleAxisColor->setColor(eStyle.middleAxis.color);
    ColorComboBox *notchColor = new ColorComboBox(tr("eStyle/eStyle_permeterLine_color"), this);
    notchColor->setColor(eStyle.notchColor);

    lineStyleComboBox *perimeterLineStyle = new lineStyleComboBox(this);
    lineStyleComboBox *markStyle = new lineStyleComboBox(this);
    lineStyleComboBox *cutStyle = new lineStyleComboBox(this);
    lineStyleComboBox *stitchStyle = new lineStyleComboBox(this);
    lineStyleComboBox *genericStyle = new lineStyleComboBox(this);
    lineStyleComboBox *middleAxisStyle = new lineStyleComboBox(this);

    QLineEdit *perimeterLineWidth = new QLineEdit(this);
    QLineEdit *markWidth = new QLineEdit(this);
    QLineEdit *cutWidth = new QLineEdit(this);
    QLineEdit *stitchWidth = new QLineEdit(this);
    QLineEdit *genericWidth = new QLineEdit(this);
    QLineEdit *middleAxisWidth = new QLineEdit(this);

    entityLayout->addWidget(label, 0, 0, 1, 2);
    entityLayout->addWidget(labelColor, 0, 2, 1, 2);
    entityLayout->addWidget(labelStyle, 0, 4, 1, 2);
    entityLayout->addWidget(labelWidth, 0, 6, 1, 2);

    entityLayout->addWidget(labelPerimeterLine, 1, 0, 1, 2);
    entityLayout->addWidget(perimeterLineColor, 1, 2, 1, 2);
    entityLayout->addWidget(perimeterLineStyle, 1, 4, 1, 2);
    entityLayout->addWidget(perimeterLineWidth, 1, 6, 1, 2);

    entityLayout->addWidget(labelMark, 2, 0, 1, 2);
    entityLayout->addWidget(markColor, 2, 2, 1, 2);
    entityLayout->addWidget(markStyle, 2, 4, 1, 2);
    entityLayout->addWidget(markWidth, 2, 6, 1, 2);

    entityLayout->addWidget(labelCut, 3, 0, 1, 2);
    entityLayout->addWidget(cutColor, 3, 2, 1, 2);
    entityLayout->addWidget(cutStyle, 3, 4, 1, 2);
    entityLayout->addWidget(cutWidth, 3, 6, 1, 2);

    entityLayout->addWidget(labelStitch, 4, 0, 1, 2);
    entityLayout->addWidget(stitchColor, 4, 2, 1, 2);
    entityLayout->addWidget(stitchStyle, 4, 4, 1, 2);
    entityLayout->addWidget(stitchWidth, 4, 6, 1, 2);

    entityLayout->addWidget(labelGeneric, 5, 0, 1, 2);
    entityLayout->addWidget(genericColor, 5, 2, 1, 2);
    entityLayout->addWidget(genericStyle, 5, 4, 1, 2);
    entityLayout->addWidget(genericWidth, 5, 6, 1, 2);

    entityLayout->addWidget(labelMiddleAxis, 6, 0, 1, 2);
    entityLayout->addWidget(middleAxisColor, 6, 2, 1, 2);
    entityLayout->addWidget(middleAxisStyle, 6, 4, 1, 2);
    entityLayout->addWidget(middleAxisWidth, 6, 6, 1, 2);

    entityLayout->addWidget(labelNotchColor, 7, 0, 1, 2);
    entityLayout->addWidget(notchColor, 7, 2, 1, 2);
    entityGroupBox->setLayout(entityLayout);


    QGridLayout *objectLayout = new QGridLayout;

    QLabel *labelKnot = new QLabel(tr("节点大小(像素)"));
    QLabel *labelSnap = new QLabel(tr("捕捉距离(像素)"));
    objectLayout->addWidget(labelKnot, 0, 0, 1, 2);
    objectLayout->addWidget(labelSnap, 1, 0, 1, 2);

    QLineEdit *knotLineEdit = new QLineEdit(this);
    QLineEdit *snapLineEdit = new QLineEdit(this);
    objectLayout->addWidget(knotLineEdit, 0, 2, 1, 1);
    objectLayout->addWidget(snapLineEdit, 1, 2, 1, 1);
    objectSizeGroupBox->setLayout(objectLayout);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(entityGroupBox, 0, 0, 10, 4);
    mainLayout->addWidget(moreColorGroupBox, 10, 0, 6, 4);
    mainLayout->addWidget(objectSizeGroupBox, 0, 4, 3, 2);
    mainLayout->addWidget(referencePointGroupBox, 3, 4, 4, 2);
    mainLayout->addWidget(findPerimeterLineGroupBox, 7, 4, 3, 2);
    mainLayout->addWidget(lineStyleGroupBox, 10, 4, 2, 2);
    mainLayout->addWidget(projectWindowGroupBox, 12, 4, 4, 2);
    setLayout(mainLayout);
}

void EntityStyleTab::onColorChanged(QString name, QColor color)
{
    emit entityStyleChanged(name, QVariant(color.rgba()));
}

AxesGridTab::AxesGridTab(QWidget *parent)
{

}

OffsetTab::OffsetTab(QWidget *parent)
{

}

LanguageTab::LanguageTab(QWidget *parent)
{

}

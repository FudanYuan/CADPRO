#include "configuredialog.h"
#include "ui_configuredialog.h"
#include <QDesktopWidget>
#include <QSizePolicy>
#include <QLabel>
#include <QStringList>
#include <QString>
#include <customwidget.h>
#include <QGroupBox>
#include <QCheckBox>
#include <QDebug>
#include <debug.h>

ConfigureDialog::ConfigureDialog(Configure *config, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigureDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("CADPRO配置"));

    // 适应屏幕大小
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    setFixedSize(screenRect.width() - 400, screenRect.height()-200);
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));

    // 添加组件
    tabWidget = new QTabWidget(this);
    EntityStyleTab *eStyleTab = new EntityStyleTab(config->eStyle, this);
    tabWidget->addTab(eStyleTab, tr("实体类型"));
    connect(eStyleTab, &EntityStyleTab::tabChanged, this, &ConfigureDialog::onChanged);

    AxesGridTab *axesGridTab = new AxesGridTab(config->axesGrid, this);
    tabWidget->addTab(axesGridTab, tr("轴和网格"));
    connect(axesGridTab, &AxesGridTab::tabChanged, this, &ConfigureDialog::onChanged);

    OffsetTab *offsetTab = new OffsetTab(config->offset, this);
    tabWidget->addTab(offsetTab, tr("轴和网格"));
    connect(offsetTab, &OffsetTab::tabChanged, this, &ConfigureDialog::onChanged);

    LanguageTab *languageTab = new LanguageTab(config->language, this);
    tabWidget->addTab(languageTab, tr("系统语言"));
    connect(languageTab, &LanguageTab::tabChanged, this, &ConfigureDialog::onChanged);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
}

ConfigureDialog::~ConfigureDialog()
{
#ifdef DEBUG
    qDebug() << "delete configure dialog";
#endif
    delete ui;
    delete tabWidget;
    delete buttonBox;
}

int ConfigureDialog::hasKey(QString key)
{
    int res = -1;
    for(int i=0; i<keyValueList.length(); i++){
        if(keyValueList.at(i).key == key){
            res = i;
            break;
        }
    }
    return res;
}

void ConfigureDialog::accept()
{
    Configure::updateConfig(keyValueList);
    QDialog::accept();
}

void ConfigureDialog::reject()
{
    QDialog::reject();
}

void ConfigureDialog::onChanged(QString key, QVariant value)
{
    int res = hasKey(key);
#ifdef DEBUG
    qDebug() << res << " " << key << " " << value;
#endif
    if(res == -1){
        KeyValue keyValue(key, value);
        keyValueList.append(keyValue);
    } else{
        //keyValueList.at(res).setValue(value);

        // 暂时以这种方式解决
        KeyValue keyValue(key, value);
        keyValueList.append(keyValue);
    }
}

EntityStyleTab::EntityStyleTab(EntityStyle &eStyle, QWidget *parent) :
    CustomTabWidget(parent)
{
    // group box
    QGroupBox *entityGroupBox = new QGroupBox(tr("实体"), this);
    QGroupBox *moreColorGroupBox = new QGroupBox(tr("更多颜色"), this);
    QGroupBox *objectSizeGroupBox = new QGroupBox(tr("对象大小"), this);
    QGroupBox *referencePointGroupBox = new QGroupBox(tr("参考点"), this);
    QGroupBox *findPerimeterLineGroupBox = new QGroupBox(tr("查找周长线"), this);
    QGroupBox *lineStyleGroupBox = new QGroupBox(tr("线型"), this);
    QGroupBox *projectWindowGroupBox = new QGroupBox(tr("项目窗口颜色"), this);

    //! [实体]
    QGridLayout *entityLayout = new QGridLayout(this);

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
    perimeterLineColor->setColor(eStyle.perimeterLine.color);

    ColorComboBox *markColor = new ColorComboBox(tr("eStyle/eStyle_mark_color"), this);
    connect(markColor, &ColorComboBox::colorChanged, this, &EntityStyleTab::onColorChanged);
    markColor->setColor(eStyle.mark.color);

    ColorComboBox *cutColor = new ColorComboBox(tr("eStyle/eStyle_cut_color"), this);
    connect(cutColor, &ColorComboBox::colorChanged, this, &EntityStyleTab::onColorChanged);
    cutColor->setColor(eStyle.cut.color);

    ColorComboBox *stitchColor = new ColorComboBox(tr("eStyle/eStyle_stitch_color"), this);
    connect(stitchColor, &ColorComboBox::colorChanged, this, &EntityStyleTab::onColorChanged);
    stitchColor->setColor(eStyle.stitch.color);

    ColorComboBox *genericColor = new ColorComboBox(tr("eStyle/eStyle_generic_color"), this);
    connect(genericColor, &ColorComboBox::colorChanged, this, &EntityStyleTab::onColorChanged);
    genericColor->setColor(eStyle.generic.color);

    ColorComboBox *middleAxisColor = new ColorComboBox(tr("eStyle/eStyle_middleAxis_color"), this);
    connect(middleAxisColor, &ColorComboBox::colorChanged, this, &EntityStyleTab::onColorChanged);
    middleAxisColor->setColor(eStyle.middleAxis.color);

    ColorComboBox *notchColor = new ColorComboBox(tr("eStyle/eStyle_notch_color"), this);
    connect(notchColor, &ColorComboBox::colorChanged, this, &EntityStyleTab::onColorChanged);
    notchColor->setColor(eStyle.notchColor);

    lineStyleComboBox *perimeterLineStyle = new lineStyleComboBox(tr("eStyle/eStyle_permeterLine_style"), this);
    perimeterLineStyle->setCurrentIndex((int)eStyle.perimeterLine.style - 1);
    connect(perimeterLineStyle, &lineStyleComboBox::customActivated, this, &EntityStyleTab::onComboBoxChanged);

    lineStyleComboBox *markStyle = new lineStyleComboBox(tr("eStyle/eStyle_mark_style"), this);
    markStyle->setCurrentIndex((int)eStyle.mark.style - 1);
    connect(markStyle, &lineStyleComboBox::customActivated, this, &EntityStyleTab::onComboBoxChanged);

    lineStyleComboBox *cutStyle = new lineStyleComboBox(tr("eStyle/eStyle_cut_style"), this);
    cutStyle->setCurrentIndex((int)eStyle.cut.style - 1);
    connect(cutStyle, &lineStyleComboBox::customActivated, this, &EntityStyleTab::onComboBoxChanged);

    lineStyleComboBox *stitchStyle = new lineStyleComboBox(tr("eStyle/eStyle_stitch_style"), this);
    stitchStyle->setCurrentIndex((int)eStyle.stitch.style - 1);
    connect(stitchStyle, &lineStyleComboBox::customActivated, this, &EntityStyleTab::onComboBoxChanged);

    lineStyleComboBox *genericStyle = new lineStyleComboBox(tr("eStyle/eStyle_generic_style"), this);
    genericStyle->setCurrentIndex((int)eStyle.generic.style - 1);
    connect(genericStyle, &lineStyleComboBox::customActivated, this, &EntityStyleTab::onComboBoxChanged);

    lineStyleComboBox *middleAxisStyle = new lineStyleComboBox(tr("eStyle/eStyle_middleAxis_style"), this);
    middleAxisStyle->setCurrentIndex((int)eStyle.middleAxis.style - 1);
    connect(middleAxisStyle, &lineStyleComboBox::customActivated, this, &EntityStyleTab::onComboBoxChanged);

    LineEdit *perimeterLineWidth = new LineEdit(tr("eStyle/eStyle_permeterLine_width"), this);
    perimeterLineWidth->setText(QString::number(eStyle.perimeterLine.width));
    connect(perimeterLineWidth, &LineEdit::customTextEdited, this, &EntityStyleTab::onTextChanged);

    LineEdit *markWidth = new LineEdit(tr("eStyle/eStyle_mark_width"), this);
    markWidth->setText(QString::number(eStyle.mark.width));
    connect(markWidth, &LineEdit::customTextEdited, this, &EntityStyleTab::onTextChanged);

    LineEdit *cutWidth = new LineEdit(tr("eStyle/eStyle_cut_width"), this);
    cutWidth->setText(QString::number(eStyle.cut.width));
    connect(cutWidth, &LineEdit::customTextEdited, this, &EntityStyleTab::onTextChanged);

    LineEdit *stitchWidth = new LineEdit(tr("eStyle/eStyle_stitch_width"), this);
    stitchWidth->setText(QString::number(eStyle.stitch.width));
    connect(stitchWidth, &LineEdit::customTextEdited, this, &EntityStyleTab::onTextChanged);

    LineEdit *genericWidth = new LineEdit(tr("eStyle/eStyle_generic_width"), this);
    genericWidth->setText(QString::number(eStyle.generic.width));
    connect(genericWidth, &LineEdit::customTextEdited, this, &EntityStyleTab::onTextChanged);

    LineEdit *middleAxisWidth = new LineEdit(tr("eStyle/eStyle_middleAxis_width"), this);
    middleAxisWidth->setText(QString::number(eStyle.middleAxis.width));
    connect(middleAxisWidth, &LineEdit::customTextEdited, this, &EntityStyleTab::onTextChanged);

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
    //! [实体]

    //! [更多颜色]
    QGridLayout *moreColorLayout = new QGridLayout(this);

    QLabel *labelBackgroundColor = new QLabel(tr("背景色"));
    QLabel *labelSelectedEntity = new QLabel(tr("所选实体"));
    QLabel *labelSelectBox = new QLabel(tr("选择框"));
    QLabel *labelentityUnderCursor = new QLabel(tr("光标下的实体"));
    QLabel *labelHiddenEntity = new QLabel(tr("隐藏实体"));

    ColorComboBox *backgroundColor = new ColorComboBox(tr("eStyle/eStyle_backgroundColor"), this);
    connect(backgroundColor, &ColorComboBox::colorChanged, this, &EntityStyleTab::onColorChanged);
    backgroundColor->setColor(eStyle.backgroundColor);

    ColorComboBox *selectedEntityColor = new ColorComboBox(tr("eStyle/eStyle_selectedEntity_color"), this);
    connect(selectedEntityColor, &ColorComboBox::colorChanged, this, &EntityStyleTab::onColorChanged);
    selectedEntityColor->setColor(eStyle.selectedEntity.color);

    ColorComboBox *selectionBoxColor = new ColorComboBox(tr("eStyle/eStyle_selectionBox"), this);
    connect(selectionBoxColor, &ColorComboBox::colorChanged, this, &EntityStyleTab::onColorChanged);
    selectionBoxColor->setColor(eStyle.selectionBox);

    ColorComboBox *entityUnderCursorColor = new ColorComboBox(tr("eStyle/eStyle_entityUnderCursor_color"), this);
    connect(entityUnderCursorColor, &ColorComboBox::colorChanged, this, &EntityStyleTab::onColorChanged);
    entityUnderCursorColor->setColor(eStyle.entityUnderCursor.color);

    ColorComboBox *hiddenEntityColor = new ColorComboBox(tr("eStyle/eStyle_hiddenEntity"), this);
    connect(hiddenEntityColor, &ColorComboBox::colorChanged, this, &EntityStyleTab::onColorChanged);
    hiddenEntityColor->setColor(eStyle.hiddenEntity);

    lineStyleComboBox *selectedEntityStyle = new lineStyleComboBox(tr("eStyle/eStyle_selectedEntity_style"), this);
    selectedEntityStyle->setCurrentIndex((int)eStyle.selectedEntity.style - 1);
    connect(selectedEntityStyle, &lineStyleComboBox::customActivated, this, &EntityStyleTab::onComboBoxChanged);

    lineStyleComboBox *entityUnderCursorStyle = new lineStyleComboBox(tr("eStyle/eStyle_entityUnderCursor_width"), this);
    entityUnderCursorStyle->setCurrentIndex((int)eStyle.entityUnderCursor.style - 1);
    connect(entityUnderCursorStyle, &lineStyleComboBox::customActivated, this, &EntityStyleTab::onComboBoxChanged);

    LineEdit *entityUnderCursorWidth = new LineEdit(tr("eStyle/eStyle_entityUnderCursor_width"), this);
    entityUnderCursorWidth->setText(QString::number(eStyle.entityUnderCursor.width));
    connect(entityUnderCursorWidth, &LineEdit::customTextEdited, this, &EntityStyleTab::onTextChanged);

    moreColorLayout->addWidget(labelBackgroundColor, 0, 0, 1, 2);
    moreColorLayout->addWidget(backgroundColor, 0, 2, 1, 2);

    moreColorLayout->addWidget(labelSelectedEntity, 1, 0, 1, 2);
    moreColorLayout->addWidget(selectedEntityColor, 1, 2, 1, 2);
    moreColorLayout->addWidget(selectedEntityStyle, 1, 4, 1, 2);

    moreColorLayout->addWidget(labelSelectBox, 2, 0, 1, 2);
    moreColorLayout->addWidget(selectionBoxColor, 2, 2, 1, 2);

    moreColorLayout->addWidget(labelentityUnderCursor, 3, 0, 1, 2);
    moreColorLayout->addWidget(entityUnderCursorColor, 3, 2, 1, 2);
    moreColorLayout->addWidget(entityUnderCursorStyle, 3, 4, 1, 2);
    moreColorLayout->addWidget(entityUnderCursorWidth, 3, 6, 1, 2);

    moreColorLayout->addWidget(labelHiddenEntity, 4, 0, 1, 2);
    moreColorLayout->addWidget(hiddenEntityColor, 4, 2, 1, 2);

    moreColorGroupBox->setLayout(moreColorLayout);
    //! [更多颜色]

    //! [对象大小]
    QGridLayout *objectLayout = new QGridLayout(this);

    QLabel *labelKnot = new QLabel(tr("节点大小(像素)"));
    QLabel *labelSnap = new QLabel(tr("捕捉距离(像素)"));

    LineEdit *knotLineEdit = new LineEdit(tr("eStyle/eStyle_objSize_knotSizeInPix"), this);
    knotLineEdit->setText(QString::number(eStyle.objSize.knotSizeInPix));
    connect(knotLineEdit, &LineEdit::customTextEdited, this, &EntityStyleTab::onTextChanged);

    LineEdit *snapLineEdit = new LineEdit(tr("eStyle/eStyle_objSize_snapDistanceInPix"), this);
    snapLineEdit->setText(QString::number(eStyle.objSize.snapDistanceInPix));
    connect(snapLineEdit, &LineEdit::customTextEdited, this, &EntityStyleTab::onTextChanged);

    objectLayout->addWidget(labelKnot, 0, 0, 1, 3);
    objectLayout->addWidget(knotLineEdit, 0, 3, 1, 1);

    objectLayout->addWidget(labelSnap, 1, 0, 1, 3);
    objectLayout->addWidget(snapLineEdit, 1, 3, 1, 1);
    objectSizeGroupBox->setLayout(objectLayout);
    //! [对象大小]

    //! [参考点]
    QGridLayout *referencePointLayout = new QGridLayout(this);

    QLabel *labelReferencePointColor = new QLabel(tr("颜色"));
    QLabel *labelReferencePointInPix = new QLabel(tr("像素大小"));
    QLabel *labelReferenceTextHeightInPix = new QLabel(tr("文字高度(像素)"));

    ColorComboBox *referencePointColor = new ColorComboBox(tr("eStyle/eStyle_referPoint_color"), this);
    connect(referencePointColor, &ColorComboBox::colorChanged, this, &EntityStyleTab::onColorChanged);
    referencePointColor->setColor(eStyle.referPoint.color);

    LineEdit *referencePointInPix = new LineEdit(tr("eStyle/eStyle_referPoint_sizeInPix"), this);
    referencePointInPix->setText(QString::number(eStyle.referPoint.sizeInPix));
    connect(referencePointInPix, &LineEdit::customTextEdited, this, &EntityStyleTab::onTextChanged);

    LineEdit *referenceTextHeightInPix = new LineEdit(tr("eStyle/eStyle_referPoint_textHeightInPix"), this);
    referenceTextHeightInPix->setText(QString::number(eStyle.referPoint.textHeightInPix));
    connect(referenceTextHeightInPix, &LineEdit::customTextEdited, this, &EntityStyleTab::onTextChanged);

    referencePointLayout->addWidget(labelReferencePointColor, 0, 0, 1, 3);
    referencePointLayout->addWidget(referencePointColor, 0, 3, 1, 1);

    referencePointLayout->addWidget(labelReferencePointInPix, 1, 0, 1, 3);
    referencePointLayout->addWidget(referencePointInPix, 1, 3, 1, 1);

    referencePointLayout->addWidget(labelReferenceTextHeightInPix, 2, 0, 1, 3);
    referencePointLayout->addWidget(referenceTextHeightInPix, 2, 3, 1, 1);

    referencePointGroupBox->setLayout(referencePointLayout);
    //! [参考点]

    //! [查找周长线]
    QGridLayout *interLineStyleLayout = new QGridLayout(this);

    QLabel *labelInterLineStyle = new QLabel(tr("内线类型"));

    ComboBox *interLineStyle = new ComboBox(tr("eStyle/eStyle_interLineStyle"), this);
    interLineStyle->addItem(tr("标记"), QVariant((int)InterLineStyle::mark));
    interLineStyle->addItem(tr("切割"), QVariant((int)InterLineStyle::cut));
    interLineStyle->addItem(tr("缝线"), QVariant((int)InterLineStyle::stitch));
    interLineStyle->addItem(tr("通用"), QVariant((int)InterLineStyle::generic));
    interLineStyle->setCurrentIndex((int)eStyle.interLineStyle);
    connect(interLineStyle, &ComboBox::customActivated, this, &EntityStyleTab::onComboBoxChanged);

    interLineStyleLayout->addWidget(labelInterLineStyle, 0, 0, 1, 2);
    interLineStyleLayout->addWidget(interLineStyle, 0, 2, 1, 1);

    findPerimeterLineGroupBox->setLayout(interLineStyleLayout);
    //! [查找周长线]

    //! [线型]
    QGridLayout *lineTypeLayout = new QGridLayout(this);

    QLabel *labelLineType = new QLabel(tr("使用合适的曲线(非必选)"));

    CheckBox *lineTypeCheck = new CheckBox(tr("eStyle/eStyle_lineTypes"), this);
    lineTypeCheck->setChecked(eStyle.lineTypes);
    connect(lineTypeCheck, &CheckBox::customStateChanged, this, &EntityStyleTab::onCheckChanged);

    lineTypeLayout->addWidget(labelLineType, 0, 0, 1, 3);
    lineTypeLayout->addWidget(lineTypeCheck, 0, 3, 1, 1);

    lineStyleGroupBox->setLayout(lineTypeLayout);
    //! [线型]

    //! [项目窗口颜色]
    QGridLayout *projectWindowLayout = new QGridLayout(this);

    QLabel *labelProjectHistory = new QLabel(tr("项目历史"));
    QLabel *labelBasicSize = new QLabel(tr("基本尺码"));

    ColorComboBox *projectHistoryColor = new ColorComboBox(tr("eStyle/eStyle_projectHistory"), this);
    connect(projectHistoryColor, &ColorComboBox::colorChanged, this, &EntityStyleTab::onColorChanged);
    projectHistoryColor->setColor(eStyle.projectHistory);

    ColorComboBox *basicSizeColor = new ColorComboBox(tr("eStyle/eStyle_basicSize"), this);
    connect(basicSizeColor, &ColorComboBox::colorChanged, this, &EntityStyleTab::onColorChanged);
    basicSizeColor->setColor(eStyle.basicSize);

    projectWindowLayout->addWidget(labelProjectHistory, 0, 0, 1, 2);
    projectWindowLayout->addWidget(projectHistoryColor, 0, 2, 1, 1);

    projectWindowLayout->addWidget(labelBasicSize, 1, 0, 1, 2);
    projectWindowLayout->addWidget(basicSizeColor, 1, 2, 1, 1);

    projectWindowGroupBox->setLayout(projectWindowLayout);
    //! [项目窗口颜色]

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(entityGroupBox, 0, 0, 10, 4);
    mainLayout->addWidget(moreColorGroupBox, 10, 0, 6, 4);
    mainLayout->addWidget(objectSizeGroupBox, 0, 4, 3, 2);
    mainLayout->addWidget(referencePointGroupBox, 3, 4, 4, 2);
    mainLayout->addWidget(findPerimeterLineGroupBox, 7, 4, 3, 2);
    mainLayout->addWidget(lineStyleGroupBox, 10, 4, 2, 2);
    mainLayout->addWidget(projectWindowGroupBox, 12, 4, 4, 2);
    setLayout(mainLayout);
}

AxesGridTab::AxesGridTab(AxesGrid &axesGrid, QWidget *parent) :
    CustomTabWidget(parent)
{
    // group box
    QGroupBox *axesStyleGroupBox = new QGroupBox(tr("轴样式"), this);
    QGroupBox *gridStyleGroupBox = new QGroupBox(tr("网格"), this);
    QGroupBox *antiAliaisingGroupBox = new QGroupBox(tr("图形效果"), this);

    //! [轴样式]
    QGridLayout *axesStyleLayout = new QGridLayout(this);

    QLabel *labelShowAxes = new QLabel(tr("显示轴"), this);
    QLabel *labelAxesType = new QLabel(tr("轴类型"), this);
    QLabel *labelXAxesColor = new QLabel(tr("X轴颜色"), this);
    QLabel *labelYAxesColor = new QLabel(tr("Y轴颜色"), this);
    QLabel *labelArrowSize = new QLabel(tr("箭头大小(像素)"), this);
    QLabel *labelAxesSize = new QLabel(tr("轴大小(像素)"), this);

    CheckBox *showAxes = new CheckBox(tr("axesGrid/axesGrid_showAxes"), this);
    showAxes->setChecked(axesGrid.axes.showAxes);
    connect(showAxes, &CheckBox::customStateChanged, this, &AxesGridTab::onCheckChanged);

    ComboBox *axesType = new ComboBox(tr("axesGrid/axesGrid_axesType"), this);
    axesType->addItem(tr("小"), QVariant((int)AxesType::small));
    axesType->addItem(tr("大"), QVariant((int)AxesType::large));
    axesType->setCurrentIndex(axesGrid.axes.axesType);
    connect(axesType, &ComboBox::customActivated, this, &AxesGridTab::onComboBoxChanged);

    ColorComboBox *xAxesColor = new ColorComboBox(tr("axesGrid/axesGrid_xAxisColor"), this);
    xAxesColor->setColor(axesGrid.axes.xAxisColor);
    connect(xAxesColor, &ColorComboBox::colorChanged, this, &AxesGridTab::onColorChanged);

    ColorComboBox *yAxesColor = new ColorComboBox(tr("axesGrid/axesGrid_yAxisColor"), this);
    yAxesColor->setColor(axesGrid.axes.yAxisColor);
    connect(yAxesColor, &ColorComboBox::colorChanged, this, &AxesGridTab::onColorChanged);

    LineEdit *arrowSize = new LineEdit(tr("axesGrid/axesGrid_arrowSizeInPix"), this);
    arrowSize->setText(QString::number(axesGrid.axes.arrowSizeInPix));
    connect(arrowSize, &LineEdit::customTextEdited, this, &AxesGridTab::onTextChanged);

    LineEdit *axesSize = new LineEdit(tr("axesGrid/axesGrid_axisSizeInPix"), this);
    axesSize->setText(QString::number(axesGrid.axes.axisSizeInPix));
    connect(axesSize, &LineEdit::customTextEdited, this, &AxesGridTab::onTextChanged);

    axesStyleLayout->addWidget(labelShowAxes, 0, 0, 1, 2);
    axesStyleLayout->addWidget(showAxes, 0, 2, 1, 1);

    axesStyleLayout->addWidget(labelAxesType, 1, 0, 1, 2);
    axesStyleLayout->addWidget(axesType, 1, 2, 1, 1);

    axesStyleLayout->addWidget(labelXAxesColor, 2, 0, 1, 2);
    axesStyleLayout->addWidget(xAxesColor, 2, 2, 1, 1);

    axesStyleLayout->addWidget(labelYAxesColor, 3, 0, 1, 2);
    axesStyleLayout->addWidget(yAxesColor, 3, 2, 1, 1);

    axesStyleLayout->addWidget(labelArrowSize, 4, 0, 1, 2);
    axesStyleLayout->addWidget(arrowSize, 4, 2, 1, 1);

    axesStyleLayout->addWidget(labelAxesSize, 5, 0, 1, 2);
    axesStyleLayout->addWidget(axesSize, 5, 2, 1, 1);

    axesStyleGroupBox->setLayout(axesStyleLayout);
    //! [轴样式]

    //! [网格]
    QGridLayout *gridStyleLayout = new QGridLayout(this);

    QLabel *labelShowGrid = new QLabel(tr("显示网格"), this);
    QLabel *labelGridType = new QLabel(tr("网格类型"), this);
    QLabel *labelGridColor = new QLabel(tr("网格颜色"), this);
    QLabel *labelGridXStep = new QLabel(tr("X步长(像素)"), this);
    QLabel *labelGridYStep = new QLabel(tr("Y步长(像素)"), this);

    CheckBox *showGrid = new CheckBox(tr("axesGrid/axesGrid_showGrid"), this);
    showGrid->setChecked(axesGrid.grid.showGrid);
    connect(showGrid, &CheckBox::customStateChanged, this, &AxesGridTab::onCheckChanged);

    ComboBox *gridType = new ComboBox(tr("axesGrid/axesGrid_gridType"), this);
    gridType->addItem(tr("矩形"), QVariant((int)GridType::rectangular));
    gridType->addItem(tr("交叉"), QVariant((int)GridType::across));
    gridType->addItem(tr("节点"), QVariant((int)GridType::node));
    gridType->setCurrentIndex(axesGrid.grid.gridType);
    connect(gridType, &ComboBox::customActivated, this, &AxesGridTab::onComboBoxChanged);

    ColorComboBox *gridColor = new ColorComboBox(tr("axesGrid/axesGrid_gridColor"), this);
    gridColor->setColor(axesGrid.grid.gridColor);
    connect(gridColor, &ColorComboBox::colorChanged, this, &AxesGridTab::onColorChanged);

    LineEdit *gridXStep = new LineEdit(tr("axesGrid/axesGrid_xStep"), this);
    gridXStep->setText(QString::number(axesGrid.grid.xStep));
    connect(gridXStep, &LineEdit::customTextEdited, this, &AxesGridTab::onTextChanged);

    LineEdit *gridYStep = new LineEdit(tr("axesGrid/axesGrid_yStep"), this);
    gridYStep->setText(QString::number(axesGrid.grid.yStep));
    connect(gridYStep, &LineEdit::customTextEdited, this, &AxesGridTab::onTextChanged);

    gridStyleLayout->addWidget(labelShowGrid, 0, 0, 1, 2);
    gridStyleLayout->addWidget(showGrid, 0, 2, 1, 1);

    gridStyleLayout->addWidget(labelGridType, 1, 0, 1, 2);
    gridStyleLayout->addWidget(gridType, 1, 2, 1, 1);

    gridStyleLayout->addWidget(labelGridColor, 2, 0, 1, 2);
    gridStyleLayout->addWidget(gridColor, 2, 2, 1, 1);

    gridStyleLayout->addWidget(labelGridXStep, 3, 0, 1, 2);
    gridStyleLayout->addWidget(gridXStep, 3, 2, 1, 1);

    gridStyleLayout->addWidget(labelGridYStep, 4, 0, 1, 2);
    gridStyleLayout->addWidget(gridYStep, 4, 2, 1, 1);

    gridStyleLayout->addItem(new QSpacerItem(gridStyleGroupBox->width(), gridStyleGroupBox->height(), QSizePolicy::Expanding, QSizePolicy::Expanding), 5, 0, 1, 3);
    gridStyleGroupBox->setLayout(gridStyleLayout);
    //! [网格]

    //! [图形效果]
    QGridLayout *antiAliaisingLayout = new QGridLayout(this);

    QLabel *labelAntiAliaising = new QLabel(tr("抗锯齿"), this);
    CheckBox *antiAliaising = new CheckBox(tr("axesGrid/axesGrid_antiAliaising"), this);
    antiAliaising->setChecked(axesGrid.antiAliaising);
    connect(antiAliaising, &CheckBox::customStateChanged, this, &AxesGridTab::onCheckChanged);

    antiAliaisingLayout->addWidget(labelAntiAliaising, 0, 0, 1, 2);
    antiAliaisingLayout->addWidget(antiAliaising, 0, 2, 1, 1);

    antiAliaisingGroupBox->setLayout(antiAliaisingLayout);
    //! [图形效果]

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(axesStyleGroupBox, 0, 0, 1, 1);
    mainLayout->addWidget(gridStyleGroupBox, 0, 1, 1, 1);
    mainLayout->addWidget(antiAliaisingGroupBox, 1, 0, 1, 1);
    mainLayout->addItem(new QSpacerItem(parent->width()/3, parent->height(), QSizePolicy::Expanding, QSizePolicy::Expanding), 2, 0, 1, 1);
    mainLayout->addItem(new QSpacerItem(parent->width()/4, parent->height(), QSizePolicy::Expanding, QSizePolicy::Expanding), 0, 2, 1, 1);
    setLayout(mainLayout);
}

OffsetTab::OffsetTab(QList<Offset> &offset, QWidget *parent) :
    CustomTabWidget(parent)
{

}

LanguageTab::LanguageTab(Language &language, QWidget *parent) :
    CustomTabWidget(parent)
{

}

CustomTabWidget::CustomTabWidget(QWidget *parent) :
    QWidget(parent)
{

}

void CustomTabWidget::onColorChanged(QString key, QColor color)
{
#ifdef DEBUG
    qDebug() << "CustomTabWidget:: " << key << " " << color.rgba();
#endif
    emit tabChanged(key, QVariant(color.rgba()));
}

void CustomTabWidget::onTextChanged(QString key, QString value)
{
    emit tabChanged(key, QVariant(value));
}

void CustomTabWidget::onComboBoxChanged(QString key, int value)
{
    qDebug() << value << "  ,penStyle: " << (Qt::PenStyle)value;
    emit tabChanged(key, QVariant(value));
}

void CustomTabWidget::onCheckChanged(QString key, bool value)
{
    emit tabChanged(key, QVariant(value));
}


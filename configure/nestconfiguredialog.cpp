#include "nestconfiguredialog.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QSizePolicy>
#include <QLabel>
#include <QStringList>
#include <QString>
#include <customwidget.h>
#include <QGroupBox>
#include <QCheckBox>
#include <QFileDialog>
#include <debug.h>

#include <QDebug>

NestConfigureDialog::NestConfigureDialog(NestConfigure *config,  QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("CADPRO配置"));

    // 适应屏幕大小
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    setFixedSize(screenRect.width() - 400, screenRect.height()-200);
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));

    // 添加组件
    tabWidget = new QTabWidget(this);
    MiscellaneousTab *miscellaneousTab = new MiscellaneousTab(this);
    tabWidget->addTab(miscellaneousTab, tr("杂项"));
    miscellaneousTab->setMainGrid(config->mainGrid);  // 设置主网格
    miscellaneousTab->setSecondGrid(config->secondGrid);  // 设置副网格
    miscellaneousTab->setAllowEntityOutOfSheet(config->allowEntityOutOfSheet);
    connect(miscellaneousTab, &MiscellaneousTab::tabChanged, this, &NestConfigureDialog::onChanged);

    ColorsTab *colorsTab = new ColorsTab(this);
    tabWidget->addTab(colorsTab, tr("颜色"));
    colorsTab->setSheetStyle(config->sheetStyle);  // 设置材料类型
    colorsTab->setPieceStyle(config->pieceStyle);  // 设置切割件类型
    colorsTab->setBackground(config->backgroundColor);  // 设置背景色
    colorsTab->setShowPiecesDistance(config->showPiecesDistance);  // 设置显示切割件间隔
    connect(colorsTab, &MiscellaneousTab::tabChanged, this, &NestConfigureDialog::onChanged);

    NestLanguageTab *languageTab = new NestLanguageTab(this);
    tabWidget->addTab(languageTab, tr("语言&文件"));
    languageTab->setLanguage(config->language);  // 设置语言
    languageTab->setWorkPath(config->workPathStr);  // 设置文件夹
    connect(languageTab, &NestLanguageTab::tabChanged, this, &NestConfigureDialog::onChanged);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
}

NestConfigureDialog::~NestConfigureDialog()
{
#ifdef DEBUG
    qDebug() << "configure dialog has deleted!";
#endif
    delete tabWidget;
    tabWidget = NULL;
    delete buttonBox;

}

int NestConfigureDialog::hasKey(QString key)
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

void NestConfigureDialog::accept()
{
    NestConfigure::updateConfig(keyValueList);
    QDialog::accept();
}

void NestConfigureDialog::reject()
{
    QDialog::reject();
}

void NestConfigureDialog::onChanged(QString key, QVariant value)
{
    int res = hasKey(key);
#ifdef DEBUG
    qDebug() << res << " " << key << " " << value;
#endif
    if(res == -1){
        NestConfigure::KeyValue keyValue(key, value);
        keyValueList.append(keyValue);
    } else{
        //keyValueList.at(res).setValue(value);

        // 暂时以这种方式解决
        NestConfigure::KeyValue keyValue(key, value);
        keyValueList.append(keyValue);
    }
}

MiscellaneousTab::MiscellaneousTab(QWidget *parent):
    CustomTabWidget(parent)
{
    // group box
    QGroupBox *mainGridGroupBox = new QGroupBox(tr("主网格"), this);

    // 主网格布局
    QGridLayout *mainGridLayout = new QGridLayout(mainGridGroupBox);

    QLabel *labelMainGridXStep = new QLabel(tr("X步长(像素)"), this);
    QLabel *labelMainGridYStep = new QLabel(tr("Y步长(像素)"), this);
    QLabel *labelMainGridColor = new QLabel(tr("网格颜色"), this);

    showMainGrid = new CheckBox(tr("mainGrid/mainGrid_showGrid"), tr("显示主网格"), this);
    connect(showMainGrid, &CheckBox::customStateChanged, this, &MiscellaneousTab::onCheckChanged);

    mainGridXStep = new LineEdit(tr("mainGrid/mainGrid_xStep"), this);
    connect(mainGridXStep, &LineEdit::customTextEdited, this, &MiscellaneousTab::onTextChanged);

    mainGridYStep = new LineEdit(tr("mainGrid/mainGrid_yStep"), this);
    connect(mainGridYStep, &LineEdit::customTextEdited, this, &MiscellaneousTab::onTextChanged);

    mainGridColor = new ColorComboBox(tr("mainGrid/mainGrid_gridColor"), this);
    connect(mainGridColor, &ColorComboBox::colorChanged, this, &MiscellaneousTab::onColorChanged);

    mainGridLayout->addWidget(labelMainGridXStep, 0, 0, 1, 1);
    mainGridLayout->addWidget(mainGridXStep, 0, 1, 1, 1);
    mainGridLayout->addWidget(labelMainGridColor, 0, 2, 1, 1);
    mainGridLayout->addWidget(mainGridColor, 0, 3, 1, 1);
    mainGridLayout->addWidget(labelMainGridYStep, 1, 0, 1, 1);
    mainGridLayout->addWidget(mainGridYStep, 1, 1, 1, 1);
    mainGridLayout->addWidget(showMainGrid, 1, 2, 1, 2);
    mainGridLayout->addItem(new QSpacerItem(mainGridGroupBox->width(),
                                             mainGridGroupBox->height(),
                                             QSizePolicy::Expanding,
                                             QSizePolicy::Expanding),
                             0, 0, 1, 1);

    // 副网格布局
    QGroupBox *secondGridGroupBox = new QGroupBox(tr("副网格"), this);
    QGridLayout *secondGridLayout = new QGridLayout(secondGridGroupBox);

    QLabel *labelSecondGridXStep = new QLabel(tr("X步长(像素)"), this);
    QLabel *labelSecondGridYStep = new QLabel(tr("Y步长(像素)"), this);
    QLabel *labelSecondGridColor = new QLabel(tr("网格颜色"), this);

    showSecondGrid = new CheckBox(tr("secondGrid/secondGrid_showGrid"), tr("显示副网格"), this);
    connect(showSecondGrid, &CheckBox::customStateChanged, this, &MiscellaneousTab::onCheckChanged);

    secondGridXStep = new LineEdit(tr("secondGrid/secondGrid_xStep"), this);
    connect(secondGridXStep, &LineEdit::customTextEdited, this, &MiscellaneousTab::onTextChanged);

    secondGridYStep = new LineEdit(tr("secondGrid/secondGrid_yStep"), this);
    connect(secondGridYStep, &LineEdit::customTextEdited, this, &MiscellaneousTab::onTextChanged);

    secondGridColor = new ColorComboBox(tr("secondGrid/secondGrid_gridColor"), this);
    connect(secondGridColor, &ColorComboBox::colorChanged, this, &MiscellaneousTab::onColorChanged);

    secondGridLayout->addWidget(labelSecondGridXStep, 0, 0, 1, 1);
    secondGridLayout->addWidget(secondGridXStep, 0, 1, 1, 1);
    secondGridLayout->addWidget(labelSecondGridColor, 0, 2, 1, 1);
    secondGridLayout->addWidget(secondGridColor, 0, 3, 1, 1);
    secondGridLayout->addWidget(labelSecondGridYStep, 1, 0, 1, 1);
    secondGridLayout->addWidget(secondGridYStep, 1, 1, 1, 1);
    secondGridLayout->addWidget(showSecondGrid, 1, 2, 1, 2);
    secondGridLayout->addItem(new QSpacerItem(secondGridGroupBox->width(),
                                             secondGridGroupBox->height(),
                                             QSizePolicy::Expanding,
                                             QSizePolicy::Expanding),
                             0, 0, 1, 1);

    // 允许实体超出材料
    allowEntityOutOfSheetCBox = new CheckBox(tr("other/other_allowEntityOutOfSheet"), tr("允许实体超出材料"), this);
    connect(allowEntityOutOfSheetCBox, &CheckBox::customStateChanged, this, &MiscellaneousTab::onCheckChanged);

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(mainGridGroupBox, 0, 0, 1, 1);
    mainLayout->addItem(new QSpacerItem(1,
                                        1,
                                        QSizePolicy::Expanding,
                                        QSizePolicy::Expanding),
                        0, 1, 1, 1);

    mainLayout->addWidget(secondGridGroupBox, 1, 0, 1, 1);
    mainLayout->addItem(new QSpacerItem(1,
                                        1,
                                        QSizePolicy::Expanding,
                                        QSizePolicy::Expanding),
                        1, 1, 1, 1);
    mainLayout->addWidget(allowEntityOutOfSheetCBox, 2, 0, 1, 1);
    mainLayout->addItem(new QSpacerItem(width(),
                                        1,
                                        QSizePolicy::Expanding,
                                        QSizePolicy::Expanding),
                        3, 0, 1, 1);
}

void MiscellaneousTab::setMainGrid(const NestConfigure::Grid &mainGrid)
{
    showMainGrid->setChecked(mainGrid.showGrid);
    mainGridXStep->setText(QString("%1").arg(mainGrid.xStep));
    mainGridYStep->setText(QString("%1").arg(mainGrid.yStep));
    mainGridColor->setColor(mainGrid.gridColor);
}

void MiscellaneousTab::setSecondGrid(const NestConfigure::Grid &secondGrid)
{
    showSecondGrid->setChecked(secondGrid.showGrid);
    secondGridXStep->setText(QString("%1").arg(secondGrid.xStep));
    secondGridYStep->setText(QString("%1").arg(secondGrid.yStep));
    secondGridColor->setColor(secondGrid.gridColor);
}

void MiscellaneousTab::setAllowEntityOutOfSheet(bool flag)
{
    allowEntityOutOfSheetCBox->setChecked(flag);
}

ColorsTab::ColorsTab(QWidget *parent) :
    CustomTabWidget(parent)
{
    // 主布局
    QGridLayout *mainLayout = new QGridLayout(this);

    // 材料
    QGroupBox *sheetStyleGroupBox = new QGroupBox(tr("查看材料"), this);

    QLabel *labelSheet = new QLabel(tr("材料"), this);
    QLabel *labelSheetMargin = new QLabel(tr("材料边距余量"), this);
    QLabel *labelSheetReinforcement = new QLabel(tr("上插板"), this);
    QLabel *labelSheetReinforcementMargin = new QLabel(tr("上插板边距余量"), this);
    QLabel *labelSheetFilled = new QLabel(tr("材料填充"), this);

    sheetColor = new ColorComboBox(tr("sheetStyle/sheetStyle_sheetColor"), this);
    connect(sheetColor, &ColorComboBox::colorChanged, this, &ColorsTab::onColorChanged);

    sheetMarginColor = new ColorComboBox(tr("sheetStyle/sheetStyle_sheetMarginColor"), this);
    connect(sheetMarginColor, &ColorComboBox::colorChanged, this, &ColorsTab::onColorChanged);

    sheetReinforcementColor = new ColorComboBox(tr("sheetStyle/sheetStyle_reinforcementColor"), this);
    connect(sheetReinforcementColor, &ColorComboBox::colorChanged, this, &ColorsTab::onColorChanged);

    sheetReinforcementMarginColor = new ColorComboBox(tr("sheetStyle/sheetStyle_reinforcementMarginColor"), this);
    connect(sheetReinforcementMarginColor, &ColorComboBox::colorChanged, this, &ColorsTab::onColorChanged);

    sheetFilled = new CheckBox(tr("sheetStyle/sheetStyle_sheetFilled"), this);
    connect(sheetFilled, &CheckBox::customStateChanged, this, &ColorsTab::onCheckChanged);

    QGridLayout *sheetStyleLayout = new QGridLayout(sheetStyleGroupBox);
    sheetStyleLayout->addWidget(labelSheet, 0, 0, 1, 1);
    sheetStyleLayout->addWidget(sheetColor, 0, 1, 1, 1);
    sheetStyleLayout->addWidget(labelSheetMargin, 1, 0, 1, 1);
    sheetStyleLayout->addWidget(sheetMarginColor, 1, 1, 1, 1);
    sheetStyleLayout->addWidget(labelSheetReinforcement, 2, 0, 1, 1);
    sheetStyleLayout->addWidget(sheetReinforcementColor, 2, 1, 1, 1);
    sheetStyleLayout->addWidget(labelSheetReinforcementMargin, 3, 0, 1, 1);
    sheetStyleLayout->addWidget(sheetReinforcementMarginColor, 3, 1, 1, 1);
    sheetStyleLayout->addWidget(labelSheetFilled, 4, 0, 1, 1);
    sheetStyleLayout->addWidget(sheetFilled, 4, 1, 1, 1);
    sheetStyleLayout->addItem(new QSpacerItem(sheetStyleGroupBox->width(),
                                              1,
                                              QSizePolicy::Expanding,
                                              QSizePolicy::Expanding),
                              5, 0, 1, 1);

    QGroupBox *pieceStyleGroupBox = new QGroupBox(tr("导入切割件"), this);

    QLabel *labelPieceOutside = new QLabel(tr("外部"), this);
    QLabel *labelPieceInside = new QLabel(tr("内部"), this);
    QLabel *labelPieceLining = new QLabel(tr("衬里"), this);
    QLabel *labelPieceReinforcement = new QLabel(tr("上插板"), this);
    QLabel *labelPieceOther = new QLabel(tr("其他"), this);

    outsideColor = new ColorComboBox(tr("pieceStyle/pieceStyle_outsideColor"), this);
    connect(outsideColor, &ColorComboBox::colorChanged, this, &ColorsTab::onColorChanged);

    insideColor = new ColorComboBox(tr("pieceStyle/pieceStyle_insideColor"), this);
    connect(insideColor, &ColorComboBox::colorChanged, this, &ColorsTab::onColorChanged);

    liningColor = new ColorComboBox(tr("pieceStyle/pieceStyle_liningColor"), this);
    connect(liningColor, &ColorComboBox::colorChanged, this, &ColorsTab::onColorChanged);

    pieceReinforcementColor = new ColorComboBox(tr("pieceStyle/pieceStyle_reinforcementColor"), this);
    connect(pieceReinforcementColor, &ColorComboBox::colorChanged, this, &ColorsTab::onColorChanged);

    otherColor = new ColorComboBox(tr("pieceStyle/pieceStyle_otherColor"), this);
    connect(otherColor, &ColorComboBox::colorChanged, this, &MiscellaneousTab::onColorChanged);

    QGridLayout *pieceStyleLayout = new QGridLayout(pieceStyleGroupBox);
    pieceStyleLayout->addWidget(labelPieceOutside, 0, 0, 1, 1);
    pieceStyleLayout->addWidget(outsideColor, 0, 1, 1, 1);
    pieceStyleLayout->addWidget(labelPieceInside, 1, 0, 1, 1);
    pieceStyleLayout->addWidget(insideColor, 1, 1, 1, 1);
    pieceStyleLayout->addWidget(labelPieceLining, 2, 0, 1, 1);
    pieceStyleLayout->addWidget(liningColor, 2, 1, 1, 1);
    pieceStyleLayout->addWidget(labelPieceReinforcement, 3, 0, 1, 1);
    pieceStyleLayout->addWidget(pieceReinforcementColor, 3, 1, 1, 1);
    pieceStyleLayout->addWidget(labelPieceOther, 4, 0, 1, 1);
    pieceStyleLayout->addWidget(otherColor, 4, 1, 1, 1);
    pieceStyleLayout->addItem(new QSpacerItem(pieceStyleGroupBox->width(),
                                              1,
                                              QSizePolicy::Expanding,
                                              QSizePolicy::Expanding),
                              5, 0, 1, 1);
    pieceStyleGroupBox->setLayout(pieceStyleLayout);

    QLabel *labelBackgroundColor = new QLabel(tr("背景色"), this);
    QLabel *labelShowPiecesDistance = new QLabel(tr("显示切割件间距"), this);

    // 背景色
    backgroundColor = new ColorComboBox(tr("other/other_backgroundColor"), this);
    connect(backgroundColor, &ColorComboBox::colorChanged, this, &ColorsTab::onColorChanged);

    // 展示零件距离
    showPiecesDistance = new CheckBox(tr("other/other_showPiecesDistance"), this);
    connect(showPiecesDistance, &CheckBox::customStateChanged, this, &ColorsTab::onCheckChanged);

    mainLayout->addWidget(sheetStyleGroupBox, 0, 0, 1, 2);
    mainLayout->addWidget(pieceStyleGroupBox, 0, 2, 1, 2);
    mainLayout->addWidget(labelBackgroundColor, 1, 0, 1, 1);
    mainLayout->addWidget(backgroundColor, 1, 1, 1, 1);
    mainLayout->addWidget(labelShowPiecesDistance, 1, 2, 1, 1);
    mainLayout->addWidget(showPiecesDistance, 1, 3, 1, 1);
    mainLayout->addItem(new QSpacerItem(this->width(),
                                        1,
                                        QSizePolicy::Expanding,
                                        QSizePolicy::Expanding),
                        2, 0, 1, 4);
}

void ColorsTab::setSheetStyle(const NestConfigure::SheetStyle &sheetStyle)
{
    sheetColor->setColor(sheetStyle.sheetColor);
    sheetMarginColor->setColor(sheetStyle.sheetMarginColor);
    sheetReinforcementColor->setColor(sheetStyle.reinforcementColor);
    sheetReinforcementMarginColor->setColor(sheetStyle.reinforcementMarginColor);
    sheetFilled->setChecked(sheetStyle.sheetFilled);
}

void ColorsTab::setPieceStyle(const NestConfigure::PieceStyle &pieceStyle)
{
    outsideColor->setColor(pieceStyle.outsideColor);
    insideColor->setColor(pieceStyle.insideColor);
    liningColor->setColor(pieceStyle.liningColor);
    pieceReinforcementColor->setColor(pieceStyle.reinforcementColor);
    otherColor->setColor(pieceStyle.otherColor);
}

void ColorsTab::setBackground(const QColor &color)
{
    backgroundColor->setColor(color);
}

void ColorsTab::setShowPiecesDistance(const bool &flag)
{
    showPiecesDistance->setChecked(flag);
}

NestLanguageTab::NestLanguageTab(QWidget *parent) :
    CustomTabWidget(parent)
{
    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 语言
    QGroupBox *languageGroupBox = new QGroupBox(tr("语言"), this);

    languageCBox = new ComboBox(tr("other/other_language"), this);
    languageCBox->addItem(tr("česky"), QVariant(NestConfigure::Czech));
    languageCBox->addItem(tr("汉语"), QVariant(NestConfigure::Chinese));
    languageCBox->addItem(tr("Deutsch"), QVariant(NestConfigure::Deutsch));
    languageCBox->addItem(tr("English"), QVariant(NestConfigure::English));
    languageCBox->addItem(tr("Français"), QVariant(NestConfigure::French));
    languageCBox->addItem(tr("Italiano"), QVariant(NestConfigure::Italian));
    languageCBox->addItem(tr("日本語"), QVariant(NestConfigure::Japanese));
    languageCBox->addItem(tr("Francja"), QVariant(NestConfigure::Poland));
    languageCBox->addItem(tr("Português"), QVariant(NestConfigure::Portuguese));
    languageCBox->addItem(tr("Pусский"), QVariant(NestConfigure::Russian));
    languageCBox->addItem(tr("El español"), QVariant(NestConfigure::Spanish));
    languageCBox->addItem(tr("Ykpaihcbka"), QVariant(NestConfigure::Ukrainian));

    connect(languageCBox, &ComboBox::customActivated, this, &NestLanguageTab::onComboBoxChanged);

    QVBoxLayout *languageLayout = new QVBoxLayout(languageGroupBox);
    languageLayout->addWidget(languageCBox);
    languageGroupBox->setLayout(languageLayout);

    // 文件夹
    QGroupBox *workPathGroupBox = new QGroupBox(tr("文件夹"), this);

    workPathLine = new LineEdit(tr("other/other_workPathStr"), this);
    connect(workPathLine, &LineEdit::customTextEdited, this, &NestLanguageTab::onTextChanged);

    QPushButton *workPathBtn = new QPushButton(tr("···"), this);
    connect(workPathBtn, &QPushButton::clicked, this, &NestLanguageTab::onWorkPathBtnClicked);

    QGridLayout *workPathLayout = new QGridLayout(workPathGroupBox);
    workPathLayout->addWidget(workPathLine, 0, 0, 1, 9);
    workPathLayout->addWidget(workPathBtn, 0, 9, 1, 1);
    workPathGroupBox->setLayout(workPathLayout);

    mainLayout->addWidget(languageGroupBox);
    mainLayout->addWidget(workPathGroupBox);
    mainLayout->addItem(new QSpacerItem(this->width(),
                                        1,
                                        QSizePolicy::Expanding,
                                        QSizePolicy::Expanding));
}

void NestLanguageTab::setLanguage(const NestConfigure::Language &language)
{
    languageCBox->setCurrentIndex((int)language);
}

void NestLanguageTab::setWorkPath(const QString &workPathStr)
{
    workPathLine->setText(workPathStr);
}

void NestLanguageTab::onWorkPathBtnClicked(bool flag)
{
    qDebug() << "flag: " << flag;
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::Directory);
    QString dirInConfig = workPathLine->text();
    if(dirInConfig.isEmpty()){
        dirInConfig = QDir::currentPath();
    }
    QString directory = fileDialog->getExistingDirectory(this, tr("选择路径"), dirInConfig);
    if (!directory.isEmpty()) {
        workPathLine->setText(directory);
        emit tabChanged(tr("other/other_workPathStr"), QVariant(directory));
    }
}

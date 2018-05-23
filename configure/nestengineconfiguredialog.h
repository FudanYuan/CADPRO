#ifndef AUTONESTCONFIGUREDIALOG_H
#define AUTONESTCONFIGUREDIALOG_H

#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QCheckBox>
#include <QWidget>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QList>

#include "nestengineconfigure.h"



//! [0]
class WholeSheetConfigTab : public QWidget
{
    Q_OBJECT
public:
    explicit WholeSheetConfigTab(NestEngineConfigure::WholeSheetNest wholesheetnest,QWidget *parent = 0);
    QCheckBox *TailPieceMixing;
    QCheckBox *TailLineMixing;
    QCheckBox *SameTypeSizeMixing;
    QCheckBox *AllMixing;
};
//! [0]


//! [1]
class StripSheetConfigTab : public QWidget
{
    Q_OBJECT
public:
    explicit StripSheetConfigTab(QWidget *parent = 0);
    QCheckBox *leftRightTurnCBox;
    QCheckBox *sizeDownCBox;
};
//! [1]


//! [2]
class PackageSheetConfig : public QWidget
{
    Q_OBJECT
public:
    explicit PackageSheetConfig(QWidget *parent = 0);
};
//! [2]

/***/
class NestEngineConfigureDialog : public QDialog
{
    Q_OBJECT
public:
    enum TabType{
        Default = -1,
        Whole,
        Strip,
        Package
    };

    explicit NestEngineConfigureDialog(NestEngineConfigure *config);
    explicit NestEngineConfigureDialog(NestEngineConfigure *config, TabType type);


    NestEngineConfigure::StripSheetNest * getCurStripConfig();
    NestEngineConfigure::WholeSheetNest * getCurWholeConfig();
public slots:
    void onTabChanged(int i);
    void onDialogButtonClicked(QAbstractButton *button);  // 响应材料信息改变
private:
    TabType tabType;
    QTabWidget *tabWidget;
    StripSheetConfigTab *sSheetTab;
    WholeSheetConfigTab *wSheetTab;
    PackageSheetConfig *pSheetTab;
    QDialogButtonBox *buttonBox;
    NestEngineConfigure::StripSheetNest *curStripConfig;
    NestEngineConfigure::WholeSheetNest *curWholeConfig;
};

#endif // AUTONESTCONFIGUREDIALOG_H

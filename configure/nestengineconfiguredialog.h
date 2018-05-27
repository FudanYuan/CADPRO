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
#include <QMap>

#include "nestengineconfigure.h"

class WholeSheetConfigTab : public QWidget
{
    Q_OBJECT
public:
    explicit WholeSheetConfigTab(QWidget *parent = 0);
    QCheckBox *TailPieceMixing;
    QCheckBox *TailLineMixing;
    QCheckBox *SameTypeSizeMixing;
    QCheckBox *AllMixing;
    QRadioButton *HorizontalNest;
    QRadioButton *VerticalNest;
    QLineEdit *degree;
    QTableWidget *tableWidget;
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
public slots:
    void onQCheckBoxChanged(bool checked);
    void onQCheckBoxChanged1(bool checked);
    void onQCheckBoxChanged2(bool checked);
    void onQCheckBoxChanged3(bool checked);
};

class StripSheetConfigTab : public QWidget
{
    Q_OBJECT
public:
    explicit StripSheetConfigTab(QWidget *parent = 0);
    QCheckBox *leftRightTurnCBox;
    QCheckBox *sizeDownCBox;
    QRadioButton *HorizontalAdaptiveSpacing;
    QRadioButton *VerticalAdaptiveSpacing;
    QTableWidget *tableWidget;
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
};

class PackageSheetConfig : public QWidget
{
    Q_OBJECT
public:
    explicit PackageSheetConfig(QWidget *parent = 0);
    QCheckBox *TailPieceMixing1;
    QCheckBox *TailLineMixing1;
    QCheckBox *SameTypeSizeMixing1;
    QCheckBox *AllMixing1;
    QRadioButton *HorizontalNest1;
    QRadioButton *VerticalNest1;
    QLineEdit *degree1;
    QTableWidget *tableWidget1;
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
public slots:
    void onQCheckBoxChanged11(bool checked);
    void onQCheckBoxChanged12(bool checked);
    void onQCheckBoxChanged13(bool checked);
    void onQCheckBoxChanged14(bool checked);
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
    NestEngineConfigure::PackageSheetNest * getCurPackageConfig();
    QTableWidget * inittableWIdget();

    QMap<int,QList<QList<int>>> dataMap;

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
    NestEngineConfigure::PackageSheetNest *curPackageConfig;
};

#endif // AUTONESTCONFIGUREDIALOG_H

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

//! [0]
class WholeSheetConfigTab : public QWidget
{
    Q_OBJECT
public:
    explicit WholeSheetConfigTab(QWidget *parent = 0);
};
//! [0]


//! [1]
class StripSheetConfigTab : public QWidget
{
    Q_OBJECT
public:
    explicit StripSheetConfigTab(QWidget *parent = 0);
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
    NestEngineConfigureDialog();

public slots:
    void onNestEngingeConfig(int i);

private:
     QTabWidget *tabWidget;
};

#endif // AUTONESTCONFIGUREDIALOG_H

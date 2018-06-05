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
    explicit WholeSheetConfigTab(QWidget *parent = 0,QList<QList<int>> dataList = QList<QList<int>>());
    void setTableWidget();
    QCheckBox *TailPieceMixing;
    QCheckBox *TailLineMixing;
    QList<QList<int>> dataList;
    QCheckBox *SameTypeSizeMixing;
    QCheckBox *AllMixing;
    QRadioButton *HorizontalNest;
    QRadioButton *VerticalNest;
    QLineEdit *degree;
    QTableWidget *tableWidget;
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
    QPushButton *button4;
public slots:
    void onQCheckBoxChanged(bool checked);
    void onQCheckBoxChanged1(bool checked);
    void onQCheckBoxChanged2(bool checked);
    void onQCheckBoxChanged3(bool checked);
    void onConfigureButtonClicked();
    void onEditButtonClicked();
    void onDeleteButtonClicked();
    void onNewButtonClicked();
    void onItemChanged();
signals:
    void wDataChanged(int index,QList<QList<int>> wData);
};

class StripSheetConfigTab : public QWidget
{
    Q_OBJECT
public:
    explicit StripSheetConfigTab(QWidget *parent = 0,QList<QList<int>> dataList = QList<QList<int>>());
    void setTabWidget();
    QList<QList<int>> dataList;
    QCheckBox *leftRightTurnCBox;
    QCheckBox *sizeDownCBox;
    QCheckBox *HorizontalAdaptiveSpacing;
    QCheckBox *TailPieceMixing;
    QTableWidget *tableWidget;
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
    QPushButton *button4;
    void onConfigureButtonClicked();
    void onEditButtonClicked();
    void onDeleteButtonClicked();
    void onNewButtonClicked();
    void onItemChanged();
signals:
    void sDataChanged(int index,QList<QList<int>> sData);
};

class PackageSheetConfig : public QWidget
{
    Q_OBJECT
public:
    explicit PackageSheetConfig(QWidget *parent = 0,QList<QList<int>> dataList = QList<QList<int>>());
    void setTableWidget();
    QList<QList<int>> dataList = QList<QList<int>>();
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
    QPushButton *button4;
    void onConfigureButtonClicked();
    void onEditButtonClicked();
    void onDeleteButtonClicked();
    void onNewButtonClicked();
    void onItemChanged();
signals:
    void pDataChanged(int index,QList<QList<int>> pData);
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
    enum RoleType{
        Manager,  // ���в��ϵĹ���
        Nest  // �Ű�ʱ���в��ϵ�����
    };

    enum TabType{
        Default = -1,
        Whole,
        Strip,
        Package
    };
    explicit NestEngineConfigureDialog(NestEngineConfigure *config);
    explicit NestEngineConfigureDialog(NestEngineConfigure *config, TabType type);

    void setDialogRole(NestEngineConfigureDialog::RoleType role);  // ���öԻ����ɫ
    NestEngineConfigureDialog::RoleType getDialogRole();  // ��ȡ�Ի����ɫ

    NestEngineConfigure::StripSheetNest * getCurStripConfig();
    NestEngineConfigure::WholeSheetNest * getCurWholeConfig();
    NestEngineConfigure::PackageSheetNest * getCurPackageConfig();

public slots:
    void onTabChanged(int i);
    void onDialogButtonClicked(QAbstractButton *button);  // ��Ӧ������Ϣ�ı�
    void onDialogButtonOkClicked();
    void onDialogButtonCancelClicked();
    void onDataChanged(int index,QList<QList<int>> changedData);            //��Ӧ�������ݱ��༭�ı�

protected:
     void closeEvent(QCloseEvent *event);

private:
    RoleType role;
    TabType tabType;
    QTabWidget *tabWidget;
    StripSheetConfigTab *sSheetTab;
    WholeSheetConfigTab *wSheetTab;
    PackageSheetConfig *pSheetTab;
    QDialogButtonBox *buttonBox;
    QMap<int,QList<QList<int>>> dataMap;
    NestEngineConfigure::StripSheetNest *curStripConfig;
    NestEngineConfigure::WholeSheetNest *curWholeConfig;
    NestEngineConfigure::PackageSheetNest *curPackageConfig;
};

#endif // AUTONESTCONFIGUREDIALOG_H

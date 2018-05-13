#ifndef __TABDLG_H__
#define __TABDLG_H__

#include <QDialog>

class QWidget;
class QFileInfo;
class QTabWidget;
class QDialogButtonBox;

//常规选项卡
class GeneralTab : public QWidget
{
        Q_OBJECT

public:
        GeneralTab(const QFileInfo &fileInfo, QWidget *parent = 0);
};

//许可权限
class PermissionsTab : public QWidget
{
        Q_OBJECT

public:
        PermissionsTab(const QFileInfo &fileInfo, QWidget *parent = 0);
};

//程序
class ApplicationsTab : public QWidget
{
        Q_OBJECT

public:
        ApplicationsTab(const QFileInfo &fileInfo, QWidget *parent = 0);
};

//自定义Tab对话框
class TabDialog:public QDialog
{
public:
  TabDialog(const QString &fileName, QWidget *parent = 0);
private:
  QTabWidget *tabWidget;
  QDialogButtonBox *buttonBox;
};


#endif // __TABDLG_H__

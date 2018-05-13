#include <QTabWidget>
#include <QWidget>
#include <QDialog>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QDateTime>
#include <QGroupBox>
#include <QCheckBox>
#include <QListWidget>

#include "tabdlg.h"

TabDialog::TabDialog(const QString &fileName, QWidget *parent):QDialog(parent)
{
  QFileInfo fileInfo(fileName);

  tabWidget=new QTabWidget;
  tabWidget->addTab(new GeneralTab(fileName),trUtf8("常规"));
  tabWidget->addTab(new PermissionsTab(fileName),trUtf8("权限"));
  tabWidget->addTab(new ApplicationsTab(fileName),trUtf8("应用程序"));

  buttonBox=new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);

  connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));//OK
  connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));//Cancel

  QVBoxLayout *mainLayout=new QVBoxLayout;
  mainLayout->addWidget(tabWidget);
  mainLayout->addWidget(buttonBox);

  setLayout(mainLayout);

  setWindowTitle(trUtf8("Tab对话框"));
}

//常规面板类构造函数实现
GeneralTab::GeneralTab(const QFileInfo &fileInfo, QWidget *parent)
  :QWidget(parent)
{
         QLabel *fileNameLabel = new QLabel(trUtf8("文件名:"));
         QLineEdit *fileNameEdit = new QLineEdit(fileInfo.fileName());//去掉目录路径前缀后的

         QLabel *pathLabel = new QLabel(trUtf8("路径:"));
         QLabel *pathValueLabel = new QLabel(fileInfo.absoluteFilePath());//完整的路径名
         pathValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);//设置label的样式

         QLabel *sizeLabel = new QLabel(trUtf8("大小:"));
         qlonglong size = fileInfo.size()/1024;//size()返回字节大小
         QLabel *sizeValueLabel = new QLabel(trUtf8("%1 K").arg(size));
         sizeValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

         QLabel *lastReadLabel = new QLabel(trUtf8("上次读取时间:"));
         QLabel *lastReadValueLabel = new QLabel(fileInfo.lastRead().toString());
         lastReadValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

         QLabel *lastModLabel = new QLabel(trUtf8("上次修改时间:"));
         QLabel *lastModValueLabel = new QLabel(fileInfo.lastModified().toString());
         lastModValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

         QVBoxLayout *mainLayout = new QVBoxLayout;
         mainLayout->addWidget(fileNameLabel);
         mainLayout->addWidget(fileNameEdit);
         mainLayout->addWidget(pathLabel);
         mainLayout->addWidget(pathValueLabel);
         mainLayout->addWidget(sizeLabel);
         mainLayout->addWidget(sizeValueLabel);
         mainLayout->addWidget(lastReadLabel);
         mainLayout->addWidget(lastReadValueLabel);
         mainLayout->addWidget(lastModLabel);
         mainLayout->addWidget(lastModValueLabel);
         //mainLayout->addStretch(1);
         setLayout(mainLayout);
}

//权限面板类
PermissionsTab::PermissionsTab(const QFileInfo &fileInfo, QWidget *parent)
  :QWidget(parent)
{
    //群组框
         QGroupBox *permissionsGroup = new QGroupBox(trUtf8("权限"));

         QCheckBox *readable = new QCheckBox(trUtf8("可读"));
         if (fileInfo.isReadable())
                 readable->setChecked(true);//勾选

         QCheckBox *writable = new QCheckBox(trUtf8("可写"));
         if ( fileInfo.isWritable() )
                 writable->setChecked(true);

         QCheckBox *executable = new QCheckBox(trUtf8("可执行"));
         if ( fileInfo.isExecutable() )
                 executable->setChecked(true);

         QGroupBox *ownerGroup = new QGroupBox(trUtf8("所有权"));

         QLabel *ownerLabel = new QLabel(trUtf8("所有者"));
         QLabel *ownerValueLabel = new QLabel(fileInfo.owner());
         ownerValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

         QLabel *groupLabel = new QLabel(trUtf8("组"));
         QLabel *groupValueLabel = new QLabel(fileInfo.group());
         groupValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

         QVBoxLayout *permissionsLayout = new QVBoxLayout;
         permissionsLayout->addWidget(readable);
         permissionsLayout->addWidget(writable);
         permissionsLayout->addWidget(executable);
         permissionsGroup->setLayout(permissionsLayout);//权限组

         QVBoxLayout *ownerLayout = new QVBoxLayout;
         ownerLayout->addWidget(ownerLabel);
         ownerLayout->addWidget(ownerValueLabel);
         ownerLayout->addWidget(groupLabel);
         ownerLayout->addWidget(groupValueLabel);
         ownerGroup->setLayout(ownerLayout);//所有权组

         QVBoxLayout *mainLayout = new QVBoxLayout;
         mainLayout->addWidget(permissionsGroup);
         mainLayout->addWidget(ownerGroup);
         //mainLayout->addStretch(1);
         setLayout(mainLayout);
}

//应用程序选项卡类构造函数实现
ApplicationsTab::ApplicationsTab(const QFileInfo &fileInfo, QWidget *parent)
  :QWidget(parent)
{
         QLabel *topLabel = new QLabel(trUtf8("打开方式...:"));

         QListWidget *applicationsListBox = new QListWidget;
         QStringList applications;

         for (int i = 1; i <= 30; ++i)
                 applications.append(trUtf8("应用程序 %1").arg(i));
         applicationsListBox->insertItems(0, applications);

         QCheckBox *alwaysCheckBox;

         if (fileInfo.suffix().isEmpty())
                 alwaysCheckBox = new QCheckBox(trUtf8("始终使用该程序"
                         "打开该类型的文件"));
         else
                 alwaysCheckBox = new QCheckBox(trUtf8("始终使用该程序"
                         "打开此扩展名文件 '%1'").arg(fileInfo.suffix()));

         QVBoxLayout *layout = new QVBoxLayout;
         layout->addWidget(topLabel);
         layout->addWidget(applicationsListBox);
         layout->addWidget(alwaysCheckBox);
         setLayout(layout);
}

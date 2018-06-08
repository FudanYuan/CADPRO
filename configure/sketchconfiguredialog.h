#ifndef CONFIGUREDIALOG_H
#define CONFIGUREDIALOG_H

#include <QDialog>
#include <customwidget.h>
#include <sketchconfigure.h>
#include <QList>

//! [0]
class EntityStyleTab : public CustomTabWidget
{
    Q_OBJECT
public:
    explicit EntityStyleTab(SketchConfigure::EntityStyle &eStyle, QWidget *parent = 0);
};
//! [0]


//! [1]
class AxesGridTab : public CustomTabWidget
{
    Q_OBJECT
public:
    explicit AxesGridTab(SketchConfigure::AxesGrid &axesGrid, QWidget *parent = 0);
};
//! [1]


//! [2]
class OffsetTab : public CustomTabWidget
{
    Q_OBJECT
public:
    explicit OffsetTab(QList<SketchConfigure::Offset> &offset, QWidget *parent = 0);
};
//! [2]


//! [3]
class LanguageTab : public CustomTabWidget
{
    Q_OBJECT
public:
    explicit LanguageTab(SketchConfigure::Language &language, QWidget *parent = 0);
};
//! [3]


class SketchConfigureDialog : public QDialog
{
    Q_OBJECT
public:
    SketchConfigureDialog(SketchConfigure *config, QWidget *parent=0);
    ~SketchConfigureDialog();
    int hasKey(QString key);

private:
    QTabWidget *tabWidget;
    QList<SketchConfigure::KeyValue> keyValueList;
    QDialogButtonBox *buttonBox;

signals:
    void changed(QList<SketchConfigure::KeyValue>);
public slots:
    void accept();
    void reject();
    void onChanged(QString key, QVariant value);
};

#endif // SKETCHCONFIGUREDIALOG_H

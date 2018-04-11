#ifndef CONFIGUREDIALOG_H
#define CONFIGUREDIALOG_H

#include <QDialog>
#include <configure.h>
#include <QList>

namespace Ui {
class ConfigureDialog;
}

class CustomTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomTabWidget(QWidget *parent=0);

signals:
    void tabChanged(QString, QVariant);

public slots:
    void onColorChanged(QString key, QColor color);
    void onTextChanged(QString key, QString value);
    void onComboBoxChanged(QString key, int value);
    void onCheckChanged(QString key, bool value);
    void onLanguageChanged(QString key, int value);
};

//! [0]
class EntityStyleTab : public CustomTabWidget
{
    Q_OBJECT
public:
    explicit EntityStyleTab(Configure::EntityStyle &eStyle, QWidget *parent = 0);
};
//! [0]


//! [1]
class AxesGridTab : public CustomTabWidget
{
    Q_OBJECT
public:
    explicit AxesGridTab(Configure::AxesGrid &axesGrid, QWidget *parent = 0);
};
//! [1]


//! [2]
class OffsetTab : public CustomTabWidget
{
    Q_OBJECT
public:
    explicit OffsetTab(QList<Configure::Offset> &offset, QWidget *parent = 0);
};
//! [2]


//! [3]
class LanguageTab : public CustomTabWidget
{
    Q_OBJECT
public:
    explicit LanguageTab(Configure::Language &language, QWidget *parent = 0);
};
//! [3]


class ConfigureDialog : public QDialog
{
    Q_OBJECT
public:
    ConfigureDialog(Configure *config, QWidget *parent=0);
    ~ConfigureDialog();
    int hasKey(QString key);

private:
    Ui::ConfigureDialog *ui;
    QTabWidget *tabWidget;
    QList<Configure::KeyValue> keyValueList;
    QDialogButtonBox *buttonBox;

signals:
    void changed(QList<Configure::KeyValue>);
public slots:
    void accept();
    void reject();
    void onChanged(QString key, QVariant value);
};

#endif // CONFIGUREDIALOG_H

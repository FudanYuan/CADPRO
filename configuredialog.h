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
};

//! [0]
class EntityStyleTab : public CustomTabWidget
{
    Q_OBJECT
public:
    explicit EntityStyleTab(EntityStyle &eStyle, QWidget *parent = 0);
};
//! [0]


//! [1]
class AxesGridTab : public CustomTabWidget
{
    Q_OBJECT
public:
    explicit AxesGridTab(AxesGrid &axesGrid, QWidget *parent = 0);
};
//! [1]


//! [2]
class OffsetTab : public CustomTabWidget
{
    Q_OBJECT
public:
    explicit OffsetTab(QList<Offset> &offset, QWidget *parent = 0);
};
//! [2]


//! [3]
class LanguageTab : public CustomTabWidget
{
    Q_OBJECT
public:
    explicit LanguageTab(Language &language, QWidget *parent = 0);
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
    QList<KeyValue> keyValueList;
    QDialogButtonBox *buttonBox;

signals:
    void changed(QList<KeyValue>);
public slots:
    void accept();
    void reject();
    void onChanged(QString key, QVariant value);
};

#endif // CONFIGUREDIALOG_H

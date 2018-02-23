#ifndef CONFIGUREDIALOG_H
#define CONFIGUREDIALOG_H

#include <QDialog>
#include <configure.h>

namespace Ui {
class ConfigureDialog;
}

//! [0]
class EntityStyleTab : public QWidget
{
    Q_OBJECT
public:
    explicit EntityStyleTab(EntityStyle &eStyle, QWidget *parent = 0);

signals:
    void entityStyleChanged(QString, QVariant);
public slots:
    void onColorChanged(QString name, QColor color);
};
//! [0]


//! [1]
class AxesGridTab : public QWidget
{
    Q_OBJECT

public:
    explicit AxesGridTab(QWidget *parent = 0);
};
//! [1]


//! [2]
class OffsetTab : public QWidget
{
    Q_OBJECT

public:
    explicit OffsetTab(QWidget *parent = 0);
};
//! [2]


//! [3]
class LanguageTab : public QWidget
{
    Q_OBJECT

public:
    explicit LanguageTab(QWidget *parent = 0);
};
//! [3]


class ConfigureDialog : public QDialog
{
    Q_OBJECT
public:
    ConfigureDialog(Configure *config, QWidget *parent=0);
    ~ConfigureDialog();

private:
    Ui::ConfigureDialog *ui;
    QTabWidget *tabWidget;
    QDialogButtonBox *buttonBox;

};

#endif // CONFIGUREDIALOG_H

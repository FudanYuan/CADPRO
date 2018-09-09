#ifndef NESTCONFIGUREDIALOG_H
#define NESTCONFIGUREDIALOG_H

#include <QDialog>
#include <customwidget.h>
#include <nestconfigure.h>
#include <QList>

//! [0]
class MiscellaneousTab : public CustomTabWidget
{
    Q_OBJECT
public:
    explicit MiscellaneousTab(QWidget *parent = 0);
    void setMainGrid(const NestConfigure::Grid &mainGrid);
    void setSecondGrid(const NestConfigure::Grid &secondGrid);
    void setAllowEntityOutOfSheet(bool flag);

private:
    CheckBox *showMainGrid;
    LineEdit *mainGridXStep;
    LineEdit *mainGridYStep;
    ColorComboBox *mainGridColor;

    CheckBox *showSecondGrid;
    LineEdit *secondGridXStep;
    LineEdit *secondGridYStep;
    ColorComboBox *secondGridColor;

    CheckBox *allowEntityOutOfSheetCBox;
};
//! [0]


//! [1]
class ColorsTab : public CustomTabWidget
{
    Q_OBJECT
public:
    explicit ColorsTab(QWidget *parent = 0);
    void setSheetStyle(const NestConfigure::SheetStyle &sheetStyle);
    void setPieceStyle(const NestConfigure::PieceStyle &pieceStyle);
    void setBackground(const QColor &color);
    void setShowPiecesDistance(const bool &flag);

private:
    ColorComboBox *sheetColor;
    ColorComboBox *sheetMarginColor;
    ColorComboBox *sheetReinforcementColor;
    ColorComboBox *sheetReinforcementMarginColor;
    CheckBox *sheetFilled;

    ColorComboBox *outsideColor;
    ColorComboBox *insideColor;
    ColorComboBox *liningColor;
    ColorComboBox *pieceReinforcementColor;
    ColorComboBox *otherColor;

    ColorComboBox *backgroundColor;
    CheckBox *showPiecesDistance;
};
//! [1]


//! [2]
class NestLanguageTab : public CustomTabWidget
{
    Q_OBJECT
public:
    explicit NestLanguageTab(QWidget *parent = 0);
    void setLanguage(const NestConfigure::Language &language);
    void setWorkPath(const QString &workPathStr);

public slots:
    void onWorkPathBtnClicked(bool flag);

private:
    ComboBox *languageCBox;
    LineEdit *workPathLine;
};

//! [2]

class NestConfigureDialog : public QDialog
{
    Q_OBJECT
public:
    NestConfigureDialog(NestConfigure *config, QWidget *parent=0);
    ~NestConfigureDialog();
    int hasKey(QString key);

private:
    QTabWidget *tabWidget;
    QList<NestConfigure::KeyValue> keyValueList;
    QDialogButtonBox *buttonBox;

signals:
    void changed(QList<NestConfigure::KeyValue>);

public slots:
    void accept();
    void reject();
    void onChanged(QString key, QVariant value);
};

#endif // NESTCONFIGUREDIALOG_H

#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QColorDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QPaintEvent>

class ColorButton : public QPushButton
{
    Q_OBJECT
public:
    ColorButton(QString color, QWidget *parent=0);

    QString getColor();  // 获取按钮颜色
private:
    QString color;  // 按钮颜色

signals:
    void clicked(QString);

public slots:
    void onPressed();
};

class ColorComboBox : public QComboBox
{
    Q_OBJECT
public:
    ColorComboBox(QString name, QWidget *parent = 0);
    void setColor(QColor color);
private:
    QString name;
    QTableWidget *tableWidget;
    QPushButton *other;
    QLabel *showColor;
signals:
    void colorChanged(QString name, QColor color);

public slots:
    void change(QString colorName);
    void otherColor();
};

class LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit LineEdit(QString name, QWidget *parent=0);
    QString getName();

private:
    QString name;

signals:
    void customTextEdited(QString name, QString value);

public slots:
    void onTextEdited(const QString &value);
};

class CheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit CheckBox(const QString &name, QWidget *parent=0);
    explicit CheckBox(const QString &name, const QString &text, QWidget *parent=0);
    QString getName();

private:
    QString name;

signals:
    void customStateChanged(QString name, int value);

public slots:
    void onStateChanged(int value);
};

class ComboBox : public QComboBox
{
  Q_OBJECT
public:
    ComboBox(QString name, QWidget *parent = 0);

private:
    QString name;

signals:
    void customActivated(QString name, int index);

public slots:
    void onActivated(int index);
};

class lineStyleComboBox : public QComboBox
{
    Q_OBJECT
public:
    lineStyleComboBox(QString name, QWidget *parent = 0);

private:
    QString name;

signals:
    void customActivated(QString name, int index);

public slots:
    void onActivated(int index);
};

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

#endif // CUSTOMWIDGET_H

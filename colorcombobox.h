#ifndef COLORCOMBOBOX_H
#define COLORCOMBOBOX_H

#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QColorDialog>

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

#endif // COLORCOMBOBOX_H

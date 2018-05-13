#ifndef ITEMPROPERTIES_H
#define ITEMPROPERTIES_H

#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QDialog>
#include <QPen>
#include "sketchconfigure.h"

class ItemProperties : public QDialog
{
    Q_OBJECT

public:
    ItemProperties();
    void initdialog();

    int getShapeid() const;
    void setShapeid(int value);

    double getLength() const;
    void setLength(double value);

    bool getOk() const;
    void setOk(bool value);

    QPen getPen() const;
    void setPen(const QPen &value);

private:
    int shapeid;
    double length;
    bool ok;
    QPen pen;

    QVBoxLayout *mainLayout;
    QTableWidget *tableWidget;
    QComboBox *linestyle;
    QComboBox *linecolor;

signals:
    void PropertiesChanged();
public slots:
    void typechanged();
    void linestylechange();
    void linecolorchange();

};

#endif // ITEMPROPERTIES_H

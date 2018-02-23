#ifndef LINESTYLECOMBOBOX_H
#define LINESTYLECOMBOBOX_H

#include <QComboBox>
#include <QPaintEvent>

class lineStyleComboBox : public QComboBox
{
    Q_OBJECT
public:
    lineStyleComboBox(QWidget *parent = 0);

protected:
//    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

#endif // LINESTYLECOMBOBOX_H

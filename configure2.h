#ifndef CONFIGURE2_H
#define CONFIGURE2_H

#include <QDialog>

namespace Ui {
class Configure2;
}

class Configure2 : public QDialog
{
    Q_OBJECT

public:
    explicit Configure2(QWidget *parent = 0);
    ~Configure2();

private:
    Ui::Configure2 *ui;
};

#endif // CONFIGURE2_H

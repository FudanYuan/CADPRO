#include "configure2.h"
#include "ui_configure2.h"

Configure2::Configure2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Configure2)
{
    ui->setupUi(this);
}

Configure2::~Configure2()
{
    delete ui;
}

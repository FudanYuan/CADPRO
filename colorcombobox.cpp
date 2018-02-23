#include "colorcombobox.h"
#include <QHeaderView>
#include <QPixmap>
#include <QIcon>
#include <QSize>
#include <QDebug>

#define ROWS 4
#define COLUMNS 6

ColorComboBox::ColorComboBox(QString name, QWidget *parent) :
    QComboBox(parent)
{
    this->name = name;
    tableWidget = new QTableWidget(ROWS, COLUMNS);
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->horizontalHeader()->setVisible(false);
    tableWidget->setShowGrid(false);
    tableWidget->setSpan(ROWS-1,0,1,COLUMNS);
    int k = 0;
    for (int i = 0; i < ROWS; ++i)
    {
        if(i<ROWS-1){
            for (int j = 0; j < COLUMNS; ++j)
                {
                    tableWidget->setColumnWidth(j, width()/COLUMNS);
                    tableWidget->setRowHeight(j, width()/COLUMNS);
                    QStringList colorList = QColor::colorNames();
                    ColorButton *colorBtn = new ColorButton(colorList[k], this);
                    k++;
                    tableWidget->setCellWidget(i, j, colorBtn);
                    connect(colorBtn, &ColorButton::clicked, this, &ColorComboBox::change);
            }
        }else{
            tableWidget->setRowHeight(i, 20);
            other =new QPushButton(this);
            other->setText(tr("更多颜色"));
            tableWidget->setCellWidget(ROWS-1, 0, other);
        }
     }
    connect(other,SIGNAL(clicked()),this,SLOT(otherColor()));
    tableWidget->setMinimumSize(width(), height());
    this->setModel(tableWidget->model());
    this->setView(tableWidget);

    showColor = new QLabel(this);
}

void ColorComboBox::setColor(QColor color)
{
    showColor->setStyleSheet(tr("QLabel {background-color: #") + QString::number(color.rgb(), 16) + tr(";border-radius:2px;}"));
    showColor->setGeometry(3,2,this->width()-26,this->height()-10);
    showColor->setVisible(true);
    this->hidePopup();
}

void ColorComboBox::change(QString colorName)
{
    qDebug() << "change " << this->width() << " " << this->height();
    showColor->setStyleSheet(tr("QLabel {background-color: ") + colorName + tr(";border-radius:2px;}"));
    showColor->setGeometry(3,2,this->width()-23,this->height()-6);
    showColor->setVisible(true);
    this->hidePopup();
    emit colorChanged(this->name, QColor(colorName));
}

void ColorComboBox::otherColor()
{
   QColor mycolor = QColorDialog::getColor(Qt::white, this);
   this->change(mycolor.name());
}

ColorButton::ColorButton(QString color, QWidget *parent) :
    QPushButton(parent)
{
    this->color = color;
    this->setStyleSheet(tr("ColorButton {background-color: ") + color + tr(";}"));
    connect(this, &ColorButton::pressed, this, &ColorButton::onPressed);
}

QString ColorButton::getColor()
{
    return color;
}

void ColorButton::onPressed()
{
    emit clicked(this->color);
}

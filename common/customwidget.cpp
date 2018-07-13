#include "customwidget.h"
#include <QHeaderView>
#include <QPixmap>
#include <QIcon>
#include <QSize>
#include <QImage>
#include <QPainter>
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

LineEdit::LineEdit(QString name, QWidget *parent) :
    QLineEdit(parent)
{
    this->name = name;
    connect(this, &LineEdit::textEdited, this, &LineEdit::onTextEdited);
}

QString LineEdit::getName()
{
    return this->name;
}

void LineEdit::onTextEdited(const QString &value)
{
    emit customTextEdited(this->name, value);
}

CheckBox::CheckBox(const QString &name, QWidget *parent) :
    QCheckBox(parent)
{
    this->name = name;
    connect(this, &CheckBox::stateChanged, this, &CheckBox::onStateChanged);
}

CheckBox::CheckBox(const QString &name, const QString &text, QWidget *parent):
    QCheckBox(text, parent)
{
    this->name = name;
    connect(this, &CheckBox::stateChanged, this, &CheckBox::onStateChanged);
}

QString CheckBox::getName()
{
    return this->name;
}

void CheckBox::onStateChanged(int value)
{
    emit customStateChanged(this->name, value);
}

lineStyleComboBox::lineStyleComboBox(QString name, QWidget *parent) :
    QComboBox(parent)
{
    this->name = name;

    QPixmap pixmap(width(), height());  //设置图像
    pixmap.fill(Qt::white);
    QPainter p(&pixmap);
    p.setPen(Qt::SolidLine);
    p.drawLine(0, height()/2, width(), height()/2);

    addItem(QIcon(pixmap), NULL, (int)Qt::SolidLine);

    QPixmap pixmap1(width(), height());  //设置图像
    pixmap1.fill(Qt::white);
    QPainter p1(&pixmap1);
    p1.setPen(Qt::DashLine);
    p1.drawLine(0, height()/2, width(), height()/2);

    addItem(QIcon(pixmap1), NULL, (int)Qt::DashLine);

    QPixmap pixmap2(width(), height());  //设置图像
    QPainter p2(&pixmap2);
    pixmap2.fill(Qt::white);
    p2.setPen(Qt::DotLine);
    p2.drawLine(0, height()/2, width(), height()/2);

    addItem(QIcon(pixmap2), NULL, (int)Qt::DotLine);

    QPixmap pixmap3(width(), height());  //设置图像
    QPainter p3(&pixmap3);
    pixmap3.fill(Qt::white);
    p3.setPen(Qt::DashDotLine);
    p3.drawLine(0, height()/2, width(), height()/2);

    addItem(QIcon(pixmap3), NULL, (int)Qt::DashDotLine);

    QPixmap pixmap4(width(), height());  //设置图像
    QPainter p4(&pixmap4);
    pixmap4.fill(Qt::white);
    p4.setPen(Qt::DashDotDotLine);
    p4.drawLine(0, height()/2, width(), height()/2);

    addItem(QIcon(pixmap4), NULL, (int)Qt::DashDotDotLine);
    setIconSize(pixmap4.size());

    setSizeAdjustPolicy(QComboBox::AdjustToContents);  //设置下拉列表的尺寸符合内容的大小

    connect(this, SIGNAL(activated(int)), this, SLOT(onActivated(int)));
}

void lineStyleComboBox::onActivated(int index)
{
    emit customActivated(this->name, index + 1);
}

ComboBox::ComboBox(QString name, QWidget *parent) :
    QComboBox(parent)
{
    this->name = name;
    connect(this, SIGNAL(activated(int)), this, SLOT(onActivated(int)));
}

void ComboBox::onActivated(int index)
{
    emit customActivated(this->name, index);
}

CustomTabWidget::CustomTabWidget(QWidget *parent) :
    QWidget(parent)
{

}

void CustomTabWidget::onColorChanged(QString key, QColor color)
{
#if 1
    qDebug() << "CustomTabWidget:: " << key << " " << color.rgba();
#endif
    emit tabChanged(key, QVariant(color.rgba()));
}

void CustomTabWidget::onTextChanged(QString key, QString value)
{
    emit tabChanged(key, QVariant(value));
}

void CustomTabWidget::onComboBoxChanged(QString key, int value)
{
    qDebug() << value << "  ,penStyle: " << (Qt::PenStyle)value;
    emit tabChanged(key, QVariant(value));
}

void CustomTabWidget::onCheckChanged(QString key, bool value)
{
    emit tabChanged(key, QVariant(value));
}

CustomDockWidget::CustomDockWidget(const QString &title, QWidget *parent) :
    QDockWidget(title, parent)
{

}

void CustomDockWidget::changeEvent(QEvent *event)
{
    qDebug() << (QEvent::Type)event->type();
    switch (event->type()) {
    case QEvent::Close:
    case QEvent::Resize:
        QDockWidget::changeEvent(event);
        break;
    default:
        event->ignore();
        break;
    }
}

void CustomDockWidget::closeEvent(QCloseEvent *event)
{
    QDockWidget::closeEvent(event);
    emit sizeChanged();
}

void CustomDockWidget::resizeEvent(QResizeEvent *event)
{
    //qDebug() << "dockwidget resize event";
    QWidget::resizeEvent(event);
    emit sizeChanged();
}

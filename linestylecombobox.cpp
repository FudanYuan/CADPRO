#include "linestylecombobox.h"
#include <QImage>
#include <QPainter>
#include <QDebug>

lineStyleComboBox::lineStyleComboBox(QWidget *parent) :
    QComboBox(parent)
{
    QPixmap pixmap(width(), height());                                  //设置图像
    QPainter p(&pixmap);
    p.setPen(Qt::SolidLine);
    p.drawLine(0, height()/2, width(), height()/2);

    addItem(QIcon(pixmap), NULL);
    setIconSize(pixmap.size());

    QPixmap pixmap1(width(), height());                                  //设置图像
    QPainter p1(&pixmap1);
    p1.setPen(Qt::DashLine);
    p1.drawLine(0, height()/2, width(), height()/2);

    addItem(QIcon(pixmap1), NULL);
    setIconSize(pixmap1.size());

    QPixmap pixmap2(width(), height());                                  //设置图像
    QPainter p2(&pixmap2);
    p2.setPen(Qt::DotLine);
    p2.drawLine(0, height()/2, width(), height()/2);

    addItem(QIcon(pixmap2), NULL);
    setIconSize(pixmap2.size());

    QPixmap pixmap3(width(), height());                                  //设置图像
    QPainter p3(&pixmap3);
    p3.setPen(Qt::DashDotLine);
    p3.drawLine(0, height()/2, width(), height()/2);

    addItem(QIcon(pixmap3), NULL);
    setIconSize(pixmap3.size());

    QPixmap pixmap4(width(), height());                                  //设置图像
    QPainter p4(&pixmap4);
    p4.setPen(Qt::DashDotDotLine);
    p4.drawLine(0, height()/2, width(), height()/2);

    addItem(QIcon(pixmap4), NULL);
    setIconSize(pixmap4.size());

    setSizeAdjustPolicy(QComboBox::AdjustToContents);   //设置下拉列表的尺寸符合内容的大小
}

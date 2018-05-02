#include "insertoffsetdialog.h"

InsertOffsetDialog::InsertOffsetDialog()
{
    intidialog();
}

void InsertOffsetDialog::intidialog()
{
    mainLayout = new QHBoxLayout;//主框架
    layout1 = new QVBoxLayout();//水平框架
    layout2 = new QVBoxLayout();

    QGroupBox *box1 =new QGroupBox(tr("属性特征"));
    QRadioButton *radio1 = new QRadioButton(tr("通用"));
    radio1->setChecked(true);
    QRadioButton *radio2 = new QRadioButton(tr("标记"));
    QRadioButton *radio3 = new QRadioButton(tr("周长"));
    QRadioButton *radio4 = new QRadioButton(tr("切割"));
    QRadioButton *radio5 = new QRadioButton(tr("缝线"));
    layout2->addWidget(radio1);
    layout2->addWidget(radio2);
    layout2->addWidget(radio3);
    layout2->addWidget(radio4);
    layout2->addWidget(radio5);
    box1->setLayout(layout2);
    layout1->addWidget(box1);

    QLabel *label1 =new QLabel(tr("距离"));
    QLineEdit *edit1 =new QLineEdit;
    edit1->setText(tr("10"));
    QLabel *label2 =new QLabel(tr("nm"));
    QHBoxLayout *layout3 = new QHBoxLayout;
    layout3->addWidget(label1);
    layout3->addWidget(edit1);
    layout3->addWidget(label2);
    layout3->setStretch(0,10);
    layout3->setStretch(1,10);
    layout3->setStretch(3,5);
    layout1->addLayout(layout3);

    QPushButton *button1 =new QPushButton(tr("自由"));
    layout1->addWidget(button1);

    layout1->setStretch(0,50);
    layout1->setStretch(1,10);
    layout1->setStretch(0,20);

    QHBoxLayout *layout4 = new QHBoxLayout();
    QTableWidget *table_widget =new QTableWidget;
    table_widget->setColumnCount(3);
    table_widget->setFrameShape(QFrame::Box);//设置边框
    table_widget->setShowGrid(true); //设置显示格子线
    //表头内容
    QStringList m_Header;
    m_Header<<tr("偏移类型")<<tr("距离")<<tr("颜色");
    table_widget->setHorizontalHeaderLabels(m_Header);
    table_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout4->addWidget(table_widget);

    mainLayout->addLayout(layout1);
    mainLayout->addLayout(layout4);
    mainLayout->setStretch(0,10);
    mainLayout->setStretch(1,17);

    //主对话框的大小、名称
    setMaximumSize(600,280);
    setMinimumSize(600,280);
    setLayout(mainLayout);
    setWindowTitle(tr("偏移"));
}

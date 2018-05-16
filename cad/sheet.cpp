#include "sheet.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QMessageBox>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

Sheet::Sheet() :
      name(""),
      type(Whole),
      material(""),
      width(1000),
      height(1000),
      componentGap(0),
      topMargin(0),
      rightMargin(0),
      bottomMargin(0),
      leftMargin(0),
      layers(1),
      margin(0),
      doubleStrip(false),
      cutPaneSize(INT_MAX)
{
    StripPW pw;
    stripPW.append(pw);
}

SheetDialog::SheetDialog() :
    role(Manager),
    sheetActive(NULL),
    currentIndex(0),
    insertFlag(false),
    editFlag(false),
    insertCounter(0),
    editCounter(0),
    removeCounter(0)
{
    initDialog();
    loadSheetInfo();
}

void SheetDialog::setDialogRole(SheetDialog::RoleType role)
{
    this->role = role;
}

SheetDialog::RoleType SheetDialog::getDialogRole()
{
    return this->role;
}

void SheetDialog::initDialog()
{
    setWindowTitle(tr("材料管理"));
    // 适应屏幕大小
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    setFixedSize(screenRect.width() - 400, screenRect.height()-100);
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));

    // 初始化材料列表部分
    initSheetListPanel();

    // 初始化材料详情部分
    initSheetInfoPanel();

    // 初始化按钮部分
    initBtnPanel();

    mainLayout = new QVBoxLayout(this); // 主布局
    mainLayout->addWidget(sheetListGroupBox);
    mainLayout->addWidget(sheetInfoGroupBox);
    mainLayout->addItem(new QSpacerItem(this->width(),
                                        10,
                                        QSizePolicy::Expanding,
                                        QSizePolicy::Expanding));
    mainLayout->addWidget(buttonBox);
}

void SheetDialog::initSheetListPanel()
{
    // group box
    sheetListGroupBox = new QGroupBox(tr("材料列表"), this);

    // 布局
    QGridLayout *listLayout = new QGridLayout(sheetListGroupBox);

    // 材料列表
    sheetListTable = new QTableWidget(1, 6, sheetListGroupBox);

    // 接受选中事件
    sheetListTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 设置列名
    QStringList sList;
    sList << tr("类型") << tr("名称")
          << tr("材料") << tr("尺寸")
          << tr("间隔") << tr("上插板宽度");
    sheetListTable->setHorizontalHeaderLabels(sList);
    sheetListTable->horizontalHeader()->setStretchLastSection(true);

    // 设置列宽
    sheetListTable->setColumnWidth(0,50);
    sheetListTable->setColumnWidth(1,100);
    sheetListTable->setColumnWidth(2,100);
    sheetListTable->setColumnWidth(3,100);
    sheetListTable->setColumnWidth(4,100);
    sheetListTable->setColumnWidth(4,100);

    //去掉水平滚动条
    sheetListTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //垂直滚动条按项移动
    sheetListTable->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);

    sheetListTable->verticalHeader()->setVisible(false);//隐藏左边垂直
    sheetListTable->setSelectionBehavior(QTableWidget::SelectRows);//一次选中一行
    sheetListTable->setSelectionMode(QAbstractItemView::SingleSelection);//只能单选
    sheetListTable->show();
    connect(sheetListTable, &QTableWidget::clicked, this, &SheetDialog::onSheetSelected);
    connect(sheetListTable, &QTableWidget::activated, this, &SheetDialog::onSheetSelected);

    // 材料管理按钮
    sheetInsertBtn = new QPushButton(tr("增加"), sheetListGroupBox);
    sheetRemoveBtn = new QPushButton(tr("删除"), sheetListGroupBox);
    sheetEditBtn = new QPushButton(tr("编辑"), sheetListGroupBox);

    sheetRemoveBtn->setHidden(true);
    sheetEditBtn->setHidden(true);
    connect(sheetInsertBtn, &QPushButton::clicked, this, &SheetDialog::onSheetInsert);
    connect(sheetRemoveBtn, &QPushButton::clicked, this, &SheetDialog::onSheetRemove);
    connect(sheetEditBtn, &QPushButton::clicked, this, &SheetDialog::onSheetEdit);

    listLayout->addWidget(sheetListTable, 0, 0, 20, 10);
    listLayout->addWidget(sheetInsertBtn, 20, 0, 1, 2);
    listLayout->addWidget(sheetRemoveBtn, 20, 2, 1, 2);
    listLayout->addWidget(sheetEditBtn, 20, 4, 1, 2);
//    sheetListGroupBox->setLayout(listLayout);
}

void SheetDialog::initSheetInfoPanel()
{
    sheetInfoGroupBox = new QGroupBox(tr("材料详情"), this);

    // 材料详情总布局
    infoLayout = new QGridLayout(sheetInfoGroupBox);

    // 材料属性布局
    sheetPropertyLayout = new QGridLayout();

    typeLabel = new QLabel(tr("类型"), sheetInfoGroupBox);
    typeCBox = new QComboBox(sheetInfoGroupBox);
    typeCBox->insertItem(0, WHOLE);
    typeCBox->insertItem(1, STRIP);
    typeCBox->insertItem(3, PACKAGE);
    typeCBox->setCurrentIndex(0);
    connect(typeCBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSheetTypeComChanged(int)));

    nameLabel = new QLabel(tr("名称"), sheetInfoGroupBox);
    nameLine = new QLineEdit(tr(""), sheetInfoGroupBox);
    materialLabel = new QLabel(tr("材质"), sheetInfoGroupBox);
    materialLine = new QLineEdit(tr(""), sheetInfoGroupBox);

    QGroupBox *sheetInfoDimensionGroupBox = new QGroupBox(tr("尺寸"), sheetInfoGroupBox);
    QHBoxLayout *dimensionLayout = new QHBoxLayout(sheetInfoDimensionGroupBox);
    xLabel = new QLabel(tr("X(宽度)"), sheetInfoDimensionGroupBox);
    xLine = new QLineEdit(tr(""), sheetInfoDimensionGroupBox);
    yLabel = new QLabel(tr("Y(长度)"), sheetInfoDimensionGroupBox);
    yLine = new QLineEdit(tr(""), sheetInfoDimensionGroupBox);
    dimensionLayout->addWidget(xLabel);
    dimensionLayout->addWidget(xLine);
    dimensionLayout->addWidget(yLabel);
    dimensionLayout->addWidget(yLine);

    distanceLabel = new QLabel(tr("切割件之间的距离"), sheetInfoGroupBox);
    distanceLine = new QLineEdit(tr(""), sheetInfoGroupBox);

    QGroupBox *sheetInfoMarginGroupBox = new QGroupBox(tr("边距余量"), sheetInfoGroupBox);
    QGridLayout *marginLayout = new QGridLayout(sheetInfoMarginGroupBox);
    leftLabel = new QLabel(tr("左"), sheetInfoMarginGroupBox);
    leftLine = new QLineEdit(tr(""), sheetInfoMarginGroupBox);
    topLabel = new QLabel(tr("顶"), sheetInfoMarginGroupBox);
    topLine = new QLineEdit(tr(""), sheetInfoMarginGroupBox);
    rightLabel = new QLabel(tr("右"), sheetInfoMarginGroupBox);
    rightLine = new QLineEdit(tr(""), sheetInfoMarginGroupBox);
    bottomLabel = new QLabel(tr("底"), sheetInfoMarginGroupBox);
    bottomLine = new QLineEdit(tr(""), sheetInfoMarginGroupBox);
    marginLabel = new QLabel(tr("间隔"), sheetInfoMarginGroupBox);
    marginLine = new QLineEdit(tr(""), sheetInfoMarginGroupBox);
    layerLabel = new QLabel(tr("切割层数"), sheetInfoMarginGroupBox);
    layerLine = new QLineEdit(tr(""), sheetInfoMarginGroupBox);
    doubleStrip = new QCheckBox(tr("双条"), sheetInfoMarginGroupBox);
    connect(doubleStrip, &QCheckBox::toggled, this, &SheetDialog::onDoubleStripChanged);
    cutSizeLabel = new QLabel(tr("平面切割尺寸"), sheetInfoMarginGroupBox);
    cutSizeLine = new QLineEdit(tr(""), sheetInfoMarginGroupBox);

    marginLayout->addWidget(leftLabel, 0, 0, 1, 1);
    marginLayout->addWidget(leftLine, 0, 1, 1, 1);
    marginLayout->addWidget(topLabel, 0, 2, 1, 1);
    marginLayout->addWidget(topLine, 0, 3, 1, 1);
    marginLayout->addWidget(rightLabel, 1, 0, 1, 1);
    marginLayout->addWidget(rightLine, 1, 1, 1, 1);
    marginLayout->addWidget(bottomLabel, 1, 2, 1, 1);
    marginLayout->addWidget(bottomLine, 1, 3, 1, 1);
    marginLayout->addWidget(marginLabel, 2, 0, 1, 1);
    marginLayout->addWidget(marginLine, 2, 1, 1, 1);
    marginLayout->addWidget(layerLabel, 2, 2, 1, 1);
    marginLayout->addWidget(layerLine, 2, 3, 1, 1);
    marginLayout->addWidget(doubleStrip, 3, 0, 1, 1);
    marginLayout->addWidget(cutSizeLabel, 3, 2, 1, 1);
    marginLayout->addWidget(cutSizeLine, 3, 3, 1, 1);

    sheetPropertyLayout->addWidget(typeLabel, 0, 0, 1, 1);
    sheetPropertyLayout->addWidget(typeCBox, 0, 1, 1, 1);
    sheetPropertyLayout->addWidget(nameLabel, 1, 0, 1, 1);
    sheetPropertyLayout->addWidget(nameLine, 1, 1, 1, 1);
    sheetPropertyLayout->addWidget(materialLabel, 2, 0, 1, 1);
    sheetPropertyLayout->addWidget(materialLine, 2, 1, 1, 1);
    sheetPropertyLayout->addWidget(sheetInfoDimensionGroupBox, 3, 0, 2, 2);
    sheetPropertyLayout->addWidget(distanceLabel, 5, 0, 1, 1);
    sheetPropertyLayout->addWidget(distanceLine, 5, 1, 1, 1);
    sheetPropertyLayout->addWidget(sheetInfoMarginGroupBox, 6, 0, 4, 2);
    // 材料属性布局结束

    // 当材料类型为上条插板时，显示位置配置
    // 材料列表
    stripConfigTable = new QTableWidget(0, 2, sheetInfoGroupBox);

    // 设置列名
    QStringList sList;
    sList << tr("位置") << tr("宽度");
    stripConfigTable->setHorizontalHeaderLabels(sList);
    stripConfigTable->horizontalHeader()->setStretchLastSection(true);

    // 双击编辑
    stripConfigTable->setEditTriggers(QAbstractItemView::DoubleClicked);

    // 设置自动等宽
    stripConfigTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //按需显示水平滚动条
    stripConfigTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //垂直滚动条按项移动
    stripConfigTable->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    stripConfigTable->verticalHeader()->setVisible(false);//隐藏左边垂直
    stripConfigTable->setSelectionBehavior(QTableWidget::SelectItems);//一次选中一项
    stripConfigTable->setSelectionMode(QAbstractItemView::SingleSelection);//只能单选

    sheetView = new View(sheetInfoGroupBox);  // 初始化sheet view
    sheetScene = new Scene(sheetView);
    SketchConfigure config;
    sheetScene->setEntityStyle(config.eStyle);
//    sheetView->setMouseFlag(false);  // 忽略鼠标事件
//    sheetView->setWheelFlag(false);  // 忽略滚轮事件
//    sheetView->setKeyboardFlag(false);  // 忽略键盘事件

    infoLayout->addLayout(sheetPropertyLayout, 0, 0, 10, 2);
    infoLayout->addWidget(stripConfigTable, 0, 2, 10, 2);
    infoLayout->addItem(new QSpacerItem(stripConfigTable->width(),
                                        stripConfigTable->height(),
                                        QSizePolicy::Expanding,
                                        QSizePolicy::Expanding),
                             0, 2, 10, 1);
    infoLayout->addWidget(sheetView, 0, 6, 10, 1);
//    sheetInfoGroupBox->setLayout(infoLayout);
    sheetInfoGroupBox->setHidden(true);
    setSheetInfoDisable(true);
}

void SheetDialog::initBtnPanel()
{
    // 将确定取消的按键加入布局
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Apply
                                     | QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel, this);
    buttonBox->button(QDialogButtonBox::Apply)->setDisabled(true);
    buttonBox->button(QDialogButtonBox::Cancel)->setDisabled(true);
    connect(buttonBox, &QDialogButtonBox::clicked, this, &SheetDialog::onDialogButtonClicked);
}

void SheetDialog::loadSheetInfo()
{
    // 读取xml文件
    sheetList.clear();
    sheetList.append(xmlFileReader(SHEET_XML));
    if(sheetList.length() == 0){
        QMessageBox::warning(this, tr("警告"), tr("材料列表为空!"));
        return;
    }
    updateSheetListTable(sheetList);
}

void SheetDialog::saveSheetInfo()
{
    qDebug() << sheetList.length() << " to save";
    xmlFileWrite(SHEET_XML, sheetList);
}

void SheetDialog::updateSheetInfo(const Sheet *sheetActive)
{
    sheetInfoGroupBox->setHidden(false);
    typeCBox->setCurrentIndex(sheetActive->type);
    nameLine->setText(sheetActive->name);
    materialLine->setText(sheetActive->material);
    xLine->setText(QString("%1").arg(sheetActive->width));
    distanceLine->setText(QString("%1").arg(sheetActive->componentGap));
    leftLine->setText(QString("%1").arg(sheetActive->leftMargin));
    topLine->setText(QString("%1").arg(sheetActive->topMargin));
    rightLine->setText(QString("%1").arg(sheetActive->rightMargin));
    bottomLine->setText(QString("%1").arg(sheetActive->bottomMargin));
    if(sheetActive->type == Sheet::Strip){
        marginLine->show();
        doubleStrip->show();
        cutSizeLabel->show();
        cutSizeLine->show();
        marginLabel->show();
        layerLabel->hide();
        layerLine->hide();
        marginLine->setText(QString("%1").arg(sheetActive->margin));
        doubleStrip->setChecked(sheetActive->doubleStrip);
        cutSizeLine->setText(QString("%1").arg(sheetActive->cutPaneSize));
        updateStripConfigTable(sheetActive->stripPW);
    }
    else{
        layerLabel->show();
        layerLine->show();
        marginLabel->hide();
        marginLine->hide();
        doubleStrip->hide();
        cutSizeLabel->hide();
        cutSizeLine->hide();
        layerLine->setText(QString("%1").arg(sheetActive->layers));
        stripConfigTable->hide();
    }

    if(sheetActive->type == Sheet::Package){
        yLine->setText(tr("00"));
        yLine->setDisabled(true);
    } else{
        yLine->setText(QString("%1").arg(sheetActive->height));
    }

    // 更新材料视图
    sheetView->setSceneRect(QRectF(0, 0, sheetActive->width, sheetActive->height));
    sheetView->centerOn(sheetView->mapFromScene(0,0));
    sheetScene->setSceneRect(sheetView->rect());
//    Rect *rect = new Rect;
//    rect->setRect(sheetActive->layoutRect());
//    sheetScene->addCustomRectItem(rect);
//    sheetScene->setCurShape(Shape::Rectangle);
    //qDebug() << sheetView->width() << " " << sheetView->height();
    //qDebug() << sheetScene->width() << " " << sheetScene->height();

    Rect *rect = new Rect;
    SketchConfigure::PenStyle pen;
    pen.setPenStyle(Qt::black, Qt::DashLine, 1);
    rect->setPenStyle(pen);
    rect->setRect(sheetActive->layoutRect());
    sheetScene->addCustomRectItem(rect);
}

void SheetDialog::updateSheetActive(Sheet *sheetActive)
{
    sheetActive->type = (Sheet::SheetType)typeCBox->currentIndex();
    sheetActive->name = nameLine->text();
    sheetActive->material = materialLine->text();
    sheetActive->width = xLine->text().toDouble();
    sheetActive->height = yLine->text().toDouble();
    sheetActive->componentGap = distanceLine->text().toDouble();
    sheetActive->leftMargin = leftLine->text().toDouble();
    sheetActive->topMargin = topLine->text().toDouble();
    sheetActive->rightMargin = rightLine->text().toDouble();
    sheetActive->bottomMargin = bottomLine->text().toDouble();
    if(sheetActive->type == Sheet::Strip){
        sheetActive->margin = marginLine->text().toDouble();
        sheetActive->doubleStrip = doubleStrip->isChecked();
        sheetActive->cutPaneSize = cutSizeLine->text().toDouble();
        int count = sheetActive->doubleStrip == true ? 2 : 1;
        sheetActive->stripPW.clear();
        for(int row=0; row<count; row++){
            qreal position = stripConfigTable->item(row, 0)->text().toDouble();
            qreal width = stripConfigTable->item(row, 1)->text().toDouble();
            StripPW strip(position, width);
            sheetActive->stripPW.append(strip);
        }
    } else{
        sheetActive->layers = layerLine->text().toInt();
    }
}

void SheetDialog::updateSheetListTable(QList<Sheet *> sheetList)
{
    sheetListTable->clearContents();
    sheetListTable->setRowCount(0);
    // 将该列表显示在表格中
    for(int row=0; row<sheetList.length(); row++){
        // 添加新的一行
        sheetListTable->insertRow(row);
        sheetListTable->setItem(row,0,new QTableWidgetItem(QString("%1").arg(sheetList[row]->type)));
        sheetListTable->setItem(row,1,new QTableWidgetItem(sheetList[row]->name));
        sheetListTable->setItem(row,2,new QTableWidgetItem(sheetList[row]->material));
        sheetListTable->setItem(row,3,new QTableWidgetItem(
                                    QString("%1 X %2")
                                    .arg(sheetList[row]->width)
                                    .arg(sheetList[row]->height)));
        sheetListTable->setItem(row,4,new QTableWidgetItem(QString("%1").arg(sheetList[row]->componentGap)));
        if(sheetList[row]->type == Sheet::Strip){
            sheetListTable->setItem(row,5,new QTableWidgetItem(QString("%1").arg(sheetList[row]->stripPW[0].width)));
        }
    }
    sheetListTable->show();
}

void SheetDialog::updateStripConfigTable(QList<StripPW> stripPW)
{
    stripConfigTable->clearContents();
    stripConfigTable->setRowCount(0);
    for(int row=0; row<stripPW.length(); row++){
        // 添加新的一行
        stripConfigTable->insertRow(row);
        stripConfigTable->setItem(row,0,new QTableWidgetItem(QString("%1").arg(stripPW[row].position)));
        stripConfigTable->setItem(row,1,new QTableWidgetItem(QString("%1").arg(stripPW[row].width)));
    }
    stripConfigTable->show();
}

void SheetDialog::setSheetInfoDisable(bool flag)
{
    typeCBox->setDisabled(flag);
    nameLine->setDisabled(flag);
    materialLine->setDisabled(flag);
    xLine->setDisabled(flag);
    yLine->setDisabled(flag);
    distanceLine->setDisabled(flag);
    leftLine->setDisabled(flag);
    topLine->setDisabled(flag);
    rightLine->setDisabled(flag);
    bottomLine->setDisabled(flag);
    marginLine->setDisabled(flag);
    layerLine->setDisabled(flag);

    doubleStrip->setDisabled(flag);
    cutSizeLine->setDisabled(flag);

    stripConfigTable->setDisabled(flag);
}

QList<Sheet *> SheetDialog::xmlFileReader(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("警告"), tr("材料信息文件无法正常打开!"));
        return QList<Sheet*>();
    }

    // 读取xml文件并解析
    QList<Sheet *> retList;
    QXmlStreamReader r(&file);
    while(!r.atEnd() && !r.hasError()){
        r.readNext();
        if(r.isStartElement()){
            if(r.name() == "Sheet"){
                Sheet* sheet = new Sheet;
                bool breakFlag = false;
                while(!r.atEnd() && !r.hasError()){
                    r.readNext();
                    if(r.name() == "name"){
                        sheet->name = r.readElementText();
                        //qDebug() << "this name: " << sheet->name;
                    }
                    if(r.name() == "type"){
                        sheet->type = (Sheet::SheetType)r.readElementText().toInt();
                        //qDebug() << "type: " << sheet->type;
                    }
                    if(r.name() == "material"){
                        sheet->material = r.readElementText();
                        //qDebug() << "material: " << sheet->material;
                    }
                    if(r.name() == "width"){
                        sheet->width = r.readElementText().toDouble();
                        //qDebug() << "width: " << sheet->width;
                    }
                    if(r.name() == "height"){
                        sheet->height = r.readElementText().toDouble();
                        //qDebug() << "height: " << sheet->height;
                    }
                    if(r.name() == "componentGap"){
                        sheet->componentGap = r.readElementText().toDouble();
                        //qDebug() << "componentGap: " << sheet->componentGap;
                    }
                    if(r.isStartElement()){
                        if(r.name() == "margins"){
                            while(!r.atEnd() && !r.hasError()){
                                r.readNext();
                                if(r.name() == "top"){
                                    sheet->topMargin = r.readElementText().toDouble();
                                    //qDebug() << "topMargin: " << sheet->topMargin;
                                }
                                if(r.name() == "right"){
                                    sheet->rightMargin = r.readElementText().toDouble();
                                    //qDebug() << "rightMargin: " << sheet->rightMargin;
                                }
                                if(r.name() == "bottom"){
                                    sheet->bottomMargin = r.readElementText().toDouble();
                                    //qDebug() << "bottomMargin: " << sheet->bottomMargin;
                                }
                                if(r.name() == "left"){
                                    sheet->leftMargin = r.readElementText().toDouble();
                                    //qDebug() << "leftMargin: " << sheet->leftMargin;
                                }
                                if(sheet->type == Sheet::Strip){
                                    if(r.name() == "margin"){
                                        sheet->margin = r.readElementText().toDouble();
                                        //qDebug() << "margin: " << sheet->margin;
                                    }
                                    if(r.name() == "doubleStrip"){
                                        sheet->doubleStrip = r.readElementText().toInt();
                                        //qDebug() << "doubleStrip: " << sheet->doubleStrip;
                                    }
                                    if(r.name() == "cutPaneSize"){
                                        sheet->cutPaneSize = r.readElementText().toDouble();
                                        //qDebug() << "cutPaneSize: " << sheet->cutPaneSize;
                                    }
                                    if(r.isStartElement()){
                                        if(r.name() == "stripPW"){
                                            //qDebug() << "stripPW";
                                            sheet->stripPW.clear();
                                            StripPW spw;
                                            bool flag = false;
                                            int count = sheet->doubleStrip == false ? 1 : 2;
                                            while(!r.atEnd() && !r.hasError()){
                                                r.readNext();
                                                if(r.name() == "position"){
                                                    spw.position = r.readElementText().toDouble();
                                                    //qDebug() << "position: " << spw.position;
                                                }
                                                if(r.name() == "width"){
                                                    spw.width = r.readElementText().toDouble();
                                                    //qDebug() << "width: " << spw.width;
                                                    flag = true;
                                                }
                                                if(flag){
                                                    sheet->stripPW.append(spw);
                                                    flag = false;
                                                    count--;
                                                }
                                                if(count == 0){
                                                    breakFlag = true;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                } else{
                                    if(r.name() == "layers"){
                                        sheet->layers = r.readElementText().toInt();
                                        //qDebug() << "layers: " << sheet->layers;
                                        breakFlag = true;  // 此处需要跳出循环
                                    }
                                }
                                if(breakFlag){
                                    break;
                                }
                            }
                        }
                    }

                    if(breakFlag){
                        retList.append(sheet);
                        break;
                    }
                }
            }
        }
    }
    file.close();
    return retList;
}

void SheetDialog::xmlFileWrite(QString fileName, QList<Sheet *> list)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("警告"), tr("材料信息文件无法正常打开!"));
        return;
    }

    // 将sheet信息写入xml文件
    QXmlStreamWriter w(&file);
    w.setAutoFormattingIndent(8);
    w.setAutoFormatting(true);
    w.writeStartDocument();
    w.writeStartElement("SHEETINFO");
    for(int i=0; i<list.length();i++){
        w.writeStartElement("Sheet");
        w.writeTextElement("name", QString("%1").arg(list[i]->name));
        w.writeTextElement("type", QString("%1").arg(list[i]->type));
        w.writeTextElement("material", QString("%1").arg(list[i]->material));
        w.writeTextElement("width", QString("%1").arg(list[i]->width));
        w.writeTextElement("height", QString("%1").arg(list[i]->height));
        w.writeTextElement("componentGap", QString("%1").arg(list[i]->componentGap));

        // 封装margins部分
        w.writeStartElement("margins");
        w.writeTextElement("top", QString("%1").arg(list[i]->topMargin));
        w.writeTextElement("right", QString("%1").arg(list[i]->rightMargin));
        w.writeTextElement("bottom", QString("%1").arg(list[i]->bottomMargin));
        w.writeTextElement("left", QString("%1").arg(list[i]->leftMargin));
        if(list[i]->type == Sheet::Strip){
            w.writeTextElement("margin", QString("%1").arg(list[i]->margin));
            w.writeTextElement("doubleStrip", QString("%1").arg(list[i]->doubleStrip));
            w.writeTextElement("cutPaneSize", QString("%1").arg(list[i]->cutPaneSize));
            w.writeStartElement("stripPW");
            for(int j=0; j<list[i]->stripPW.length(); j++){
                w.writeTextElement("position", QString("%1").arg(list[i]->stripPW[j].position));
                w.writeTextElement("width", QString("%1").arg(list[i]->stripPW[j].width));
            }
            w.writeEndElement();
        } else{
            w.writeTextElement("layers", QString("%1").arg(list[i]->layers));
        }
        w.writeEndElement();
        w.writeEndElement();
    }
    w.writeEndElement();

    w.writeEndDocument();

    file.close();
}

Sheet *SheetDialog::getSheetActive()
{
    return sheetActive;
}

bool SheetDialog::sheetNameConflict(const int index)
{
    // 检查名称是否冲突
    for(int i=0;i<sheetList.length();i++){
        if(i == index){
            continue;
        }
        if(sheetList[i]->name == nameLine->text()){
            return true;
        }
    }
    return false;
}

void SheetDialog::closeEvent(QCloseEvent *)
{
    if(insertFlag || editFlag){
        QMessageBox::StandardButton box;
        box = QMessageBox::warning(this,tr("确认"),tr("是否保存修改？"),
        QMessageBox::Yes|QMessageBox::No);
        if(box == QMessageBox::Yes) {
            if(sheetNameConflict(currentIndex)){
                QMessageBox::warning(this, tr("警告"), tr("材料列表中该名称已存在，无法保存!"));
                return;
            }
            saveSheetInfo();
        }
    }
    if(insertCounter != 0 || editCounter != 0 || removeCounter != 0){
        saveSheetInfo();
    }
    if(role == Nest && !sheetActive){
        QMessageBox::warning(this, tr("警告"), tr("请选择一种材料用于排版！"));
        return;
    }
}

void SheetDialog::onDoubleStripChanged(bool checkd)
{
    if(checkd){
        stripConfigTable->setRowCount(2);
    } else{
        stripConfigTable->setRowCount(1);
    }
}

void SheetDialog::onSheetTypeComChanged(int index)
{
    if(index == 1){
        marginLine->show();
        doubleStrip->show();
        cutSizeLabel->show();
        cutSizeLine->show();
        marginLabel->show();
        layerLabel->hide();
        layerLine->hide();
        updateStripConfigTable(sheetActive->stripPW);
        stripConfigTable->show();
    } else{
        layerLabel->show();
        layerLine->show();
        marginLabel->hide();
        marginLine->hide();
        doubleStrip->hide();
        cutSizeLabel->hide();
        cutSizeLine->hide();
    }
    if(index == 2){
        yLine->setText(tr("00"));
        yLine->setDisabled(true);
    } else{
        yLine->setText(tr("1000"));
        yLine->setDisabled(false);
    }

}

void SheetDialog::onDialogButtonClicked(QAbstractButton *button)
{
    if(button->text() == "Apply"){
        if(sheetNameConflict(currentIndex)){
            QMessageBox::warning(this, tr("警告"), tr("材料列表中该名称已存在!"));
            nameLine->setFocus();
            return;
        }
        // 重新赋值
        updateSheetActive(sheetActive);
        updateSheetListTable(sheetList);
        sheetInsertBtn->setDisabled(false);
        sheetRemoveBtn->setDisabled(false);
        sheetEditBtn->setDisabled(false);
        setSheetInfoDisable(true);
        buttonBox->button(QDialogButtonBox::Apply)->setDisabled(true);
        buttonBox->button(QDialogButtonBox::Cancel)->setDisabled(true);
        buttonBox->button(QDialogButtonBox::Ok)->setDisabled(false);
        if(insertFlag) {
            insertCounter++;
            insertFlag = false;
        }
        if(editFlag){
            editCounter++;
            editFlag = false;
        }
    }
    if(button->text() == "Cancel"){
        if(insertFlag){
            QMessageBox::StandardButton box;
            box = QMessageBox::warning(this,tr("确认"),tr("是否保存添加的材料？"),
            QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
            if(box == QMessageBox::Yes) {
                if(sheetNameConflict(currentIndex)){
                    QMessageBox::warning(this, tr("警告"), tr("材料列表中该名称已存在!"));
                    nameLine->setFocus();
                    return;
                }
                updateSheetActive(sheetActive);
                updateSheetListTable(sheetList);
                insertCounter++;
            } else if(box == QMessageBox::No){
                sheetActive = sheetList.last();
                onSheetRemove();
                return;
            } else if(box == QMessageBox::Cancel){
                return;
            }
            insertFlag = false;
        }
        if(editFlag){
            QMessageBox::StandardButton box;
            box = QMessageBox::warning(this,tr("确认"),tr("是否保存编辑的材料？"),
            QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
            if(box == QMessageBox::Yes) {
                if(sheetNameConflict(currentIndex)){
                    QMessageBox::warning(this, tr("警告"), tr("材料列表中该名称已存在!"));
                    nameLine->setFocus();
                    return;
                }
                updateSheetActive(sheetActive);
                editCounter++;
            } else if(box == QMessageBox::No){
                updateSheetInfo(sheetActive);
            } else if(box == QMessageBox::Cancel){
                return;
            }
            editFlag = false;
        }
        sheetInsertBtn->setDisabled(false);
        sheetRemoveBtn->setDisabled(false);
        sheetEditBtn->setDisabled(false);
        setSheetInfoDisable(true);
        buttonBox->button(QDialogButtonBox::Apply)->setDisabled(true);
        buttonBox->button(QDialogButtonBox::Cancel)->setDisabled(true);
        buttonBox->button(QDialogButtonBox::Ok)->setDisabled(false);
    }
    if(button->text() == "OK"){
        if(insertCounter != 0 || editCounter != 0 || removeCounter != 0){
            saveSheetInfo();
        }
        if(role == Nest && !sheetActive){
            QMessageBox::warning(this, tr("警告"), tr("请选择一种材料用于排版！"));
            return;
        }
        QDialog::accept();
    }
}

void SheetDialog::onSheetSelected(const QModelIndex &index)
{
    if(insertFlag){
        return;
    }
    currentIndex = index.row();
    if(sheetList.length() < currentIndex+1){
        buttonBox->button(QDialogButtonBox::Apply)->setDisabled(true);
        setSheetInfoDisable(true);
        sheetActive = NULL;
        sheetRemoveBtn->hide();
        sheetEditBtn->hide();
        return;
    }

    // 选择时，使能并显示删除、编辑按钮
    sheetRemoveBtn->setDisabled(false);
    sheetEditBtn->setDisabled(false);
    sheetRemoveBtn->show();
    sheetEditBtn->show();
    sheetActive = sheetList[currentIndex];
    updateSheetInfo(sheetActive);
    setSheetInfoDisable(true);
}

void SheetDialog::onSheetInsert()
{
    qDebug() << "添加";
    sheetActive = new Sheet;
    sheetActive->name = "新增材料";
    updateSheetInfo(sheetActive);
    sheetList.append(sheetActive);
    updateSheetListTable(sheetList);
    sheetListTable->selectRow(sheetList.length()-1);
    currentIndex = sheetList.length() - 1;
    // 增加标志
    insertFlag = true;
    sheetRemoveBtn->setDisabled(true);
    sheetEditBtn->setDisabled(true);
    setSheetInfoDisable(false);
    buttonBox->button(QDialogButtonBox::Apply)->setDisabled(false);
    buttonBox->button(QDialogButtonBox::Cancel)->setDisabled(false);
    buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);
    updateSheetInfo(sheetActive);
}

void SheetDialog::onSheetRemove()
{
    qDebug() << "删除";
    if(!sheetActive){
        QMessageBox::warning(this, tr("警告"), tr("没有材料被选中!"));
        return;
    }
    sheetList.removeOne(sheetActive);
    updateSheetListTable(sheetList);
    removeCounter++;

    // 自动更新至最后一条
    if(sheetList.length() > 1){
        sheetActive = sheetList.last();
        currentIndex = sheetList.length() - 1;
        sheetListTable->selectRow(currentIndex);
        updateSheetInfo(sheetActive);
    }
}

void SheetDialog::onSheetEdit()
{
    qDebug() << "编辑";
    if(!sheetActive){
        QMessageBox::warning(this, tr("警告"), tr("没有材料被选中!"));
        return;
    }
    editFlag = true;
    sheetInsertBtn->setDisabled(true);
    sheetRemoveBtn->setDisabled(true);
    setSheetInfoDisable(false);
    buttonBox->button(QDialogButtonBox::Apply)->setDisabled(false);
    buttonBox->button(QDialogButtonBox::Cancel)->setDisabled(false);
    buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);
    updateSheetInfo(sheetActive);
}

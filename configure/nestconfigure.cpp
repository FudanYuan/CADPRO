#include "nestconfigure.h"
#include <QList>
#include <QFileInfo>
#include <debug.h>
#include <common.h>

#include <QDebug>

NestConfigure::NestConfigure(QObject *parent) :
    QObject(parent),
    allowEntityOutOfSheet(false),
    backgroundColor(Qt::gray),
    showPiecesDistance(false),
    language(Chinese),
    workPathStr("")
{
    settings = new QSettings(NEST_CONFG_FILE_PATH, QSettings::IniFormat);
    readConfig(settings);
}

NestConfigure::~NestConfigure()
{
#ifdef DEBUG
    //qDebug() << "NestConfigure has been deleted!";
#endif
    delete settings;
}

void NestConfigure::writeConfig(QSettings *settings)
{
    writeConfigGrid(settings);
    writeConfigSheetStyle(settings);
    writeConfigPieceStyle(settings);
    writeConfigOthers(settings);
}

void NestConfigure::readConfig(QSettings *settings)
{
    QFileInfo file(NEST_CONFG_FILE_PATH);
    // 若配置文件不存在，初始化系统配置
    if(!file.exists()){
#ifdef DEBUG
        qDebug() << "配置文件不存在";
#endif
        writeConfig(settings);
        return;
    }
    // 若配置文件存在，读入配置
#ifdef DEBUG
    //qDebug() << "配置文件已存在";
#endif
    readConfigGrid(settings);
    readConfigSheetStyle(settings);
    readConfigPieceStyle(settings);
    readConfigOthers(settings);
}

void NestConfigure::writeConfigGrid(QSettings *settings)
{
    // 写入主网格配置
    settings->beginGroup("mainGrid");
    settings->setValue("mainGrid_showGrid", QVariant(mainGrid.showGrid));
    settings->setValue("mainGrid_gridType", QVariant(mainGrid.gridType));
    settings->setValue("mainGrid_gridColor", QVariant(mainGrid.gridColor.rgba()));
    settings->setValue("mainGrid_xStep", QVariant(mainGrid.xStep));
    settings->setValue("mainGrid_yStep", QVariant(mainGrid.yStep));
    settings->endGroup();

    // 写入副网格配置
    settings->beginGroup("secondGrid");
    settings->setValue("secondGrid_showGrid", QVariant(secondGrid.showGrid));
    settings->setValue("secondGrid_gridType", QVariant(secondGrid.gridType));
    settings->setValue("secondGrid_gridColor", QVariant(secondGrid.gridColor.rgba()));
    settings->setValue("secondGrid_xStep", QVariant(secondGrid.xStep));
    settings->setValue("secondGrid_yStep", QVariant(secondGrid.yStep));
    settings->endGroup();
}

void NestConfigure::readConfigGrid(QSettings *settings)
{
    // 读入主网格配置
    settings->beginGroup("mainGrid");
    mainGrid.showGrid = settings->value("mainGrid_showGrid").toBool();
    mainGrid.gridType = (GridType)settings->value("mainGrid_gridType").toInt();
    mainGrid.gridColor = intToColor(settings->value("mainGrid_gridColor").toInt());
    mainGrid.xStep = settings->value("mainGrid_xStep").toInt();
    mainGrid.yStep = settings->value("mainGrid_yStep").toInt();
    settings->endGroup();

    // 读入副网格配置
    settings->beginGroup("secondGrid");
    secondGrid.showGrid = settings->value("secondGrid_showGrid").toBool();
    secondGrid.gridType = (GridType)settings->value("secondGrid_gridType").toInt();
    secondGrid.gridColor = intToColor(settings->value("secondGrid_gridColor").toInt());
    secondGrid.xStep = settings->value("secondGrid_xStep").toInt();
    secondGrid.yStep = settings->value("secondGrid_yStep").toInt();
    settings->endGroup();
}

void NestConfigure::writeConfigSheetStyle(QSettings *settings)
{
    // 写入材料配置
    settings->beginGroup("sheetStyle");
    settings->setValue("sheetStyle_sheetColor", QVariant(sheetStyle.sheetColor.rgba()));
    settings->setValue("sheetStyle_sheetMarginColor", QVariant(sheetStyle.sheetMarginColor.rgba()));
    settings->setValue("sheetStyle_reinforcementColor", QVariant(sheetStyle.reinforcementColor.rgba()));
    settings->setValue("sheetStyle_reinforcementMarginColor", QVariant(sheetStyle.reinforcementMarginColor.rgba()));
    settings->setValue("sheetStyle_sheetFilled", QVariant(sheetStyle.sheetFilled));
    settings->endGroup();
}

void NestConfigure::readConfigSheetStyle(QSettings *settings)
{
    // 读入材料配置
    settings->beginGroup("sheetStyle");
    sheetStyle.sheetColor = intToColor(settings->value("sheetStyle_sheetColor").toInt());
    sheetStyle.sheetMarginColor = intToColor(settings->value("sheetStyle_sheetMarginColor").toInt());
    sheetStyle.reinforcementColor = intToColor(settings->value("sheetStyle_reinforcementColor").toInt());
    sheetStyle.reinforcementMarginColor = intToColor(settings->value("sheetStyle_reinforcementMarginColor").toInt());
    sheetStyle.sheetFilled = settings->value("sheetStyle_sheetFilled").toBool();
    settings->endGroup();
}

void NestConfigure::writeConfigPieceStyle(QSettings *settings)
{
    // 写入切割件配置
    settings->beginGroup("pieceStyle");
    settings->setValue("pieceStyle_outsideColor", QVariant(pieceStyle.outsideColor.rgba()));
    settings->setValue("pieceStyle_insideColor", QVariant(pieceStyle.insideColor.rgba()));
    settings->setValue("pieceStyle_liningColor", QVariant(pieceStyle.liningColor.rgba()));
    settings->setValue("pieceStyle_reinforcementColor", QVariant(pieceStyle.reinforcementColor.rgba()));
    settings->setValue("pieceStyle_otherColor", QVariant(pieceStyle.otherColor.rgba()));
    settings->endGroup();
}

void NestConfigure::readConfigPieceStyle(QSettings *settings)
{
    // 读入切割件配置
    settings->beginGroup("pieceStyle");
    pieceStyle.outsideColor = intToColor(settings->value("pieceStyle_outsideColor").toInt());
    pieceStyle.insideColor = intToColor(settings->value("pieceStyle_insideColor").toInt());
    pieceStyle.liningColor = intToColor(settings->value("pieceStyle_liningColor").toInt());
    pieceStyle.reinforcementColor = intToColor(settings->value("pieceStyle_reinforcementColor").toInt());
    pieceStyle.otherColor = intToColor(settings->value("pieceStyle_otherColor").toInt());
    settings->endGroup();
}

void NestConfigure::writeConfigOthers(QSettings *settings)
{
    // 写入其他配置
    settings->beginGroup("other");
    settings->setValue("other_allowEntityOutOfSheet", QVariant(allowEntityOutOfSheet));
    settings->setValue("other_backgroundColor", QVariant(backgroundColor.rgba()));
    settings->setValue("other_showPiecesDistance", QVariant(showPiecesDistance));
    settings->setValue("other_language", QVariant(language));
    settings->setValue("other_workPathStr", QVariant(workPathStr));
    settings->endGroup();
}

void NestConfigure::readConfigOthers(QSettings *settings)
{
    // 读入其他配置
    settings->beginGroup("other");
    allowEntityOutOfSheet = settings->value("other_allowEntityOutOfSheet").toBool();
    backgroundColor = intToColor(settings->value("other_backgroundColor").toInt());
    showPiecesDistance = settings->value("other_allowEntityOutOfSheet").toBool();
    language = (Language)settings->value("other_language").toInt();
    workPathStr = settings->value("other_workPathStr").toString();
    settings->endGroup();
}

void NestConfigure::updateConfig(QList<NestConfigure::KeyValue> keyValue)
{
    QSettings *settings = new QSettings(NEST_CONFG_FILE_PATH, QSettings::IniFormat);
    for(int i=0; i<keyValue.length();i++){
        settings->setValue(keyValue.at(i).key, keyValue.at(i).value);
    }
    delete settings;
    settings = NULL;
}

void NestConfigure::onConfigChanged(QString name, QVariant value)
{
    if(settings->value(name) == value){
        return;
    }
#ifdef DEBUG
    qDebug() << "配置改变" << name << "   " << value;
#endif
    settings->setValue(name, value);
}

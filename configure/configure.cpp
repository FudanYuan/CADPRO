#include "configure.h"
#include <QList>
#include <QFileInfo>
#include <QDebug>
#include <debug.h>

Configure::Configure(QObject *parent) :
    QObject(parent)
{
    settings = new QSettings(CONFG_FILE_PATH, QSettings::IniFormat);
    readConfig(settings);
}

Configure::~Configure()
{
#ifdef DEBUG
    qDebug() << "configure has been deleted!";
#endif
    delete settings;
}

void Configure::writeConfig(QSettings *settings)
{
    writeConfigEntityStyle(settings);
    writeConfigAxesGrid(settings);
    writeConfigOffset(settings);
}

void Configure::readConfig(QSettings *settings)
{
    QFileInfo file(CONFG_FILE_PATH);
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
    qDebug() << "配置文件已存在";
#endif
    readConfigEntityStyle(settings);
    readConfigAxesGrid(settings);
    readConfigView(settings);
}

void Configure::writeConfigEntityStyle(QSettings *settings)
{
    settings->beginGroup("eStyle");
    settings->setValue("eStyle_permeterLine_color", QVariant(eStyle.perimeterLine.color.rgba()));
#ifdef DEBUG
    qDebug() << "eStyle.perimeterLine.color.rgba():" << eStyle.perimeterLine.color.rgba();
    qDebug() << "QVariant(eStyle.perimeterLine.color.rgba()):" << QVariant(eStyle.perimeterLine.color.rgba());
#endif
    settings->setValue("eStyle_permeterLine_style", QVariant((int)eStyle.perimeterLine.style));
    settings->setValue("eStyle_permeterLine_width", QVariant(eStyle.perimeterLine.width));
    settings->setValue("eStyle_mark_color", QVariant(eStyle.mark.color.rgba()));
    settings->setValue("eStyle_mark_style", QVariant((int)eStyle.mark.style));
    settings->setValue("eStyle_mark_width", QVariant(eStyle.mark.width));
    settings->setValue("eStyle_cut_color", QVariant(eStyle.cut.color.rgba()));
#ifdef DEBUG
    qDebug() << "eStyle.cut.color.rgba():" << eStyle.cut.color.rgba();
    qDebug() << "QVariant(eStyle.cut.color.rgba()):" << QVariant(eStyle.cut.color.rgba());
#endif
    settings->setValue("eStyle_cut_style", QVariant((int)eStyle.cut.style));
    settings->setValue("eStyle_cut_width", QVariant(eStyle.cut.width));
    settings->setValue("eStyle_stitch_color", QVariant(eStyle.stitch.color.rgba()));
    settings->setValue("eStyle_stitch_style", QVariant((int)eStyle.stitch.style));
    settings->setValue("eStyle_stitch_width", QVariant(eStyle.stitch.width));
    settings->setValue("eStyle_generic_color", QVariant(eStyle.generic.color.rgba()));
    settings->setValue("eStyle_generic_style", QVariant((int)eStyle.generic.style));
    settings->setValue("eStyle_generic_width", QVariant(eStyle.generic.width));
    settings->setValue("eStyle_middleAxis_color", QVariant(eStyle.middleAxis.color.rgba()));
    settings->setValue("eStyle_middleAxis_width", QVariant(eStyle.middleAxis.width));
    settings->setValue("eStyle_middleAxis_style", QVariant((int)eStyle.middleAxis.style));
    settings->setValue("eStyle_notch_color", QVariant(eStyle.notchColor.rgba()));
    settings->setValue("eStyle_objSize_knotSizeInPix", QVariant(eStyle.objSize.knotSizeInPix));
    settings->setValue("eStyle_objSize_snapDistanceInPix", QVariant(eStyle.objSize.snapDistanceInPix));
    settings->setValue("eStyle_referPoint_color", QVariant(eStyle.referPoint.color.rgba()));
    settings->setValue("eStyle_referPoint_sizeInPix", QVariant(eStyle.referPoint.sizeInPix));
    settings->setValue("eStyle_referPoint_textHeightInPix", QVariant(eStyle.referPoint.textHeightInPix));
    settings->setValue("eStyle_selectedEntity_color", QVariant(eStyle.selectedEntity.color.rgba()));
    settings->setValue("eStyle_selectedEntity_style", QVariant((int)eStyle.selectedEntity.style));
    settings->setValue("eStyle_selectedEntity_width", QVariant(eStyle.selectedEntity.width));
    settings->setValue("eStyle_backgroundColor", QVariant(eStyle.backgroundColor.rgba()));
    settings->setValue("eStyle_selectionBox", QVariant(eStyle.selectionBox.rgba()));
    settings->setValue("eStyle_entityUnderCursor_color", QVariant(eStyle.entityUnderCursor.color.rgba()));
    settings->setValue("eStyle_entityUnderCursor_style", QVariant((int)eStyle.entityUnderCursor.style));
    settings->setValue("eStyle_entityUnderCursor_width", QVariant(eStyle.entityUnderCursor.width));
    settings->setValue("eStyle_hiddenEntity", QVariant(eStyle.hiddenEntity.rgba()));

    settings->setValue("eStyle_interLineStyle", QVariant((int)eStyle.interLineStyle));
    settings->setValue("eStyle_lineTypes", QVariant(eStyle.lineTypes));

    settings->setValue("eStyle_projectHistory", QVariant(eStyle.projectHistory));
    settings->setValue("eStyle_basicSize", QVariant(eStyle.basicSize));
    settings->endGroup();
}

void Configure::readConfigEntityStyle(QSettings *settings)
{
    settings->beginGroup("eStyle");
    eStyle.perimeterLine.color = intToColor(settings->value("eStyle_permeterLine_color").toInt());
    eStyle.perimeterLine.style = (Qt::PenStyle)settings->value("eStyle_permeterLine_style").toInt();
    eStyle.perimeterLine.width = settings->value("eStyle_permeterLine_width").toInt();

    eStyle.mark.color = intToColor(settings->value("eStyle_mark_color").toInt());
    eStyle.mark.style = (Qt::PenStyle)settings->value("eStyle_mark_style").toInt();
    eStyle.mark.width = settings->value("eStyle_mark_width").toInt();

    eStyle.cut.color = intToColor(settings->value("eStyle_cut_color").toInt());
    eStyle.cut.style = (Qt::PenStyle)settings->value("eStyle_cut_style").toInt();
    eStyle.cut.width = settings->value("eStyle_cut_width").toInt();

    eStyle.stitch.color = intToColor(settings->value("eStyle_stitch_color").toInt());
    eStyle.stitch.style = (Qt::PenStyle)settings->value("eStyle_stitch_style").toInt();
    eStyle.stitch.width = settings->value("eStyle_stitch_width").toInt();

    eStyle.generic.color = intToColor(settings->value("eStyle_generic_color").toInt());
    eStyle.generic.style = (Qt::PenStyle)settings->value("eStyle_generic_style").toInt();
    eStyle.generic.width = settings->value("eStyle_generic_width").toInt();

    eStyle.middleAxis.color = intToColor(settings->value("eStyle_middleAxis_color").toInt());
    eStyle.middleAxis.style = (Qt::PenStyle)settings->value("eStyle_middleAxis_style").toInt();
    eStyle.middleAxis.width = settings->value("eStyle_middleAxis_width").toInt();

    eStyle.notchColor = intToColor(settings->value("eStyle_notch_color").toInt());

    eStyle.backgroundColor = intToColor(settings->value("eStyle_backgroundColor").toInt());

    eStyle.selectedEntity.color = intToColor(settings->value("eStyle_selectedEntity_color").toInt());
    eStyle.selectedEntity.style = (Qt::PenStyle)settings->value("eStyle_selectedEntity_style").toInt();
    eStyle.selectedEntity.width = settings->value("eStyle_selectedEntity_width").toInt();

    eStyle.selectionBox = intToColor(settings->value("eStyle_selectionBox").toInt());

    eStyle.entityUnderCursor.color = intToColor(settings->value("eStyle_entityUnderCursor_color").toInt());
    eStyle.entityUnderCursor.style = (Qt::PenStyle)settings->value("eStyle_entityUnderCursor_style").toInt();
    eStyle.entityUnderCursor.width = settings->value("eStyle_entityUnderCursor_width").toInt();

    eStyle.hiddenEntity = intToColor(settings->value("eStyle_hiddenEntity").toInt());

    eStyle.objSize.knotSizeInPix = settings->value("eStyle_objSize_knotSizeInPix").toInt();
    eStyle.objSize.snapDistanceInPix = settings->value("eStyle_objSize_snapDistanceInPix").toInt();

    eStyle.referPoint.color = intToColor(settings->value("eStyle_referPoint_color").toInt());
    eStyle.referPoint.sizeInPix = settings->value("eStyle_referPoint_sizeInPix").toInt();
    eStyle.referPoint.textHeightInPix = settings->value("eStyle_referPoint_textHeightInPix").toInt();

    eStyle.interLineStyle = (InterLineStyle)settings->value("eStyle_interLineStyle").toInt();
    eStyle.lineTypes = settings->value("eStyle_lineTypes").toBool();

    eStyle.projectHistory = settings->value("eStyle_projectHistory").toInt();
    eStyle.basicSize = settings->value("eStyle_basicSize").toInt();

    settings->endGroup();
}

void Configure::writeConfigAxesGrid(QSettings *settings)
{
    settings->beginGroup("axesGrid");
    settings->setValue("axesGrid_showAxes", QVariant(axesGrid.axes.showAxes));
    settings->setValue("axesGrid_axesType", QVariant(axesGrid.axes.axesType));
    settings->setValue("axesGrid_xAxisColor", QVariant(axesGrid.axes.xAxisColor.rgba()));
    settings->setValue("axesGrid_yAxisColor", QVariant(axesGrid.axes.yAxisColor.rgba()));
    settings->setValue("axesGrid_arrowSizeInPix", QVariant(axesGrid.axes.arrowSizeInPix));
    settings->setValue("axesGrid_axisSizeInPix", QVariant(axesGrid.axes.axisSizeInPix));

    settings->setValue("axesGrid_showGrid", QVariant(axesGrid.grid.showGrid));
    settings->setValue("axesGrid_gridType", QVariant(axesGrid.grid.gridType));
    settings->setValue("axesGrid_gridColor", QVariant(axesGrid.grid.gridColor.rgba()));
    settings->setValue("axesGrid_xStep", QVariant(axesGrid.grid.xStep));
    settings->setValue("axesGrid_yStep", QVariant(axesGrid.grid.yStep));

    settings->setValue("axesGrid_antiAliaising", QVariant(axesGrid.antiAliaising));
    settings->endGroup();
}

void Configure::readConfigAxesGrid(QSettings *settings)
{
    settings->beginGroup("axesGrid");
    axesGrid.axes.showAxes = settings->value("axesGrid_showAxes").toBool();
    axesGrid.axes.axesType = (AxesType)settings->value("axesGrid_axesType").toInt();
    axesGrid.axes.xAxisColor = intToColor(settings->value("axesGrid_xAxisColor").toInt());
    axesGrid.axes.yAxisColor = intToColor(settings->value("axesGrid_yAxisColor").toInt());
    axesGrid.axes.arrowSizeInPix = settings->value("axesGrid_arrowSizeInPix").toInt();
    axesGrid.axes.axisSizeInPix = settings->value("axesGrid_axisSizeInPix").toInt();

    axesGrid.grid.showGrid = settings->value("axesGrid_showGrid").toBool();
    axesGrid.grid.gridType = (GridType)settings->value("axesGrid_gridType").toInt();
    axesGrid.grid.gridColor = intToColor(settings->value("axesGrid_gridColor").toInt());
    axesGrid.grid.xStep = settings->value("axesGrid_xStep").toInt();
    axesGrid.grid.yStep = settings->value("axesGrid_yStep").toInt();

    axesGrid.antiAliaising = settings->value("axesGrid_antiAliaising").toBool();
    settings->endGroup();
}

void Configure::writeConfigOffset(QSettings *settings)
{
    for(int i=0;i<offset.length();i++){
        settings->beginGroup(tr("offset") + QString::number(i));
        settings->setValue("offset_type", QVariant(offset.at(i).type));
        settings->setValue("offset_distance", QVariant(offset.at(i).distance));
        settings->setValue("offset_color", QVariant(offset.at(i).color.rgba()));

        settings->endGroup();
    }
}

void Configure::readConfigOffset(QSettings *settings)
{
    for(int i=0;i<offset.length();i++){
        settings->beginGroup(tr("offset") + QString::number(i));
        offset[i].type = settings->value("offset_type").toString();
        offset[i].distance = settings->value("offset_distance").toReal();
        offset[i].color = intToColor(settings->value("offset_color").toInt());

        settings->endGroup();
    }
}

void Configure::writeConfigLanguage(QSettings *settings)
{
    settings->setValue("language", QVariant(Chinese));
}

void Configure::readConfigLanguage(QSettings *settings)
{
    language = (Language)settings->value("language").toInt();
}

void Configure::writeConfigView(QSettings *settings)
{
    settings->beginGroup("view");
    settings->setValue("view_grading_rules", QVariant(view.gradingRules));
    settings->setValue("view_filled_patterns", QVariant(view.filledPatterns));
    settings->setValue("view_tool_find_style", QVariant(view.toolFindStyle));
    settings->setValue("view_tool_project", QVariant(view.toolProject));
    settings->setValue("view_tool_properties",QVariant(view.toolProperties));
    settings->endGroup();
}

void Configure::readConfigView(QSettings *settings)
{
    settings->beginGroup("view");
    view.gradingRules = settings->value("view_grading_rules").toBool();
    view.filledPatterns = settings->value("view_filled_patterns").toBool();
    view.toolFindStyle = settings->value("view_tool_find_style").toBool();
    view.toolProject = settings->value("view_tool_project").toBool();
    view.toolProperties = settings->value("view_tool_properties").toBool();
    settings->endGroup();
}

QColor Configure::intToColor(int rgb)
{
    //将Color 从int 转换成 QColor
    int blue = rgb & 255;
    int green = rgb >> 8 & 255;
    int red = rgb >> 16 & 255;
    int alpha = rgb >> 24 & 255;
    return QColor(red, green, blue, alpha);
}

QColor Configure::intToColor(int rgb, bool a)
{
    //将Color 从int 转换成 QColor
    int blue = rgb & 255;
    int green = rgb >> 8 & 255;
    int red = rgb >> 16 & 255;
    int alpha = 255;
    if(a){
        alpha = rgb >> 24 & 255;
    }
    return QColor(red, green, blue, alpha);
}

void Configure::updateConfig(QList<KeyValue> keyValue)
{
    QSettings *settings = new QSettings(CONFG_FILE_PATH, QSettings::IniFormat);
    for(int i=0; i<keyValue.length();i++){
        settings->setValue(keyValue.at(i).key, keyValue.at(i).value);
    }
    delete settings;
}

void Configure::onConfigChanged(QString name, QVariant value)
{
    if(settings->value(name) == value){
        return;
    }
#ifdef DEBUG
    qDebug() << "配置改变" << name << "   " << value;
#endif
    settings->setValue(name, value);
}


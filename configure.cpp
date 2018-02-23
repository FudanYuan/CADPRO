#include "configure.h"
#include <QList>
#include <QFileInfo>
#include <QDebug>

Configure::Configure(QWidget *parent) :
    QWidget(parent)
{
    settings = new QSettings(CONFG_FILE_PATH, QSettings::IniFormat);
    readConfig(settings);
}

Configure::~Configure()
{
    qDebug() << "delete configure";
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
        qDebug() << "配置文件不存在";
        writeConfig(settings);
        return;
    }
    // 若配置文件存在，读入配置
    qDebug() << "配置文件已存在";
    readConfigEntityStyle(settings);
    readConfigAxesGrid(settings);
    readConfigView(settings);
}

void Configure::writeConfigEntityStyle(QSettings *settings)
{
    settings->beginGroup("eStyle");
    settings->setValue("eStyle_permeterLine_color", QVariant(eStyle.perimeterLine.color.rgba()));
    qDebug() << "eStyle.perimeterLine.color.rgba():" << eStyle.perimeterLine.color.rgba();
    qDebug() << "QVariant(eStyle.perimeterLine.color.rgba()):" << QVariant(eStyle.perimeterLine.color.rgba());
    settings->setValue("eStyle_permeterLine_style", QVariant((int)eStyle.perimeterLine.style));
    settings->setValue("eStyle_permeterLine_width", QVariant(eStyle.perimeterLine.width));
    settings->setValue("eStyle_mark_color", QVariant(eStyle.mark.color));
    settings->setValue("eStyle_mark_style", QVariant((int)eStyle.mark.style));
    settings->setValue("eStyle_mark_width", QVariant(eStyle.mark.width));
    settings->setValue("eStyle_cut_width", QVariant(eStyle.cut.width));
    settings->setValue("eStyle_cut_color", QVariant(eStyle.cut.color));
    settings->setValue("eStyle_cut_style", QVariant((int)eStyle.cut.style));
    settings->setValue("eStyle_stitch_style", QVariant((int)eStyle.stitch.style));
    settings->setValue("eStyle_stitch_color", QVariant(eStyle.stitch.color));
    settings->setValue("eStyle_stitch_width", QVariant(eStyle.stitch.width));
    settings->setValue("eStyle_generic_color", QVariant(eStyle.generic.color));
    settings->setValue("eStyle_generic_style", QVariant((int)eStyle.generic.style));
    settings->setValue("eStyle_generic_width", QVariant(eStyle.generic.width));
    settings->setValue("eStyle_middleAxis_color", QVariant(eStyle.middleAxis.color));
    settings->setValue("eStyle_middleAxis_width", QVariant(eStyle.middleAxis.width));
    settings->setValue("eStyle_middleAxis_style", QVariant((int)eStyle.middleAxis.style));
    settings->setValue("eStyle_notch_color", QVariant(eStyle.notchColor));
    settings->setValue("eStyle_objSize_knotSizeInPix", QVariant(eStyle.objSize.knotSizeInPix));
    settings->setValue("eStyle_objSize_snapDistanceInPix", QVariant(eStyle.objSize.snapDistanceInPix));
    settings->setValue("eStyle_referPoint_color", QVariant(eStyle.referPoint.color));
    settings->setValue("eStyle_referPoint_sizeInPix", QVariant(eStyle.referPoint.sizeInPix));
    settings->setValue("eStyle_referPoint_textHeightInPix", QVariant(eStyle.referPoint.textHeightInPix));
    settings->setValue("eStyle_selectedEntity_color", QVariant(eStyle.selectedEntity.color));
    settings->setValue("eStyle_selectedEntity_style", QVariant((int)eStyle.selectedEntity.style));
    settings->setValue("eStyle_selectedEntity_width", QVariant(eStyle.selectedEntity.width));
    settings->setValue("eStyle_backgroundColor", QVariant(eStyle.backgroundColor));
    settings->setValue("eStyle_selectionBox", QVariant(eStyle.selectionBox));
    settings->setValue("eStyle_entityUnderCursor_color", QVariant(eStyle.entityUnderCursor.color));
    settings->setValue("eStyle_entityUnderCursor_style", QVariant((int)eStyle.entityUnderCursor.style));
    settings->setValue("eStyle_entityUnderCursor_width", QVariant(eStyle.entityUnderCursor.width));
    settings->setValue("eStyle_hiddenEntity", QVariant(eStyle.hiddenEntity));
    settings->setValue("eStyle_projectHistory", QVariant(eStyle.projectHistory));
    settings->setValue("eStyle_basicSize", QVariant(eStyle.basicSize));
    settings->setValue("eStyle_lineTypes", QVariant(eStyle.lineTypes));
    settings->endGroup();
}

void Configure::readConfigEntityStyle(QSettings *settings)
{
    settings->beginGroup("eStyle");
    eStyle.perimeterLine.color = intToColor(settings->value("eStyle_permeterLine_color").toInt());
    qDebug() << "read color: " << eStyle.perimeterLine.color.rgba();
    eStyle.perimeterLine.style = (Qt::PenStyle)settings->value("eStyle_permeterLine_style").toInt();
    eStyle.perimeterLine.width = settings->value("eStyle_permeterLine_width").toInt();
    eStyle.mark.color = settings->value("eStyle_mark_color").toInt();
    eStyle.mark.style = (Qt::PenStyle)settings->value("eStyle_mark_style").toInt();
    eStyle.mark.width = settings->value("eStyle_mark_width").toInt();
    eStyle.cut.width = settings->value("eStyle_permeterLine_width").toInt();
    eStyle.cut.color = settings->value("eStyle_permeterLine_color").toInt();
    eStyle.cut.style = (Qt::PenStyle)settings->value("eStyle_permeterLine_style").toInt();
    eStyle.stitch.style = (Qt::PenStyle)settings->value("eStyle_stitch_style").toInt();
    eStyle.stitch.color = settings->value("eStyle_stitch_color").toInt();
    eStyle.stitch.width = settings->value("eStyle_stitch_width").toInt();
    eStyle.generic.color = settings->value("eStyle_generic_color").toInt();
    eStyle.generic.width = settings->value("eStyle_generic_width").toInt();
    eStyle.generic.style = (Qt::PenStyle)settings->value("eStyle_generic_style").toInt();
    eStyle.middleAxis.color = settings->value("eStyle_middleAxis_color").toInt();
    eStyle.middleAxis.width = settings->value("eStyle_middleAxis_width").toInt();
    eStyle.middleAxis.style = (Qt::PenStyle)settings->value("eStyle_middleAxis_style").toInt();
    eStyle.notchColor = settings->value("eStyle_notch_color").toInt();
    eStyle.objSize.knotSizeInPix = settings->value("eStyle_objSize_knotSizeInPix").toInt();
    eStyle.objSize.snapDistanceInPix = settings->value("eStyle_objSize_snapDistanceInPix").toInt();
    eStyle.referPoint.color = settings->value("eStyle_referPoint_color").toInt();
    eStyle.referPoint.sizeInPix = settings->value("eStyle_referPoint_sizeInPix").toInt();
    eStyle.referPoint.textHeightInPix = settings->value("eStyle_referPoint_textHeightInPix").toInt();
    eStyle.selectedEntity.color = settings->value("eStyle_selectedEntity_color").toInt();
    eStyle.selectedEntity.width = settings->value("eStyle_selectedEntity_width").toInt();
    eStyle.selectedEntity.style = (Qt::PenStyle)settings->value("eStyle_selectedEntity_style").toInt();
    eStyle.backgroundColor = settings->value("eStyle_backgroundColor").toInt();
    eStyle.selectionBox = settings->value("eStyle_selectionBox").toInt();
    eStyle.entityUnderCursor.color = settings->value("eStyle_entityUnderCursor_color").toInt();
    eStyle.entityUnderCursor.width = settings->value("eStyle_entityUnderCursor_width").toInt();
    eStyle.entityUnderCursor.style = (Qt::PenStyle)settings->value("eStyle_entityUnderCursor_style").toInt();
    eStyle.hiddenEntity = settings->value("eStyle_hiddenEntity").toInt();
    eStyle.projectHistory = settings->value("eStyle_projectHistory").toInt();
    eStyle.basicSize = settings->value("eStyle_basicSize").toInt();
    eStyle.lineTypes = settings->value("eStyle_lineTypes").toBool();
    settings->endGroup();
}

void Configure::writeConfigAxesGrid(QSettings *settings)
{
    settings->beginGroup("axesGrid");
    settings->setValue("axesGrid_showAxes", QVariant(axesGrid.axes.showAxes));
    settings->setValue("axesGrid_xAxisColor", QVariant(axesGrid.axes.xAxisColor));
    settings->setValue("axesGrid_yAxisColor", QVariant(axesGrid.axes.yAxisColor));
    settings->setValue("axesGrid_arrowSizeInPix", QVariant(axesGrid.axes.arrowSizeInPix));
    settings->setValue("axesGrid_axisSizeInPix", QVariant(axesGrid.axes.axisSizeInPix));
    settings->setValue("axesGrid_showGrid", QVariant(axesGrid.grid.showGrid));
    settings->setValue("axesGrid_gridColor", QVariant(axesGrid.grid.gridColor));
    settings->setValue("axesGrid_xStep", QVariant(axesGrid.grid.xStep));
    settings->setValue("axesGrid_yStep", QVariant(axesGrid.grid.yStep));
    settings->setValue("axesGrid_antiAliaising", QVariant(axesGrid.antiAliaising));
    settings->endGroup();
}

void Configure::readConfigAxesGrid(QSettings *settings)
{
    settings->beginGroup("axesGrid");
    axesGrid.axes.showAxes = settings->value("axesGrid_showAxes").toBool();
    axesGrid.axes.xAxisColor = settings->value("axesGrid_xAxisColor").toInt();
    axesGrid.axes.yAxisColor = settings->value("axesGrid_yAxisColor").toInt();
    axesGrid.axes.arrowSizeInPix = settings->value("axesGrid_arrowSizeInPix").toInt();
    axesGrid.axes.axisSizeInPix = settings->value("axesGrid_axisSizeInPix").toInt();

    axesGrid.grid.showGrid = settings->value("axesGrid_showGrid").toBool();
    axesGrid.grid.gridColor = settings->value("axesGrid_gridColor").toInt();
    axesGrid.grid.xStep = settings->value("axesGrid_xStep").toInt();
    axesGrid.grid.yStep = settings->value("axesGrid_yStep").toInt();
    axesGrid.antiAliaising = settings->value("axesGrid_antiAliaising").toInt();
    settings->endGroup();
}

void Configure::writeConfigOffset(QSettings *settings)
{
    for(int i=0;i<offset.length();i++){
        settings->beginGroup(tr("offset") + QString::number(i));
        settings->setValue("offset_type", QVariant(offset.at(i).type));
        settings->setValue("offset_distance", QVariant(offset.at(i).distance));
        settings->setValue("offset_color", QVariant(offset.at(i).color));

        settings->endGroup();
    }
}

void Configure::readConfigOffset(QSettings *settings)
{
    for(int i=0;i<offset.length();i++){
        settings->beginGroup(tr("offset") + QString::number(i));
        offset[i].type = settings->value("offset_type").toString();
        offset[i].distance = settings->value("offset_distance").toReal();
        offset[i].color = settings->value("offset_color").toInt();

        settings->endGroup();
    }
}

void Configure::writeConfigLanguage(QSettings *settings)
{
    settings->setValue("language", QVariant(Language::Chinese));
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
    int alpha = rgb >> 32 & 255;
    return QColor(red, green, blue, alpha);
}

void Configure::onConfigChanged(QString name, QVariant value)
{
    qDebug() << "配置改变" << name << "   " << value;
    settings->setValue(name, value);
}


#include "dxffilter.h"
#include <QDebug>
#include "stdio.h"
#include "iostream"

DxfFilter::DxfFilter()
{

}

void DxfFilter::addLayer(const DL_LayerData &data)
{
    qDebug() << "LAYER: "
             << "name: " << QString::fromStdString(data.name) << "    "
             << "flags: " << data.flags << "    "
             << "off:   " << data.off;
    MyLayerData layer(data, attributes);
    layers.append(layer);
    getEntityAttributy();
}

void DxfFilter::addPoint(const DL_PointData &data)
{
    qDebug() << "POINT: "
             << "x: " << data.x << "    "
             << "y: " << data.y << "    "
             << "z: " << data.z;
    getEntityAttributy();
    MyPointData point(data, attributes);
    points.append(point);
}

void DxfFilter::addLine(const DL_LineData &data)
{
    qDebug() << "LINE: "
             << "point_start: (" << data.x1 << ", " << data.y1 << ", " << data.z1 << ")"
             << "point_end: (" << data.x2 << ", " << data.y2 << ", " << data.z2 << ")";
    getEntityAttributy();
    MyLineData line(data, attributes);
    lines.append(line);
}

void DxfFilter::addArc(const DL_ArcData &data)
{

}

void DxfFilter::addCircle(const DL_CircleData &data)
{

}

void DxfFilter::addPolyline(const DL_PolylineData &data)
{

}

void DxfFilter::addVertex(const DL_VertexData &data)
{

}

void DxfFilter::add3dFace(const DL_3dFaceData &data)
{

}

void DxfFilter::getEntityAttributy()
{
    qDebug() << "Attributes: Layer: " << QString::fromStdString(attributes.getLayer());
    qDebug() << " Color: ";
    if (attributes.getColor()==256)	{
        qDebug() << "BYLAYER";
    } else if (attributes.getColor()==0) {
        qDebug() << "BYBLOCK";
    } else {
        qDebug() << attributes.getColor();
    }
    qDebug() << " Width: ";
    if (attributes.getWidth()==-1) {
        qDebug() << "BYLAYER";
    } else if (attributes.getWidth()==-2) {
        qDebug() << "BYBLOCK";
    } else if (attributes.getWidth()==-3) {
        qDebug() << "DEFAULT";
    } else {
        qDebug() << attributes.getWidth();
    }
    qDebug() << " Type: " << QString::fromStdString(attributes.getLinetype());
}

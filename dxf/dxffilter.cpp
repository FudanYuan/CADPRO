#include "dxffilter.h"
#include <QDebug>
#include "stdio.h"
#include "iostream"

DxfFilter::DxfFilter()
{
    polylineIndex = 0;
}

void DxfFilter::addLayer(const DL_LayerData &data)
{
    MyLayerData layer(data, attributes);
    layers.append(layer);
    getEntityAttributy();
#ifdef DXFDEBUG
    qDebug() << "LAYER: "
             << "name: " << QString::fromStdString(data.name) << "    "
             << "flags: " << data.flags << "    "
             << "off:   " << data.off;
#endif
}

void DxfFilter::addPoint(const DL_PointData &data)
{
    getEntityAttributy();
    MyPointData point(data, attributes);
    points.append(point);
#ifdef DXFDEBUG
    qDebug() << "POINT: "
             << "x: " << data.x << "    "
             << "y: " << data.y << "    "
             << "z: " << data.z;
#endif
}

void DxfFilter::addLine(const DL_LineData &data)
{
    getEntityAttributy();
    MyLineData line(data, attributes);
    lines.append(line);
#ifdef DXFDEBUG
    qDebug() << "LAYER: " << transformText(attributes.getLayer())
             << "LINE: "
             << "point_start: (" << data.x1 << ", " << data.y1 << ")"
             << "point_end: (" << data.x2 << ", " << data.y2 << ")";
#endif
}

void DxfFilter::addRay(const DL_RayData &data)
{
    getEntityAttributy();
    MyRayData ray(data, attributes);
    rays.append(ray);
#ifdef DXFDEBUG
    qDebug() << "LAYER: " << transformText(attributes.getLayer())
             << "RAY: "
             << "point_start: (" << data.bx << ", " << data.by << ")"
             << "point_end: (" << data.dx << ", " << data.dy << ")";
#endif
}

void DxfFilter::addArc(const DL_ArcData &data)
{
    MyArcData arc(data, attributes);
    arcs.append(arc);
    getEntityAttributy();
#ifdef DXFDEBUG
    qDebug() << "LAYER: " << transformText(attributes.getLayer())
             << "ARC: "
             << "cPoint: (" << data.cx << ", " <<data.cy << ")"
             << "radius: " << data.radius
             << "angle1: " << data.angle1 << ", " << "angle2: " << data.angle2;
#endif
}

void DxfFilter::addCircle(const DL_CircleData &data)
{
    MyCircleData circle(data, attributes);
    circles.append(circle);
    getEntityAttributy();
#ifdef DXFDEBUG
    qDebug() << "LAYER: " << transformText(attributes.getLayer())
             << "CIRCLE: "
             << "cPoint: (" << data.cx << ", " <<data.cy << ")"
             << "radius: " << data.radius;
#endif
}

void DxfFilter::addEllipse(const DL_EllipseData &data)
{
    MyEllipseData ellipse(data, attributes);
    ellipses.append(ellipse);
    getEntityAttributy();
#ifdef DXFDEBUG
    qDebug() << "LAYER: " << transformText(attributes.getLayer())
             << "ELLIPSE: "
             << "cPoint: (" << data.cx << ", " <<data.cy << ")"
             << "radius1: " << data.mx - data.cx << ", " << data.my - data.cy
             << "sAngle: " << data.angle1 << ", "
             << "eAngle: " << data.angle2
             << "ratio: " << data.ratio;
#endif
}

void DxfFilter::addPolyline(const DL_PolylineData &data)
{
    MyPolylineData polyline(data, attributes);
    polylines.append(polyline);
    getEntityAttributy();
#ifdef DXFDEBUG
    qDebug() << "LAYER: " << transformText(attributes.getLayer())
             << "POLYLINE: "
             << "num: " << data.number
             << "m: " << data.m
             << "n: " << data.n
             << "elevation: " << data.elevation
             << "flags: " << data.flags;
#endif
}

void DxfFilter::addVertex(const DL_VertexData &data)
{
    MyVertexData vertex(data, attributes);
    vertexs.append(vertex);
    getEntityAttributy();
#ifdef DXFDEBUG
    qDebug() << "LAYER: " << transformText(attributes.getLayer())
             << "VERTEX: "
             << "point: (" << data.x  << ", " << data.y << ")"
             << "bulge: " << data.bulge;
#endif
}

void DxfFilter::addText(const DL_TextData &data)
{
    MyTextData text(data, attributes);
    texts.append(text);
    getEntityAttributy();
#ifdef DXFDEBUG
    qDebug() << "LAYER: " << transformText(attributes.getLayer())
             << "TEXT: "
             << "insert point: (" << data.ipx  << ", " << data.ipy << ")"
             << "alignment point: (" << data.apx << ", " << data.apy << ")"
             << "height: " << data.height
             << "x scale: " << data.xScaleFactor
             << "generation flags: " << data.textGenerationFlags
             << "h just: " << data.hJustification
             << "v just: " << data.vJustification
             << "text: " << transformText(data.text)
             << "style: " << transformText(data.style)
             << "angle: " << data.angle;
#endif
}

void DxfFilter::endEntity()
{
#ifdef DXFDEBUG
    qDebug()<<"end entity***********************";
    qDebug()<<"attribute ***************** "<<this->getAttributes().getColor();
#endif
}

void DxfFilter::endSequence()
{
    for(int i=0; i<vertexs.length(); i++){
        polylines[polylineIndex].vertexList.append(vertexs[i].vertex);
    }
    vertexs.clear();
    polylineIndex++;
}

void DxfFilter::getEntityAttributy()
{
#ifdef DXFDEBUG
    qDebug() << "";
    qDebug() << "----------- attribute -------- start ";
    qDebug() << "Layer: " << QString::fromStdString(attributes.getLayer());
    qDebug() << "Color: ";
    if (attributes.getColor()==256)	{
        qDebug() << "BYLAYER";
    } else if (attributes.getColor()==0) {
        qDebug() << "BYBLOCK";
    } else {
        qDebug() << attributes.getColor();
    }
    qDebug() << "Width: ";
    if (attributes.getWidth()==-1) {
        qDebug() << "BYLAYER";
    } else if (attributes.getWidth()==-2) {
        qDebug() << "BYBLOCK";
    } else if (attributes.getWidth()==-3) {
        qDebug() << "DEFAULT";
    } else {
        qDebug() << attributes.getWidth();
    }
    qDebug() << "Type: " << QString::fromStdString(attributes.getLinetype());
    qDebug() << "----------- attribute -------- end ";
    qDebug() << " ";
#endif
}

QString DxfFilter::transformText(const std::string text) const
{
     return QString::fromStdString(text);
}

QColor DxfFilter::transformColor(const int color) const
{
    /*
        black = 250,
        green = 3,
        red = 1,
        brown = 15,
        yellow = 2,
        cyan = 4,
        magenta = 6,
        gray = 8,
        blue = 5,
        l_blue = 163,
        l_green = 121,
        l_cyan = 131,
        l_red = 23,
        l_magenta = 221,
        l_gray = 252,
        white = 7,
        bylayer = 256,
        byblock = 0
    */
    QColor ret;
    switch (color) {
    case 0:
        ret = Qt::color0; // byblock = 0
    case 1:
    case 23:
        ret = Qt::red;
        break;
    case 2:
        ret = Qt::yellow;
        break;
    case 3:
    case 121:
        ret = Qt::green;
        break;
    case 4:
    case 131:
        ret = Qt::cyan;
        break;
    case 5:
        ret = Qt::blue;
        break;
    case 6:
    case 221:
        ret = Qt::magenta;
        break;
    case 7:
        ret = Qt::white;
        break;
    case 8:
    case 252:
        ret = Qt::gray;
        break;
    case 15:
        ret = Qt::darkRed;  // brown
        break;
    case 256:
        ret = Qt::color1;  //  bylayer = 256
    case 250:
    default:
        ret = Qt::black;
        break;
    }
    return ret;
}

int DxfFilter::transformColor(const QColor color) const
{
    int ret;
    switch (color.rgb()) {
    case Qt::color0:
        ret = 0; // byblock = 0
    case Qt::red:
        ret = 1;
        break;
    case Qt::yellow:
        ret = 2;
        break;
    case Qt::green:
        ret = 3;
        break;
    case Qt::cyan:
        ret = 4;
        break;
    case Qt::blue:
        ret = 5;
        break;
    case Qt::magenta:
        ret = 6;
        break;
    case Qt::white:
        ret = 7;
        break;
    case Qt::gray:
        ret = 8;
        break;
    case Qt::darkRed:
        ret = 15;  // brown
        break;
    case Qt::color1:
        ret = 256;  //  bylayer = 256
    case Qt::black:
    default:
        ret = 250;
        break;
    }
    return ret;
}

int DxfFilter::transformWidth(const int width) const
{
    int w = 0;
    switch (width) {
    case -1:
    case -2:
    case -3:
        break;
    default:
        w = width;
        break;
    }
    return w;
}

Qt::PenStyle DxfFilter::transformStyle(std::string style) const
{
    QString s = QString::fromStdString(style);
    if(s == "BYLAYER" || s == "BYBLOCK" || s == "CONTINUOUS"){
        return Qt::SolidLine;
    }
    if(s == "DASH"){
        return Qt::DashLine;
    }
    return Qt::SolidLine;
}

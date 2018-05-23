#ifndef DXFFILTER_H
#define DXFFILTER_H

#include "dl_creationadapter.h"
#include "dl_entities.h"
#include "customentities.h"
#include "common.h"
#include "debug.h"
#include <QList>

class DxfFilter : public DL_CreationAdapter
{
public:
    DxfFilter();

    virtual void addLayer(const DL_LayerData& data);
    virtual void addPoint(const DL_PointData& data);
    virtual void addLine(const DL_LineData& data);
    virtual void addRay(const DL_RayData& data);
    virtual void addArc(const DL_ArcData& data);
    virtual void addCircle(const DL_CircleData& data);
    virtual void addEllipse(const DL_EllipseData& data);
    virtual void addPolyline(const DL_PolylineData& data);
    virtual void addVertex(const DL_VertexData& data);
    virtual void addText(const DL_TextData& data);
    virtual void endEntity();
    virtual void endSequence();

    void getEntityAttributy();
    QString transformText(const std::string text) const;
    QColor transformColor(const int color) const;
    int transformColor(const QColor color) const;
    int transformWidth(const int width) const;
    Qt::PenStyle transformStyle(std::string style) const;

    QList<MyLayerData> layers;
    QList<MyPointData> points;
    QList<MyLineData> lines;
    QList<MyRayData> rays;
    QList<MyArcData> arcs;
    QList<MyCircleData> circles;
    QList<MyEllipseData> ellipses;
    QList<MyTextData> texts;
    QList<MyDimAngularData> dimangulars;
    QList<MyDimLinearData> dimlines;
    QList<MyDimRadialData> radias;
    QList<MyVertexData> vertexs;
    QList<MyPolylineData> polylines;
    QList<MyMTextData> mtexts;
    QList<MyHatchData> hatches;
    QList<MyHatchEdgeData> hatchedges;
    QList<QString> hatchcolors;
    int polylineIndex;  // 多边形的序号
};

#endif // DXFFILTER_H

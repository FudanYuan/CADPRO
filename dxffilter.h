#ifndef DXFFILTER_H
#define DXFFILTER_H

#include "dxflib/dl_creationadapter.h"
#include "dxflib/dl_creationadapter.h"
#include "dxflib/dl_entities.h"
#include "mydxfentities.h"
#include <QList>

class DxfFilter : public DL_CreationAdapter
{
public:
    DxfFilter();

    virtual void addLayer(const DL_LayerData& data);
    virtual void addPoint(const DL_PointData& data);
    virtual void addLine(const DL_LineData& data);
    virtual void addArc(const DL_ArcData& data);
    virtual void addCircle(const DL_CircleData& data);
    virtual void addPolyline(const DL_PolylineData& data);
    virtual void addVertex(const DL_VertexData& data);
    virtual void add3dFace(const DL_3dFaceData& data);

    void getEntityAttributy();
    QList<MyLayerData> layers;
    QList<MyPointData> points;
    QList<MyLineData> lines;
    QList<MyArcData> arcs;
    /*
    QList<MyCircleData> circles;
    QList<MyEllipseData> ellipses;
    QList<MyTextData> texts;
    QList<MyDimAngularData> dimangulars;
    QList<MyDimLinearData> dimlines;
    QList<MyDimRadialData> radias;
    QList<MyVertexData> vertices;
    QList<MyPolylineData> polylines;
    QList<MyMTextData> mtexts;
    QList<MyHatchData> hatches;
    QList<MyHatchEdgeData> hatchedges;
    QList<QString> hatchcolors;
    */
};

#endif // DXFFILTER_H

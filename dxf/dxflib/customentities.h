#ifndef CUSTOMENTITIES_H
#define CUSTOMENTITIES_H

#endif // CUSTOMENTITIES_H
#include "dl_attributes.h"
#include "dl_entities.h"

/**
 * My Layer Data.
 */
struct MyLayerData
{
    MyLayerData(DL_LayerData layer,
                DL_Attributes attribute) :
    layer(layer), attribute(attribute){

    }

    DL_LayerData layer;
    DL_Attributes attribute;
};

/**
 * My Point Data.
 */
struct MyPointData
{
     MyPointData(DL_PointData point,
                 DL_Attributes attribute) :
     point(point), attribute(attribute){

     }

     DL_PointData point;
     DL_Attributes attribute;
};

/**
 * My Line Data.
 */
struct MyLineData
{
     MyLineData(DL_LineData line,
                 DL_Attributes attribute) :
     line(line), attribute(attribute){

     }

     DL_LineData line;
     DL_Attributes attribute;
};

/**
 * My Arc Data.
 */
struct MyArcData
{
     MyArcData(DL_ArcData arc,
                 DL_Attributes attribute) :
     arc(arc), attribute(attribute){

     }

     DL_ArcData arc;
     DL_Attributes attribute;
};

/**
 * My Circle Data.
 */
struct MyCircleData
{
     MyCircleData(DL_CircleData circle,
                 DL_Attributes attribute) :
     circle(circle), attribute(attribute){

     }

     DL_CircleData circle;
     DL_Attributes attribute;
};

/**
 * My Ellipse Data.
 */
struct MyEllipseData
{
     MyEllipseData(DL_EllipseData ellipse,
                 DL_Attributes attribute) :
     ellipse(ellipse), attribute(attribute){

     }

     DL_EllipseData ellipse;
     DL_Attributes attribute;
};

/**
 * My Text Data.
 */
struct MyTextData
{
     MyTextData(DL_TextData text,
                 DL_Attributes attribute) :
     text(text), attribute(attribute){

     }

     DL_TextData text;
     DL_Attributes attribute;
};

/**
 * My Angular Dimension Data.
 */
struct MyDimAngularData
{
     MyDimAngularData(DL_DimAngularData dimAngular,
                 DL_Attributes attribute) :
     dimAngular(dimAngular), attribute(attribute){

     }

     DL_DimAngularData dimAngular;
     DL_Attributes attribute;
};

/**
 * My Linear Dimension Data.
 */
struct MyDimLinearData
{
     MyDimLinearData(DL_DimLinearData dimLinear,
                 DL_Attributes attribute) :
     dimLinear(dimLinear), attribute(attribute){

     }

     DL_DimLinearData dimLinear;
     DL_Attributes attribute;
};

/**
 * My Radia Dimension Data.
 */
struct MyDimRadialData
{
     MyDimRadialData(DL_DimRadialData dimRadia,
                 DL_Attributes attribute) :
     dimRadia(dimRadia), attribute(attribute){

     }

     DL_DimRadialData dimRadia;
     DL_Attributes attribute;
};

/**
 * My Vertex Data.
 */
struct MyVertexData
{
     MyVertexData(DL_VertexData vertex,
                 DL_Attributes attribute) :
     vertex(vertex), attribute(attribute){

     }

     DL_VertexData vertex;
     DL_Attributes attribute;
};

/**
 * My Polyline Data.
 */
struct MyPolylineData
{
     MyPolylineData(DL_PolylineData polyline,
                 DL_Attributes attribute) :
     polyline(polyline), attribute(attribute){

     }

     DL_PolylineData polyline;
     DL_Attributes attribute;
};

/**
 * My MText Data.
 */
struct MyMTextData
{
     MyMTextData(DL_MTextData mTextData,
                 DL_Attributes attribute) :
     mTextData(mTextData), attribute(attribute){

     }

     DL_MTextData mTextData;
     DL_Attributes attribute;
};

/**
 * My HatchData Data.
 */
struct MyHatchData
{
     MyHatchData(DL_HatchData hatchData,
                 DL_Attributes attribute) :
     hatchData(hatchData), attribute(attribute){

     }

     DL_HatchData hatchData;
     DL_Attributes attribute;
};

/**
 * My HatchData Data.
 */
struct MyHatchEdgeData
{
     MyHatchEdgeData(DL_HatchEdgeData hatchEdgeData,
                 DL_Attributes attribute) :
     hatchEdgeData(hatchEdgeData), attribute(attribute){

     }

     DL_HatchEdgeData hatchEdgeData;
     DL_Attributes attribute;
};

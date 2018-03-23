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
struct  MyPointData
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
struct  MyLineData
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
struct  MyArcData
{
     MyArcData(DL_ArcData arc,
                 DL_Attributes attribute) :
     arc(arc), attribute(attribute){

     }

     DL_ArcData arc;
     DL_Attributes attribute;
};

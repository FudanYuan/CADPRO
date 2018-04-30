#ifndef NF_STRUCT_H
#define NF_STRUCT_H

#ifndef _WIN32
#include <strings.h>
#endif

#include <iostream>
#include <fstream>
#include "sheet.h"

struct PieceCenter
{
    int id;
    int sheetId;
    int type;
    double angle;
    int reserve2;
    double x;
    double y;

    PieceCenter():
        id(0),
        sheetId(0),
        type(0),
        angle(0.000),
        reserve2(0),
        x(0.000),
        y(0.000)
    {

    }

    PieceCenter(int i, int s, int t, double a, int res2, int px, int py) {
        id = i;
        sheetId = s;
        type = t;
        angle = a;
        reserve2 = res2;
        x = px;
        y = py;
    }

    QPointF getCenter(){
        return QPointF(x, y);
    }
};

struct PieceOffsetHeader
{
    int reserve1;
    std::string layer;
    std::string type;
    std::string sheet;
    int typeNum;
    int reserve2;
    PieceOffsetHeader(int res1, std::string l,
                      std::string t, std::string s,
                      int n, int res2) {
        reserve1 = res1;
        layer = l;
        type = t;
        sheet = s;
        typeNum = n;
        reserve2 = res2;
    }
};

struct PiecePointsList
{
    double x;
    double y;
    double z;
    PiecePointsList(double px, double py, double pz) {
        x = px;
        y = py;
        z = pz;
    }
};

struct PieceOffset
{
    int typeId;
    double reserve1;
    double reserve2;
    double reserve3;
    std::string reserve4;
    double reserve5;
    int reserve6;
    int count;
    QList<PiecePointsList> pointsList;

    PieceOffset() :
        typeId(0),
        reserve1(0.000),
        reserve2(0.000),
        reserve3(0.000),
        reserve4(""),
        reserve5(0.000),
        reserve6(0),
        count(0)
    {
        pointsList.clear();
    }

    PieceOffset(int i, int res1, int res2,
                int  res3, std::string res4,
                double res5, int res6,
                int c, QList<PiecePointsList> list) {
        typeId = i;
        reserve1 = res1;
        reserve2 = res2;
        reserve3 = res3;
        reserve4 = res4;
        reserve5 = res5;
        reserve6 = res6;
        count = c;
        pointsList.append(list);
    }
};

#endif // NF_STRUCT_H

#ifndef NESTENGINECONFIGURE_H
#define NESTENGINECONFIGURE_H

#include <QObject>
#include <QList>
#include "nestengine.h"

#define NESTENGINECONFIGURE_XML "nest_engine_configure.xml"

/**
 * @brief The AutoNestConfigure class
 * // to do
 * 1. 构造几个结构体，每个结构体对应于每种材料的排版配置
 * 3. 能对配置进行保存、载入及修改
 * 4. 能够实现配置列表，参照sheet
 */

class NestEngineConfigure
{
public:
    struct StripSheetNest{
        StripSheetNest() :
            strategy(NestEngine::NoStrategy),
            stripmixing(NestEngine::NoMixing),
            stripadaptive(NestEngine::NoAdaptiveSpacing)
        {}
        NestEngine::NestEngineStrategys strategy;
        NestEngine::NestMixingTypes stripmixing;
        NestEngine::NestAdaptiveSpacingTypes stripadaptive;
    };

    struct WholeSheetNest{
        WholeSheetNest() :
            wholemixing(NestEngine::NoMixing),
            wholeorientation(NestEngine::VerticalNest)
            {}
        NestEngine::NestMixingTypes wholemixing;
        NestEngine::NestOrientations wholeorientation;
        int wholedegree=0;
    };

    struct PackageSheetNest{
        PackageSheetNest() :
            packagemixing(NestEngine::NoMixing),
            packageorientation(NestEngine::VerticalNest)
            {}
        NestEngine::NestMixingTypes packagemixing;
        NestEngine::NestOrientations packageorientation;
        int packagedegree=0;
    };

    explicit NestEngineConfigure();
    QMap<int,QList<QList<int>>> * LoadConfigureXml();
    void WriteConfigureXml(QMap<int,QList<QList<int>>> * writedata);
    WholeSheetNest getWholeSheetNest();
    StripSheetNest getStripSheetNest();
    StripSheetNest stripSheetNest;
    WholeSheetNest wholeSheetNest;
    PackageSheetNest packageSheetNest;
};

#endif

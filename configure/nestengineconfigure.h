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
    // 针对于整体材料的配置
    struct WholeSheetNest{
        WholeSheetNest() :
            wholemixing(NestEngine::NoMixing),
            wholeorientation(NestEngine::HorizontalNest),
            wholedegree(0)
        {

        }
        NestEngine::NestMixingTypes wholemixing;
        NestEngine::NestOrientations wholeorientation;
        int wholedegree;
    };

    // 针对于条板材料的配置
    struct StripSheetNest{
        StripSheetNest() :
            strategy(NestEngine::NoStrategy),
            stripmixing(NestEngine::NoMixing),
            stripadaptive(NestEngine::NoAdaptiveSpacing)
        {

        }
        NestEngine::NestEngineStrategys strategy;
        NestEngine::NestMixingTypes stripmixing;
        NestEngine::NestAdaptiveSpacingTypes stripadaptive;
    };

    // 针对于卷装材料的配置
    struct PackageSheetNest{
        PackageSheetNest() :
            packagemixing(NestEngine::NoMixing),
            packageorientation(NestEngine::VerticalNest),
            packagedegree(0)
        {

        }
        NestEngine::NestMixingTypes packagemixing;
        NestEngine::NestOrientations packageorientation;
        int packagedegree;
    };

    // 公有配置
    struct CommonConfig
    {
        CommonConfig()
        {

        }
    };
    explicit NestEngineConfigure();
    QMap<int,QList<QList<int>>>  LoadConfigureXml();
    void WriteConfigureXml(QMap<int,QList<QList<int>>> & writedata);

    void setWholeSheetNest(WholeSheetNest wholeNest);
    WholeSheetNest getWholeSheetNest();
    void setStripSheetNest(StripSheetNest stripNest);
    StripSheetNest getStripSheetNest();
    void setPackageSheetNest(PackageSheetNest packageNest);
    PackageSheetNest getPackageSheetNest();

private:
    StripSheetNest stripSheetNest;
    WholeSheetNest wholeSheetNest;
    PackageSheetNest packageSheetNest;
};

#endif // NESTENGINECONFIGURE_H

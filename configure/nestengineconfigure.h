#ifndef NESTENGINECONFIGURE_H
#define NESTENGINECONFIGURE_H

#include <QObject>
#include <QList>
#include "nestengine.h"

#define NEST_ENGINE_CONFIGURE_XML "nest_engine_configure.xml"

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
    // to do
    // 定义结构体
    struct StripSheetNest{
        StripSheetNest() :
            strategy(NestEngine::NoStrategy)
        {}
        NestEngine::NestEngineStrategys strategy;
    };
    // ...

    // strcut WholeSheetNest
    // ...
    struct WholeSheetNest{
        WholeSheetNest() :
            mixtype(NestEngine::NoMixing)
        {}
        NestEngine::NestMixingTypes mixtype;

    };
    // strcut PackageSheetNest
    // ...
    explicit NestEngineConfigure();
    void LoadConfigureXml(int index);
    void WriteConfigureXml(int index ,QList<int> configurelist);
    WholeSheetNest getWholeSheetNest();
    StripSheetNest getStripSheetNest();
private:
    // to do
    // 声明上面结构体的变量
    StripSheetNest stripSheetNest;
    WholeSheetNest wholeSheetNest;

    // List, 保存配置好的排版配置
};

#endif // NESTENGINECONFIGURE_H

#ifndef NESTENGINE_H
#define NESTENGINE_H

#include <QObject>
#include <QFlags>
#include <QVector>
#include <piece.h>
#include <sheet.h>

class NestEngine : public QObject
{
    Q_OBJECT
public:
    /**
     * 排版零件
     * @brief The NestPiece struct
     */
    struct NestPiece
    {
        NestPiece() :
            index(-1),
            typeID(-1),
            sheetID(-1),
            position(QPointF(-INT_MAX, -INT_MAX)),
            alpha(0),
            nested(false)
        {
        }

        NestPiece(int i, int t) :
            index(i),
            typeID(t),
            sheetID(-1),
            position(QPointF(-INT_MAX, -INT_MAX)),
            alpha(0),
            nested(false)
        {}

        int index;  // 在整个零件列表中的序号
        int typeID;  // 零件类型ID
        int sheetID;  // 材料ID
        QPointF position;  // 参考点的位置
        qreal alpha;  // 旋转角度
        bool nested;  // 是否已排
    };

    /**
     * @brief The SameTypePiece struct
     * 同型体零件，指零件外形一致，只是大小不一
     */
    struct SameTypePiece
    {
        SameTypePiece() {}
        int typeID;  // 同型体ID
        QVector<int> pieceIDList;  // 零件ID列表
    };

    enum NestEngineType{
        PackPointNest,
        MinRectNest
    };

    /**
     * @brief The NestType enum
     * 排版方式
     */
    enum NestType{
        SigleRow,  // 单排
        DoubleRow,  // 双排
        PairWiseSigleRow,  // 对头单排
        PairWiseDoubleRow,  // 对头双排
    };

    /**
     * @brief The NestMixingType enum
     * 排版混合类型
     */
    enum NestMixingType{
        NoMixing = 0x00,  // 不混合
        TailPieceMixing = 0x01,  // 行内尾只混合
        TailLineMixing = 0x02,  // 尾行优化，允许任意角度
        SameTypeSizeMixing = 0x04,  // 同型体内尺码混合
        AllMixing = TailPieceMixing | TailLineMixing | SameTypeSizeMixing,  // 全混合
    };

    Q_DECLARE_FLAGS(NestMixingTypes, NestMixingType)
    Q_FLAG(NestMixingTypes)

    /**
     * @brief The NestAdaptiveSpacing enum
     * 排版自适应间距
     */
    enum NestAdaptiveSpacingType{
        NoAdaptiveSpacing = 0x00,  // 无自适应间距
        HorizontalAdaptiveSpacing = 0x01,  // 横向自适应间距
        VerticalAdaptiveSpacing = 0x02,  // 纵向自适应间距
        AllAdaptiveSpacing = HorizontalAdaptiveSpacing | VerticalAdaptiveSpacing,  // 全向自适应间距
    };
    Q_DECLARE_FLAGS(NestAdaptiveSpacingTypes, NestAdaptiveSpacingType)
    Q_FLAG(NestAdaptiveSpacingTypes)

    enum NestDirection{
        HorizontalNest = 0x01,  // 横向排版
        VerticalNest = 0x02,  // 纵向排版
        AllDirectionNest = HorizontalNest | VerticalNest,  // 全向排版
    };
    Q_DECLARE_FLAGS(NestDirections, NestDirection)
    Q_FLAG(NestDirections)

    /**
     * @brief The NestEngineStrategy enum
     * 排版策略
     */
    enum NestEngineStrategy{
        NoStrategy = 0x00,  // 无策略
        LeftRightTurn = 0x01, // 左右交替
        SizeDown = 0x02,  // 尺码由大到小
        AllStrategy = LeftRightTurn | SizeDown,  // 使用全部策略
        Reserved = 0xff,  // 预留
    };
    Q_DECLARE_FLAGS(NestEngineStrategys, NestEngineStrategy)
    Q_FLAG(NestEngineStrategys)

    NestEngine();
    NestEngine(const QVector<Piece> pieceList, QVector<Sheet> sheetList);
    ~NestEngine();

    void setAutoRepeatLastSheet(bool flag);  // 设置是否自动重复使用最后一张材料
    bool getAutoRepeatLastSheet();  // 获取是否自动重复使用最后一张材料

    void setCompactStep(qreal step);  // 设置靠接步长
    qreal getCompactStep();  // 获取靠接步长

    void setCompactAccuracy(qreal accuracy);  // 设置靠接精度
    qreal getCompactAccuracy();  // 获取靠接精度

    void setNestEngineType(NestEngineType type);  // 设置排版引擎类型
    NestEngineType getNestEngineType();  // 获取排版引擎类型

    void setNestType(NestType type);  // 设置排版方式
    NestType getNestType();  // 获取排版方式

    void setNestMixingTypes(NestMixingTypes types);  // 设置混合方式
    NestMixingTypes getNestMixingTypes();  // 获取混合方式

    void setNestAdaptiveSpacingTypes(NestAdaptiveSpacingTypes types);  // 设置自适应间距方式
    NestAdaptiveSpacingTypes getNestAdaptiveSpacingTypes();  // 获取自适应间距方式

    void setNestDirections(NestEngine::NestDirections dires);  // 设置排版方向
    NestDirections getNestDirections();  // 获取排版方向

    void setNestEngineStrategys(NestEngineStrategys strategys);  // 设置排版引擎策略
    NestEngineStrategys getNestEngineStrategys();  // 获取排版策略

    void setCutStep(qreal step);  // 设置一刀切切割长度
    qreal getCutStep();  // 获取一刀切切割长度

    void setMaxRotateAngle(int angle);  // 设置最大旋转角度
    qreal getMaxRotateAngle();  // 获取最大旋转角度

    void setMinHeightOpt(bool flag);  // 设置是否自动重复使用最后一张材料
    bool getMinHeightOpt();  // 获取是否自动重复使用最后一张材料

    QVector<Piece> getSortedPieceListByArea(QVector<Piece> pieceList);  // 按面积将多边形列表排序
    void initNestPieceList();  // 初始化排版零件列表，默认按面积降序排序
    virtual void layoutAlg(QVector<int> indexList) = 0;  //  排版算法
    virtual bool packOnePiece(Piece piece, NestEngine::NestPiece &nestPiece) = 0;  // 排放单个零件
    virtual bool packOnePieceOnSheet(Piece piece, int sheetID, NestEngine::NestPiece &nestPiece) = 0;  // 在给定材料上排放单个零件
    virtual bool compact(int sheetID, NestPiece &nestPiece) = 0;  // 紧凑算法
    virtual bool collidesWithOtherPieces(int sheetID, Piece piece) = 0;  // 判断该零件是否与其他零件重叠

//protected:
    QVector<Piece> pieceList;  // 零件列表
    QVector<Sheet> sheetList;  // 材料列表
    QVector<NestPiece> nestPieceList;  // 排版零件列表
    QVector<int> nestedPieceIDlist;  // 已排零件ID列表
    QVector<int> unnestedPieceIDlist;  // 未排零件ID列表
    QMap<int, QVector<int>> nestSheetPieceMap;  // 排样材料-零件索引
    bool autoRepeatLastSheet;  // 自动重复使用最后一张材料
    qreal compactStep;  // 靠接步长，单位为mm
    qreal compactAccuracy;  // 靠接精度，单位为mm

    NestEngineType nestEngineType;  // 排版引擎类型
    NestType nestType;  // 排版方式
    NestMixingTypes mixingTyes;  // 混合方式
    NestAdaptiveSpacingTypes adaptiveSpacingTypes;  // 自适应间距类型
    NestDirections directions;  // 排版方向
    NestEngineStrategys nestEngineStrategys;  // 排版策略
    bool oneKnifeCut;  // 采用一刀切
    qreal cutStep;  // 切割步长
    bool rotatable;  // 可旋转
    int maxRotateAngle;  // 最大转动角度
    bool minHeightOpt;  // 最小高度优化
};

Q_DECLARE_OPERATORS_FOR_FLAGS(NestEngine::NestMixingTypes)
Q_DECLARE_OPERATORS_FOR_FLAGS(NestEngine::NestAdaptiveSpacingTypes)
Q_DECLARE_OPERATORS_FOR_FLAGS(NestEngine::NestDirections)
Q_DECLARE_OPERATORS_FOR_FLAGS(NestEngine::NestEngineStrategys)
#endif // NESTENGINE_H

#ifndef NESTENGINE_H
#define NESTENGINE_H

#include <QObject>
#include <QFlags>
#include <QVector>
#include <piece.h>
#include <sheet.h>

class NestEngineConfigure;

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
            referenceLine(QLineF()),
            alpha(0),
            nested(false)
        {
        }

        NestPiece(int i, int t) :
            index(i),
            typeID(t),
            sheetID(-1),
            position(QPointF(-INT_MAX, -INT_MAX)),
            referenceLine(QLineF()),
            alpha(0),
            nested(false)
        {}

        NestPiece(int i, int t, qreal a) :
            index(i),
            typeID(t),
            sheetID(-1),
            position(QPointF(-INT_MAX, -INT_MAX)),
            referenceLine(QLineF()),
            alpha(a),
            nested(false)
        {}

        int index;  // 在整个零件列表中的序号
        int typeID;  // 零件类型ID
        int sheetID;  // 材料ID
        QPointF position;  // 参考点的位置
        QLineF referenceLine;  // 参考线
        qreal alpha;  // 旋转角度
        bool nested;  // 是否已排
    };

    /**
     * @brief The SameTypePiece struct
     * 同型体零件，指零件外形一致，只是大小不一
     */
    struct SameTypePiece
    {
        SameTypePiece() :
            typeID(-1),
            pieceIDList(QVector<int>())
        {}

        SameTypePiece(int id, QVector<int> idList) :
            typeID(id),
            pieceIDList(idList)
        {}

        int typeID;  // 同型体ID
        QVector<int> pieceIDList;  // 零件ID列表
    };

    /**
     * @brief The PairPiece struct
     * 同双零件，指零件大小一致，形状互为镜像
     */
    struct PairPiece
    {
        PairPiece() :
            pairID(-1),
            size(-1),
            leftID(-1),
            rightID(-1)
        {

        }

        PairPiece(int id, int s, int left, int right) :
            pairID(id),
            size(s),
            leftID(left),
            rightID(right)
        {

        }

        int pairID;  // 同双ID
        int size;  // 零件尺码
        int leftID;  // 左支零件ID
        int rightID;  // 右支零件ID
    };

    /**
     * @brief The IDRange struct
     * 零件组成排版零件后在列表中的序号范围
     */
    struct PieceIndexRange
    {
        PieceIndexRange() :
            pieceID(-1),
            minIndex(-1),
            maxIndex(-1),
            step(1)
        {

        }
        PieceIndexRange(int id, int min, int max) :
            pieceID(id),
            minIndex(min),
            maxIndex(max),
            step(1)
        {

        }
        PieceIndexRange(int id, int min, int max, int s) :
            pieceID(id),
            minIndex(min),
            maxIndex(max),
            step(s)
        {

        }
        int pieceID;  // 零件id
        int minIndex;  // 最小序号
        int maxIndex;  // 最大序号
        int step;  // 步长
    };

    /**
     * @brief The nestException enum
     * 排版异常
     */
    enum NestException{
        NoSheetError,  // 无材料
        SheetSizeError,  // 材料大小错误
        PiecePairError,  // 零件不成对
    };

    /**
     * @brief The NestEngineType enum
     * 引擎类型
     */
    enum NestEngineType{
        MinRectNest,
        PackPointNest,
        ContinueNest
    };

    /**
     * @brief The NestType enum
     * 排版方式
     */
    enum NestType{
        NoNestType,  // 无
        SingleRow,  // 单排,默认为单排
        DoubleRow,  // 双排
        OppositeSingleRow,  // 对头单排
        OppositeDoubleRow,  // 对头双排
    };

    /**
     * @brief The BestNestType struct
     * 最佳排版配置
     */
    struct BestNestType
    {
        BestNestType() :
            pieceID(-1),
            nestType(SingleRow),
            alpha(0.0f),
            xStep(0.0f),
            pOffset(QPointF()),
            rCOffset(QPointF()),
            yStep(0.0f)
        {

        }

        BestNestType(int id, NestType type, qreal a, qreal x, QPointF po, QPointF ro) :
            pieceID(id),
            nestType(type),
            alpha(a),
            xStep(x),
            pOffset(po),
            rCOffset(ro),
            yStep(0.0f)
        {

        }

        BestNestType(int id, NestType type, qreal a, qreal x, QPointF po, QPointF ro, qreal y) :
            pieceID(id),
            nestType(type),
            alpha(a),
            xStep(x),
            pOffset(po),
            rCOffset(ro),
            yStep(y)
        {

        }

        int pieceID;  // 零件Id
        NestType nestType;  // 排版方式
        qreal alpha;  // 旋转角度
        qreal xStep;  // x方向送料步距
        QPointF pOffset;  // 针对双排方式的位置偏移
        QPointF rCOffset;  // 组合外包矩形中心点偏移
        qreal yStep;  // y方向送料步距
    };

    /**
     * @brief The SinglePieceStatus struct
     * 单个零件状态
     */
    struct SinglePieceStatus
    {
        SinglePieceStatus() :
            pos(QPointF()),
            pieceWidth(0.0f),
            pieceHeight(0.0f),
            alpha(0.0f),
            errorFlag(false)
        {

        }

        SinglePieceStatus(QPointF p,
                          qreal pW,
                          qreal pH,
                          qreal a) :
            pos(p),
            pieceWidth(pW),
            pieceHeight(pH),
            alpha(a),
            errorFlag(false)
        {

        }

        QPointF pos;  // 零件的位置
        qreal pieceWidth;  // 单个零件的宽
        qreal pieceHeight;  // 单个零件的高
        qreal alpha;  // 零件1旋转角度
        bool errorFlag;  // 错误标志
    };

    /**
     * @brief The PairPieceStatus struct
     * 组合零件状态
     */
    struct PairPieceStatus
    {
        PairPieceStatus() :
            pos1(QPointF()),
            pos2(QPointF()),
            pieceWidth1(0.0f),
            pieceHeight1(0.0f),
            pieceWidth2(0.0f),
            pieceHeight2(0.0f),
            alpha1(0.0f),
            alpha2(0.0f),
            xStep(0.0f),
            yStep(0.0f),
            pairCenter(QPointF()),
            pairWidth(0.0f),
            pairHeight(0.0f),
            errorFlag(false)
        {

        }

        PairPieceStatus(QPointF p1, QPointF p2,
                        qreal piW1, qreal piH1,
                        qreal piW2, qreal piH2,
                        qreal a1, qreal a2,
                        qreal x, qreal y,
                        QPointF c,
                        qreal paW, qreal paH) :
            pos1(p1),
            pos2(p2),
            pieceWidth1(piW1),
            pieceHeight1(piH1),
            pieceWidth2(piW2),
            pieceHeight2(piH2),
            alpha1(a1),
            alpha2(a2),
            xStep(x),
            yStep(y),
            pairCenter(c),
            pairWidth(paW),
            pairHeight(paH),
            errorFlag(false)
        {

        }

        QPointF pos1;  // 零件1的位置
        QPointF pos2;  // 零件2的位置
        qreal pieceWidth1;  // 单个零件的宽
        qreal pieceHeight1;  // 单个零件的高
        qreal pieceWidth2;  // 单个零件的宽
        qreal pieceHeight2;  // 单个零件的高
        qreal alpha1;  // 零件1旋转角度
        qreal alpha2;  // 零件2旋转角度
        qreal xStep;  // 送料x步距
        qreal yStep;  // 送料y步距
        QPointF pairCenter;  // 组合零件的中心
        qreal pairWidth;  // 组合零件的宽
        qreal pairHeight;  // 组合零件的高
        bool errorFlag;  // 状态是否错误
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

    enum NestOrientation{
        HorizontalNest = 0x01,  // 横向排版
        VerticalNest = 0x02,  // 纵向排版
        AllOrientationNest = HorizontalNest | VerticalNest,  // 全向排版
    };
    Q_DECLARE_FLAGS(NestOrientations, NestOrientation)
    Q_FLAG(NestOrientations)

    /**
     * @brief The NestEngineStrategy enum
     * 排版策略
     */
    enum NestEngineStrategy{
        NoStrategy = 0x00,  // 无策略
        SizeDown = 0x01,  // 尺码由大到小
        LeftRightTurn = 0x02, // 左右交替
        ReferenceLine = 0x04, // 参考线排版
        AllStrategys = LeftRightTurn | SizeDown,  // 使用全部策略
        Reserved = 0xff,  // 预留
    };
    Q_DECLARE_FLAGS(NestEngineStrategys, NestEngineStrategy)
    Q_FLAG(NestEngineStrategys)

    explicit NestEngine(QObject *parent=0);
    explicit NestEngine(QObject *parent, const QVector<Piece> pieceList, QVector<Sheet> sheetList);
    explicit NestEngine(QObject *parent, const QVector<Piece> pieceList, QVector<Sheet> sheetList, QVector<SameTypePiece> sameTypePieceList);
    virtual ~NestEngine();

    void setPieceList(const QVector<Piece> &pieceList);  // 设置切割件列表
    QVector<Piece> getPieceList();  // 获取切割件列表

    void setSheetList(QVector<Sheet> &sheetList);  // 设置材料列表
    QVector<Sheet> getSheetList();  // 获取材料列表

    void setSameTypePieceList(QVector<SameTypePiece> sameTypePieceList);  // 设置同型体
    QVector<SameTypePiece> getSameTypePieceList();  // 获得同型体

    void setPairPieceList(QVector<PairPiece> pairPieceList);  // 设置同双零件列表
    QVector<PairPiece> getPairPieceList();  // 获得同双体

    QVector<NestPiece> getNestPieceList();  // 获取排样切割件列表
    QVector<int> getNestedPieceIndexlist();  // 获取已排样的切割件序号列表

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

    void setNestOrientations(NestEngine::NestOrientations oriens);  // 设置排版方向
    NestOrientations getNestOrientations();  // 获取排版方向

    void setNestEngineStrategys(NestEngineStrategys strategys);  // 设置排版引擎策略
    NestEngineStrategys getNestEngineStrategys();  // 获取排版策略

    void setCutStep(qreal step);  // 设置一刀切切割长度
    qreal getCutStep();  // 获取一刀切切割长度

    void setMaxRotateAngle(int angle);  // 设置最大旋转角度
    qreal getMaxRotateAngle();  // 获取最大旋转角度

    void setMinHeightOpt(bool flag);  // 设置是否自动重复使用最后一张材料
    bool getMinHeightOpt();  // 获取是否自动重复使用最后一张材料

    void sortedPieceListByArea(QVector<Piece> pieceList, QMap<int, QVector<int>> &transformMap);  // 按面积将多边形列表排序, 并可得到映射关系
    void initQuadTreeMap(int sheetID);  // 初始化四叉树Map
    void initNestPieceList();  // 初始化排版零件列表，默认按面积降序排序
    void initSameTypeNestPieceIndexMap();  // 初始化同型体排版零件列表Map
    void initSamePairNestPieceIndexMap();  // 初始化同双体排版零件列表Map
    void initNestEngineConfig(Sheet::SheetType sheetType, NestEngineConfigure *proConfig);  // 初始化排版引擎配置

    void singleRowNest(Piece piece, qreal &alpha, qreal &stepX, qreal &width, qreal &height);  // 最优单排
    void doubleRowNest(Piece piece, const int n, qreal &alpha, qreal &stepX, QPointF &cOffset, qreal &width, qreal &height);  // 最优双排
    void pairwiseDoubleRowNest(Piece piece, qreal &alpha, QPointF &cOffset, qreal &width, qreal &height);  // 最优对头双排

    qreal calRealBestXStep(const Piece &piece, const qreal alpha, const QPointF &offset, bool forOppFlag=true) const;  // 对对头排版计算真实的送料步距
    /**
     * 4中单零件排样策略，返回材料利用率
     */
    qreal singleRowNestWithVerAlg(const Piece &piece,
                                  qreal &alpha,
                                  qreal &step,
                                  const int maxRotateAngle=180,
                                  const qreal maxWidth=LONG_MAX,
                                  const qreal maxHeight=LONG_MAX);  // 单排，使用顶点算法

    qreal doubleRowNestWithVerAlg(const Piece &piece,
                                  qreal &alpha,
                                  qreal &step,
                                  qreal &X,
                                  qreal &H,
                                  const qreal n=100,
                                  const int maxRotateAngle=180,
                                  const qreal maxWidth=LONG_MAX,
                                  const qreal maxHeight=LONG_MAX);  // 双排，使用顶点算法

    qreal oppositeSingleRowNestWithVerAlg(const Piece &piece, qreal &alpha,
                                          qreal &step, QPointF &offset,
                                          const int maxRotateAngle=180,
                                          const qreal maxWidth=LONG_MAX,
                                          const qreal maxHeight=LONG_MAX);  // 对头单排，使用顶点算法

    qreal oppositeDoubleRowNestWithVerAlg(const Piece &piece,
                                          qreal &alpha,
                                          qreal &step,
                                          QPointF &offset,
                                          qreal &H,
                                          const qreal n=100,
                                          const int maxRotateAngle=180,
                                          const qreal maxWidth=LONG_MAX,
                                          const qreal maxHeight=LONG_MAX);  // 对头双排，使用顶点算法

    NestType getPieceBestNestType(const Piece &piece,
                                  qreal &alpha,
                                  qreal &xStep,
                                  QPointF &pOffset,
                                  QPointF &rCOffset,
                                  const int maxRotateAngle=180,
                                  const qreal maxWidth=LONG_MAX,
                                  const qreal maxHeight=LONG_MAX);  // 获取零件的最佳排版方式

    void getAllBestNestTypes(QVector<Piece> pieceList);  // 获取所有零件最佳排样方式

    qreal oppositeDoubleRowNestWithVerAlgForStrip(const QRectF &layoutRect,
                                                  const Piece &piece1,
                                                  const Piece &piece2,
                                                  QPointF &pos1,
                                                  QPointF &pos2,
                                                  qreal &xStep,
                                                  const bool flag, QRectF &boundRect1, QRectF &boundRect2, QRectF &pairBoundRect);

    void packAlg();  // 排版算法

    virtual void packPieces(QVector<int> indexList);  //  排版算法
    virtual bool packOnePiece(Piece piece, NestEngine::NestPiece &nestPiece);  // 排放单个零件
    virtual bool packOnePieceOnSheet(Piece piece, int sheetID, NestEngine::NestPiece &nestPiece);  // 在给定材料上排放单个零件
    virtual bool compact(int sheetID, NestPiece &nestPiece);  // 紧凑算法
    virtual bool collidesWithOtherPieces(int sheetID, Piece piece);  // 判断该零件是否与其他零件重叠

signals:
    void nestPieceUpdate(NestEngine::NestPiece nestPiece);  // 排版零件更新
    void nestFinished(QVector<NestEngine::NestPiece> nestPieceList);  // 排版完成信号
    void nestException(NestException e);  // 排版错误
    void nestInterrupted(int remainNum);  // 排版中断
    void autoRepeatedLastSheet(Sheet sheet);  // 添加材料
    void progress(int count);  // 排版进程
    void nestDebug(int, QPointF, QPointF);  // 测试用
    void nestDebugLine(int, QLineF);  // 测试用
    void nestDebugRemainRect(int, QRectF);  // 测试用

public slots:
    void onNestStart();  // 开始排版

protected:
    QVector<Piece> pieceList;  // 零件列表
    QVector<Sheet> sheetList;  // 材料列表
    QVector<SameTypePiece> sameTypePieceList;  // 同型体零件
    QVector<PairPiece> pairPieceList;  // 同双零件
    QVector<NestPiece> nestPieceList;  // 排版零件列表
    QMap<int, QVector<int>> transformMap;  // 零件面积由小到大排序之后的索引与排序之前的索引转换关系
                                           // Map<索引, <变换前的索引, 变换后的索引]>
                                           // 当key为当前索引时，value[1]代表之前索引
                                           // 当key为之前索引时，value[0]代表当前索引
    QMap<int, PieceIndexRange> nestPieceIndexRangeMap;  // 排版零件的index范围 Map<零件id, 排样零件序号范围>
    QMap<int, QVector<int>> sameTypeNestPieceIndexMap;  // 同型体排版零件index列表 Map<同型体id, 零件序号列表>
    QMap<int, int> samePairNestPieceIndexMap;  // 同双体排版零件index列表 Map<零件id，其同双零件id>
    QVector<int> nestedPieceIndexlist;  // 已排零件Index列表
    QVector<int> unnestedPieceIndexlist;  // 未排零件Index列表
    QMap<int, QVector<int>> nestSheetPieceMap;  // 排样材料-零件索引 Map<材料id, 零件序号列表>
    QMap<int, BestNestType> pieceBestNestTypeMap;   // 记录零件-最佳排版方式 Map<零件id, 最佳排版方式>
    //QMap<int, QMap<int, QList<int>>> sheetRowPieceMap;  // 记录材料-行-零件 Map<材料id, Map<行id, 零件id列表>>
    QMap<int, int> pieceMaxPackPointMap;  // 记录零件-最大排样点序号 Map<零件id, 排样点id>   /////迁移至packPointNestEngine
    QMap<int, QuadTreeNode<Object>*> quadTreeMap;  // 四叉树 Map<材料id, 四叉树>

    bool isStripSheet;  // 条形板材料标志
    bool autoRepeatLastSheet;  // 自动重复使用最后一张材料
    qreal compactStep;  // 靠接步长，单位为mm
    qreal compactAccuracy;  // 靠接精度，单位为mm
    NestEngineType nestEngineType;  // 排版引擎类型
    NestType nestType;  // 排版方式
    NestMixingTypes mixingTyes;  // 混合方式
    NestAdaptiveSpacingTypes adaptiveSpacingTypes;  // 自适应间距类型
    NestOrientations orientations;  // 排版方向
    NestEngineStrategys nestEngineStrategys;  // 排版策略
    bool oneKnifeCut;  // 采用一刀切
    qreal cutStep;  // 切割步长
    bool rotatable;  // 可旋转
    int maxRotateAngle;  // 最大转动角度
    bool minHeightOpt;  // 最小高度优化
    int collisionCount;  // 碰撞检测次数

    // debug
    int counter;

    QRectF getPairBoundingRect(QPointF &pos1, QPointF &pos2, const qreal pieceWidth, const qreal pieceHeight);  // 计算组合零件的外包矩形
    QRectF getPairBoundingRect(QRectF &bRect1, QRectF &bRect2);  // 计算组合零件的外包矩形
};

Q_DECLARE_OPERATORS_FOR_FLAGS(NestEngine::NestMixingTypes)
Q_DECLARE_OPERATORS_FOR_FLAGS(NestEngine::NestAdaptiveSpacingTypes)
Q_DECLARE_OPERATORS_FOR_FLAGS(NestEngine::NestOrientations)
Q_DECLARE_OPERATORS_FOR_FLAGS(NestEngine::NestEngineStrategys)
#endif // NESTENGINE_H

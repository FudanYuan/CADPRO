#ifndef NESTENGINE_H
#define NESTENGINE_H

#include <QVector>
#include <piece.h>
#include <sheet.h>

class NestEngine
{
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

    enum NestEngineType{
        PackPointNest,
        MinRectNest
    };

    NestEngine();
    NestEngine(const QVector<Piece> pieceList, QVector<Sheet> sheetList);
    ~NestEngine();

    void setAutoRepeatLastSheet(bool flag);  // 设置是否自动重复使用最后一张材料
    bool getAutoRepeatLastSheet();  // 获取是否自动重复使用最后一张材料

    void setCompactStep(qreal step);  // 设置靠接步长
    qreal getCompactStep();  // 获取靠接步长

    void setCompactAccuracy(qreal accuracy);  // 设置靠接精度
    qreal getCompactAccuracy();  // 获取靠接精度

    QVector<Piece> getSortedPieceListByArea(QVector<Piece> pieceList);  // 按面积将多边形列表排序
    void initNestPieceList();  // 初始化排版零件列表，默认按面积降序排序
    virtual void layoutAlg(QVector<int> indexList) = 0;  //  排版算法
    virtual bool packOnePiece(Piece piece, NestEngine::NestPiece &nestPiece) = 0;  // 排放单个零件
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
};

#endif // NESTENGINE_H

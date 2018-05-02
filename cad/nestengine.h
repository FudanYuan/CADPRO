#ifndef NESTENGINE_H
#define NESTENGINE_H

#include <QList>
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
        NestPiece(int i, int t) :
            index(i),
            typeId(t),
            sheetId(-1),
            position(QPointF(-INT_MAX, -INT_MAX)),
            alpha(0),
            nested(false)
        {}

        int index;  // 在整个零件列表中的序号
        int typeId;  // 零件类型ID
        int sheetId;  // 材料ID
        QPointF position;  // 参考点的位置
        qreal alpha;  // 旋转角度
        bool nested;  // 是否已排
    };

    enum NestEngineType{
        PackPointNest,
        MinRectNest
    };
    NestEngine();
    NestEngine(const QList<Piece> pieceList, QList<Sheet> sheetList);
    ~NestEngine();

    QList<Piece> getSortedPieceListByArea(QList<Piece> pieceList);  // 按面积将多边形列表排序
    void initNestPieceList();  // 初始化排版零件列表，默认按面积降序排序
    virtual void layoutAlg(QList<int> indexList) = 0;  //  排版算法
    virtual void packOnePiece(Piece piece, NestEngine::NestPiece &nestPiece) = 0;  // 排放单个零件
    virtual bool collidesWithOtherPieces(int sheetID, Piece piece) = 0;  // 判断该零件是否与其他零件重叠
protected:
    QList<Piece> pieceList;  // 零件列表
    QList<Sheet> sheetList;  // 材料列表
    QList<NestPiece> nestPieceList;  // 排版零件列表
    bool autoRepeatLastSheet;  // 自动重复使用最后一张材料
};

#endif // NESTENGINE_H

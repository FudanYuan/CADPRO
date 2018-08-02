#ifndef CONTINUENESTENGINE_H
#define CONTINUENESTENGINE_H

#include "nestengine.h"
#include "piece.h"
#include "common.h"

class ContinueNestEngine : public NestEngine
{
public:
    enum RectType{
        NoRectType = 0x00,
        FirstRow = 0x01,
        SecondRow = 0x02,
        TailPiece = 0x04,
        TailLine = 0x08,
        NewPiece = 0x10,
    };

    Q_DECLARE_FLAGS(RectTypes, RectType)
    Q_FLAG(RectTypes)

    explicit ContinueNestEngine(QObject *parent);
    explicit ContinueNestEngine(QObject *parent, const QVector<Piece> pieceList, const QVector<Sheet> sheetList);
    explicit ContinueNestEngine(QObject *parent, const QVector<Piece> pieceList, const QVector<Sheet> sheetList, QVector<SameTypePiece> sameTypePieceList);
    ~ContinueNestEngine();

    bool packPieceByLayoutRect(const int sheetID,
                               const QRectF& layoutRect,
                               const int pieceType,
                               int pieceIndex,
                               const int pieceMaxIndex,
                               const BestNestType bestNestType,
                               PairPieceStatus &status,
                               QPointF &anchorPos,
                               qreal &spaceDelta,
                               bool &sheetAvailable,
                               QVector<int> &sameRowPieceList,
                               QList<int> &nestedList, QList<int> &unnestedList,
                               const RectTypes rectType, bool wholeSheetFlag,
                               QRectF &layoutRect1,
                               QRectF &layoutRect2);  // 按行排放零件
    PairPieceStatus initPairPieceStatus(const QRectF &layoutRect, Piece &piece, const BestNestType &bestNestType);  // 初始化状态
    void packPieces(QVector<int> indexList) Q_DECL_OVERRIDE;  // 排版算法
    bool packOnePiece(Piece piece, NestEngine::NestPiece &nestPiece) Q_DECL_OVERRIDE;  // 排放单个零件
    bool packOnePieceOnSheet(Piece piece, int sheetID, NestEngine::NestPiece &nestPiece) Q_DECL_OVERRIDE;  // 在给定材料上排放单个零件
    bool compact(int sheetID, NestPiece &nestPiece) Q_DECL_OVERRIDE;  // 紧凑算法
    bool collidesWithOtherPieces(int sheetID, Piece piece) Q_DECL_OVERRIDE;  // 判断该零件是否与其他零件碰撞

private:
    QRectF getPairBoundingRect(QPointF &pos1, QPointF &pos2, const qreal pieceWidth, const qreal pieceHeight);  // 计算组合零件的外包矩形
};
Q_DECLARE_OPERATORS_FOR_FLAGS(ContinueNestEngine::RectTypes)
#endif // CONTINUENESTENGINE_H

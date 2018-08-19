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

    PairPieceStatus initPairPieceStatus(const QRectF &layoutRect, Piece &piece, const BestNestType &bestNestType);  // 初始化状态

    PairPieceStatus initPairPieceStatus(const QRectF &layoutRect, Piece &piece1, Piece &piece2);

    SinglePieceStatus initSinglePieceStatus(const QRectF &layoutRect, const Piece &piece, int &flag);  // 初始化单个零件排版状态

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
                               QList<int> &nestedList,
                               QList<int> &unnestedList,
                               const RectTypes rectType,
                               QRectF &layoutRect1,
                               QRectF &layoutRect2);  // 按零件矩形排放零件

    bool packPieceByReferenceLine(const int sheetID,
                                  const QRectF &layoutRect,
                                  int pieceType,
                                  int pieceIndex,
                                  const int pieceMaxIndex,
                                  NestEngine::PairPieceStatus &status,
                                  qreal &spaceDelta,
                                  bool &sheetAvailable,
                                  QVector<int> &sameRowPieceList,
                                  QList<int> &nestedList,
                                  QList<int> &unnestedList,
                                  QRectF &layoutRect1,
                                  int &packFlag);  // 按参考线排放零件2

    bool packTailPiece(const int sheetID,
                       const QRectF &layoutRect,
                       int pieceType,
                       int pieceIndex,
                       NestEngine::PairPieceStatus &status,
                       qreal &spaceDelta,
                       bool &sheetAvailable,
                       QVector<int> &sameRowPieceList,
                       QList<int> &nestedList,
                       QList<int> &unnestedList,
                       QRectF &layoutRect1,
                       int &packFlag, bool isStripSheet);  // 排放尾只

    void packPieces(QVector<int> indexList) Q_DECL_OVERRIDE;  // 排版算法
    bool packOnePiece(Piece piece, NestEngine::NestPiece &nestPiece) Q_DECL_OVERRIDE;  // 排放单个零件
    bool packOnePieceOnSheet(Piece piece, int sheetID, NestEngine::NestPiece &nestPiece) Q_DECL_OVERRIDE;  // 在给定材料上排放单个零件
    bool compact(int sheetID, NestPiece &nestPiece) Q_DECL_OVERRIDE;  // 紧凑算法
    qreal compactOnHD(int sheetID, Piece piece);  // 水平方向靠接
    qreal compactOnVD(int sheetID, Piece piece);  // 垂直方向靠接
    bool collidesWithOtherPieces(int sheetID, Piece piece) Q_DECL_OVERRIDE;  // 判断该零件是否与其他零件碰撞
};
Q_DECLARE_OPERATORS_FOR_FLAGS(ContinueNestEngine::RectTypes)
#endif // CONTINUENESTENGINE_H

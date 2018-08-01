#ifndef CONTINUENESTENGINE_H
#define CONTINUENESTENGINE_H

#include "nestengine.h"
#include "piece.h"
#include "common.h"

class ContinueNestEngine : public NestEngine
{
public:
    enum RectType{
        FirstRow,
        SecondRow,
        TailPiece,
        TailLine
    };
    explicit ContinueNestEngine(QObject *parent);
    explicit ContinueNestEngine(QObject *parent, const QVector<Piece> pieceList, const QVector<Sheet> sheetList);
    explicit ContinueNestEngine(QObject *parent, const QVector<Piece> pieceList, const QVector<Sheet> sheetList, QVector<SameTypePiece> sameTypePieceList);
    ~ContinueNestEngine();

    void singleRowNest(Piece piece, qreal &alpha, qreal &stepX, qreal &width, qreal &height);  // 最优单排
    void doubleRowNest(Piece piece, const int n, qreal &alpha, qreal &stepX, QPointF &cOffset, qreal &width, qreal &height);  // 最优双排
    void pairwiseDoubleRowNest(Piece piece, qreal &alpha, QPointF &cOffset, qreal &width, qreal &height);  // 最优对头双排

    /**
     * 4中单零件排样策略，返回材料利用率
     */
    qreal singleRowNestWithVerAlg(Piece piece, qreal &alpha, qreal &step, const int maxRotateAngle=180, const qreal maxHeight=LONG_MAX);  // 单排，使用顶点算法
    qreal doubleRowNestWithVerAlg(Piece piece, qreal &alpha, qreal &step, qreal &X, qreal &H, const qreal n=100, const int maxRotateAngle=180, const qreal maxHeight=LONG_MAX);  // 双排，使用顶点算法
    qreal oppositeSingleRowNestWithVerAlg(Piece piece, qreal &alpha, qreal &step, QPointF &offset, const int maxRotateAngle=180, const qreal maxHeight=LONG_MAX);  // 对头单排，使用顶点算法
    qreal oppositeDoubleRowNestWithVerAlg(Piece piece, qreal &alpha, qreal &step, QPointF &offset, qreal &H, const qreal n=100, const int maxRotateAngle=180, const qreal maxHeight=LONG_MAX);  // 对头双排，使用顶点算法
    NestType getPieceBestNestType(const Piece &piece, qreal &alpha, qreal &xStep, QPointF &pOffset, QPointF &rCOffset, qreal &yStep, const int maxRotateAngle=180, const qreal maxHeight=LONG_MAX);  // 获取零件的最佳排版方式
    void getAllBestNestTypes(QVector<Piece> pieceList);  // 获取所有零件最佳排样方式
    bool packPieceByLayoutRect(const int sheetID,
                               const QRectF& layoutRect,
                               const int pieceType,
                               int pieceIndex,
                               const int pieceMaxIndex,
                               const BestNestType bestNestType,
                               int &columnCounter,
                               int &pieceCounter,
                               QPointF &anchorPos,
                               qreal &spaceDelta,
                               bool &sheetAvailable,
                               QVector<int> &sameRowPieceList,
                               QList<int> &nestedList, QList<int> &unnestedList,
                               const RectType rectType, bool wholeSheetFlag,
                               QRectF &layoutRect1,
                               QRectF &layoutRect2);  // 按行排放零件
    void initStatus(const QRectF &layoutRect, Piece piece, const BestNestType bestNestType, qreal &pieceWidth, qreal &pieceHeight,
                    QPointF &pos1, QPointF &pos2,
                    qreal &alpha1, qreal &alpha2,
                    qreal &xStep, qreal &yStep);  // 初始化状态
    void packPieces(QVector<int> indexList) Q_DECL_OVERRIDE;  // 排版算法
    bool packOnePiece(Piece piece, NestEngine::NestPiece &nestPiece) Q_DECL_OVERRIDE;  // 排放单个零件
    bool packOnePieceOnSheet(Piece piece, int sheetID, NestEngine::NestPiece &nestPiece) Q_DECL_OVERRIDE;  // 在给定材料上排放单个零件
    bool compact(int sheetID, NestPiece &nestPiece) Q_DECL_OVERRIDE;  // 紧凑算法
    bool collidesWithOtherPieces(int sheetID, Piece piece) Q_DECL_OVERRIDE;  // 判断该零件是否与其他零件碰撞
};

#endif // CONTINUENESTENGINE_H

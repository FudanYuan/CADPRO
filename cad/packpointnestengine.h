#ifndef PACKPOINTNESTENGINE_H
#define PACKPOINTNESTENGINE_H

#include "nestengine.h"
#include "piece.h"
#include "common.h"
#include <QMap>

class PackPointNestEngine : public NestEngine
{
public:
    struct PackPoint
    {
        PackPoint() :
            index(-1),
            position(QPointF(-INT_MAX, -INT_MAX)),
            coverd(false)
        {

        }
        PackPoint(int i, QPointF p, bool c) :
            index(i),
            position(p),
            coverd(c)
        {

        }

        int index;  // 排样点序号
        QPointF position;  // 排样点坐标
        bool coverd;  // 排样点是否被覆盖
    };

    struct PackPointInfo{
        PackPointInfo() :
            sheetID(-1),
            rows(0),
            columns(0),
            XOffset(0),
            YOffset(0)
        {

        }
        PackPointInfo(int id, int r, int c, qreal x, qreal y) :
            sheetID(id),
            rows(r),
            columns(c),
            XOffset(x),
            YOffset(y)
        {

        }

        PackPointInfo(int id, int r, int c, qreal x, qreal y, QVector<qreal> list) :
            sheetID(id),
            rows(r),
            columns(c),
            XOffset(x),
            YOffset(y),
            columnPosList(list)
        {

        }

        // 获取排样点个数
        int getPackPointNumber(){
            return rows * columns;
        }

        int sheetID;  // 材料ID
        int rows;  // 行数
        int columns; // 列数
        qreal XOffset;  // x方向的偏移
        qreal YOffset;  // y方向的偏移
        QVector<qreal> columnPosList;  // 该数组是针对于样板材料设置，用于记录参考线排版位置
        QVector<int> coverdList;  // 覆盖列表
    };

    PackPointNestEngine();
    PackPointNestEngine(const QVector<Piece> pieceList, const QVector<Sheet> sheetList, qreal PPD, int RN=1);
    ~PackPointNestEngine();

    void initPackPoint(QVector<Sheet> sheetList, qreal PPD);  // 初始化排样点
    void initPackPointOneSheet(int sheetID, qreal PPD);  // 初始化一个材料的排样点
    void updatePackPointOneSheet(int sheetID, Piece piece);  // 更新排样点

    void packPieces(QVector<int> indexList) Q_DECL_OVERRIDE;  // 排版算法
    bool packOnePiece(Piece piece, NestEngine::NestPiece &nestPiece) Q_DECL_OVERRIDE;  // 排放单个零件
    bool packOnePieceOnSheet(Piece piece, int sheetID, NestEngine::NestPiece &nestPiece) Q_DECL_OVERRIDE;  // 在给定材料上排放单个零件
    bool packOnePieceAttempt(Piece piece, int sheetID, NestEngine::NestPiece &nestPiece, QList<int> packPointList, int maxRotateAngle, int RN);  // 在给定材料上尝试排放单个零件    
    bool compact(int sheetID, NestPiece &nestPiece) Q_DECL_OVERRIDE;  // 紧凑算法
    bool collidesWithOtherPieces(int sheetID, Piece piece) Q_DECL_OVERRIDE;  // 判断该零件是否与其他零件重叠

private:
    qreal PPD; // pack point distance--排样点取样间隔
    int RN;  // rotate number--旋转个数
    QVector<PackPointInfo> packPointInfoList;  // 材料排样点信息
    QMap<int, QMap<int, PackPoint>> sheetPackPointPositionMap;  // 材料排样点状态
    QMap<int, QList<int>> unusedSheetPackPointMap;  // 剩余排样点
    qreal minHeight;  // 最小高度值，使用HAPE排版的重心值
};

#endif // PACKPOINTNESTENGINE_H

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
        PackPointInfo(int id, int r, int c, qreal x, qreal y) :
            sheetID(id),
            rows(r),
            columns(c),
            XOffset(x),
            YOffset(y)
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
        QList<int> coverdList;  // 覆盖列表
    };

    PackPointNestEngine();
    PackPointNestEngine(const QList<Piece> pieceList, const QList<Sheet> sheetList, qreal PPD, int RN);
    ~PackPointNestEngine();

    void initPackPoint(QList<Sheet> sheetList, qreal PPD, int PN);  // 初始化排样点
    void updatePackPoint(int sheetID, Piece piece);  // 更新排样点

    void layoutAlg(QList<int> indexList) Q_DECL_OVERRIDE;  // 排版算法
    void packOnePiece(Piece piece, NestEngine::NestPiece &nestPiece) Q_DECL_OVERRIDE;  // 排放单个零件
    bool collidesWithOtherPieces(Piece piece) Q_DECL_OVERRIDE;  // 判断该零件是否与其他零件重叠
private:
    qreal PPD; // pack point distance--排样点取样间隔
    int RN;  // rotate number--旋转个数
    QList<PackPointInfo> packPointInfoList;  // 材料排样点信息
    QMap<int, QMap<int, PackPoint>> sheetPackPointPositionMap;  // 材料排样点状态
    qreal minHeight;  // 最小高度值，使用HAPE排版的重心值
};

#endif // PACKPOINTNESTENGINE_H

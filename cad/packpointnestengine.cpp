#include "packpointnestengine.h"

PackPointNestEngine::PackPointNestEngine() :
    NestEngine(),
    PPD(5),
    RN(4),
    minHeight(LONG_MAX)
{

}

PackPointNestEngine::PackPointNestEngine(const QList<Piece> pieceList, const QList<Sheet> sheetList, qreal PPD, int RN) :
    NestEngine(pieceList, sheetList)
{
    this->PPD = PPD;
    this->RN = RN;
}

PackPointNestEngine::~PackPointNestEngine()
{

}

void PackPointNestEngine::initPackPoint(QList<Sheet> sheetList, qreal PPD, int PN)
{
    // 清空材料-排样点Map
    sheetPackPointPositionMap.clear();
    // 初始化材料-排样点Map
    for(int i=0; i<sheetList.length(); i++){
        Sheet sheet = sheetList[i];
        QRectF layoutRect = sheet.layoutRect();
        qreal width = layoutRect.width();
        qreal height = layoutRect.height();
        qreal XOffset = sheet.leftMargin;
        qreal YOffset = sheet.topMargin;

        // debug
        width = 1000;
        height = 10000;
        // debug

        // 记录第i个材料的排样点信息
        QMap<int, PackPoint> packPointMap;
        // 获取行列数即排样点个数总数
        int rows = qFloor(height / PPD + 1);
        int columns = qFloor(width / PPD + 1);
        int PPN = rows * columns;
        for(int j=0; j<PPN; j++){
            int row = j / rows;  // 获取序号为j的排样点在第几行
            int column = j - columns * row;  // 获取序号为j的排样点在第几列
            // 将序号为j的排样点添加至map中
            qreal px = column * PPD + XOffset;
            qreal py = row * PPD + YOffset;
            PackPoint packPoint(j, QPointF(px, py), false);
            packPointMap.insert(j, packPoint);
        }
        // 记录该材料的排样点信息
        PackPointInfo info(i, rows, columns, XOffset, YOffset);
        packPointInfoList.append(info);
        // 将第i个材料的排样点信息加入到map中去
        sheetPackPointPositionMap.insert(i, packPointMap);
    }
}

void PackPointNestEngine::updatePackPoint(int sheetID, Piece piece)
{
    // 获取该材料的排样点状态
    PackPointInfo &info = packPointInfoList[sheetID];
    int columns = info.columns;
    qreal XOffset = info.XOffset;
    qreal YOffset = info.YOffset;

    // 获取该零件的最小包络矩形
    QRectF minBoundingRect = piece.getMinBoundingRect();
    // 首先根据包络矩形，判断出排样点大致范围
    // 获取边界
    qreal minX, minY, maxX, maxY;
    getRectBoundValue(minBoundingRect, minX, minY, maxX, maxY);

    // 获取边界范围内排样点的序号
    int minRow = (minY - YOffset) / PPD + 1;  // 最小行
    int minColumn = (minX - XOffset) / PPD + 1;  // 最小列
    int maxRow = (maxY - YOffset) / PPD;  // 最大行
    int maxColumn = (maxX - XOffset) / PPD;  // 最大列

    // 记录多边形可能覆盖的排样点
    QList<int> packPointsList;
    for(int row=minRow; row<=maxRow; row++){
        for(int column=minColumn; column<=maxColumn; column++){
            int packPointID = row * columns + column;
            packPointsList.append(packPointID);
        }
    }

    // 判断这些可疑点是否被覆盖
    QMap<int, PackPoint> &packPointMap = sheetPackPointPositionMap[sheetID];  // 获取该材料的排样点状态
    foreach(int id, packPointsList){
        QPointF point = packPointMap[id].position;
        if(piece.contains(point)){
            packPointMap[id].coverd = true;
            info.coverdList.append(id);
        }
    }
}

void PackPointNestEngine::layoutAlg(QList<int> indexList)
{
    // 现写出只有一张材料时的排版算法，也不先考虑一刀切工艺
    for(int i=0; i<indexList.length(); i++){
        int index = indexList[i];
        // to do
        int typeId = nestPieceList[index].typeId;
        Piece piece = pieceList[typeId];
        packOnePiece(piece, nestPieceList[index]);
    }
}

void PackPointNestEngine::packOnePiece(Piece piece, NestEngine::NestPiece &nestPiece)
{
    /***
     * 将零件排至第i张材料上，
     * 如何材料不够排下目标零件，则判断比该零件面积小的零件是否可排下，
     * 如果可以排下，就排该零件，跳出循环，目标零件排放完毕
     * 如果面积最小的都无法排下，则继续循环至下张材料，继续排版；
     */
    for(int i=0; i<sheetList.length(); i++){
        QMap<int, PackPoint> packPointMap = sheetPackPointPositionMap[i];  // 获取该材料的排样点状态
        int PPN = packPointMap.size();  // 排样点个数
        for(int j=0; j<PPN; j++){
            for(int k=0; k<RN; k++){
                QPointF pos = packPointMap[j].position;  // 该排样点对应的位置坐标
                piece.moveTo(pos);  // 将零件最小包络矩形中心移至该位置

                qreal alpha = 2 * M_PI * k /RN;  // 旋转角度
                piece.rotate(pos, alpha);  // 绕参考点旋转alpha度
                qreal height = piece.getCenterPoint().ry();  // 得到零件形心

                /***
                 *  如果高度（重心）小于最小高度（重心）&&
                 *  目标零件完全包含在材料内 &&
                 * 目标零件不与其他已排零件重叠
                 * 则更新最优排样姿态
                 */
                if(height < minHeight
                        && piece.containsInSheet(sheetList[i])
                        && collidesWithOtherPieces(i, piece)){
                    minHeight = height;
                    nestPiece.sheetId = i;
                    nestPiece.position = pos;
                    nestPiece.alpha = alpha;
                    nestPiece.nested = true;
                }
            }
        }

        /***
         * 判断目标零件是否已排，
         * 如果已排则直接跳出循环，该零件排版成功；
         * 否则，继续循环，在下一张材料上对目标零件排版
         */
        if(nestPiece.nested){
            updatePackPoint(i, piece);  // 更新排样点状态
            break;
        }
    }
    // 重置最小高度（重心）
    minHeight = LONG_MAX;
}

bool PackPointNestEngine::collidesWithOtherPieces(int sheetID, Piece piece)
{
    // 获取该材料的排样点状态
    PackPointInfo packPointInfo = packPointInfoList[sheetID];
    QList<int> coveredList = packPointInfo.coverdList;

}








#include "nestengine.h"

NestEngine::NestEngine() :
    autoRepeatLastSheet(false)
{

}

NestEngine::NestEngine(const QList<Piece> pieceList, QList<Sheet> sheetList) :
    autoRepeatLastSheet(false)
{
    this->pieceList = getSortedPieceListByArea(pieceList);
    this->sheetList = sheetList;
    initNestPieceList();
}

NestEngine::~NestEngine()
{
    pieceList.clear();
}

QList<Piece> NestEngine::getSortedPieceListByArea(QList<Piece> pieceList)
{
    // QMap 默认按key值升序排列
    QMap<qreal, QList<int>> pieceAreaMap;
    for(int i=0; i<pieceList.length(); i++){
        Piece piece = pieceList[i];
        qreal area = piece.getArea();
        if(!pieceAreaMap.contains(area)){
            QList<int> list;
            list.append(i);
            pieceAreaMap.insert(-area, list);
        }
        pieceAreaMap[-area].append(i);
    }

    // 获取排序后的零件列表
    QList<Piece> pieceListRet;

    QMap<qreal, QList<int>>::const_iterator i;
    for(i=pieceAreaMap.constBegin(); i!=pieceAreaMap.constEnd(); ++i){
        foreach (int index, i.value()) {
            pieceListRet.append(pieceList[index]);
        }
    }
    return pieceListRet;
}

void NestEngine::initNestPieceList()
{
    int count=0;
    for(int i=0; i<pieceList.length(); i++){
        Piece piece = pieceList[i];
        for(int j=0; j<piece.getCount(); j++){
            nestPieceList.append(NestPiece(i, count++));
        }
    }
}

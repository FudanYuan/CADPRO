#include "nestengine.h"

#include <QDebug>

NestEngine::NestEngine() :
    autoRepeatLastSheet(false),
    compactStep(1),
    compactAccuracy(0.0001)
{

}

NestEngine::NestEngine(const QVector<Piece> pieceList, QVector<Sheet> sheetList) :
    autoRepeatLastSheet(false),
    compactStep(1),
    compactAccuracy(0.0001)
{
    this->pieceList = getSortedPieceListByArea(pieceList);
    this->sheetList = sheetList;
    initNestPieceList();
}

NestEngine::~NestEngine()
{
    pieceList.clear();
}

void NestEngine::setAutoRepeatLastSheet(bool flag)
{
    autoRepeatLastSheet = flag;
}

bool NestEngine::getAutoRepeatLastSheet()
{
    return autoRepeatLastSheet;
}

void NestEngine::setCompactStep(qreal step)
{
    compactStep = step;
}

qreal NestEngine::getCompactStep()
{
    return compactStep;
}

void NestEngine::setCompactAccuracy(qreal accuracy)
{
    compactAccuracy = accuracy;
}

qreal NestEngine::getCompactAccuracy()
{
    return compactAccuracy;
}

QVector<Piece> NestEngine::getSortedPieceListByArea(QVector<Piece> pieceList)
{
    // QMap 默认按key值升序排列
    QMap<qreal, QVector<int>> pieceAreaMap;
    for(int i=0; i<pieceList.length(); i++){
        Piece piece = pieceList[i];
        qreal area = piece.getArea();
        if(!pieceAreaMap.contains(area)){
            QVector<int> list;
            list.append(i);
            pieceAreaMap.insert(-area, list);
        }
        pieceAreaMap[-area].append(i);
    }

    // 获取排序后的零件列表
    QVector<Piece> pieceListRet;

    QMap<qreal, QVector<int>>::const_iterator i;
    for(i=pieceAreaMap.constBegin(); i!=pieceAreaMap.constEnd(); ++i){
        foreach (int index, i.value()) {
            pieceListRet.append(pieceList[index]);
            qDebug() << "面积：" << i.key() << ", index:" << index;
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
            nestPieceList.append(NestPiece(count++, i));
        }
    }
}

#include "nesttread.h"
#include <QDebug>

NestTread::NestTread(QObject *parent,
                     NestEngine *engine) :
    QThread(parent),
    nestEngine(engine)
{
    qDebug() << "线程初始化";
    qDebug() << nestEngine->getNestEngineType();
}

void NestTread::run()
{
    nestEngine->initNestPieceList();  // 初始化排样零件
    nestEngine->packAlg();  // 进行排版
    emit nestFinished(nestEngine);  // 将排版后的对象传回主线程
    //emit nestFinished(tr("finish"));
}

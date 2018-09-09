#ifndef NESTTREAD_H
#define NESTTREAD_H

#include <QThread>
#include "nestengine.h"
#include "packpointnestengine.h"
#include "nestengineconfigure.h"

class NestTread : public QThread
{
    Q_OBJECT
public:
    NestTread(QObject *parent, NestEngine *engine);
    void run() Q_DECL_OVERRIDE;

signals:
    void nestFinished(NestEngine *);  // 排版结束后将排版引擎对象指针返回主线程
    //void nestFinished(QString);  // 排版结束后将排版引擎对象指针返回主线程
private:
    NestEngine *nestEngine;  // 排版引擎
};

#endif // NESTTREAD_H

#ifndef BINARYFILE_H
#define BINARYFILE_H

#include <QList>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <QDebug>

template <typename T>
class BinaryFile {
public:
    explicit BinaryFile() :
        data(QList<T*>())
    {

    }

    explicit BinaryFile(QList<T*> list)
    {
        this->data.clear();
        this->data.append(list);
    }

    void write(QString outFileName)  // 写二进制文件
    {
        if(outFileName.isEmpty()){
            throw("文件名为空");
        }

        FILE *fp;
        char* out;
        QByteArray ba = outFileName.toLatin1(); // must
        out = ba.data();
        if((fp=fopen(out,"wb+"))==NULL) {
            throw("无法打开文件");
        }

        T *d;
        foreach (d, data) {
            qDebug() << d->name;
            qDebug() << d->width;
            qDebug() << d->height;
            qDebug() << d->type;
            qDebug() << d->layoutRect;
            fwrite(d, sizeof(T), 1, fp);
        }
        fclose(fp);
    }

    QList<T*> read(QString inFileName)  // 读二进制文件，返回列表
    {
        if(inFileName.isEmpty()){
            throw("文件名为空");
        }

        FILE *fp;
        char* in;
        QByteArray ba = inFileName.toLatin1(); // must
        in = ba.data();
        if((fp=fopen(in,"rb"))==NULL) {
            throw("无法打开文件");
        }

        QList<T*> retList;
        while(!feof(fp))
        {
            T *rData = new T;
            fread(rData, sizeof(T), 1, fp);
            retList.append(rData);
        }
        fclose(fp);
        data.clear();
        data.append(retList);
        return retList;
    }

    void setData(QList<T*> list){
        this->data.clear();
        this->data.append(list);
    }

    QList<T*> getData(){
        return this->data;
    }

private:
    QList<T*> data;
};
#endif // BINARYFILE_H

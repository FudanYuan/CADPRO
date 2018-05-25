#include "nestengineconfigure.h"
#include <QMessageBox>

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QList>
#include <QFile>

NestEngineConfigure::NestEngineConfigure()
{

}

QMap<int,QList<QList<int>>> NestEngineConfigure::LoadConfigureXml()
{
    QFile file(NESTENGINECONFIGURE_XML);
    QMap<int,QList<QList<int>>> readcontent;
    QList<int> configureList;
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug()<<"Read NESTENGINECONFIGURE_XML Wrong ";
        return readcontent;
    }

    QXmlStreamReader r(&file);
    while(!r.atEnd() && !r.hasError()){
        r.readNext();
        qDebug() << r.name() <<  "  " <<  r.isStartElement();
        if(r.isStartElement()){
            if(r.name() == "WholeInfo"){
                QList<QList<int>> wItem;
                while(!r.atEnd() && !r.hasError()){
                    r.readNext();
                    if(r.isStartElement()){
                        if(r.name() == "Whole"){
                            QList<int> configureList;
                            while(!r.atEnd() && !r.hasError()){
                                r.readNext();
                                if(r.name() == "maxRotate"){
                                    configureList.append(r.readElementText().toInt());
                                    qDebug() << "this name: " << r.readElementText();
                                }
                                if(r.name() == "VerticalNest"){
                                    configureList.append(r.readElementText().toInt());
                                    qDebug() << "type: " << r.readElementText();
                                }
                                if(r.name() == "HorizontalNest"){
                                    configureList.append(r.readElementText().toInt());
                                }
                                if(r.name() == "TailPieceMixing"){
                                    configureList.append(r.readElementText().toInt());
                                    qDebug() << "material: " << r.readElementText();
                                }
                                if(r.name() == "TailLineMixing"){
                                    configureList.append(r.readElementText().toInt());
                                    qDebug() << "width: " << r.readElementText();
                                }
                                if(r.name() == "SameTypeSizeMixing"){
                                    configureList.append(r.readElementText().toInt());
                                    qDebug() << "height: " << r.readElementText();
                                    //qDebug()<<"samesize_qhh"<<configureList[configureList.length()-1];
                                }
                                if(r.name() == "AllMixing"){
                                    configureList.append(r.readElementText().toInt());
                                    break;
                                    //qDebug()<<"hello_qhh"<<configureList[configureList.length()-1];
                                    qDebug() << "componentGap: " << r.readElementText();
                                }
                            }
                            wItem.append(configureList);

                        }
                    }

            }
            readcontent[0] = wItem;
            }

            if(r.name() == "StripInfo"){
                QList<QList<int>> sItem;

                while(!r.atEnd() && !r.hasError()){

                    r.readNext();
                    if(r.isStartElement()){
                        if(r.name() == "Strip"){
                            QList<int> configureList;
                            while(!r.atEnd() && !r.hasError()){
                                r.readNext();
                                if(r.name() == "���ҽ���"){
                                    configureList.append(r.readElementText().toInt());
                                    //qDebug() << "this name: " << sheet->name;
                                }
                                if(r.name() == "�����ɴ�С"){
                                    configureList.append(r.readElementText().toInt());
                                    //qDebug() << "type: " << sheet->type;
                                }
                                if(r.name() == "��������Ӧ���"){
                                    configureList.append(r.readElementText().toInt());
                                }
                                if(r.name() == "βֻ���"){
                                    configureList.append(r.readElementText().toInt());
                                    break;
                                    //qDebug() << "material: " << sheet->material;
                                }

                            }
                            sItem.append(configureList);
                        }
                    }

            }
            readcontent[1] = sItem;
            }

            if(r.name() =="PackageInfo"){
                QList<QList<int>> pItem;

                while(!r.atEnd() && !r.hasError()){

                    r.readNext();
                    if(r.isStartElement()){
                        if(r.name() == "Package"){
                            QList<int> configureList;
                            while(!r.atEnd() && !r.hasError()){
                                r.readNext();
                                if(r.name() == "�ڶ��Ƕ�"){
                                    configureList.append(r.readElementText().toInt());
                                    //qDebug() << "this name: " << sheet->name;
                                }
                                if(r.name() == "����"){
                                    configureList.append(r.readElementText().toInt());
                                    //qDebug() << "type: " << sheet->type;
                                }
                                if(r.name() == "����"){
                                    configureList.append(r.readElementText().toInt());
                                }
                                if(r.name() == "βֻ���"){
                                    configureList.append(r.readElementText().toInt());
                                    //qDebug() << "material: " << sheet->material;
                                }
                                if(r.name() == "β���Ż�"){
                                    configureList.append(r.readElementText().toInt());
                                    //qDebug() << "width: " << sheet->width;
                                }
                                if(r.name() == "ͬ�����ڳ�����"){
                                    configureList.append(r.readElementText().toInt());
                                    //qDebug() << "height: " << sheet->height;
                                    //qDebug()<<"samesize_qhh"<<configureList[configureList.length()-1];
                                }
                                if(r.name() == "ȫ���"){
                                    configureList.append(r.readElementText().toInt());
                                    //qDebug()<<"hello_qhh"<<configureList[configureList.length()-1];
                                    //qDebug() << "componentGap: " << sheet->componentGap;
                                    break;
                                }
                            }
                            pItem.append(configureList);
                        }
                    }

            }
            readcontent[2] = pItem;
            }
        }
    }
    return readcontent;
    //input for test
    foreach(int index,readcontent.keys()){
        switch(index){
        case 0:
            qDebug()<<"WholeInfo";
            QList<QList<int>> mItem;
            mItem = readcontent[index];
            foreach(QList<int> item,mItem){
                qDebug()<<"Whole";
                for(int i = 0; i<item.length(); i++){
                    //qDebug()<<item.length();
                    qDebug()<<item[i];
                }
            }
            break;
        }
    }






    //for(int i = 0; i<configureList.length(); i++){
    //qDebug()<<"hhhhh"<<configureList[i];
    //}
    // qDebug()<<"read size =="<<configureList.size();

    //Ϊ��testע�͵���
//    if(configureList[3] == 1&&configureList[4] == 0 &&configureList[5] ==0&&configureList[6]==0){
//        wholeSheetNest.mixtype = NestEngine::TailPieceMixing;
//    }
//    else if(configureList[3] == 0&&configureList[4] == 1 &&configureList[5] ==0&&configureList[6]==0){
//        wholeSheetNest.mixtype = NestEngine::TailLineMixing;
//    }
//    else if(configureList[3] == 0&&configureList[4] == 0 &&configureList[5] == 1&&configureList[6]==0){
//        wholeSheetNest.mixtype = NestEngine::SameTypeSizeMixing;
//    }
//    else if(configureList[3] == 1&&configureList[4] == 1 &&configureList[5] == 0&&configureList[6]==0){
//        wholeSheetNest.mixtype = NestEngine::TailPieceMixing|NestEngine::TailLineMixing;
//        //qDebug()<<"TTL";
//    }
//    else if(configureList[3] == 1&&configureList[4] == 0 &&configureList[5] == 1&&configureList[6]==0){
//        wholeSheetNest.mixtype = NestEngine::TailPieceMixing|NestEngine::SameTypeSizeMixing;
//    }
//    else if(configureList[3] == 1&&configureList[4] == 1 &&configureList[5] == 0&&configureList[6]==1){
//        wholeSheetNest.mixtype = NestEngine::TailLineMixing|NestEngine::SameTypeSizeMixing;
//    }
//    else if((configureList[3] == 1&&configureList[4] == 1 &&configureList[5] == 1)||configureList[6]==1){
//        wholeSheetNest.mixtype = NestEngine::AllMixing;
//        //qDebug()<<"ȫ���";
//    }
//    if(configureList[3] == 0&&configureList[4] == 0 &&configureList[5] == 0&&configureList[6]==0){
//        wholeSheetNest.mixtype = NestEngine::NoMixing;
//        //qDebug()<<"No mixing in read";
//    }

    //����

//    while(!r.atEnd() && !r.hasError()){
//        r.readNext();
//        if(r.isStartElement()){
//            if(r.name() == "Strip"){

//                while(!r.atEnd() && !r.hasError()){
//                    r.readNext();
//                    if(r.name() == "���ҽ���"){
//                        configureList.append(r.readElementText().toInt());
//                        //qDebug() << "this name: " << sheet->name;
//                    }
//                    if(r.name() == "�����ɴ�С"){
//                        configureList.append(r.readElementText().toInt());
//                        //qDebug() << "type: " << sheet->type;
//                    }
//                    if(r.name() == "��������Ӧ���"){
//                        configureList.append(r.readElementText().toInt());
//                        //qDebug() << "material: " << sheet->material;
//                    }
//                    if(r.name() == "βֻ���"){
//                        configureList.append(r.readElementText().toInt());
//                        //qDebug() << "material: " << sheet->material;
//                    }

//                }
//            }
//        }
//    }
//    if(configureList[0] == 1 &&configureList[1] == 0){
//        stripSheetNest.strategy = NestEngine::LeftRightTurn;
//    }
//    if(configureList[0] == 0 &&configureList[1] == 1){
//        stripSheetNest.strategy = NestEngine::SizeDown;
//    }
//    if(configureList[0] == 1 &&configureList[1] == 1){
//        stripSheetNest.strategy = NestEngine::AllStrategys;
//    }

//    //��װ
//    while(!r.atEnd() && !r.hasError()){
//        r.readNext();
//        if(r.isStartElement()){
//            if(r.name() == "Package"){

//                while(!r.atEnd() && !r.hasError()){
//                    r.readNext();
//                    if(r.name() == "�ڶ��Ƕ�"){
//                        configureList.append(r.readElementText().toInt());
//                        //qDebug() << "this name: " << sheet->name;
//                    }
//                    if(r.name() == "����"){
//                        configureList.append(r.readElementText().toInt());
//                        //qDebug() << "type: " << sheet->type;
//                    }
//                    if(r.name() == "βֻ���"){
//                        configureList.append(r.readElementText().toInt());
//                        //qDebug() << "material: " << sheet->material;
//                    }
//                    if(r.name() == "β���Ż�"){
//                        configureList.append(r.readElementText().toInt());
//                        //qDebug() << "width: " << sheet->width;
//                    }
//                    if(r.name() == "ͬ�����ڳ�����"){
//                        configureList.append(r.readElementText().toInt());
//                        //qDebug() << "height: " << sheet->height;
//                    }
//                    if(r.name() == "ȫ���"){
//                        configureList.append(r.readElementText().toInt());
//                        //qDebug() << "componentGap: " << sheet->componentGap;
//                    }
//                }
//            }
//        }
//    }


}

void NestEngineConfigure::WriteConfigureXml(QMap<int,QList<QList<int>>> writedata)
{
    QFile file(NESTENGINECONFIGURE_XML);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug()<<"write error";
        return;
    }

    QXmlStreamWriter w(&file);
    w.setAutoFormattingIndent(8);
    w.setAutoFormatting(true);
    w.writeStartDocument();

    foreach(int index , writedata.keys()){
       // qDebug()<<"index"<<index;
    switch (index) {
    case  0:    //����
    {
        // ��sheet��Ϣд��xml�ļ�
        w.writeStartElement("WholeInfo");
        for(int i=0; i< writedata[index].length();i++){
            w.writeStartElement("Whole");
            w.writeTextElement("maxRotate", QString("%1").arg(writedata[index][i][0]));
            w.writeTextElement("VerticalNest", QString("%1").arg(writedata[index][i][1]));
            w.writeTextElement("HorizontalNest", QString("%1").arg(writedata[index][i][2]));
            w.writeTextElement("TailPieceMixing", QString("%1").arg(writedata[index][i][3]));
            w.writeTextElement("TailLineMixing", QString("%1").arg(writedata[index][i][4]));
            w.writeTextElement("SameTypeSizeMixing", QString("%1").arg(writedata[index][i][5]));
            w.writeTextElement("AllMixing", QString("%1").arg(writedata[index][i][6]));
            w.writeEndElement();
        }
        w.writeEndElement();
        break;
    }
    case 1://����
    {
        w.writeStartElement("StripInfo");
        for(int i=0; i< writedata[index].length();i++){
            w.writeStartElement("Strip");
            w.writeTextElement("���ҽ���", QString("%1").arg(writedata[index][i][0]));
            w.writeTextElement("�����ɴ�С", QString("%1").arg(writedata[index][i][1]));
            w.writeTextElement("��������Ӧ���", QString("%1").arg(writedata[index][i][2]));
            w.writeTextElement("βֻ���", QString("%1").arg(writedata[index][i][3]));
            w.writeEndElement();
        }
        w.writeEndElement();
        break;}
    case 2://��װ
    {
        w.writeStartElement("PackageInfo");

        for(int i=0; i< writedata[index].length();i++){
            w.writeStartElement("Package");
            //qDebug()<<"hellwwo"<<writedata[index].length();
            w.writeTextElement("�ڶ��Ƕ�", QString("%1").arg(writedata[index][i][0]));
            w.writeTextElement("����", QString("%1").arg(writedata[index][i][1]));
            w.writeTextElement("����", QString("%1").arg(writedata[index][i][2]));
            w.writeTextElement("βֻ���", QString("%1").arg(writedata[index][i][3]));
            w.writeTextElement("β���Ż�", QString("%1").arg(writedata[index][i][4]));
            w.writeTextElement("ͬ�����ڳ�����", QString("%1").arg(writedata[index][i][5]));
            w.writeTextElement("ȫ���", QString("%1").arg(writedata[index][i][6]));
            w.writeEndElement();
        }
        w.writeEndElement();
        break;
}
    default:
        break;
    }

}
    w.writeEndDocument();

    file.close();
}


NestEngineConfigure::WholeSheetNest NestEngineConfigure::getWholeSheetNest()
{
    return wholeSheetNest;
}

NestEngineConfigure::StripSheetNest NestEngineConfigure::getStripSheetNest()
{
    return stripSheetNest;
}

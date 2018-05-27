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
    QList<QList<int>> wItem;
    QList<QList<int>> sItem;
    QList<QList<int>> pItem;


    while(!r.atEnd() && !r.hasError()){
        r.readNext();
        qDebug() << r.name() <<  "name 1 " <<  r.isStartElement();
        if(r.isStartElement()){
            if(r.name() == "Whole"){
                QList<int> configureList;
                while(!r.atEnd() && !r.hasError()){
                    r.readNext();
                    if(r.name() == "maxRotate"){
                        configureList.append(r.readElementText().toInt());
                                    //qDebug() << "this name: " << r.readElementText();
                    }
                    if(r.name() == "VerticalNest"){
                        configureList.append(r.readElementText().toInt());
                                    //qDebug() << "type: " << r.readElementText();
                    }
                    if(r.name() == "HorizontalNest"){
                        configureList.append(r.readElementText().toInt());
                    }
                    if(r.name() == "TailPieceMixing"){
                        configureList.append(r.readElementText().toInt());
                                    //qDebug() << "material: " << r.readElementText();
                    }
                    if(r.name() == "TailLineMixing"){
                        configureList.append(r.readElementText().toInt());
                                    //qDebug() << "width: " << r.readElementText();
                    }
                    if(r.name() == "SameTypeSizeMixing"){
                        configureList.append(r.readElementText().toInt());
                                    //qDebug() << "height: " << r.readElementText();
                                    //qDebug()<<"samesize_qhh"<<configureList[configureList.length()-1];
                    }
                    if(r.name() == "AllMixing"){
                        configureList.append(r.readElementText().toInt());
                        wItem.append(configureList);
                        break;
                                    //qDebug()<<"hello_qhh"<<configureList[configureList.length()-1];
                                    //qDebug() << "componentGap: " << r.readElementText();
                    }
                }
            }

            if(r.name() == "Strip"){
                QList<int> configureList;
                while(!r.atEnd() && !r.hasError()){
                    r.readNext();
                    if(r.name() == "LeftRightTurn"){
                        configureList.append(r.readElementText().toInt());
                                    //qDebug() << "this name: " << r.readElementText();
                    }
                    if(r.name() == "SizeDown"){
                        configureList.append(r.readElementText().toInt());
                                    //qDebug() << "type: " << r.readElementText();
                    }
                    if(r.name() == "HorizontaoSpace"){
                        configureList.append(r.readElementText().toInt());
                    }
                    if(r.name() == "TailPieceMixing"){
                        configureList.append(r.readElementText().toInt());
                        sItem.append(configureList);
                        break;
                                    //qDebug() << "material: " << r.readElementText();
                    }
                }
            }

            if(r.name() == "Package"){
                QList<int> configureList;
                while(!r.atEnd() && !r.hasError()){
                    r.readNext();
                    if(r.name() == "maxRotate"){
                        configureList.append(r.readElementText().toInt());
                                    //qDebug() << "this name: " << r.readElementText();
                    }
                    if(r.name() == "VerticalNest"){
                        configureList.append(r.readElementText().toInt());
                                    //qDebug() << "type: " << r.readElementText();
                    }
                    if(r.name() == "HorizontalNest"){
                        configureList.append(r.readElementText().toInt());
                    }
                    if(r.name() == "TailPieceMixing"){
                        configureList.append(r.readElementText().toInt());
                                    //qDebug() << "material: " << r.readElementText();
                    }
                    if(r.name() == "TailLineMixing"){
                        configureList.append(r.readElementText().toInt());
                                    //qDebug() << "width: " << r.readElementText();
                    }
                    if(r.name() == "SameTypeSizeMixing"){
                        configureList.append(r.readElementText().toInt());
                                    //qDebug() << "height: " << r.readElementText();
                                    //qDebug()<<"samesize_qhh"<<configureList[configureList.length()-1];
                    }
                    if(r.name() == "AllMixing"){
                        configureList.append(r.readElementText().toInt());
                        pItem.append(configureList);
                        break;
                    }
                }
            }
        }
    }
    file.close();
    readcontent.insert(0,wItem);
    readcontent.insert(1,sItem);
    readcontent.insert(2,pItem);

    return readcontent;
}






    //for(int i = 0; i<configureList.length(); i++){
    //qDebug()<<"hhhhh"<<configureList[i];
    //}
    // qDebug()<<"read size =="<<configureList.size();

    //???test??????
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
//        //qDebug()<<"?????";
//    }
//    if(configureList[3] == 0&&configureList[4] == 0 &&configureList[5] == 0&&configureList[6]==0){
//        wholeSheetNest.mixtype = NestEngine::NoMixing;
//        //qDebug()<<"No mixing in read";
//    }

    //????

//    while(!r.atEnd() && !r.hasError()){
//        r.readNext();
//        if(r.isStartElement()){
//            if(r.name() == "Strip"){

//                while(!r.atEnd() && !r.hasError()){
//                    r.readNext();
//                    if(r.name() == "???????"){
//                        configureList.append(r.readElementText().toInt());
//                        //qDebug() << "this name: " << sheet->name;
//                    }
//                    if(r.name() == "???????????"){
//                        configureList.append(r.readElementText().toInt());
//                        //qDebug() << "type: " << sheet->type;
//                    }
//                    if(r.name() == "?????????????"){
//                        configureList.append(r.readElementText().toInt());
//                        //qDebug() << "material: " << sheet->material;
//                    }
//                    if(r.name() == "???????"){
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

//    //???
//    while(!r.atEnd() && !r.hasError()){
//        r.readNext();
//        if(r.isStartElement()){
//            if(r.name() == "Package"){

//                while(!r.atEnd() && !r.hasError()){
//                    r.readNext();
//                    if(r.name() == "??????"){
//                        configureList.append(r.readElementText().toInt());
//                        //qDebug() << "this name: " << sheet->name;
//                    }
//                    if(r.name() == "????"){
//                        configureList.append(r.readElementText().toInt());
//                        //qDebug() << "type: " << sheet->type;
//                    }
//                    if(r.name() == "???????"){
//                        configureList.append(r.readElementText().toInt());
//                        //qDebug() << "material: " << sheet->material;
//                    }
//                    if(r.name() == "???????"){
//                        configureList.append(r.readElementText().toInt());
//                        //qDebug() << "width: " << sheet->width;
//                    }
//                    if(r.name() == "??????????????"){
//                        configureList.append(r.readElementText().toInt());
//                        //qDebug() << "height: " << sheet->height;
//                    }
//                    if(r.name() == "?????"){
//                        configureList.append(r.readElementText().toInt());
//                        //qDebug() << "componentGap: " << sheet->componentGap;
//                    }
//                }
//            }
//        }
//    }


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

    w.writeStartElement("Info");
    foreach(int index , writedata.keys()){
       // qDebug()<<"index"<<index;
    switch (index) {
    case  0:    //整体
    {
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
        break;
    }
    case 1://样条
    {
        for(int i=0; i< writedata[index].length();i++){
            w.writeStartElement("Strip");
            w.writeTextElement("LeftRightTurn", QString("%1").arg(writedata[index][i][0]));
            w.writeTextElement("SizeDown", QString("%1").arg(writedata[index][i][1]));
            w.writeTextElement("HorizontaoSpace", QString("%1").arg(writedata[index][i][2]));
            w.writeTextElement("TailPieceMixing", QString("%1").arg(writedata[index][i][3]));
            w.writeEndElement();
        }
        break;}
    case 2://卷装
    {

        for(int i=0; i< writedata[index].length();i++){
            w.writeStartElement("Package");
            //qDebug()<<"hellwwo"<<writedata[index].length();
            w.writeTextElement("maxRotate", QString("%1").arg(writedata[index][i][0]));
            w.writeTextElement("VerticalNest", QString("%1").arg(writedata[index][i][1]));
            w.writeTextElement("HorizontalNest", QString("%1").arg(writedata[index][i][2]));
            w.writeTextElement("TailPieceMixing", QString("%1").arg(writedata[index][i][3]));
            w.writeTextElement("TailLineMixing", QString("%1").arg(writedata[index][i][4]));
            w.writeTextElement("SameTypeSizeMixing", QString("%1").arg(writedata[index][i][5]));
            w.writeTextElement("AllMixing", QString("%1").arg(writedata[index][i][6]));
            w.writeEndElement();
        }
        break;
}
    default:
        break;
    }

}
    w.writeEndElement();
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
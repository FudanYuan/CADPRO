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
    QMap<int,QList<QList<int>>> readcontent ;
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


void NestEngineConfigure::WriteConfigureXml(QMap<int,QList<QList<int>>> & writedata)
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
        case  0:    // whole
        {
            for(int i=0; i< writedata[0].length();i++){
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
        case 1://ÑùÌõ
        {
            for(int i=0; i< writedata[1].length();i++){
                w.writeStartElement("Strip");
                w.writeTextElement("LeftRightTurn", QString("%1").arg(writedata[index][i][0]));
                w.writeTextElement("SizeDown", QString("%1").arg(writedata[index][i][1]));
                w.writeTextElement("HorizontaoSpace", QString("%1").arg(writedata[index][i][2]));
                w.writeTextElement("TailPieceMixing", QString("%1").arg(writedata[index][i][3]));
                w.writeEndElement();
            }
            break;}
        case 2://¾í×°
        {

            for(int i=0; i< writedata[2].length();i++){
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

void NestEngineConfigure::setWholeSheetNest(NestEngineConfigure::WholeSheetNest wholeNest)
{
    wholeSheetNest = wholeNest;
}


NestEngineConfigure::WholeSheetNest NestEngineConfigure::getWholeSheetNest()
{
    return wholeSheetNest;
}

void NestEngineConfigure::setStripSheetNest(NestEngineConfigure::StripSheetNest stripNest)
{
    stripSheetNest = stripNest;
}

NestEngineConfigure::StripSheetNest NestEngineConfigure::getStripSheetNest()
{
    return stripSheetNest;
}

void NestEngineConfigure::setPackageSheetNest(NestEngineConfigure::PackageSheetNest packageNest)
{
    packageSheetNest = packageNest;
}

NestEngineConfigure::PackageSheetNest NestEngineConfigure::getPackageSheetNest()
{
    return packageSheetNest;
}

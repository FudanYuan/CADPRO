#include "nestengineconfigure.h"
#include <QMessageBox>

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QList>
#include <QFile>

NestEngineConfigure::NestEngineConfigure()
{

}

void NestEngineConfigure::LoadConfigureXml(int index)
{
    QFile file(NEST_ENGINE_CONFIGURE_XML);
    QList<int> configureList;
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug()<<"Read NEST_ENGINE_CONFIGURE_XML Wrong ";
        return;
    }
    QXmlStreamReader r(&file);
    switch (index) {
    case 0://����
        while(!r.atEnd() && !r.hasError()){
               r.readNext();
               if(r.isStartElement()){
                   if(r.name() == "Whole"){
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
                           }
                       }
                   }
               }
        }
        //for(int i = 0; i<configureList.length(); i++){
            //qDebug()<<"hhhhh"<<configureList[i];
        //}
       // qDebug()<<"read size =="<<configureList.size();
        if(configureList[3] == 1&&configureList[4] == 0 &&configureList[5] ==0&&configureList[6]==0){
            wholeSheetNest.mixtype = NestEngine::TailPieceMixing;
        }
        else if(configureList[3] == 0&&configureList[4] == 1 &&configureList[5] ==0&&configureList[6]==0){
            wholeSheetNest.mixtype = NestEngine::TailLineMixing;
        }
        else if(configureList[3] == 0&&configureList[4] == 0 &&configureList[5] == 1&&configureList[6]==0){
            wholeSheetNest.mixtype = NestEngine::SameTypeSizeMixing;
        }
        else if(configureList[3] == 1&&configureList[4] == 1 &&configureList[5] == 0&&configureList[6]==0){
            wholeSheetNest.mixtype = NestEngine::TailPieceMixing|NestEngine::TailLineMixing;
            //qDebug()<<"TTL";
        }
        else if(configureList[3] == 1&&configureList[4] == 0 &&configureList[5] == 1&&configureList[6]==0){
            wholeSheetNest.mixtype = NestEngine::TailPieceMixing|NestEngine::SameTypeSizeMixing;
        }
        else if(configureList[3] == 1&&configureList[4] == 1 &&configureList[5] == 0&&configureList[6]==1){
            wholeSheetNest.mixtype = NestEngine::TailLineMixing|NestEngine::SameTypeSizeMixing;
        }
        else if((configureList[3] == 1&&configureList[4] == 1 &&configureList[5] == 1)||configureList[6]==1){
            wholeSheetNest.mixtype = NestEngine::AllMixing;
            //qDebug()<<"ȫ���";
        }
        if(configureList[3] == 0&&configureList[4] == 0 &&configureList[5] == 0&&configureList[6]==0){
            wholeSheetNest.mixtype = NestEngine::NoMixing;
            //qDebug()<<"No mixing in read";
        }

        break;
    case 1://����
        while(!r.atEnd() && !r.hasError()){
               r.readNext();
               if(r.isStartElement()){
                   if(r.name() == "Strip"){

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
                               //qDebug() << "material: " << sheet->material;
                           }
                           if(r.name() == "βֻ���"){
                               configureList.append(r.readElementText().toInt());
                               //qDebug() << "material: " << sheet->material;
                           }

                       }
                   }
               }
        }
        if(configureList[0] == 1 &&configureList[1] == 0){
            stripSheetNest.strategy = NestEngine::LeftRightTurn;
        }
        if(configureList[0] == 0 &&configureList[1] == 1){
            stripSheetNest.strategy = NestEngine::SizeDown;
        }
        if(configureList[0] == 1 &&configureList[1] == 1){
            stripSheetNest.strategy = NestEngine::AllStrategys;
        }
        break;
    case 2://��װ
        while(!r.atEnd() && !r.hasError()){
               r.readNext();
               if(r.isStartElement()){
                   if(r.name() == "Package"){

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
                           }
                           if(r.name() == "ȫ���"){
                               configureList.append(r.readElementText().toInt());
                               //qDebug() << "componentGap: " << sheet->componentGap;
                           }
                       }
                   }
               }
        }
        break;
    default:
        break;
    }


}

void NestEngineConfigure::WriteConfigureXml(int index,QList<int> configurelist)
{
    //qDebug()<<"hello_world";
    QFile file(NEST_ENGINE_CONFIGURE_XML);
    if (!file.open(QIODevice::WriteOnly)) {
        //QMessageBox::warning(this, tr("����"), tr("������Ϣ�ļ��޷�������!"));
        qDebug()<<"hello_cad";
        return;
    }
   /* for(int i = 0; i<configurelist.length(); i++){
        qDebug()<<configurelist[i];
    }*/
    QXmlStreamWriter w(&file);
    w.setAutoFormattingIndent(8);
    w.setAutoFormatting(true);
    w.writeStartDocument();

    switch (index) {
    case  0://����
        // ��sheet��Ϣд��xml�ļ�
         w.writeStartElement("WholeInfo");
         for(int i=0; i< 1/*configurelist.length()*/;i++){
             w.writeStartElement("Whole");
             w.writeTextElement("�ڶ��Ƕ�", QString("%1").arg(configurelist[0]));
             w.writeTextElement("����", QString("%1").arg(configurelist[1]));
             w.writeTextElement("����", QString("%1").arg(configurelist[2]));
             w.writeTextElement("βֻ���", QString("%1").arg(configurelist[3]));
             w.writeTextElement("β���Ż�", QString("%1").arg(configurelist[4]));
             w.writeTextElement("ͬ�����ڳ�����", QString("%1").arg(configurelist[5]));
             w.writeTextElement("ȫ���", QString("%1").arg(configurelist[6]));
             w.writeEndElement();
         }
         w.writeEndElement();

         w.writeEndDocument();

         file.close();
        break;
    case 1://����

    default:
    case 2://��װ

        break;
    }
}


NestEngineConfigure::WholeSheetNest NestEngineConfigure::getWholeSheetNest()
{
    return wholeSheetNest;
}

NestEngineConfigure::StripSheetNest NestEngineConfigure::getStripSheetNest()
{
    return stripSheetNest;
}

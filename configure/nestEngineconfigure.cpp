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
    QFile file(NESTENGINECONFIGURE_XML);
    QList<int> configureList;
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        //QMessageBox::warning(this, tr("警告"), tr("材料信息文件无法正常打开!"));
        //qDebug()<<"Read NESTENGINECONFIGURE_XML Wrong ";
        return;
    }
    QXmlStreamReader r(&file);
    switch (index) {
    case 0://整体
        while(!r.atEnd() && !r.hasError()){
               r.readNext();
               if(r.isStartElement()){
                   if(r.name() == "Whole"){

                       while(!r.atEnd() && !r.hasError()){
                           r.readNext();
                           if(r.name() == "摆动角度"){
                               configureList.append(r.readElementText().toInt());
                               //qDebug() << "this name: " << sheet->name;
                           }
                           if(r.name() == "纵向"){
                               configureList.append(r.readElementText().toInt());
                               //qDebug() << "type: " << sheet->type;
                           }
                           if(r.name() == "横向"){
                               configureList.append(r.readElementText().toInt());
                           }
                           if(r.name() == "尾只混合"){
                               configureList.append(r.readElementText().toInt());
                               //qDebug() << "material: " << sheet->material;
                           }
                           if(r.name() == "尾行优化"){
                               configureList.append(r.readElementText().toInt());
                               //qDebug() << "width: " << sheet->width;
                           }
                           if(r.name() == "同型体内尺码混合"){
                               configureList.append(r.readElementText().toInt());
                               //qDebug() << "height: " << sheet->height;
                               qDebug()<<"samesize_qhh"<<configureList[configureList.length()-1];
                           }
                           if(r.name() == "全混合"){
                               configureList.append(r.readElementText().toInt());
                               qDebug()<<"hello_qhh"<<configureList[configureList.length()-1];
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
            qDebug()<<"TTL";
        }
        else if(configureList[3] == 1&&configureList[4] == 0 &&configureList[5] == 1&&configureList[6]==0){
            wholeSheetNest.mixtype = NestEngine::TailPieceMixing|NestEngine::SameTypeSizeMixing;
        }
        else if(configureList[3] == 1&&configureList[4] == 1 &&configureList[5] == 0&&configureList[6]==1){
            wholeSheetNest.mixtype = NestEngine::TailLineMixing|NestEngine::SameTypeSizeMixing;
        }
        else if((configureList[3] == 1&&configureList[4] == 1 &&configureList[5] == 1)||configureList[6]==1){
            wholeSheetNest.mixtype = NestEngine::AllMixing;
            qDebug()<<"全混合";
        }
        if(configureList[3] == 0&&configureList[4] == 0 &&configureList[5] == 0&&configureList[6]==0){
            wholeSheetNest.mixtype = NestEngine::NoMixing;
            qDebug()<<"No mixing in read";
        }

        break;
    case 1://样条
        while(!r.atEnd() && !r.hasError()){
               r.readNext();
               if(r.isStartElement()){
                   if(r.name() == "Strip"){

                       while(!r.atEnd() && !r.hasError()){
                           r.readNext();
                           if(r.name() == "左右交替"){
                               configureList.append(r.readElementText().toInt());
                               //qDebug() << "this name: " << sheet->name;
                           }
                           if(r.name() == "尺码由大到小"){
                               configureList.append(r.readElementText().toInt());
                               //qDebug() << "type: " << sheet->type;
                           }
                           if(r.name() == "横向自适应间距"){
                               configureList.append(r.readElementText().toInt());
                               //qDebug() << "material: " << sheet->material;
                           }
                           if(r.name() == "尾只混合"){
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
    case 2://卷装
        while(!r.atEnd() && !r.hasError()){
               r.readNext();
               if(r.isStartElement()){
                   if(r.name() == "Package"){

                       while(!r.atEnd() && !r.hasError()){
                           r.readNext();
                           if(r.name() == "摆动角度"){
                               configureList.append(r.readElementText().toInt());
                               //qDebug() << "this name: " << sheet->name;
                           }
                           if(r.name() == "纵向"){
                               configureList.append(r.readElementText().toInt());
                               //qDebug() << "type: " << sheet->type;
                           }
                           if(r.name() == "尾只混合"){
                               configureList.append(r.readElementText().toInt());
                               //qDebug() << "material: " << sheet->material;
                           }
                           if(r.name() == "尾行优化"){
                               configureList.append(r.readElementText().toInt());
                               //qDebug() << "width: " << sheet->width;
                           }
                           if(r.name() == "同型体内尺码混合"){
                               configureList.append(r.readElementText().toInt());
                               //qDebug() << "height: " << sheet->height;
                           }
                           if(r.name() == "全混合"){
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
    QFile file(NESTENGINECONFIGURE_XML);
    if (!file.open(QIODevice::WriteOnly)) {
        //QMessageBox::warning(this, tr("警告"), tr("材料信息文件无法正常打开!"));
        //qDebug()<<"hello_cad";
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
    case  0://整体
        // 将sheet信息写入xml文件
         w.writeStartElement("WholeInfo");
         for(int i=0; i< 1/*configurelist.length()*/;i++){
             w.writeStartElement("Whole");
             w.writeTextElement("摆动角度", QString("%1").arg(configurelist[0]));
             w.writeTextElement("纵向", QString("%1").arg(configurelist[1]));
             w.writeTextElement("横向", QString("%1").arg(configurelist[2]));
             w.writeTextElement("尾只混合", QString("%1").arg(configurelist[3]));
             w.writeTextElement("尾行优化", QString("%1").arg(configurelist[4]));
             w.writeTextElement("同型体内尺码混合", QString("%1").arg(configurelist[5]));
             w.writeTextElement("全混合", QString("%1").arg(configurelist[6]));
             w.writeEndElement();
         }
         w.writeEndElement();

         w.writeEndDocument();

         file.close();
        break;
    case 1://条板

    default:
    case 2://卷装

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

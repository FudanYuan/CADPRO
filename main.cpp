#include <QApplication>
#include "sketch.h"
#include "nest.h"
#include <QDebug>
#include <sys/time.h>

using namespace std;

//Round(1.234,2) = 1.23
//Round(1.234,0) = 1.0
//Round(123.4,-1) = 120.0

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("Jeremy");
    QApplication::setApplicationName("CADPRO");
    qsrand(time(0));

    // 记录程序运行时间
    struct timeval tpstart,tpend;
    float timeuse;
    gettimeofday(&tpstart,NULL);

    Nest w;// Nest w; Sketch w;
    w.show();
//    qDebug() << Round(1.234,2); // = 1.23
//    qDebug() << Round(1.234,0); // = 1.0
//    qDebug() << Round(123.4,-1); // = 120.0

    gettimeofday(&tpend,NULL);
    timeuse=(1000000*(tpend.tv_sec-tpstart.tv_sec) + tpend.tv_usec-tpstart.tv_usec)/1000000.0;
    qDebug()<< "程序运行时间：" << timeuse << "s";

    return a.exec();

#if 0

    void visit(BBSTNode<int*> *p)
    {
        int *b = p->data;
        qDebug() << *b;
    }

    RectNestEngine::EmptyRectArea m(0,0,100,100);
    RectNestEngine::updateEmptyRectArea(&m);

    for(int i=0; i<RectNestEngine::emptyRectArea.length();i++){
        qDebug() << RectNestEngine::emptyRectArea[i].leftBottom() << "  " << RectNestEngine::emptyRectArea[i].rightTop();
    }

    QList<RectNestEngine::MinRect> rectList;
    RectNestEngine::MinRect rect1(50, 10, 1, false);
    RectNestEngine::MinRect rect2(60, 20, 2, false);
    RectNestEngine::MinRect rect3(40, 30, 3, true);
    rectList.append(rect1);
    rectList.append(rect2);
    rectList.append(rect3);

    RectNestEngine::minRectsArea = 360;
    for(int i=0; i<rectList.length();i++){
        RectNestEngine::MinRect *currentRect = &rectList[i];
        double rectWidth = currentRect->getWidth();  // 得到矩形宽度
        double rectHight = currentRect->getHeight();  // 得到矩形长度
        qDebug() << "排放第" << i << "个矩形" << "  " << rectWidth << " " << rectHight;
        for(int j=0; j<RectNestEngine::emptyRectArea.length();j++){
            RectNestEngine::EmptyRectArea lowestRect = RectNestEngine::emptyRectArea[j];
            if(lowestRect.width() >= rectWidth && lowestRect.height() >= rectHight){
                // 如果该线段的宽度大于排入零件的宽度，则可排放该零件
                // 将该零件排放在此位置
                currentRect->position = lowestRect.leftBottom();
                RectNestEngine::updateEmptyRectArea(new RectNestEngine::EmptyRectArea(*currentRect));
                break;
            }
        }
    }
    qDebug() << "";
    qDebug() << "-----------剩余矩形集合--------";
    for(int i=0; i<RectNestEngine::emptyRectArea.length();i++){
        qDebug() << RectNestEngine::emptyRectArea[i].leftBottom() << "  " << RectNestEngine::emptyRectArea[i].rightTop();
    }
    qDebug() << "-----------剩余矩形集合--------";
    qDebug() << "";
    for(int i=0; i<rectList.length();i++){
        int index = rectList[i].index;
        qDebug() << index << "号矩形" << "第" << i << "个排放,  排放位置：" << rectList[i].position << "  ，旋转" << rectList[i].rotate;
    }
#endif
}

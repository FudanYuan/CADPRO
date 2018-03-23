#include <QApplication>
#include "sketch.h"
 #include "nest.h"
#include <QDebug>
#include "binarytree.h"
#include <sys/time.h>

using namespace std;

void visit(BBSTNode<int*> *p)
{
    int *b = p->data;
    qDebug() << *b;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("Jeremy");
    QApplication::setApplicationName("CADPRO");
    qsrand(time(0));

    struct timeval tpstart,tpend;
    float timeuse;

    gettimeofday(&tpstart,NULL);

    Sketch w; // Sketch w;
    w.show();

    gettimeofday(&tpend,NULL);
    timeuse=(1000000*(tpend.tv_sec-tpstart.tv_sec) + tpend.tv_usec-tpstart.tv_usec)/1000000.0;

    qDebug()<< "程序运行时间：" << timeuse << "s";
    return a.exec();

#if 0
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
    return a.exec();
#endif

/* //    CAVLTree<RectNestEngine::BaseLine*> *CAVLTreeObj1 = new CAVLTree<RectNestEngine::BaseLine*>();
//    RectNestEngine::BaseLine bLine[5];
//    for(int i = 1; i <= 5; i++){
//        int px1 = 0;
//        int px2 = i * 10;
//        int py = i * 10;
//        RectNestEngine::BaseLine b(px1, px2, py);
//        bLine[i-1] = b;
//    }

//    CAVLTree<int *> *CAVLTreeObj1 = new CAVLTree<int*>();
//    int bLine[1000];
//    for(int i = 1; i <= 1000; i++){
//        int h = i;
//        bLine[i-1] = h;
//    }
//    qDebug() << "";
//    CAVLTreeObj1->createAVLTree(bLine, 1000);
//    qDebug() << "AVL Tree中序遍历结果：";
//    CAVLTreeObj1->inOrderTraverse(visit, CAVLTreeObj1->root);
//    qDebug() << endl;

//    qDebug() << "AVL Tree先序遍历结果：";
//    CAVLTreeObj1->preOrderTraverse(visit, CAVLTreeObj1->root);
//    qDebug() << endl;

//    for(int i = 10; i <= 39; i++){
//        CAVLTreeObj1->root = CAVLTreeObj1->deleteNode(CAVLTreeObj1->root, &i);
//        CAVLTreeObj1->root = CAVLTreeObj1->insertNode(CAVLTreeObj1->root, &i);
//    }

//    qDebug() << "AVL Tree中序遍历结果：";
//    CAVLTreeObj1->inOrderTraverse(visit, CAVLTreeObj1->root);
//    qDebug() << endl;


//    return a.exec();

//    RectNestEngine::BaseLine b2(0, 10, 50);
//    //RectNestEngine::updateHightProfileLine(&b2);

//    BBSTNode<RectNestEngine::BaseLine *> *p = CAVLTreeObj1->searchNode(CAVLTreeObj1->root, &b2);
//    if(p == NULL){
//        qDebug() << "不存在该线集";
//    } else{
//        RectNestEngine::BaseLine *pLink = p->data;
//        qDebug() << "存在要插入节点, 其头节点为：" << pLink->line() << "  长度" << pLink->width();
//        // 首先在二叉树中先删除该节点
//        CAVLTreeObj1->root = CAVLTreeObj1->deleteNode(CAVLTreeObj1->root, p->data);
//        qDebug() << "删除后先序遍历：" << endl;
//        CAVLTreeObj1->preOrderTraverse(visit, CAVLTreeObj1->root);

//        qDebug() << pLink->line() << " 宽度：" << pLink->width() << " 高度： " << pLink->py;

//        pLink = pLink->insertBaseLine(pLink, b2);
//        qDebug() << "更新后线集";
//        qDebug() << pLink->line() << "  宽度：" << pLink->width() << " 高度： " << pLink->py;

//        RectNestEngine::BaseLine *b = pLink;
//        qDebug() << "遍历链表：";
//        while(b){
//            qDebug() << "1线集： " << b->line() << "  宽度：" << b->width() << " 高度： " << b->py;
//            b = b->next;
//        }
//        CAVLTreeObj1->root = CAVLTreeObj1->insertNode(CAVLTreeObj1->root, pLink);
//    }

//    qDebug() << "添加后中序遍历：";
//    CAVLTreeObj1->inOrderTraverse(visit, CAVLTreeObj1->root);
//    qDebug() << endl;

//    RectNestEngine::BaseLine b3(5, 50, 50);
//    ///RectNestEngine::updateHightProfileLine(&b3);

//    p = CAVLTreeObj1->searchNode(CAVLTreeObj1->root, &b3);
//    if(p == NULL){
//        qDebug() << "不存在该线集";
//    } else{
//        RectNestEngine::BaseLine *pLink = p->data;
//        qDebug() << "存在要插入节点, 其头节点为：" << pLink->line() << "  长度" << pLink->width();
//        // 首先在二叉树中先删除该节点
//        CAVLTreeObj1->root = CAVLTreeObj1->deleteNode(CAVLTreeObj1->root, p->data);
//        qDebug() << "删除后先序遍历：" << endl;
//        CAVLTreeObj1->preOrderTraverse(visit, CAVLTreeObj1->root);

//        qDebug() << pLink->line() << " 宽度：" << pLink->width() << " 高度： " << pLink->py;

//        pLink = pLink->insertBaseLine(pLink, b3);
//        qDebug() << "更新后线集";
//        qDebug() << pLink->line() << "  宽度：" << pLink->width() << " 高度： " << pLink->py;

//        RectNestEngine::BaseLine *b = pLink;
//        qDebug() << "遍历链表：";
//        while(b){
//            qDebug() << "1线集： " << b->line() << "  宽度：" << b->width() << " 高度： " << b->py;
//            b = b->next;
//        }
//        CAVLTreeObj1->root = CAVLTreeObj1->insertNode(CAVLTreeObj1->root, pLink);
//    }

//    qDebug() << "3添加后中序遍历：";
//    CAVLTreeObj1->inOrderTraverse(visit, CAVLTreeObj1->root);
//    qDebug() << endl;

//    return a.exec();
//    RectNestEngine::BaseLine b4(0, 5, 30);
//    ///RectNestEngine::updateHightProfileLine(&b4);

//    p = CAVLTreeObj1->searchNode(CAVLTreeObj1->root, &b4);
//    if(p == NULL){
//        qDebug() << "不存在该线集";
//    } else{
//        RectNestEngine::BaseLine *pLink = p->data;
//        qDebug() << "存在要插入节点, 其头节点为：" << pLink->line() << "  长度" << pLink->width();
//        // 首先在二叉树中先删除该节点
//        CAVLTreeObj1->root = CAVLTreeObj1->deleteNode(CAVLTreeObj1->root, p->data);
//        qDebug() << "删除后先序遍历：" << endl;
//        CAVLTreeObj1->preOrderTraverse(visit, CAVLTreeObj1->root);

//        qDebug() << pLink->line() << " 宽度：" << pLink->width() << " 高度： " << pLink->py;

//        pLink = pLink->insertBaseLine(pLink, b4);
//        qDebug() << "更新后线集";
//        qDebug() << pLink->line() << "  宽度：" << pLink->width() << " 高度： " << pLink->py;

//        RectNestEngine::BaseLine *b = pLink;
//        qDebug() << "遍历链表：";
//        while(b){
//            qDebug() << "1线集： " << b->line() << "  宽度：" << b->width() << " 高度： " << b->py;
//            b = b->next;
//        }
//        CAVLTreeObj1->root = CAVLTreeObj1->insertNode(CAVLTreeObj1->root, pLink);
//    }

//    qDebug() << "添加后先序遍历：";
//    CAVLTreeObj1->preOrderTraverse(visit, CAVLTreeObj1->root);
//    qDebug() << "4添加后中序遍历：";
//    CAVLTreeObj1->inOrderTraverse(visit, CAVLTreeObj1->root);

//    RectNestEngine::BaseLine b5(20, 30, 30);
//    ///RectNestEngine::updateHightProfileLine(&b5);

//    p = CAVLTreeObj1->searchNode(CAVLTreeObj1->root, &b5);
//    if(p == NULL){
//        qDebug() << "不存在该线集";
//    } else{
//        RectNestEngine::BaseLine *pLink = p->data;
//        qDebug() << "存在要插入节点, 其头节点为：" << pLink->line() << "  长度" << pLink->width();
//        // 首先在二叉树中先删除该节点
//        CAVLTreeObj1->root = CAVLTreeObj1->deleteNode(CAVLTreeObj1->root, p->data);
//        qDebug() << "删除后先序遍历：" << endl;
//        CAVLTreeObj1->preOrderTraverse(visit, CAVLTreeObj1->root);

//        qDebug() << pLink->line() << " 宽度：" << pLink->width() << " 高度： " << pLink->py;

//        pLink = pLink->insertBaseLine(pLink, b5);
//        qDebug() << "更新后线集";
//        qDebug() << pLink->line() << "  宽度：" << pLink->width() << " 高度： " << pLink->py;

//        RectNestEngine::BaseLine *b = pLink;
//        qDebug() << "遍历链表：";
//        while(b){
//            qDebug() << "1线集： " << b->line() << "  宽度：" << b->width() << " 高度： " << b->py;
//            b = b->next;
//        }
//        CAVLTreeObj1->root = CAVLTreeObj1->insertNode(CAVLTreeObj1->root, pLink);
//    }

//    qDebug() << "添加后先序遍历：";
//    CAVLTreeObj1->preOrderTraverse(visit, CAVLTreeObj1->root);
//    qDebug() << "5添加后中序遍历：";
//    CAVLTreeObj1->inOrderTraverse(visit, CAVLTreeObj1->root);

//    return a.exec();
 */
}

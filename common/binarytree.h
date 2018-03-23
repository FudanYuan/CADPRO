#ifndef BINARYTREE_H
#define BINARYTREE_H
#include "stdlib.h"
#include "iostream"
#include <QDebug>
//平衡二叉排序树
template <typename T>
struct BBSTNode
{
    T data;
    struct BBSTNode<T> *lchild, *rchild, *parent;
};

//AVL tree类封装
template <typename T>
class CAVLTree{
private:
    //供内部调用的函数
    //求得树的高度
    int GetAVLHight(BBSTNode<T> *root)
    {
        if (root == NULL)
            return 0;
        else
        {
            int left = GetAVLHight(root->lchild);
            int right = GetAVLHight(root->rchild);
            return 1 + fmax(left, right);
        }
    }
    //单旋转，左左插入导致的不平衡，右旋操作
    void LL_RRotate(BBSTNode<T> **p)
    {
        BBSTNode<T> *OP = (*p)->parent;
        BBSTNode<T> *N = (*p)->lchild;
        if(!N){
            //qDebug() << "--------------p无左节点-------------";
        }
        (*p)->lchild = N->rchild;
        if((*p)->lchild){
            (*p)->lchild->parent = *p;
        }
        N->rchild = *p;
        if(N->rchild){
            N->rchild->parent = N;
        }
        if(N){
            N->parent = OP;
        }
        *p = N;
    }
    //单旋转，右右插入导致的不平衡，左旋操作
    void RR_LRotate(BBSTNode<T> **p)
    {
        BBSTNode<T> *OP = (*p)->parent;
        BBSTNode<T> *N = (*p)->rchild;
        if(!N){
            //qDebug() << "--------------p无右节点-------------";
            return;
        }
        (*p)->rchild = N->lchild;
        if((*p)->rchild){
            (*p)->rchild->parent = *p;
        }
        N->lchild = *p;
        if(N->lchild){
            N->lchild->parent = N;
        }
        if(N){
            N->parent = OP;
        }
        *p = N;
    }
    //双旋转，左右插入导致的不平衡
    void LR_Rotate(BBSTNode<T> **p)
    {
        //双旋转可以通过两次单旋转实现
        //对p的左结点进行RR旋转，再对根节点进行LL旋转
        RR_LRotate(&(*p)->lchild);
        LL_RRotate(p);
    }
    //双旋转，右左插入导致的不平衡
    void RL_Rotate(BBSTNode<T> **p)
    {
        LL_RRotate(&(*p)->rchild);
        RR_LRotate(p);
    }

public:
    //默认构造函数
    CAVLTree()
    {
        root = NULL;
    }
    //析构函数
    ~CAVLTree()
    {
        deleteTree(root);
    }
    //创建AVL树
    void createAVLTree(T a, int n)
    {
        if (root) return;
        for (int i = 0; i < n; i++)
        {
            root=insertNode(root, &a[i]);
        }
    }
    //删除树
    void deleteTree(BBSTNode<T> *t)
    {
        //qDebug() << "deleteTree";
        if (NULL == t)
            return;

        //qDebug() << "-------lchild--------";
        deleteTree(t->lchild);
        //qDebug() << "-------rchild--------";
        deleteTree(t->rchild);
        //qDebug() << "-------delete t--------";
        delete t;
        t = NULL;
    }
    //插入节点
    BBSTNode<T> *insertNode(BBSTNode<T> *root, T val)
    {
        BBSTNode<T> *pNewNode = new BBSTNode<T>;
        pNewNode->data = val;
        pNewNode->lchild = NULL;
        pNewNode->rchild = NULL;
        pNewNode->parent = NULL;

        if (NULL == root)
        {
            root = pNewNode;
            return root;
        }

        //qDebug() << "插入" << val->line() << " , 根结点为：" << root->data->line();

        //需要插入节点的树非空
        //插入的元素已经存在于树中，不符合要求
        if (*val == *(root->data))
        {
            //std::cout << "元素中有重复，构建AVL树失败！" << std::endl;
            qDebug() << "元素中有重复，构建AVL树失败！" ;//<< *val << "  "<< *(root->data);
            //qDebug() << val->line() << " , " << root->data->line();
            if(root->parent){
                //qDebug() << "父亲节点为：" << root->parent->data->line();
            } else{
                //qDebug() << "父亲节点为空";
            }
            return root;
        }

        //要插入的值小于根节点的值，将其插入左子树中
        if (*val < *(root->data))
        {
            //将其插入根节点的左子树中
            if(root->lchild){
                //qDebug() << "将其插入根节点的左子树中, 左子树为：" << root->lchild->data->line();

                if(*(root->lchild->data) == *val){
                    //qDebug() << "----------其左子树==插入节点！----------";
                    return root;
                }
            }
            root->lchild = insertNode(root->lchild, val);
            if(root->lchild){
                root->lchild->parent = root;
            }
            //判断平衡条件是否仍然满足
            if (GetAVLHight(root->lchild) - GetAVLHight(root->rchild) > 1)
            {
                //分两种情况进行旋转操作
                //插入点位于root的左子结点的左子树
                if (*val < *(root->lchild->data)){
                    //实施单旋转-右旋转
                    LL_RRotate(&root);
                }
                else{
                    //插入点位于root的左子结点的右子树，实施双右旋转
                    LR_Rotate(&root);
                }
            }
        }
        //要插入的值大于根节点的值，将其插入右子树中
        if (*val > *(root->data))
        {
            if(root->rchild){
                //qDebug() << "将其插入根节点的右子树中, 右子树为：" << root->rchild->data->line();

                if(*(root->rchild->data) == *val){
                    //qDebug() << "----------其右子树==插入节点！----------";
                    return root;
                }
            }
            root->rchild = insertNode(root->rchild, val);
            if(root->rchild){
                root->rchild->parent = root;
            }

            //判断平衡条件是否仍然满足
            if (GetAVLHight(root->rchild) - GetAVLHight(root->lchild) > 1)
            {
                //节点插入到root的右子节点的右子树中
                if (*val > *(root->rchild->data)){
                    //实施单旋转-左旋转
                    RR_LRotate(&root);
                }
                else{
                    //节点插入到root的右子节点的左子树上
                    //实施双旋转-左旋转
                    RL_Rotate(&root);
                }
            }
        }
        return root;
    }
    //删除树中元素值等于某值的节点
    BBSTNode<T> *deleteNode(BBSTNode<T> *root, const T val)
    {
        if (!root)
        {
            std::cout << "The tree is NULL, delete failed";
            return root;
        }

        // 浪费时间，需要优化
        BBSTNode<T> *searchedNode = searchNode(root, val);
        //没有找到相应的节点，删除失败
        if (!searchedNode)
        {
            std::cout << "Cann't find the node to delete " << val;
            return root;
        }

        //找到了需要删除的节点
        //需要删除的节点就是当前子树的根节点
        if (*val == *(root->data))
        {
            //qDebug() << "找到需要删除的节点" << (*val).line();
            //左右子树都非空
            if (root->lchild && root->rchild)
            {
                //在高度更大的那个子树上进行删除操作
                if (GetAVLHight(root->lchild) > GetAVLHight(root->rchild))
                {
                    //左子树高度大，删除左子树中元素值最大的那个节点，同时将其值赋值给根节点
                    root->data = findMaxAVL(root->lchild)->data;
                    //qDebug() << "左子树高度大，删除左子树中元素值最大的那个节点，同时将其值赋值给根节点: ";
                    //qDebug() << root->data->line();
                    root->lchild = deleteNode(root->lchild, root->data);
                    if(root->lchild){
                        root->lchild->parent = root;
                    }
                }
                else{
                    //删除右子树中元素值最小的那个节点，同时将其值赋值给根节点
                    root->data = findMinAVL(root->rchild)->data;
                    //qDebug() << "右子树高度大，删除右子树中元素值最小的那个节点，同时将其值赋值给根节点: ";
                    //qDebug() << root->data->line();
                    root->rchild = deleteNode(root->rchild, root->data);
                    if(root->rchild){
                        root->rchild->parent = root;
                    }
                }
            }
            else{
                //左右子树中有一个不为空，那个直接用需要被删除的节点的子节点替换之即可
                BBSTNode<T> *oldNode = root;
                root = (root->lchild ? root->lchild : root->rchild);
                //qDebug() << "左右子树中有一个不为空，那个直接用需要被删除的节点的子节点替换之即可： ";
                if(root){
                    //qDebug() << root->data->line();
                } else{
                    //qDebug() << "root = NULL";
                }
                delete oldNode;//释放节点所占的空间
                oldNode = NULL;
            }
        }
        else if (*val < *(root->data))//要删除的节点在左子树中
        {
            //在左子树中进行递归删除
            root->lchild = deleteNode(root->lchild, val);
            //判断是否仍然满足平衡条件
            if (GetAVLHight(root->rchild) - GetAVLHight(root->lchild) > 1)
            {
                if (GetAVLHight(root->rchild->lchild) > GetAVLHight(root->rchild->rchild))
                {
                    //左双旋转
                    RL_Rotate(&root);
                }
                else//进行左单旋转
                    RR_LRotate(&root);
            }
        }
        else//需要删除的节点在右子树中
        {
            root->rchild = deleteNode(root->rchild, val);
            //判断是否满足平衡条件
            if (GetAVLHight(root->lchild) - GetAVLHight(root->rchild) > 1)
            {
                if (GetAVLHight(root->lchild->rchild) > GetAVLHight(root->lchild->lchild))
                    //右双旋转
                    LR_Rotate(&root);
                else
                    //右单旋转
                    LL_RRotate(&root);
            }
        }
        return root;
    }
    //搜寻元素值等于某值的节点
    BBSTNode<T> *searchNode(BBSTNode<T> *root, T val)
    {
        if (!root) return NULL;

        //搜索到
        if (*val == *(root->data)){
            //qDebug() << "搜索到：" << root->data->line();
            return root;
        }
        else if (*val < *(root->data))
        {
            if(root->lchild){
                //qDebug() << "在左子树中搜索, 左子树头节点：" << root->lchild->data->line();
            }
            //在左子树中搜索
            return searchNode(root->lchild, val);
        }
        else
        {
            if(root->rchild){
                //qDebug() << "在右子树中搜索, 右子树头节点：" << root->rchild->data->line();
            }
            //在右子树中搜索
            return searchNode(root->rchild, val);
        }
    }
    //前序遍历输出树
    void preOrderTraverse(void(*visit)(BBSTNode<T>*), BBSTNode<T> *root)
    {
        if (root)
        {
            visit(root);
            preOrderTraverse(visit, root->lchild);
            preOrderTraverse(visit, root->rchild);
        }
    }
    //中序遍历输出树
    void inOrderTraverse(void(*visit)(BBSTNode<T>*), BBSTNode<T> *root)
    {
        if (root)
        {
            inOrderTraverse(visit, root->lchild);
            visit(root);
            inOrderTraverse(visit, root->rchild);
        }
    }
    // 得到树中的元素值最大的节点
    BBSTNode<T> *findMaxAVL(BBSTNode<T> *p)
    {
        if (NULL==p) return NULL;
        if (p->rchild == NULL)
            return p;
        return findMaxAVL(p->rchild);
    }
    // 得到树中的元素值最小的那个节点
    BBSTNode<T> *findMinAVL(BBSTNode<T> *p)
    {
        if (NULL == p)
            return NULL;
        if (p->lchild == NULL)
            return p;
        return findMinAVL(p->lchild);
    }
    // 得到比某个元素值大的最小节点
    // 如果该节点有右子树，那么寻找右子树上的最小节点
    // 如果该节点没有右子树，则判断该节点的根，
    // 如果该节点的根大于该节点，则返回根结点，否则返回NULL
    BBSTNode<T> *findMinLargerAVL(BBSTNode<T> *p)
    {
        BBSTNode<T> *res = NULL;
        if (NULL == p)
            return res;
        if (p->rchild == NULL){
            if(p->parent){
                //qDebug() << "p->parent：" << p->parent->data->line();
                if(*(p->parent->data) > *(p->data)){
                    res = p->parent;
                }
            } else{
                //qDebug() << "p->parent = null";
            }
        }
        else{
            //qDebug() << "findMinLargerAVL---寻找最小值";
            res = findMinAVL(p->rchild);
        }
        return res;
    }
    /*
    // 得到比某个元素值小的最大节点
    // 如果该节点有左子树，那么寻找左子树上的最大节点
    // 如果该节点没有左子树但有右子树，则比该节点大的最小节点为该节点的右子树
    // 如果该节点没有子树，则比该节点大的最小节点为比该节点根小的最大值
    BBSTNode<T> *findMaxSmallerAVL(BBSTNode<T> *p)
    {
        BBSTNode<T> *res = NULL;
        if (NULL == p)
            return res;
        if (p->rchild == NULL){
            return p->parent;
        }
        else{
            res = findMinAVL(p->rchild);
        }
        return res;
    }
    */
    BBSTNode<T> *root;

    /*
    static void visit(BBSTNode<T> *p)
    {
        std::cout << "visit: " << std::endl;
        std::cout << *(*p).data << "\t";
        if(p->parent){
            std::cout << "父母：：" << *(*p).parent->data << "\t";
        }
        if(p->lchild){
            std::cout << "左孩子：：" << *(*p).lchild->data << "\t";
        }
        if(p->rchild){
            std::cout << "右孩子：：" << *(*p).rchild->data << "\t";
        }
    }
    */
};
#endif // BINARYTREE_H

// 使用例程
/*
    void visit(BBSTNode<int> *p)
    {
        std::cout << (*p).data << "\t";
    }
    int arr[] = {18,14,20,16,12};

    CAVLTree<int*> *CAVLTreeObj1 = new CAVLTree<int*>();
    CAVLTreeObj1->createAVLTree(arr, 5);
    cout << "AVL Tree中序遍历结果：" << endl;
    CAVLTreeObj1->inOrderTraverse(visit, CAVLTreeObj1->root);
    cout << endl;

    cout << "AVL Tree先序遍历结果：" << endl;
    CAVLTreeObj1->preOrderTraverse(visit, CAVLTreeObj1->root);
    cout << endl;

    int insertedVal1 = 12;
    CAVLTreeObj1->root = CAVLTreeObj1->insertNode(CAVLTreeObj1->root, &insertedVal1);
    cout << "向AVL树中插入元素  " << insertedVal1 << "之后的中序遍历结果：" << endl;
    CAVLTreeObj1->inOrderTraverse(visit, CAVLTreeObj1->root);
    cout << endl;
    cout << "向AVL树中插入元素  " << insertedVal1 << "之后的先序遍历结果：" << endl;
    CAVLTreeObj1->preOrderTraverse(visit, CAVLTreeObj1->root);
    cout << endl;

    int searchValue = 14;
    BBSTNode<int*> *sNode = CAVLTreeObj1->searchNode(CAVLTreeObj1->root, &searchValue);
    cout << "在AVL树中查询元素  " << *(sNode->data) << endl;

    BBSTNode<int*> *lNode = CAVLTreeObj1->findMinLargerAVL(sNode);
    if(lNode){
        cout << "在AVL树中查询比元素" << *sNode->data << "大的最小元素为：  " << *lNode->data << endl;
    }

    int deletedVal2 = 14;
    CAVLTreeObj1->root = CAVLTreeObj1->deleteNode(CAVLTreeObj1->root, &deletedVal2);
    cout << "删除元素值为 " << deletedVal2 << "的节点之后的树中序遍历结果：" << endl;
    CAVLTreeObj1->inOrderTraverse(visit, CAVLTreeObj1->root);
    cout << endl;
    cout << "删除元素值为 " << deletedVal2 << "的节点之后的树先序遍历结果：" << endl;
    CAVLTreeObj1->preOrderTraverse(visit, CAVLTreeObj1->root);
    cout << endl;

    searchValue = 20;
    sNode = CAVLTreeObj1->searchNode(CAVLTreeObj1->root, &searchValue);
    cout << "在AVL树中查询元素  " << *sNode->data << endl;

    lNode = CAVLTreeObj1->findMinLargerAVL(sNode);
    if(lNode){
        cout << "在AVL树中查询比元素" << *sNode->data << "大的最小元素为：  " << *lNode->data << endl;
    }
 * */

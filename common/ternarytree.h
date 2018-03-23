#ifndef TERNARYTREE_H
#define TERNARYTREE_H
#include "stdlib.h"
#include "stdio.h"
#include "iostream"
#include "stack"
#include "queue"
#include "assert.h"
#include <QList>

template <typename T>
struct TTNode{
    T data;
    TTNode<T> *lchild,*mchild,*rchild;
};

enum Tags{
    Left,Mid,Right
};

enum style{
    Pre,In01,In02,Post
};

template <typename T>
struct StackElem{
    TTNode<T> *p;
    Tags flag;
};

template <typename T>
class TernaryTree
{
protected:
    TTNode<T> *root;
private:
    void destroyTernaryTree(TTNode<T> *&t){
        if(t!=NULL){
            destroyTernaryTree(t->lchild);
            destroyTernaryTree(t->mchild);
            destroyTernaryTree(t->rchild);
            delete t;
            t=NULL;
        }

    }
public:
    TernaryTree(){
        root=NULL;
    }

    ~TernaryTree(){
        destroyTernaryTree(root);
    }

    // 由文件新建一棵三叉树
    void createTernaryTreeFromFile(std::ifstream &f){
        T e;
        f >> e;
        if(e==NULL) return; // if(e==Nil) return; // 遇到结束标志即返回
        root =new TTNode<T>;
        assert(root!=NULL);
        root->data=e;
        TernaryTree<T> left,mid,right;
        left.CreateTernaryTreeFromFile(f);
        mid.CreateTernaryTreeFromFile(f);
        right.CreateTernaryTreeFromFile(f);

        root->lchild=left.root;
        left.root=NULL;

        root->mchild=mid.root;
        mid.root=NULL;

        root->rchild=right.root;
        right.root=NULL;
    }

    // 新建一棵三叉排序树
    void createTernarySortTree(QList<T> a)
    {
        if(root) return;
        for(int i = 0; i<a.length(); i++)
        {
            root=insertTTNode(root, a[i]);
        }
    }

    // 查找节点
    TTNode<T>* searchTTNode(TTNode<T> *root, T data)
    {
        if(!root) return NULL;
        //搜索到
        if(data == root->data) {
            return root;
        } else if(data < root->data) {
            //在左子树中搜索
            return searchNode(root->lchild, data);
        } else if(data > root->data) {
            //在右子树中搜索
            return searchNode(root->rchild, data);
        } else if(data >= root->data) {
            //在中子树中搜索
            return searchNode(root->mchild, data);
        } else if(data <= root->data){
            return NULL;
        }
    }

    // 插入节点
    void insertTTNode(TTNode<T> *root, T data)
    {
        //初始化插入节点
        TTNode<T> *p = (TTNode<T>*)malloc(sizeof(struct TTNode<T>));
        if(!p) return;
        p->data = data;
        p->lchild = p->mchild = p->rchild = NULL;

        // 空树时，直接作为根节点
        if(root == NULL) {
            root = p;
            return;
        }

        // 是否存在，已存在则返回，不插入
        if(searchTTNode(root, data) != NULL) return;

        // 进行插入，首先找到要插入的位置的父节点
        TTNode<T> *tnode = NULL, *troot = root;
        while (troot)
        {
            tnode = troot;
            if(data < troot->data){
                troot = troot->lchild;
            } else if(data > troot->data){
                troot = troot->rchild;
            } else if(data <= troot->data){
                troot = troot->mchild;
            }
        }
        // 小插左；大插右；否则，插中间
        if (data < tnode->data)
            tnode->lchild = p;
        else if(data > tnode->data)
            tnode->rchild = p;
        else
            tnode->mchild = p;
    }

    // 判断树是否为空
    bool isEmpty()const{
        return root==NULL;
    }

    // 得到三叉树的深度
    int ternaryTreeDepth(TTNode<T>* t){
        int i,j,k,bri;
        if(t==NULL) return 0;
        else{
            i=TernaryTreeDepth(t->rchild);
            k=TernaryTreeDepth(t->mchild);
            j=TernaryTreeDepth(t->lchild);
            bri=i>j?i:j;
            return bri>k?bri+1:k+1;
        }
    }

    // 得到根节点
    TTNode<T>* getRoot(){
        return root;
    }

    // 返回节点值
    T value(TTNode<T>* p)const{
        return p->data;
    }

    // 为节点赋值
    void assign(TTNode<T>* p,T value){
        p->data=value;
    }

    // 前序遍历三叉树（根->左->中->右）
    void preOrderTraverse(void(*visit)(TTNode<T>*))const{
        std::stack<TTNode<T>*> s;
        TTNode<T> *t=root;
        s.push(NULL);
        while(t!=NULL){
            visit(t);
            if(t->rchild!=NULL)
                s.push(t->rchild);
            if(t->mchild!=NULL)
                s.push(t->mchild);
            if(t->lchild!=NULL)
                t=t->lchild;
            else{
                t=s.top();
                s.pop();
            }
        }
    }
    // 中1序遍历三叉树（左->根->中->右）
    void inOneOrderTraverse(void(*visit)(TTNode<T>*))const{
        StackElem<T> se;
        std::stack<StackElem<T> > s;
        TTNode<T> *t=root;
        if(t==NULL) return ;
        while(!s.empty()||t!=NULL){
            while(t!=NULL){
                se.p=t;
                se.flag=Left;
                s.push(se);
                t=t->lchild;
            }
            se=s.top();
            s.pop();
            t=se.p;
            if(se.flag==Left){
                visit(t);
                se.flag=Mid;
                s.push(se);
                t=t->mchild;
            }
            else{
                if(se.flag==Mid){
                    se.flag=Right;
                    s.push(se);
                    t=t->rchild;
                }
                else{
                    t=NULL;
                }
            }
        }

    }
    // 中2序遍历三叉树（左->中->根->右）
    void inTwoOrderTraverse(void(*visit)(TTNode<T>*))const{
        StackElem<T> se;
        std::stack<StackElem<T> > s;
        TTNode<T> *t=root;
        if(t==NULL) return ;
        while(!s.empty()||t!=NULL){
            while(t!=NULL){
                se.p=t;
                se.flag=Left;
                s.push(se);
                t=t->lchild;
            }
            se=s.top();
            s.pop();
            t=se.p;
            if(se.flag==Left){

                se.flag=Mid;
                s.push(se);
                t=t->mchild;
            }
            else{
                if(se.flag==Mid){
                    visit(t);
                    se.flag=Right;
                    s.push(se);
                    t=t->rchild;
                }
                else{
                    t=NULL;
                }
            }
        }

    }
    // 后序遍历三叉树（左->中->右->根）
    void postOrderTraverse(void(*visit)(TTNode<T>*))const{
        StackElem<T> se;
        std::stack<StackElem<T> > s;
        TTNode<T> *t=root;
        if(t==NULL) return ;

        while(!s.empty()||t!=NULL){
            while(t!=NULL){
                se.p=t;
                se.flag=Left;
                s.push(se);
                t=t->lchild;
            }
            se=s.top();
            s.pop();
            t=se.p;
            if(se.flag==Left){
                se.flag=Mid;
                s.push(se);
                t=t->mchild;
            }
            else{
                if(se.flag==Mid){
                    se.flag=Right;
                    s.push(se);
                    t=t->rchild;
                }
                else{
                    visit(t);
                    t=NULL;
                }
            }
        }
    }
    // 层序遍历三叉树
    void levelOrderTraverse(void(*visit)(TTNode<T>*))const{
        std::queue<TTNode<T>*> q;
        TTNode<T> *a,*t=root;
        if(t!=NULL){
            q.push(t);
            while(!q.empty()){
                a=q.front();
                q.pop();
                visit(a);
                if(a->lchild!=NULL)
                    q.push(a->lchild);
                if(a->mchild!=NULL)
                    q.push(a->mchild);
                if(a->rchild!=NULL)
                    q.push(a->rchild);
            }
        }
    }
    // 递归遍历
    void orderTraverse(TTNode<T>* t,style mode,void(*visit)(TTNode<T>*))const{
        if(t!=NULL){
            if(mode==Pre)
                visit(t);

            orderTraverse(t->lchild,mode,visit);

            if(mode==In01)
                visit(t);
            orderTraverse(t->mchild,mode,visit);

            if(mode==In02)
                visit(t);
            orderTraverse(t->rchild,mode,visit);

            if(mode==Post)
                visit(t);
        }
    }
};
#endif // TERNARYTREE_H


// 使用例程
/*
    TernaryTree<T> t,c;
    TTNode<T> *p;
    T e;
    ifstream fin("test.txt");
    t.CreateTernaryTreeFromFile(fin);
    fin.close();
    qDebug()<<"由文件test.txt创建三叉树t后，树t空否？"<<boolalpha<<t.TernaryTreeEmpty();
    qDebug()<<"。树t的深度="<<t.TernaryTreeDepth(t.Root())<<endl;
    qDebug()<<endl<<"层序遍历删除后的三叉树t：";
    t.LevelOrderTraverse(Visit);
    p=t.Root();
    qDebug()<<endl<<"t的根结点="<<t.Value(p)<<"。给根结点赋新值，请输入新值：";
    e = 'b';
    t.Assign(p, e);
    qDebug()<<endl<<"层序遍历删除后的三叉树t：";
    t.LevelOrderTraverse(Visit);

    qDebug()<<endl<<"先序遍历删除后的三叉树t：";
    t.PreOrderTraverse(Visit);
    qDebug()<<endl<<"先序递归遍历删除后的三叉树t：";
    t.OrderTraverse(t.Root(), Pre, Visit);
    qDebug()<<endl;
    qDebug()<<endl<<"中1序遍历删除后的三叉树t：";
    t.In01OrderTraverse(Visit);
    qDebug()<<endl<<"中1序递归遍历删除后的三叉树t：";
    t.OrderTraverse(t.Root(), In01, Visit);
    qDebug()<<endl;
    qDebug()<<endl<<"中2序遍历删除后的三叉树t：";
    t.In02OrderTraverse(Visit);
    qDebug()<<endl<<"中2序递归遍历删除后的三叉树t：";
    t.OrderTraverse(t.Root(), In02, Visit);
    qDebug()<<endl;
    qDebug()<<endl<<"后序遍历删除后的三叉树t：";
    t.PostOrderTraverse(Visit);
    qDebug()<<endl<<"后序递归遍历删除后的三叉树t：";
    t.OrderTraverse(t.Root(), Post, Visit);
*/

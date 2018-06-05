#ifndef QUADTREENODE_H
#define QUADTREENODE_H

#include <list>
#include <vector>
#include <QLineF>
#include <QRectF>
#include <QDebug>

/*
//被管理的对象类
*/
class Object
{
public:
    Object(int i, float _x,float _y,float _width,float _height):
        id(i),
        x(_x),
        y(_y),
        width(_width),
        height(_height)
    {

    }
    Object(float _x,float _y,float _width,float _height):
        id(-1),
        x(_x),
        y(_y),
        width(_width),
        height(_height)
    {

    }
    Object(int i, QRectF rect)
    {
        id = i;
        x = rect.topLeft().rx();
        y = rect.topLeft().ry();
        width = rect.width();
        height = rect.height();
    }
    Object(QRectF rect)
    {
        id = -1;
        x = rect.topLeft().rx();
        y = rect.topLeft().ry();
        width = rect.width();
        height = rect.height();
    }
    ~Object()
    {

    }

    std::list<Object*> carve(Object* object){
        std::list<Object*> retObjsList;
        float objX = x;
        float objY = y;
        float objW = width;
        float objH = height;
        float cX = object->x + object->width / 2;
        float cY = object->y + object->height / 2;
        //qDebug() << "所在区域: Object(" << object->x << ", " << object->y << ", " << object->width << ", " << object->height << ")";
        //qDebug() << "对象: Object(" << x << ", " << y << ", " << width << ", " << height << ")";
        // 左部 或 右部
        if((objX + objW) < cX || objX > cX){
            //qDebug() << "左部 或 右部";
            retObjsList.push_back(new Object(id, objX, objY, objW, cY-objY));
            retObjsList.push_back(new Object(id, objX, cY, objW, objY+objH-cY));
        }
        // 上部 或 底部
        if((objY + objH) < cY || objY > cY){
            //qDebug() << "上部 或 底部";
            retObjsList.push_back(new Object(id, objX, objY, cX-objX, objH));
            retObjsList.push_back(new Object(id, cX, objY, objX+objW-cX, objH));
        }
        // 中部
        if(objX < cX
                && ((objX + objW) > cX)
                && objY < cY
                && ((objY + objH) > cY) ){
            //qDebug() << "中部";
            retObjsList.push_back(new Object(id, objX, objY, cX-objX, cY-objY));
            retObjsList.push_back(new Object(id, cX, objY, objX+objW-cX, cY-objY));
            retObjsList.push_back(new Object(id, objX, cY, cX-objX, objY+objH-cY));
            retObjsList.push_back(new Object(id, cX, cY, objX+objW-cX, objY+objH-cY));
        }
        return retObjsList;
    }
public:
    //对象的属性，例如ID,坐标和长宽，以左上角为锚点
    int id;
    float x;
    float y;
    float width;
    float height;
};

/*
//四叉树节点类，用头节点代表四叉树
//坐标系坐上角为原点，左往右为x轴递增，上往下y轴递增
//本四叉树的策略是：
//1，插入时动态分配节点和删除节点，不是满树；
//2，当矩形区域完全包含某个节点时才获取或剔除；
//3，对象放在完全包含它的区域节点内，非根节点也存储对象
*/
//四叉树类型枚举
enum QuadType
{
    ROOT,         //根
    UP_RIGHT,     //象限Ⅰ
    UP_LEFT,      //象限Ⅱ
    BOTTOM_LEFT,  //象限Ⅲ
    BOTTOM_RIGHT  //象限Ⅳ
};

template <typename T>
class QuadTreeNode
{
public:
    QuadTreeNode(
            float _x,float _y,float _width,float _height,
            int _level,int _maxLevel, int _maxObject,
            QuadType _quadType,
            QuadTreeNode *_parent) :
        quadType(_quadType),
        x(_x),
        y(_y),
        width(_width),
        height(_height),
        level(_level),
        maxLevel(_maxLevel),
        maxObject(_maxObject)
        {
            parent = _parent;
            upRightNode = NULL;
            upLeftNode = NULL;
            bottomLeftNode = NULL;
            bottomRightNode = NULL;
        }

    ~QuadTreeNode(){
        if(level == maxLevel){
            return;
        }
        //如果不是叶子节点，就销毁子节点
        parent = NULL;
    }
public:
    //插入对象
    void InsertObject(T *object){
        //如果是叶子节点，则存在叶子节点
        if(level==maxLevel) {
            objects.push_back(object);
            return;
        }

        //非叶子节点，如果下层节点可以包含该对象，则递归构建子节点并插入对象,边构建边插入
        if(IsContain(x+width/2,y,width/2,height/2,object)) {
            if(!upRightNode){ //避免重复创建覆盖掉原来的节点
                upRightNode=new QuadTreeNode(x+width/2,y,width/2,height/2,level+1,maxLevel,maxObject,UP_RIGHT,this);//如果没有子节点就创建子节点，parent节点是当前节点
            }
            upRightNode->InsertObject(object);
            return;
        } else if(IsContain(x,y,width/2,height/2,object)) {
            if(!upLeftNode){
                upLeftNode=new QuadTreeNode(x,y,width/2,height/2,level+1,maxLevel,maxObject,UP_LEFT,this);
            }
            upLeftNode->InsertObject(object);
            return;
        } else if(IsContain(x,y+height/2,width/2,height/2,object)) {
            if(!bottomLeftNode){
                bottomLeftNode=new QuadTreeNode(x,y+height/2,width/2,height/2,level+1,maxLevel,maxObject,BOTTOM_LEFT,this);
            }
            bottomLeftNode->InsertObject(object);
            return;
        } else if(IsContain(x+width/2,y+height/2,width/2,height/2,object)) {
            if(!bottomRightNode)
                bottomRightNode=new QuadTreeNode(x+width/2,y+height/2,width/2,height/2,level+1,maxLevel,maxObject,BOTTOM_RIGHT,this);
            bottomRightNode->InsertObject(object);
            return;
        }
        //下层节点不能完全包含该对象，则插入到当前非叶子节点
        //这个判断也可以省去
        if(IsContain(x,y,width,height,object)){
            objects.push_back(object);
        }
    }

    //查询对象,获得一片区域里的对象链表，此处只考虑完全包含的
    std::list<T *> GetObjectsAt(float px,float py,float w,float h){
        std::list<T *> resObjects;
        //如果当前节点完全被包含，把当前节点存的对象放到列表末尾,空链表也行
        if(IsContain(px,py,w,h,this)) {
            resObjects.insert(resObjects.end(),objects.begin(),objects.end());
            //最后一层
            if(level==maxLevel){
                return resObjects;
            }
        }

        //如果有下层节点就把下层节点包含的对象加进来
        if(upRightNode) {
            std::list<T *> upRightChild;
            upRightChild=upRightNode->GetObjectsAt(px,py,w,h);
            resObjects.insert(resObjects.end(),upRightChild.begin(),upRightChild.end());
        }
        if(upLeftNode) {
            std::list<T *> upLeftChild;
            upLeftChild=upLeftNode->GetObjectsAt(px,py,w,h);
            resObjects.insert(resObjects.end(),upLeftChild.begin(),upLeftChild.end());
        }
        if(bottomLeftNode) {
            std::list<T *> bottomLeftChild;
            bottomLeftChild=bottomLeftNode->GetObjectsAt(px,py,w,h);
            resObjects.insert(resObjects.end(),bottomLeftChild.begin(),bottomLeftChild.end());
        }
        if(bottomRightNode) {
            std::list<T *> bottomRightChild;
            bottomRightChild=bottomRightNode->GetObjectsAt(px,py,w,h);
            resObjects.insert(resObjects.end(),bottomRightChild.begin(),bottomRightChild.end());
        }
        return resObjects;
    }

    //查询对象,获得一片区域里的对象链表，此处只考虑完全包含的，重载
    std::list<T *> GetObjectsAt(QRectF rect){
        return GetObjectsAt(rect.topLeft().rx(), rect.topLeft().ry(), rect.width(), rect.height());
    }

    //删除对象，删除一片区域里的对象和节点，此处只考虑完全包含的
    void RemoveObjectsAt(float px,float py,float w,float h){
        //如果本层节点被包含则删除本层节点的对象
        //这个判断主要是对根节点起作用，其他子节点实际在上层都做了判断
        if(IsContain(px,py,w,h,this)) {
            //清除本节点层的对象
            objects.clear();
            //最后一层
            if(level==maxLevel){
                return;
            }
        }
        //如果有子节点且被包含就销毁子节点，注意别产生野指针
        //其实只要上层被包含了，下层肯定被包含，代码还需改进
        if(upRightNode&&IsContain(px,py,w,h,upRightNode)) {
            upRightNode->RemoveObjectsAt(px,py,w,h);
            delete upRightNode;
            upRightNode = NULL;

        }
        if(upLeftNode&&IsContain(px,py,w,h,upLeftNode)) {
            upLeftNode->RemoveObjectsAt(px,py,w,h);
            delete upLeftNode;
            upLeftNode = NULL;

        }
        if(bottomLeftNode&&IsContain(px,py,w,h,bottomLeftNode)) {
            bottomLeftNode->RemoveObjectsAt(px,py,w,h);
            delete bottomLeftNode;
            bottomLeftNode = NULL;

        }
        if(bottomRightNode&&IsContain(px,py,w,h,bottomRightNode)) {
            bottomRightNode->RemoveObjectsAt(px,py,w,h);
            delete bottomRightNode;
            bottomRightNode = NULL;
        }
    }

    //判断物体属于那个象限
    QuadType getQuadType(Object *object){
        bool onTop = (object->y + object->height) <= (y + height / 2);
        bool onBottom = object->y >= (y + height / 2);
        bool onLeft = (object->x + object->width) <= (x + width / 2);
        bool onRight = object->x >= (x + width / 2);
        if(onTop){
            if(onRight){
                return UP_RIGHT;
            } else if(onLeft){
                return UP_LEFT;
            }
        } else if(onBottom){
            if(onLeft){
                return BOTTOM_LEFT;
            } else if(onRight){
                return BOTTOM_RIGHT;
            }
        }
        return ROOT;
    }

    // 插入对象
    void insert(T *object){
        //qDebug() << "插入对象: Object(" << object->id << ", " << object->x << ", " << object->y << ", " << object->width << ", " << object->height << ")";
        // 如果该节点下存在子节点
        if(upRightNode != NULL || upLeftNode != NULL
                || bottomLeftNode != NULL || bottomRightNode != NULL){
            //qDebug() << "存在子节点";
            QuadType type = getQuadType(object);
            //qDebug() << "新的对象在象限：" << type;
            switch (type) {
            case UP_RIGHT:
                upRightNode->insert(object);
                break;
            case UP_LEFT:
                upLeftNode->insert(object);
                break;
            case BOTTOM_LEFT:
                bottomLeftNode->insert(object);
                break;
            case BOTTOM_RIGHT:
                bottomRightNode->insert(object);
                break;
            case ROOT:
                objects.push_back(object);
                break;
            default:
                break;
            }
            return;
        }

        // 否则存储在当下节点
        objects.push_back(object);
        //qDebug() << "对象个数: " << objects.size();
        //qDebug() << "";
        // 如果当前节点存储的数量超过了MAX_OBJECTS, 对该区域进行分割
        if((upRightNode == NULL && upLeftNode == NULL
            && bottomLeftNode == NULL && bottomRightNode == NULL)
                && (int)objects.size() > maxObject
                && level < maxLevel){
            //qDebug() << "对该区域进行分割: " ;
            //qDebug() << QRectF(x, y, width, height);
            //qDebug() << "分割前对象个数: " << objects.size();;
            split();  // 分割
            std::list<T *> objs;
            objs.insert(objs.end(), objects.begin(), objects.end());
            for(auto &obj : objs){
                QuadType type = getQuadType(obj);
                //qDebug() << "插入子对象: Object(" << obj->x << ", " << obj->y << ", " << obj->width << ", " << obj->height << ")";
                //qDebug() << "对象在子象限：" << type;
                switch (type) {
                case UP_RIGHT:
                    upRightNode->insert(obj);
                    objects.remove(obj);  // 删除该节点处的对象
                    break;
                case UP_LEFT:
                    upLeftNode->insert(obj);
                    objects.remove(obj);  // 删除该节点处的对象
                    break;
                case BOTTOM_LEFT:
                    bottomLeftNode->insert(obj);
                    objects.remove(obj);  // 删除该节点处的对象
                    break;
                case BOTTOM_RIGHT:
                    bottomRightNode->insert(obj);
                    objects.remove(obj);  // 删除该节点处的对象
                    break;
                case ROOT:
                    break;
                default:
                    break;
                }
            }
            objs.clear();
            //qDebug() << "分割后对象个数: " << objects.size();
        }
    }

    // 检测可能碰撞的区域
    std::list<T *> retrieve(T *obj){
        std::list<T *> result;
        //qDebug() << "objects: " << objects.size();
        result.insert(result.end(), objects.begin(), objects.end());
        // 如果该节点下存在子节点
        if(upRightNode != NULL || upLeftNode != NULL
                || bottomLeftNode != NULL || bottomRightNode != NULL){
            QuadType type = getQuadType(obj);
            switch (type) {
            case UP_RIGHT:
                result.merge(upRightNode->retrieve(obj));
                break;
            case UP_LEFT:
                result.merge(upLeftNode->retrieve(obj));
                break;
            case BOTTOM_LEFT:
                result.merge(bottomLeftNode->retrieve(obj));
                break;
            case BOTTOM_RIGHT:
                result.merge(bottomRightNode->retrieve(obj));
                break;
            case ROOT:{
                // 切割矩形
                //qDebug() << "切割矩形";
                std::list<T*> objSplit = obj->carve(new T(-1, x, y, width, height));
                for(auto &split : objSplit){
                    //qDebug() << split->x << "  " << split->y << " " << split->width << "  " << split->height;
                    QuadType type1 = getQuadType(split);
                    //qDebug() << "切割后的象限：" << type1;
                    switch(type1){
                    case UP_RIGHT:
                        result.merge(upRightNode->retrieve(split));
                        break;
                    case UP_LEFT:
                        result.merge(upLeftNode->retrieve(split));
                        break;
                    case BOTTOM_LEFT:
                        result.merge(bottomLeftNode->retrieve(split));
                        break;
                    case BOTTOM_RIGHT:
                        result.merge(bottomRightNode->retrieve(split));
                        break;
                    default:
                        break;
                    }
                }
                break;
            }
            default:
                break;
            }
        }
        //qDebug() << "";
        return result;
    }

    // 检测可能碰撞的区域，重载
    std::list<T *> retrieve(int id, float x, float y, float w, float h){
        return retrieve(new T(id, x, y, w, h));
    }

    // 获取对象
    std::list<Object *> getObjects(QuadType type = ROOT){
        std::list<T *> retList;
        switch (type) {
        case UP_RIGHT:
            if(upRightNode != NULL){
                retList.merge(upRightNode->getObjects(ROOT));
            }
            break;
        case UP_LEFT:
            if(upLeftNode != NULL){
                retList.merge(upLeftNode->getObjects(ROOT));
            }
            break;
        case BOTTOM_LEFT:
            if(bottomLeftNode != NULL){
                retList.merge(bottomLeftNode->getObjects(ROOT));
            }
            break;
        case BOTTOM_RIGHT:
            if(bottomRightNode != NULL){
                retList.merge(bottomRightNode->getObjects(ROOT));
            }
            break;
        case ROOT:
            retList.merge(objects);
            break;
        default:
            break;
        }
        return retList;
    }

    // 获取分界线
    std::list<QLineF *> getMiddleAxis()
    {
        std::list<QLineF* > lineList;
        lineList.push_back(new QLineF(x+width/2, y, x+width/2, y+height));
        lineList.push_back(new QLineF(x, y+height/2, x+width, y+height/2));
        if(upRightNode != NULL){
            lineList.merge(upRightNode->getMiddleAxis());
        }
        if(upLeftNode != NULL){
            lineList.merge(upLeftNode->getMiddleAxis());
        }
        if(bottomLeftNode != NULL){
            lineList.merge(bottomLeftNode->getMiddleAxis());
        }
        if(bottomRightNode != NULL){
            lineList.merge(bottomRightNode->getMiddleAxis());
        }
        return lineList;
    }
private:
    // 分割函数，如果象限内存储的物体数量超过了MAX_OBJECTS，则对这个节点进行划分
    void split(){
        upRightNode = new QuadTreeNode(x+width/2, y, width/2, height/2, level+1, maxLevel, maxObject,UP_RIGHT, this);
        upLeftNode = new QuadTreeNode(x, y, width/2, height/2, level+1, maxLevel, maxObject, UP_LEFT, this);
        bottomLeftNode = new QuadTreeNode(x, y+height/2, width/2, height/2, level+1, maxLevel, maxObject, BOTTOM_LEFT, this);
        bottomRightNode = new QuadTreeNode(x+width/2, y+height/2, width/2, height/2, level+1, maxLevel, maxObject, BOTTOM_RIGHT, this);
        level++;
    }

private:
    //判断某个区域是否包含某对象
    bool IsContain(float px,float py,float w,float h,T *object) const
    {
        if(object->x>=px
                && object->x+object->width<=px+w
                && object->y>=py
                && object->y+object->height<=py+h){
            return true;
        }
        return false;
    }
    //重载，判断某个区域是否包含某个节点
    bool IsContain(float px,float py,float w,float h,QuadTreeNode<T> *quadTreeNode) const
    {
        if(quadTreeNode->x>=px
                &&quadTreeNode->x+quadTreeNode->width<=px+w
                &&quadTreeNode->y>=py
                &&quadTreeNode->y+quadTreeNode->height<=py+h){
            return true;
        }
        return false;
    }

//private:
public:
    std::list<T *> objects; //节点数据队列

    //父、子节点，分四个象限
    QuadTreeNode *parent;
    QuadTreeNode *upRightNode;
    QuadTreeNode *upLeftNode;
    QuadTreeNode *bottomLeftNode;
    QuadTreeNode *bottomRightNode;

    //节点类型
    QuadType quadType;

    //坐标和长宽属性，左上角为锚点
    float x;
    float y;
    float width;
    float height;

    int level; //当前深度
    int maxLevel; //最大深度

    int maxObject;  // 最大物体数量
};

#endif // QUADTREENODE_H

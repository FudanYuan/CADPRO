#ifndef RECTNESTENGINE_H
#define RECTNESTENGINE_H

#include <QObject>
#include <QList>
#include "nest.h"

// 矩形排版引擎
// BL -> TL
class RectNestEngine{
public:
    // 最小矩形件
    struct MinRect
    {
        MinRect() :
            page(0),
            position(QPointF(0, 0)),
            width(0),
            height(0),
            area(0.0),
            ratio(0.0),
            index(-1),
            rotate(false),
            layFlag(false)
        {}

        MinRect(qreal w, qreal h, int i, bool r, int id) :
            position(QPointF(0, 0))
        {
            width = w;
            height = h;
            area = w * h;
            ratio = qMax(h/w, w/h);
            index = i;
            rotate = r;
            componentId = id;
        }

        QLineF upper(){
            double w = width;
            double h = height;
            if(rotate){
                double temp = w;
                w = h;
                h = temp;
            }
            return QLineF(position + QPointF(0, h), position + QPointF(w, h));
        }

        QLineF lower(){
            double w = width;
            double h = height;
            if(rotate){
                double temp = w;
                w = h;
                h = temp;
            }
            return QLineF(position, position + QPointF(w, 0));
        }

        QRectF rect(){
            double w = width;
            double h = height;
            if(rotate){
                double temp = w;
                w = h;
                h = temp;
            }
            return QRectF(position.rx(), position.ry(), w, h);
        }

        qreal getWidth(){
            double w = width;
            if(rotate){
                w = height;
            }
            return w;
        }

        qreal getHeight(){
            double h = height;
            if(rotate){
                h = width;
            }
            return h;
        }

        // 设置旋转
        void setRotate(bool r) {
            rotate = r;
        }

        // 交换位置
        void exchange(MinRect &rect){
            QPointF pos = rect.position;
            rect.position = position;
            position = pos;
        }

        // 设置对应切割件的id
        void setComponentId(int id){
            componentId = id;
        }

        // 获取对应切割件的id
        int getComponentId(){
            return componentId;
        }
        int page;  // 页数
        QPointF position;  // 位置，指的是右上角坐标
        qreal width;  // 宽
        qreal height;  // 高
        qreal area;  // 面积
        qreal ratio;  // 长宽比
        int index;  // 矩形序号
        bool rotate;  // 是否旋转90*，统一逆时针
        int componentId;  // 对应的切割件的id
        bool layFlag;
    };

    // 空白区域
    struct EmptyRectArea
    {
        EmptyRectArea() :
            px1(0),
            py1(0),
            px2(0),
            py2(0)
        {}

        EmptyRectArea(QRectF rect) :
            px1(rect.bottomLeft().rx()),
            py1(rect.bottomLeft().ry()),
            px2(rect.topRight().rx()),
            py2(rect.topRight().ry())
        {}

        EmptyRectArea(MinRect rect) :
            px1(rect.position.rx()),
            py1(rect.position.ry())
        {
            px2 = rect.position.rx() + (rect.rotate ? rect.height : rect.width);
            py2 = rect.position.ry() + (rect.rotate ? rect.width : rect.height);
        }

        EmptyRectArea(QPointF lb, QPointF rt) :
            px1(lb.rx()),
            py1(lb.ry()),
            px2(rt.rx()),
            py2(rt.ry())
        {}

        EmptyRectArea(QPointF p, qreal w, qreal h) :
            px1(p.rx()),
            py1(p.ry()),
            px2(p.rx()+w),
            py2(p.ry()+h)
        {}

        EmptyRectArea(qreal x1, qreal y1, qreal x2, qreal y2) :
            px1(x1),
            py1(y1),
            px2(x2),
            py2(y2)
        {}

        qreal px1, py1, px2, py2;   // 顶点坐标

        // 左下角的点
        QPointF  leftBottom()
        {
            return QPointF(px1, py1);
        }

        // 右上角的点
        QPointF  rightTop()
        {
            return QPointF(px2, py2);
        }

        // 宽度
        qreal width(){
            return px2 - px1;
        }

        // 高度
        qreal height(){
            return py2 - py1;
        }

        // 面积
        qreal area(){
            return (px2-px1)*(py2-py1);
        }

        // 是否相交（包含）
        bool intersect(const EmptyRectArea rect, EmptyRectArea &interRect){
            qreal left = 0;
            qreal bottom = 0;
            qreal right = 0;
            qreal top = 0;

            // 计算两矩形可能相交矩形的边界
            left = px1 >= rect.px1 ? px1 : rect.px1;
            bottom = py1 >= rect.py1 ? py1 : rect.py1;
            right = px2 <= rect.px2 ? px2 : rect.px2;
            top = py2 <= rect.py2 ? py2 : rect.py2;
            // 判断是否相交
            if (left > right || bottom > top)
            {
                return false;
            }
            else
            {
                interRect.px1 = left;
                interRect.py1 = bottom;
                interRect.px2 = right;
                interRect.py2 = top;
                return true;
            }
        }

        // 是否包含（不包括相等）
        bool contains(const EmptyRectArea rect){
            if(*this == rect){
                return false;
            }
            if(px1 <= rect.px1 && px2 >= rect.px2 &&
                    py1 <= rect.py1 && py2 >= rect.py2){
                return true;
            }
            else
            {
                return false;
            }
        }

        // 做差集
        QList<EmptyRectArea> subtraction(const EmptyRectArea rect){
            QList<EmptyRectArea> res;
            EmptyRectArea intersectRect; // 相交矩形
            // 如果两矩形不相交 或 相邻，则返回原矩形
            if(!intersect(rect, intersectRect) || intersectRect.area() == 0){
                res.append(*this);
                return res;
            }
            // 如果矩形包含 或 等于 被减矩形，则返回为空
            EmptyRectArea r = rect;
            if(r.contains(*this) || r == *this){
                return res;
            }

            EmptyRectArea rect1(px1, py1, px2, intersectRect.py1);
            if(rect1.area() != 0) res.append(rect1);

            EmptyRectArea rect2(px1, py1, intersectRect.px1, py2);
            if(rect2.area() != 0) res.append(rect2);

            EmptyRectArea rect3(px1, intersectRect.py2, px2, py2);
            if(rect3.area() != 0) res.append(rect3);

            EmptyRectArea rect4(intersectRect.px2, py1, px2, py2);
            if(rect4.area() != 0) res.append(rect4);
#ifdef NESTDEBUG
            qDebug() << "差集为：" << res.length();
            for(int i=0;i<res.length();i++){
                qDebug() << res[i].leftBottom() << "   " << res[i].rightTop();
            }
#endif
            return res;
        }
        // 运算符重载
        friend bool operator>(const EmptyRectArea &rect1 , const EmptyRectArea &rect2){
            if(rect1.py1 > rect2.py1){
                return true;
            }
            if(rect1.py1 == rect2.py1){
                if(rect1.px1 > rect2.px1){
                    return true;
                }
            }
            return false;
        }
        friend bool operator>=(const EmptyRectArea &rect1 , const EmptyRectArea &rect2){
            if(rect1.py1 >= rect2.py1){
                return true;
            }
            if(rect1.py1 == rect2.py1){
                if(rect1.px1 >= rect2.px1){
                    return true;
                }
            }
            return false;
        }
        friend bool operator==(const EmptyRectArea &rect1 , const EmptyRectArea &rect2){
            if(rect1.px1 == rect2.px1 &&
                    rect1.py1 == rect2.py1 &&
                    rect1.px2 == rect2.px2 &&
                    rect1.py2 == rect2.py2){
                return true;
            }
            return false;
        }
        friend bool operator!=(const EmptyRectArea &rect1 , const EmptyRectArea &rect2){
            if(rect1.px1 == rect2.px1 &&
                    rect1.py1 == rect2.py1 &&
                    rect1.px2 == rect2.px2 &&
                    rect1.py2 == rect2.py2){
                return false;
            }
            return true;
        }
        friend bool operator<(const EmptyRectArea &rect1 , const EmptyRectArea &rect2){
            if(rect1.py1 < rect2.py1){
                return true;
            }
            if(rect1.py1 == rect2.py1){
                if(rect1.px1 < rect2.px1){
                    return true;
                }
            }
            return false;
        }
        friend bool operator<=(const EmptyRectArea &rect1 , const EmptyRectArea &rect2){
            if(rect1.py1 <= rect2.py1){
                return true;
            }
            if(rect1.py1 == rect2.py1){
                if(rect1.px1 <= rect2.px1){
                    return true;
                }
            }
            return false;
        }
    };

    friend class Nest;
    RectNestEngine();
    ~RectNestEngine();

    static void quickSort(QList<EmptyRectArea> &list, int l, int r);
    static void updateEmptyRectArea(EmptyRectArea *eRect);  // 更新空白矩形区域
    static void resetAllParameter();  // 重置参数
    static double fitnessFunc(Genome &g);  // 适应度函数,根据“高度调整填充法”计算
    static void layoutAlg(QVector<double> gVector);  // 排版算法
    static double calculateScore();  // 计算评分

private:
    static QList<Nest::Component> components;  // 零件
    static QList<MinRect> compMinRects;  // 零件的最小矩形
    static QList<EmptyRectArea> emptyRectArea;  // 空白矩形

    static double mWidth;  // 材料宽度
    static double mHeight;  // 材料高度
    static double maxHight;  // 最大高度值
    static long allRectsArea; // 矩形零件面积
    static long minRectsArea; // 最小矩形零件面积
};

#endif // RECTNESTENGINE_H

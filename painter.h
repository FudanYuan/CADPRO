#ifndef PAINTER_H
#define PAINTER_H

#include <QPainter>

class Painter : public QPainter
{
public:
    enum crossType{
        normal,
        upright
    };
    Painter();

    void drawCrossPoint(QPointF point, int offset, crossType type);  // 画交叉点
    void drawNodePoint(QPointF point, int radius);  // 画点
    void drawRectPoint(QPointF point, int length);  // 画矩形点
    void drawLineWithArrow(QLineF line, int offset);
};

#endif // PAINTER_H

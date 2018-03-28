#ifndef COMMON_H
#define COMMON_H

#include <QColor>
#include <QPainter>
#include <QPointF>
#include <QLineF>
#include <qmath.h>

enum crossType{
    none = 0,
    normal = 1,
    upright = 2
};
QColor intToColor(const int rgb, bool a=false); // 将int转华为color
QPointF transformY(QPointF p);  // 转变纵坐标轴
QPointF transformRotate(QPointF o, qreal r, qreal angle); // 返回旋转某一点之后的坐标
//QRectF getLineBoundingRect(QLineF line);
void drawCrossPoint(QPainter* painter, QPointF point, int offset, crossType type);  // 画交叉点
void drawNodePoint(QPainter* painter, QPointF point, int radius);  // 画点
void drawRectPoint(QPainter* painter, QPointF point, int length);  // 画矩形点
void drawLineWithArrow(QPainter *painter, QLineF line, int offset); // 画点箭头的直线

#endif // COMMON_H

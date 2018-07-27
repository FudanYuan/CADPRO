#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <sstream>
#include <QColor>
#include <QPainter>
#include <QPointF>
#include <QLineF>
#include <QVector>
#include <qmath.h>

#define PRECISION 6  // 保留6位小数

enum crossType{
    none = 0,
    normal = 1,
    upright = 2
};
// 类型转换
void str2int(int &int_temp,const std::string &string_temp);
void str2float(float &float_temp,const std::string &string_temp);
void str2double(double &double_temp,const std::string &string_temp);
QColor intToColor(const int rgb, bool a=false); // 将int转华为color

// 计算精度
qreal qrealPrecision(const qreal &dVal, short iPlaces);  // 数的保留小数位数
QPointF pointPrecision(const QPointF &point, short iPlaces);  // 点的保留小数位数
QLineF linePrecision(const QLineF &line, short iPlaces);  // 点的保留小数位数
QRectF rectPrecision(const QRectF &rect, short iPlaces);  // 矩形的保留小数位数
QVector<QPointF> pointsListPrecision(const QVector<QPointF> pointsList, short iPlaces);  // 点集保留小数位数
QVector<QLineF> linesListPrecision(const QVector<QLineF> linesList, short iPlaces);  // 点集保留小数位数

// 坐标转换
QPointF transformY(QPointF p);  // 转变纵坐标轴
QPointF transformRotate(QPointF o, qreal r, qreal angle); // 返回旋转某一点之后的坐标
QPointF transformRotate(QPointF o, QPointF p, qreal angle);  // 返回一点绕另一点旋转的坐标

// 图形计算
qreal calculatePointsDistance(QPointF p1, QPointF p2);  // 获取两点之间的距离
QRectF calculatePolygonBoundingRect(QVector<QPointF> pList);  // 获取边缘矩形
void getRectBoundValue(const QRectF rect, qreal &minX, qreal &minY, qreal &maxX, qreal &maxY);  // 获取矩形边界
bool boundingRectSeperate(const QRectF rect1, const QRectF rect2);  // 判断两矩形是分离
bool boundingRectContain(const QRectF rect1, const QRectF rect2);  // 判断两矩形是否包含，rect1包含rect2
bool pointContainsInPolygon(QVector<QPointF> pList, const QPointF &point);  // 多边形包含某点
bool pointOnPolygonBoundary(QVector<QPointF> pList, const QPointF &point);  // 点在多边形边上
qreal calVerToOppSideXDis(QVector<QPointF> pList);  // 计算多边形顶点到对边的水平距离，返回最大值
qreal calVerToCrossMaxMinDiff(QVector<QPointF> pList, const qreal step, const qreal H, qreal &left);  // 计算各顶点到错开零件各边最大值与最小值的差
qreal calVerToLeftXDis(QVector<QPointF> pList, const qreal H=0);  // 计算多边形各顶点到该多边形外包矩形最左边的水平距离
qreal cal2PolygonMaxMinDiff(QVector<QPointF> pList1, QVector<QPointF> pList2);  // 计算两多边形点到对应边最大值最小值的差
double calculatePolygonArea(QVector<QPointF> points);//多边形面积
bool calculatePolygonDirection(QVector<QPointF> points);//多边形方向顺时针为true，逆时针为false
QPointF calculatePolygonGravityCenter(QVector<QPointF> mPoints);//求多边形重心
bool isConcaveConvex(QPointF ppoint, QPointF cpoint, QPointF npoint, bool direction);//多边形三个点凹凸性,凸为true
double calculatePloygonMinBoundingRectArea(QVector<QPointF> points, qreal &alpha, QRectF &minBoundingRect);//多边形最小包络矩形面积

// 画特殊图形
void drawCrossPoint(QPainter* painter, QPointF point, int offset, crossType type);  // 画交叉点
void drawNodePoint(QPainter* painter, QPointF point, int radius);  // 画点
void drawRectPoint(QPainter* painter, QPointF point, int length);  // 画矩形点
void drawLineWithArrow(QPainter *painter, QLineF line, int offset); // 画点箭头的直线

#endif // COMMON_H

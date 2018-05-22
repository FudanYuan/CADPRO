#include "common.h"
#include <QDebug>
#include <iostream>
#include <iomanip>//要加入头文件

void str2int(int &int_temp, const std::string &string_temp)
{
    std::stringstream stream(string_temp);
    stream>>int_temp;
}

void str2float(float &float_temp, const std::string &string_temp)
{
    std::stringstream stream(string_temp);
    stream>>float_temp;
}

void str2double(double &double_temp, const std::string &string_temp)
{
    std::stringstream stream(string_temp);
    stream>>double_temp;
}

QColor intToColor(const int rgb, bool a)
{
    //将Color 从int 转换成 QColor
    int red = rgb & 255;
    int green = rgb >> 8 & 255;
    int blue = rgb >> 16 & 255;
    int alpha = 255;
    if(a){
        alpha = rgb >> 24 & 255;
    }
    return QColor(red, green, blue, alpha);
}

//qrealPrecision(1.234,2) = 1.23
//qrealPrecision(1.234,0) = 1.0
//qrealPrecision(123.4,-1) = 120.0
qreal qrealPrecision(const qreal &dVal, short iPlaces) {
    qreal dRetval;
    qreal dMod = 0.0000001;
    if (dVal<0.0) dMod=-0.0000001;
    dRetval=dVal;
    dRetval += (5.0 / pow(10.0, iPlaces + 1.0));
    dRetval *= pow(10.0, iPlaces);
    dRetval = floor(dRetval + dMod);
    dRetval /= pow(10.0, iPlaces);
    return dRetval;
}

QPointF pointPrecision(const QPointF &point, short iPlaces){
    return QPointF(qrealPrecision(point.x(), iPlaces),
                   qrealPrecision(point.y(), iPlaces));
}

QLineF linePrecision(const QLineF &line, short iPlaces){
    return QLineF(pointPrecision(line.p1(), iPlaces),
                  pointPrecision(line.p2(), iPlaces));
}

QVector<QPointF> pointsListPrecision(const QVector<QPointF> pointsList, short iPlaces){
    QVector<QPointF> retList;
    for(int i=0; i<pointsList.length(); i++){
        retList.append(pointPrecision(pointsList[i], iPlaces));
    }
    return retList;
}

QVector<QLineF> linesListPrecision(const QVector<QLineF> linesList, short iPlaces){
    QVector<QLineF> retList;
    for(int i=0; i<linesList.length(); i++){
        QLineF line;
        line.setP1(pointPrecision(linesList[i].p1(), iPlaces));
        line.setP2(pointPrecision(linesList[i].p2(), iPlaces));
        retList.append(line);
    }
    return retList;
}

QPointF transformY(QPointF p)
{
    return QPointF(p.rx(), -p.ry());
}

QPointF transformRotate(QPointF o, qreal r, qreal angle)
{
    QPointF res(o.rx()+r*qCos(M_PI*angle/180),
                o.ry()+r*qSin(M_PI*angle/180));
    return res;
}

QPointF transformRotate(QPointF o, QPointF p, qreal angle)
{
    // 将点映射到普通平面直角坐标系
    qreal ox = o.rx();
    qreal oy = -o.ry();
    qreal px = p.rx();
    qreal py = -p.ry();
    qreal a = M_PI*angle/180;
    qreal rx= (px - ox)*qCos(a) - (py - oy)*qSin(a) + ox;
    qreal ry= (px - ox)*qSin(a) + (py - oy)*qCos(a) + oy;
    return QPointF(rx, -ry);
}

qreal calculatePointsDistance(QPointF p1, QPointF p2)
{
    qreal px1 = p1.rx();
    qreal py1 = p1.ry();

    qreal px2 = p2.rx();
    qreal py2 = p2.ry();

    qreal mid1 = qPow((px2-px1), 2);
    qreal mid2 = qPow((py2-py1), 2);
    qreal dis = qSqrt(mid1 + mid2);

    return dis;
}

QRectF calculatePolygonBoundingRect(QVector<QPointF> pList)
{
    int len = pList.length();
    if(len < 1){
        return QRectF();
    }
    if(pList[0] == pList[len-1]){  // 如果首尾相等时，删除最后一个点
        pList.removeAt(len-1);
        len = len-1;
    }
    qreal minX = pList[0].rx();
    qreal minY = pList[0].ry();
    qreal maxX = pList[0].rx();
    qreal maxY = pList[0].ry();
    for(int i=1; i<len-1; i++){
        if(pList[i].rx() < minX){
            minX = pList[i].rx();
        }
        if(pList[i].ry() < minY){
            minY = pList[i].ry();
        }
        if(pList[i].rx() > maxX){
            maxX = pList[i].rx();
        }
        if(pList[i].ry() > maxY){
            maxY = pList[i].ry();
        }
    }
    qreal width = maxX - minX;
    qreal height = maxY - minY;
    QRectF ret(minX, minY, width, height);
    return ret;
}

void getRectBoundValue(const QRectF rect, qreal &minX, qreal &minY, qreal &maxX, qreal &maxY)
{
    QPointF topLeftPoint = rect.topLeft();  // 左上角坐标
    QPointF bottomRightPoint = rect.bottomRight();  // 右下角坐标
    // 获取边界值
    minX = topLeftPoint.rx();
    minY = topLeftPoint.ry();
    maxX = bottomRightPoint.rx();
    maxY = bottomRightPoint.ry();
}

bool boundingRectSeperate(const QRectF rect1, const QRectF rect2)
{
    qreal minX1, minX2, minY1, minY2, maxX1, maxX2, maxY1, maxY2;
    getRectBoundValue(rect1, minX1, minY1, maxX1, maxY1);
    getRectBoundValue(rect2, minX2, minY2, maxX2, maxY2);
    if(minX1 >= maxX2
            || maxX1 <= minX2
            || minY1 >= maxY2
            || maxY1 <= minY2){
        return true;
    }
    return false;
}

bool boundingRectContain(const QRectF rect1, const QRectF rect2)
{
    qreal minX1, minX2, minY1, minY2, maxX1, maxX2, maxY1, maxY2;
    getRectBoundValue(rect1, minX1, minY1, maxX1, maxY1);
    getRectBoundValue(rect2, minX2, minY2, maxX2, maxY2);

    //qDebug() << "";
    //qDebug() << minX1 << "  " << maxX1 << "  " << minY1 << "  " << maxY1;
    //qDebug() << minX2 << "  " << maxX2 << "  " << minY2 << "  " << maxY2;
    // 最终使用这种
    if(minX1 > minX2 || maxX1 < maxX2 || minY1 > minY2 || maxY1 < maxY2){
        //qDebug() << "false";
        return false;
    }

    return true;
}

bool pointContainsInPolygon(QVector<QPointF> pList, const QPointF &point)
{
    int len = pList.length();
    if(pList[0] == pList[len-1]){  // 如果首尾相等时，删除最后一个点
        pList.removeAt(len-1);
        len = len-1;
    }
    int nCross = 0;
    for (int i = 0; i < len; i++) {
        QPointF p1 = pList[i];
        QPointF p2 = pList[(i+1)%len];
        // 取多边形任意一个边,做点point的水平延长线,求解与当前边的交点个数
        // p1p2是水平线段,要么没有交点,要么有无限个交点
        if (p1.y() == p2.y()){
            double minX = qMin(p1.x(), p2.x());
            double maxX = qMax(p1.x(), p2.x());
            // point在水平线段p1p2上,直接return false
            if ((point.y() == p1.y()) && (point.x() >= minX && point.x() <= maxX)) {
                return false;
            }
            continue;
        }
        // point 在p1p2 底部 --> 无交点
        if (point.y() < qMin(p1.y(), p2.y())){
            continue;
        }
        // point 在p1p2 顶部 --> 无交点
        if (point.y() >= qMax(p1.y(), p2.y())){
            continue;
        }
        // 求解point点水平线与当前p1p2边的交点的 X 坐标
        double x = (point.y() - p1.y()) * (p2.x() - p1.x())
                / (p2.y() - p1.y()) + p1.x();
        if (x == point.x()){  // 当x=point.x时,说明point在p1p2线段上
            return false;
        }
        if (x > point.x()){  // 只统计单边交点
            nCross++;
        }
    }
    // 单边交点为偶数，点在多边形之外 ---
    return (nCross % 2 == 1);
}

bool pointOnPolygonBoundary(QVector<QPointF> pList, const QPointF &point)
{
    int len = pList.length();
    if(pList[0] == pList[len-1]){  // 如果首尾相等时，删除最后一个点
        pList.removeAt(len-1);
        len = len-1;
    }
    for (int i = 0; i < len; i++) {
        QPointF p1 = pList[i];
        QPointF p2 = pList[(i+1)%len];

        // 判断是否为顶点
        if(point == p1 || point == p2){
            return true;
        }
        // 取多边形任意一个边,做点point的水平延长线,求解与当前边的交点个数
        // p1p2是水平线段,要么没有交点,要么有无限个交点
        if (p1.y() == p2.y()){
            double minX = qMin(p1.x(), p2.x());
            double maxX = qMax(p1.x(), p2.x());
            // point在水平线段p1p2上,直接return true
            if ((point.y() == p1.y()) && (point.x() >= minX && point.x() <= maxX)) {
                return true;
            }
            continue;
        }
        // point 在p1p2 底部 --> 无交点
        if (point.y() < qMin(p1.y(), p2.y())){
            continue;
        }
        // point 在p1p2 顶部 --> 无交点
        if (point.y() > qMax(p1.y(), p2.y())){
            continue;
        }
        double x = (point.y() - p1.y()) * (p2.x() - p1.x())
                / (p2.y() - p1.y()) + p1.x();
        if (x == point.x()) { // 当x=point.x时,说明point在p1p2线段上
            return true;
        }
    }
    return false;
}

double calculatePolygonArea(QVector<QPointF> points){
    double area=0;
    if(points.length()==2){
        return 0;
    }else{
        for(int i=0;i<points.length()-1;i++){
            area=area+(points[i].rx()*points[i+1].ry())-(points[i].ry()*points[i+1].rx());
        }
        return abs(area)/2;
    }
}

bool calculatePolygonDirection(QVector<QPointF> points){

    int flag, pp, np, cp=0;
    if(points.length()==2){
        return false;
    }
    for(int i=0;i<points.length()-1;i++){
        if( points[cp].ry() <= points[i].ry()){
            cp=i;
        }
    }
    pp=(cp-1+points.length())%points.length();
    np=(cp+1)%points.length();

    flag=(points[cp].rx() - points[pp].rx())*(points[np].ry()- points[cp].ry())
            - (points[cp].ry() - points[pp].ry())*(points[np].rx()- points[cp].rx());
    if(flag>0){
        return true;
    }
    return false;
}

QPointF calculatePolygonGravityCenter(QVector<QPointF> mPoints){
    double area = 0.0;//多边形面积
    double Gx = 0.0, Gy = 0.0;// 重心的x、y
    for (int i = 1; i <= mPoints.length(); i++) {
        double irx = mPoints[i % mPoints.length()].rx();
        double iry = mPoints[i % mPoints.length()].ry();
        double nextLat = mPoints[i - 1].rx();
        double nextLng = mPoints[i - 1].ry();
        double temp = (irx * nextLng - iry * nextLat) / 2.0;
        area += temp;
        Gx += temp * (irx + nextLat) / 3.0;
        Gy += temp * (iry + nextLng) / 3.0;
        }
        Gx = Gx / area;
        Gy = Gy / area;
        return QPointF(Gx, Gy);
}

bool isConcaveConvex(QPointF ppoint,QPointF cpoint,QPointF npoint,bool direction){

    int flag=(cpoint.rx() - ppoint.rx())*(npoint.ry()- cpoint.ry())-(cpoint.ry() - ppoint.ry())*(npoint.rx()- cpoint.rx());
    if (flag>0 && direction == true){
        return true;
    }else if(flag<0 && direction == true){
        return false;
    }else if(flag<0 && direction == false){
        return true;
    }else{
        return false;
    }
}

double calculatePloygonMinBoundingRectArea(QVector<QPointF> &points, qreal &alpha, QRectF &minBoundingRect){
    QPointF maxp,minp,centerpont = QPointF(0,0); //calculatePolygonGravityCenter(points);
    QVector<QPointF> listpoint;
    qreal angles[points.length()];
    double area,areas[points.length()];
    int minid,w,h;
    //qDebug()<<"中心点"<<centerpont;
    for(int i=0;i<points.length();i++){
        QLineF line0(points[i], QPointF(points[i].rx() + 10, points[i].ry()));
        QLineF line1(points[i], points[(i+1)%points.length()]);
        qreal angle = line0.angleTo(line1);
        //qDebug()<<"角度"<<angle;

        QVector<QPointF> cpoints;
        for(int j=0;j<points.length();j++){
            cpoints.append(transformRotate(centerpont, points[j], -angle));
        }
        maxp = minp = cpoints[0];
        for(int j=1;j<points.length();j++){
            if(maxp.rx() < cpoints[j].rx()){
                maxp.rx() = cpoints[j].rx();
            }
            if(maxp.ry() < cpoints[j].ry()){
                maxp.ry() = cpoints[j].ry();
            }
            if(minp.rx() > cpoints[j].rx()){
                minp.rx() = cpoints[j].rx();
            }
            if(minp.ry() > cpoints[j].ry()){
                minp.ry() = cpoints[j].ry();
            }
        }
        angles[i]=angle;
        listpoint.append(maxp);
        listpoint.append(minp);
        //qDebug() << maxp << "   " << minp;
        areas[i]= (maxp.rx()-minp.rx())*(maxp.ry()-minp.ry());
        //qDebug()<<i<<"面积"<<areas[i];
    }
    area=areas[0];
    for(int i=0;i<points.length();i++){
        if(area>=areas[i]){
            minid=i;
            area=areas[i];
        }
    }
    alpha=angles[minid];
    QVector<QPointF> cpoints;
    for(int j=0;j<points.length();j++){
        cpoints.append(transformRotate(centerpont, points[j], -alpha));
    }
    points.clear();
    points.append(cpoints);
    w=abs(listpoint[minid*2].rx()-listpoint[minid*2+1].rx());
    h=abs(listpoint[minid*2].ry()-listpoint[minid*2+1].ry());
    minBoundingRect.setRect(listpoint[minid*2+1].rx(), listpoint[minid*2+1].ry(),w,h);
    //qDebug()<<"最小包络id:"<<minid<<"面积:"<<area<<"角度："<<angles[minid];
    return area;
}

void drawCrossPoint(QPainter *painter, QPointF point, int offset=2, crossType type=upright)
{
    qreal rx = point.rx();
    qreal ry = point.ry();
    switch (type) {
    case normal:
        painter->drawLine(QPointF(rx-offset, ry+offset), QPointF(rx+offset, ry-offset));
        painter->drawLine(QPointF(rx+offset, ry+offset), QPointF(rx-offset, ry-offset));
        break;
    case upright:
        painter->drawLine(QPointF(rx, ry+offset), QPointF(rx, ry-offset));
        painter->drawLine(QPointF(rx+offset, ry), QPointF(rx-offset, ry));
        break;
    default:
        break;
    }
}

void drawNodePoint(QPainter *painter, QPointF point, int radius=2)
{
    painter->drawEllipse(point, radius, radius);
}

void drawRectPoint(QPainter *painter, QPointF point, int length=2)
{
    qreal rx = point.rx();
    qreal ry = point.ry();
    painter->drawRect(rx-length, ry-length, 2*length, 2*length);
}

void drawLineWithArrow(QPainter *painter, QLineF line, int offset)
{
    if(line.dx() == line.dy()){
        return;
    }
    QLineF v = line.unitVector();
    v.setLength(offset);
    v.translate(QPointF(line.dx(), line.dy()));

    QLineF n = v.normalVector();
    n.setLength(n.length() * 0.5);
    QLineF n2 = n.normalVector().normalVector();

    QPointF p1 = v.p2();
    QPointF p2 = n.p2();
    QPointF p3 = n2.p2();

    painter->drawLine(line);
    QVector<QPointF> vector;
    vector << p1 << p2 << p3 << p1;
    QPolygonF polygon(vector);
    painter->drawPolygon(polygon);
}


QRectF rectPrecision(const QRectF &rect, short iPlaces)
{
    QPointF topLeft = pointPrecision(rect.topLeft(), iPlaces);
    qreal width = qrealPrecision(rect.width(), iPlaces);
    qreal height = qrealPrecision(rect.height(), iPlaces);
    return QRectF(topLeft.rx(), topLeft.ry(), width, height);
}

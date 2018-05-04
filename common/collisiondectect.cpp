#include "collisiondectect.h"
#include <qmath.h>

#include <QDebug>

CollisionDectect::CollisionDectect(QVector<QPointF> pList1, QVector<QPointF> pList2, bool isCircle1, bool isCircle2)
{
    this->pList1 = pList1;
    this->pList2 = pList2;
    this->isCircle1 = isCircle1;
    this->isCircle2 = isCircle2;
}

CollisionDectect::ItemType CollisionDectect::getItemType(QVector<QPointF> pList)
{
    ItemType retType;
    int len = pList.length();
    if(len == 1){
        retType = Point;
    } else if(len == 2){
        retType = Line;
    } else{
        ConcavePolygon concavePolygon(pList);
        if(concavePolygon.isConcavePolygon(pList)){
            retType = ConcavePoly;
        } else {
            retType = ConvexPoly;
        }
    }
    return retType;
}

/**
 * @brief CollisionDectect::getBoundingRect
 * 包围盒碰撞检测.顺序:左上右下四个值→Xmin,Ymin,Xmax,Ymax
 * @param pList
 * @return
 */
QVector<qreal> CollisionDectect::getBoundingRect(QVector<QPointF> pList)
{
    int len = pList.length();
    if(len < 1){
        return QVector<qreal>(4);
    }
    qreal minX = pList[0].rx();
    qreal minY = pList[0].ry();
    qreal maxX = pList[0].rx();
    qreal maxY = pList[0].ry();
    for(int i=1; i<len; i++){
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

    QVector<qreal> ret;
    ret.append(minX);
    ret.append(minY);
    ret.append(maxX);
    ret.append(maxY);

    return ret;
}

CollisionDectect::CircleInfo CollisionDectect::getBoundingCircle(QVector<QPointF> pList)
{
    int len = pList.length();
    if(len != 2){
        return CircleInfo();
    }
    return CircleInfo(pList[0], pList[1].rx());
}

bool CollisionDectect::circleHit(QVector<QPointF> pList1, QVector<QPointF> pList2)
{
    if(pList1.length() != 2 || pList2.length() != 2){
        return false;
    }
    qreal dis2 = qPow(pList1[0].rx()-pList2[0].rx(), 2)
            + qPow(pList1[0].ry()-pList2[0].ry(), 2);
    qreal rd = qPow(pList1[1].rx()+pList2[1].rx(), 2);
    return dis2 < rd;
}

bool CollisionDectect::boundHit(QVector<qreal> bound1, QVector<qreal> bound2)
{
    if(bound1.length() != 4 || bound2.length() != 4){
        return false;
    }
    if(!overlap(QPointF(bound1[0],bound1[2]),QPointF(bound2[0], bound2[2]))) {
        return false;
    }
    if(!overlap(QPointF(bound1[1],bound1[3]),QPointF(bound2[1], bound2[3]))) {
        return false;
    }
    return true;
}

qreal CollisionDectect::dotProduct(QPointF v1, QPointF v2)
{
    return v1.rx() * v2.rx() + v1.ry() * v2.ry();
}

QPointF CollisionDectect::normalize(QPointF v)
{
    int n = qSqrt(v.rx() * v.rx() + v.ry() * v.ry());
    if(n <= 0){
        return v;
    }
    v.setX(v.rx() / n);
    v.setY(v.ry() / n);
    return v;
}

QPointF CollisionDectect::perpendicular(QPointF v)
{
    qreal tmp = v.rx();
    v.setX(v.ry());
    v.setY(-tmp);
    return v;
}

QVector<QPointF> CollisionDectect::getSeparatingAxis(QVector<QPointF> pList, QVector<QPointF> separatingAxis)
{
    int len = pList.length();
    bool flag = false;
    QPointF nor, segment;
    for(int i=0; i<len; i++){
        if(i >= len-1){
            segment.setX(pList[0].rx() - pList[i].rx());
            segment.setY(pList[0].ry() - pList[i].ry());
        } else{
            segment.setX(pList[i+1].rx() - pList[i].rx());
            segment.setY(pList[i+1].ry() - pList[i].ry());
        }
        nor = perpendicular(normalize(segment));
        if(nor.rx() <= 0){
            if(nor.rx() == 0){
                if(nor.ry() < 0) {
                    nor.setY(-nor.ry());
                } else {
                    nor.setX(-nor.rx());
                    nor.setY(-nor.ry());
                }
            }
        }
        flag = true;
        //这里如过边数比较多可能会有些消耗，可以修改为二分法查找，这里只为演示功能，简单处理
        for(int j=0; j<separatingAxis.length(); j++){
            if(separatingAxis[j].rx() != nor.rx()){
                continue;
            }
            if(separatingAxis[j].ry() != nor.ry()){
                continue;
            }
            flag = false;
            break;
        }
        if(!flag){
            continue;
        }
        separatingAxis.append(nor);
    }
    return separatingAxis;
}

bool CollisionDectect::overlap(QPointF s1, QPointF s2)
{
    if(s1.rx() > s2.ry() || s1.ry() < s2.rx()){
        return false;
    }
    return true;
}

QPointF CollisionDectect::getPolygonProjection(QVector<QPointF> pList, QPointF axis)
{
    QPointF ret;
    qreal min = dotProduct(pList[0], axis);
    qreal max = min;
    for(int i=1; i<pList.length(); i++){
        qreal n = dotProduct(pList[i], axis);
        if(n < min){
            min = n;
        }
        if(n > max){
            max = n;
        }
    }
    ret.setX(min);
    ret.setY(max);
    return ret;
}

QPointF CollisionDectect::getCircleProjection(QVector<QPointF> pList, QPointF axis)
{
    QPointF ret;
    ret.setX(dotProduct(pList[0], axis) - pList[1].rx());
    ret.setY(ret.rx() + 2 * pList[1].rx());
    return ret;
}

bool CollisionDectect::collision()
{
    if(pList1.length() < 1 && pList2.length() < 1){
        return false;
    }
    qDebug() << "长度都符合规范" ;
    bool isConcavePoly1 = getItemType(pList1) == ConcavePoly;
    bool isConcavePoly2 = getItemType(pList2) == ConcavePoly;
    // 如果两多边形都不为凹多边形
    qDebug() << isConcavePoly1 << "  " << isConcavePoly2;
    if(!isConcavePoly1 && !isConcavePoly2){
        qDebug() << "两多边形都不为凹多边形" ;
        return convexPolygonCollision(pList1, pList2, isCircle1, isCircle2);
    }
    // 如果两多边形都为凹多边形
    if(isConcavePoly1 && isConcavePoly2){
        qDebug() << "两多边形都为凹多边形" ;
        ConcavePolygon concavePoly1(pList1);
        ConcavePolygon concavePoly2(pList2);
        QMap<int, QVector<QPointF>> splitRes1 = concavePoly1.onSeparateConcavePoly(pList1);
        QMap<int, QVector<QPointF>> splitRes2 = concavePoly2.onSeparateConcavePoly(pList2);
        for(int i=0; i<splitRes1.size(); i++){
            for(int j=0; j<splitRes2.size(); j++){
                if(convexPolygonCollision(splitRes1[i], splitRes2[j])){
                    return true;
                }
            }
        }
        return false;
    }
    // 如果两多边形不全为凹多边形
    if(isConcavePoly1){
        qDebug() << "第一个图形为凹多边形" ;
        ConcavePolygon concavePoly1(pList1);
        QMap<int, QVector<QPointF>> splitRes1 = concavePoly1.onSeparateConcavePoly(pList1);
        for(int i=0; i<splitRes1.size(); i++){
            if(convexPolygonCollision(splitRes1[i], pList2)){
                return true;
            }
        }
        return false;
    }
    if(isConcavePoly2){
        qDebug() << "第二个图形为凹多边形" ;
        ConcavePolygon concavePoly2(pList2);
        QMap<int, QVector<QPointF>> splitRes2 = concavePoly2.onSeparateConcavePoly(pList2);
        for(int i=0; i<splitRes2.size(); i++){
            if(convexPolygonCollision(splitRes2[i], pList2)){
                return true;
            }
        }
        return false;
    }
    return false;
}

bool CollisionDectect::convexPolygonCollision(QVector<QPointF> pList1, QVector<QPointF> pList2, bool isCircle1, bool isCircle2)
{
    if(pList1.length() < 1 && pList2.length() < 1){
        return false;
    }

    // 如果两个图形都是圆，则直接进行圆的碰撞检测
    if(isCircle1 && isCircle2){
        return circleHit(pList1, pList2);
    }

    // 如果至少有一个不是圆，则需要利用分离轴进行碰撞检测
    QVector<QPointF> separatingAxis;
    if(!isCircle1){
        separatingAxis = getSeparatingAxis(pList1, separatingAxis);
    }
    if(!isCircle2){
        separatingAxis = getSeparatingAxis(pList2, separatingAxis);
    }
    QPointF extreme1, extreme2;
    for(int i=0; i<separatingAxis.length(); i++){
        if(isCircle1){
            extreme1 = getCircleProjection(pList1, separatingAxis[i]);
        } else{
            extreme1 = getPolygonProjection(pList1, separatingAxis[i]);
        }
        if(isCircle2){
            extreme2 = getCircleProjection(pList2, separatingAxis[i]);
        } else{
            extreme2 = getPolygonProjection(pList2, separatingAxis[i]);
        }
        if(!overlap(extreme1, extreme2)){
            return false;
        }
    }
    return false;
}

ConcavePolygon::ConcavePolygon(QVector<QPointF> list)
{
    pList = list;
}

bool ConcavePolygon::isConcavePolygon(QVector<QPointF> pList)
{
    if(pList.length() <= 3){
        return false;
    }
    qDebug() << "检测该多边形是否为凹多边形" ;
    return getNextConcaveIndex(pList, 0) >=0;
}

QMap<int, QVector<QPointF>> ConcavePolygon::onSeparateConcavePoly(QVector<QPointF> pList)
{
    QMap<int, QVector<QPointF>> ret;
    // 默认为逆时针，否反转多边形
    if(!isAntiClockDir(pList)){
        qDebug() << "顺时针";
        conversPoly(pList);
        for(int i=0; i<pList.length(); i++){
            qDebug() << pList[i];
        }
    }
    _separateConcavePoly(pList, ret, 0);
    return ret;
}

int ConcavePolygon::getNextConcaveIndex(QVector<QPointF> pList, int startIndex)
{
    int len = pList.length();
    if(len <= 3){
        return -1;
    }
    int curDir = 0;
    int nMax = len + startIndex;
    for(int i=startIndex; i<nMax; i++){
        curDir = getMutiPtClockDir(pList[(i + len) % len],
                pList[(i - 1 + len) % len],
                pList[(i + 1 + len) % len]);
        if(curDir == AnticlockWise){
            return i % len;
        }
    }
    return -1;
}

QVector<int> ConcavePolygon::getAllConcaveIndex(QVector<QPointF> pList, int startIndex)
{
    int len = pList.length();
    if(len <= 3){
        return QVector<int>();
    }
    QVector<int> ret;
    int curDir = 0;
    int nMax = len + startIndex;
    for(int i=startIndex; i<nMax; i++){
        curDir = getMutiPtClockDir(pList[(i + len) % len],
                pList[(i - 1 + len) % len],
                pList[(i + 1 + len) % len]);
        if(curDir == AnticlockWise){
            ret.append(i % len);
        }
    }
    return ret;
}

void ConcavePolygon::_separateConcavePoly(QVector<QPointF> pList, QMap<int, QVector<QPointF>> &map, int startIndex)
{
    int len = pList.length();
    if(len <= 3){
        qDebug() << "map.size: " << map.size();
        map.insert(map.size(), pList);
        return;
    }
    int nextConcaveIndex = getNextConcaveIndex(pList, startIndex);
    qDebug() << "下一凹点：" << pList[nextConcaveIndex];
    startIndex = nextConcaveIndex + 1;
    if(nextConcaveIndex < 0){
        map.insert(map.size(), pList);
        return;
    }
    IntersectionPoint intersectionPoint;
    if(getSplitPointByVertexRegion(pList, nextConcaveIndex, intersectionPoint)){
        qDebug() << "交点" << intersectionPoint.index << "  " << intersectionPoint.intersection;
        QVector<QPointF> pLeftList;
        QVector<QPointF> pRightList;
        splitPolyByIntersection(pList, nextConcaveIndex, intersectionPoint, pLeftList, pRightList);
        _separateConcavePoly(pLeftList, map, startIndex);
        _separateConcavePoly(pRightList, map, startIndex);
        return;
    }
    map.insert(map.size(), pList);
}

bool ConcavePolygon::splitPolyByIntersection(QVector<QPointF> pList, int concaveIndex, ConcavePolygon::IntersectionPoint intersectionPoint, QVector<QPointF> &pLeftList, QVector<QPointF> &pRightList)
{
    int len = pList.length();
    if(concaveIndex < 0 || concaveIndex >= len){
        return false;
    }
    int intersectionPointIndex = intersectionPoint.index;
    if(intersectionPointIndex < 0 || intersectionPointIndex >= len){
        return false;
    }
    int i = 0;
    // 左边矩形
    if(concaveIndex <= intersectionPointIndex + 1){
        for(i = intersectionPointIndex; i <= concaveIndex + len; i++){
            QPointF pn1 = pList[i % len];
            pLeftList.append(pn1);
        }
    } else{
        for(i = intersectionPointIndex; i <= concaveIndex; i++){
            QPointF pn2 = pList[i];
            pLeftList.append(pn2);
        }
    }
    // 右边矩形
    if(concaveIndex <= intersectionPointIndex){
        for(i = concaveIndex; i <= intersectionPointIndex; i++){
            QPointF pn3 = pList[i];
            pRightList.append(pn3);
        }
    } else{
        for(i = concaveIndex; i <= intersectionPointIndex + len; i++){
            QPointF pn4 = pList[i % len];
            pRightList.append(pn4);
        }
    }
    int index = intersectionPoint.index;
    if(pList[index].rx() != intersectionPoint.intersection.rx()
            && pList[index].ry() != intersectionPoint.intersection.ry()){
        pLeftList.append(intersectionPoint.intersection);
        pRightList.append(intersectionPoint.intersection);
    }
    return true;
}

bool ConcavePolygon::getSplitPointByVertexRegion(QVector<QPointF> pList, int concaveIndex, ConcavePolygon::IntersectionPoint &intersectionPoint)
{
    int len = pList.length();
    if(len <= 3){
        return false;
    }
    qDebug() << concaveIndex << "";
    int preIndex = concaveIndex > 0 ? concaveIndex - 1 : len - 1;
    int nextIndex = concaveIndex < len - 1 ? concaveIndex + 1 : 0;
    int ret1 = 0;
    int ret2 = 0;
    // 分区计算
    QVector<int> A;
    QVector<int> B;
    QVector<int> C;
    QVector<int> D;
    // 可见点区域
    QVector<int> A1;
    QVector<int> B1;
    QVector<int> C1;
    QVector<int> D1;
    int nMax = (nextIndex <= preIndex) ? preIndex : preIndex + len;
    int nCur = 0;
    for(int i=nextIndex; i<=nMax; i++){
        nCur = i % len;
        ret1 = getMutiPtClockDir(pList[concaveIndex], pList[preIndex], pList[nCur]);
        ret2 = getMutiPtClockDir(pList[concaveIndex], pList[nextIndex], pList[nCur]);
        // 计算所在区域
        if(ret1 < 0 && ret2 > 0){
            A.append(nCur);
        } else if(ret1 >= 0 && ret2 >= 0){
            B.append(nCur);
        } else if(ret1 <= 0 && ret2 < 0){
            C.append(nCur);
        } else if(ret1 > 0 && ret2 < 0){
            D.append(nCur);
        }
    }
    // 取可见点分区
    visibleRegionPtSet(pList, concaveIndex, A, A1);
    B1.append(B);
    visibleRegionPtSet(pList, concaveIndex, C, C1);
    D1.append(D);
    if(A1.length() > 0){
        QVector<int> setA, setB;
        setSplitByRegion(pList, A1, setA, setB);
        if(setB.length() > 0){
            intersectionPoint.index = getBestIntersectionPt(pList, concaveIndex, setB);
        } else{
            intersectionPoint.index = getBestIntersectionPt(pList, concaveIndex, setA);
        }
        if(intersectionPoint.index < 0 ||
                intersectionPoint.index >= pList.length()){
            return false;
        }
        intersectionPoint.intersection = pList[intersectionPoint.index];
        return true;
    }
    // 如果A为空，BC必不为空
    if(B1.length() < 1 || C1.length() < 1){
        // qDebug() << "BC分区为空错误";
        return false;
    }
    int mid = concaveIndex;
    int left = B1[B1.length() - 1];
    int right = C1[0];
    // BC区域的首位点必在一条直线上
    QPointF d1(pList[right].rx() - pList[left].rx(),
               pList[right].ry() - pList[left].ry());
    QPointF d00(pList[concaveIndex].rx() - pList[preIndex].rx(),
                pList[concaveIndex].ry() - pList[preIndex].ry());
    QPointF d01(pList[concaveIndex].rx() - pList[nextIndex].rx(),
                pList[concaveIndex].ry() - pList[nextIndex].ry());
    // A区域与交点区域的角平分线
    QPointF d0((d00.rx() + d01.rx()) / 2,
               (d00.ry() + d01.ry()) / 2);
    QPointF crossPt(0, 0);
    if(getCrossByRadialAndSegment(pList[concaveIndex], d0, pList[left], d1, crossPt) != 1){
        return false;
    }
    intersectionPoint.index = left;
    intersectionPoint.intersection = crossPt;
    return true;
}

ConcavePolygon::PolyDirection ConcavePolygon::getMutiPtClockDir(QPointF p1, QPointF p2, QPointF p3)
{
    qreal ret = (p1.rx() - p2.rx()) * (p3.ry() - p1.ry()) - (p2.rx() - p1.rx()) * (p1.ry() - p2.ry());
    return ret > 0 ? AnticlockWise : (ret < 0 ? Clockwise : InALine);
}

ConcavePolygon::PolyDirection ConcavePolygon::getMutiPtClockDirByIndex(QVector<QPointF> pList, int index)
{
    int len = pList.length();
    if(len <= 2){
        return InALine;
    }
    return getMutiPtClockDir(pList[index % len], pList[index+len-1], pList[(index+1)%len]);
}

bool ConcavePolygon::isAntiClockDir(QVector<QPointF> pList)
{
    int len = pList.length();
    int count = 0;
    for(int i=0; i<len; i++){
        int nextIndex1 = (i + 1) % len;
        int nextIndex2 = (i + 2) % len;
        int n = (pList[nextIndex1].rx() - pList[i].rx()) * (pList[nextIndex2].ry() - pList[nextIndex1].ry());
        n -= (pList[nextIndex1].ry() - pList[i].ry()) * (pList[nextIndex2].rx() - pList[nextIndex1].rx());
        if(n < 0){
            count--;
        } else if(n > 0){
            count++;
        }
    }
    return count <= 0;
}

void ConcavePolygon::conversPoly(QVector<QPointF> &pList)
{
    int len = pList.length();
    int i = -1;
    while(++i < len / 2){
        QPointF pn1, pn2;
        pn1 = pList[i];
        pn2 = pList[len - i - 1];
        pList[i] = pn2;
        pList[len - i - 1] = pn1;
    }
}

bool ConcavePolygon::isVectorInsection(QPointF p1, QPointF d1, QPointF p2, QPointF d2)
{
    QPointF ePoint(p2.rx()-p1.rx(), p2.ry()-p2.ry());
    qreal cross = d1.rx() * d2.ry() - d1.ry() * d2.rx();
    qreal sqrtCross = cross * cross;
    qreal sqrtLen1 = d1.rx() * d1.rx() + d1.ry() + d1.ry();
    qreal sqrtLen2 = d2.rx() * d2.rx() + d2.ry() + d2.ry();
    qreal sqrtEpsilon = 0.01;
    if(sqrtCross > sqrtEpsilon * sqrtLen1 * sqrtLen2){
        // lines of the segments are not parallel
        qreal s = (ePoint.rx() * d2.ry() - ePoint.ry() * d2.rx()) / cross;
        if(s < 0 || s > 1){
            //intersection of lines is not a point on segment P0 + s * D0
            return false;
        }
        qreal t = (ePoint.rx() * d1.ry() - ePoint.ry() * d1.rx()) / cross;
        if(t < 0 || t > 1){
            return false;
        }
        return true;
    }
    return false;
}

bool ConcavePolygon::isVisiblePtToConcave(QVector<QPointF> pList, int index1, int index2)
{
    int nextIndex = 0;
    int len = pList.length();
    for(int i=0; i<len; i++){
        nextIndex = (i >= len - 1) ? 0 : i + 1;
        if(i == index1
                || i == index2
                || nextIndex == index1
                || nextIndex == index2){
            continue;
        }
        if(isVectorInsection(pList[index1], pList[index2], pList[i], pList[nextIndex])){
            return false;
        }
    }
    return true;
}

void ConcavePolygon::visibleRegionPtSet(QVector<QPointF> pList, int concaveIndex, QVector<int> region, QVector<int> &region1)
{
    int i = -1;
    while(++i < region.length()){
        if(isVisiblePtToConcave(pList, concaveIndex, region[i])){
            region1.append(region[i]);
        }
    }
}

void ConcavePolygon::setSplitByRegion(QVector<QPointF> pList, QVector<int> region, QVector<int> &setA, QVector<int> &setB)
{
    int i = -1;
    while(++i < region.length()){
        if(getMutiPtClockDirByIndex(pList, region[i])){
            setA.append(region[i]);
        } else{
            setB.append(region[i]);
        }
    }
}

void ConcavePolygon::normalizeVector(QPointF &v)
{
    int n = qSqrt(v.rx() * v.rx() + v.ry() * v.ry());
    if(n <= 0){
        return;
    }
    v.setX(v.rx() / n);
    v.setY(v.ry() / n);
}

qreal ConcavePolygon::dotVector(QPointF v1, QPointF v2)
{
    return v1.rx() * v2.rx() + v1.ry() * v2.ry();
}

int ConcavePolygon::getBestIntersectionPt(QVector<QPointF> pList, int concaveIndex, QVector<int> ptSets)
{
    if(ptSets.length() < 1){
        return -1;
    }
    int len = pList.length();
    int bestIndex = -1;
    int preIndex = (len + concaveIndex - 1) % len;
    int nextIndex = (len + concaveIndex + 1) % len;
    QPointF d00(pList[concaveIndex].rx() - pList[preIndex].rx(),
                pList[concaveIndex].ry() - pList[preIndex].ry());
    QPointF d01(pList[concaveIndex].rx() - pList[nextIndex].rx(),
                pList[concaveIndex].ry() - pList[nextIndex].ry());
    normalizeVector(d00);
    normalizeVector(d01);
    int fLen = -1;
    int i = -1;
    while(++i < ptSets.length()){
        QPointF tp = pList[ptSets[i]];
        QPointF dp(tp.rx() - pList[concaveIndex].rx(),
                   tp.ry() - pList[concaveIndex].ry());
        normalizeVector(dp);
        qreal nTemLen = qAbs(dotVector(d00, dp));
        if(nTemLen > fLen){
            bestIndex = ptSets[i];
        }
    }
    return bestIndex;
}

bool ConcavePolygon::getCrossByRadialAndSegment(QPointF p1, QPointF d1, QPointF p2, QPointF d2, QPointF &crossPoint)
{
    QPointF ePoint(p2.rx()-p1.rx(), p2.ry()-p2.ry());
    qreal cross = d1.rx() * d2.ry() - d1.ry() * d2.rx();
    qreal sqrtCross = cross * cross;
    qreal sqrtLen1 = d1.rx() * d1.rx() + d1.ry() + d1.ry();
    qreal sqrtLen2 = d2.rx() * d2.rx() + d2.ry() + d2.ry();
    qreal sqrtEpsilon = 0.01;
    if(sqrtCross > sqrtEpsilon * sqrtLen1 * sqrtLen2){
        // lines of the segments are not parallel
        qreal s = (ePoint.rx() * d2.ry() - ePoint.ry() * d2.rx()) / cross;
        if(s < 0){
            return false;
        }
        qreal t = (ePoint.rx() * d1.ry() - ePoint.ry() * d1.rx()) / cross;
        if(t < 0 || t > 1){
            //intersection of lines is not a point on segment P1 + t * D1
            return false;
        }
        QPointF pt(d1.rx() * s, d1.ry() * s);
        crossPoint.setX(p1.rx() + pt.rx());
        crossPoint.setY(p1.ry() + pt.ry());
        return true;
    }
    return false;
}

ConcavePolygon::PointRelationToLine ConcavePolygon::GetPosRelationToLine(QPointF sPoint, QPointF ePoint, QPointF p)
{
    qreal ret = p.rx() * (ePoint.ry() - sPoint.ry())
            + p.ry() * (sPoint.rx() - ePoint.rx())
            + sPoint.ry() * ePoint.rx()
            - sPoint.rx() * ePoint.ry();
    return ret > 0 ? RightLine : (ret < 0 ? LeftLine : InLine) ;
}




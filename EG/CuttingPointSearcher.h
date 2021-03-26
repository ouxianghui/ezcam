#ifndef CuttingPointSearcher_H
#define CuttingPointSearcher_H

#include "eg_global.h"
#include <vector>
#include "Typedef.h"
#include "RVector.h"
#include "RBox.h"
#include "qvector.h"
#include "BoardRectTable.h"

struct XSegment
{
    double startX;
    double endX;
};

struct YSegment
{
    double startY;
    double endY;
};

struct aboveRectSegment
{
    XSegment xs;
    RectF rect;
};

struct belowRectSegment
{
    XSegment xs;
    RectF rect;
};

struct rightRectSegment
{
    YSegment ys;
    RectF rect;
};

struct leftRectSegment
{
    YSegment ys;
    RectF rect;
};

class EGSHARED_EXPORT CuttingPointSearcher
{
public:
    CuttingPointSearcher(double dOffset);
    ~CuttingPointSearcher();
    void search(std::vector<RectF> &vecRect);
    RVector search(const RectF &s, const std::vector<RectF> &vecRect, std::vector<RectF> &vecDeletedRect);

protected:
    void sortByArea(std::vector<RectF> &vec);
    bool isAbove(const RectF &s, const RectF &r);
    bool isBelow(const RectF &s, const RectF &r);
    bool isLeft(const RectF &s, const RectF &r);
    bool isRight(const RectF &s, const RectF &r);
    void sortByPosition(std::vector<RectF> &vec);
    void sortByXY(std::vector<RectF> &vec);

    void aboveSegment(const RectF &s, const RectF &r, QVector<XSegment> &vecXSegment);
    void belowSegment(const RectF &s, const RectF &r, QVector<XSegment> &vecXSegment);
    void rightSegment(const RectF &s, const RectF &r, QVector<YSegment> &vecYSegment);
    void leftSegment(const RectF &s, const RectF &r, QVector<YSegment> &vecYSegment);

protected:
    double m_dOffset;//下刀点沿长边移动距离
    double m_dDistance;//两个矩形的距离，超过该距离视为非相邻矩形，不影响下刀点
    double m_dTolerance;//double型矩形误差容忍度
};

inline bool operator==(const RectF &r1, const RectF &r2)
{
    return r1.x==r2.x && r1.y==r2.y && r1.width==r2.width && r1.height==r2.height;
}

inline bool operator!=(const RectF &r1, const RectF &r2)
{
    return r1.x!=r2.x || r1.y!=r2.y || r1.width!=r2.width || r1.height!=r2.height;
}

#endif // CuttingPointSearcher_H

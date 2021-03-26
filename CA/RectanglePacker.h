#ifndef RECTANGLEPACKER_H
#define RECTANGLEPACKER_H

#include "ca_global.h"
#include "algotypedef.h"
#include "AlgoMaxRects.h"
#include <QList>
#include <QVector>

class CASHARED_EXPORT RectanglePacker
{
public:

    //method
    enum {GUILLOTINE, MAXRECTS};

    //heuristic
    enum {NONE, TL, BAF, BSSF, BLSF, MINW, MINH, HEURISTIC_NUM};

    //sort
    enum {SORT_NONE, WIDTH, HEIGHT, SORT_AREA, SORT_MAX, SORT_NUM};

    //rotation
    enum {NEVER, ONLY_WHEN_NEEDED, H2_WIDTH_H, WIDTH_GREATHER_HEIGHT, WIDTH_GREATHER_2HEIGHT, W2_HEIGHT_W, HEIGHT_GREATHER_WIDTH, HEIGHT_GREATHER_2WIDTH, ROTATION_NUM};

public:
    RectanglePacker();

    void init(const QList<InputRect>& lsRects);
    void pack(int nHeur, int nW, int nH);

private:
    unsigned addRectsToBins(int nHeur, int nW, int nH);

    void cropLastRect(int nHeur, int nW, int nH, bool wh);
    void divideLastRect(int nHeur, int nW, int nH, bool wh);

    float getFillRate();

    void clearBin(int nBinIndex);

    int fillBin(int nHeur, int nW, int nH, int nBinIndex);

    void sort();

    void sortRects(int nW, int nH);

public:

    QList<InputRect> m_rects;
    QList<QSize> m_bins;

private:
    quint64 m_nArea;
    int m_nMissingRects;
    Border m_border;
    int m_nRotate;
    int m_nSortOrder;
};

#endif // RECTANGLEPACKER_H

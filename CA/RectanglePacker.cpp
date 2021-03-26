#include "RectanglePacker.h"
#include <QDebug>
#include "AlgoMaxRects.h"
#include <limits>

bool rectCompareByHeight(const InputRect &i1, const InputRect &i2)
{
    return (i1.sizeCurrent.height() << 10) + i1.sizeCurrent.width() >
            (i2.sizeCurrent.height() << 10) + i2.sizeCurrent.width();
}
bool rectCompareByWidth(const InputRect &i1, const InputRect &i2)
{
    return (i1.sizeCurrent.width() << 10) + i1.sizeCurrent.height() >
            (i2.sizeCurrent.width() << 10) + i2.sizeCurrent.height();
}
bool rectCompareByArea(const InputRect &i1, const InputRect &i2)
{
    return i1.sizeCurrent.height() * i1.sizeCurrent.width() >
            i2.sizeCurrent.height() * i2.sizeCurrent.width();
}

bool rectCompareByMax(const InputRect &i1, const InputRect &i2)
{
    int first = i1.sizeCurrent.height() > i1.sizeCurrent.width() ?
                i1.sizeCurrent.height() : i1.sizeCurrent.width();
    int second = i2.sizeCurrent.height() > i2.sizeCurrent.width() ?
                i2.sizeCurrent.height() : i2.sizeCurrent.width();
    if(first == second)
    {
        return rectCompareByArea(i1, i2);
    }
    else
    {
        return first > second;
    }
}

RectanglePacker::RectanglePacker()
{

}

void RectanglePacker::init(const QList<InputRect>& lsRects)
{
    m_rects.clear();
    m_rects = lsRects;
    m_border.b = 0;
    m_border.l = 0;
    m_border.r = 0;
    m_border.t = 0;
}

void RectanglePacker::pack(int nHeur, int nW, int nH)
{
    sortRects(nW, nH);

    m_nMissingRects = 1;
    m_nArea = 0;
    m_bins.clear();

    unsigned int areaBuf = addRectsToBins(nHeur, nW, nH);

    if(areaBuf && !m_nMissingRects)
    {
        cropLastRect(nHeur, nW, nH, false);
    }
}

void RectanglePacker::sortRects(int nW, int nH)
{
    QSize size;
    for(int i = 0; i < m_rects.size(); i++)
    {
        m_rects.operator [](i).pos = QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());

        size = m_rects.at(i).size;

        if(size.width() == nW)
        {
            size.setWidth(size.width() - m_border.l - m_border.r);
        }

        if(size.height() == nH)
        {
            size.setHeight(size.height() - m_border.t - m_border.b);
        }

        size += QSize(m_border.l + m_border.r, m_border.t + m_border.b);

        m_rects.operator [](i).rotated = false;
        if((m_nRotate == WIDTH_GREATHER_HEIGHT && size.width() > size.height()) ||
                (m_nRotate == WIDTH_GREATHER_2HEIGHT && size.width() > 2 * size.height()) ||
                (m_nRotate == HEIGHT_GREATHER_WIDTH && size.height() > size.width()) ||
                (m_nRotate == H2_WIDTH_H && size.height() > size.width() &&
                 size.width() * 2 > size.height()) ||
                (m_nRotate == W2_HEIGHT_W && size.width() > size.height() &&
                 size.height() * 2 > size.width()) ||
                (m_nRotate == HEIGHT_GREATHER_2WIDTH && size.height() > 2 * size.width()))
        {
            size.transpose();
            m_rects.operator [](i).rotated = true;
        }

        m_rects.operator [](i).sizeCurrent = size;
    }
    sort();
}

void RectanglePacker::sort()
{
    switch(m_nSortOrder)
    {
    case 1:
        qSort(m_rects.begin(), m_rects.end(), rectCompareByWidth);
        break;
    case 2:
        qSort(m_rects.begin(), m_rects.end(), rectCompareByHeight);
        break;
    case 3:
        qSort(m_rects.begin(), m_rects.end(), rectCompareByArea);
        break;
    case 4:
        qSort(m_rects.begin(), m_rects.end(), rectCompareByMax);
        break;
    }
}


int RectanglePacker::fillBin(int nHeur, int nW, int nH, int nBinIndex)
{
    int areaBuf = 0;
    AlgoMaxRects algo;
    RectNode node;
    node.rect = QRect(0, 0, nW, nH);
    algo.m_lsRectNode << node;
    algo.m_nHeuristic = nHeur;
    algo.m_nW = nW;
    algo.m_nH = nH;
    algo.m_nRotation = m_nRotate;
    algo.m_pBorder = &m_border;

    for(int i = 0; i < m_rects.size(); i++)
    {
        if(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()) != m_rects.at(i).pos)
        {
            continue;
        }
        m_rects.operator [](i).pos = algo.insertNode(&m_rects.operator [](i));
        m_rects.operator [](i).binIndex = nBinIndex;
        if(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()) == m_rects.at(i).pos)
        {
            m_nMissingRects++;
        }
        else
        {
            areaBuf += m_rects.at(i).sizeCurrent.width() * m_rects.at(i).sizeCurrent.height();
            m_nArea += m_rects.at(i).sizeCurrent.width() * m_rects.at(i).sizeCurrent.height();
        }
    }
    return areaBuf;
}

void RectanglePacker::clearBin(int nBinIndex)
{
    for(int i = 0; i < m_rects.size(); i++)
    {
        if(m_rects.at(i).binIndex == nBinIndex)
        {
            m_nArea -= m_rects.at(i).sizeCurrent.width() * m_rects.at(i).sizeCurrent.height();
            m_rects.operator [](i).pos = QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
        }
    }
}

unsigned RectanglePacker::addRectsToBins(int nHeur, int nW, int nH)
{
    int binIndex = m_bins.count() - 1;
    unsigned areaBuf = 0;
    unsigned lastAreaBuf = 0;
    do
    {
        m_nMissingRects = 0;
        m_bins << QSize(nW, nH);
        lastAreaBuf = fillBin(nHeur, nW , nH , ++binIndex);
        if(!lastAreaBuf)
        {
            m_bins.removeLast();
        }
        areaBuf += lastAreaBuf;
    }
    while(m_nMissingRects && lastAreaBuf);
    return areaBuf;
}

void RectanglePacker::cropLastRect(int nHeur, int nW, int nH, bool wh)
{
    m_nMissingRects = 0;
    QList<InputRect> lastRects = m_rects;
    QList<QSize> lastBins = m_bins;
    quint64 lastArea = m_nArea;

    m_bins.removeLast();
    clearBin(m_bins.count());

    if(wh)
    {
        nW /= 2;
    }
    else
    {
        nH /= 2;
    }
    wh = !wh;

    int binIndex = m_bins.count();
    m_nMissingRects = 0;
    m_bins << QSize(nW, nH);
    fillBin(nHeur, nW , nH, binIndex);
    if(m_nMissingRects)
    {
        m_rects = lastRects;
        m_bins = lastBins;
        m_nArea = lastArea;
        m_nMissingRects = 0;

        if(!wh)
        {
            nW *= 2;
        }
        else
        {
            nH *= 2;
        }
        wh = !wh;
    }
    else
    {
        cropLastRect(nHeur, nW , nH, wh);
    }
}

void RectanglePacker::divideLastRect(int nHeur, int nW, int nH, bool wh)
{
    m_nMissingRects = 0;
    QList<InputRect> lastRects = m_rects;
    QList<QSize> lastBins = m_bins;
    quint64 lastArea = m_nArea;

    m_bins.removeLast();
    clearBin(m_bins.count());

    if(wh)
    {
        nW /= 2;
    }
    else
    {
        nH /= 2;
    }
    wh = !wh;

    addRectsToBins(nHeur, nW, nH);
    if(m_nMissingRects)
    {
        m_rects = lastRects;
        m_bins = lastBins;
        m_nArea = lastArea;
        m_nMissingRects = 0;
    }
    else
    {
        cropLastRect(nHeur, nW, nH, wh);
    }
}

float RectanglePacker::getFillRate()
{
    quint64 binArea = 0;
    for(int i = 0; i < m_bins.count(); i++)
    {
        binArea += m_bins.at(i).width() * m_bins.at(i).height();
    }
    return (float)((double)m_nArea / (double)binArea);
}

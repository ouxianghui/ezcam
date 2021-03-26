#ifndef ALGOMAXRECTS_H
#define ALGOMAXRECTS_H

#include "ca_global.h"
#include "Algotypedef.h"
#include <QList>

class CASHARED_EXPORT AlgoMaxRects
{
public:
    AlgoMaxRects();
    QPoint insertNode(InputRect* pNode);

public:
    QList<RectNode> m_lsRectNode;
    QList<QRect> m_lsRect;

    int m_nHeuristic;
    int m_nW;
    int m_nH;
    int m_nRotation;
    bool m_bLeftToRight;
    Border* m_pBorder;
};

#endif // ALGOMAXRECTS_H

#include "AlgoMaxRects.h"
#include "RectanglePacker.h"
#include <limits>

AlgoMaxRects::AlgoMaxRects()
{

}

QPoint AlgoMaxRects::insertNode(InputRect* pNode)
{
    int i;
    int min = std::numeric_limits<int>::max();
    int mini = -1;
    int m;

    QSize size = pNode->sizeCurrent;

    //if(size.width() == w) size.setWidth(size.width() - border->l - border->r);
    //if(size.height() == h) size.setHeight(size.height() - border->t - border->b);

    if(size.width() == 0 || size.height() == 0)
    {
        return QPoint(0, 0);
    }

    bool leftNeighbor = false;
    bool rightNeighbor = false;
    bool _leftNeighbor = false;
    bool _rightNeighbor = false;
    bool rotated = false;
    bool bestIsRotated = false;

    for(i = 0; i < m_lsRectNode.size(); i++)
    {
        if((m_lsRectNode.at(i).rect.width() >= size.width() && m_lsRectNode.at(i).rect.height() >= size.height()) ||
                (m_lsRectNode.at(i).rect.width() >= size.height() && m_lsRectNode.at(i).rect.height() >= size.width()))
        {
            rotated = false;
            m = 0;
            if((m_lsRectNode.at(i).rect.width() >= size.height() && m_lsRectNode.at(i).rect.height() >= size.width()) &&
                    !(m_lsRectNode.at(i).rect.width() >= size.width() && m_lsRectNode.at(i).rect.height() >= size.height()))
            {
                if(m_nRotation == 0)
                {
                    continue;
                }
                //pNode->rotated = !pNode->rotated;
                //pNode->sizeCurrent.transpose();
                size.transpose();
                rotated = true;
                m += size.height();
            }
            switch(m_nHeuristic)
            {
            case RectanglePacker::NONE:
                mini = i;
                i = m_lsRectNode.size();
                continue;
            case RectanglePacker::TL:
                m += m_lsRectNode.at(i).rect.y();
                _leftNeighbor = _rightNeighbor = false;
                for(int k = 0; k < m_lsRect.size(); k++)
                {
                    if(qAbs(m_lsRect.at(k).y() + m_lsRect.at(k).height() / 2 - m_lsRectNode.at(i).rect.y() - m_lsRectNode.at(i).rect.height() / 2) <
                            qMax(m_lsRect.at(k).height(), m_lsRectNode.at(i).rect.height()) / 2)
                    {
                        if(m_lsRect.at(k).x() + m_lsRect.at(k).width() == m_lsRectNode.at(i).rect.x())
                        {
                            m -= 5;
                            _leftNeighbor = true;
                        }
                        if(m_lsRect.at(k).x() == m_lsRectNode.at(i).rect.x() + m_lsRectNode.at(i).rect.width())
                        {
                            m -= 5;
                            _rightNeighbor = true;
                        }
                    }
                }
                if(_leftNeighbor || _rightNeighbor == false)
                {
                    if(m_lsRectNode.at(i).rect.x() + m_lsRectNode.at(i).rect.width() == m_nW)
                    {
                        m -= 1;
                        _rightNeighbor = true;
                    }
                    if(m_lsRectNode.at(i).rect.x() == 0)
                    {
                        m -= 1;
                        _leftNeighbor = true;
                    }
                }
                break;
            case RectanglePacker::BAF:
                m += m_lsRectNode.at(i).rect.width() * m_lsRectNode.at(i).rect.height();
                break;
            case RectanglePacker::BSSF:
                m += qMin(m_lsRectNode.at(i).rect.width() - size.width(), m_lsRectNode.at(i).rect.height() - size.height());
                break;
            case RectanglePacker::BLSF:
                m += qMax(m_lsRectNode.at(i).rect.width() - size.width(), m_lsRectNode.at(i).rect.height() - size.height());
                break;
            case RectanglePacker::MINW:
                m += m_lsRectNode.at(i).rect.width();
                break;
            case RectanglePacker::MINH:
                m += m_lsRectNode.at(i).rect.height();
            }
            if(m < min)
            {
                min = m;
                mini = i;
                leftNeighbor = _leftNeighbor;
                rightNeighbor = _rightNeighbor;
                bestIsRotated = rotated;
            }
            if(rotated)
            {
                size.transpose();
            }
        }
    }
    if(bestIsRotated)
    {
        size.transpose();
        pNode->rotated = !pNode->rotated;
        pNode->sizeCurrent.transpose();
    }
    if(mini >= 0)
    {
        i = mini;
        RectNode node0;
        QRect buf(m_lsRectNode.at(i).rect.x(), m_lsRectNode.at(i).rect.y(), size.width(), size.height());
        if(m_nHeuristic == RectanglePacker::TL)
        {
            if(!leftNeighbor && m_lsRectNode.at(i).rect.x() != 0 &&
                    m_lsRectNode.at(i).rect.width() + m_lsRectNode.at(i).rect.x() == m_nW)
            {
                buf = QRect(m_nW - size.width(), m_lsRectNode.at(i).rect.y(), size.width(), size.height());
            }
            if(!leftNeighbor && rightNeighbor)
            {
                buf = QRect(m_lsRectNode.at(i).rect.x() + m_lsRectNode.at(i).rect.width() - size.width(), m_lsRectNode.at(i).rect.y(),
                            size.width(), size.height());
            }
        }
        node0.rect = buf;
        m_lsRect << buf;
        if(m_lsRectNode.at(i).rect.width() > size.width())
        {
            RectNode node;
            node.rect = QRect(m_lsRectNode.at(i).rect.x() + (buf.x() == m_lsRectNode.at(i).rect.x() ? size.width() : 0),
                        m_lsRectNode.at(i).rect.y(), m_lsRectNode.at(i).rect.width() - size.width(), m_lsRectNode.at(i).rect.height());

            m_lsRectNode << node;
        }
        if(m_lsRectNode.at(i).rect.height() > size.height())
        {
            RectNode node;
            node.rect = QRect(m_lsRectNode.at(i).rect.x(), m_lsRectNode.at(i).rect.y() + size.height(), m_lsRectNode.at(i).rect.width(),
                        m_lsRectNode.at(i).rect.height() - size.height());

            m_lsRectNode << node;
        }

        m_lsRectNode.removeAt(i);

        //intersect
        for(i = 0; i < m_lsRectNode.size(); i++)
        {
            if(m_lsRectNode.at(i).rect.intersects(node0.rect))
            {
                if(node0.rect.x() + node0.rect.width() < m_lsRectNode.at(i).rect.x() + m_lsRectNode.at(i).rect.width())
                {
                    RectNode node;
                    node.rect = QRect(node0.rect.width() + node0.rect.x(), m_lsRectNode.at(i).rect.y(),
                                m_lsRectNode.at(i).rect.width() + m_lsRectNode.at(i).rect.x() - node0.rect.width() - node0.rect.x(),
                                m_lsRectNode.at(i).rect.height());

                    m_lsRectNode << node;
                }
                if(node0.rect.y() + node0.rect.height() < m_lsRectNode.at(i).rect.y() + m_lsRectNode.at(i).rect.height())
                {
                    RectNode node;
                    node.rect = QRect(m_lsRectNode.at(i).rect.x(), node0.rect.height() + node0.rect.y(),
                                m_lsRectNode.at(i).rect.width(), m_lsRectNode.at(i).rect.height() + m_lsRectNode.at(i).rect.y() - node0.rect.height() -
                                node0.rect.y());

                    m_lsRectNode << node;
                }
                if(node0.rect.x() > m_lsRectNode.at(i).rect.x())
                {
                    RectNode node;
                    node.rect = QRect(m_lsRectNode.at(i).rect.x(), m_lsRectNode.at(i).rect.y(), node0.rect.x() - m_lsRectNode.at(i).rect.x(),
                                m_lsRectNode.at(i).rect.height());

                    m_lsRectNode << node;
                }
                if(node0.rect.y() > m_lsRectNode.at(i).rect.y())
                {
                    RectNode node;
                    node.rect = QRect(m_lsRectNode.at(i).rect.x(), m_lsRectNode.at(i).rect.y(), m_lsRectNode.at(i).rect.width(),
                                node0.rect.y() - m_lsRectNode.at(i).rect.y());

                    m_lsRectNode << node;
                }
                m_lsRectNode.removeAt(i);
                i--;
            }
        }

        for(i = 0; i < m_lsRectNode.size(); i++)
        {
            for(int j = i + 1; j < m_lsRectNode.size(); j++)
            {
                if(i != j  && m_lsRectNode.at(i).rect.contains(m_lsRectNode.at(j).rect))
                {
                    m_lsRectNode.removeAt(j);
                    j--;
                }
            }
        }
        return QPoint(node0.rect.x(), node0.rect.y());
    }
    return QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
}

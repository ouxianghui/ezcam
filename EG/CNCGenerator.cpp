#include "CNCGenerator.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QException>
#include "RDocument.h"
#include "RDocumentInterface.h"
#include "BoardRectTable.h"
#include "RPolyline.h"
#include "RPolylineData.h"
#include "RPolylineEntity.h"
#include "RArc.h"
#include "RArcData.h"
#include "RArcEntity.h"
#include "RLine.h"
#include "RLineData.h"
#include "RLineEntity.h"
#include "RCircle.h"
#include "RCircleData.h"
#include "RCircleEntity.h"
#include "RShape.h"
#include "RMixedOperation.h"
#include "RAddObjectOperation.h"
#include "RPoint.h"
#include "RPointData.h"
#include "RPointEntity.h"
#include "PolylineEquidistant.h"
#include "ProductBoardTable.h"
#include "Typedef.h"

static bool cmpBoxArea(QSharedPointer<REntity> e1, QSharedPointer<REntity> e2)
{
    RBox box1 = e1->getBoundingBox();
    RBox box2 = e2->getBoundingBox();
    double dArea1 = box1.getWidth()*box1.getHeight();
    double dArea2 = box2.getWidth()*box2.getHeight();

    return dArea1 < dArea2;
}

CNCGenerator::CNCGenerator(int nCuttingSpeed,
                           int nProcessSpeed,
                           int nTerminalSpeed,
                           double dSafetyHeight,
                           double dCuttingDepth,
                           double dKnifeDiameter,
                           const QString& slotKnifeType,
                           int nUseDrills,
                           const QMap<QString, QVariant>& drillsMap,
                           const QMap<QString, QVariant>& holesMap,
                           double dSourceBoardW,
                           double dSourceBoardH,
                           const QString& knifeChangeMode,
                           const QString& strNCFileHeader,
                           const QString& strNCFileTail)
    : m_nCuttingSpeed(nCuttingSpeed),
      m_nProcessSpeed(nProcessSpeed),
      m_nTerminalSpeed(nTerminalSpeed),
      m_dSafetyHeight(dSafetyHeight),
      m_dCuttingDepth(dCuttingDepth),
      m_dKnifeDiameter(dKnifeDiameter),
      _slotKnifeType(slotKnifeType),
      m_nUseDrills(nUseDrills),
      _drillsMap(drillsMap),
      _holesMap(holesMap),
      m_dSourceBoardW(dSourceBoardW),
      m_dSourceBoardH(dSourceBoardH),
      _knifeChangeMode(knifeChangeMode),
      m_strNCFileHeader(strNCFileHeader),
      m_strNCFileTail(strNCFileTail)
{
}

CNCGenerator::~CNCGenerator()
{

}

int CNCGenerator::processGeneralInnerShape(QTextStream& ncStream, QSharedPointer<RPolyline> orgPolyline)
{
    double dOffset = m_dKnifeDiameter/2.0;

    // 最后一个参数:false,表示向左偏置
    QSharedPointer<RPolyline> equidistantPolyline = PolylineEquidistant::getEquidistantPolyline(orgPolyline, dOffset, false);

    // 如果是逆时针，就先转换为顺时针
    if (equidistantPolyline->getOrientation(true) == RS::CCW)
    {
        equidistantPolyline->reverse();
    }

    int nSize = equidistantPolyline->getVertices().size();
    for (int i = 0; i <= nSize; ++i)
    {
        RVector p1 = equidistantPolyline->getVertexAt(i%nSize);
        RVector p2 = equidistantPolyline->getVertexAt((i+1)%nSize);

        if (RPolyline::isStraight(equidistantPolyline->getBulgeAt(i%nSize)))
        {
            if (i == 0)
            {
                ncStream << "G0 X" << p1.x << " Y" << p1.y << " Z" << m_dSafetyHeight << endl;
                ncStream << "G1 X" << p1.x << " Y" << p1.y << " Z" << 0.000 << " F" << m_nCuttingSpeed << endl;
                ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << -(m_dCuttingDepth) << " F" << m_nProcessSpeed << endl;
            }
            else if (i == nSize)
            {
                ncStream << "G1 X" << p1.x << " Y" << p1.y << " Z" << -(m_dCuttingDepth) << endl;
                ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << -(m_dCuttingDepth) << " F" << m_nTerminalSpeed << endl;
                ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << m_dSafetyHeight << endl;
            }
            else
            {
                ncStream << "G1 X" << p1.x << " Y" << p1.y << " Z" << -(m_dCuttingDepth) << endl;
                ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << -(m_dCuttingDepth) << endl;
            }
        }
        else
        {

            double bulge = equidistantPolyline->getBulgeAt(i);

            RArc arc;

            if (!generateArc(p1, p2, bulge, arc))
            {
                continue;
            }

            if (i == 0)
            {
                ncStream << "G0 X" << p1.x << " Y" << p1.y << " Z" << m_dSafetyHeight << endl;
                ncStream << "G0 X" << p1.x << " Y" << p1.y << " Z" << -(m_dCuttingDepth) << " F" << m_nCuttingSpeed << endl;
                if (arc.isReversed())
                {
                    ncStream << "G2 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(m_dCuttingDepth) << endl;
                }
                else
                {
                    ncStream << "G3 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(m_dCuttingDepth) << endl;
                }

            }
            else if (i == nSize)
            {
                //if (arc.isReversed())
                //{
                //    ncStream << "G2 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(m_dCuttingDepth) << endl;
                //}
                //else
                //{
                //    ncStream << "G3 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(m_dCuttingDepth) << endl;
                //}
                //ncStream << "G0 X" << p2.x << " Y" << p2.y << " Z" << m_dSafetyHeight << endl;
            }
            else
            {
                if (arc.isReversed())
                {
                    ncStream << "G2 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(m_dCuttingDepth) << endl;
                }
                else
                {
                    ncStream << "G3 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(m_dCuttingDepth) << endl;
                }
            }

        }
    }

    return 0;
}

int CNCGenerator::processID007InnerShape(QTextStream& ncStream, QSharedPointer<RPolyline> orgPolyline, std::map<QString, QVariant>& paramsMap)
{
    //QTextCodec *tc = QTextCodec::codecForName("GBK");

    double dSlotDepth = 0.0;
    double dSlotWidth = 0.0;

    bool ok = false;

    QVariant var = paramsMap["Depth"];
    dSlotDepth = var.toDouble(&ok);
    if (!ok)
    {
        dSlotDepth = 0.0f;
    }

    if (paramsMap.find("W") != paramsMap.end())
    {
        var = paramsMap["W"];
        ok = false;
        dSlotWidth = var.toDouble(&ok);
        if (!ok)
        {
            dSlotWidth = 0.0;
        }
    }

    double dCuttingDepth = m_dCuttingDepth;

    // 如果dVarDepth不等于0，就用dVarDepth
    if (!(dSlotDepth <= EPSINON && dSlotDepth >= -EPSINON))
    {
        dCuttingDepth = dSlotDepth;
    }

    double dSlotKnifeDiameter = m_dKnifeDiameter;

    int nCycle = ceil((dSlotWidth/2.0-dSlotKnifeDiameter/2.0)/(dSlotKnifeDiameter-1.0));

    // 如果槽宽等于刀具直径，至少走刀一圈
    if (nCycle == 0 && dSlotWidth == dSlotKnifeDiameter)
    {
        nCycle = 1;
    }

    for (int n = 0; n < nCycle; ++n)
    {
        double dOFS = dSlotKnifeDiameter/2.0+(dSlotKnifeDiameter-1)*n;
        if (n == 0)
        {
            //QSharedPointer<RPolyline> equidistantPolyline = PolylineEquidistant::getEquidistantPolyline(orgPolyline, 0.0, false);
            //equidistantPolyline->reverse();
            //equidistantPolyline->setClosed(true);
            int nSize = orgPolyline->getVertices().size();
            for (int i = 0; i < nSize; ++i)
            {
                RVector p1 = orgPolyline->getVertexAt(i%nSize);
                RVector p2 = orgPolyline->getVertexAt((i+1)%nSize);

                if (RPolyline::isStraight(orgPolyline->getBulgeAt(i%nSize)))
                {
                    if (i == 0)
                    {
                        ncStream << "G0 X" << p1.x << " Y" << p1.y << " Z" << m_dSafetyHeight << endl;
                        ncStream << "G1 X" << p1.x << " Y" << p1.y << " Z" << -(dCuttingDepth) << " F" << m_nCuttingSpeed << endl;
                        ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << -(dCuttingDepth) << " F" << m_nProcessSpeed << endl;
                    }
                    else if (n == (nCycle-1) && i == (nSize-1))
                    {
                        ncStream << "G1 X" << p1.x << " Y" << p1.y << " Z" << -(dCuttingDepth) << endl;
                        ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << -(dCuttingDepth) << " F" << m_nTerminalSpeed << endl;
                        ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << m_dSafetyHeight << endl;
                    }
                    else
                    {
                        ncStream << "G1 X" << p1.x << " Y" << p1.y << " Z" << -(dCuttingDepth) << endl;
                        ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << -(dCuttingDepth) << endl;
                    }
                }
                else
                {
                    double bulge = orgPolyline->getBulgeAt(i);

                    RArc arc;

                    if (!generateArc(p1, p2, bulge, arc))
                    {
                        continue;
                    }

                    if (arc.isReversed())
                    {
                        ncStream << "G2 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(dCuttingDepth) << endl;
                    }
                    else
                    {
                        ncStream << "G3 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(dCuttingDepth) << endl;
                    }
                }
            }
        }
        else if (n == (nCycle-1))
        {
            QSharedPointer<RPolyline> equidistantPolyline = PolylineEquidistant::getEquidistantPolyline(orgPolyline, dOFS, false);
            equidistantPolyline->reverse();
            equidistantPolyline->setClosed(true);
            int nSize = equidistantPolyline->getVertices().size();
            for (int i = 0; i < nSize; ++i)
            {
                RVector p1 = equidistantPolyline->getVertexAt(i%nSize);
                RVector p2 = equidistantPolyline->getVertexAt((i+1)%nSize);

                if (RPolyline::isStraight(equidistantPolyline->getBulgeAt(i%nSize)))
                {
                    if (i == nSize-1)
                    {
                        ncStream << "G1 X" << p1.x << " Y" << p1.y << " Z" << -(dCuttingDepth) << endl;
                        ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << -(dCuttingDepth) << " F" << m_nTerminalSpeed << endl;
                        ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << m_dSafetyHeight << endl;
                    }
                    else
                    {
                        ncStream << "G1 X" << p1.x << " Y" << p1.y << " Z" << -(dCuttingDepth) << endl;
                        ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << -(dCuttingDepth) << endl;
                    }
                }
                else
                {
                    double bulge = equidistantPolyline->getBulgeAt(i);

                    RArc arc;

                    if (!generateArc(p1, p2, bulge, arc))
                    {
                        continue;
                    }

                    if (arc.isReversed())
                    {
                        ncStream << "G2 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(dCuttingDepth) << endl;
                    }
                    else
                    {
                        ncStream << "G3 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(dCuttingDepth) << endl;
                    }
                }
            }
        }
        else
        {
            QSharedPointer<RPolyline> equidistantPolyline = PolylineEquidistant::getEquidistantPolyline(orgPolyline, dOFS, false);
            equidistantPolyline->reverse();
            equidistantPolyline->setClosed(true);
            int nSize = equidistantPolyline->getVertices().size();
            for (int i = 0; i < nSize; ++i)
            {
                RVector p1 = equidistantPolyline->getVertexAt(i%nSize);
                RVector p2 = equidistantPolyline->getVertexAt((i+1)%nSize);

                if (RPolyline::isStraight(equidistantPolyline->getBulgeAt(i%nSize)))
                {
                    ncStream << "G1 X" << p1.x << " Y" << p1.y << " Z" << -(dCuttingDepth) << endl;
                    ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << -(dCuttingDepth) << endl;
                }
                else
                {
                    double bulge = equidistantPolyline->getBulgeAt(i);

                    RArc arc;

                    if (!generateArc(p1, p2, bulge, arc))
                    {
                        continue;
                    }

                    if (arc.isReversed())
                    {
                        ncStream << "G2 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(dCuttingDepth) << endl;
                    }
                    else
                    {
                        ncStream << "G3 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(dCuttingDepth) << endl;
                    }
                }
            }
        }
    }

    return 0;
}

int CNCGenerator::processID008InnerShape(QTextStream& ncStream, QSharedPointer<RPolyline> orgPolyline, std::map<QString, QVariant>& paramsMap)
{
    return processID007InnerShape(ncStream, orgPolyline, paramsMap);
}

int CNCGenerator::processID013InnerShape(QTextStream& ncStream, QSharedPointer<RPolyline> orgPolyline, std::map<QString, QVariant>& paramsMap)
{
    //QTextCodec *tc = QTextCodec::codecForName("GBK");

    double dVarDepth = 0.0;
    double dVarWidth = 0.0;
    double dVarHeight = 0.0;
    bool ok = false;

    QVariant var = paramsMap["Depth"];
    dVarDepth = var.toDouble(&ok);
    if (!ok)
    {
        dVarDepth = 0.0f;
    }

    if (paramsMap.find("W") != paramsMap.end())
    {
        var = paramsMap["W"];
        ok = false;
        dVarWidth = var.toDouble(&ok);
        if (!ok)
        {
            dVarWidth = 0.0;
        }
    }
    if (paramsMap.find("H") != paramsMap.end())
    {
        var = paramsMap["H"];
        ok = false;
        dVarHeight = var.toDouble(&ok);
        if (!ok)
        {
            dVarHeight = 0.0;
        }
    }

    double dCuttingDepth = m_dCuttingDepth;

    // 如果dVarDepth不等于0，就用dVarDepth
    if (!(dVarDepth <= EPSINON && dVarDepth >= -EPSINON))
    {
        dCuttingDepth = dVarDepth;
    }

    double dSlotKnifeDiameter = m_dKnifeDiameter;

    double dMinSide = dVarWidth < dVarHeight ? dVarWidth : dVarHeight;

    int nCycle = ceil((dMinSide/2.0-dSlotKnifeDiameter/2.0)/(dSlotKnifeDiameter-1.0));
    //RBox orgBox = orgPolyline->getBoundingBox();

    for (int n = 0; n < nCycle; ++n)
    {
        double dOFS = dSlotKnifeDiameter/2.0+(dSlotKnifeDiameter-1.0)*n;
        if (n == 0)
        {
            QSharedPointer<RPolyline> equidistantPolyline = PolylineEquidistant::getEquidistantPolyline(orgPolyline, dOFS, false);
            equidistantPolyline->reverse();
            equidistantPolyline->setClosed(true);
            int nSize = equidistantPolyline->getVertices().size();
            for (int i = 0; i < nSize; ++i)
            {
                RVector p1 = equidistantPolyline->getVertexAt(i%nSize);
                RVector p2 = equidistantPolyline->getVertexAt((i+1)%nSize);

                if (RPolyline::isStraight(equidistantPolyline->getBulgeAt(i%nSize)))
                {
                    if (i == 0)
                    {
                        ncStream << "G0 X" << p1.x << " Y" << p1.y << " Z" << m_dSafetyHeight << endl;
                        ncStream << "G1 X" << p1.x << " Y" << p1.y << " Z" << -(dCuttingDepth) << " F" << m_nCuttingSpeed << endl;
                        ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << -(dCuttingDepth) << " F" << m_nProcessSpeed << endl;
                    }
                    else if (n == (nCycle-1) && i == (nSize-1))
                    {
                        ncStream << "G1 X" << p1.x << " Y" << p1.y << " Z" << -(dCuttingDepth) << endl;
                        ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << -(dCuttingDepth) << " F" << m_nTerminalSpeed << endl;
                        ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << m_dSafetyHeight << endl;
                    }
                    else
                    {
                        ncStream << "G1 X" << p1.x << " Y" << p1.y << " Z" << -(dCuttingDepth) << endl;
                        ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << -(dCuttingDepth) << endl;
                    }
                }
                else
                {
                    double bulge = equidistantPolyline->getBulgeAt(i);

                    RArc arc;

                    if (!generateArc(p1, p2, bulge, arc))
                    {
                        continue;
                    }

                    if (arc.isReversed())
                    {
                        ncStream << "G2 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(dCuttingDepth) << endl;
                    }
                    else
                    {
                        ncStream << "G3 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(dCuttingDepth) << endl;
                    }
                }
            }
        }
        else if (n == (nCycle-1))
        {
            QSharedPointer<RPolyline> equidistantPolyline = PolylineEquidistant::getEquidistantPolyline(orgPolyline, dOFS, false);
            equidistantPolyline->reverse();
            equidistantPolyline->setClosed(true);
            int nSize = equidistantPolyline->getVertices().size();
            for (int i = 0; i < nSize; ++i)
            {
                RVector p1 = equidistantPolyline->getVertexAt(i%nSize);
                RVector p2 = equidistantPolyline->getVertexAt((i+1)%nSize);

                if (RPolyline::isStraight(equidistantPolyline->getBulgeAt(i%nSize)))
                {
                    if (i == nSize-1)
                    {
                        ncStream << "G1 X" << p1.x << " Y" << p1.y << " Z" << -(dCuttingDepth) << endl;
                        ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << -(dCuttingDepth) << " F" << m_nTerminalSpeed << endl;
                        ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << m_dSafetyHeight << endl;
                    }
                    else
                    {
                        ncStream << "G1 X" << p1.x << " Y" << p1.y << " Z" << -(dCuttingDepth) << endl;
                        ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << -(dCuttingDepth) << endl;
                    }
                }
                else
                {
                    double bulge = equidistantPolyline->getBulgeAt(i);

                    RArc arc;

                    if (!generateArc(p1, p2, bulge, arc))
                    {
                        continue;
                    }

                    if (arc.isReversed())
                    {
                        ncStream << "G2 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(dCuttingDepth) << endl;
                    }
                    else
                    {
                        ncStream << "G3 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(dCuttingDepth) << endl;
                    }
                }
            }
        }
        else
        {
            QSharedPointer<RPolyline> equidistantPolyline = PolylineEquidistant::getEquidistantPolyline(orgPolyline, dOFS, false);
            equidistantPolyline->reverse();
            equidistantPolyline->setClosed(true);
            int nSize = equidistantPolyline->getVertices().size();
            for (int i = 0; i < nSize; ++i)
            {
                RVector p1 = equidistantPolyline->getVertexAt(i%nSize);
                RVector p2 = equidistantPolyline->getVertexAt((i+1)%nSize);

                if (RPolyline::isStraight(equidistantPolyline->getBulgeAt(i%nSize)))
                {
                    ncStream << "G1 X" << p1.x << " Y" << p1.y << " Z" << -(dCuttingDepth) << endl;
                    ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << -(dCuttingDepth) << endl;
                }
                else
                {
                    double bulge = equidistantPolyline->getBulgeAt(i);

                    RArc arc;

                    if (!generateArc(p1, p2, bulge, arc))
                    {
                        continue;
                    }

                    if (arc.isReversed())
                    {
                        ncStream << "G2 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(dCuttingDepth) << endl;
                    }
                    else
                    {
                        ncStream << "G3 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(dCuttingDepth) << endl;
                    }
                }
            }
        }
    }

    return 0;
}

int CNCGenerator::generateCNC(RDocument& document, const QString& strSourceBoardID, const QString strOutputPathName)
{
    try
    {
        //QTextCodec *tc = QTextCodec::codecForName("GBK");

        QFile ncFile(strOutputPathName);
        if (!ncFile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
        {
            return 9;
        }

        double dOffset = m_dKnifeDiameter/2.0;

        QTextStream ncStream(&ncFile);

        // 写入NC文件头部
        ncStream << m_strNCFileHeader << endl;

        ncStream.setRealNumberPrecision(3);
        ncStream.setRealNumberNotation(QTextStream::FixedNotation);

        std::vector<BoardRectSt> vecBoardRectSt;

        vecBoardRectSt.clear();

        if (BoardRectTable::getBoardRectsBySourceBoardID(strSourceBoardID, vecBoardRectSt))
        {
            // 按nIdx升序排序
            std::sort(vecBoardRectSt.begin(), vecBoardRectSt.end());

            for (std::vector<BoardRectSt>::iterator it = vecBoardRectSt.begin(); it != vecBoardRectSt.end(); ++it)
            {
                BoardRectSt& st = *it;

                RVector ptCutting(st.dNcStartPtY*m_dSourceBoardH, -st.dNcStartPtX*m_dSourceBoardW);
                ptCutting.rotate(90.0*M_PI/180.0, RVector(0.0, 0.0));

                RVector c1(st.dLtY*m_dSourceBoardH, -st.dLtX*m_dSourceBoardW);
                RVector c2(st.dRbY*m_dSourceBoardH, -st.dRbX*m_dSourceBoardW);
                RBox box(c1, c2);

                if (box.isValid())
                {
                    // 先记录在这个外包矩形内的所有折线和圆形
                    std::vector<QSharedPointer<REntity>> vecEntities;
                    QSet<REntity::Id> idSet = document.queryContainedEntitiesXY(box);
                    for (QSet<REntity::Id>::iterator iter = idSet.begin(); iter != idSet.end(); ++iter)
                    {
                        QSharedPointer<REntity> e = document.queryEntity(*iter);
                        if (!e.isNull())
                        {
                            if (e->getType() == RS::EntityPolyline
                                    || e->getType() == RS::EntityCircle
                                    || e->getType() == RS::EntityLine
                                    || e->getType() == RS::EntityArc)
                            {
                                // 绕坐标系原点逆时针旋转90度，用于生成NC代码
                                e->rotate(90.0*M_PI/180.0, RVector(0.0, 0.0));
                                vecEntities.push_back(e);
                            }
                        }
                    }

                    // 外包矩形内的所有实体按面积升序排序
                    std::sort(vecEntities.begin(), vecEntities.end(), cmpBoxArea);

                    // v3.1.1版本的变更,先切割拉槽,后割板件边缘,拉槽以外的异形则保持原来的切割方法不变 --oxh 20160509
                    // 判断是否为内部实体，然后再判断是否为7、8、13三种拉槽，最后对这三种拉槽特殊处理
                    for (std::vector<QSharedPointer<REntity>>::iterator entityIt = vecEntities.begin(); entityIt != vecEntities.end(); ++entityIt)
                    {
                        QSharedPointer<REntity>& e = *entityIt;

                        if (entityIt+1 != vecEntities.end())
                        {
                            if (e->getType() == RS::EntityPolyline)
                            {
                                QSharedPointer<RPolylineEntity> polylineEntity = e.dynamicCast<RPolylineEntity>();
                                if (!polylineEntity.isNull())
                                {
                                    int nID = -2;

                                    std::map<QString, QVariant> paramsMap;
                                    paramsMap.clear();

                                    DataSt dataSt;
                                    if (ProductBoardTable::getBoardByBarcodeNo(st.strBarcodeNo, dataSt))
                                    {
                                        QStringList sl = dataSt.strDissimilarShapes.split(';');
                                        for (int i = 0; i < sl.size(); ++i)
                                        {
                                            QString str = sl.at(i);
                                            if (!str.isEmpty())
                                            {
                                                QStringList slParam = str.split('=');
                                                if (slParam.size() >= 2)
                                                {
                                                    QString strKey = slParam.at(0);
                                                    QString strValue = slParam.at(1);
                                                    bool ok = false;
                                                    strValue.toDouble(&ok);
                                                    if (ok)
                                                    {
                                                        paramsMap[strKey] = strValue;
                                                    }
                                                }
                                            }
                                        }

                                        if (paramsMap.find("ID") != paramsMap.end())
                                        {
                                            QString strID = paramsMap["ID"].toString();
                                            bool ok = false;
                                            nID = strID.toInt(&ok);
                                            if (!ok)
                                            {
                                                nID = -2;
                                            }
                                        }
                                    }

                                    if (_currentKnife != "T1")
                                    {
                                        // 把换刀模式字符串中的X换成目标刀具T后面的数字
                                        QString src = "T1";
                                        QString str = _knifeChangeMode;
                                        QString dst = src.mid(1);
                                        str.replace("X", dst);
                                        ncStream << str << endl;
                                        _currentKnife = "T1";
                                    }

                                    RPolylineData& data = polylineEntity->getData();
                                    QSharedPointer<RPolyline> orgPolyline = QSharedPointer<RPolyline>(new RPolyline(data.getExploded()));
                                    orgPolyline->setClosed(true);

                                    if (nID == 7)
                                    {
                                        int nRet = processID007InnerShape(ncStream, orgPolyline, paramsMap);
                                        if (0 != nRet)
                                        {
                                            ncFile.close();
                                            return nRet;
                                        }
                                    }
                                    else if (nID == 8)
                                    {
                                        int nRet = processID008InnerShape(ncStream, orgPolyline, paramsMap);
                                        if (0 != nRet)
                                        {
                                            ncFile.close();
                                            return nRet;
                                        }
                                    }
                                    else if (nID == 13)
                                    {
                                        int nRet = processID013InnerShape(ncStream, orgPolyline, paramsMap);
                                        if (0 != nRet)
                                        {
                                            ncFile.close();
                                            return nRet;
                                        }
                                    }
                                    else
                                    {
                                        int nRet = processGeneralInnerShape(ncStream, orgPolyline);
                                        if (0 != nRet)
                                        {
                                            ncFile.close();
                                            return nRet;
                                        }
                                    }
                                }
                            }
                            else if (e->getType() == RS::EntityCircle)
                            {
                                QSharedPointer<RCircleEntity> circleEntity = e.dynamicCast<RCircleEntity>();
                                if (!circleEntity.isNull())
                                {
                                    RCircleData& data = circleEntity->getData();

                                    RVector ptCenter = data.getCenter();

                                    // 找一把合适的刀来打孔（刀具直径等于圆孔直径），实现一下刀一提刀打一个孔
                                    bool matchKnife = false;
                                    double holeDepth = 0.0;
                                    QString key = "";

                                    QList<QString> keys = _drillsMap.keys();

                                    for (int idx = 0; idx < keys.length(); ++idx)
                                    {
                                        key = keys[idx];

                                        bool ok = false;

                                        double value = _drillsMap[key].toDouble(&ok);
                                        if (ok)
                                        {
                                            double delta = fabs(value - data.getRadius()*2);
                                            if (EPSINON > delta && -EPSINON < delta )
                                            {
                                                matchKnife = true;
                                                holeDepth = _holesMap[_drillsMap[key].toString()].toDouble();
                                                break;
                                            }
                                        }
                                    }

                                    // 找到合适的刀具就直接打孔
                                    if (m_nUseDrills == 1
                                            && circleEntity->getColor() == RColor(255, 0, 0)
                                            && matchKnife)
                                    {
                                        if (_currentKnife != key)
                                        {
                                            QString str = _knifeChangeMode;
                                            QString dst = key.mid(1);
                                            str.replace("X", dst);
                                            ncStream << str << endl;
                                            _currentKnife = key;
                                        }

                                        ncStream << "G0 X" << ptCenter.x << " Y" << ptCenter.y << " Z" << m_dSafetyHeight << endl;
                                        ncStream << "G1 X" << ptCenter.x << " Y" << ptCenter.y << " Z" << 0.000 << " F" << m_nCuttingSpeed << endl;
                                        ncStream << "G1 X" << ptCenter.x << " Y" << ptCenter.y << " Z" << -(holeDepth) << " F" << m_nProcessSpeed << endl;
                                        ncStream << "G1 X" << ptCenter.x << " Y" << ptCenter.y << " Z" << m_dSafetyHeight << endl;
                                    }
                                    // 找不到匹配的刀具就用走刀的方式切割圆
                                    else
                                    {
                                        if (_currentKnife != "T1")
                                        {
                                            QString src = "T1";
                                            QString str = _knifeChangeMode;
                                            QString dst = src.mid(1);
                                            str.replace("X", dst);
                                            ncStream << str << endl;
                                            _currentKnife = "T1";
                                        }

                                        double dRadius = data.getRadius()-dOffset;

                                        // 选择圆心正上方的点作为切割的起点
                                        RVector ptStart(ptCenter.x, ptCenter.y+dRadius);

                                        ncStream << "G0 X" << ptStart.x << " Y" << ptStart.y << " Z" << m_dSafetyHeight << endl;
                                        ncStream << "G1 X" << ptStart.x << " Y" << ptStart.y << " Z" << 0.000 << " F" << m_nCuttingSpeed << endl;
                                        ncStream << "G1 X" << ptStart.x << " Y" << ptStart.y << " Z" << -(m_dCuttingDepth) << " F" << m_nProcessSpeed << endl;
                                        ncStream << "G2 X" << ptStart.x << " Y" << ptStart.y << " I" << (ptCenter.x-ptStart.x) << " J" << (ptCenter.y-ptStart.y) << " Z" << -(m_dCuttingDepth) << endl;
                                        ncStream << "G1 X" << ptStart.x << " Y" << ptStart.y << " Z" << m_dSafetyHeight << endl;
                                    }
                                }
                            }
                            else if (e->getType() == RS::EntityLine)
                            {
                                QSharedPointer<RLineEntity> lineEntity = e.dynamicCast<RLineEntity>();
                                if (!lineEntity.isNull() && lineEntity->getColor() == RColor(255, 0, 0))
                                {
                                    if (_slotKnifeType != _currentKnife)
                                    {
                                        QString str = _knifeChangeMode;
                                        QString dst = _slotKnifeType.mid(1);
                                        str.replace("X", dst);
                                        ncStream << str << endl;
                                        _currentKnife = _slotKnifeType;
                                    }

                                    RLineData& lineData = lineEntity->getData();
                                    RVector ptStart = lineData.getStartPoint();
                                    RVector ptEnd = lineData.getEndPoint();
                                    ncStream << "G0 X" << ptStart.x << " Y" << ptStart.y << " Z" << m_dSafetyHeight << endl;
                                    ncStream << "G1 X" << ptStart.x << " Y" << ptStart.y << " Z" << 0.000 << " F" << m_nCuttingSpeed << endl;
                                    ncStream << "G1 X" << ptEnd.x << " Y" << ptEnd.y << " Z" << -(m_dCuttingDepth) << " F" << m_nProcessSpeed << endl;
                                    ncStream << "G1 X" << ptEnd.x << " Y" << ptEnd.y << " Z" << m_dSafetyHeight << endl;
                                }
                            }
                            else if (e->getType() == RS::EntityArc)
                            {
                                QSharedPointer<RArcEntity> arcEntity = e.dynamicCast<RArcEntity>();
                                if (!arcEntity.isNull() && arcEntity->getColor() == RColor(255, 0, 0))
                                {
                                    if (_slotKnifeType != _currentKnife)
                                    {
                                        QString str = _knifeChangeMode;
                                        QString dst = _slotKnifeType.mid(1);
                                        str.replace("X", dst);
                                        ncStream << str << endl;
                                        _currentKnife = _slotKnifeType;
                                    }

                                    RArc arc = arcEntity->getData().getArc();
                                    RVector p1 = arc.getStartPoint();
                                    RVector p2 = arc.getEndPoint();

                                    ncStream << "G0 X" << p1.x << " Y" << p1.y << " Z" << m_dSafetyHeight << endl;
                                    ncStream << "G0 X" << p1.x << " Y" << p1.y << " Z" << -(m_dCuttingDepth) << " F" << m_nCuttingSpeed << endl;
                                    if (arc.isReversed())
                                    {
                                        ncStream << "G2 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(m_dCuttingDepth) << endl;
                                    }
                                    else
                                    {
                                        ncStream << "G3 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(m_dCuttingDepth) << endl;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if (_currentKnife != "T1")
            {
                QString src = "T1";
                QString str = _knifeChangeMode;
                QString dst = src.mid(1);
                str.replace("X", dst);
                ncStream << str << endl;
                _currentKnife = "T1";
            }

            for (std::vector<BoardRectSt>::iterator it = vecBoardRectSt.begin(); it != vecBoardRectSt.end(); ++it)
            {
                BoardRectSt& st = *it;

                RVector ptCutting(st.dNcStartPtY*m_dSourceBoardH, -st.dNcStartPtX*m_dSourceBoardW);
                ptCutting.rotate(90.0*M_PI/180.0, RVector(0.0, 0.0));

                RVector c1(st.dLtY*m_dSourceBoardH, -st.dLtX*m_dSourceBoardW);
                RVector c2(st.dRbY*m_dSourceBoardH, -st.dRbX*m_dSourceBoardW);
                RBox box(c1, c2);

                if (box.isValid())
                {
                    // 先记录在这个外包矩形内的所有折线和圆形
                    std::vector<QSharedPointer<REntity>> vecEntities;
                    QSet<REntity::Id> idSet = document.queryContainedEntitiesXY(box);
                    for (QSet<REntity::Id>::iterator iter = idSet.begin(); iter != idSet.end(); ++iter)
                    {
                        QSharedPointer<REntity> e = document.queryEntity(*iter);
                        if (!e.isNull())
                        {
                            if (e->getType() == RS::EntityPolyline || e->getType() == RS::EntityCircle)
                            {
                                // 绕坐标系原点逆时针旋转90度，用于生成NC代码
                                e->rotate(90.0*M_PI/180.0, RVector(0.0, 0.0));
                                vecEntities.push_back(e);
                            }
                        }
                    }

                    // 外包矩形内的所有实体按面积升序排序
                    std::sort(vecEntities.begin(), vecEntities.end(), cmpBoxArea);

                    for (std::vector<QSharedPointer<REntity>>::iterator entityIt = vecEntities.begin(); entityIt != vecEntities.end(); ++entityIt)
                    {
                        QSharedPointer<REntity>& e = *entityIt;

                        // 板件最外层边缘(按面积排序后vecEntities中的最后一个面积最大，是板件最外层)
                        if (entityIt+1 == vecEntities.end())
                        {
                            // 最外部形状逆时针切割
                            if (e->getType() == RS::EntityPolyline)
                            {
                                QSharedPointer<RPolylineEntity> polylineEntity = e.dynamicCast<RPolylineEntity>();
                                if (!polylineEntity.isNull())
                                {
                                    RPolylineData& data = polylineEntity->getData();
                                    int nRet = rebuildPolylineDataWithCuttingPoint(data, ptCutting);

                                    if (0 == nRet)
                                    {
                                        QSharedPointer<RPolyline> orgPolyline = QSharedPointer<RPolyline>(new RPolyline(data.getExploded()));
                                        orgPolyline->setClosed(true);
                                        QSharedPointer<RPolyline> equidistantPolyline = PolylineEquidistant::getEquidistantPolyline(orgPolyline, dOffset, true);
                                        equidistantPolyline->setClosed(true);

                                        // 如果是顺时针，就先转换为逆时针
                                        if (equidistantPolyline->getOrientation(true) == RS::CW)
                                        {
                                            equidistantPolyline->reverse();
                                        }

                                        int nSize = equidistantPolyline->getVertices().size();
                                        for (int i = 0; i <= nSize; ++i)
                                        {
                                            RVector p1 = equidistantPolyline->getVertexAt(i%nSize);
                                            RVector p2 = equidistantPolyline->getVertexAt((i+1)%nSize);

                                            if (RPolyline::isStraight(equidistantPolyline->getBulgeAt(i%nSize)))
                                            {
                                                if (i == 0)
                                                {
                                                    ncStream << "G0 X" << p1.x << " Y" << p1.y << " Z" << m_dSafetyHeight << endl;
                                                    ncStream << "G1 X" << p1.x << " Y" << p1.y << " Z" << 0.000 << " F" << m_nCuttingSpeed << endl;
                                                    ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << -(m_dCuttingDepth) << " F" << m_nProcessSpeed << endl;
                                                }
                                                else if (i == nSize)
                                                {
                                                    ncStream << "G1 X" << p1.x << " Y" << p1.y << " Z" << -(m_dCuttingDepth) << endl;
                                                    ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << -(m_dCuttingDepth) << " F" << m_nTerminalSpeed << endl;
                                                    ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << m_dSafetyHeight << endl;
                                                }
                                                else
                                                {
                                                    ncStream << "G1 X" << p1.x << " Y" << p1.y << " Z" << -(m_dCuttingDepth) << endl;
                                                    ncStream << "G1 X" << p2.x << " Y" << p2.y << " Z" << -(m_dCuttingDepth) << endl;
                                                }
                                            }
                                            else
                                            {
                                                double bulge = equidistantPolyline->getBulgeAt(i);

                                                RArc arc;

                                                if (!generateArc(p1, p2, bulge, arc))
                                                {
                                                    continue;
                                                }

                                                if (i == 0)
                                                {
                                                    ncStream << "G0 X" << p1.x << " Y" << p1.y << " Z" << m_dSafetyHeight << endl;
                                                    ncStream << "G0 X" << p1.x << " Y" << p1.y << " Z" << -(m_dCuttingDepth) << " F" << m_nCuttingSpeed << endl;
                                                    if (arc.isReversed())
                                                    {
                                                        ncStream << "G2 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(m_dCuttingDepth) << endl;
                                                    }
                                                    else
                                                    {
                                                        ncStream << "G3 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(m_dCuttingDepth) << endl;
                                                    }

                                                }
                                                else if (i != nSize)
                                                {
                                                    if (arc.isReversed())
                                                    {
                                                        ncStream << "G2 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(m_dCuttingDepth) << endl;
                                                    }
                                                    else
                                                    {
                                                        ncStream << "G3 X" << p2.x << " Y" << p2.y << " R" << arc.getRadius() << " Z" << -(m_dCuttingDepth) << endl;
                                                    }
                                                }

                                            }
                                        }
                                    }
                                    else if (1 == nRet)
                                    {
                                        // "在折线中插入下刀点失败：原折线不是闭合的.
                                        ncFile.close();
                                        return 1;
                                    }
                                    else if (2 == nRet)
                                    {
                                        // 在折线中插入下刀点失败：圆折线线段数量小于3.
                                        ncFile.close();
                                        return 2;
                                    }
                                    else if (3 == nRet)
                                    {
                                        // 在折线中插入下刀点失败：下刀点不在折线上."
                                        ncFile.close();
                                        return 3;
                                    }
                                    else if (4 == nRet)
                                    {
                                        // 在折线中插入下刀点失败：不允许下刀点在圆弧上.
                                        ncFile.close();
                                        return 4;
                                    }
                                    else
                                    {
                                        // 在折线中插入下刀点失败：未知错误.
                                        ncFile.close();
                                        return 5;
                                    }
                                }
                                else
                                {
                                    // 动态转换实体失败.
                                    ncFile.close();
                                    return 6;
                                }
                            }
                            else if (e->getType() == RS::EntityCircle)
                            {
                                QSharedPointer<RCircleEntity> circleEntity = e.dynamicCast<RCircleEntity>();
                                if (!circleEntity.isNull())
                                {
                                    RCircleData& data = circleEntity->getData();

                                    double dRadius = data.getRadius()+dOffset;

                                    RVector ptCenter = data.getCenter();

                                    // 选择圆心正上方的点作为切割的起点
                                    RVector ptStart(ptCenter.x, ptCenter.y+dRadius);

                                    ncStream << "G0 X" << ptStart.x << " Y" << ptStart.y << " Z" << m_dSafetyHeight << endl;
                                    ncStream << "G1 X" << ptStart.x << " Y" << ptStart.y << " Z" << 0.000 << " F" << m_nCuttingSpeed << endl;
                                    ncStream << "G1 X" << ptStart.x << " Y" << ptStart.y << " Z" << -(m_dCuttingDepth) << " F" << m_nProcessSpeed << endl;
                                    ncStream << "G3 X" << ptStart.x << " Y" << ptStart.y << " I" << (ptCenter.x-ptStart.x) << " J" << (ptCenter.y-ptStart.y) << " Z" << -(m_dCuttingDepth) << endl;
                                    ncStream << "G1 X" << ptStart.x << " Y" << ptStart.y << " Z" << m_dSafetyHeight << endl;
                                }
                            }
                        }
                    }
                }
            }
        }

        // 写入NC文件头尾
        ncStream << m_strNCFileTail << endl;

        ncFile.close();

        return 0;
    }
    catch (QException& e)
    {
        qCritical("CNCGenerator::generateCNC() exception: %s", e.what());
        return 5;
    }
}

// 把下刀点插入RPolyLine里面
int CNCGenerator::rebuildPolylineDataWithCuttingPoint(RPolylineData& data, const RVector& ptCutting)
{
    try
    {
        double tolerance = 1.0e-1;

        RPolyline PolylineNew;
        PolylineNew.clear();

        if (!data.isClosed())
        {
            return 1;
        }

        if (data.countSegments() < 2)
        {
            return 2;
        }

        int nSize = data.getVertices().size();

        bool isPtCuttingOnLine = false;

        // 先检查下刀点是否在直线上
        for (int i = 0; i < nSize; ++i)
        {
            RVector p1 = data.getVertexAt(i);
            RVector p2 = data.getVertexAt((i+1) % nSize);

            if (RPolyline::isStraight(data.getBulgeAt(i)))
            {
                RLine line(p1, p2);
                if (line.isOnShape(ptCutting, true, tolerance))
                {
                    isPtCuttingOnLine = true;
                }
            }
            else
            {
                double bulge = data.getBulgeAt(i);

                RArc arc;
                if (!generateArc(p1, p2, bulge, arc))
                {
                    continue;
                }

                if (arc.isOnShape(ptCutting, true, tolerance))
                {
                    return 4;
                }
            }
        }

        if (!isPtCuttingOnLine)
        {
            return 3;
        }

        for (int i = 0; i < nSize; ++i)
        {
            RVector p1 = data.getVertexAt(i);
            RVector p2 = data.getVertexAt((i+1) % nSize);

            if (RPolyline::isStraight(data.getBulgeAt(i)))
            {
                RLine line(p1, p2);
                if (line.isOnShape(ptCutting, true, tolerance))
                {
                    data.insertVertex((i+1), ptCutting);

                    int iSize = data.getVertices().size();

                    for (int n = 0; n < iSize; ++n)
                    {
                        int nIdx = n + (i+1);

                        RVector ip1 = data.getVertexAt(nIdx % iSize);
                        RVector ip2 = data.getVertexAt((nIdx+1) % iSize);

                        if (RPolyline::isStraight(data.getBulgeAt(nIdx % iSize)))
                        {
                            RLine iLine(ip1, ip2);
                            PolylineNew.appendShape(iLine);
                        }
                        else
                        {
                            double bulge = data.getBulgeAt(nIdx % iSize);

                            RArc arc;
                            if (!generateArc(ip1, ip2, bulge, arc))
                            {
                                continue;
                            }

                            PolylineNew.appendShape(arc);
                        }
                    }

                    PolylineNew.setClosed(true);
                    data.clear();
                    data = RPolylineData(PolylineNew);

                    break;
                }
            }
        }

        if (PolylineNew.countSegments() > 2 && PolylineNew.isClosed())
        {
            return 0;
        }
        else
        {
            return 5;
        }
    }
    catch (QException& e)
    {
        qCritical("CNCGenerator::rebuildPolylineDataWithCuttingPoint() exception: %s", e.what());
        return 5;
    }
}

bool CNCGenerator::generateArc(const RVector& pt1, const RVector& pt2, double bulge, RArc& arc)
{
    try
    {
        bool reversed = bulge < 0.0;
        double alpha = atan(bulge)*4.0;

        if (fabs(alpha) > 2*M_PI-RS::AngleTolerance)
        {
            return false;
        }

        double radius;
        RVector center;
        RVector middle;
        double dist;
        double angle;

        middle = (pt1+pt2)/2.0;
        dist = pt1.getDistanceTo(pt2)/2.0;
        angle = pt1.getAngleTo(pt2);

        // alpha can't be 0.0 at this point
        radius = fabs(dist / sin(alpha/2.0));

        double rootTerm = fabs(radius*radius - dist*dist);
        double h = sqrt(rootTerm);

        if (bulge > 0.0)
        {
            angle += M_PI/2.0;
        }
        else
        {
            angle -= M_PI/2.0;
        }

        if (fabs(alpha) > M_PI)
        {
            h *= -1.0;
        }

        center.setPolar(h, angle);
        center += middle;

        double a1;
        double a2;

        a1 = center.getAngleTo(pt1);
        a2 = center.getAngleTo(pt2);

        arc = RArc(center, radius, a1, a2, reversed);

        return true;
    }
    catch (QException& e)
    {
        qCritical("CNCGenerator::generateArc() exception: %s", e.what());
        return false;
    }
}

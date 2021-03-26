#include "DissimilarLibraryOperator.h"
#include <QApplication>
#include <QDir>
#include <QPen>
#include <QRectF>
#include "RArc.h"
#include "RArcData.h"
#include "RArcEntity.h"
#include "RPolyline.h"
#include "RPolylineData.h"
#include "RPolylineEntity.h"
#include "RCircle.h"
#include "RCircleData.h"
#include "RCircleEntity.h"
#include "SystemConfigManager.h"
const double EPSINON = 1E-6;

DissimilarLibraryOperator::DissimilarLibraryOperator()
{

}

DissimilarLibraryOperator::~DissimilarLibraryOperator()
{

}

bool DissimilarLibraryOperator::getAllDissimilarImageName(QStringList& sl)
{
    sl.clear();

    QString strDir = QApplication::applicationDirPath();
    if (strDir.isEmpty())
    {
        return false;
    }
    strDir += "/dissimilar";
    QDir dir(strDir);
    if (dir.exists() && dir.isReadable())
    {
        QStringList slFilters;
        slFilters << "*.png" << "*.jpg" << "*.jpeg";
        sl = dir.entryList(slFilters, QDir::Files|QDir::Readable, QDir::Name);
    }
    else
    {
        return false;
    }

    return true;
}

bool DissimilarLibraryOperator::getDissimilarParametersName(int nID, QStringList& sl)
{
    sl.clear();

    if (nID == 0)
    {
        sl << "ID" << "Name" << "Width" << "Height" << "a" << "b" << "c" << "d" << "e" << "f" << "g" << "h";
    }
    else if (nID == 1)
    {
        sl << "ID" << "Name" << "Width" << "Height" << "r1" << "r2" << "r3" << "r4";
    }
    else if (nID == 2)
    {
        sl << "ID" << "Name" << "Width" << "Height" << "a" << "b" << "c" << "d" << "e" << "f" << "g" << "h";
    }
    else if (nID == 3)
    {
        sl << "ID" << "Name" << "Width" << "Height" << "a" << "b" << "c";
    }
    else if (nID == 4)
    {
        sl << "ID" << "Name" << "Width" << "Height" << "a" << "b" << "c";
    }
    else if (nID == 5)
    {
        sl << "ID" << "Name" << "Width" << "Height" << "r" << "a" << "b";
    }
    else if (nID == 6)
    {
        sl << "ID" << "Name" << "Width" << "Height" << "a" << "b" << "W" << "H";
    }
    else if (nID == 7)
    {
        sl << "ID" << "Name" << "Width" << "Height" << "a" << "W" << "Depth";
    }
    else if (nID == 8)
    {
        sl << "ID" << "Name" << "Width" << "Height" << "a" << "W" << "Depth";
    }
    else if (nID == 9)
    {
        sl << "ID" << "Name" << "Width" << "Height" << "a" << "b" << "r1" << "r2" << "r3";
    }
    else if (nID == 10)
    {
        sl << "ID" << "Name" << "Width" << "Height" << "a" << "b" << "r1" << "r2" << "r3";
    }
    else if (nID == 11)
    {
        sl << "ID" << "Name" << "Width" << "Height" << "a" << "r1" << "r2" << "n";
    }
    else if (nID == 12)
    {
        sl << "ID" << "Name" << "Width" << "Height" << "a" << "b" << "c" << "n";
    }
    else if (nID == 13)
    {
        sl << "ID" << "Name" << "Width" << "Height" << "a" << "b" << "W" << "H" << "Depth";
    }
    else if (nID == 14)
    {
        sl << "ID" << "Name" << "Width" << "Height" << "a" << "b" << "c" << "d" << "e" << "f" << "g" << "h";
    }
    else
    {
        return false;
    }

    return true;
}

bool DissimilarLibraryOperator::draw2QPainter(QPainter& painter, int nID, std::map<QString, QVariant>& paramsMap)
{
    if (nID == 0)
    {
        return draw2QPainterID000(painter, paramsMap);
    }
    else if (nID == 1)
    {
        return draw2QPainterID001(painter, paramsMap);
    }
    else if (nID == 2)
    {
        return draw2QPainterID002(painter, paramsMap);
    }
    else if (nID == 3)
    {
        return draw2QPainterID003(painter, paramsMap);
    }
    else if (nID == 4)
    {
        return draw2QPainterID004(painter, paramsMap);
    }
    else if (nID == 5)
    {
        return draw2QPainterID005(painter, paramsMap);
    }
    else if (nID == 6)
    {
        return draw2QPainterID006(painter, paramsMap);
    }
    else if (nID == 7)
    {
        return draw2QPainterID007(painter, paramsMap);
    }
    else if (nID == 8)
    {
        return draw2QPainterID008(painter, paramsMap);
    }
    else if (nID == 9)
    {
        return draw2QPainterID009(painter, paramsMap);
    }
    else if (nID == 10)
    {
        return draw2QPainterID010(painter, paramsMap);
    }
    else if (nID == 11)
    {
        return draw2QPainterID011(painter, paramsMap);
    }
    else if (nID == 12)
    {
        return draw2QPainterID012(painter, paramsMap);
    }
    else if (nID == 13)
    {
        return draw2QPainterID013(painter, paramsMap);
    }
    else if (nID == 14)
    {
        return draw2QPainterID014(painter, paramsMap);
    }
    else
    {
        return false;
    }

    return true;
}

bool DissimilarLibraryOperator::draw2QPainterID000(QPainter& painter, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dB = 0.0f;
    double dC = 0.0f;
    double dD = 0.0f;
    double dE = 0.0f;
    double dF = 0.0f;
    double dG = 0.0f;
    double dH = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("b") != paramsMap.end())
    {
        var = paramsMap["b"];
        dB = var.toDouble(&ok);
        if (!ok)
        {
            dB = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("c") != paramsMap.end())
    {
        var = paramsMap["c"];
        dC = var.toDouble(&ok);
        if (!ok)
        {
            dC = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("d") != paramsMap.end())
    {
        var = paramsMap["d"];
        dD = var.toDouble(&ok);
        if (!ok)
        {
            dD = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("e") != paramsMap.end())
    {
        var = paramsMap["e"];
        dE = var.toDouble(&ok);
        if (!ok)
        {
            dE = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("f") != paramsMap.end())
    {
        var = paramsMap["f"];
        dF = var.toDouble(&ok);
        if (!ok)
        {
            dF = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("g") != paramsMap.end())
    {
        var = paramsMap["g"];
        dG = var.toDouble(&ok);
        if (!ok)
        {
            dG = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("h") != paramsMap.end())
    {
        var = paramsMap["h"];
        dH = var.toDouble(&ok);
        if (!ok)
        {
            dH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    QRect rc = painter.viewport();
    double dDrawerW = rc.width();
    double dDrawerH = rc.height();

    double dBoardWHScale = dBoardW/dBoardH;
    double dDrawerWHScale = dDrawerW/dDrawerH;

    double dRealW = 0.0f;
    double dRealH = 0.0f;

    double dRealX = 0.0f;
    double dRealY = 0.0f;

    // 以画布的高为基准
    if (dBoardWHScale < dDrawerWHScale)
    {
        dRealH = dDrawerH;
        dRealW = dRealH*dBoardWHScale;

        dRealX = (dDrawerW-dRealW)/2.f;
        dRealY = 0.0f;
    }
    // 以画布的宽为基准
    else
    {
        dRealW = dDrawerW;
        dRealH = dRealW/dBoardWHScale;

        dRealX = 0.0f;
        dRealY = (dDrawerH-dRealH)/2.0f;
    }

    // 往中心收缩4个像素，空出边缘绘图
    dRealX += 4.0;
    dRealY += 4.0;
    dRealW -= 8.0;
    dRealH -= 8.0;

    double dYScale = dRealH/dBoardH;
    double dXScale = dRealW/dBoardW;
    dA = dA * dXScale;
    dB = dB * dYScale;

    dC = dC * dXScale;
    dD = dD * dYScale;

    dE = dE * dXScale;
    dF = dF * dYScale;

    dH = dH * dXScale;
    dG = dG * dYScale;

    QPen pen(QColor(255, 255, 255), 2.0f);
    painter.setPen(pen);

    QPointF pt1(dRealX+dA, dRealY);
    QPointF pt2(dRealX+dA, dRealY+dB);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX);
    pt2.setY(dRealY+dB);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX);
    pt2.setY(dRealY+dRealH-dG);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dH);
    pt2.setY(dRealY+dRealH-dG);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dH);
    pt2.setY(dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW-dE);
    pt2.setY(dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW-dE);
    pt2.setY(dRealY+dRealH-dF);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY+dRealH-dF);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY+dC);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW-dD);
    pt2.setY(dRealY+dC);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW-dD);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dA);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    return true;
}

bool DissimilarLibraryOperator::draw2QPainterID001(QPainter& painter, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dR1 = 0.0f;
    double dR2 = 0.0f;
    double dR3 = 0.0f;
    double dR4 = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r1") != paramsMap.end())
    {
        var= paramsMap["r1"];
        dR1 = var.toDouble(&ok);
        if (!ok)
        {
            dR1 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r2") != paramsMap.end())
    {
        var = paramsMap["r2"];
        dR2 = var.toDouble(&ok);
        if (!ok)
        {
            dR2 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r3") != paramsMap.end())
    {
        var = paramsMap["r3"];
        dR3 = var.toDouble(&ok);
        if (!ok)
        {
            dR3 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r4") != paramsMap.end())
    {
        var = paramsMap["r4"];
        dR4 = var.toDouble(&ok);
        if (!ok)
        {
            dR4 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    QRect rc = painter.viewport();
    double dDrawerW = rc.width();
    double dDrawerH = rc.height();

    double dBoardWHScale = dBoardW/dBoardH;
    double dDrawerWHScale = dDrawerW/dDrawerH;

    double dRealW = 0.0f;
    double dRealH = 0.0f;

    double dRealX = 0.0f;
    double dRealY = 0.0f;

    // 以画布的高为基准
    if (dBoardWHScale < dDrawerWHScale)
    {
        dRealH = dDrawerH;
        dRealW = dRealH*dBoardWHScale;

        dRealX = (dDrawerW-dRealW)/2.f;
        dRealY = 0.0f;
    }
    // 以画布的宽为基准
    else
    {
        dRealW = dDrawerW;
        dRealH = dRealW/dBoardWHScale;

        dRealX = 0.0f;
        dRealY = (dDrawerH-dRealH)/2.0f;
    }

    // 往中心收缩4个像素，空出边缘绘图
    dRealX += 4.0;
    dRealY += 4.0;
    dRealW -= 8.0;
    dRealH -= 8.0;

    double dXScale = dRealW/dBoardW;
    //double dYScale = dRealH/dBoardH;

    dR1 = dR1 * dXScale;
    dR2 = dR2 * dXScale;
    dR3 = dR3 * dXScale;
    dR4 = dR4 * dXScale;

    QPen pen(QColor(255, 255, 255), 2.0f);
    painter.setPen(pen);

    QPointF pt1(dRealX, dRealY);
    QPointF pt2(dRealX+dR1*2, dRealY+dR1*2);
    int nStartAngle = 90 * 16;
    int nSpanAngle = 90 * 16;
    painter.drawArc(QRectF(pt1, pt2), nStartAngle, nSpanAngle);

    pt1.setX(dRealX);
    pt1.setY(dRealY+dR1);
    pt2.setX(dRealX);
    pt2.setY(dRealY+dRealH-dR4);
    painter.drawLine(pt1, pt2);

    pt1.setX(dRealX);
    pt1.setY(dRealY+dRealH-dR4*2);
    pt2.setX(pt1.x() + dR4*2);
    pt2.setY(pt1.y() + dR4*2);
    nStartAngle = 180 * 16;
    nSpanAngle = 90 * 16;
    painter.drawArc(QRectF(pt1, pt2), nStartAngle, nSpanAngle);

    pt1.setX(dRealX+dR4);
    pt1.setY(dRealY+dRealH);
    pt2.setX(dRealX+dRealW-dR3);
    pt2.setY(dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1.setX(dRealX+dRealW-dR3*2);
    pt1.setY(dRealY+dRealH-dR3*2);
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY+dRealH);
    nStartAngle = 270 * 16;
    nSpanAngle = 90 * 16;
    painter.drawArc(QRectF(pt1, pt2), nStartAngle, nSpanAngle);

    pt1.setX(dRealX+dRealW);
    pt1.setY(dRealY+dRealH-dR3);
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY+dR2);
    painter.drawLine(pt1, pt2);

    pt1.setX(dRealX+dRealW-dR2*2);
    pt1.setY(dRealY);
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY+dR2*2);
    nStartAngle = 0 * 16;
    nSpanAngle = 90 * 16;
    painter.drawArc(QRectF(pt1, pt2), nStartAngle, nSpanAngle);

    pt1.setX(dRealX+dRealW-dR2);
    pt1.setY(dRealY);
    pt2.setX(dRealX+dR1);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    return true;
}

bool DissimilarLibraryOperator::draw2QPainterID002(QPainter& painter, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dB = 0.0f;
    double dC = 0.0f;
    double dD = 0.0f;
    double dE = 0.0f;
    double dF = 0.0f;
    double dG = 0.0f;
    double dH = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("b") != paramsMap.end())
    {
        var = paramsMap["b"];
        dB = var.toDouble(&ok);
        if (!ok)
        {
            dB = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("c") != paramsMap.end())
    {
        var = paramsMap["c"];
        dC = var.toDouble(&ok);
        if (!ok)
        {
            dC = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("d") != paramsMap.end())
    {
        var = paramsMap["d"];
        dD = var.toDouble(&ok);
        if (!ok)
        {
            dD = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("e") != paramsMap.end())
    {
        var = paramsMap["e"];
        dE = var.toDouble(&ok);
        if (!ok)
        {
            dE = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("f") != paramsMap.end())
    {
        var = paramsMap["f"];
        dF = var.toDouble(&ok);
        if (!ok)
        {
            dF = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("g") != paramsMap.end())
    {
        var = paramsMap["g"];
        dG = var.toDouble(&ok);
        if (!ok)
        {
            dG = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("h") != paramsMap.end())
    {
        var = paramsMap["h"];
        dH = var.toDouble(&ok);
        if (!ok)
        {
            dH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    QRect rc = painter.viewport();
    double dDrawerW = rc.width();
    double dDrawerH = rc.height();

    double dBoardWHScale = dBoardW/dBoardH;
    double dDrawerWHScale = dDrawerW/dDrawerH;

    double dRealW = 0.0f;
    double dRealH = 0.0f;

    double dRealX = 0.0f;
    double dRealY = 0.0f;

    // 以画布的高为基准
    if (dBoardWHScale < dDrawerWHScale)
    {
        dRealH = dDrawerH;
        dRealW = dRealH*dBoardWHScale;

        dRealX = (dDrawerW-dRealW)/2.f;
        dRealY = 0.0f;
    }
    // 以画布的宽为基准
    else
    {
        dRealW = dDrawerW;
        dRealH = dRealW/dBoardWHScale;

        dRealX = 0.0f;
        dRealY = (dDrawerH-dRealH)/2.0f;
    }

    // 往中心收缩4个像素，空出边缘绘图
    dRealX += 4.0;
    dRealY += 4.0;
    dRealW -= 8.0;
    dRealH -= 8.0;

    double dYScale = dRealH/dBoardH;
    double dXScale = dRealW/dBoardW;
    dA = dA * dYScale;
    dB = dB * dXScale;

    dC = dC * dXScale;
    dD = dD * dYScale;

    dE = dE * dYScale;
    dF = dF * dXScale;

    dH = dH * dYScale;
    dG = dG * dXScale;

    QPen pen(QColor(255, 255, 255), 2.0f);
    painter.setPen(pen);

    QPointF pt1(dRealX+dB, dRealY);
    QPointF pt2(dRealX, dRealY+dA);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX);
    pt2.setY(dRealY+dRealH-dH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dG);
    pt2.setY(dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW-dF);
    pt2.setY(dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY+dRealH-dE);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY+dD);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW-dC);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dB);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    return true;
}

bool DissimilarLibraryOperator::draw2QPainterID003(QPainter& painter, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dB = 0.0f;
    double dC = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("b") != paramsMap.end())
    {
        var = paramsMap["b"];
        dB = var.toDouble(&ok);
        if (!ok)
        {
            dB = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("c") != paramsMap.end())
    {
        var = paramsMap["c"];
        dC = var.toDouble(&ok);
        if (!ok)
        {
            dC = 0.0f;
        }
    }
    else
    {
        return false;
    }

    QRect rc = painter.viewport();
    double dDrawerW = rc.width();
    double dDrawerH = rc.height();

    double dBoardWHScale = dBoardW/dBoardH;
    double dDrawerWHScale = dDrawerW/dDrawerH;

    double dRealW = 0.0f;
    double dRealH = 0.0f;

    double dRealX = 0.0f;
    double dRealY = 0.0f;

    // 以画布的高为基准
    if (dBoardWHScale < dDrawerWHScale)
    {
        dRealH = dDrawerH;
        dRealW = dRealH*dBoardWHScale;

        dRealX = (dDrawerW-dRealW)/2.f;
        dRealY = 0.0f;
    }
    // 以画布的宽为基准
    else
    {
        dRealW = dDrawerW;
        dRealH = dRealW/dBoardWHScale;

        dRealX = 0.0f;
        dRealY = (dDrawerH-dRealH)/2.0f;
    }

    // 往中心收缩4个像素，空出边缘绘图
    dRealX += 4.0;
    dRealY += 4.0;
    dRealW -= 8.0;
    dRealH -= 8.0;

    double dYScale = dRealH/dBoardH;
    double dXScale = dRealW/dBoardW;
    dA = dA * dXScale;
    dB = dB * dYScale;
    dC = dC * dXScale;

    QPen pen(QColor(255, 255, 255), 2.0f);
    painter.setPen(pen);

    QPointF pt1(dRealX, dRealY);
    QPointF pt2(dRealX, dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dA+dC);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dA+dC);
    pt2.setY(dRealY+dB);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dA);
    pt2.setY(dRealY+dB);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dA);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    return true;
}

bool DissimilarLibraryOperator::draw2QPainterID004(QPainter& painter, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dB = 0.0f;
    double dC = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }

    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("b") != paramsMap.end())
    {
        var = paramsMap["b"];
        dB = var.toDouble(&ok);
        if (!ok)
        {
            dB = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("c") != paramsMap.end())
    {
        var = paramsMap["c"];
        dC = var.toDouble(&ok);
        if (!ok)
        {
            dC = 0.0f;
        }
    }
    else
    {
        return false;
    }

    QRect rc = painter.viewport();
    double dDrawerW = rc.width();
    double dDrawerH = rc.height();

    double dBoardWHScale = dBoardW/dBoardH;
    double dDrawerWHScale = dDrawerW/dDrawerH;

    double dRealW = 0.0f;
    double dRealH = 0.0f;

    double dRealX = 0.0f;
    double dRealY = 0.0f;

    // 以画布的高为基准
    if (dBoardWHScale < dDrawerWHScale)
    {
        dRealH = dDrawerH;
        dRealW = dRealH*dBoardWHScale;

        dRealX = (dDrawerW-dRealW)/2.f;
        dRealY = 0.0f;
    }
    // 以画布的宽为基准
    else
    {
        dRealW = dDrawerW;
        dRealH = dRealW/dBoardWHScale;

        dRealX = 0.0f;
        dRealY = (dDrawerH-dRealH)/2.0f;
    }

    // 往中心收缩4个像素，空出边缘绘图
    dRealX += 4.0;
    dRealY += 4.0;
    dRealW -= 8.0;
    dRealH -= 8.0;

    double dYScale = dRealH/dBoardH;
    double dXScale = dRealW/dBoardW;
    dA = dA * dXScale;
    dB = dB * dYScale;
    dC = dC * dXScale;

    QPen pen(QColor(255, 255, 255), 2.0f);
    painter.setPen(pen);

    QPointF pt1(dRealX, dRealY);
    QPointF pt2(dRealX, dRealY+dA);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dB);
    pt2.setY(dRealY+dA);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dB);
    pt2.setY(dRealY+dA+dC);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX);
    pt2.setY(dRealY+dA+dC);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX);
    pt2.setY(dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    return true;
}

bool DissimilarLibraryOperator::draw2QPainterID005(QPainter& painter, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dB = 0.0f;
    double dR = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("b") != paramsMap.end())
    {
        var = paramsMap["b"];
        dB = var.toDouble(&ok);
        if (!ok)
        {
            dB = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r") != paramsMap.end())
    {
        var = paramsMap["r"];
        dR = var.toDouble(&ok);
        if (!ok)
        {
            dR = 0.0f;
        }
    }
    else
    {
        return false;
    }

    QRect rc = painter.viewport();
    double dDrawerW = rc.width();
    double dDrawerH = rc.height();

    double dBoardWHScale = dBoardW/dBoardH;
    double dDrawerWHScale = dDrawerW/dDrawerH;

    double dRealW = 0.0f;
    double dRealH = 0.0f;

    double dRealX = 0.0f;
    double dRealY = 0.0f;

    // 以画布的高为基准
    if (dBoardWHScale < dDrawerWHScale)
    {
        dRealH = dDrawerH;
        dRealW = dRealH*dBoardWHScale;

        dRealX = (dDrawerW-dRealW)/2.f;
        dRealY = 0.0f;
    }
    // 以画布的宽为基准
    else
    {
        dRealW = dDrawerW;
        dRealH = dRealW/dBoardWHScale;

        dRealX = 0.0f;
        dRealY = (dDrawerH-dRealH)/2.0f;
    }

    // 往中心收缩4个像素，空出边缘绘图
    dRealX += 4.0;
    dRealY += 4.0;
    dRealW -= 8.0;
    dRealH -= 8.0;

    double dYScale = dRealH/dBoardH;
    double dXScale = dRealW/dBoardW;
    dA = dA * dXScale;
    dB = dB * dYScale;
    dR = dR * dXScale;

    QPen pen(QColor(255, 255, 255), 2.0f);
    painter.setPen(pen);

    QPointF pt1(dRealX, dRealY);
    QPointF pt2(dRealX, dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    QPointF ptCenter(dRealX+dA, dRealY+dRealH-dB);
    painter.drawEllipse(ptCenter, dR, dR);

    return true;
}

bool DissimilarLibraryOperator::draw2QPainterID006(QPainter& painter, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dB = 0.0f;
    double dW = 0.0f;
    double dH = 0.0f;

    bool ok = false;
    QVariant var;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("b") != paramsMap.end())
    {
        var = paramsMap["b"];
        dB = var.toDouble(&ok);
        if (!ok)
        {
            dB = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("W") != paramsMap.end())
    {
        var = paramsMap["W"];
        dW = var.toDouble(&ok);
        if (!ok)
        {
            dW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("H") != paramsMap.end())
    {
        var = paramsMap["H"];
        dH = var.toDouble(&ok);
        if (!ok)
        {
            dH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    QRect rc = painter.viewport();
    double dDrawerW = rc.width();
    double dDrawerH = rc.height();

    double dBoardWHScale = dBoardW/dBoardH;
    double dDrawerWHScale = dDrawerW/dDrawerH;

    double dRealW = 0.0f;
    double dRealH = 0.0f;

    double dRealX = 0.0f;
    double dRealY = 0.0f;

    // 以画布的高为基准
    if (dBoardWHScale < dDrawerWHScale)
    {
        dRealH = dDrawerH;
        dRealW = dRealH*dBoardWHScale;

        dRealX = (dDrawerW-dRealW)/2.f;
        dRealY = 0.0f;
    }
    // 以画布的宽为基准
    else
    {
        dRealW = dDrawerW;
        dRealH = dRealW/dBoardWHScale;

        dRealX = 0.0f;
        dRealY = (dDrawerH-dRealH)/2.0f;
    }

    // 往中心收缩4个像素，空出边缘绘图
    dRealX += 4.0;
    dRealY += 4.0;
    dRealW -= 8.0;
    dRealH -= 8.0;

    double dYScale = dRealH/dBoardH;
    double dXScale = dRealW/dBoardW;
    dA = dA * dXScale;
    dB = dB * dYScale;
    dW = dW * dXScale;
    dH = dH * dYScale;

    QPen pen(QColor(255, 255, 255), 2.0f);
    painter.setPen(pen);

    QPointF pt1(dRealX, dRealY);
    QPointF pt2(dRealX, dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    pt1.setX(dRealX+dA);
    pt1.setY(dRealY+dRealH-dB-dH);
    pt2.setX(dRealX+dA+dW);
    pt2.setY(dRealY+dRealH-dB);

    painter.drawRect(QRectF(pt1, pt2));

    return true;
}

bool DissimilarLibraryOperator::draw2QPainterID007(QPainter& painter, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dW = 0.0f;
    double dDepth = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("W") != paramsMap.end())
    {
        var = paramsMap["W"];
        dW = var.toDouble(&ok);
        if (!ok)
        {
            dW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Depth") != paramsMap.end())
    {
        var = paramsMap["Depth"];
        dDepth = var.toDouble(&ok);
        if (!ok)
        {
            dDepth = 0.0f;
        }
    }
    else
    {
        return false;
    }

    QRect rc = painter.viewport();
    double dDrawerW = rc.width();
    double dDrawerH = rc.height();

    double dBoardWHScale = dBoardW/dBoardH;
    double dDrawerWHScale = dDrawerW/dDrawerH;

    double dRealW = 0.0f;
    double dRealH = 0.0f;

    double dRealX = 0.0f;
    double dRealY = 0.0f;

    // 以画布的高为基准
    if (dBoardWHScale < dDrawerWHScale)
    {
        dRealH = dDrawerH;
        dRealW = dRealH*dBoardWHScale;

        dRealX = (dDrawerW-dRealW)/2.f;
        dRealY = 0.0f;
    }
    // 以画布的宽为基准
    else
    {
        dRealW = dDrawerW;
        dRealH = dRealW/dBoardWHScale;

        dRealX = 0.0f;
        dRealY = (dDrawerH-dRealH)/2.0f;
    }

    // 往中心收缩4个像素，空出边缘绘图
    dRealX += 4.0;
    dRealY += 4.0;
    dRealW -= 8.0;
    dRealH -= 8.0;

    double dYScale = dRealH/dBoardH;
    //double dXScale = dRealW/dBoardW;

    dA = dA * dYScale;
    dW = dW * dYScale;

    QPen pen(QColor(255, 255, 255), 2.0f);
    painter.setPen(pen);

    QPointF pt1(dRealX, dRealY);
    QPointF pt2(dRealX, dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    QRectF rcf(dRealX, dRealY+dA, dRealW, dW);
    painter.drawRect(rcf);
    painter.fillRect(rcf, QBrush(QColor(255, 255, 255)));

    return true;
}

bool DissimilarLibraryOperator::draw2QPainterID008(QPainter& painter, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dW = 0.0f;
    double dDepth = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("W") != paramsMap.end())
    {
        var = paramsMap["W"];
        dW = var.toDouble(&ok);
        if (!ok)
        {
            dW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Depth") != paramsMap.end())
    {
        var = paramsMap["Depth"];
        dDepth = var.toDouble(&ok);
        if (!ok)
        {
            dDepth = 0.0f;
        }
    }
    else
    {
        return false;
    }

    QRect rc = painter.viewport();
    double dDrawerW = rc.width();
    double dDrawerH = rc.height();

    double dBoardWHScale = dBoardW/dBoardH;
    double dDrawerWHScale = dDrawerW/dDrawerH;

    double dRealW = 0.0f;
    double dRealH = 0.0f;

    double dRealX = 0.0f;
    double dRealY = 0.0f;

    // 以画布的高为基准
    if (dBoardWHScale < dDrawerWHScale)
    {
        dRealH = dDrawerH;
        dRealW = dRealH*dBoardWHScale;

        dRealX = (dDrawerW-dRealW)/2.f;
        dRealY = 0.0f;
    }
    // 以画布的宽为基准
    else
    {
        dRealW = dDrawerW;
        dRealH = dRealW/dBoardWHScale;

        dRealX = 0.0f;
        dRealY = (dDrawerH-dRealH)/2.0f;
    }

    // 往中心收缩4个像素，空出边缘绘图
    dRealX += 4.0;
    dRealY += 4.0;
    dRealW -= 8.0;
    dRealH -= 8.0;

    double dYScale = dRealH/dBoardH;
    //double dXScale = dRealW/dBoardW;

    dA = dA * dYScale;
    dW = dW * dYScale;

    QPen pen(QColor(255, 255, 255), 2.0f);
    painter.setPen(pen);

    QPointF pt1(dRealX, dRealY);
    QPointF pt2(dRealX, dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    QRectF rcf(dRealX+(dRealW-dA-dW), dRealY, dW, dRealH);
    painter.drawRect(rcf);
    painter.fillRect(rcf, QBrush(QColor(255, 255, 255)));

    return true;
}

bool DissimilarLibraryOperator::draw2QPainterID009(QPainter& painter, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dB = 0.0f;
    double dR1 = 0.0f;
    double dR2 = 0.0f;
    double dR3 = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("b") != paramsMap.end())
    {
        var = paramsMap["b"];
        dB = var.toDouble(&ok);
        if (!ok)
        {
            dB = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r1") != paramsMap.end())
    {
        var = paramsMap["r1"];
        dR1 = var.toDouble(&ok);
        if (!ok)
        {
            dR1 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r2") != paramsMap.end())
    {
        var = paramsMap["r2"];
        dR2 = var.toDouble(&ok);
        if (!ok)
        {
            dR2 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r3") != paramsMap.end())
    {
        var = paramsMap["r3"];
        dR3 = var.toDouble(&ok);
        if (!ok)
        {
            dR3 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    QRect rc = painter.viewport();
    double dDrawerW = rc.width();
    double dDrawerH = rc.height();

    double dBoardWHScale = dBoardW/dBoardH;
    double dDrawerWHScale = dDrawerW/dDrawerH;

    double dRealW = 0.0f;
    double dRealH = 0.0f;

    double dRealX = 0.0f;
    double dRealY = 0.0f;

    // 以画布的高为基准
    if (dBoardWHScale < dDrawerWHScale)
    {
        dRealH = dDrawerH;
        dRealW = dRealH*dBoardWHScale;

        dRealX = (dDrawerW-dRealW)/2.f;
        dRealY = 0.0f;
    }
    // 以画布的宽为基准
    else
    {
        dRealW = dDrawerW;
        dRealH = dRealW/dBoardWHScale;

        dRealX = 0.0f;
        dRealY = (dDrawerH-dRealH)/2.0f;
    }

    // 往中心收缩4个像素，空出边缘绘图
    dRealX += 4.0;
    dRealY += 4.0;
    dRealW -= 8.0;
    dRealH -= 8.0;

    double dYScale = dRealH/dBoardH;
    double dXScale = dRealW/dBoardW;
    dA = dA * dXScale;
    dB = dB * dYScale;
    dR1 = dR1 * dXScale;
    dR2 = dR2 * dXScale;
    dR3 = dR3 * dXScale;

    QPen pen(QColor(255, 255, 255), 2.0f);
    painter.setPen(pen);

    QPointF pt1(dRealX, dRealY);
    QPointF pt2(dRealX, dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY+dRealH-dB+dR3);
    painter.drawLine(pt1, pt2);

    pt1.setX(dRealX+dRealW-dR3*2);
    pt1.setY(dRealY+dRealH-dB);
    pt2.setX(pt1.x() + dR3*2);
    pt2.setY(pt1.y() + dR3*2);
    int nStartAngle = 0 * 16;
    int nSpanAngle = 90 * 16;
    painter.drawArc(QRectF(pt1, pt2), nStartAngle, nSpanAngle);

    pt1.setX(dRealX+dRealW-dR3);
    pt1.setY(dRealY+dRealH-dB);
    pt2.setX(dRealX+dA+dR2);
    pt2.setY(dRealY+dRealH-dB);
    painter.drawLine(pt1, pt2);

    pt1.setX(dRealX+dA);
    pt1.setY(dRealY+dRealH-dB-dR2*2);
    pt2.setX(pt1.x() + dR2*2);
    pt2.setY(pt1.y() + dR2*2);
    nStartAngle = 180 * 16;
    nSpanAngle = 90 * 16;
    painter.drawArc(QRectF(pt1, pt2), nStartAngle, nSpanAngle);

    pt1.setX(dRealX+dA);
    pt1.setY(dRealY+dRealH-dB-dR2);
    pt2.setX(dRealX+dA);
    pt2.setY(dRealY+dR1);
    painter.drawLine(pt1, pt2);

    pt1.setX(dRealX+dA-dR1*2);
    pt1.setY(dRealY);
    pt2.setX(pt1.x() + dR1*2);
    pt2.setY(pt1.y() + dR1*2);
    nStartAngle = 0 * 16;
    nSpanAngle = 90 * 16;
    painter.drawArc(QRectF(pt1, pt2), nStartAngle, nSpanAngle);

    pt1.setX(dRealX+dA-dR1);
    pt1.setY(dRealY);
    pt2.setX(dRealX);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);


    return true;
}

bool DissimilarLibraryOperator::draw2QPainterID010(QPainter& painter, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dB = 0.0f;
    double dR1 = 0.0f;
    double dR2 = 0.0f;
    double dR3 = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("b") != paramsMap.end())
    {
        var = paramsMap["b"];
        dB = var.toDouble(&ok);
        if (!ok)
        {
            dB = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r1") != paramsMap.end())
    {
        var = paramsMap["r1"];
        dR1 = var.toDouble(&ok);
        if (!ok)
        {
            dR1 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r2") != paramsMap.end())
    {
        var = paramsMap["r2"];
        dR2 = var.toDouble(&ok);
        if (!ok)
        {
            dR2 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r3") != paramsMap.end())
    {
        var = paramsMap["r3"];
        dR3 = var.toDouble(&ok);
        if (!ok)
        {
            dR3 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    QRect rc = painter.viewport();
    double dDrawerW = rc.width();
    double dDrawerH = rc.height();

    double dBoardWHScale = dBoardW/dBoardH;
    double dDrawerWHScale = dDrawerW/dDrawerH;

    double dRealW = 0.0f;
    double dRealH = 0.0f;

    double dRealX = 0.0f;
    double dRealY = 0.0f;

    // 以画布的高为基准
    if (dBoardWHScale < dDrawerWHScale)
    {
        dRealH = dDrawerH;
        dRealW = dRealH*dBoardWHScale;

        dRealX = (dDrawerW-dRealW)/2.f;
        dRealY = 0.0f;
    }
    // 以画布的宽为基准
    else
    {
        dRealW = dDrawerW;
        dRealH = dRealW/dBoardWHScale;

        dRealX = 0.0f;
        dRealY = (dDrawerH-dRealH)/2.0f;
    }

    // 往中心收缩4个像素，空出边缘绘图
    dRealX += 4.0;
    dRealY += 4.0;
    dRealW -= 8.0;
    dRealH -= 8.0;

    double dYScale = dRealH/dBoardH;
    double dXScale = dRealW/dBoardW;
    dA = dA * dXScale;
    dB = dB * dYScale;
    dR1 = dR1 * dXScale;
    dR2 = dR2 * dXScale;
    dR3 = dR3 * dXScale;

    QPen pen(QColor(255, 255, 255), 2.0f);
    painter.setPen(pen);

    QPointF pt1(dRealX+dRealW, dRealY);
    QPointF pt2(dRealX, dRealY);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX);
    pt2.setY(dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dA-dR1);
    pt2.setY(dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1.setX(dRealX+dA-dR1*2);
    pt1.setY(dRealY+dRealH-dR1*2);
    pt2.setX(pt1.x() + dR1*2);
    pt2.setY(pt1.y() + dR1*2);
    int nStartAngle = 270 * 16;
    int nSpanAngle = 90 * 16;
    painter.drawArc(QRectF(pt1, pt2), nStartAngle, nSpanAngle);

    pt1.setX(dRealX+dA);
    pt1.setY(dRealY+dRealH-dR1);
    pt2.setX(dRealX+dA);
    pt2.setY(dRealY+dB+dR2);
    painter.drawLine(pt1, pt2);

    pt1.setX(dRealX+dA);
    pt1.setY(dRealY+dB);
    pt2.setX(pt1.x() + dR2*2);
    pt2.setY(pt1.y() + dR2*2);
    nStartAngle = 90 * 16;
    nSpanAngle = 90 * 16;
    painter.drawArc(QRectF(pt1, pt2), nStartAngle, nSpanAngle);

    pt1.setX(dRealX+dA+dR2);
    pt1.setY(dRealY+dB);
    pt2.setX(dRealX+dRealW-dR3);
    pt2.setY(dRealY+dB);
    painter.drawLine(pt1, pt2);

    pt1.setX(dRealX+dRealW-dR3*2);
    pt1.setY(dRealY+dB-dR3*2);
    pt2.setX(pt1.x() + dR3*2);
    pt2.setY(pt1.y() + dR3*2);
    nStartAngle = 270 * 16;
    nSpanAngle = 90 * 16;
    painter.drawArc(QRectF(pt1, pt2), nStartAngle, nSpanAngle);

    pt1.setX(dRealX+dRealW);
    pt1.setY(dRealY+dB-dR3);
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);


    return true;
}

bool DissimilarLibraryOperator::draw2QPainterID011(QPainter& painter, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dR1 = 0.0f;
    double dR2 = 0.0f;
    int nN = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r1") != paramsMap.end())
    {
        var = paramsMap["r1"];
        dR1 = var.toDouble(&ok);
        if (!ok)
        {
            dR1 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r2") != paramsMap.end())
    {
        var = paramsMap["r2"];
        dR2 = var.toDouble(&ok);
        if (!ok)
        {
            dR2 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("n") != paramsMap.end())
    {
        var = paramsMap["n"];
        nN = var.toInt(&ok);
        if (!ok)
        {
            nN = 0;
        }
    }
    else
    {
        return false;
    }

    QRect rc = painter.viewport();
    double dDrawerW = rc.width();
    double dDrawerH = rc.height();

    double dBoardWHScale = dBoardW/dBoardH;
    double dDrawerWHScale = dDrawerW/dDrawerH;

    double dRealW = 0.0f;
    double dRealH = 0.0f;

    double dRealX = 0.0f;
    double dRealY = 0.0f;

    // 以画布的高为基准
    if (dBoardWHScale < dDrawerWHScale)
    {
        dRealH = dDrawerH;
        dRealW = dRealH*dBoardWHScale;

        dRealX = (dDrawerW-dRealW)/2.f;
        dRealY = 0.0f;
    }
    // 以画布的宽为基准
    else
    {
        dRealW = dDrawerW;
        dRealH = dRealW/dBoardWHScale;

        dRealX = 0.0f;
        dRealY = (dDrawerH-dRealH)/2.0f;
    }

    // 往中心收缩4个像素，空出边缘绘图
    dRealX += 4.0;
    dRealY += 4.0;
    dRealW -= 8.0;
    dRealH -= 8.0;

    double dYScale = dRealH/dBoardH;
    double dXScale = dRealW/dBoardW;
    dA = dA * dYScale;
    dR1 = dR1 * dXScale;
    dR2 = dR2 * dXScale;

    QPen pen(QColor(255, 255, 255), 2.0f);
    painter.setPen(pen);

    QPointF pt1(dRealX, dRealY);
    QPointF pt2(dRealX+dR2*2, dRealY+dR2*2);
    int nStartAngle = 90 * 16;
    int nSpanAngle = 90 * 16;
    painter.drawArc(QRectF(pt1, pt2), nStartAngle, nSpanAngle);

    pt1.setX(dRealX);
    pt1.setY(dRealY+dR2);
    pt2.setX(dRealX);
    pt2.setY(dRealY+dRealH-dR1);
    painter.drawLine(pt1, pt2);

    pt1.setX(dRealX);
    pt1.setY(dRealY+dRealH-dR1*2);
    pt2.setX(pt1.x() + dR1*2);
    pt2.setY(pt1.y() + dR1*2);
    nStartAngle = 180 * 16;
    nSpanAngle = 90 * 16;
    painter.drawArc(QRectF(pt1, pt2), nStartAngle, nSpanAngle);

    pt1.setX(dRealX+dR1);
    pt1.setY(dRealY+dRealH);
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1.setX(dRealX+dR2);
    pt1.setY(dRealY);
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    if (nN <= 0)
    {
        pt1.setX(dRealX+dRealW);
        pt1.setY(dRealY+dRealH);
        pt2.setX(dRealX+dRealW);
        pt2.setY(dRealY);
        painter.drawLine(pt1, pt2);
    }
    else
    {
        double dDepth = dRealW/2.0f;
        double dAvgH = (dRealH-dA*nN)/(nN+1);
        for (int i = 0; i < nN; ++i)
        {
            pt1.setX(dRealX+dRealW);
            pt1.setY(dRealY+i*(dAvgH+dA));
            pt2.setX(dRealX+dRealW);
            pt2.setY(dRealY+i*(dAvgH+dA)+dAvgH);
            painter.drawLine(pt1, pt2);

            pt1 = pt2;
            pt2.setX(dRealX+dDepth);
            pt2.setY(dRealY+i*(dAvgH+dA)+dAvgH);
            painter.drawLine(pt1, pt2);

            pt1 = pt2;
            pt2.setX(dRealX+dDepth);
            pt2.setY(dRealY+i*(dAvgH+dA)+dAvgH+dA);
            painter.drawLine(pt1, pt2);

            pt1 = pt2;
            pt2.setX(dRealX+dRealW);
            pt2.setY(dRealY+i*(dAvgH+dA)+dAvgH+dA);
            painter.drawLine(pt1, pt2);

            if (i == (nN-1))
            {
                pt1 = pt2;
                pt2.setX(dRealX+dRealW);
                pt2.setY(dRealY+dRealH);
                painter.drawLine(pt1, pt2);
            }
        }
    }

    return true;
}

bool DissimilarLibraryOperator::draw2QPainterID012(QPainter& painter, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dB = 0.0f;
    double dC = 0.0f;
    int nN = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("b") != paramsMap.end())
    {
        var = paramsMap["b"];
        dB = var.toDouble(&ok);
        if (!ok)
        {
            dB = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("c") != paramsMap.end())
    {
        var = paramsMap["c"];
        dC = var.toDouble(&ok);
        if (!ok)
        {
            dC = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("n") != paramsMap.end())
    {
        var = paramsMap["n"];
        nN = var.toInt(&ok);
        if (!ok)
        {
            nN = 0;
        }
    }
    else
    {
        return false;
    }

    QRect rc = painter.viewport();
    double dDrawerW = rc.width();
    double dDrawerH = rc.height();

    double dBoardWHScale = dBoardW/dBoardH;
    double dDrawerWHScale = dDrawerW/dDrawerH;

    double dRealW = 0.0f;
    double dRealH = 0.0f;

    double dRealX = 0.0f;
    double dRealY = 0.0f;

    // 以画布的高为基准
    if (dBoardWHScale < dDrawerWHScale)
    {
        dRealH = dDrawerH;
        dRealW = dRealH*dBoardWHScale;

        dRealX = (dDrawerW-dRealW)/2.f;
        dRealY = 0.0f;
    }
    // 以画布的宽为基准
    else
    {
        dRealW = dDrawerW;
        dRealH = dRealW/dBoardWHScale;

        dRealX = 0.0f;
        dRealY = (dDrawerH-dRealH)/2.0f;
    }

    // 往中心收缩4个像素，空出边缘绘图
    dRealX += 4.0;
    dRealY += 4.0;
    dRealW -= 8.0;
    dRealH -= 8.0;

    double dYScale = dRealH/dBoardH;
    //double dXScale = dRealW/dBoardW;
    dA = dA * dYScale;
    dB = dB * dYScale;
    dC = dC * dYScale;

    QPen pen(QColor(255, 255, 255), 2.0f);
    painter.setPen(pen);

    QPointF pt1(dRealX+dRealW, dRealY);
    QPointF pt2(dRealX, dRealY);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX);
    pt2.setY(dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    if (nN <= 0)
    {
        pt1.setX(dRealX+dRealW);
        pt1.setY(dRealY+dRealH);
        pt2.setX(dRealX+dRealW);
        pt2.setY(dRealY);
        painter.drawLine(pt1, pt2);

        return true;
    }
    else if (nN == 1)
    {
        double dDepth = dRealW/2.0f;

        // 先画b参数方向的竖线
        pt1.setX(dRealX+dRealW);
        pt1.setY(dRealY);
        pt2.setX(dRealX+dRealW);
        pt2.setY(dRealY+dB);
        painter.drawLine(pt1, pt2);

        pt1.setX(dRealX+dRealW);
        pt1.setY(dRealY+dB);
        pt2.setX(dRealX+dDepth);
        pt2.setY(dRealY+dB);
        painter.drawLine(pt1, pt2);

        pt1 = pt2;
        pt2.setX(dRealX+dDepth);
        pt2.setY(dRealY+dB+dC);
        painter.drawLine(pt1, pt2);

        pt1 = pt2;
        pt2.setX(dRealX+dRealW);
        pt2.setY(dRealY+dB+dC);
        painter.drawLine(pt1, pt2);

        // 画a参数方向的竖线
        pt1.setX(dRealX+dRealW);
        pt1.setY(dRealY+dB+dC);
        pt2.setX(dRealX+dRealW);
        pt2.setY(dRealY+dRealH);
        painter.drawLine(pt1, pt2);

        return true;
    }
    else
    {
        // 先画b参数方向的竖线
        pt1.setX(dRealX+dRealW);
        pt1.setY(dRealY);
        pt2.setX(dRealX+dRealW);
        pt2.setY(dRealY+dB);
        painter.drawLine(pt1, pt2);


        double dDepth = dRealW/2.0f;
        double dAvgH = (dRealH-dC*nN-dB-dA)/(nN-1);
        for (int i = 0; i < nN; ++i)
        {
            pt1.setX(dRealX+dRealW);
            pt1.setY(dRealY+dB+i*(dAvgH+dC));
            pt2.setX(dRealX+dDepth);
            pt2.setY(dRealY+dB+i*(dAvgH+dC));
            painter.drawLine(pt1, pt2);

            pt1 = pt2;
            pt2.setX(dRealX+dDepth);
            pt2.setY(dRealY+dB+i*(dAvgH+dC)+dC);
            painter.drawLine(pt1, pt2);

            pt1 = pt2;
            pt2.setX(dRealX+dRealW);
            pt2.setY(dRealY+dB+i*(dAvgH+dC)+dC);
            painter.drawLine(pt1, pt2);

            if (i != (nN-1))
            {
                pt1 = pt2;
                pt2.setX(dRealX+dRealW);
                pt2.setY(dRealY+dB+i*(dAvgH+dC)+dC+dAvgH);
                painter.drawLine(pt1, pt2);
            }
        }

        // 画a参数方向的竖线
        pt1.setX(dRealX+dRealW);
        pt1.setY(dRealY+dRealH-dA);
        pt2.setX(dRealX+dRealW);
        pt2.setY(dRealY+dRealH);
        painter.drawLine(pt1, pt2);
    }

    return true;
}

bool DissimilarLibraryOperator::draw2QPainterID013(QPainter& painter, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dB = 0.0f;
    double dW = 0.0f;
    double dH = 0.0f;
    double dDepth = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("b") != paramsMap.end())
    {
        var = paramsMap["b"];
        dB = var.toDouble(&ok);
        if (!ok)
        {
            dB = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("W") != paramsMap.end())
    {
        var = paramsMap["W"];
        dW = var.toDouble(&ok);
        if (!ok)
        {
            dW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("H") != paramsMap.end())
    {
        var = paramsMap["H"];
        dH = var.toDouble(&ok);
        if (!ok)
        {
            dH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Depth") != paramsMap.end())
    {
        var = paramsMap["Depth"];
        dDepth = var.toDouble(&ok);
        if (!ok)
        {
            dDepth = 0.0f;
        }
    }
    else
    {
        return false;
    }

    QRect rc = painter.viewport();
    double dDrawerW = rc.width();
    double dDrawerH = rc.height();

    double dBoardWHScale = dBoardW/dBoardH;
    double dDrawerWHScale = dDrawerW/dDrawerH;

    double dRealW = 0.0f;
    double dRealH = 0.0f;

    double dRealX = 0.0f;
    double dRealY = 0.0f;

    // 以画布的高为基准
    if (dBoardWHScale < dDrawerWHScale)
    {
        dRealH = dDrawerH;
        dRealW = dRealH*dBoardWHScale;

        dRealX = (dDrawerW-dRealW)/2.f;
        dRealY = 0.0f;
    }
    // 以画布的宽为基准
    else
    {
        dRealW = dDrawerW;
        dRealH = dRealW/dBoardWHScale;

        dRealX = 0.0f;
        dRealY = (dDrawerH-dRealH)/2.0f;
    }

    // 往中心收缩4个像素，空出边缘绘图
    dRealX += 4.0;
    dRealY += 4.0;
    dRealW -= 8.0;
    dRealH -= 8.0;

    double dYScale = dRealH/dBoardH;
    double dXScale = dRealW/dBoardW;
    dA = dA * dXScale;
    dB = dB * dYScale;
    dW = dW * dXScale;
    dH = dH * dYScale;

    QPen pen(QColor(255, 255, 255), 2.0f);
    painter.setPen(pen);

    QPointF pt1(dRealX, dRealY);
    QPointF pt2(dRealX, dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    pt1.setX(dRealX+dA);
    pt1.setY(dRealY+dRealH-dB-dH);
    pt2.setX(dRealX+dA+dW);
    pt2.setY(dRealY+dRealH-dB);

    QRectF rcf(pt1, pt2);
    painter.drawRect(rcf);
    painter.fillRect(rcf, QBrush(QColor(255, 24, 24)));

    return true;
}

bool DissimilarLibraryOperator::draw2QPainterID014(QPainter& painter, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dB = 0.0f;
    double dC = 0.0f;
    double dD = 0.0f;
    double dE = 0.0f;
    double dF = 0.0f;
    double dG = 0.0f;
    double dH = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("b") != paramsMap.end())
    {
        var = paramsMap["b"];
        dB = var.toDouble(&ok);
        if (!ok)
        {
            dB = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("c") != paramsMap.end())
    {
        var = paramsMap["c"];
        dC = var.toDouble(&ok);
        if (!ok)
        {
            dC = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("d") != paramsMap.end())
    {
        var = paramsMap["d"];
        dD = var.toDouble(&ok);
        if (!ok)
        {
            dD = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("e") != paramsMap.end())
    {
        var = paramsMap["e"];
        dE = var.toDouble(&ok);
        if (!ok)
        {
            dE = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("f") != paramsMap.end())
    {
        var = paramsMap["f"];
        dF = var.toDouble(&ok);
        if (!ok)
        {
            dF = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("g") != paramsMap.end())
    {
        var = paramsMap["g"];
        dG = var.toDouble(&ok);
        if (!ok)
        {
            dG = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("h") != paramsMap.end())
    {
        var = paramsMap["h"];
        dH = var.toDouble(&ok);
        if (!ok)
        {
            dH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    QRect rc = painter.viewport();
    double dDrawerW = rc.width();
    double dDrawerH = rc.height();

    double dBoardWHScale = dBoardW/dBoardH;
    double dDrawerWHScale = dDrawerW/dDrawerH;

    double dRealW = 0.0f;
    double dRealH = 0.0f;

    double dRealX = 0.0f;
    double dRealY = 0.0f;

    // 以画布的高为基准
    if (dBoardWHScale < dDrawerWHScale)
    {
        dRealH = dDrawerH;
        dRealW = dRealH*dBoardWHScale;

        dRealX = (dDrawerW-dRealW)/2.f;
        dRealY = 0.0f;
    }
    // 以画布的宽为基准
    else
    {
        dRealW = dDrawerW;
        dRealH = dRealW/dBoardWHScale;

        dRealX = 0.0f;
        dRealY = (dDrawerH-dRealH)/2.0f;
    }

    // 往中心收缩4个像素，空出边缘绘图
    dRealX += 4.0;
    dRealY += 4.0;
    dRealW -= 8.0;
    dRealH -= 8.0;

    double dYScale = dRealH/dBoardH;
    double dXScale = dRealW/dBoardW;
    dA = dA * dYScale;
    dB = dB * dXScale;

    dC = dC * dYScale;
    dD = dD * dXScale;

    dE = dE * dYScale;
    dF = dF * dXScale;

    dH = dH * dXScale;
    dG = dG * dYScale;

    QPen pen(QColor(255, 255, 255), 2.0f);
    painter.setPen(pen);

    QPointF pt1(dRealX+dB, dRealY);
    QPointF pt2(dRealX+dB, dRealY+dA);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX);
    pt2.setY(dRealY+dA);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX);
    pt2.setY(dRealY+dA+dC);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dD);
    pt2.setY(dRealY+dA+dC);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dD);
    pt2.setY(dRealY+dA+dC+dE);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX);
    pt2.setY(dRealY+dA+dC+dE);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX);
    pt2.setY(dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY+dRealH);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW-dF);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW-dF);
    pt2.setY(dRealY+dG);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW-dF-dH);
    pt2.setY(dRealY+dG);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dRealW-dF-dH);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);

    pt1 = pt2;
    pt2.setX(dRealX+dB);
    pt2.setY(dRealY);
    painter.drawLine(pt1, pt2);


    return true;
}

bool DissimilarLibraryOperator::drawRPolyline(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, int nID, std::map<QString, QVariant>& paramsMap)
{
    if (!pDocument || !op)
    {
        return false;
    }

    if (nID == 0)
    {
        return drawRPolylineID000(pDocument, op, offset, dDepth, bRotate, paramsMap);
    }
    else if (nID == 1)
    {
        return drawRPolylineID001(pDocument, op, offset, dDepth, bRotate, paramsMap);
    }
    else if (nID == 2)
    {
        return drawRPolylineID002(pDocument, op, offset, dDepth, bRotate, paramsMap);
    }
    else if (nID == 3)
    {
        return drawRPolylineID003(pDocument, op, offset, dDepth, bRotate, paramsMap);
    }
    else if (nID == 4)
    {
        return drawRPolylineID004(pDocument, op, offset, dDepth, bRotate, paramsMap);
    }
    else if (nID == 5)
    {
        return drawRPolylineID005(pDocument, op, offset, dDepth, bRotate, paramsMap);
    }
    else if (nID == 6)
    {
        return drawRPolylineID006(pDocument, op, offset, dDepth, bRotate, paramsMap);
    }
    else if (nID == 7)
    {
        return drawRPolylineID007(pDocument, op, offset, dDepth, bRotate, paramsMap);
    }
    else if (nID == 8)
    {
        return drawRPolylineID008(pDocument, op, offset, dDepth, bRotate, paramsMap);
    }
    else if (nID == 9)
    {
        return drawRPolylineID009(pDocument, op, offset, dDepth, bRotate, paramsMap);
    }
    else if (nID == 10)
    {
        return drawRPolylineID010(pDocument, op, offset, dDepth, bRotate, paramsMap);
    }
    else if (nID == 11)
    {
        return drawRPolylineID011(pDocument, op, offset, dDepth, bRotate, paramsMap);
    }
    else if (nID == 12)
    {
        return drawRPolylineID012(pDocument, op, offset, dDepth, bRotate, paramsMap);
    }
    else if (nID == 13)
    {
        return drawRPolylineID013(pDocument, op, offset, dDepth, bRotate, paramsMap);
    }
    else if (nID == 14)
    {
        return drawRPolylineID014(pDocument, op, offset, dDepth, bRotate, paramsMap);
    }
    else
    {
        return false;
    }

    return true;
}

bool DissimilarLibraryOperator::drawRPolylineID000(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dB = 0.0f;
    double dC = 0.0f;
    double dD = 0.0f;
    double dE = 0.0f;
    double dF = 0.0f;
    double dG = 0.0f;
    double dH = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("b") != paramsMap.end())
    {
        var = paramsMap["b"];
        dB = var.toDouble(&ok);
        if (!ok)
        {
            dB = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("c") != paramsMap.end())
    {
        var = paramsMap["c"];
        dC = var.toDouble(&ok);
        if (!ok)
        {
            dC = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("d") != paramsMap.end())
    {
        var = paramsMap["d"];
        dD = var.toDouble(&ok);
        if (!ok)
        {
            dD = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("e") != paramsMap.end())
    {
        var = paramsMap["e"];
        dE = var.toDouble(&ok);
        if (!ok)
        {
            dE = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("f") != paramsMap.end())
    {
        var = paramsMap["f"];
        dF = var.toDouble(&ok);
        if (!ok)
        {
            dF = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("g") != paramsMap.end())
    {
        var = paramsMap["g"];
        dG = var.toDouble(&ok);
        if (!ok)
        {
            dG = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("h") != paramsMap.end())
    {
        var = paramsMap["h"];
        dH = var.toDouble(&ok);
        if (!ok)
        {
            dH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    RPolyline polyline;

    if ((dA <= EPSINON && dA >= -EPSINON) || (dB <= EPSINON && dB >= -EPSINON))
    {
        dA = 0.0f;
        dB = 0.0f;
        polyline.appendVertex(RVector(dA, -dB, dDepth));
    }
    else
    {
        polyline.appendVertex(RVector(dA, -0.0, dDepth));
        polyline.appendVertex(RVector(dA, -dB, dDepth));
        polyline.appendVertex(RVector(0.0, -dB, dDepth));
    }

    if ((dG <= EPSINON && dG >= -EPSINON) || (dH <= EPSINON && dH >= -EPSINON))
    {
        dG = 0.0f;
        dH = 0.0f;
        polyline.appendVertex(RVector(dH, -(dBoardH-dG), dDepth));
    }
    else
    {
        polyline.appendVertex(RVector(0.0, -(dBoardH-dG), dDepth));
        polyline.appendVertex(RVector(dH, -(dBoardH-dG), dDepth));
        polyline.appendVertex(RVector(dH, -dBoardH, dDepth));
    }

    if ((dE <= EPSINON && dE >= -EPSINON) || (dF <= EPSINON && dF >= -EPSINON))
    {
        dE = 0.0f;
        dF = 0.0f;
        polyline.appendVertex(RVector(dBoardW-dE, -(dBoardH-dF), dDepth));
    }
    else
    {
        polyline.appendVertex(RVector(dBoardW-dE, -dBoardH, dDepth));
        polyline.appendVertex(RVector(dBoardW-dE, -(dBoardH-dF), dDepth));
        polyline.appendVertex(RVector(dBoardW, -(dBoardH-dF), dDepth));
    }

    if ((dC <= EPSINON && dC >= -EPSINON) || (dD <= EPSINON && dD >= -EPSINON))
    {
        dC = 0.0f;
        dD = 0.0f;
        polyline.appendVertex(RVector(dBoardW-dD, -dC, dDepth));
    }
    else
    {
        polyline.appendVertex(RVector(dBoardW, -dC, dDepth));
        polyline.appendVertex(RVector(dBoardW-dD, -dC, dDepth));
        polyline.appendVertex(RVector(dBoardW-dD, 0.0, dDepth));
    }

    polyline.setClosed(true);

    if (!bRotate)
    {
        RVector pt1(dBoardW/2.0, -dBoardH/2.0);
        RVector pt2(dBoardH/2.0, -dBoardW/2.0);
        RVector ptOffset = pt2-pt1;
        polyline.move(ptOffset);
        polyline.rotate(-90.0*M_PI/180.0, pt2);
    }

    polyline.move(offset);

    //polyline.rotate(-90.0*M_PI/180.0, polyline.getBoundingBox().getCenter());

    QSharedPointer<RPolylineEntity> polylineEntity = QSharedPointer<RPolylineEntity>(
                new RPolylineEntity(pDocument, RPolylineData(polyline)));
    polylineEntity->setColor(RColor(0, 128, 255));
    //polylineEntity->setLinetypeId(pDocument->getLinetypeId("dashed"));
    polylineEntity->setLineweight(RLineweight::Weight050);

    op->addObject(polylineEntity, false);

    return true;
}

bool DissimilarLibraryOperator::drawRPolylineID001(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dR1 = 0.0f;
    double dR2 = 0.0f;
    double dR3 = 0.0f;
    double dR4 = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r1") != paramsMap.end())
    {
        var= paramsMap["r1"];
        dR1 = var.toDouble(&ok);
        if (!ok)
        {
            dR1 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r2") != paramsMap.end())
    {
        var = paramsMap["r2"];
        dR2 = var.toDouble(&ok);
        if (!ok)
        {
            dR2 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r3") != paramsMap.end())
    {
        var = paramsMap["r3"];
        dR3 = var.toDouble(&ok);
        if (!ok)
        {
            dR3 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r4") != paramsMap.end())
    {
        var = paramsMap["r4"];
        dR4 = var.toDouble(&ok);
        if (!ok)
        {
            dR4 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    RPolyline polyline;

    if (!(dR1 <= EPSINON && dR1 >= -EPSINON))
    {
        RArc arc1(RVector(dR1, -dR1, dDepth), dR1, 90.0*M_PI/180.0, 180.0*M_PI/180.0);
        polyline.appendShape(arc1);
    }

    RLine line1(RVector(0.0, -dR1, dDepth), RVector(0.0, -(dBoardH-dR4), dDepth));
    polyline.appendShape(line1);

    if (!(dR4 <= EPSINON && dR4 >= -EPSINON))
    {
        RArc arc4(RVector(dR4, -(dBoardH-dR4), dDepth), dR4, 180.0*M_PI/180.0, 270.0*M_PI/180.0);
        polyline.appendShape(arc4);
    }

    RLine line4(RVector(dR4, -dBoardH, dDepth), RVector(dBoardW-dR3, -dBoardH, dDepth));
    polyline.appendShape(line4);

    if (!(dR3 <= EPSINON && dR3 >= -EPSINON))
    {
        RArc arc3(RVector(dBoardW-dR3, -(dBoardH-dR3), dDepth), dR3, 270.0*M_PI/180.0, 0.0*M_PI/180.0);
        polyline.appendShape(arc3);
    }

    RLine line3(RVector(dBoardW, -(dBoardH-dR3), dDepth), RVector(dBoardW, -dR2, dDepth));
    polyline.appendShape(line3);

    if (!(dR2 <= EPSINON && dR2 >= -EPSINON))
    {
        RArc arc2(RVector(dBoardW-dR2, -dR2, dDepth), dR2, 0.0*M_PI/180.0, 90.0*M_PI/180.0);
        polyline.appendShape(arc2);
    }

    RLine line2(RVector(dBoardW-dR2, 0.0, dDepth), RVector(dR1, 0.0, dDepth));
    polyline.appendShape(line2);

    polyline.setClosed(true);

    if (!bRotate)
    {
        RVector pt1(dBoardW/2.0, -dBoardH/2.0);
        RVector pt2(dBoardH/2.0, -dBoardW/2.0);
        RVector ptOffset = pt2-pt1;
        polyline.move(ptOffset);
        polyline.rotate(-90.0*M_PI/180.0, pt2);
    }

    polyline.move(offset);

    //polyline.rotate(90.0, polyline.getBoundingBox().getCenter());
    QSharedPointer<RPolylineEntity> polylineEntity = QSharedPointer<RPolylineEntity>(
                new RPolylineEntity(pDocument, RPolylineData(polyline)));
    polylineEntity->setColor(RColor(0, 128, 255));
    //polylineEntity->setLinetypeId(pDocument->getLinetypeId("dashed"));
    polylineEntity->setLineweight(RLineweight::Weight050);

    op->addObject(polylineEntity, false);

    return true;
}

bool DissimilarLibraryOperator::drawRPolylineID002(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dB = 0.0f;
    double dC = 0.0f;
    double dD = 0.0f;
    double dE = 0.0f;
    double dF = 0.0f;
    double dG = 0.0f;
    double dH = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("b") != paramsMap.end())
    {
        var = paramsMap["b"];
        dB = var.toDouble(&ok);
        if (!ok)
        {
            dB = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("c") != paramsMap.end())
    {
        var = paramsMap["c"];
        dC = var.toDouble(&ok);
        if (!ok)
        {
            dC = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("d") != paramsMap.end())
    {
        var = paramsMap["d"];
        dD = var.toDouble(&ok);
        if (!ok)
        {
            dD = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("e") != paramsMap.end())
    {
        var = paramsMap["e"];
        dE = var.toDouble(&ok);
        if (!ok)
        {
            dE = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("f") != paramsMap.end())
    {
        var = paramsMap["f"];
        dF = var.toDouble(&ok);
        if (!ok)
        {
            dF = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("g") != paramsMap.end())
    {
        var = paramsMap["g"];
        dG = var.toDouble(&ok);
        if (!ok)
        {
            dG = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("h") != paramsMap.end())
    {
        var = paramsMap["h"];
        dH = var.toDouble(&ok);
        if (!ok)
        {
            dH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    RPolyline polyline;

    if ((dA <= EPSINON && dA >= -EPSINON) || (dB <= EPSINON && dB >= -EPSINON))
    {
        dA = 0.0;
        dB = 0.0;
        polyline.appendVertex(RVector(0.0, 0.0, dDepth));
    }
    else
    {
        polyline.appendVertex(RVector(dB, 0.0, dDepth));
        polyline.appendVertex(RVector(0.0, -dA, dDepth));
    }

    if ((dG <= EPSINON && dG >= -EPSINON) || (dH <= EPSINON && dH >= -EPSINON))
    {
        dG = 0.0;
        dH = 0.0;
        polyline.appendVertex(RVector(0.0, -dBoardH, dDepth));
    }
    else
    {
        polyline.appendVertex(RVector(0.0, -(dBoardH-dH), dDepth));
        polyline.appendVertex(RVector(dG, -dBoardH, dDepth));
    }

    if ((dE <= EPSINON && dE >= -EPSINON) || (dF <= EPSINON && dF >= -EPSINON))
    {
        dE = 0.0;
        dF = 0.0;
        polyline.appendVertex(RVector(dBoardW, -dBoardH, dDepth));
    }
    else
    {
        polyline.appendVertex(RVector(dBoardW-dF, -dBoardH, dDepth));
        polyline.appendVertex(RVector(dBoardW, -(dBoardH-dE), dDepth));
    }

    if ((dC <= EPSINON && dC >= -EPSINON) || (dD <= EPSINON && dD >= -EPSINON))
    {
        dC = 0.0;
        dD = 0.0;
        polyline.appendVertex(RVector(dBoardW, 0.0, dDepth));
    }
    else
    {
        polyline.appendVertex(RVector(dBoardW, -dD, dDepth));
        polyline.appendVertex(RVector(dBoardW-dC, 0.0, dDepth));
    }

    polyline.setClosed(true);

    if (!bRotate)
    {
        RVector pt1(dBoardW/2.0, -dBoardH/2.0);
        RVector pt2(dBoardH/2.0, -dBoardW/2.0);
        RVector ptOffset = pt2-pt1;
        polyline.move(ptOffset);
        polyline.rotate(-90.0*M_PI/180.0, pt2);
    }

    polyline.move(offset);

    QSharedPointer<RPolylineEntity> polylineEntity = QSharedPointer<RPolylineEntity>(
                new RPolylineEntity(pDocument, RPolylineData(polyline)));
    polylineEntity->setColor(RColor(0, 128, 255));
    //polylineEntity->setLinetypeId(pDocument->getLinetypeId("dashed"));
    polylineEntity->setLineweight(RLineweight::Weight050);

    op->addObject(polylineEntity, false);

    return true;
}

bool DissimilarLibraryOperator::drawRPolylineID003(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dB = 0.0f;
    double dC = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("b") != paramsMap.end())
    {
        var = paramsMap["b"];
        dB = var.toDouble(&ok);
        if (!ok)
        {
            dB = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("c") != paramsMap.end())
    {
        var = paramsMap["c"];
        dC = var.toDouble(&ok);
        if (!ok)
        {
            dC = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (dA <= EPSINON && dA >= -EPSINON)
    {
        dA = 1.0;
    }

    if (dB <= EPSINON && dB >= -EPSINON)
    {
        dB = 1.0;
    }

    if (dC<= EPSINON && dC >= -EPSINON)
    {
        dC = 1.0;
    }

    RPolyline polyline;

    polyline.appendVertex(RVector(0.0, 0.0, dDepth));
    polyline.appendVertex(RVector(0.0, -dBoardH, dDepth));
    polyline.appendVertex(RVector(dBoardW, -dBoardH, dDepth));
    polyline.appendVertex(RVector(dBoardW, 0.0, dDepth));
    polyline.appendVertex(RVector(dA+dC, 0.0, dDepth));
    polyline.appendVertex(RVector(dA+dC, -dB, dDepth));
    polyline.appendVertex(RVector(dA, -dB, dDepth));
    polyline.appendVertex(RVector(dA, 0.0, dDepth));

    polyline.setClosed(true);

    if (!bRotate)
    {
        RVector pt1(dBoardW/2.0, -dBoardH/2.0);
        RVector pt2(dBoardH/2.0, -dBoardW/2.0);
        RVector ptOffset = pt2-pt1;
        polyline.move(ptOffset);
        polyline.rotate(-90.0*M_PI/180.0, pt2);
    }

    polyline.move(offset);

    QSharedPointer<RPolylineEntity> polylineEntity = QSharedPointer<RPolylineEntity>(
                new RPolylineEntity(pDocument, RPolylineData(polyline)));
    polylineEntity->setColor(RColor(0, 128, 255));
    //polylineEntity->setLinetypeId(pDocument->getLinetypeId("dashed"));
    polylineEntity->setLineweight(RLineweight::Weight050);

    op->addObject(polylineEntity, false);

    return true;
}

bool DissimilarLibraryOperator::drawRPolylineID004(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dB = 0.0f;
    double dC = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }

    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("b") != paramsMap.end())
    {
        var = paramsMap["b"];
        dB = var.toDouble(&ok);
        if (!ok)
        {
            dB = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("c") != paramsMap.end())
    {
        var = paramsMap["c"];
        dC = var.toDouble(&ok);
        if (!ok)
        {
            dC = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (dA <= EPSINON && dA >= -EPSINON)
    {
        dA = 1.0;
    }

    if (dB <= EPSINON && dB >= -EPSINON)
    {
        dB = 1.0;
    }

    if (dC <= EPSINON && dC >= -EPSINON)
    {
        dC = 1.0;
    }

    RPolyline polyline;

    polyline.appendVertex(RVector(0.0, 0.0, dDepth));
    polyline.appendVertex(RVector(0.0, -dA, dDepth));
    polyline.appendVertex(RVector(dB, -dA, dDepth));
    polyline.appendVertex(RVector(dB, -(dA+dC), dDepth));
    polyline.appendVertex(RVector(0.0, -(dA+dC), dDepth));
    polyline.appendVertex(RVector(0.0, -dBoardH, dDepth));
    polyline.appendVertex(RVector(dBoardW, -dBoardH, dDepth));
    polyline.appendVertex(RVector(dBoardW, 0.0, dDepth));

    polyline.setClosed(true);

    if (!bRotate)
    {
        RVector pt1(dBoardW/2.0, -dBoardH/2.0);
        RVector pt2(dBoardH/2.0, -dBoardW/2.0);
        RVector ptOffset = pt2-pt1;
        polyline.move(ptOffset);
        polyline.rotate(-90.0*M_PI/180.0, pt2);
    }

    polyline.move(offset);

    QSharedPointer<RPolylineEntity> polylineEntity = QSharedPointer<RPolylineEntity>(
                new RPolylineEntity(pDocument, RPolylineData(polyline)));
    polylineEntity->setColor(RColor(0, 128, 255));
    //polylineEntity->setLinetypeId(pDocument->getLinetypeId("dashed"));
    polylineEntity->setLineweight(RLineweight::Weight050);

    op->addObject(polylineEntity, false);

    return true;
}

bool DissimilarLibraryOperator::drawRPolylineID005(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dB = 0.0f;
    double dR = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("b") != paramsMap.end())
    {
        var = paramsMap["b"];
        dB = var.toDouble(&ok);
        if (!ok)
        {
            dB = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r") != paramsMap.end())
    {
        var = paramsMap["r"];
        dR = var.toDouble(&ok);
        if (!ok)
        {
            dR = 0.0f;
        }
    }
    else
    {
        return false;
    }

    RPolyline polyline;

    polyline.appendVertex(RVector(0.0, 0.0, dDepth));
    polyline.appendVertex(RVector(0.0, -dBoardH, dDepth));
    polyline.appendVertex(RVector(dBoardW, -dBoardH, dDepth));
    polyline.appendVertex(RVector(dBoardW, 0.0, dDepth));

    polyline.setClosed(true);

    if (!bRotate)
    {
        RVector pt1(dBoardW/2.0, -dBoardH/2.0);
        RVector pt2(dBoardH/2.0, -dBoardW/2.0);
        RVector ptOffset = pt2-pt1;
        polyline.move(ptOffset);
        polyline.rotate(-90.0*M_PI/180.0, pt2);
    }

    polyline.move(offset);

    QSharedPointer<RPolylineEntity> polylineEntity = QSharedPointer<RPolylineEntity>(
                new RPolylineEntity(pDocument, RPolylineData(polyline)));
    polylineEntity->setColor(RColor(0, 128, 255));
    //polylineEntity->setLinetypeId(pDocument->getLinetypeId("dashed"));
    polylineEntity->setLineweight(RLineweight::Weight050);

    op->addObject(polylineEntity, false);

    if (!(dR <= EPSINON && dR >= -EPSINON))
    {
        RVector PtCenter(dA, -(dBoardH-dB), dDepth);
        RCircle circle(PtCenter, dR);

        if (!bRotate)
        {
            RVector pt1(dBoardW/2.0, -dBoardH/2.0);
            RVector pt2(dBoardH/2.0, -dBoardW/2.0);
            RVector ptOffset = pt2-pt1;
            circle.move(ptOffset);
            circle.rotate(-90.0*M_PI/180.0, pt2);
        }

        circle.move(offset);

        QSharedPointer<RCircleEntity> circleEntity = QSharedPointer<RCircleEntity>(
                    new RCircleEntity(pDocument, RCircleData(circle)));
        circleEntity->setColor(RColor(0, 128, 255));
        //circleEntity->setLinetypeId(pDocument->getLinetypeId("dashed"));
        circleEntity->setLineweight(RLineweight::Weight050);

        op->addObject(circleEntity, false);
    }

    return true;
}

bool DissimilarLibraryOperator::drawRPolylineID006(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dB = 0.0f;
    double dW = 0.0f;
    double dH = 0.0f;

    bool ok = false;
    QVariant var;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("b") != paramsMap.end())
    {
        var = paramsMap["b"];
        dB = var.toDouble(&ok);
        if (!ok)
        {
            dB = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("W") != paramsMap.end())
    {
        var = paramsMap["W"];
        dW = var.toDouble(&ok);
        if (!ok)
        {
            dW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("H") != paramsMap.end())
    {
        var = paramsMap["H"];
        dH = var.toDouble(&ok);
        if (!ok)
        {
            dH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    RPolyline polyline;

    polyline.appendVertex(RVector(0.0, 0.0, dDepth));
    polyline.appendVertex(RVector(0.0, -dBoardH, dDepth));
    polyline.appendVertex(RVector(dBoardW, -dBoardH, dDepth));
    polyline.appendVertex(RVector(dBoardW, 0.0, dDepth));

    polyline.setClosed(true);

    RVector pt1(dBoardW/2.0, -dBoardH/2.0);
    RVector pt2(dBoardH/2.0, -dBoardW/2.0);
    RVector ptOffset = pt2-pt1;
    if (!bRotate)
    {
        polyline.move(ptOffset);
        polyline.rotate(-90.0*M_PI/180.0, pt2);
    }

    polyline.move(offset);

    QSharedPointer<RPolylineEntity> polylineEntity = QSharedPointer<RPolylineEntity>(
                new RPolylineEntity(pDocument, RPolylineData(polyline)));
    polylineEntity->setColor(RColor(0, 128, 255));
    //polylineEntity->setLinetypeId(pDocument->getLinetypeId("dashed"));
    polylineEntity->setLineweight(RLineweight::Weight050);

    op->addObject(polylineEntity, false);


    RPolyline innerPolyline;

    innerPolyline.appendVertex(RVector(dA, -(dBoardH-dH-dB), dDepth));
    innerPolyline.appendVertex(RVector(dA, -(dBoardH-dB), dDepth));
    innerPolyline.appendVertex(RVector(dA+dW, -(dBoardH-dB), dDepth));
    innerPolyline.appendVertex(RVector(dA+dW, -(dBoardH-dH-dB), dDepth));

    innerPolyline.setClosed(true);

    if (!bRotate)
    {
        innerPolyline.move(ptOffset);
        innerPolyline.rotate(-90.0*M_PI/180.0, pt2);
    }

    innerPolyline.move(offset);

    QSharedPointer<RPolylineEntity> innerPolylineEntity = QSharedPointer<RPolylineEntity>(
                new RPolylineEntity(pDocument, RPolylineData(innerPolyline)));
    innerPolylineEntity->setColor(RColor(0, 128, 255));
    //innerPolylineEntity->setLinetypeId(pDocument->getLinetypeId("dashed"));
    innerPolylineEntity->setLineweight(RLineweight::Weight050);

    op->addObject(innerPolylineEntity, false);


    return true;
}

bool DissimilarLibraryOperator::drawRPolylineID007(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dW = 0.0f;
    double dSlotDepth = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("W") != paramsMap.end())
    {
        var = paramsMap["W"];
        dW = var.toDouble(&ok);
        if (!ok)
        {
            dW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Depth") != paramsMap.end())
    {
        var = paramsMap["Depth"];
        dSlotDepth = var.toDouble(&ok);
        if (!ok)
        {
            dSlotDepth = 0.0f;
        }
    }
    else
    {
        return false;
    }

    RPolyline polyline;

    polyline.appendVertex(RVector(0.0, 0.0, dDepth));
    polyline.appendVertex(RVector(0.0, -dBoardH, dDepth));
    polyline.appendVertex(RVector(dBoardW, -dBoardH, dDepth));
    polyline.appendVertex(RVector(dBoardW, 0.0, dDepth));

    polyline.setClosed(true);

    RVector pt1(dBoardW/2.0, -dBoardH/2.0);
    RVector pt2(dBoardH/2.0, -dBoardW/2.0);
    RVector ptOffset = pt2-pt1;

    if (!bRotate)
    {
        polyline.move(ptOffset);
        polyline.rotate(-90.0*M_PI/180.0, pt2);
    }

    polyline.move(offset);

    QSharedPointer<RPolylineEntity> polylineEntity = QSharedPointer<RPolylineEntity>(
                new RPolylineEntity(pDocument, RPolylineData(polyline)));
    polylineEntity->setColor(RColor(0, 128, 255));
    //polylineEntity->setLinetypeId(pDocument->getLinetypeId("dashed"));
    polylineEntity->setLineweight(RLineweight::Weight050);

    op->addObject(polylineEntity, false);

    // 特例，根据刀具直径和使用多刀头生产dxf
    SystemConfigManager& smgr = SystemConfigManager::GetInstance();

    double dKnifeDiameter = smgr.getKinfeDiameter();

    double dSlotKnifeRadius = dKnifeDiameter/2.0;

    RPolyline innerPolyline;

    /*
    innerPolyline.appendVertex(RVector(0.0, -dA, dSlotDepth));
    innerPolyline.appendVertex(RVector(0.0, -(dA+dW), dSlotDepth));
    innerPolyline.appendVertex(RVector(dBoardW, -(dA+dW), dSlotDepth));
    innerPolyline.appendVertex(RVector(dBoardW, -dA, dSlotDepth));
    */

    innerPolyline.appendVertex(RVector(0.0, -(dA+dSlotKnifeRadius), dSlotDepth));
    innerPolyline.appendVertex(RVector(0.0, -(dA+dW-dSlotKnifeRadius), dSlotDepth));
    innerPolyline.appendVertex(RVector(dBoardW, -(dA+dW-dSlotKnifeRadius), dSlotDepth));
    innerPolyline.appendVertex(RVector(dBoardW, -(dA+dSlotKnifeRadius), dSlotDepth));

    innerPolyline.setClosed(true);

    if (!bRotate)
    {
        innerPolyline.move(ptOffset);
        innerPolyline.rotate(-90.0*M_PI/180.0, pt2);
    }

    innerPolyline.move(offset);

    QSharedPointer<RPolylineEntity> innerPolylineEntity = QSharedPointer<RPolylineEntity>(
                new RPolylineEntity(pDocument, RPolylineData(innerPolyline)));
    innerPolylineEntity->setColor(RColor(0, 128, 255));
    //innerPolylineEntity->setLinetypeId(pDocument->getLinetypeId("dashed"));
    innerPolylineEntity->setLineweight(RLineweight::Weight050);

    op->addObject(innerPolylineEntity, false);

    return true;
}

bool DissimilarLibraryOperator::drawRPolylineID008(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dW = 0.0f;
    double dSlotDepth = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("W") != paramsMap.end())
    {
        var = paramsMap["W"];
        dW = var.toDouble(&ok);
        if (!ok)
        {
            dW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Depth") != paramsMap.end())
    {
        var = paramsMap["Depth"];
        dSlotDepth = var.toDouble(&ok);
        if (!ok)
        {
            dSlotDepth = 0.0f;
        }
    }
    else
    {
        return false;
    }

    RPolyline polyline;

    polyline.appendVertex(RVector(0.0, 0.0, dDepth));
    polyline.appendVertex(RVector(0.0, -dBoardH, dDepth));
    polyline.appendVertex(RVector(dBoardW, -dBoardH, dDepth));
    polyline.appendVertex(RVector(dBoardW, 0.0, dDepth));

    polyline.setClosed(true);

    RVector pt1(dBoardW/2.0, -dBoardH/2.0);
    RVector pt2(dBoardH/2.0, -dBoardW/2.0);
    RVector ptOffset = pt2-pt1;

    if (!bRotate)
    {
        polyline.move(ptOffset);
        polyline.rotate(-90.0*M_PI/180.0, pt2);
    }

    polyline.move(offset);

    QSharedPointer<RPolylineEntity> polylineEntity = QSharedPointer<RPolylineEntity>(
                new RPolylineEntity(pDocument, RPolylineData(polyline)));
    polylineEntity->setColor(RColor(0, 128, 255));
    //polylineEntity->setLinetypeId(pDocument->getLinetypeId("dashed"));
    polylineEntity->setLineweight(RLineweight::Weight050);

    op->addObject(polylineEntity, false);


    // 特例，根据刀具直径和使用多刀头生产dxf
    SystemConfigManager& smgr = SystemConfigManager::GetInstance();

    double dKnifeDiameter = smgr.getKinfeDiameter();

    double dSlotKnifeRadius = dKnifeDiameter/2.0;

    RPolyline innerPolyline;

    /*
    innerPolyline.appendVertex(RVector(dBoardW-dA-dW, -0.0, dSlotDepth));
    innerPolyline.appendVertex(RVector(dBoardW-dA-dW, -dBoardH, dSlotDepth));
    innerPolyline.appendVertex(RVector(dBoardW-dA, -dBoardH, dSlotDepth));
    innerPolyline.appendVertex(RVector(dBoardW-dA, -0.0, dSlotDepth));
    */

    innerPolyline.appendVertex(RVector((dBoardW-dA-dW)+dSlotKnifeRadius, -0.0, dSlotDepth));
    innerPolyline.appendVertex(RVector((dBoardW-dA-dW)+dSlotKnifeRadius, -dBoardH, dSlotDepth));
    innerPolyline.appendVertex(RVector((dBoardW-dA)-dSlotKnifeRadius, -dBoardH, dSlotDepth));
    innerPolyline.appendVertex(RVector((dBoardW-dA)-dSlotKnifeRadius, -0.0, dSlotDepth));

    innerPolyline.setClosed(true);

    if (!bRotate)
    {
        innerPolyline.move(ptOffset);
        innerPolyline.rotate(-90.0*M_PI/180.0, pt2);
    }

    innerPolyline.move(offset);

    QSharedPointer<RPolylineEntity> innerPolylineEntity = QSharedPointer<RPolylineEntity>(
                new RPolylineEntity(pDocument, RPolylineData(innerPolyline)));
    innerPolylineEntity->setColor(RColor(0, 128, 255));
    //innerPolylineEntity->setLinetypeId(pDocument->getLinetypeId("dashed"));
    innerPolylineEntity->setLineweight(RLineweight::Weight050);

    op->addObject(innerPolylineEntity, false);

    return true;
}

bool DissimilarLibraryOperator::drawRPolylineID009(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dB = 0.0f;
    double dR1 = 0.0f;
    double dR2 = 0.0f;
    double dR3 = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("b") != paramsMap.end())
    {
        var = paramsMap["b"];
        dB = var.toDouble(&ok);
        if (!ok)
        {
            dB = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r1") != paramsMap.end())
    {
        var = paramsMap["r1"];
        dR1 = var.toDouble(&ok);
        if (!ok)
        {
            dR1 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r2") != paramsMap.end())
    {
        var = paramsMap["r2"];
        dR2 = var.toDouble(&ok);
        if (!ok)
        {
            dR2 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r3") != paramsMap.end())
    {
        var = paramsMap["r3"];
        dR3 = var.toDouble(&ok);
        if (!ok)
        {
            dR3 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    //QList<QSharedPointer<RShape> > segments;

    RPolyline polyline;

    //QSharedPointer<RLine> line1 = QSharedPointer<RLine>(new RLine(0.0, 0.0, 0.0, -dBoardH));
    RLine line1(RVector(0.0, 0.0, dDepth), RVector(0.0, -dBoardH, dDepth));
    polyline.appendShape(line1);
    //segments.append(line1);

    //QSharedPointer<RLine> line2 = QSharedPointer<RLine>(new RLine(0.0, -dBoardH, dBoardW, -dBoardH));
    RLine line2(RVector(0.0, -dBoardH, dDepth), RVector(dBoardW, -dBoardH, dDepth));
    polyline.appendShape(line2);
    //segments.append(line2);

    //QSharedPointer<RLine> line3 = QSharedPointer<RLine>(new RLine(dBoardW, -dBoardH, dBoardW, -(dBoardH-dB+dR3)));
    RLine line3(RVector(dBoardW, -dBoardH, dDepth), RVector(dBoardW, -(dBoardH-dB+dR3), dDepth));
    polyline.appendShape(line3);
    //segments.append(line3);

    if (!(dR3 <= EPSINON && dR3 >= -EPSINON))
    {
        //QSharedPointer<RArc> arc3 = QSharedPointer<RArc>(new RArc(dBoardW-dR3, -(dBoardH-dB+dR3), dR3, 0.0*M_PI/180.0, 90.0*M_PI/180.0));
        RArc arc3(RVector(dBoardW-dR3, -(dBoardH-dB+dR3), dDepth), dR3, 0.0*M_PI/180.0, 90.0*M_PI/180.0);
        polyline.appendShape(arc3);
        //segments.append(arc3);
    }

    //QSharedPointer<RLine> line4 = QSharedPointer<RLine>(new RLine(dBoardW-dR3, -(dBoardH-dB), dA+dR2, -(dBoardH-dB)));
    RLine line4(RVector(dBoardW-dR3, -(dBoardH-dB), dDepth), RVector(dA+dR2, -(dBoardH-dB), dDepth));
    polyline.appendShape(line4);
    //segments.append(line4);


    if (!(dR2 <= EPSINON && dR2 >= -EPSINON))
    {
        //QSharedPointer<RArc> arc2 = QSharedPointer<RArc>(new RArc(dA+dR2, -(dBoardH-dB-dR2), dR2, 270.0*M_PI/180.0, 180.0*M_PI/180.0, true));
        RArc arc2(RVector(dA+dR2, -(dBoardH-dB-dR2), dDepth), dR2, 270.0*M_PI/180.0, 180.0*M_PI/180.0, true);
        polyline.appendShape(arc2);
        //segments.append(arc2);
    }

    //QSharedPointer<RLine> line5 = QSharedPointer<RLine>(new RLine(dA, -(dBoardH-dB-dR2), dA, -dR1));
    RLine line5(RVector(dA, -(dBoardH-dB-dR2), dDepth), RVector(dA, -dR1, dDepth));
    polyline.appendShape(line5);
    //segments.append(line5);

    if (!(dR1 <= EPSINON && dR1 >= -EPSINON))
    {
        //QSharedPointer<RArc> arc1 = QSharedPointer<RArc>(new RArc(dA-dR1, -dR1, dR1, 0.0*M_PI/180.0, 90.0*M_PI/180.0));
        RArc arc1(RVector(dA-dR1, -dR1, dDepth), dR1, 0.0*M_PI/180.0, 90.0*M_PI/180.0);
        polyline.appendShape(arc1);
        //segments.append(arc1);
    }

    //QSharedPointer<RLine> line6 = QSharedPointer<RLine>(new RLine(dA-dR1, 0.0, 0.0, 0.0));
    RLine line6(RVector(dA-dR1, 0.0, dDepth), RVector(0.0, 0.0, dDepth));
    polyline.appendShape(line6);
    //segments.append(line6);

    polyline.setClosed(true);

    //RPolyline polyline(segments);

    if (!bRotate)
    {
        RVector pt1(dBoardW/2.0, -dBoardH/2.0);
        RVector pt2(dBoardH/2.0, -dBoardW/2.0);
        RVector ptOffset = pt2-pt1;
        polyline.move(ptOffset);
        polyline.rotate(-90.0*M_PI/180.0, pt2);
    }

    polyline.move(offset);

    QSharedPointer<RPolylineEntity> polylineEntity = QSharedPointer<RPolylineEntity>(
                new RPolylineEntity(pDocument, RPolylineData(polyline)));
    polylineEntity->setColor(RColor(0, 128, 255));
    //polylineEntity->setLinetypeId(pDocument->getLinetypeId("dashed"));
    polylineEntity->setLineweight(RLineweight::Weight050);

    op->addObject(polylineEntity, false);

    return true;
}

bool DissimilarLibraryOperator::drawRPolylineID010(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dB = 0.0f;
    double dR1 = 0.0f;
    double dR2 = 0.0f;
    double dR3 = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("b") != paramsMap.end())
    {
        var = paramsMap["b"];
        dB = var.toDouble(&ok);
        if (!ok)
        {
            dB = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r1") != paramsMap.end())
    {
        var = paramsMap["r1"];
        dR1 = var.toDouble(&ok);
        if (!ok)
        {
            dR1 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r2") != paramsMap.end())
    {
        var = paramsMap["r2"];
        dR2 = var.toDouble(&ok);
        if (!ok)
        {
            dR2 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r3") != paramsMap.end())
    {
        var = paramsMap["r3"];
        dR3 = var.toDouble(&ok);
        if (!ok)
        {
            dR3 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    RPolyline polyline;

    RLine line1(RVector(0.0, 0.0, dDepth), RVector(0.0, -dBoardH, dDepth));
    polyline.appendShape(line1);

    RLine line2(RVector(0.0, -dBoardH, dDepth), RVector(dA-dR1, -dBoardH, dDepth));
    polyline.appendShape(line2);

    if (!(dR1 <= EPSINON && dR1 >= -EPSINON))
    {
        RArc arc1(RVector(dA-dR1, -(dBoardH-dR1), dDepth), dR1, 270.0*M_PI/180.0, 0.0*M_PI/180.0);
        polyline.appendShape(arc1);
    }

    RLine line3(RVector(dA, -(dBoardH-dR1), dDepth), RVector(dA, -(dB+dR2), dDepth));
    polyline.appendShape(line3);

    if (!(dR2 <= EPSINON && dR2 >= -EPSINON))
    {
        RArc arc2(RVector(dA+dR2, -(dB+dR2), dDepth), dR2, 180.0*M_PI/180.0, 90.0*M_PI/180.0, true);
        polyline.appendShape(arc2);
    }

    RLine line4(RVector(dA+dR2, -dB, dDepth), RVector(dBoardW-dR3, -dB, dDepth));
    polyline.appendShape(line4);

    if (!(dR3 <= EPSINON && dR3 >= -EPSINON))
    {
        RArc arc3(RVector(dBoardW-dR3, -(dB-dR3), dDepth), dR3, 270.0*M_PI/180.0, 0.0*M_PI/180.0);
        polyline.appendShape(arc3);
    }

    RLine line5(RVector(dBoardW, -(dB-dR3), dDepth), RVector(dBoardW, 0.0, dDepth));
    polyline.appendShape(line5);

    RLine line6(RVector(dBoardW, 0.0, dDepth), RVector(0.0, 0.0, dDepth));
    polyline.appendShape(line6);

    polyline.setClosed(true);

    if (!bRotate)
    {
        RVector pt1(dBoardW/2.0, -dBoardH/2.0);
        RVector pt2(dBoardH/2.0, -dBoardW/2.0);
        RVector ptOffset = pt2-pt1;
        polyline.move(ptOffset);
        polyline.rotate(-90.0*M_PI/180.0, pt2);
    }
    polyline.move(offset);

    QSharedPointer<RPolylineEntity> polylineEntity = QSharedPointer<RPolylineEntity>(
                new RPolylineEntity(pDocument, RPolylineData(polyline)));
    polylineEntity->setColor(RColor(0, 128, 255));
    //polylineEntity->setLinetypeId(pDocument->getLinetypeId("dashed"));
    polylineEntity->setLineweight(RLineweight::Weight050);

    op->addObject(polylineEntity, false);

    return true;
}

bool DissimilarLibraryOperator::drawRPolylineID011(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap)
{
    SystemConfigManager& smgr = SystemConfigManager::GetInstance();
    double dKnifeDiameter = smgr.getKinfeDiameter();

    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dR1 = 0.0f;
    double dR2 = 0.0f;
    int nN = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r1") != paramsMap.end())
    {
        var = paramsMap["r1"];
        dR1 = var.toDouble(&ok);
        if (!ok)
        {
            dR1 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("r2") != paramsMap.end())
    {
        var = paramsMap["r2"];
        dR2 = var.toDouble(&ok);
        if (!ok)
        {
            dR2 = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("n") != paramsMap.end())
    {
        var = paramsMap["n"];
        nN = var.toInt(&ok);
        if (!ok)
        {
            nN = 0;
        }
    }
    else
    {
        return false;
    }

    RPolyline polyline;

    if (!(dR2 <= EPSINON && dR2 >= -EPSINON))
    {
        RArc arc2(RVector(dR2, -dR2, dDepth), dR2, 90.0*M_PI/180.0, 180.0*M_PI/180.0);
        polyline.appendShape(arc2);
    }

    RLine line1(RVector(0.0, -dR2, dDepth), RVector(0.0, -(dBoardH-dR1), dDepth));
    polyline.appendShape(line1);

    if (!(dR1 <= EPSINON && dR1 >= -EPSINON))
    {
        RArc arc1(RVector(dR1, -(dBoardH-dR1), dDepth), dR1, 180.0*M_PI/180.0, 270.0*M_PI/180.0);
        polyline.appendShape(arc1);
    }

    RLine line2(RVector(dR1, -dBoardH, dDepth), RVector(dBoardW, -dBoardH, dDepth));
    polyline.appendShape(line2);

    if (nN == 0)
    {
        RLine line3(RVector(dBoardW, -dBoardH, dDepth), RVector(dBoardW, 0.0, dDepth));
        polyline.appendShape(line3);
    }
    else
    {
        double dSlotDepth = dBoardW/2.0f - dKnifeDiameter;
        double dAvgH = (dBoardH-dA*nN)/(nN+1);
        for (int i = 0; i < nN; ++i)
        {
            RVector pt1(dBoardW, -(dBoardH-i*(dAvgH+dA)), dDepth);
            RVector pt2(dBoardW, -(dBoardH-(i*(dAvgH+dA)+dAvgH)), dDepth);
            RLine line(pt1, pt2);
            polyline.appendShape(line);

            pt1 = pt2;
            pt2.setX(dSlotDepth);
            pt2.setY(-(dBoardH-(i*(dAvgH+dA)+dAvgH)));
            line.setStartPoint(pt1);
            line.setEndPoint(pt2);
            polyline.appendShape(line);

            pt1 = pt2;
            pt2.setX(dSlotDepth);
            pt2.setY(-(dBoardH-(i*(dAvgH+dA)+dAvgH+dA)));
            line.setStartPoint(pt1);
            line.setEndPoint(pt2);
            polyline.appendShape(line);

            pt1 = pt2;
            pt2.setX(dBoardW);
            pt2.setY(-(dBoardH-(i*(dAvgH+dA)+dAvgH+dA)));
            line.setStartPoint(pt1);
            line.setEndPoint(pt2);
            polyline.appendShape(line);

            if (i == (nN-1))
            {
                pt1 = pt2;
                pt2.setX(dBoardW);
                pt2.setY(0.0);
                line.setStartPoint(pt1);
                line.setEndPoint(pt2);
                polyline.appendShape(line);
            }
        }
    }

    RLine line4(RVector(dBoardW, 0.0, dDepth), RVector(dR2, 0.0, dDepth));
    polyline.appendShape(line4);

    polyline.setClosed(true);

    if (!bRotate)
    {
        RVector pt1(dBoardW/2.0, -dBoardH/2.0);
        RVector pt2(dBoardH/2.0, -dBoardW/2.0);
        RVector ptOffset = pt2-pt1;
        polyline.move(ptOffset);
        polyline.rotate(-90.0*M_PI/180.0, pt2);
    }

    polyline.move(offset);

    QSharedPointer<RPolylineEntity> polylineEntity = QSharedPointer<RPolylineEntity>(
                new RPolylineEntity(pDocument, RPolylineData(polyline)));
    polylineEntity->setColor(RColor(0, 128, 255));
    //polylineEntity->setLinetypeId(pDocument->getLinetypeId("dashed"));
    polylineEntity->setLineweight(RLineweight::Weight050);

    op->addObject(polylineEntity, false);

    return true;
}

bool DissimilarLibraryOperator::drawRPolylineID012(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap)
{
    SystemConfigManager& smgr = SystemConfigManager::GetInstance();
    double dKnifeDiameter = smgr.getKinfeDiameter();

    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dB = 0.0f;
    double dC = 0.0f;
    int nN = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("b") != paramsMap.end())
    {
        var = paramsMap["b"];
        dB = var.toDouble(&ok);
        if (!ok)
        {
            dB = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("c") != paramsMap.end())
    {
        var = paramsMap["c"];
        dC = var.toDouble(&ok);
        if (!ok)
        {
            dC = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("n") != paramsMap.end())
    {
        var = paramsMap["n"];
        nN = var.toInt(&ok);
        if (!ok)
        {
            nN = 0;
        }
    }
    else
    {
        return false;
    }

    RPolyline polyline;

    polyline.appendVertex(RVector(0.0, 0.0, dDepth));
    polyline.appendVertex(RVector(0.0, -dBoardH, dDepth));
    polyline.appendVertex(RVector(dBoardW, -dBoardH, dDepth));

    if (nN <= 0)
    {
        polyline.appendVertex(RVector(dBoardW, 0.0, dDepth));
    }
    else if (nN == 1)
    {
        double dSlotOffset = dBoardW/2.0f - dKnifeDiameter;

        // 先画a参数方向的竖线
        polyline.appendVertex(RVector(dBoardW, -(dBoardH-dA), dDepth));

        polyline.appendVertex(RVector(dSlotOffset, -(dBoardH-dA), dDepth));

        polyline.appendVertex(RVector(dSlotOffset, -(dBoardH-dA-dC), dDepth));

        polyline.appendVertex(RVector(dBoardW, -(dBoardH-dA-dC), dDepth));

        polyline.appendVertex(RVector(dBoardW, 0.0, dDepth));
    }
    else
    {
        // 先画a参数方向的竖线
        polyline.appendVertex(RVector(dBoardW, -(dBoardH-dA), dDepth));

        double dSlotOffset = dBoardW/2.0f - dKnifeDiameter;
        double dAvgH = (dBoardH-dC*nN-dB-dA)/(nN-1);
        for (int i = 0; i < nN; ++i)
        {
            polyline.appendVertex(RVector(dBoardW, -(dBoardH-dA-i*(dAvgH+dC)), dDepth));
            polyline.appendVertex(RVector(dSlotOffset, -(dBoardH-dA-i*(dAvgH+dC)), dDepth));
            polyline.appendVertex(RVector(dSlotOffset, -(dBoardH-dA-i*(dAvgH+dC)-dC), dDepth));
            polyline.appendVertex(RVector(dBoardW, -(dBoardH-dA-i*(dAvgH+dC)-dC), dDepth));

            if (i != (nN-1))
            {
                polyline.appendVertex(RVector(dBoardW, -(dBoardH-dA-i*(dAvgH+dC)-dC-dAvgH), dDepth));
            }
        }

        polyline.appendVertex(RVector(dBoardW, 0.0, dDepth));
    }

    polyline.setClosed(true);

    if (!bRotate)
    {
        RVector pt1(dBoardW/2.0, -dBoardH/2.0);
        RVector pt2(dBoardH/2.0, -dBoardW/2.0);
        RVector ptOffset = pt2-pt1;
        polyline.move(ptOffset);
        polyline.rotate(-90.0*M_PI/180.0, pt2);
    }

    polyline.move(offset);

    QSharedPointer<RPolylineEntity> polylineEntity = QSharedPointer<RPolylineEntity>(
                new RPolylineEntity(pDocument, RPolylineData(polyline)));
    polylineEntity->setColor(RColor(0, 128, 255));
    //polylineEntity->setLinetypeId(pDocument->getLinetypeId("dashed"));
    polylineEntity->setLineweight(RLineweight::Weight050);

    op->addObject(polylineEntity, false);

    return true;
}

bool DissimilarLibraryOperator::drawRPolylineID013(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dB = 0.0f;
    double dW = 0.0f;
    double dH = 0.0f;
    double dSlotDepth = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("b") != paramsMap.end())
    {
        var = paramsMap["b"];
        dB = var.toDouble(&ok);
        if (!ok)
        {
            dB = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("W") != paramsMap.end())
    {
        var = paramsMap["W"];
        dW = var.toDouble(&ok);
        if (!ok)
        {
            dW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("H") != paramsMap.end())
    {
        var = paramsMap["H"];
        dH = var.toDouble(&ok);
        if (!ok)
        {
            dH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Depth") != paramsMap.end())
    {
        var = paramsMap["Depth"];
        dSlotDepth = var.toDouble(&ok);
        if (!ok)
        {
            dSlotDepth = 0.0f;
        }
    }
    else
    {
        return false;
    }

    RPolyline polyline;

    polyline.appendVertex(RVector(0.0, 0.0, dDepth));
    polyline.appendVertex(RVector(0.0, -dBoardH, dDepth));
    polyline.appendVertex(RVector(dBoardW, -dBoardH, dDepth));
    polyline.appendVertex(RVector(dBoardW, 0.0, dDepth));

    polyline.setClosed(true);

    RVector pt1(dBoardW/2.0, -dBoardH/2.0);
    RVector pt2(dBoardH/2.0, -dBoardW/2.0);
    RVector ptOffset = pt2-pt1;

    if (!bRotate)
    {
        polyline.move(ptOffset);
        polyline.rotate(-90.0*M_PI/180.0, pt2);
    }

    polyline.move(offset);

    QSharedPointer<RPolylineEntity> polylineEntity = QSharedPointer<RPolylineEntity>(
                new RPolylineEntity(pDocument, RPolylineData(polyline)));
    polylineEntity->setColor(RColor(0, 128, 255));
    //polylineEntity->setLinetypeId(pDocument->getLinetypeId("dashed"));
    polylineEntity->setLineweight(RLineweight::Weight050);

    op->addObject(polylineEntity, false);

    RPolyline innerPolyline;

    innerPolyline.appendVertex(RVector(dA, -(dBoardH-dH-dB), dSlotDepth));
    innerPolyline.appendVertex(RVector(dA, -(dBoardH-dB), dSlotDepth));
    innerPolyline.appendVertex(RVector(dA+dW, -(dBoardH-dB), dSlotDepth));
    innerPolyline.appendVertex(RVector(dA+dW, -(dBoardH-dH-dB), dSlotDepth));

    innerPolyline.setClosed(true);

    if (!bRotate)
    {
        innerPolyline.move(ptOffset);
        innerPolyline.rotate(-90.0*M_PI/180.0, pt2);
    }

    innerPolyline.move(offset);

    QSharedPointer<RPolylineEntity> innerPolylineEntity = QSharedPointer<RPolylineEntity>(
                new RPolylineEntity(pDocument, RPolylineData(innerPolyline)));
    innerPolylineEntity->setColor(RColor(0, 128, 255));
    //innerPolylineEntity->setLinetypeId(pDocument->getLinetypeId("dashed"));
    innerPolylineEntity->setLineweight(RLineweight::Weight050);

    op->addObject(innerPolylineEntity, false);

    return true;
}

bool DissimilarLibraryOperator::drawRPolylineID014(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap)
{
    double dBoardW = 0.0f;
    double dBoardH = 0.0f;
    double dA = 0.0f;
    double dB = 0.0f;
    double dC = 0.0f;
    double dD = 0.0f;
    double dE = 0.0f;
    double dF = 0.0f;
    double dG = 0.0f;
    double dH = 0.0f;

    QVariant var;
    bool ok = false;

    if (paramsMap.find("Width") != paramsMap.end())
    {
        var = paramsMap["Width"];
        dBoardW = var.toDouble(&ok);
        if (!ok)
        {
            dBoardW = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("Height") != paramsMap.end())
    {
        var = paramsMap["Height"];
        dBoardH = var.toDouble(&ok);
        if (!ok)
        {
            dBoardH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("a") != paramsMap.end())
    {
        var = paramsMap["a"];
        dA = var.toDouble(&ok);
        if (!ok)
        {
            dA = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("b") != paramsMap.end())
    {
        var = paramsMap["b"];
        dB = var.toDouble(&ok);
        if (!ok)
        {
            dB = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("c") != paramsMap.end())
    {
        var = paramsMap["c"];
        dC = var.toDouble(&ok);
        if (!ok)
        {
            dC = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("d") != paramsMap.end())
    {
        var = paramsMap["d"];
        dD = var.toDouble(&ok);
        if (!ok)
        {
            dD = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("e") != paramsMap.end())
    {
        var = paramsMap["e"];
        dE = var.toDouble(&ok);
        if (!ok)
        {
            dE = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("f") != paramsMap.end())
    {
        var = paramsMap["f"];
        dF = var.toDouble(&ok);
        if (!ok)
        {
            dF = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("g") != paramsMap.end())
    {
        var = paramsMap["g"];
        dG = var.toDouble(&ok);
        if (!ok)
        {
            dG = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (paramsMap.find("h") != paramsMap.end())
    {
        var = paramsMap["h"];
        dH = var.toDouble(&ok);
        if (!ok)
        {
            dH = 0.0f;
        }
    }
    else
    {
        return false;
    }

    if (dA <= EPSINON && dA >= -EPSINON)
    {
        dA = 1.0f;
    }

    if (dB <= EPSINON && dB >= -EPSINON)
    {
        dB = 1.0f;
    }

    if (dC <= EPSINON && dC >= -EPSINON)
    {
        dC = 1.0f;
    }

    if (dD <= EPSINON && dD >= -EPSINON)
    {
        dD = 1.0f;
    }

    if (dE <= EPSINON && dE >= -EPSINON)
    {
        dE = 1.0f;
    }

    if (dF <= EPSINON && dF >= -EPSINON)
    {
        dF = 1.0f;
    }

    if (dG <= EPSINON && dG >= -EPSINON)
    {
        dG = 1.0f;
    }

    if (dH <= EPSINON && dH >= -EPSINON)
    {
        dH = 1.0f;
    }

    RPolyline polyline;

    polyline.appendVertex(RVector(dB, -0.0, dDepth));
    polyline.appendVertex(RVector(dB, -dA, dDepth));
    polyline.appendVertex(RVector(0.0, -dA, dDepth));
    polyline.appendVertex(RVector(0.0, -(dA+dC), dDepth));
    polyline.appendVertex(RVector(dD, -(dA+dC), dDepth));
    polyline.appendVertex(RVector(dD, -(dA+dC+dE), dDepth));
    polyline.appendVertex(RVector(0.0, -(dA+dC+dE), dDepth));
    polyline.appendVertex(RVector(0.0, -dBoardH, dDepth));
    polyline.appendVertex(RVector(dBoardW, -dBoardH, dDepth));
    polyline.appendVertex(RVector(dBoardW, -0.0, dDepth));
    polyline.appendVertex(RVector(dBoardW-dF, -0.0, dDepth));
    polyline.appendVertex(RVector(dBoardW-dF, -dG, dDepth));
    polyline.appendVertex(RVector(dBoardW-dF-dH, -dG, dDepth));
    polyline.appendVertex(RVector(dBoardW-dF-dH, -0.0, dDepth));

    polyline.setClosed(true);

    if (!bRotate)
    {
        RVector pt1(dBoardW/2.0, -dBoardH/2.0);
        RVector pt2(dBoardH/2.0, -dBoardW/2.0);
        RVector ptOffset = pt2-pt1;
        polyline.move(ptOffset);
        polyline.rotate(-90.0*M_PI/180.0, pt2);
    }

    polyline.move(offset);

    QSharedPointer<RPolylineEntity> polylineEntity = QSharedPointer<RPolylineEntity>(
                new RPolylineEntity(pDocument, RPolylineData(polyline)));
    polylineEntity->setColor(RColor(0, 128, 255));
    //polylineEntity->setLinetypeId(pDocument->getLinetypeId("dashed"));
    polylineEntity->setLineweight(RLineweight::Weight050);

    op->addObject(polylineEntity, false);

    return true;
}

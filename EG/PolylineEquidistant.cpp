/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file gpl-2.0.txt included in the
** packaging of this file.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
** This copyright notice MUST APPEAR in all copies of the script!
**
**********************************************************************/

#include "PolylineEquidistant.h"
#include<cmath>
#include "RShape.h"
#include "RArc.h"
#include "RLine.h"
#include "RPolyline.h"
#include "RDirected.h"
#include "RDebug.h"
#include "emu_c99.h"

PolylineEquidistant::PolylineEquidistant()
{
}

PolylineEquidistant::~PolylineEquidistant()
{
}

QSharedPointer<RPolyline> PolylineEquidistant::getEquidistantPolyline(QSharedPointer<RPolyline> originalPolyline, double dDist, bool isRightSide)
{
    if (originalPolyline.isNull())
    {
        return QSharedPointer<RPolyline>();
    }

    QList<QSharedPointer<RShape>> allShapes = originalPolyline->getExploded();
    if (allShapes.isEmpty())
    {
        return QSharedPointer<RPolyline>();
    }

    QList<QSharedPointer<RShape>> lsShape;

    for (int i = 0; i < allShapes.size(); ++i)
    {
        QSharedPointer<RShape> shape = allShapes.at(i);

        QSharedPointer<RArc> arcShape = shape.dynamicCast<RArc>();

        if (!arcShape.isNull())
        {
            if (arcShape->getLength() > 1.0e-10)
            {
                lsShape.append(shape);
            }
        }
        else
        {
            QSharedPointer<RLine> lineShape = shape.dynamicCast<RLine>();
            if (!lineShape.isNull())
            {
                if (lineShape->getLength() > 1.0e-10)
                {
                    lsShape.append(shape);
                }
            }
        }

       /*
       QSharedPointer<RDirected> directed = shape.dynamicCast<RDirected>();
       if (!directed.isNull() && directed->getDistanceFromStart(directed->getEndPoint()) > 1.0e-12)
       {
           lsShape.append(shape);
       }*/
    }

    double neg = 1.0;
    if(isRightSide)
    {
        neg = -1.0;
    }

    // Create new helper entities

    // 当前直线
    QSharedPointer<RLine> line1 = QSharedPointer<RLine>(new RLine(RVector(), RVector()));

    // 上一条直线
    QSharedPointer<RLine> lineFirst = QSharedPointer<RLine>(new RLine(RVector(), RVector()));

    // 当前圆弧
    QSharedPointer<RArc> arc1 = QSharedPointer<RArc>(new RArc(RVector(), 0, 0, 0, false));

    // 上一个圆弧
    QSharedPointer<RArc> arcFirst = QSharedPointer<RArc>(new RArc(RVector(), 0, 0, 0, false));


    QSharedPointer<RPolyline> newPolyline = QSharedPointer<RPolyline>(new RPolyline());
    bool first = true;
    bool closed = originalPolyline->isClosed();
    double bulge = 0.0;

    QSharedPointer<RShape> en;
    QSharedPointer<RShape> prevShape = lsShape.last();
    QSharedPointer<RShape> currShape;

    for (int i = 0; i < lsShape.size(); ++i)
    {
        en = lsShape.at(i);
        RVector v = RVector::invalid;

        QSharedPointer<RArc> arcShape = en.dynamicCast<RArc>();

        if (!arcShape.isNull())
        {
            currShape = arc1;
            calculateOffset(currShape, en, dDist*neg);
            bulge = arc1->getBulge();
        }
        else
        {
            currShape = line1;
            calculateOffset(currShape, en, dDist*neg);
            bulge = 0.0;
        }

        if (first)
        {
            if (closed)
            {
                QSharedPointer<RArc> prevArcShape = prevShape.dynamicCast<RArc>();

                if (!prevArcShape.isNull())
                {
                    prevShape = calculateOffset(arcFirst, prevShape, dDist*neg);
                }
                else
                {
                    prevShape = calculateOffset(lineFirst, prevShape, dDist*neg);
                }
                v = calculateIntersection(prevShape, currShape);
            }

            if (!v.isValid())
            {
                QSharedPointer<RDirected> currDirected = currShape.dynamicCast<RDirected>();
                if (!currDirected.isNull())
                {
                    v = currDirected->getStartPoint();
                    closed = false;
                }
            }
            else
            {
                QSharedPointer<RArc> currArcShape = currShape.dynamicCast<RArc>();
                if (!currArcShape.isNull())
                {
                    //update bulge
                    arc1->setStartAngle(arc1->getCenter().getAngleTo(v));
                    //arc1.calculateEndpoints();
                    bulge = arc1->getBulge();
                }
            }

            first = false;

            //prevent crash if not exist offset for prevEntity
            if (prevShape.isNull())
            {
                break;
            }
        }
        else
        {
            QSharedPointer<RDirected> prevDirected = prevShape.dynamicCast<RDirected>();
            QSharedPointer<RDirected> currDirected = currShape.dynamicCast<RDirected>();

            v = calculateIntersection(prevShape, currShape);
            if (!v.isValid())
            {
                v = prevDirected->getEndPoint();

                double dess = currDirected->getStartPoint().getDistanceTo(prevDirected->getEndPoint());

                if (dess > 1.0e-10)
                {
                    newPolyline->appendVertex(v, bulge);
                    prevShape = QSharedPointer<RShape>();
                    break;
                }
            }

            double startAngle = 0.0;

            QSharedPointer<RArc> prevArcShape = prevShape.dynamicCast<RArc>();
            if (!prevArcShape.isNull())
            {
                startAngle = prevArcShape->getStartPoint().getAngleTo(prevArcShape->getEndPoint());

                arcFirst->setEndAngle(arcFirst->getCenter().getAngleTo(v));
                //arcFirst.calculateEndpoints();
                //newPolyline->setNextBulge(arcFirst.getBulge());
            }
            else
            {
                QSharedPointer<RLine> prevLineShape = prevShape.dynamicCast<RLine>();
                startAngle = prevLineShape->getStartPoint().getAngleTo(prevLineShape->getEndPoint());
            }

            //check if the entity are reverted
            if (fabs(remainder(prevDirected->getStartPoint().getAngleTo(prevDirected->getEndPoint())- startAngle, 2.0*M_PI)) > 0.785)
            {
                prevShape = newPolyline->getSegmentAt(newPolyline->countSegments()-1);
                RVector v0 = calculateIntersection(prevShape, currShape);
                double bulge0 = 0.0;
                prevArcShape = prevShape.dynamicCast<RArc>();
                if (!prevArcShape.isNull())
                {
                    prevArcShape->setEndAngle(arcFirst->getCenter().getAngleTo(v0));
                    //((RS_Arc*)prevEntity)->calculateEndpoints();
                    //newPolyline->setNextBulge(((RS_Arc*)prevEntity)->getBulge());
                    bulge0 = prevArcShape->getBulge();
                }
                else
                {
                    QSharedPointer<RLine> prevLineShape = prevShape.dynamicCast<RLine>();
                    prevLineShape->setEndPoint(v0);
                    //newPolyline->setNextBulge(0.0);
                    bulge0 = 0.0;
                }

                //newPolyline->setEndPoint(v0);
                newPolyline->appendVertex(v0, bulge0);
            }

            QSharedPointer<RArc> currArcShape = currShape.dynamicCast<RArc>();
            if (!currArcShape.isNull())
            {
                arc1->setStartAngle(arc1->getCenter().getAngleTo(v));
                //arc1.calculateEndpoints();
                bulge = arc1->getBulge();
            }
            else
            {
                bulge = 0.0;
            }
        }

        if (!prevShape.isNull())
        {
            newPolyline->appendVertex(v, bulge);
            QSharedPointer<RArc> currArcShape = currShape.dynamicCast<RArc>();
            if (!currArcShape.isNull())
            {
                arcFirst->setRadius(arc1->getRadius());
                arcFirst->setCenter(arc1->getCenter());
                arcFirst->setStartAngle(arc1->getStartAngle());
                arcFirst->setEndAngle(arc1->getEndAngle());
                arcFirst->setReversed(arc1->isReversed());

                prevShape = arcFirst;
            }
            else
            {
                RVector ptStart = line1->getStartPoint();
                RVector ptEnd = line1->getEndPoint();
                lineFirst->setStartPoint(ptStart);
                lineFirst->setEndPoint(ptEnd);

                prevShape = lineFirst;
            }
        }
    }

    //properly terminated, check closed
    if (!prevShape.isNull())
    {
        QSharedPointer<RDirected> currDirected = currShape.dynamicCast<RDirected>();
        if (closed)
        {
            if (!currDirected.isNull())
            {
                arc1->setEndAngle(arc1->getCenter().getAngleTo(newPolyline->getStartPoint()));
                //arc1.calculateEndpoints();
                //newPolyline->setNextBulge(arc1.getBulge());
                bulge = arc1->getBulge();
            }

            newPolyline->setClosed(true);
        }
        else
        {
            newPolyline->appendVertex(currDirected->getEndPoint(), bulge);
        }
    }

    //newPolyline->setClosed(true);
    return newPolyline;
}

/**
 * Helper function for makeContour
 * Modify newEntity to parellel of orgEntity at distance dist
 * If dist is positive the offset is in left else in right
 * Bot newEntity and orgEntity are the same type of entity
 * if not return NULL pointer
 *
 * @retval QSharedPointer<RShape> of parellel RShape
 *
 * @author Rallaz
 */
QSharedPointer<RShape> PolylineEquidistant::calculateOffset(QSharedPointer<RShape> newShape, QSharedPointer<RShape> orgShape, double dDist)
{
    QSharedPointer<RArc> newArc = newShape.dynamicCast<RArc>();
    QSharedPointer<RArc> orgArc = orgShape.dynamicCast<RArc>();
    QSharedPointer<RLine> newLine = newShape.dynamicCast<RLine>();
    QSharedPointer<RLine> orgLine = orgShape.dynamicCast<RLine>();

    if (!newArc.isNull() && !orgArc.isNull())
    {
        double r0 = orgArc->getRadius();
        double r = 0.0;

        if (orgArc->isReversed())
            r = r0 + dDist;
        else
            r = r0 - dDist;
        if(r < 0)
            return QSharedPointer<RShape>();

        newArc->setRadius(r);
        newArc->setCenter(orgArc->getCenter());
        newArc->setStartAngle(orgArc->getStartAngle());
        newArc->setEndAngle(orgArc->getEndAngle());
        newArc->setReversed(orgArc->isReversed());
        return newArc;
    }
    else if (!newLine.isNull() && !orgLine.isNull())
    {
        RVector v0 = orgLine->getStartPoint();
        RVector v1(v0.x, v0.y + dDist);
        RVector v2(v0.x + orgLine->getLength(), v0.y + dDist);
        newLine->setStartPoint(v1);
        newLine->setEndPoint(v2);
        newLine->rotate(orgLine->getAngle(), v0);
        return newLine;
    }

    return QSharedPointer<RShape>();
}

/**
 * Helper function for makeContour
 * Calculate the intersection point of first and last RShape
 * The first vertex is not added and the last is returned instead of added
 *
 * @retval RVector nearest to startpoint of last and endpoint of first or RVector::invalid if not
 *
 * @author Rallaz
 */
RVector PolylineEquidistant::calculateIntersection(QSharedPointer<RShape> fstShape, QSharedPointer<RShape> lstShape)
{
    RShape* pFstShape = fstShape.data();
    RShape* pLstShape = lstShape.data();

    QList<RVector> vsol;

    vsol = RShape::getIntersectionPoints(*pFstShape, *pLstShape, false);
    if (vsol.size() == 0)
    {
        //Parallel entities
        return RVector::invalid;
    }
    else if (vsol.size() > 1)
    {
        QSharedPointer<RDirected> lstDirected = lstShape.dynamicCast<RDirected>();

        if (!lstDirected.isNull() && vsol.at(0).getDistanceTo(lstDirected->getStartPoint()) > vsol.at(1).getDistanceTo(lstDirected->getStartPoint()))
        {
            return vsol.at(1);
        }
    }

    return vsol.at(0);
}

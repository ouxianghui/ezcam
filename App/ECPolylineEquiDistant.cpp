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

#include "ECPolylineEquiDistant.h"
#include<cmath>
#include <QAction>
#include "RShape.h"
#include "RArc.h"
#include "RArcData.h"
#include "RArcEntity.h"
#include "RLine.h"
#include "RLineData.h"
#include "RLineEntity.h"
#include "RPolyline.h"
#include "RPolylineData.h"
#include "RPolylineEntity.h"
#include "RDebug.h"

ECPolylineEquidistant::ECPolylineEquidistant(RS_EntityContainer& container,
                                             RS_GraphicView& graphicView)
    :RS_PreviewActionInterface("Create Equidistant Polylines",
                               container, graphicView)
    ,m_dDist(1.)
    ,m_nNumber(1)
{
    actionType=RS2::ActionPolylineEquidistant;
}


//QAction* ECPolylineEquidistant::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/)
//{
//    QAction* action = new QAction(QIcon(":/extui/polylineequidstant.png"), tr("Create &Equidistant Polylines"), nullptr);
//    //		action->setShortcut(QKeySequence());
//    action->setStatusTip(tr("Create Equidistant Polylines"));
//    return action;
//}

void ECPolylineEquidistant::init(int status)
{
    RS_PreviewActionInterface::init(status);
    originalEntity = nullptr;
    m_targetPoint = RVector::invalid;
    m_bRightSide = false;
}

/**
 * Helper function for makeContour
 * Modify newEntity to parellel of orgEntity at distance dist
 * If dist is positive the offset is in left else in right
 * Bot newEntity and orgEntity are the same type of entity
 * if not return NULL pointer
 *
 * @retval RS_Entity* of parellel entity
 *
 * @author Rallaz
 */
QSharedPointer<REntity> ECPolylineEquidistant::calculateOffset(QSharedPointer<REntity> newEntity, QSharedPointer<REntity> orgEntity, double dDist)
{
    if (orgEntity->getType()==RS::EntityArc && newEntity->getType()==RS::EntityArc)
    {
        QSharedPointer<RArcEntity> arcNewEntity = newEntity.dynamicCast<RArcEntity>();
        QSharedPointer<RArcEntity> arcOrgEntity = orgEntity.dynamicCast<RArcEntity>();
        double r0 = arcOrgEntity->getRadius();
        double r;
        if (arcOrgEntity->isReversed())
            r = r0 + dist;
        else
            r = r0 - dist;
        if(r < 0)
            return QSharedPointer<REntity>();
        arcNewEntity->setShape(arcOrgEntity->getData().getArc());
        arcNewEntity->setRadius(r);
        //arcNewEntity->calculateEndpoints();
        return newEntity;
    }
    else if (orgEntity->getType()==RS::EntityLine && newEntity->getType()==RS::EntityLine)
    {
        QSharedPointer<RLineEntity> line0 = orgEntity.dynamicCast<RLineEntity>();
        QSharedPointer<RLineEntity> line1 = newEntity.dynamicCast<RLineEntity>();
        RVector v0 = line0->getStartPoint();
        RVector v1(v0.x,v0.y+dDist);
        RVector v2(v0.x+line0->getLength(), v0.y+dDist);
        line1->setStartPoint(v1);
        line1->setEndPoint(v2);
        line1->rotate(v0, line0->getAngle());
        return newEntity;
    }
    return nullptr;
}

/**
 * Helper function for makeContour
 * Calculate the intersection point of first and last entities
 * The first vertex is not added and the last is returned instead of added
 *
 * @retval RS_Vector nearest to startpoint of last and endpoint of first or RS_Vector(false) if not
 *
 * @author Rallaz
 */
RVector ECPolylineEquidistant::calculateIntersection(QSharedPointer<REntity> first, QSharedPointer<REntity> last)
{
    QList<RVector> vsol;

    QSharedPointer<RLineEntity> firstEntity = first.dynamicCast<RLineEntity>();
    QSharedPointer<RLineEntity> lastEntity = last.dynamicCast<RLineEntity>();
    RShape& firstShape = first->castToShape();
    RShape& lastShape = last->castToShape();

    vsol = RShape::getIntersectionPoints(firstShape, lastShape);
    if (vsol.size()==0)
    {
        //Parallel entities
        return RVector(RVector::invalid);
    }
    else if (vsol.size()>1 &&
             vsol.at(0).getDistanceTo(lastEntity->getStartPoint()) > vsol.at(1).getDistanceTo(lastEntity->getStartPoint()))
    {
        return vsol.at(1);
    }
    return vsol.at(0);
}

bool ECPolylineEquidistant::makeContour()
{
    if (!container)
    {
        RDebug::printBacktrace("ECPolylineEquidistant::makeContour: no valid container");
        return false;
    }

    QSharedPointer<RPolylineEntity> originalPolyline = originalEntity.dynamicCast<RPolylineEntity>();

    //create a list of entities to offset without length = 0
    QList<QSharedPointer<RShape>> entities = originalPolyline->getExploded();

    if (entities.isEmpty())
    {
        return false;
    }

    double neg = 1.0;
    if(m_bRightSide)
        neg = -1.0;

    // Create new helper entities
    RLine line1(RVector(), RVector());//current line
    QSharedPointer<RLineEntity> line1Entity =  QSharedPointer<RLineEntity>(new RLineEntity(NULL, RLineData(line1)));

    RLine lineFirst(RVector(), RVector());//previous line
    QSharedPointer<RLineEntity> lineFirstEntity =  QSharedPointer<RLineEntity>(new RLineEntity(NULL, RLineData(lineFirst)));


    RArc arc1(RVector(), 0, 0, 0, false);//current arc
    QSharedPointer<RArcEntity> arc1Entity =  QSharedPointer<RLineEntity>(new RArcEntity(NULL, RArc(arc1)));

    RArc arcFirst(RVector(), 0, 0, 0, false);//previous arc
    QSharedPointer<RArcEntity> arcFirstEntity =  QSharedPointer<RLineEntity>(new RArcEntity(NULL, RArc(arcFirst)));

    for (int num=1; num<=m_nNumber || (m_nNumber==0 && num<=1); num++)
    {
        QSharedPointer<RPolylineEntity> newPolyline = QSharedPointer<RPolylineEntity>(new RPolylineEntity(NULL, RPolylineData());

        //newPolyline->setLayer(((RS_Polyline*)originalEntity)->getLayer());
        //newPolyline->setPen(((RS_Polyline*)originalEntity)->getPen());

        bool first = true;
        bool closed = originalPolyline->isClosed();
        double bulge = 0.0;
        QSharedPointer<REntity> en;
        QSharedPointer<REntity> prevEntity = entities.last();
        QSharedPointer<REntity> currEntity = NULL;
        for (int i = 0; i < entities.size(); ++i)
        {
            en = entities.at(i);
            RVector v(false);
            if (en->getType()==RS::EntityArc)
            {
                currEntity = arc1Entity;
                calculateOffset(currEntity, en, m_dDist*num*neg);
                bulge = arc1.getBulge();
            }
            else
            {
                currEntity = line1Entity;
                bulge = 0.0;
                calculateOffset(currEntity, en, m_dDist*num*neg);
            }
            if (first)
            {
                if (closed)
                {
                    if (prevEntity->getType()==RS::EntityArc)
                    {
                        prevEntity = calculateOffset(&arcFirst, prevEntity, m_dDist*num*neg);
                    }
                    else
                    {
                        prevEntity = calculateOffset(&lineFirst, prevEntity, m_dDist*num*neg);
                    }
                    v = calculateIntersection(prevEntity, currEntity);
                }
                if (!v.valid)
                {
                    v = currEntity->getStartPoint();
                    closed = false;
                }
                else if (currEntity->getType()==RS::EntityArc)
                {
                    //update bulge
                    arc1.setStartAngle(arc1.getCenter().getAngleTo(v));
                    //arc1.calculateEndPoints();
                    bulge = arc1.getBulge();
                }
                first = false;
                if (!prevEntity) break; //prevent crash if not exist offset for prevEntity
            }
            else
            {
                v = calculateIntersection(prevEntity, currEntity);
                if (!v.valid)
                {
                    v= prevEntity->getEndpoint();
                    double dess = currEntity->getStartPoint().distanceTo(prevEntity->getEndPoint());
                    if (dess > 1.0e-12)
                    {
                        newPolyline->addVertex(v, bulge);
                        prevEntity = NULL;
                        break;
                    }
                }
                double startAngle = prevEntity->getStartpoint().angleTo(prevEntity->getEndpoint());
                if (prevEntity->getType()==RS::EntityArc)
                {
                    arcFirst.setEndAngle(arcFirst.getCenter().getAngleTo(v));
                    //arcFirst.calculateEndPoints();
                    newPolyline->setBulgeAt(arcFirst.getBulge());
                }

                //check if the entity are reverted
                if (fabs(remainder(prevEntity->getStartpoint().angleTo(prevEntity->getEndpoint())- startAngle, 2.*M_PI)) > 0.785)
                {
                    prevEntity = newPolyline->lastEntity();
                    RS_Vector v0 = calculateIntersection(prevEntity, currEntity);
                    if (prevEntity->getType()==RS::EntityArc)
                    {
                        ((RS_Arc*)prevEntity)->setAngle2(arcFirst.getCenter().angleTo(v0));
                        ((RS_Arc*)prevEntity)->calculateEndpoints();
                        newPolyline->setNextBulge( ((RS_Arc*)prevEntity)->getBulge() );
                    }
                    else
                    {
                        ((RS_Line*)prevEntity)->setEndpoint(v0);
                        newPolyline->setNextBulge( 0.0 );
                    }
                    newPolyline->setEndPoint(v0);
                }
                if (currEntity->getType()==RS::EntityArc)
                {
                    arc1.setAngle1(arc1.getCenter().angleTo(v));
                    arc1.calculateEndpoints();
                    bulge = arc1.getBulge();
                }
                else
                    bulge = 0.0;
            }
            if (prevEntity)
            {
                newPolyline->addVertex(v, bulge, false);
                if (currEntity->getType()==RS::EntityArc)
                {
                    arcFirst.setData(arc1.getData());
                    arcFirst.calculateEndpoints();
                    prevEntity = &arcFirst;
                }
                else
                {
                    lineFirst.setStartpoint(line1.getStartpoint());
                    lineFirst.setEndpoint(line1.getEndpoint());
                    prevEntity = &lineFirst;
                }
            }
        }
        //properly terminated, check closed
        if (prevEntity)
        {
            if (closed)
            {
                if (currEntity->getType()==RS::EntityArc)
                {
                    arc1.setAngle2(arc1.getCenter().angleTo(newPolyline->getStartpoint()));
                    arc1.calculateEndpoints();
                    newPolyline->setNextBulge(arc1.getBulge());
                    bulge = arc1.getBulge();
                }
                newPolyline->setClosed(true, bulge);
            }
            else
            {
                newPolyline->addVertex(currEntity->getEndpoint(), bulge);
            }
        }

        if (!newPolyline->isEmpty())
        {
            container->addEntity(newPolyline);
            document->addUndoable(newPolyline);
        }
    }

    if (document)
    {
        document->endUndoCycle();
    }

    if (graphicView)
    {
        graphicView->redraw();
    }

    return true;
}

void ECPolylineEquidistant::trigger()
{

    RS_DEBUG->print("ECPolylineEquidistant::trigger()");

    if (originalEntity && targetPoint.valid )
    {

        originalEntity->setHighlighted(false);
        graphicView->drawEntity(originalEntity);

        makeContour();

        originalEntity = NULL;
        targetPoint = RS_Vector(false);
        m_bRightSide = false;
        setStatus(ChooseEntity);

        RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected(),container->totalSelectedLength());
    }
    ////////////////////////////////////////2006/06/15
    graphicView->redraw();
    ////////////////////////////////////////
}

void ECPolylineEquidistant::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button()==Qt::LeftButton)
    {
        switch (getStatus())
        {
        case ChooseEntity:
            originalEntity = catchEntity(e);
            if (originalEntity==NULL)
            {
                RS_DIALOGFACTORY->commandMessage(tr("No Entity found."));
            }
            else if (originalEntity->rtti()!=RS2::EntityPolyline)
            {

                RS_DIALOGFACTORY->commandMessage(tr("Entity must be a polyline."));
            }
            else
            {
                targetPoint = snapFree(e);
                originalEntity->setHighlighted(true);
                graphicView->drawEntity(originalEntity);
                double d = graphicView->toGraphDX(snapRange)*0.9;
                RS_Entity* Segment = ((RS_Polyline*)originalEntity)->getNearestEntity( targetPoint, &d, RS2::ResolveNone);
                if (Segment->rtti() == RS2::EntityLine)
                {
                    double ang = ((RS_Line*)Segment)->getAngle1();
                    double ang1 = ((RS_Line*)Segment)->getStartpoint().angleTo(RS_Vector(targetPoint));
                    if( ang > ang1 || ang + M_PI < ang1 )
                        m_bRightSide = true;
                }
                else
                {
                    RS_Vector cen = ((RS_Arc*)Segment)->getCenter();
                    if (cen.distanceTo(targetPoint) > ((RS_Arc*)Segment)->getRadius() && ((RS_Arc*)Segment)->getBulge() > 0 )
                        m_bRightSide = true;
                }

                ////////////////////////////////////////2006/06/15
                graphicView->redraw();
                ////////////////////////////////////////
                trigger();
            }
            break;
        default:
            break;
        }
    }
    else if (e->button()==Qt::RightButton)
    {
        deleteSnapper();
        if (originalEntity)
        {
            originalEntity->setHighlighted(false);
            graphicView->drawEntity(originalEntity);
            ////////////////////////////////////////2006/06/15
            graphicView->redraw();
            ////////////////////////////////////////
        }
        init(getStatus()-1);
    }
}

void ECPolylineEquidistant::showOptions()
{
    RS_ActionInterface::showOptions();

    RS_DIALOGFACTORY->requestOptions(this, true);
}

void ECPolylineEquidistant::hideOptions()
{
    RS_ActionInterface::hideOptions();

    RS_DIALOGFACTORY->requestOptions(this, false);
}

void ECPolylineEquidistant::updateMouseCursor()
{
    graphicView->setMouseCursor(RS2::CadCursor);
}

void ECPolylineEquidistant::updateMouseButtonHints()
{
    switch (getStatus())
    {
    case ChooseEntity:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Choose the original polyline"), tr("Cancel"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget();
        break;
    }
}

// EOF

#include "SegmentsCreator.h"
#include "RPolylineEntity.h"
#include "REllipseEntity.h"
#include "RSplineEntity.h"
#include "RHatchData.h"
#include "RShape.h"

double SegmentsCreator::_tolerance = 1e-3;

SegmentsCreator::SegmentsCreator(const RDocument& document)
    : _document(document),
      _connectionPoint(RVector::invalid),
      _errorPoint(RVector::invalid)
{

}

SegmentsCreator::~SegmentsCreator()
{

}

/**
 * Verifies the entities entityIds as potential hatch boundary.
 */
bool SegmentsCreator::verifyBoundary(const QList<REntity::Id>& entityIds)
{
    for (int i = 0; i < entityIds.length(); ++i)
    {
        REntity::Id entityId = entityIds[i];
        QSharedPointer<REntity> entity = _document.queryEntityDirect(entityId);
        if (entity->getType() != RS::EntityLine
                && entity->getType() != RS::EntityArc
                && entity->getType() != RS::EntityCircle
                && entity->getType() != RS::EntityEllipse
                && entity->getType() != RS::EntityPolyline
                && entity->getType() != RS::EntitySpline)
        {
            return false;
        }
    }

    return true;
}

/**
 * \return RHatchData object created from the given boundary entities entityIds or undefined.
 */
QList<QList<QSharedPointer<RShape>>> SegmentsCreator::excractSegments(const QList<REntity::Id>& entityIds)
{
    RHatchData hatchData;

    if (!verifyBoundary(entityIds))
    {
        return hatchData.getBoundary();
    }

    _connectionPoint = RVector::invalid;
    _errorPoint = RVector::invalid;
    _traversed.clear();

    for (int i = 0; i < entityIds.length(); ++i)
    {
        const REntity::Id& entityId = entityIds[i];
        if (_traversed[entityId] == true)
        {
            continue;
        }
        QSharedPointer<REntity> entity = _document.queryEntityDirect(entityId);
        traverse(hatchData, entity, entityIds);
    }

    return hatchData.getBoundary();
}

/**
 * Traverses the given candidates recursively for connected entities.
 * Resulting loops are appened to the given hatch data.
 */
void SegmentsCreator::traverse(RHatchData& hatchData, QSharedPointer<REntity> entity, const QList<REntity::Id>& candidateIds)
{
    // handle closed polyline loops:
    if (isClosedPolyline(entity))
    {
        if (entity->getData().castToShape()->getLength() < RS::PointTolerance)
        {
            // zero length closed polyline: safe to ignore:
            return;
        }

        QSharedPointer<RPolylineEntity> polylineEntity = entity.dynamicCast<RPolylineEntity>();
        hatchData.newLoop();
        QList<QSharedPointer<RShape> > segments = polylineEntity->getExploded();
        for (int i = 0; i < segments.length(); ++i)
        {
            QSharedPointer<RShape> segment = segments[i];
            hatchData.addBoundary(segment);
        }
        // TODO: add polyline as boundary:
        //hatchData.addBoundary(entity.getData().castToShape());
        _traversed[entity->getId()] = true;
        return;
    }

    // handle circle, full ellipse and closed spline loops:
    if (isClosedCurve(entity)) {
        hatchData.newLoop();
        QSharedPointer<RShape> shape = QSharedPointer<RShape>(entity->getData().castToShape()->clone());
        hatchData.addBoundary(shape);
        _traversed[entity->getId()] = true;
        return ;
    }

    QSharedPointer<RShape> shape = QSharedPointer<RShape>(entity->getData().castToShape()->clone());
    if (shape.isNull() || shape->getLength() < RS::PointTolerance)
    {
        // ignore zero length entity:
        return;
    }

    // connect 'loose' boundary elements into loops:
    hatchData.newLoop();
    _traversed[entity->getId()] = true;
    hatchData.addBoundary(shape);
    QSharedPointer<RShape> currentShape = shape;

    QSharedPointer<RDirected> directed = shape.dynamicCast<RDirected>();
    RVector loopStartPoint = directed->getStartPoint();

    // find connected entities:
    bool done2 = true;
    _connectionPoint = RVector::invalid;
    do
    {
        done2 = true;
        directed = currentShape.dynamicCast<RDirected>();
        _connectionPoint = directed->getEndPoint();
        for (int i = 0; i < candidateIds.length(); i++)
        {
            REntity::Id entityId = candidateIds[i];
            if (_traversed[entityId] == true) {
                continue;
            }

            // query entity from document:
            entity = _document.queryEntityDirect(entityId);

            if (isClosedCurve(entity) || isClosedPolyline(entity)) {
                continue;
            }

            shape = QSharedPointer<RShape>(entity->getData().castToShape()->clone());
            if (!shape.isNull())
            {
                // ignore zero length entities:
                if (shape->getLength() < RS::PointTolerance) {
                    _traversed[entityId] = true;
                    continue;
                }
            }

            directed = shape.dynamicCast<RDirected>();
            RVector sp = directed->getStartPoint();
            RVector ep = directed->getEndPoint();

            bool spConnects = _connectionPoint.equalsFuzzy(sp, _tolerance);
            bool epConnects = _connectionPoint.equalsFuzzy(ep, _tolerance);

            if (spConnects || epConnects)
            {
                _traversed[entityId] = true;
                shape = QSharedPointer<RShape>(entity->getData().castToShape()->clone());
                directed = shape.dynamicCast<RDirected>();
                if (epConnects) {
                    directed->reverse();
                }
                hatchData.addBoundary(shape);
                currentShape = shape;
                done2 = false;
                //qDebug("Hatch: next loop shape: ", shape, epConnects ? " (reversed)" : "");
                break;
            }

            //qDebug("gap sp: ", _connectionPoint.getDistanceTo(sp));
            //qDebug("gap ep: ", _connectionPoint.getDistanceTo(ep));
        }
    } while (!done2);

    if (!_connectionPoint.equalsFuzzy(loopStartPoint, _tolerance))
    {
        _errorPoint = _connectionPoint;
        hatchData.cancelLoop();
        //qDebug("start does not connect to end by: ", _connectionPoint.getDistanceTo(loopStartPoint));
        return;
    }

    return;
}

bool SegmentsCreator::isClosedPolyline(QSharedPointer<REntity> entity)
{
    if (entity->getType() == RS::EntityPolyline)
    {
        QSharedPointer<RPolylineEntity> polylineEntity = entity.dynamicCast<RPolylineEntity>();
        if (!polylineEntity.isNull())
        {
            return polylineEntity->isClosed();
        }
    }

    return false;
}

bool SegmentsCreator::isClosedCurve(QSharedPointer<REntity> entity)
{
    if (entity->getType() == RS::EntityCircle)
    {
        return true;
    }
    else if (entity->getType() == RS::EntityEllipse)
    {
        QSharedPointer<REllipseEntity> ellipseEntity = entity.dynamicCast<REllipseEntity>();
        if (!ellipseEntity.isNull())
        {
            return ellipseEntity->isFullEllipse();
        }
    }
    else if (entity->getType() == RS::EntitySpline)
    {
        QSharedPointer<RSplineEntity> splineEntity = entity.dynamicCast<RSplineEntity>();
        if (!splineEntity.isNull())
        {
            return splineEntity->isClosed();
        }
    }

    return false;
}

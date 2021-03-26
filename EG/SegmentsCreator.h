#ifndef SEGMENTSCREATOR_H
#define SEGMENTSCREATOR_H

#include "eg_global.h"
#include <QSharedPointer>
#include "REntity.h"
#include "RDocument.h"
#include "RVector.h"

class RHatchData;

class EGSHARED_EXPORT SegmentsCreator
{
public:
    SegmentsCreator(const RDocument& document);
    ~SegmentsCreator();

    QList<QList<QSharedPointer<RShape>>> excractSegments(const QList<REntity::Id>& entityIds);

private:
    bool verifyBoundary(const QList<REntity::Id>& entityIds);

    bool isClosedPolyline(QSharedPointer<REntity> entity);
    bool isClosedCurve(QSharedPointer<REntity> entity);

    void traverse(RHatchData& hatchData, QSharedPointer<REntity> entity, const QList<REntity::Id>& candidateIds);

private:
    const RDocument& _document;
    static double _tolerance;

    RVector _connectionPoint;
    RVector _errorPoint;

    QMap<REntity::Id, bool> _traversed;
};

#endif // SEGMENTSCREATOR_H

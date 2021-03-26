#ifndef EG_H
#define EG_H

#include "eg_global.h"
#include "RArc.h"

class RDocument;
class RDocumentInterface;

class EGSHARED_EXPORT EG
{

public:
    EG();
    static RArc getArc2PR(RVector point1,
                          RVector point2,
                          double radius,
                          bool reversed = false,
                          bool alternativeSolution = false);

    static bool copyShapes2Document(const QString& fileName, const RVector& pt, double rotationDegrees, bool flipH, bool flipV, RDocument& dst);
    static bool copyShapes2Document(const QString& fileName, const RVector& pt, double rotationDegrees, bool flipH, bool flipV, RDocumentInterface* dst);

};

#endif // EG_H

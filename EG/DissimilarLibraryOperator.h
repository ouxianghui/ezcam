#ifndef DISSIMILARLIBRARYOPERATOR_H
#define DISSIMILARLIBRARYOPERATOR_H

#include "eg_global.h"
#include <vector>
#include <map>
#include <QPainter>
#include <QStringList>
#include <QSharedPointer>
#include "RDocument.h"
#include "RShape.h"
#include "RMixedOperation.h"

class EGSHARED_EXPORT DissimilarLibraryOperator
{
public:
    DissimilarLibraryOperator();
    ~DissimilarLibraryOperator();

    static bool getAllDissimilarImageName(QStringList& sl);
    static bool getDissimilarParametersName(int nID, QStringList& sl);
    static bool draw2QPainter(QPainter& painter, int nID, std::map<QString, QVariant>& paramsMap);
    static bool drawRPolyline(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, int nID, std::map<QString, QVariant>& paramsMap);

protected:
    static bool draw2QPainterID000(QPainter& painter, std::map<QString, QVariant>& paramsMap);
    static bool draw2QPainterID001(QPainter& painter, std::map<QString, QVariant>& paramsMap);
    static bool draw2QPainterID002(QPainter& painter, std::map<QString, QVariant>& paramsMap);
    static bool draw2QPainterID003(QPainter& painter, std::map<QString, QVariant>& paramsMap);
    static bool draw2QPainterID004(QPainter& painter, std::map<QString, QVariant>& paramsMap);
    static bool draw2QPainterID005(QPainter& painter, std::map<QString, QVariant>& paramsMap);
    static bool draw2QPainterID006(QPainter& painter, std::map<QString, QVariant>& paramsMap);
    static bool draw2QPainterID007(QPainter& painter, std::map<QString, QVariant>& paramsMap);
    static bool draw2QPainterID008(QPainter& painter, std::map<QString, QVariant>& paramsMap);
    static bool draw2QPainterID009(QPainter& painter, std::map<QString, QVariant>& paramsMap);
    static bool draw2QPainterID010(QPainter& painter, std::map<QString, QVariant>& paramsMap);
    static bool draw2QPainterID011(QPainter& painter, std::map<QString, QVariant>& paramsMap);
    static bool draw2QPainterID012(QPainter& painter, std::map<QString, QVariant>& paramsMap);
    static bool draw2QPainterID013(QPainter& painter, std::map<QString, QVariant>& paramsMap);
    static bool draw2QPainterID014(QPainter& painter, std::map<QString, QVariant>& paramsMap);

    static bool drawRPolylineID000(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap);
    static bool drawRPolylineID001(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap);
    static bool drawRPolylineID002(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap);
    static bool drawRPolylineID003(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap);
    static bool drawRPolylineID004(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap);
    static bool drawRPolylineID005(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap);
    static bool drawRPolylineID006(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap);
    static bool drawRPolylineID007(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap);
    static bool drawRPolylineID008(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap);
    static bool drawRPolylineID009(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap);
    static bool drawRPolylineID010(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap);
    static bool drawRPolylineID011(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap);
    static bool drawRPolylineID012(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap);
    static bool drawRPolylineID013(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap);
    static bool drawRPolylineID014(RDocument* pDocument, RMixedOperation* op, const RVector& offset, double dDepth, bool bRotate, std::map<QString, QVariant>& paramsMap);
};

#endif // DISSIMILARLIBRARYOPERATOR_H

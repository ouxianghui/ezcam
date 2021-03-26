#ifndef EZNAVIGATIONACTION_H
#define EZNAVIGATIONACTION_H

#include <QWidget>
#include <QPanGesture>
#include <QPinchGesture>
#include <QCursor>
#include "RActionAdapter.h"
#include "RMouseEvent.h"
#include "RGraphicsView.h"
#include "RGuiAction.h"
#include "RGraphicsView.h"
#include "RRulerQt.h"
#include "RWheelEvent.h"


class EZNavigationAction : public RActionAdapter
{
public:
    enum WheelBehavior
    {
        Zoom = 0,
        Pan
    };

    EZNavigationAction(QWidget* m_pWidget = NULL, RGuiAction* guiAction = NULL);
    ~EZNavigationAction();

    virtual void setGraphicsView(RGraphicsView* pGraphicsView);
    virtual void keyPressEvent(QKeyEvent& event);
    virtual void mousePressEvent(RMouseEvent& event);
    virtual void mouseReleaseEvent(RMouseEvent& event);
    virtual void mouseMoveEvent(RMouseEvent& event);
    virtual void wheelEvent(RWheelEvent& event);
    virtual void panGestureEvent(QPanGesture& gesture);
    virtual void pinchGestureEvent(QPinchGesture& gesture);

private:
    bool m_isPanning;
    RVector m_panOrigin;
    RVector m_panOffset;
    QWidget* m_pWidget;
    RGraphicsView* m_pGraphicsView;
    QCursor m_savedCursor;
    QCursor m_lastCursor;
    RRulerQt* m_pHRuler;
    RRulerQt* m_pVRuler;

    WheelBehavior m_wheelBehavior;
    bool m_isPanGesture;

};

#endif // EZNAVIGATIONACTION_H

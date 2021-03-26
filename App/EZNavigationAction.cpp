#include "EZNavigationAction.h"

EZNavigationAction::EZNavigationAction(QWidget* pWidget, RGuiAction* guiAction)
    : RActionAdapter(guiAction),
      m_pWidget(pWidget),
      m_isPanning(false),
      m_pGraphicsView(NULL),
      m_pHRuler(NULL),
      m_pVRuler(NULL)
{
    m_panOrigin = RVector();

    if (m_pWidget)
    {
        m_pHRuler = pWidget->findChild<RRulerQt*>("HorizontalRuler");
        m_pVRuler = pWidget->findChild<RRulerQt*>("VerticalRuler");
    }

    m_wheelBehavior = Zoom;
    m_isPanGesture = false;
    m_panOffset = RVector(0, 0);
}

EZNavigationAction::~EZNavigationAction()
{

}

void EZNavigationAction::setGraphicsView(RGraphicsView* pGraphicsView)
{
    m_pGraphicsView = pGraphicsView;
}

void EZNavigationAction::keyPressEvent(QKeyEvent& event)
{
    if (m_pGraphicsView == NULL)
    {
        return;
    }

    RVector v;

    if (event.isAccepted())
    {
        // oxh todo
        //return;
    }

    if (event.key() == Qt::Key_Left)
    {
        v = RVector(50, 0);
    }
    else if (event.key() == Qt::Key_Right)
    {
        v = RVector(-50, 0);
    }
    else if (event.key() == Qt::Key_Up)
    {
        v = RVector(0, 50);
    }
    else if (event.key() == Qt::Key_Down)
    {
        v = RVector(0, -50);
    }

    if (v.isValid())
    {
        m_pGraphicsView->pan(v);
        event.accept();
    }
}

void EZNavigationAction::mousePressEvent(RMouseEvent& event)
{
    if (m_pGraphicsView == NULL)
    {
        return;
    }

    if (event.button() == Qt::MidButton ||
            ( event.button() == Qt::LeftButton && event.modifiers() == Qt::ControlModifier))
    {
        m_panOrigin = event.getScreenPosition();
        m_isPanning = true;
        m_savedCursor = m_pGraphicsView->getCursor();
        m_pGraphicsView->setCursor(QCursor(Qt::OpenHandCursor));
        event.getGraphicsView().startPan();
    }
}

void EZNavigationAction::mouseReleaseEvent(RMouseEvent& event)
{
    if (m_pGraphicsView == NULL)
    {
        return;
    }

    if (event.button() == Qt::MidButton || event.button() == Qt::LeftButton)
    {
        m_isPanning = false;
        m_pGraphicsView->setCursor(m_savedCursor);
    }
}

void EZNavigationAction::mouseMoveEvent(RMouseEvent& event)
{
    if (m_pGraphicsView == NULL)
    {
        return;
    }

    if (m_isPanning && ( event.buttons() == Qt::MidButton ||
                         ( event.buttons() == Qt::LeftButton && event.modifiers() == Qt::ControlModifier )))
    {

        RVector panTarget = event.getScreenPosition();
        RVector panDelta = panTarget - m_panOrigin;
        if (fabs(panDelta.x) > 2 || fabs(panDelta.y) > 2)
        {
            m_pGraphicsView->setCursor(Qt::ClosedHandCursor);
            event.getGraphicsView().pan(panDelta);
            m_panOrigin = panTarget;
        }
    }

    if (m_pHRuler)
    {
        m_pHRuler->update();
        m_pHRuler->updateCoordinate(m_pGraphicsView->getDocumentInterface());
    }

    if (m_pVRuler)
    {
        m_pVRuler->update();
        m_pVRuler->updateCoordinate(m_pGraphicsView->getDocumentInterface());
    }
}

void EZNavigationAction::wheelEvent(RWheelEvent& event)
{
    if (m_pGraphicsView == NULL)
    {
        return;
    }

    int wheelDelta = event.delta();

    switch (event.modifiers())
    {

    // scroll up / down:
    case Qt::ControlModifier:
        m_pGraphicsView->pan(RVector(0, wheelDelta / 2));
        m_pGraphicsView->simulateMouseMoveEvent();
        break;

        // scroll left / right:
    case Qt::ShiftModifier:
        m_pGraphicsView->pan(RVector(wheelDelta / 2, 0));
        m_pGraphicsView->simulateMouseMoveEvent();
        break;

        // zoom in / out:
    case Qt::NoModifier:
        // wheel behavior is "zoom":
        if (m_wheelBehavior == Zoom)
        {
            RVector position = event.getModelPosition();
            if (wheelDelta > 0)
            {
                m_pGraphicsView->zoomIn(position);
            }
            else if (wheelDelta < 0)
            {
                m_pGraphicsView->zoomOut(position);
            }
        }

        // wheel behavior is "pan":
        else
        {
            if (event.orientation() == Qt::Vertical)
            {
                m_panOffset = m_panOffset + RVector(0, wheelDelta/2);
            }
            else
            {
                m_panOffset = m_panOffset + RVector(wheelDelta/2, 0);
            }

            // TODO: limit check to wheel events if possible:
            if (QCoreApplication::hasPendingEvents())
            {
                return;
            }
            else
            {
                m_pGraphicsView->pan(m_panOffset);
                m_panOffset = RVector();
            }
        }
        m_pGraphicsView->simulateMouseMoveEvent();
        break;
    }
}

void EZNavigationAction::panGestureEvent(QPanGesture& gesture)
{
    if (m_isPanGesture == false)
    {
        return;
    }

    if (m_pGraphicsView == NULL)
    {
        return;
    }

    switch (gesture.state())
    {
    case Qt::GestureStarted:
        m_lastCursor = m_pGraphicsView->getCursor();
        m_pGraphicsView->setCursor(QCursor(Qt::OpenHandCursor));
        break;

    case Qt::GestureUpdated:
        m_pGraphicsView->setCursor(QCursor(Qt::ClosedHandCursor));
        break;

    default:
        m_pGraphicsView->setCursor(m_lastCursor);
        break;
    }

    QPointF delta = gesture.delta();
    m_pGraphicsView->pan(RVector(delta.x(), delta.y()));
    m_pGraphicsView->simulateMouseMoveEvent();
}

void EZNavigationAction::pinchGestureEvent(QPinchGesture& gesture)
{
    if (m_pGraphicsView == NULL)
    {
        return;
    }

    QPinchGesture::ChangeFlags changeFlags = gesture.changeFlags();
    RVector center = m_pGraphicsView->getLastKnownMousePosition();

    if (!center.isValid())
    {
        return;
    }

    if (changeFlags & QPinchGesture::RotationAngleChanged)
    {
        // rotation does nothing
    }
    if (changeFlags & QPinchGesture::ScaleFactorChanged)
    {
        bool ok = false;
        double value = gesture.property("scaleFactor").toDouble(&ok);
        if (!ok || value <= 0.0)
        {
            return;
        }
        m_pGraphicsView->setCurrentStepScaleFactor(value);

        RVector offset = m_pGraphicsView->getOffset(false);
        RVector newOffset = offset - (center * (m_pGraphicsView->getCurrentStepScaleFactor()) - center);

        newOffset = newOffset / (m_pGraphicsView->getCurrentStepScaleFactor());

        // avoid erratic behavior:
        if (offset.getDistanceTo(newOffset) < 10*m_pGraphicsView->mapDistanceFromView(m_pGraphicsView->getWidth()))
        {
            m_pGraphicsView->setCurrentStepOffset(newOffset - (m_pGraphicsView->getOffset(false)));
        }
    }

    if (gesture.state() == Qt::GestureFinished)
    {
        m_pGraphicsView->setFactor(m_pGraphicsView->getFactor(true));
        m_pGraphicsView->setOffset(m_pGraphicsView->getOffset(true));

        m_pGraphicsView->setCurrentStepScaleFactor(1.0);
        m_pGraphicsView->setCurrentStepOffset(RVector(0,0));
    }

    m_pGraphicsView->simulateMouseMoveEvent();
    m_pGraphicsView->regenerate(true);
}

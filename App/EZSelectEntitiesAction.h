#ifndef EZSELECTENTITIESACTION_H
#define EZSELECTENTITIESACTION_H

#include "RAction.h"
#include "RActionAdapter.h"
#include "RGuiAction.h"
#include "RDocument.h"
#include "RDocumentInterface.h"

class EZSelectEntitiesAction : public RActionAdapter
{
public:

    enum State
    {
       Neutral = 0,
       Dragging,
       SettingCorner2
   };

   EZSelectEntitiesAction(RGuiAction* pGuiAction);
   ~EZSelectEntitiesAction();

   virtual void beginEvent();
   virtual void suspendEvent();
   virtual void resumeEvent();
   virtual void mouseMoveEvent(RMouseEvent& event);
   virtual void mouseReleaseEvent(RMouseEvent& event);
   virtual void mousePressEvent(RMouseEvent& event);
   virtual void mouseDoubleClickEvent(RMouseEvent& event);
   virtual void escapeEvent();
   virtual void coordinateEvent(RCoordinateEvent& event);
   virtual void coordinateEventPreview(RCoordinateEvent& event);
   virtual void keyPressEvent(QKeyEvent& event);
   virtual void keyReleaseEvent(QKeyEvent& event);

protected:
   void previewSelectionBox(RDocumentInterface* di, RBox box, bool crossSelection);
   void previewSelectionPolygon(RDocumentInterface* di, const QList<RVector>& points, bool crossSelection);
   void setState(State state);
   void highlightEntity(REntity::Id& entityId);
   void highlightReferencePoint(const RVector& referencePoint);
   void deselectEntity(REntity::Id& entityId);
   void selectEntity(REntity::Id& entityId, bool add);
   void entityDoubleClicked(REntity::Id& entityId, RMouseEvent& event);
   void pickCoordinate(RCoordinateEvent& event, bool preview);

private:
   int m_nPickRangePixels;
   int m_nMinPickRangePixels;
   RVector m_d1Model;
   RVector m_d1Screen;
   RVector m_d2Model;
   RVector m_d2Screen;
   RDocument* m_pDocument;
   State m_state;
};

#endif // EZSELECTENTITIESACTION_H

#ifndef EZMODIFYCUTTINGPOINTACTION_H
#define EZMODIFYCUTTINGPOINTACTION_H

#include "RActionAdapter.h"
#include "RAction.h"
#include "RGuiAction.h"
#include "RVector.h"
#include "RCoordinateEvent.h"
#include "ROperation.h"
#include "RPointEntity.h"
#include "RMouseEvent.h"
#include "RAddObjectsOperation.h"
#include "BoardRectTable.h"

class EZModifyCuttingPointAction : public RActionAdapter
{
public:
    enum State
    {
        NoState = -1,
        SettingPosition = 0
    };

    EZModifyCuttingPointAction(const QString& strSourceBoardID, RGuiAction* pGuiAction = NULL);
    ~EZModifyCuttingPointAction();

    virtual void beginEvent();
    virtual void mouseReleaseEvent(RMouseEvent& event);
    virtual void mousePressEvent(RMouseEvent& event);
    virtual void escapeEvent();
    virtual void coordinateEvent(RCoordinateEvent& event);
    virtual void coordinateEventPreview(RCoordinateEvent& event);
    virtual void updatePreview(bool clear);
    virtual void applyOperation();

protected:
    void initState();
    void setState(State state);
    void simulateMouseMoveEvent();
    void pickCoordinate(RCoordinateEvent& event, bool preview);
    RAddObjectsOperation* getOperation();

private:
    int m_nPickRangePixels;
    RVector m_point;
    State m_state;
    QString m_strSourceBoardID;
    double m_dSourceBoardW;
    double m_dSourceBoardH;
    std::vector<BoardRectSt> m_vecBoardRectSt;
};

#endif // EZMODIFYCUTTINGPOINTACTION_H

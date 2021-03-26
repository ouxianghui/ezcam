#ifndef EZEDITNCORDERACTION_H
#define EZEDITNCORDERACTION_H

#include <vector>
#include "Typedef.h"
#include "RActionAdapter.h"
#include "RAction.h"
#include "RGuiAction.h"
#include "RVector.h"
#include "RCoordinateEvent.h"
#include "ROperation.h"
#include "RPointEntity.h"
#include "RMouseEvent.h"
#include "SourceBoardTable.h"
#include "BoardRectTable.h"

class EZEditNCOrderAction : public RActionAdapter
{
public:
    enum State
    {
        NoState = -1,
        SettingPosition = 0
    };

    EZEditNCOrderAction(const QString& strSourceBoardID, RGuiAction* pGuiAction = NULL);
    ~EZEditNCOrderAction();

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
    ROperation* getOperation();

    RVector getSuitableCutPoint(const RBox& box, double dOffset);

private:
    int m_nNcOrder;
    int m_nPickRangePixels;
    RVector m_point;
    State m_state;
    QString m_strSourceBoardID;
    double m_dSourceBoardW;
    double m_dSourceBoardH;
    std::vector<BoardRectSt> m_vecBoardRectSt;
    QSet<REntity::Id> m_targetIdSet;
    std::vector<RectF> m_vecDeleteRectF;
    std::vector<RectF> m_vecAllRectF;
    int m_nBoardSpacing;
};

#endif // EZEDITNCORDERACTION_H

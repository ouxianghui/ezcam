#ifndef EZPRINTLABELACTION_H
#define EZPRINTLABELACTION_H

#include "RAction.h"
#include "RActionAdapter.h"
#include "RGuiAction.h"

class EZPrintLabelAction : public RActionAdapter
{
public:
    EZPrintLabelAction(const QString& strSourceBoardID, RGuiAction* pGuiAction);
    ~EZPrintLabelAction();

protected:
    virtual void beginEvent();

private:
    QString m_strSourceBoardID;
};

#endif // EZPRINTLABELACTION_H

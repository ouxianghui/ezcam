#ifndef EZEXPORTNCACTION_H
#define EZEXPORTNCACTION_H

#include "RAction.h"
#include "RActionAdapter.h"
#include "RGuiAction.h"
#include "RPolyline.h"
#include "RPolylineData.h"
#include "RPolylineEntity.h"

class EZExportNCAction : public RActionAdapter
{
public:
    EZExportNCAction(const QString& strSourceBoardID, RGuiAction* pGuiAction);
    ~EZExportNCAction();

protected:
    virtual void beginEvent();

private:
    QString m_strSourceBoardID;
};

#endif // EZEXPORTNCACTION_H

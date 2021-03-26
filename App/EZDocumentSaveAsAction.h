#ifndef EZDOCUMENTSAVEASACTION_H
#define EZDOCUMENTSAVEASACTION_H

#include "RAction.h"
#include "RActionAdapter.h"
#include "RGuiAction.h"

class EZDocumentSaveAsAction : public RActionAdapter
{
public:
    EZDocumentSaveAsAction(RGuiAction* pGuiAction);
    ~EZDocumentSaveAsAction();

protected:
    virtual void beginEvent();
};

#endif // EZDOCUMENTSAVEASACTION_H

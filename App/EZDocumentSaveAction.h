#ifndef EZDOCUMENTSAVEACTION_H
#define EZDOCUMENTSAVEACTION_H

#include "RAction.h"
#include "RActionAdapter.h"
#include "RGuiAction.h"

class EZDocumentSaveAction : public RActionAdapter
{
public:
    EZDocumentSaveAction(RGuiAction* pGuiAction);
    ~EZDocumentSaveAction();

protected:
    virtual void beginEvent();
};

#endif // EZDOCUMENTSAVEACTION_H

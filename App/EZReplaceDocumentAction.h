#ifndef EZREPLACEDOCUMENTACTION_H
#define EZREPLACEDOCUMENTACTION_H

#include "RAction.h"
#include "RActionAdapter.h"
#include "RGuiAction.h"

class EZReplaceDocumentAction : public RActionAdapter
{
public:
    EZReplaceDocumentAction(RGuiAction* pGuiAction);
    ~EZReplaceDocumentAction();

protected:
    virtual void beginEvent();
};

#endif // EZREPLACEDOCUMENTACTION_H

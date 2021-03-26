#include "EZResetAction.h"
#include "RDocumentInterface.h"
#include "RSnapFree.h"

EZResetAction::EZResetAction(RGuiAction* pGuiAction)
    :RActionAdapter(pGuiAction)
{

}

EZResetAction::~EZResetAction()
{

}

void EZResetAction::beginEvent()
{
    if (!documentInterface)
    {
        return;
    }

    documentInterface->killAllActions();
    documentInterface->deselectAll();
    documentInterface->setSnap(new RSnapFree());
    this->terminate();
}

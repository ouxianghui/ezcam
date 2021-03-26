#ifndef EZRESETACTION_H
#define EZRESETACTION_H

#include "RAction.h"
#include "RActionAdapter.h"
#include "RGuiAction.h"

class EZResetAction : public RActionAdapter
{
public:
    EZResetAction(RGuiAction* pGuiAction = NULL);
    ~EZResetAction();

    virtual void beginEvent();
};

#endif // EZRESETACTION_H

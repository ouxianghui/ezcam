#ifndef EZEXPORTBITMAPACTION_H
#define EZEXPORTBITMAPACTION_H

#include "RAction.h"
#include "RActionAdapter.h"
#include "RGuiAction.h"

class EZExportBitmapAction : public RActionAdapter
{
public:
    EZExportBitmapAction(RGuiAction* pGuiAction);
    ~EZExportBitmapAction();

protected:
    virtual void beginEvent();
    QString getFileName();
    QString getSaveFileName(QWidget* parentWidget, const QString& caption, const QStringList& filterStrings);
};

#endif // EZEXPORTBITMAPACTION_H

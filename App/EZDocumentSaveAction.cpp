#include "EZDocumentSaveAction.h"
#include "RDocument.h"
#include "RDocumentInterface.h"
#include <QMessageBox>

EZDocumentSaveAction::EZDocumentSaveAction(RGuiAction* pGuiAction)
    : RActionAdapter(pGuiAction)
{

}

EZDocumentSaveAction::~EZDocumentSaveAction()
{

}

void EZDocumentSaveAction::beginEvent()
{
    if (!documentInterface)
    {
        return;
    }

    RDocument& document = documentInterface->getDocument();

    if (document.isModified())
    {
        QString strFileName = document.getFileName();

        QString strFileVersion = document.getFileVersion();

        strFileVersion = strFileVersion.isEmpty() ? "" : strFileVersion;

        strFileName = documentInterface->getCorrectedFileName(strFileName, strFileVersion);

        if (!documentInterface->exportFile(strFileName, strFileVersion))
        {
            QString strMsg = QString("File %1 has not been saved.").arg(strFileName);
            qWarning(strMsg.toStdString().c_str());
        }
    }

    this->terminate();
}



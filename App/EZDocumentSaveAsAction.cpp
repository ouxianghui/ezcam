#include "EZDocumentSaveAsAction.h"
#include "RDocument.h"
#include "RDocumentInterface.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QTextCodec>
#include "SystemConfigManager.h"

EZDocumentSaveAsAction::EZDocumentSaveAsAction(RGuiAction* pGuiAction)
    : RActionAdapter(pGuiAction)
{

}

EZDocumentSaveAsAction::~EZDocumentSaveAsAction()
{

}

void EZDocumentSaveAsAction::beginEvent()
{
    if (!documentInterface)
    {
        return;
    }

    QTextCodec *tc = QTextCodec::codecForName("GBK");

    SystemConfigManager& sMgr = SystemConfigManager::GetInstance();
    QString strDir = sMgr.getRecentlyUsedDxfFileDir();
    strDir = strDir + "/untitled.dxf";
    QString strSaveFilePathName = QFileDialog::getSaveFileName(NULL, tc->toUnicode("保存DXF文件"),
                                                               strDir,
                                                               "Dxf Files (*.dxf)");
    if (!strSaveFilePathName.isEmpty() && !strSaveFilePathName.isNull())
    {
        RDocument& document = documentInterface->getDocument();

        //QString strFileName = document.getFileName();

        QString strFileVersion = document.getFileVersion();

        strFileVersion = strFileVersion.isEmpty() ? "" : strFileVersion;

        //strFileName = documentInterface->getCorrectedFileName(strFileName, strFileVersion);

        if (!documentInterface->exportFile(strSaveFilePathName, strFileVersion))
        {
            QString strMsg = QString("File %1 has not been saved.").arg(strSaveFilePathName);
            qWarning(strMsg.toStdString().c_str());
        }

        QFileInfo inf(strSaveFilePathName);
        sMgr.setRecentlyUsedDxfFileDir(inf.path());
    }

    this->terminate();
}



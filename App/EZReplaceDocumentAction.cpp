#include "EZReplaceDocumentAction.h"
#include "RDocument.h"
#include "RDocumentInterface.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextCodec>
#include "SystemConfigManager.h"

EZReplaceDocumentAction::EZReplaceDocumentAction(RGuiAction* pGuiAction)
    : RActionAdapter(pGuiAction)
{

}

EZReplaceDocumentAction::~EZReplaceDocumentAction()
{

}

void EZReplaceDocumentAction::beginEvent()
{
    if (!documentInterface)
    {
        return;
    }

    SystemConfigManager& sMgr = SystemConfigManager::GetInstance();

    QTextCodec *tc = QTextCodec::codecForName("GBK");

    QString strNewFilePathName = QFileDialog::getOpenFileName(NULL, tc->toUnicode("打开DXF文件"),
                                                              sMgr.getRecentlyUsedDxfFileDir(),
                                                              "Dxf Files (*.dxf)");
    if (!strNewFilePathName.isEmpty())
    {
        QFileInfo fileInfo(strNewFilePathName);
        if (fileInfo.exists() && fileInfo.isFile())
        {
            RDocument& document = documentInterface->getDocument();

            QString strCurrentFilePathName = document.getFileName();

            QString strFileVersion = document.getFileVersion();

            strFileVersion = strFileVersion.isEmpty() ? "" : strFileVersion;

            strCurrentFilePathName = documentInterface->getCorrectedFileName(strCurrentFilePathName, strFileVersion);

            QFileInfo info(strCurrentFilePathName);
            QString strCurrentFileName = info.fileName();
            QString strPath = info.path();

            // 先备份文件
            strCurrentFileName.prepend("~");
            QString strBackupFilePathName = strPath + "/" + strCurrentFileName;

            // 导出当前文件，作为备份文件
            if (documentInterface->exportFile(strBackupFilePathName, strFileVersion))
            {
                documentInterface->clear();

                // 引入用户选择的文件
                if (documentInterface->importFile(strNewFilePathName, "(*.dxf)") == RDocumentInterface::IoErrorNoError)
                {
                    documentInterface->regenerateScenes();
                    documentInterface->autoZoom();

                    QFileInfo inf(strNewFilePathName);
                    sMgr.setRecentlyUsedDxfFileDir(inf.path());

                    // 将用户选择的文件保存为原来的文件，即替换
                    if (!documentInterface->exportFile(strCurrentFilePathName, strFileVersion))
                    {
                        QString strMsg = QString("File %1 has not been saved.").arg(strCurrentFilePathName);
                        qWarning(strMsg.toStdString().c_str());
                    }
                }
                else
                {
                    QString strMsg = QString("Import file %1 failed.").arg(strNewFilePathName);
                    qWarning(strMsg.toStdString().c_str());
                }
            }
            else
            {
                QString strMsg = QString("File %1 has not been saved.").arg(strBackupFilePathName);
                qWarning(strMsg.toStdString().c_str());
            }
        }
    }

    this->terminate();
}

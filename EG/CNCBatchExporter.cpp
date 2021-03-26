#include "CNCBatchExporter.h"
#include <QException>
#include <QMessageBox>
#include <QTextCodec>
#include <QFileDialog>
#include <QProgressDialog>
#include "ProjectConfigManager.h"
#include "SystemConfigManager.h"
#include "DatabaseManager.h"
#include "MaterialListTable.h"
#include "MaterialGroupTable.h"
#include "CuttingSolutionTable.h"
#include "CNCGenerator.h"
#include "SourceBoardTable.h"
#include "Typedef.h"
#include "RDocument.h"
#include "RDxfImporter.h"
#include "RMemoryStorage.h"
#include "RSpatialIndexNavel.h"

CNCBatchExporter::CNCBatchExporter()
{

}

CNCBatchExporter::~CNCBatchExporter()
{

}

bool CNCBatchExporter::exportCNC(const QString& strNCExportDir)
{
    try
    {
        if (strNCExportDir.isEmpty() || strNCExportDir.isNull())
        {
            return false;
        }

        QTextCodec *tc = QTextCodec::codecForName("GBK");

        // 打开工程配置文件
        ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();

        // 获取数据库文件
        QString strDatabaseFile = QString("%1/%2.db").arg(mgr.getProjectPath()).arg(mgr.getProjectName());
        QFileInfo dbFileInfo(strDatabaseFile);
        if (dbFileInfo.exists() && dbFileInfo.isFile())
        {
            CDatabaseManager& dbMgr = CDatabaseManager::GetInstance();
            dbMgr.SetDriverName("QSQLITE");
            dbMgr.SetDatabasePathName(strDatabaseFile);

            std::map<QString, std::vector<DataSt>> materialMap;

            // 从数据库获取料单数据
            if (MaterialListTable::getMaterialLists(materialMap))
            {
                SystemConfigManager& smgr = SystemConfigManager::GetInstance();

                double dSourceBoardW = smgr.getSourceMaterialBoardWidth();
                double dSourceBoardH = smgr.getSourceMaterialBoardHeight();

                int nCuttingSpeed = smgr.getCuttingSpeed();
                int nProcessSpeed = smgr.getProcessingSpeed();
                int nTerminalSpped = smgr.getTerminalSpeed();

                double dSafetyHeight = smgr.getSafetyHeight();
                double dCuttingDepth = smgr.getCuttingDepth();
                double dKnifeDiameter = smgr.getKinfeDiameter();
                int nUseDrills = smgr.getUseDrills();
                QString strSlotKnifeType = smgr.getSlotKnifeType();

                QMap<QString, QVariant> drillsMap;
                smgr.getDrills(drillsMap);

                QMap<QString, QVariant> holesMap;
                smgr.getHoles(holesMap);

                QString strKnifeChangeMode = smgr.getKnifeChangeMode();
                QString strNCFileHeader = smgr.getNCFileHeader();
                QString strNCFileTail = smgr.getNCFileTail();

                CNCGenerator generator(nCuttingSpeed,
                                       nProcessSpeed,
                                       nTerminalSpped,
                                       dSafetyHeight,
                                       dCuttingDepth,
                                       dKnifeDiameter,
                                       strSlotKnifeType,
                                       nUseDrills,
                                       drillsMap,
                                       holesMap,
                                       dSourceBoardW,
                                       dSourceBoardH,
                                       strKnifeChangeMode,
                                       strNCFileHeader,
                                       strNCFileTail);

                QString strDxfDir = QString("%1/%2").arg(mgr.getProjectPath()).arg("dxf");

                std::vector<NCExporterSt> vecNCExporterSt;

                // 从数据库获取各个料单数据
                for (std::map<QString, std::vector<DataSt>>::iterator it = materialMap.begin(); it != materialMap.end(); ++it)
                {
                    std::vector<DataSt>& vecData = it->second;

                    QString strMateriaFileName = it->first;

                    std::map<QString, DataSt> boardMap;
                    std::vector<DataSt>::iterator dataStIt = vecData.begin();
                    for (; dataStIt != vecData.end(); ++dataStIt)
                    {
                        DataSt& st = *dataStIt;
                        boardMap[st.strBarCodeNo] = st;
                    }

                    if (vecData.size() > 0)
                    {
                        // 取出料单ID
                        QString strMaterialListID = vecData.at(0).strMaterialListID;

                        // 从数据库获取各个料单分组
                        std::vector<MaterialGroupSt> vecGroup;
                        if (MaterialGroupTable::getGroupsByMaterialListID(strMaterialListID, vecGroup))
                        {
                            for (std::vector<MaterialGroupSt>::iterator mgIt = vecGroup.begin(); mgIt != vecGroup.end(); ++mgIt)
                            {
                                MaterialGroupSt& mgSt = *mgIt;

                                // 添加方案子节点
                                std::vector<CuttingSolutionSt> vecCsSt;
                                if (CuttingSolutionTable::getSolutionsByMaterialGroupID(mgSt.strMaterialGroupID, vecCsSt))
                                {
                                    for (std::vector<CuttingSolutionSt>::iterator csIt = vecCsSt.begin(); csIt != vecCsSt.end(); ++ csIt)
                                    {
                                        CuttingSolutionSt& csSt = *csIt;

                                        // 准备相应方案的板块用料数据
                                        Solution solution;
                                        solution.solutionSt = csSt;
                                        if (SourceBoardTable::getBoardsBySolutionID(csSt.strSolutionID, solution.vecSourceBoard))
                                        {
                                            std::vector<SourceBoardSt>::iterator sbIt = solution.vecSourceBoard.begin();
                                            for (; sbIt != solution.vecSourceBoard.end(); ++sbIt)
                                            {
                                                SourceBoardSt& sbSt = *sbIt;

                                                NCExporterSt ncSt;
                                                ncSt.strMaterialFileName = strMateriaFileName;
                                                ncSt.groupSt = mgSt;
                                                ncSt.solutionSt = csSt;
                                                ncSt.BoardSt = sbSt;
                                                vecNCExporterSt.push_back(ncSt);
                                            }
                                        }
                                        else
                                        {
                                            qCritical(tc->toUnicode("读取数据库失败: 获取不到原料板数据.").toStdString().c_str());
                                            return false;
                                        }
                                    }
                                }
                                else
                                {
                                    qCritical(tc->toUnicode("读取数据库失败: 获取不到方案数据.").toStdString().c_str());
                                    return false;
                                }
                            }
                        }
                        else
                        {
                            qCritical(tc->toUnicode("读取数据库失败: 获取不到料单分组.").toStdString().c_str());
                            return false;
                        }
                    }
                }

                int nMaximum = vecNCExporterSt.size();
                QString strMsg = QString(tc->toUnicode("正在导出NC文件 ..."));
                QProgressDialog dlgProgress(NULL);
                dlgProgress.setWindowTitle(tc->toUnicode("导出NC文件"));
                dlgProgress.setWindowModality(Qt::WindowModal);
                dlgProgress.setLabelText(strMsg);
                dlgProgress.setRange(0, nMaximum);
                dlgProgress.setCancelButtonText(tc->toUnicode("取消"));

                std::vector<NCExporterSt>::iterator ncIt = vecNCExporterSt.begin();
                for (int i = 0; ncIt != vecNCExporterSt.end(); ++ncIt, ++i)
                {
                    NCExporterSt& ncSt = *ncIt;

                    dlgProgress.setValue(i);

                    strMsg = QString(tc->toUnicode("正在生成第 %1 张料板的NC文件 ...")).arg(i+1);
                    dlgProgress.setLabelText(strMsg);

                    if (dlgProgress.wasCanceled())
                    {
                        break;
                    }

                    QString strFilePathName = QString("%1/%2").arg(strDxfDir).arg(ncSt.BoardSt.strDxfFileName);

                    RMemoryStorage* pStorage = new RMemoryStorage();
                    RSpatialIndexNavel* pSpatialIndex = new RSpatialIndexNavel();
                    RDocument document(*pStorage, *pSpatialIndex);

                    RDxfImporter dxfImpoter(document);
                    if (dxfImpoter.importFile(strFilePathName, ""))
                    {
                        QString strSaveFilePathName = QString("%1/%2-%3-%4-%5-x%6.nc")
                                .arg(strNCExportDir)
                                .arg(ncSt.strMaterialFileName)
                                .arg(ncSt.groupSt.strMaterialGroupName)
                                .arg(ncSt.solutionSt.strSolutionName)
                                .arg(ncSt.BoardSt.nIdx)
                                .arg(ncSt.BoardSt.nNum);
                        //if (!generator.generateCNC(document, ncSt.BoardSt.strBoardID, strSaveFilePathName))
                        //{
                        //    qCritical(tc->toUnicode("生成CNC文件失败.").toStdString().c_str());
                        //    return false;
                        //}
                        QString strError;
                        int nRet = generator.generateCNC(document, ncSt.BoardSt.strBoardID, strSaveFilePathName);
                        if (1 == nRet)
                        {
                            strError = QString(tc->toUnicode("第 %1 张料板生成CNC文件失败: 无法在折线中插入下刀点, 因为原折线不是闭合的.")).arg(i+1);
                            QMessageBox::warning(NULL, tc->toUnicode("警告"), strError, QMessageBox::Ok);
                            return false;
                        }
                        else if (2 == nRet)
                        {
                            strError = QString(tc->toUnicode("第 %1 张料板生成CNC文件失败: 无法在折线中插入下刀点, 因为原折线线段数量小于3.")).arg(i+1);
                            QMessageBox::warning(NULL, tc->toUnicode("警告"), strError, QMessageBox::Ok);
                            return false;
                        }
                        else if (3 == nRet)
                        {
                            strError = QString(tc->toUnicode("第 %1 张料板生成CNC文件失败: 无法在折线中插入下刀点, 因为下刀点不在折线上.")).arg(i+1);
                            QMessageBox::warning(NULL, tc->toUnicode("警告"), strError, QMessageBox::Ok);
                            return false;
                        }
                        else if (4 == nRet)
                        {
                            strError = QString(tc->toUnicode("第 %1 张料板生成CNC文件失败: 无法在折线中插入下刀点, 因为不允许下刀点在圆弧上.")).arg(i+1);
                            QMessageBox::warning(NULL, tc->toUnicode("警告"), strError, QMessageBox::Ok);
                            return false;
                        }
                        else if (5 == nRet)
                        {
                            strError = QString(tc->toUnicode("第 %1 张料板生成CNC文件失败: 未知错误.")).arg(i+1);
                            QMessageBox::warning(NULL, tc->toUnicode("警告"), strError, QMessageBox::Ok);
                            return false;
                        }
                        else if (6 == nRet)
                        {
                            strError = QString(tc->toUnicode("第 %1 张料板生成CNC文件失败: 动态转换实体失败.")).arg(i+1);
                            QMessageBox::warning(NULL, tc->toUnicode("警告"), strError, QMessageBox::Ok);
                            return false;
                        }
                        else if (7 == nRet)
                        {
                            strError = QString(tc->toUnicode("第 %1 张料板生成CNC文件失败: 因为拉槽刀具T2无法加工拉槽.")).arg(i+1);
                            QMessageBox::critical(NULL, tc->toUnicode("错误"), strError, QMessageBox::Ok);
                            return false;
                        }
                        else if (8 == nRet)
                        {
                            strError = QString(tc->toUnicode("第 %1 张料板生成CNC文件失败: 因为拉槽刀具T3无法加工拉槽.")).arg(i+1);
                            QMessageBox::critical(NULL, tc->toUnicode("错误"), strError, QMessageBox::Ok);
                            return false;
                        }
                        else if (9 == nRet)
                        {
                            strError = QString(tc->toUnicode("第 %1 张料板生成CNC文件失败: 因为无法打开一个nc文件用以写入G代码.")).arg(i+1);
                            QMessageBox::critical(NULL, tc->toUnicode("错误"), strError, QMessageBox::Ok);
                            return false;
                        }
                    }
                    else
                    {
                        qCritical(tc->toUnicode("读取dxf文件失败.").toStdString().c_str());
                        return false;
                    }
                }
                dlgProgress.setValue(nMaximum);
                strMsg = QString(tc->toUnicode("已完成"));
                dlgProgress.setLabelText(strMsg);
            }
            else
            {
                qCritical(tc->toUnicode("读取数据库失败: 获取不到料单数据.").toStdString().c_str());
                return false;
            }
        }
        else
        {
            qCritical(tc->toUnicode("读取数据库失败: 无效数据库文件或数据库文件不存在.").toStdString().c_str());
            return false;
        }

        return true;
    }
    catch (QException& e)
    {
        qCritical("CNCBatchExporter::exportCNC() exception: %s", e.what());
        return false;
    }
}


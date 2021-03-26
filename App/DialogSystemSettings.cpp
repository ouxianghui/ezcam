#include "DialogSystemSettings.h"
#include "ui_DialogSystemSettings.h"
#include <QValidator>
#include <QRegExpValidator>
#include <QMessageBox>
#include <QException>
#include <QTextCodec>
#include <QTableWidgetItem>
#include <QMap>
#include "ProjectConfigManager.h"
#include "SystemConfigManager.h"
#include "DatabaseManager.h"
#include "MaterialListTable.h"
#include "CuttingSolutionTable.h"
#include "SourceBoardTable.h"
#include "ProductBoardTable.h"
#include "MaterialGroupTable.h"
#include "Global.h"

DialogSystemSettings::DialogSystemSettings(WorkspaceTreeWidget* pWorkSpaceTreeWidget, std::map<QString, std::map<QString, std::vector<DataSt>>>& materialMap, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSystemSettings),
    m_pWorkSpaceTreeWidget(pWorkSpaceTreeWidget),
    m_materialMap(materialMap)
{
    ui->setupUi(this);

    //正整数正则表达式
    QRegExp regx("(^[0-9]*[1-9][0-9]*$)");
    QValidator *validator = new QRegExpValidator(regx, this);
    ui->lineEditMaterialWidth->setValidator(validator);
    ui->lineEditMaterialTrimming->setValidator(validator);
    ui->lineEditMaterialHeight->setValidator(validator);
    ui->lineEditBoardSpacing->setValidator(validator);
    ui->lineEditCuttingDepth->setValidator(validator);

    ui->lineEditProcessingSpeed->setValidator(validator);
    ui->lineEditCuttingSpeed->setValidator(validator);
    ui->lineEditTerminalVelocity->setValidator(validator);

    //正浮点数正则表达式
    QRegExp dRegx("([0-9]+(\\.[0-9]+)?)");
    QValidator *pValidator = new QRegExpValidator(dRegx, this);
    ui->lineEditSafetyHeight->setValidator(pValidator);
    ui->lineEditKnifeDiameter->setValidator(pValidator);
    ui->lineEditCuttingDepth->setValidator(pValidator);
    ui->lineEditObliqueDistance->setValidator(pValidator);

    SystemConfigManager& sysMgr = SystemConfigManager::GetInstance();

    if (sysMgr.getSplitMaterialsListByType() == 1)
    {
        ui->checkBoxSplitByType->setChecked(true);
    }
    else
    {
        ui->checkBoxSplitByType->setChecked(false);
    }

    if (sysMgr.getSplitMaterialsListByThickness() == 1)
    {
        ui->checkBoxSplitByThickness->setChecked(true);
    }
    else
    {
        ui->checkBoxSplitByThickness->setChecked(false);
    }

    ui->lineEditMaterialWidth->setText(QString("%1").arg(sysMgr.getSourceMaterialBoardWidth()));
    ui->lineEditMaterialHeight->setText(QString("%1").arg(sysMgr.getSourceMaterialBoardHeight()));
    ui->lineEditMaterialTrimming->setText(QString("%1").arg(sysMgr.getSourceMaterialBoardTrimming()));
    ui->lineEditBoardSpacing->setText(QString("%1").arg(sysMgr.getBoardSpacing()));
    ui->lineEditCuttingDepth->setText(QString("%1").arg(sysMgr.getCuttingDepth()));
    ui->lineEditKnifeDiameter->setText(QString("%1").arg(sysMgr.getKinfeDiameter()));


    if (sysMgr.getSlotKnifeType() == "T1")
    {
        ui->radioButtonT1->setChecked(true);
        ui->radioButtonT2->setChecked(false);
        ui->radioButtonT3->setChecked(false);
    }
    else if (sysMgr.getSlotKnifeType() == "T2")
    {
        ui->radioButtonT1->setChecked(false);
        ui->radioButtonT2->setChecked(true);
        ui->radioButtonT3->setChecked(false);
    }
    else if (sysMgr.getSlotKnifeType() == "T3")
    {
        ui->radioButtonT1->setChecked(false);
        ui->radioButtonT2->setChecked(false);
        ui->radioButtonT3->setChecked(true);
    }

    ui->lineEditProcessingSpeed->setText(QString("%1").arg(sysMgr.getProcessingSpeed()));
    ui->lineEditCuttingSpeed->setText(QString("%1").arg(sysMgr.getCuttingSpeed()));
    ui->lineEditTerminalVelocity->setText(QString("%1").arg(sysMgr.getTerminalSpeed()));
    ui->lineEditSafetyHeight->setText(QString("%1").arg(sysMgr.getSafetyHeight()));
    ui->lineEditObliqueDistance->setText(QString("%1").arg(sysMgr.getObliqueDistance()));
    ui->textEditKnifeChangeMode->setText(sysMgr.getKnifeChangeMode());
    ui->textEditNCFileHeader->setText(sysMgr.getNCFileHeader());
    ui->textEditNCFileTail->setText(sysMgr.getNCFileTail());

    ui->comboBoxMaxSolutionNum->insertItem(0, "1", 1);
    ui->comboBoxMaxSolutionNum->insertItem(1, "2", 2);
    ui->comboBoxMaxSolutionNum->insertItem(2, "3", 3);
    ui->comboBoxMaxSolutionNum->insertItem(3, "4", 4);
    ui->comboBoxMaxSolutionNum->insertItem(4, "5", 5);
    ui->comboBoxMaxSolutionNum->setCurrentIndex(sysMgr.getMaxSolutionNum()-1);

    if (sysMgr.getUseDrills() == 1)
    {
        ui->checkBoxUseDrills->setChecked(true);
        ui->tableWidgetDrills->setEnabled(true);
    }
    else
    {
        ui->checkBoxUseDrills->setChecked(false);
        ui->tableWidgetDrills->setEnabled(false);
    }
}

DialogSystemSettings::~DialogSystemSettings()
{
    delete ui;
}

void DialogSystemSettings::on_checkBoxSplitByType_clicked()
{

}

void DialogSystemSettings::on_checkBoxSplitByThickness_clicked()
{

}

void DialogSystemSettings::on_pushButtonOK_clicked()
{
    ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();
    SystemConfigManager& sysMgr = SystemConfigManager::GetInstance();

    if (mgr.isProjectOpened())
    {
        bool ok = false;
        QString str = ui->lineEditMaterialWidth->text();
        int nVal = str.toInt(&ok);
        if (ok)
        {
            sysMgr.setSourceMaterialBoardWidth(nVal);
        }

        ok = false;
        str = ui->lineEditMaterialHeight->text();
        nVal = str.toInt(&ok);
        if (ok)
        {
            sysMgr.setSourceMaterialBoardHeight(nVal);
        }

        ok = false;
        str = ui->lineEditMaterialTrimming->text();
        nVal = str.toInt(&ok);
        if (ok)
        {
            sysMgr.setSourceMaterialBoardTrimming(nVal);
        }

        ok = false;
        str = ui->lineEditBoardSpacing->text();
        nVal = str.toInt(&ok);
        if (ok)
        {
            sysMgr.setBoardSpacing(nVal);
        }

        ok = false;
        str = ui->lineEditCuttingDepth->text();
        double dVal = str.toDouble(&ok);
        if (ok)
        {
            sysMgr.setCuttingDepth(dVal);
        }

        sysMgr.setMaxSolutionNum(ui->comboBoxMaxSolutionNum->currentIndex()+1);

        ok = false;
        str = ui->lineEditKnifeDiameter->text();
        dVal = str.toDouble(&ok);
        if (ok)
        {
            sysMgr.setKinfeDiameter(dVal);
        }

        if (ui->checkBoxUseDrills->isChecked())
        {
            sysMgr.setUseDrills(1);
        }
        else
        {
            sysMgr.setUseDrills(0);
        }

        if (ui->radioButtonT1->isChecked())
        {
            sysMgr.setSlotKnifeType("T1");
        }
        else if (ui->radioButtonT2->isChecked())
        {
            sysMgr.setSlotKnifeType("T2");
        }
        else if (ui->radioButtonT3->isChecked())
        {
            sysMgr.setSlotKnifeType("T3");
        }

        ok = false;
        str = ui->lineEditProcessingSpeed->text();
        nVal = str.toInt(&ok);
        if (ok)
        {
            sysMgr.setProcessingSpeed(nVal);
        }

        ok = false;
        str = ui->lineEditCuttingSpeed->text();
        nVal = str.toInt(&ok);
        if (ok)
        {
            sysMgr.setCuttingSpeed(nVal);
        }

        ok = false;
        str = ui->lineEditTerminalVelocity->text();
        nVal = str.toInt(&ok);
        if (ok)
        {
            sysMgr.setTerminalSpeed(nVal);
        }

        ok = false;
        str = ui->lineEditSafetyHeight->text();
        dVal = str.toDouble(&ok);
        if (ok)
        {
            sysMgr.setSafetyHeight(dVal);
        }

        ok = false;
        str = ui->lineEditObliqueDistance->text();
        dVal = str.toDouble(&ok);
        if (ok)
        {
            sysMgr.setObliqueDistance(dVal);
        }

        sysMgr.setKnifeChangeMode(ui->textEditKnifeChangeMode->toPlainText());
        sysMgr.setNCFileHeader(ui->textEditNCFileHeader->toPlainText());
        sysMgr.setNCFileTail(ui->textEditNCFileTail->toPlainText());

        QTextCodec *tc = QTextCodec::codecForName("GBK");

        int nSplitByType = ui->checkBoxSplitByType->isChecked() ? 1 : 0;
        int nSplitByThickness = ui->checkBoxSplitByThickness->isChecked() ? 1 : 0;
        if (nSplitByType != sysMgr.getSplitMaterialsListByType()
                || nSplitByThickness != sysMgr.getSplitMaterialsListByThickness())
        {
            if (QMessageBox::Yes == QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("将重新建立分组和方案，是否继续?"), QMessageBox::Yes, QMessageBox::No))
            {
                if (ui->checkBoxSplitByType->isChecked())
                {
                    sysMgr.setSplitMaterialsListByType(1);
                }
                else
                {
                    sysMgr.setSplitMaterialsListByType(0);
                }

                if (ui->checkBoxSplitByThickness->isChecked())
                {
                    sysMgr.setSplitMaterialsListByThickness(1);
                }
                else
                {
                    sysMgr.setSplitMaterialsListByThickness(0);
                }

                // 先删除工作目录树
                removeWorkspaceTree();

                // 删除当前分组数据
                m_materialMap.clear();

                std::map<QString, std::vector<DataSt>> materialListMap;

                // 从数据库获取各个料单数据
                if (MaterialListTable::getMaterialLists(materialListMap))
                {
                    std::map<QString, std::vector<DataSt>>::iterator it = materialListMap.begin();
                    for (; it != materialListMap.end(); ++it)
                    {
                        const QString& strKey = it->first;
                        std::vector<DataSt>& vecData = it->second;
                        if (vecData.size() > 0)
                        {
                            QString strMaterialListID = vecData.at(0).strMaterialListID;

                            // 按材料类型、厚度拆分料单
                            splitMaterialList(strKey, vecData);

                            QTreeWidgetItem* pRootItem = m_pWorkSpaceTreeWidget->topLevelItem(0);

                            if (pRootItem)
                            {
                                // 添加料单名作为一级子节点
                                QTreeWidgetItem* pMaterialListItem = new QTreeWidgetItem();
                                pMaterialListItem->setData(0, ITEM_TYPE, ITEM_TYPE_MATERIAL_LIST);
                                pMaterialListItem->setData(0, MATERIAL_LIST_ID, strMaterialListID);
                                pMaterialListItem->setData(0, MATERIAL_LIST_NAME, strKey);
                                pMaterialListItem->setData(0, XBEL_TARGET, "folder");
                                pMaterialListItem->setText(0, strKey);
                                pRootItem->addChild(pMaterialListItem);

                                std::map<QString, std::vector<DataSt>>& materialGroupMap = m_materialMap[strKey];

                                // 添加分组子节点
                                std::map<QString, std::vector<DataSt>>::iterator mgIt = materialGroupMap.begin();
                                for (; mgIt != materialGroupMap.end(); ++mgIt)
                                {
                                    MaterialGroupSt groupSt;
                                    groupSt.strMaterialGroupName = mgIt->first;
                                    groupSt.strMaterialListID = strMaterialListID;
                                    if (MaterialGroupTable::addGroup(groupSt))
                                    {
                                        // 添加分组子节点
                                        QTreeWidgetItem* pMaterialGroupItem = new QTreeWidgetItem();
                                        pMaterialGroupItem->setData(0, ITEM_TYPE, ITEM_TYPE_MATERIAL_GROUP);
                                        pMaterialGroupItem->setData(0, MATERIAL_GROUP_ID, QVariant(groupSt.strMaterialGroupID));
                                        pMaterialGroupItem->setData(0, MATERIAL_GROUP_NAME, QVariant(groupSt.strMaterialGroupName));
                                        pMaterialGroupItem->setData(0, XBEL_TARGET, "folder");
                                        pMaterialGroupItem->setText(0, groupSt.strMaterialGroupName);
                                        pMaterialListItem->addChild(pMaterialGroupItem);
                                    }
                                    else
                                    {
                                        qCritical("Add material group to database failed.\n");
                                    }
                                }

                                pRootItem->addChild(pMaterialListItem);
                            }
                        }
                    }
                }
            }
            else
            {
            }
        }

        //QMap<QString, QVariant> map;
        //QString key;
        //QVariant val;

        //for (int r = 0; r < ui->tableWidgetDrills->rowCount(); ++r)
        //{
        //    for (int c = 0; c < ui->tableWidgetDrills->columnCount(); ++c)
        //    {
        //        if (c == 0)
        //        {
        //            QTableWidgetItem* item = ui->tableWidgetDrills->item(r, c);

        //            key = item->text();
        //        }
        //        else if (c == 1)
        //        {
        //            QTableWidgetItem* item = ui->tableWidgetDrills->item(r, c);
        //            bool ok = false;
        //            double value = item->text().toDouble(&ok);
        //            if (!ok)
        //            {
        //                value = 0.0;
        //            }
        //            val = value;
        //        }
        //    }

        //    map[key] = val;
        //}

        //sysMgr.setDrills(map);
    }

    QDialog::accept();
}

void DialogSystemSettings::on_pushButtonCancel_clicked()
{
    QDialog::reject();
}

void DialogSystemSettings::removeWorkspaceTree()
{
    QTreeWidgetItem* pRootItem = m_pWorkSpaceTreeWidget->topLevelItem(0);
    if (pRootItem)
    {
        int nMaterialListNum = pRootItem->childCount();
        for (int i = 0; i < nMaterialListNum; ++i)
        {
            QTreeWidgetItem* pMaterialListItem = pRootItem->child(0);
            if (pMaterialListItem)
            {
                int nGroupNum = pMaterialListItem->childCount();
                for (int j = 0; j < nGroupNum; ++j)
                {
                    QTreeWidgetItem* pGroupItem = pMaterialListItem->child(0);
                    if (pGroupItem)
                    {
                        int nSolutionNum = pGroupItem->childCount();
                        for (int n = 0; n < nSolutionNum; ++n)
                        {
                            QTreeWidgetItem* pSolutionItem = pGroupItem->child(0);
                            if (pSolutionItem)
                            {
                                QString strSolutionID = pSolutionItem->data(0, CUTTING_SOLUTION_ID).toString();
                                if (!strSolutionID.isEmpty())
                                {
                                    if (SourceBoardTable::deleteBoardsBySolutionID(strSolutionID))
                                    {
                                    }
                                }
                                pGroupItem->removeChild(pSolutionItem);
                                delete pSolutionItem;
                                pSolutionItem = NULL;
                            }
                        }

                        QString strGroupID = pGroupItem->data(0, MATERIAL_GROUP_ID).toString();
                        if (!strGroupID.isEmpty())
                        {
                            if (CuttingSolutionTable::deleteSolutionByMaterialGroupID(strGroupID))
                            {
                            }
                        }
                        pMaterialListItem->removeChild(pGroupItem);
                        delete pGroupItem;
                        pGroupItem = NULL;
                    }
                }

                QString strMaterialListID = pMaterialListItem->data(0, MATERIAL_LIST_ID).toString();
                if (!strMaterialListID.isEmpty())
                {
                    if (MaterialGroupTable::deleteGroupByMaterialListID(strMaterialListID))
                    {
                    }
                }
                pRootItem->removeChild(pMaterialListItem);
                delete pMaterialListItem;
                pMaterialListItem = NULL;
            }
        }
    }
}

void DialogSystemSettings::splitMaterialList(const QString& strKey, const std::vector<DataSt>& vecData)
{
    try
    {
        if (m_materialMap.find(strKey) != m_materialMap.end())
        {
            return;
        }

        SystemConfigManager& sysMgr = SystemConfigManager::GetInstance();

        std::map<QString, std::vector<DataSt>> subMaterialMap;

        subMaterialMap.clear();

        // 按材料类型和厚度拆分料单
        if (sysMgr.getSplitMaterialsListByType() == 1 && sysMgr.getSplitMaterialsListByThickness() == 1)
        {
            // 先按材料类型分类
            std::map<QString, std::vector<DataSt>> typeMap;
            for (std::vector<DataSt>::const_iterator it = vecData.begin(); it != vecData.end(); ++it)
            {
                // 在类型map中不存在，先添加一个map再向map写记录，否则直接向已经存在的map中写记录
                if (typeMap.find(it->strMaterialType) != typeMap.end())
                {
                    std::vector<DataSt>& vecData = typeMap[it->strMaterialType];
                    vecData.push_back(*it);
                }
                else
                {
                    typeMap[it->strMaterialType] = std::vector<DataSt>();
                    std::vector<DataSt>& vecData = typeMap[it->strMaterialType];
                    vecData.push_back(*it);
                }
            }

            // 接下来按材料厚度分类
            std::map<QString, std::vector<DataSt>>::iterator it = typeMap.begin();
            for (; it != typeMap.end(); ++it)
            {
                // 从按材料类型分类的结果中依次取出std::vector<DataSt>进行按材料厚度分类
                std::vector<DataSt>& vecDataSt = it->second;
                for (std::vector<DataSt>::iterator vecIt = vecDataSt.begin(); vecIt != vecDataSt.end(); ++vecIt)
                {
                    // 在厚度map中不存在，先添加一个map再向map写记录，否则直接向已经存在的map中写记录
                    // 材料名称+材料厚度作为key
                    QString strKey = QString("%1-%2mm").arg(it->first).arg(vecIt->nBoardThickness);
                    if (subMaterialMap.find(strKey) != subMaterialMap.end())
                    {
                        std::vector<DataSt>& vecData = subMaterialMap[strKey];
                        vecData.push_back(*vecIt);
                    }
                    else
                    {
                        subMaterialMap[strKey] = std::vector<DataSt>();
                        std::vector<DataSt>& vecData = subMaterialMap[strKey];
                        vecData.push_back(*vecIt);
                    }
                }
            }
        }
        // 按材料类型拆分料单
        else if (sysMgr.getSplitMaterialsListByType() == 1 && sysMgr.getSplitMaterialsListByThickness() == 0)
        {
            for (std::vector<DataSt>::const_iterator it = vecData.begin(); it != vecData.end(); ++it)
            {
                // 在类型map中不存在，先添加一个map再向map写记录，否则直接向已经存在的map中写记录
                if (subMaterialMap.find(it->strMaterialType) != subMaterialMap.end())

                {
                    std::vector<DataSt>& vecData = subMaterialMap[it->strMaterialType];
                    vecData.push_back(*it);
                }
                else
                {
                    subMaterialMap[it->strMaterialType] = std::vector<DataSt>();
                    std::vector<DataSt>& vecData = subMaterialMap[it->strMaterialType];
                    vecData.push_back(*it);
                }
            }
        }
        // 按材料厚度拆分料单
        else if (sysMgr.getSplitMaterialsListByType() == 0 && sysMgr.getSplitMaterialsListByThickness() == 1)
        {
            for (std::vector<DataSt>::const_iterator it = vecData.begin(); it != vecData.end(); ++it)
            {
                // 在类型map中不存在，先添加一个map再向map写记录，否则直接向已经存在的map中写记录
                // 材料厚度作为key
                QString strKey = QString("%1mm").arg(it->nBoardThickness);
                if (subMaterialMap.find(strKey) != subMaterialMap.end())
                {
                    std::vector<DataSt>& vecData = subMaterialMap[strKey];
                    vecData.push_back(*it);
                }
                else
                {
                    subMaterialMap[strKey] = std::vector<DataSt>();
                    std::vector<DataSt>& vecData = subMaterialMap[strKey];
                    vecData.push_back(*it);
                }
            }
        }
        // 不进行料单拆分
        else if (sysMgr.getSplitMaterialsListByType() == 0 && sysMgr.getSplitMaterialsListByThickness() == 0)
        {
            ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();

            QString strKey = mgr.getProjectName();
            for (std::vector<DataSt>::const_iterator it = vecData.begin(); it != vecData.end(); ++it)
            {
                // 在类型map中不存在，先添加一个map再向map写记录，否则直接向已经存在的map中写记录
                // 材料厚度作为key
                if (subMaterialMap.find(strKey) != subMaterialMap.end())
                {
                    std::vector<DataSt>& vecData = subMaterialMap[strKey];
                    vecData.push_back(*it);
                }
                else
                {
                    subMaterialMap[strKey] = std::vector<DataSt>();
                    std::vector<DataSt>& vecData = subMaterialMap[strKey];
                    vecData.push_back(*it);
                }
            }
        }

        m_materialMap[strKey] = subMaterialMap;
    }
    catch (QException& e)
    {
        qCritical("MainWindow::splitMaterialList() exception: %s", e.what());
    }
}

bool DialogSystemSettings::materialBoardSizeTest(int nMaxWidth, int nMaxHeight, int nSpacing, const std::vector<DataSt>& vecData)
{
    bool isLeagl = true;

    std::vector<DataSt>::const_iterator it = vecData.begin();
    for (; it != vecData.end(); ++it)
    {
        const DataSt& st = *it;

        // 小板件的宽、高都小于最大值才合法，否则，看该小板件是否允许旋转
        if (st.nBoardWidth+nSpacing > nMaxWidth || st.nBoardHeight+nSpacing > nMaxHeight)
        {
            // 旋转后是否合法
            if (st.nRotate == 1)
            {
                if (st.nBoardWidth+nSpacing > nMaxHeight || st.nBoardHeight+nSpacing > nMaxWidth)
                {
                    isLeagl = false;
                    break;
                }
            }
            else
            {
                isLeagl = false;
                break;
            }
        }
    }

    return isLeagl;
}

void DialogSystemSettings::on_pushButtonBoardsSizeTest_clicked()
{
    ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();
    if (mgr.isProjectOpened() == true)
    {
        QTextCodec *tc = QTextCodec::codecForName("GBK");

        bool ok = false;
        QString str = ui->lineEditMaterialWidth->text();
        if (str.isEmpty())
        {
            QMessageBox::warning(this, tc->toUnicode("警告"), tc->toUnicode("参数:'料板宽度'为空."), tc->toUnicode("知道了"));
            return;
        }
        int nSourceBoardWidth = ui->lineEditMaterialWidth->text().toInt(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, tc->toUnicode("警告"), tc->toUnicode("非法参数:'料板宽度'."), tc->toUnicode("知道了"));
            return;
        }

        str = ui->lineEditMaterialHeight->text();
        if (str.isEmpty())
        {
            QMessageBox::warning(this, tc->toUnicode("警告"), tc->toUnicode("参数:'料板高度'为空."), tc->toUnicode("知道了"));
            return;
        }
        int nSourceBoardHeight = ui->lineEditMaterialHeight->text().toInt(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, tc->toUnicode("警告"), tc->toUnicode("非法参数:'料板高度'."), tc->toUnicode("知道了"));
            return;
        }

        str = ui->lineEditMaterialTrimming->text();
        if (str.isEmpty())
        {
            QMessageBox::warning(this, tc->toUnicode("警告"), tc->toUnicode("参数:'料板修边'为空."), tc->toUnicode("知道了"));
            return;
        }
        int nBoardTrimming = ui->lineEditMaterialTrimming->text().toInt(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, tc->toUnicode("警告"), tc->toUnicode("非法参数:'料板修边'."), tc->toUnicode("知道了"));
            return;
        }

        str = ui->lineEditBoardSpacing->text();
        if (str.isEmpty())
        {
            QMessageBox::warning(this, tc->toUnicode("警告"), tc->toUnicode("参数:'板件间距'为空."), tc->toUnicode("知道了"));
            return;
        }
        int nBoardSpacing = ui->lineEditBoardSpacing->text().toInt(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, tc->toUnicode("警告"), tc->toUnicode("非法参数:'板件间距'."), tc->toUnicode("知道了"));
            return;
        }

        int nMaxWidth = 0;
        int nMaxHeight = 0;

        double dSpacing = nBoardSpacing;
        double dTrimming = nBoardTrimming;

        // 调整大料板宽度
        nMaxWidth = nSourceBoardWidth + (dSpacing/2.0-dTrimming)*2;
        nMaxHeight = nSourceBoardHeight + (dSpacing/2.0-dTrimming)*2;

        std::map<QString, std::vector<DataSt>> materialMap;
        if (MaterialListTable::getMaterialLists(materialMap))
        {
            std::map<QString, std::vector<DataSt>>::iterator mapIt = materialMap.begin();
            for (; mapIt != materialMap.end(); ++mapIt)
            {
                if (!materialBoardSizeTest(nMaxWidth, nMaxHeight, nBoardSpacing, mapIt->second))
                {
                    QMessageBox::warning(this, tc->toUnicode("警告"), tc->toUnicode("不合法的板件尺寸或切割参数."), tc->toUnicode("知道了"));
                    return;
                }
            }
        }
        QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("板件尺寸、切割参数均合法."), tc->toUnicode("知道了"));
    }
}

void DialogSystemSettings::on_checkBoxUseDrills_clicked()
{
    if (ui->checkBoxUseDrills->isChecked())
    {
        ui->tableWidgetDrills->setEnabled(true);
    }
    else
    {
        ui->tableWidgetDrills->setDisabled(true);
    }
}

#include "WorkspaceTreeWidget.h"
#include <QException>
#include <QMessageBox>
#include <QTextCodec>
#include "ProjectConfigManager.h"
#include "DatabaseManager.h"
#include "MaterialListTable.h"
#include "CuttingSolutionTable.h"
#include "SourceBoardTable.h"
#include "BoardRectTable.h"
#include "ProductBoardTable.h"
#include "MaterialGroupTable.h"
#include "Global.h"

WorkspaceTreeWidget::WorkspaceTreeWidget(QWidget *parent)
    : QTreeWidget(parent),
      m_pMenu(NULL),
      m_pActionDeleteMaterialList(NULL),
      m_pActionDeleteMaterialGroup(NULL),
      m_pActionDeleteCuttingSolution(NULL)
{
    createWidgets();
}

WorkspaceTreeWidget::~WorkspaceTreeWidget()
{

}

void WorkspaceTreeWidget::createWidgets()
{
    QTextCodec *tc = QTextCodec::codecForName("GBK");

    m_pMenu = new QMenu(this);
    m_pActionDeleteMaterialList = new QAction(QIcon(":/icons/delete_list_128.png"), tc->toUnicode("删除料单"), this);
    //m_pActionDeleteMaterialGroup = new QAction(QIcon(":/icons/delete_solution_128.png"), tc->toUnicode("删除分组"), this);
    m_pActionDeleteCuttingSolution = new QAction(QIcon(":/icons/delete_solution_128.png"), tc->toUnicode("删除方案"), this);
    connect(m_pActionDeleteMaterialList, &QAction::triggered, this, &WorkspaceTreeWidget::onActionDeleteMaterialList);
    //connect(m_pActionDeleteMaterialGroup, &QAction::triggered, this, &WorkspaceTreeWidget::onActionDeleteMaterialGroup);
    connect(m_pActionDeleteCuttingSolution, &QAction::triggered, this, &WorkspaceTreeWidget::onActionDeleteCuttingSolution);
    m_pMenu->addAction(m_pActionDeleteMaterialList);
    //m_pMenu->addAction(m_pActionDeleteMaterialGroup);
    m_pMenu->addAction(m_pActionDeleteCuttingSolution);
}

void WorkspaceTreeWidget::onActionDeleteMaterialList(bool)
{
    try
    {
        QTreeWidgetItem* pMaterialListItem = this->currentItem();
        if (pMaterialListItem)
        {
            QTextCodec *tc = QTextCodec::codecForName("GBK");

            bool ok = false;
            int nItemType = pMaterialListItem->data(0, ITEM_TYPE).toInt(&ok);
            if (ok)
            {
                if (nItemType == ITEM_TYPE_MATERIAL_LIST)
                {
                    QString strMaterialListName = pMaterialListItem->data(0, MATERIAL_LIST_NAME).toString();
                    if (!strMaterialListName.isEmpty())
                    {
                        QString strMsg = QString(tc->toUnicode("此操作将删除料单 '%1' 以及所有分组，是否继续？")).arg(strMaterialListName);
                        if (QMessageBox::No == QMessageBox::warning(this, tc->toUnicode("警告"), strMsg, QMessageBox::Yes, QMessageBox::No))
                        {
                            return;
                        }
                    }
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
                                        std::vector<SourceBoardSt> vecSbSt;
                                        if (SourceBoardTable::getBoardsBySolutionID(strSolutionID, vecSbSt))
                                        {
                                            std::vector<SourceBoardSt>::iterator sbIt = vecSbSt.begin();
                                            for (; sbIt != vecSbSt.end(); ++sbIt)
                                            {
                                                SourceBoardSt& sbSt = *sbIt;
                                                if (BoardRectTable::deleteBoardRectsBySourceBoardID(sbSt.strBoardID))
                                                {

                                                }
                                            }
                                            if (SourceBoardTable::deleteBoardsBySolutionID(strSolutionID))
                                            {
                                                pGroupItem->removeChild(pSolutionItem);
                                                delete pSolutionItem;
                                                pSolutionItem = NULL;
                                            }
                                        }
                                    }
                                }
                            }

                            QString strGroupID = pGroupItem->data(0, MATERIAL_GROUP_ID).toString();
                            if (!strGroupID.isEmpty())
                            {
                                if (CuttingSolutionTable::deleteSolutionByMaterialGroupID(strGroupID))
                                {
                                    pMaterialListItem->removeChild(pGroupItem);
                                    delete pGroupItem;
                                    pGroupItem = NULL;
                                }
                            }
                        }
                    }

                    QString strMaterialListID = pMaterialListItem->data(0, MATERIAL_LIST_ID).toString();
                    if (!strMaterialListID.isEmpty())
                    {
                        if (MaterialGroupTable::deleteGroupByMaterialListID(strMaterialListID))
                        {
                            if (MaterialListTable::deletetMaterialListByID(strMaterialListID))
                            {
                                pMaterialListItem->parent()->removeChild(pMaterialListItem);
                                delete pMaterialListItem;
                                pMaterialListItem = NULL;

                                // 将新建工程时拷贝进来的料单文件删除
                                ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();
                                QString strMaterialFilePathName = QString("%1/%2/%3.xls").arg(mgr.getProjectPath()).arg(tr("material")).arg(strMaterialListName);
                                QFile file(strMaterialFilePathName);

                                if (file.exists())
                                {
                                    file.remove();
                                }

                                // 告知外部料单已经删除，工程中的相关数据可以根据料单名称同步删除
                                emit deleteMaterialList(strMaterialListName);
                            }
                        }
                    }
                }
            }
        }
    }
    catch (QException& e)
    {
        qCritical("WorkspaceTreeWidget::onActionDeleteMaterialList() exception: %s", e.what());
    }
}

void WorkspaceTreeWidget::onActionDeleteMaterialGroup(bool)
{
    try
    {
        QTreeWidgetItem* pGroupItem = this->currentItem();
        if (pGroupItem)
        {
            QTextCodec *tc = QTextCodec::codecForName("GBK");

            bool ok = false;
            int nItemType = pGroupItem->data(0, ITEM_TYPE).toInt(&ok);
            if (ok)
            {
                if (nItemType == ITEM_TYPE_MATERIAL_GROUP)
                {
                    QString strGroupName = pGroupItem->data(0, MATERIAL_GROUP_NAME).toString();
                    if (!strGroupName.isEmpty())
                    {
                        QString strMsg = QString(tc->toUnicode("此操作将删除分组 '%1'，是否继续？")).arg(strGroupName);
                        if (QMessageBox::No == QMessageBox::warning(this, tc->toUnicode("警告"), strMsg, QMessageBox::Yes, QMessageBox::No))
                        {
                            return;
                        }
                    }

                    int nSolutionNum = pGroupItem->childCount();
                    for (int n = 0; n < nSolutionNum; ++n)
                    {
                        QTreeWidgetItem* pSolutionItem = pGroupItem->child(0);
                        if (pSolutionItem)
                        {
                            QString strSolutionID = pSolutionItem->data(0, CUTTING_SOLUTION_ID).toString();
                            if (!strSolutionID.isEmpty())
                            {
                                std::vector<SourceBoardSt> vecSbSt;
                                if (SourceBoardTable::getBoardsBySolutionID(strSolutionID, vecSbSt))
                                {
                                    std::vector<SourceBoardSt>::iterator sbIt = vecSbSt.begin();
                                    for (; sbIt != vecSbSt.end(); ++sbIt)
                                    {
                                        SourceBoardSt& sbSt = *sbIt;
                                        if (BoardRectTable::deleteBoardRectsBySourceBoardID(sbSt.strBoardID))
                                        {

                                        }
                                    }
                                    if (SourceBoardTable::deleteBoardsBySolutionID(strSolutionID))
                                    {
                                        pGroupItem->removeChild(pSolutionItem);
                                        delete pSolutionItem;
                                        pSolutionItem = NULL;
                                    }
                                }
                            }
                        }
                    }

                    QString strGroupID = pGroupItem->data(0, MATERIAL_GROUP_ID).toString();
                    if (!strGroupID.isEmpty())
                    {
                        if (CuttingSolutionTable::deleteSolutionByMaterialGroupID(strGroupID))
                        {
                            if (MaterialGroupTable::deleteGroupByID(strGroupID))
                            {
                                pGroupItem->parent()->removeChild(pGroupItem);
                                delete pGroupItem;
                                pGroupItem = NULL;
                            }
                        }
                    }
                }
            }
        }
    }
    catch (QException& e)
    {
        qCritical("WorkspaceTreeWidget::onActionDeleteMaterialGroup() exception: %s", e.what());
    }
}

void WorkspaceTreeWidget::onActionDeleteCuttingSolution(bool)
{
    try
    {
        QTreeWidgetItem* pSolutionItem = this->currentItem();
        if (pSolutionItem)
        {
            QTextCodec *tc = QTextCodec::codecForName("GBK");

            bool ok = false;
            int nItemType = pSolutionItem->data(0, ITEM_TYPE).toInt(&ok);
            if (ok)
            {
                if (nItemType == ITEM_TYPE_CUTTING_SOLUTION)
                {
                    QString strSolutionName = pSolutionItem->data(0, CUTTING_SOLUTION_NAME).toString();
                    if (!strSolutionName.isEmpty())
                    {
                        QString strMsg = QString(tc->toUnicode("此操作将删除方案 '%1'，是否继续？")).arg(strSolutionName);
                        if (QMessageBox::No == QMessageBox::warning(this, tc->toUnicode("警告"), strMsg, QMessageBox::Yes, QMessageBox::No))
                        {
                            return;
                        }
                    }

                    QString strSolutionID = pSolutionItem->data(0, CUTTING_SOLUTION_ID).toString();
                    if (!strSolutionID.isEmpty())
                    {
                        std::vector<SourceBoardSt> vecSbSt;
                        if (SourceBoardTable::getBoardsBySolutionID(strSolutionID, vecSbSt))
                        {
                            std::vector<SourceBoardSt>::iterator sbIt = vecSbSt.begin();
                            for (; sbIt != vecSbSt.end(); ++sbIt)
                            {
                                SourceBoardSt& sbSt = *sbIt;
                                if (BoardRectTable::deleteBoardRectsBySourceBoardID(sbSt.strBoardID))
                                {

                                }
                            }
                            if (SourceBoardTable::deleteBoardsBySolutionID(strSolutionID))
                            {
                                if (CuttingSolutionTable::deleteSolutionByID(strSolutionID))
                                {
                                    pSolutionItem->parent()->removeChild(pSolutionItem);
                                    delete pSolutionItem;
                                    pSolutionItem = NULL;

                                    emit deleteCuttingSolution(strSolutionID);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    catch (QException& e)
    {
        qCritical("WorkspaceTreeWidget::onActionDeleteCuttingSolution() exception: %s", e.what());
    }
}

void WorkspaceTreeWidget::contextMenuEvent(QContextMenuEvent* event)
{
    if (event != NULL)
    {
        QTreeWidgetItem* pItem = this->itemAt(event->pos());
        if (pItem != NULL && pItem->isSelected())
        {
            bool ok = false;
            int nItemType = pItem->data(0, ITEM_TYPE).toInt(&ok);
            if (ok)
            {
                if (nItemType == ITEM_TYPE_MATERIAL_LIST)
                {
                    m_pActionDeleteMaterialList->setEnabled(true);
                    //m_pActionDeleteMaterialGroup->setEnabled(false);
                    m_pActionDeleteCuttingSolution->setEnabled(false);
                    m_pMenu->exec(event->globalPos());
                }
                /*
                else if (nItemType == ITEM_TYPE_MATERIAL_GROUP)
                {
                    m_pActionDeleteMaterialList->setEnabled(false);
                    m_pActionDeleteMaterialGroup->setEnabled(true);
                    m_pActionDeleteCuttingSolution->setEnabled(false);
                    m_pMenu->exec(event->globalPos());
                }
                */
                else if (nItemType == ITEM_TYPE_CUTTING_SOLUTION)
                {
                    m_pActionDeleteMaterialList->setEnabled(false);
                    //m_pActionDeleteMaterialGroup->setEnabled(false);
                    m_pActionDeleteCuttingSolution->setEnabled(true);
                    m_pMenu->exec(event->globalPos());
                }
            }
        }
    }

    QTreeWidget::contextMenuEvent(event);
}

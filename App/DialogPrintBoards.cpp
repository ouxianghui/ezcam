#include "DialogPrintBoards.h"
#include "ui_DialogPrintBoards.h"
#include <QTreeWidgetItem>
#include <QTextCodec>
#include <QGridLayout>
#include <QFileInfo>
#include <QMessageBox>
#include <QException>
#include <QHeaderView>
#include <QVariant>
#include <QPushButton>
#include <QLabel>
#include "MaterialListTable.h"
#include "MaterialGroupTable.h"
#include "BoardRectTable.h"
#include "SystemConfigManager.h"
#include "ProjectConfigManager.h"
#include "DatabaseManager.h"
#include "DialogPrintPreviewer.h"

static bool boardIndexCmp(BoardSt& boardSt1, BoardSt& boardSt2)
{
    return boardSt1.nBoardIndex < boardSt2.nBoardIndex;
}

static bool sourceBoardIndexCmp(BoardSt& boardSt1, BoardSt& boardSt2)
{
    return boardSt1.nSourceBoardIndex < boardSt2.nSourceBoardIndex;
}

DialogPrintBoards::DialogPrintBoards(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogPrintBoards)
{
    ui->setupUi(this);

    createWidgets();

    readBoardsFromDatabase();
}

DialogPrintBoards::~DialogPrintBoards()
{
    delete ui;
}

void DialogPrintBoards::createWidgets()
{
    QTextCodec *tc = QTextCodec::codecForName("GBK");

    m_pTreeWidget = new QTreeWidget(this);
    connect(m_pTreeWidget, &QTreeWidget::itemChanged, this, &DialogPrintBoards::onTreeWidgetItemChanged);

    QTreeWidgetItem* pHeader = new QTreeWidgetItem();
    pHeader->setTextAlignment(0, Qt::AlignLeft);
    pHeader->setText(0, "");
    m_pTreeWidget->setHeaderItem(pHeader);

    QStringList slHeader;
    slHeader << tc->toUnicode("料单")
             << tc->toUnicode("分组")
             << tc->toUnicode("方案")
             << tc->toUnicode("大板号")
             << tc->toUnicode("小板号")
             << tc->toUnicode("订单号")
             << tc->toUnicode("条码编号")
             << tc->toUnicode("产品名称")
             << tc->toUnicode("成品名称")
             << tc->toUnicode("材料类型")
             << tc->toUnicode("长度")
             << tc->toUnicode("宽度")
             << tc->toUnicode("厚度")
             << tc->toUnicode("数量")
             << tc->toUnicode("封边量")
             << tc->toUnicode("成品长度")
             << tc->toUnicode("成品宽度")
             << tc->toUnicode("成品厚度")
             << tc->toUnicode("异形")
             << tc->toUnicode("旋转")
             << tc->toUnicode("客户信息")
             << tc->toUnicode("加盟店");

    m_pTreeWidget->setHeaderLabels(slHeader);
    m_pTreeWidget->headerItem()->setTextAlignment(0, Qt::AlignCenter);
    m_pTreeWidget->headerItem()->setTextAlignment(1, Qt::AlignCenter);
    m_pTreeWidget->headerItem()->setTextAlignment(2, Qt::AlignCenter);
    m_pTreeWidget->headerItem()->setTextAlignment(3, Qt::AlignCenter);
    m_pTreeWidget->headerItem()->setTextAlignment(4, Qt::AlignCenter);
    m_pTreeWidget->headerItem()->setTextAlignment(5, Qt::AlignCenter);
    m_pTreeWidget->headerItem()->setTextAlignment(6, Qt::AlignCenter);
    m_pTreeWidget->headerItem()->setTextAlignment(7, Qt::AlignCenter);
    m_pTreeWidget->headerItem()->setTextAlignment(8, Qt::AlignCenter);
    m_pTreeWidget->headerItem()->setTextAlignment(9, Qt::AlignCenter);
    m_pTreeWidget->headerItem()->setTextAlignment(10, Qt::AlignCenter);
    m_pTreeWidget->headerItem()->setTextAlignment(11, Qt::AlignCenter);
    m_pTreeWidget->headerItem()->setTextAlignment(12, Qt::AlignCenter);
    m_pTreeWidget->headerItem()->setTextAlignment(13, Qt::AlignCenter);
    m_pTreeWidget->headerItem()->setTextAlignment(14, Qt::AlignCenter);
    m_pTreeWidget->headerItem()->setTextAlignment(15, Qt::AlignCenter);
    m_pTreeWidget->headerItem()->setTextAlignment(16, Qt::AlignCenter);
    m_pTreeWidget->headerItem()->setTextAlignment(17, Qt::AlignCenter);
    m_pTreeWidget->headerItem()->setTextAlignment(18, Qt::AlignCenter);
    m_pTreeWidget->headerItem()->setTextAlignment(19, Qt::AlignCenter);
    m_pTreeWidget->headerItem()->setTextAlignment(20, Qt::AlignCenter);
    m_pTreeWidget->headerItem()->setTextAlignment(21, Qt::AlignCenter);

    m_pTreeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QGridLayout* pGridLayout = new QGridLayout(this);
    this->setLayout(pGridLayout);

    QPushButton* pButtonPrint = new QPushButton(this);
    pButtonPrint->setText(tc->toUnicode("预览/打印"));
    connect(pButtonPrint, &QPushButton::clicked, this, &DialogPrintBoards::onPrintButtonClicked);

    QPushButton* pButtonClose = new QPushButton(this);
    pButtonClose->setText(tc->toUnicode("关闭"));
    connect(pButtonClose, &QPushButton::clicked, this, &DialogPrintBoards::onCloseButtonClicked);

    pGridLayout->addWidget(m_pTreeWidget, 0, 0, 19, 20);
    QHBoxLayout* pHLayout = new QHBoxLayout();
    //QSpacerItem* pItem = new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    pHLayout->addWidget(new QLabel(this), 18);
    pHLayout->addWidget(pButtonPrint, 1);
    pHLayout->addWidget(pButtonClose, 1);

    pGridLayout->addLayout(pHLayout, 19, 0, 1, 20);
}

void DialogPrintBoards::readBoardsFromDatabase()
{
    try
    {
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
                // 从数据库获取各个料单数据
                for (std::map<QString, std::vector<DataSt>>::iterator it = materialMap.begin(); it != materialMap.end(); ++it)
                {
                    const QString& strMaterialListName = it->first;
                    std::vector<DataSt>& vecData = it->second;

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

                        // 按材料类型、厚度拆分料单
                        //splitMaterialList(strMaterialListName, vecData);

                        // 添加料单名作为一级子节点
                        QTreeWidgetItem* pMaterialListItem = new QTreeWidgetItem();
                        pMaterialListItem->setData(0, ITEM_TYPE, ITEM_TYPE_MATERIAL_LIST);
                        pMaterialListItem->setData(0, MATERIAL_LIST_ID, strMaterialListID);
                        pMaterialListItem->setData(0, MATERIAL_LIST_NAME, strMaterialListName);
                        pMaterialListItem->setText(0, strMaterialListName);
                        pMaterialListItem->setTextAlignment(0, Qt::AlignCenter);
                        m_pTreeWidget->addTopLevelItem(pMaterialListItem);

                        pMaterialListItem->setExpanded(true);

                        // 从数据库获取各个料单分组
                        std::vector<MaterialGroupSt> vecGroup;
                        if (MaterialGroupTable::getGroupsByMaterialListID(strMaterialListID, vecGroup))
                        {
                            for (std::vector<MaterialGroupSt>::iterator mgIt = vecGroup.begin(); mgIt != vecGroup.end(); ++mgIt)
                            {
                                MaterialGroupSt& mgSt = *mgIt;

                                // 添加分组子节点
                                QTreeWidgetItem* pMaterialGroupItem = new QTreeWidgetItem();
                                pMaterialGroupItem->setData(0, ITEM_TYPE, ITEM_TYPE_MATERIAL_GROUP);
                                pMaterialGroupItem->setData(0, MATERIAL_GROUP_ID, QVariant(mgSt.strMaterialGroupID));
                                pMaterialGroupItem->setData(0, MATERIAL_GROUP_NAME, QVariant(mgSt.strMaterialGroupName));
                                pMaterialGroupItem->setText(1, mgSt.strMaterialGroupName);
                                pMaterialGroupItem->setTextAlignment(1, Qt::AlignCenter);
                                pMaterialListItem->addChild(pMaterialGroupItem);

                                //pMaterialGroupItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
                                pMaterialGroupItem->setExpanded(true);

                                // 添加方案子节点
                                std::vector<CuttingSolutionSt> vecCsSt;
                                if (CuttingSolutionTable::getSolutionsByMaterialGroupID(mgSt.strMaterialGroupID, vecCsSt))
                                {
                                    for (std::vector<CuttingSolutionSt>::iterator csIt = vecCsSt.begin(); csIt != vecCsSt.end(); ++ csIt)
                                    {
                                        CuttingSolutionSt& csSt = *csIt;
                                        QTreeWidgetItem* pSolutionItem = new QTreeWidgetItem();
                                        pSolutionItem->setData(0, ITEM_TYPE, ITEM_TYPE_CUTTING_SOLUTION);
                                        pSolutionItem->setData(0, CUTTING_SOLUTION_ID, csSt.strSolutionID);
                                        pSolutionItem->setData(0, CUTTING_SOLUTION_NAME, csSt.strSolutionName);
                                        pSolutionItem->setText(2, csSt.strSolutionName);
                                        pSolutionItem->setTextAlignment(2, Qt::AlignCenter);
                                        pMaterialGroupItem->addChild(pSolutionItem);

                                        pSolutionItem->setExpanded(true);

                                        // 准备相应方案的板块用料数据
                                        Solution solution;
                                        solution.solutionSt = csSt;
                                        if (SourceBoardTable::getBoardsBySolutionID(csSt.strSolutionID, solution.vecSourceBoard))
                                        {
                                            std::vector<SourceBoardSt>::iterator sbIt = solution.vecSourceBoard.begin();
                                            for (; sbIt != solution.vecSourceBoard.end(); ++sbIt)
                                            {
                                                SourceBoardSt& sbSt = *sbIt;

                                                QTreeWidgetItem* pSourceBoardItem = new QTreeWidgetItem();
                                                pSourceBoardItem->setData(0, ITEM_TYPE, ITEM_TYPE_SOURCE_BOARD);
                                                pSourceBoardItem->setData(0, SOURCE_BOARD_ID, sbSt.strBoardID);
                                                pSourceBoardItem->setData(0, SOURCE_BOARD_INDEX_ITEM, sbSt.nIdx);
                                                pSourceBoardItem->setText(3, QString("%1 (%2)").arg(sbSt.nIdx).arg(sbSt.nNum));
                                                pSourceBoardItem->setTextAlignment(3, Qt::AlignCenter);

                                                pSolutionItem->addChild(pSourceBoardItem);
                                                pSourceBoardItem->setExpanded(true);
                                                pSourceBoardItem->setCheckState(3, Qt::Unchecked);
                                                std::vector<BoardRectSt> vecRects;
                                                if (BoardRectTable::getBoardRectsBySourceBoardID(sbSt.strBoardID, vecRects))
                                                {
                                                    std::sort(vecRects.begin(), vecRects.end());
                                                    std::vector<BoardRectSt>::iterator brIt = vecRects.begin();
                                                    for (; brIt != vecRects.end(); ++brIt)
                                                    {
                                                        BoardRectSt& brSt = *brIt;
                                                        QTreeWidgetItem* pBoardItem = new QTreeWidgetItem();
                                                        pBoardItem->setData(0, ITEM_TYPE, ITEM_TYPE_BOARD);
                                                        pBoardItem->setData(0, BOARD_ID_ITEM, brSt.strBoardRectID);
                                                        pBoardItem->setData(0, BOARD_INDEX_ITEM, brSt.nIdx);
                                                        pBoardItem->setText(4, QString("%1").arg(brSt.nIdx));
                                                        pBoardItem->setTextAlignment(4, Qt::AlignCenter);

                                                        DataSt& st = boardMap[brSt.strBarcodeNo];

                                                        pBoardItem->setData(0, BOARD_ORDERS_NO_ITEM, st.strOrdersNo);
                                                        pBoardItem->setText(5, st.strOrdersNo);
                                                        pBoardItem->setTextAlignment(5, Qt::AlignCenter);

                                                        pBoardItem->setData(0, BOARD_BARCODE_ITEM, st.strBarCodeNo);
                                                        pBoardItem->setText(6, st.strBarCodeNo);
                                                        pBoardItem->setTextAlignment(6, Qt::AlignCenter);

                                                        pBoardItem->setData(0, BOARD_PRODUCT_NAME_ITEM, st.strProductName);
                                                        pBoardItem->setText(7, st.strProductName);
                                                        pBoardItem->setTextAlignment(7, Qt::AlignCenter);

                                                        pBoardItem->setData(0, BOARD_FINISHED_PRODUCT_NAME_ITEM, st.strFinishedProductName);
                                                        pBoardItem->setText(8, st.strFinishedProductName);
                                                        pBoardItem->setTextAlignment(8, Qt::AlignCenter);

                                                        pBoardItem->setData(0, BOARD_MATERIAL_TYPE_ITEM, st.strMaterialType);
                                                        pBoardItem->setText(9, st.strMaterialType);
                                                        pBoardItem->setTextAlignment(9, Qt::AlignCenter);

                                                        pBoardItem->setData(0, BOARD_HEIGHT_ITEM, st.nBoardHeight);
                                                        pBoardItem->setText(10, QString("%1").arg(st.nBoardHeight));
                                                        pBoardItem->setTextAlignment(11, Qt::AlignCenter);

                                                        pBoardItem->setData(0, BOARD_WIDTH_ITEM, st.nBoardWidth);
                                                        pBoardItem->setText(11, QString("%1").arg(st.nBoardWidth));
                                                        pBoardItem->setTextAlignment(10, Qt::AlignCenter);

                                                        pBoardItem->setData(0, BOARD_THICKNESS_ITEM, st.nBoardThickness);
                                                        pBoardItem->setText(12, QString("%1").arg(st.nBoardThickness));
                                                        pBoardItem->setTextAlignment(12, Qt::AlignCenter);

                                                        pBoardItem->setData(0, BOARD_NUM_ITEM, st.nBoardNum);
                                                        pBoardItem->setText(13, QString("%1").arg(st.nBoardNum));
                                                        pBoardItem->setTextAlignment(13, Qt::AlignCenter);

                                                        pBoardItem->setData(0, BOARD_EDGE_WEIGHT_ITEM, st.strEdgeWeight);
                                                        pBoardItem->setText(14, st.strEdgeWeight);
                                                        pBoardItem->setTextAlignment(14, Qt::AlignCenter);

                                                        pBoardItem->setData(0, BOARD_FINISHED_PRODUCT_HEIGHT_ITEM, st.nFinishedProductHeight);
                                                        pBoardItem->setText(15, QString("%1").arg(st.nFinishedProductHeight));
                                                        pBoardItem->setTextAlignment(15, Qt::AlignCenter);

                                                        pBoardItem->setData(0, BOARD_FINISHED_PRODUCT_WIDTH_ITEM, st.nFinishedProductWidth);
                                                        pBoardItem->setText(16, QString("%1").arg(st.nFinishedProductWidth));
                                                        pBoardItem->setTextAlignment(16, Qt::AlignCenter);

                                                        pBoardItem->setData(0, BOARD_FINISHED_PRODUCT_THICKNESS_ITEM, st.nFinishedProductThickness);
                                                        pBoardItem->setText(17, QString("%1").arg(st.nFinishedProductThickness));
                                                        pBoardItem->setTextAlignment(17, Qt::AlignCenter);

                                                        pBoardItem->setData(0, BOARD_DISSIMILAR_SHAPE_ITEM, st.strDissimilarShapes);
                                                        pBoardItem->setText(18, st.strDissimilarShapes);
                                                        pBoardItem->setTextAlignment(18, Qt::AlignCenter);

                                                        pBoardItem->setData(0, BOARD_ROTATE_ITEM, st.nRotate);
                                                        pBoardItem->setText(19, QString("%1").arg(st.nRotate));
                                                        pBoardItem->setTextAlignment(19, Qt::AlignCenter);

                                                        pBoardItem->setData(0, BOARD_CLIENT_INFORMATION_ITEM, st.strClientInformation);
                                                        pBoardItem->setText(20, st.strClientInformation);
                                                        pBoardItem->setTextAlignment(20, Qt::AlignCenter);

                                                        pBoardItem->setData(0, BOARD_FRANCHIES_STORE_ITEM, st.strFranchiseStore);
                                                        pBoardItem->setText(21, st.strFranchiseStore);
                                                        pBoardItem->setTextAlignment(21, Qt::AlignCenter);

                                                        pSourceBoardItem->addChild(pBoardItem);

                                                        pBoardItem->setCheckState(4, Qt::Unchecked);
                                                    }

                                                }
                                                else
                                                {
                                                   QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("读取数据库失败: 获取不到板件信息."), tc->toUnicode("知道了"));
                                                }
                                            }
                                        }
                                        else
                                        {
                                            QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("读取数据库失败: 获取不到原料板数据."), tc->toUnicode("知道了"));
                                        }
                                    }
                                }
                                else
                                {
                                    QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("读取数据库失败: 获取不到方案数据."), tc->toUnicode("知道了"));
                                }
                            }
                        }
                        else
                        {
                            QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("读取数据库失败: 获取不到料单分组."), tc->toUnicode("知道了"));
                        }
                    }
                }
            }
            else
            {
                QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("读取数据库失败: 获取不到料单数据."), tc->toUnicode("知道了"));
            }
        }
        else
        {
            QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("读取数据库失败: 无效数据库文件或数据库文件不存在."), tc->toUnicode("知道了"));
        }
    }
    catch (QException& e)
    {
        qCritical("MainWindow::on_actionOpenProject_triggered() exception: %s", e.what());
    }
}

void DialogPrintBoards::onTreeWidgetItemChanged(QTreeWidgetItem *pItem, int /*column*/)
{
    if (!pItem)
    {
        return;
    }

    QVariant var = pItem->data(0, ITEM_TYPE);
    bool ok = false;
    int nType = var.toInt(&ok);
    if (!ok)
    {
        return;
    }

    if (nType == ITEM_TYPE_SOURCE_BOARD)
    {
        int nNum = pItem->childCount();
        if (pItem->checkState(3) == Qt::Checked)
        {
            for (int i = 0; i < nNum; ++i)
            {
                QTreeWidgetItem* child = pItem->child(i);
                child->setCheckState(4, Qt::Checked);
            }
        }
        else if (pItem->checkState(3) == Qt::Unchecked)
        {
            bool isAllChecked = true;
            for (int i = 0; i < nNum; ++i)
            {
                QTreeWidgetItem* child = pItem->child(i);
                if (child->checkState(4) == Qt::Unchecked)
                {
                    isAllChecked = false;
                    break;
                }
            }

            if (isAllChecked)
            {
                for (int i = 0; i < nNum; ++i)
                {
                    QTreeWidgetItem* child = pItem->child(i);
                    child->setCheckState(4, Qt::Unchecked);
                }
            }
        }
    }
    else if (nType == ITEM_TYPE_BOARD)
    {
        QTreeWidgetItem* parent = pItem->parent();
        if (parent)
        {
            if (pItem->checkState(4) == Qt::Unchecked)
            {
                parent->setCheckState(3, Qt::Unchecked);
            }
            else if (pItem->checkState(4) == Qt::Checked)
            {
                bool isAllChecked = true;

                int nNum = parent->childCount();
                for (int i = 0; i < nNum; ++i)
                {
                    QTreeWidgetItem* child = parent->child(i);
                    if (child->checkState(4) == Qt::Unchecked)
                    {
                        isAllChecked = false;
                        break;
                    }
                }

                if (isAllChecked)
                {
                    parent->setCheckState(3, Qt::Checked);
                }
                else
                {
                    parent->setCheckState(3, Qt::Unchecked);
                }
            }
        }
    }
}

bool DialogPrintBoards::readDataFromItem(QTreeWidgetItem *pItem, BoardSt& st)
{
    if (!pItem)
    {
        return false;
    }

    QVariant var = pItem->data(0, ITEM_TYPE);
    bool ok = false;
    int nType = var.toInt(&ok);
    if (!ok)
    {
        return false;
    }

    if (nType != ITEM_TYPE_BOARD)
    {
        return false;
    }

    QTreeWidgetItem* parent = pItem->parent();
    if (parent && parent->data(0, ITEM_TYPE).toInt() == ITEM_TYPE_SOURCE_BOARD)
    {
        ok = false;
        var = parent->data(0, SOURCE_BOARD_INDEX_ITEM);
        st.nSourceBoardIndex = var.toInt(&ok);
        if (!ok)
        {
            return false;
        }
    }

    ok = false;
    var = pItem->data(0, BOARD_INDEX_ITEM);
    st.nBoardIndex = var.toInt(&ok);
    if (!ok)
    {
        return false;
    }

    var = pItem->data(0, BOARD_ORDERS_NO_ITEM);
    st.dataSt.strOrdersNo = var.toString();

    var = pItem->data(0, BOARD_BARCODE_ITEM);
    st.dataSt.strBarCodeNo = var.toString();

    var = pItem->data(0, BOARD_PRODUCT_NAME_ITEM);
    st.dataSt.strProductName = var.toString();

    var = pItem->data(0, BOARD_FINISHED_PRODUCT_NAME_ITEM);
    st.dataSt.strFinishedProductName = var.toString();

    var = pItem->data(0, BOARD_MATERIAL_TYPE_ITEM);
    st.dataSt.strMaterialType = var.toString();

    ok = false;
    var = pItem->data(0, BOARD_WIDTH_ITEM);
    st.dataSt.nBoardWidth = var.toInt(&ok);
    if (!ok)
    {
        return false;
    }

    ok = false;
    var = pItem->data(0, BOARD_HEIGHT_ITEM);
    st.dataSt.nBoardHeight = var.toInt(&ok);
    if (!ok)
    {
        return false;
    }

    ok = false;
    var = pItem->data(0, BOARD_THICKNESS_ITEM);
    st.dataSt.nBoardThickness = var.toInt(&ok);
    if (!ok)
    {
        return false;
    }

    ok = false;
    var = pItem->data(0, BOARD_NUM_ITEM);
    st.dataSt.nBoardNum = var.toInt(&ok);
    if (!ok)
    {
        return false;
    }

    var = pItem->data(0, BOARD_EDGE_WEIGHT_ITEM);
    st.dataSt.strEdgeWeight = var.toString();


    ok = false;
    var = pItem->data(0, BOARD_FINISHED_PRODUCT_WIDTH_ITEM);
    st.dataSt.nFinishedProductWidth = var.toInt(&ok);
    if (!ok)
    {
        return false;
    }

    ok = false;
    var = pItem->data(0, BOARD_FINISHED_PRODUCT_HEIGHT_ITEM);
    st.dataSt.nFinishedProductHeight = var.toInt(&ok);
    if (!ok)
    {
        return false;
    }

    ok = false;
    var = pItem->data(0, BOARD_FINISHED_PRODUCT_THICKNESS_ITEM);
    st.dataSt.nFinishedProductThickness = var.toInt(&ok);
    if (!ok)
    {
        return false;
    }

    var = pItem->data(0, BOARD_DISSIMILAR_SHAPE_ITEM);
    st.dataSt.strDissimilarShapes = var.toString();

    ok = false;
    var = pItem->data(0, BOARD_ROTATE_ITEM);
    st.dataSt.nRotate = var.toInt(&ok);
    if (!ok)
    {
        return false;
    }

    var = pItem->data(0, BOARD_CLIENT_INFORMATION_ITEM);
    st.dataSt.strClientInformation = var.toString();

    var = pItem->data(0, BOARD_FRANCHIES_STORE_ITEM);
    st.dataSt.strFranchiseStore = var.toString();

    return true;
}

void DialogPrintBoards::transformCheckedItem2VectorDataSt(std::vector<BoardSt>& vecDataSt)
{
    if (m_pTreeWidget)
    {
        int nNum = m_pTreeWidget->topLevelItemCount();
        for (int i = 0; i < nNum; ++i)
        {
            QTreeWidgetItem* pItem = m_pTreeWidget->topLevelItem(i);
            if (pItem)
            {
                readChildItem(pItem, vecDataSt);
            }
        }
    }
}

void DialogPrintBoards::readChildItem(QTreeWidgetItem* pItem, std::vector<BoardSt>& vecDataSt)
{
    if (pItem)
    {
        QVariant var = pItem->data(0, ITEM_TYPE);
        bool ok = false;
        int nType = var.toInt(&ok);
        if (ok && nType == ITEM_TYPE_BOARD && pItem->checkState(4) == Qt::Checked)
        {
             BoardSt st;
             readDataFromItem(pItem, st);
             vecDataSt.push_back(st);
        }

        int nNum = pItem->childCount();
        for (int i = 0; i < nNum; ++i)
        {
            QTreeWidgetItem* child = pItem->child(i);

            // 递归
            readChildItem(child, vecDataSt);
        }
    }
}

void DialogPrintBoards::sortVectorDataSt(std::vector<BoardSt>& vecDataSt)
{
    // 天才第一步，先按大板序号分类
    std::map<int, std::vector<BoardSt>> boardStMap;
    std::vector<BoardSt>::iterator it = vecDataSt.begin();
    for (; it != vecDataSt.end(); ++it)
    {
        BoardSt& st = *it;
        if (boardStMap.find(st.nSourceBoardIndex) == boardStMap.end())
        {
            std::vector<BoardSt> vecBoardSt;
            vecBoardSt.push_back(st);
            boardStMap[st.nSourceBoardIndex] = vecBoardSt;
        }
        else
        {
            std::vector<BoardSt>& vecBoardSt = boardStMap[st.nSourceBoardIndex];
            vecBoardSt.push_back(st);
        }
    }

    std::vector<BoardSt> newVecDataSt;

    // 第二步，按小板索引排序
    std::map<int, std::vector<BoardSt>>::iterator mapIt = boardStMap.begin();
    for (; mapIt != boardStMap.end(); ++mapIt)
    {
        std::vector<BoardSt>& vecBoardSt = mapIt->second;
        std::sort(vecBoardSt.begin(), vecBoardSt.end(), boardIndexCmp);

        std::vector<BoardSt>::iterator vecIt = vecBoardSt.begin();
        for (; vecIt != vecBoardSt.end(); ++vecIt)
        {
            newVecDataSt.push_back(*vecIt);
        }
    }

    // 第三步，重新构建一个std::vector<BoardSt>数据返回给外部
    vecDataSt.swap(newVecDataSt);
}

void DialogPrintBoards::onPrintButtonClicked(bool /*checked*/)
{
    std::vector<BoardSt> vecDataSt;

    transformCheckedItem2VectorDataSt(vecDataSt);

    //sortVectorDataSt(vecDataSt);

    if (vecDataSt.size() > 0)
    {
        DialogPrintPreviewer dlg(vecDataSt, this);
        if (QDialog::Accepted == dlg.exec())
        {

        }
    }
    else
    {
        QTextCodec *tc = QTextCodec::codecForName("GBK");
        QMessageBox::information(NULL, tc->toUnicode("信息"), tc->toUnicode("没有选中任何板件."), QMessageBox::Ok);
    }
}

void DialogPrintBoards::onCloseButtonClicked(bool /*checked*/)
{
    QDialog::accept();
}

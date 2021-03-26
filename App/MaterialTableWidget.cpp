#include "MaterialTableWidget.h"
#include <QHeaderView>
#include <QTextCodec>
#include <QAbstractItemView>
#include "Global.h"
#include "DialogDissimilarShapes.h"
#include "ProductBoardTable.h"
#include <QMessageBox>

#include "DialogCustomDissimilarShape.h"

MaterialTableWidget::MaterialTableWidget(QWidget *parent)
    : QTableWidget(parent)
{
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);

    QTextCodec *tc = QTextCodec::codecForName("GBK");

    m_pMenu = new QMenu(this);

    m_pActionAddDissimilar = new QAction(QIcon(":/icons/add_common_shape_128.png"), tc->toUnicode("添加常用异形"), this);
    connect(m_pActionAddDissimilar, &QAction::triggered, this, &MaterialTableWidget::onActionAddDissimilar);

    m_pActionAddCustomDissimilar = new QAction(QIcon(":/icons/add_custom_shape_128.png"), tc->toUnicode("自定义异形"), this);
    connect(m_pActionAddCustomDissimilar, &QAction::triggered, this, &MaterialTableWidget::onActionAddCustomDissimilar);

    m_pActionDelDissimilar = new QAction(QIcon(":/icons/delete_128.png"), tc->toUnicode("删除异形"), this);
    connect(m_pActionDelDissimilar, &QAction::triggered, this, &MaterialTableWidget::onActionDelDissimilar);

    m_pMenu->addAction(m_pActionAddDissimilar);
    m_pMenu->addAction(m_pActionAddCustomDissimilar);
    m_pMenu->addAction(m_pActionDelDissimilar);
}

MaterialTableWidget::~MaterialTableWidget()
{

}

void MaterialTableWidget::setMaterialListName(const QString& strMaterialListName)
{
    m_strMaterialListName = strMaterialListName;
}

void MaterialTableWidget::setTableHeader(const QStringList& sl)
{
    // 先把table的内容清空
    int nColumn = this->columnCount();
    this->clear();
    for (int n = 0; n < nColumn; ++n)
    {
        this->removeColumn(0);
    }

    //重新创建表头
    this->setColumnCount(sl.size());
    this->setHorizontalHeaderLabels(sl);
}

void MaterialTableWidget::removeRowData()
{
    int nRow = this->rowCount();
    int nCol = this->columnCount();

    for (int i = 0; i < nRow; ++i)
    {
        for (int j = 0; j < nCol; ++j)
        {
            QTableWidgetItem* pItem = this->item(i, j);
            delete pItem;
            pItem = NULL;
        }
        this->removeRow(0);
    }

    this->clearContents();
}

void MaterialTableWidget::displayData(const std::vector<DataSt>& vecData)
{
    int nRow = vecData.size();

    //插入新数据
    for (int r = 0; r < nRow; ++r)  //行
    {
        const DataSt& dataSt = vecData.at(r);

        this->insertRow(r); //插入新行
        for (int c = 0; c < this->columnCount(); ++c )  //列
        {
            if (c == 0)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(dataSt.strOrdersNo);
                pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
            else if (c == 1)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(dataSt.strBarCodeNo);
                pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
                pItem->setTextAlignment(Qt::AlignCenter);
                pItem->setData(ITEM_TYPE, MATERIAL_TABLE_BARCODE_ITEM);
                this->setItem(r, c, pItem);
            }
            else if (c == 2)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(dataSt.strProductName);
                pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
            else if (c == 3)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(dataSt.strFinishedProductName);
                pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
                pItem->setData(ITEM_TYPE, MATERIAL_TABLE_FINISHED_PRODUCT_NAME_ITEM);
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
            else if (c == 4)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(dataSt.strMaterialType);
                pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
            else if (c == 5)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(QString("%1").arg(dataSt.nBoardHeight));
                pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
                pItem->setData(ITEM_TYPE, MATERIAL_TABLE_HEIGHT_ITEM);
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
            else if (c == 6)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(QString("%1").arg(dataSt.nBoardWidth));
                pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
                pItem->setData(ITEM_TYPE, MATERIAL_TABLE_WIDTH_ITEM);
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
            else if (c == 7)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(QString("%1").arg(dataSt.nBoardThickness));
                pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
            else if (c == 8)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(QString("%1").arg(dataSt.nBoardNum));
                pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
            else if (c == 9)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(dataSt.strEdgeWeight);
                pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
            else if (c == 10)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(QString("%1").arg(dataSt.nFinishedProductHeight));
                pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
            else if (c == 11)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(QString("%1").arg(dataSt.nFinishedProductWidth));
                pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
            else if (c == 12)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(QString("%1").arg(dataSt.nFinishedProductThickness));
                pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
            else if (c == 13)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(dataSt.strDissimilarShapes);
                //pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
                pItem->setData(ITEM_TYPE, MATERIAL_TABLE_DISSIMILAR_ITEM);
                pItem->setData(MATERIAL_TABLE_PRODUCT_BOARD_ID, dataSt.strProductBoardID);
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
            else if (c == 14)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(QString("%1").arg(dataSt.nRotate));
                pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
            else if (c == 15)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(dataSt.strClientInformation);
                pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
            else if (c == 16)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(dataSt.strFranchiseStore);
                pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
        }
    }
}

void MaterialTableWidget::contextMenuEvent(QContextMenuEvent* event)
{
    if (event != NULL)
    {
        QTableWidgetItem* pItem = this->itemAt(event->pos());
        if (pItem != NULL && pItem->isSelected())
        {
            m_pMenu->exec(event->globalPos());
        }
    }

    QTableWidget::contextMenuEvent(event);
}

void MaterialTableWidget::onActionAddDissimilar(bool /*checked*/)
{
    QList<QTableWidgetItem*> itemLs = this->selectedItems();
    if (itemLs.size() >= this->columnCount())
    {
        QString strFinishedProductName;
        QString strBarcodeNo;
        int nBoardW = 0;
        int nBoardH = 0;
        QTableWidgetItem* pDissimilarItem = NULL;
        for (int i = 0; i < itemLs.size(); ++i)
        {
            QTableWidgetItem* pItem = itemLs.at(i);

            bool ok = false;
            int nItemType = pItem->data(ITEM_TYPE).toInt(&ok);
            if (!ok)
            {
                continue;
            }

            if (nItemType == MATERIAL_TABLE_DISSIMILAR_ITEM)
            {
                pDissimilarItem = pItem;
            }
            else if (nItemType == MATERIAL_TABLE_WIDTH_ITEM)
            {
                nBoardW = pItem->text().toInt(&ok);
                if (!ok)
                {
                    nBoardW = 0;
                }
            }
            else if (nItemType == MATERIAL_TABLE_HEIGHT_ITEM)
            {
                nBoardH = pItem->text().toInt(&ok);
                if (!ok)
                {
                    nBoardH = 0;
                }
            }
            else if (nItemType == MATERIAL_TABLE_FINISHED_PRODUCT_NAME_ITEM)
            {
                strFinishedProductName = pItem->text();
            }
            else if (nItemType == MATERIAL_TABLE_BARCODE_ITEM)
            {
                strBarcodeNo = pItem->text();
            }
        }

        if (pDissimilarItem != NULL && !strBarcodeNo.isEmpty() && nBoardW > 0 && nBoardH > 0 && !strFinishedProductName.isEmpty())
        {
            int nID = -2;
            QString str = pDissimilarItem->text();
            if (!str.isEmpty())
            {
                std::map<QString, QVariant> paramsMap;
                extractParameters(str, paramsMap);
                QVariant val = paramsMap["ID"];
                bool ok = false;
                nID = val.toInt(&ok);
                if (!ok)
                {
                    nID = -2;
                }
            }
            DialogDissimilarShapes dlg(strFinishedProductName, nBoardW, nBoardH, nID, this);
            if (dlg.exec() == QDialog::Accepted)
            {
                QString strParameters;
                dlg.getDissimilarParameters(strParameters);
                pDissimilarItem->setText(strParameters);

                emit updateDissimilar(m_strMaterialListName, strBarcodeNo, strParameters);
            }
        }
    }
}

void MaterialTableWidget::extractParameters(const QString& str, std::map<QString, QVariant>& paramsMap)
{
    QStringList sl = str.split(';');
    for (int i = 0; i < sl.size(); ++i)
    {
        QString str = sl.at(i);
        if (!str.isEmpty())
        {
            QStringList slParam = str.split('=');
            if (slParam.size() >= 2)
            {
                QString strKey = slParam.at(0);
                QString strValue = slParam.at(1);
                bool ok = false;
                strValue.toDouble(&ok);
                if (ok)
                {
                    paramsMap[strKey] = strValue;
                }
                else
                {
                    return;
                }
            }
        }
    }
}

void MaterialTableWidget::onActionAddCustomDissimilar(bool /*checked*/)
{
    QList<QTableWidgetItem*> itemLs = this->selectedItems();
    if (itemLs.size() >= this->columnCount())
    {
        QString strBarcodeNo;
        QString strProductName;
        int nBoardW = 0;
        int nBoardH = 0;
        QTableWidgetItem* pDissimilarItem = NULL;
        for (int i = 0; i < itemLs.size(); ++i)
        {
            QTableWidgetItem* pItem = itemLs.at(i);

            bool ok = false;
            int nItemType = pItem->data(ITEM_TYPE).toInt(&ok);
            if (!ok)
            {
                continue;
            }

            if (nItemType == MATERIAL_TABLE_DISSIMILAR_ITEM)
            {
                pDissimilarItem = pItem;
            }
            else if (nItemType == MATERIAL_TABLE_WIDTH_ITEM)
            {
                nBoardW = pItem->text().toInt(&ok);
                if (!ok)
                {
                    nBoardW = 0;
                }
            }
            else if (nItemType == MATERIAL_TABLE_HEIGHT_ITEM)
            {
                nBoardH = pItem->text().toInt(&ok);
                if (!ok)
                {
                    nBoardH = 0;
                }
            }
            else if (nItemType == MATERIAL_TABLE_FINISHED_PRODUCT_NAME_ITEM)
            {
                strProductName = pItem->text();
            }
            else if (nItemType == MATERIAL_TABLE_BARCODE_ITEM)
            {
                strBarcodeNo = pItem->text();
            }
        }

        if (pDissimilarItem != NULL && !strBarcodeNo.isEmpty() && nBoardW > 0 && nBoardH > 0)
        {
            int nID = -2;
            QString str = pDissimilarItem->text();
            if (!str.isEmpty())
            {
                std::map<QString, QVariant> paramsMap;
                extractParameters(str, paramsMap);
                QVariant val = paramsMap["ID"];
                bool ok = false;
                nID = val.toInt(&ok);
                if (!ok)
                {
                    nID = -2;
                }
            }

            bool editExisting = nID == -1 ? true : false;

            DialogCustomDissimilarShape dlg(strBarcodeNo, strProductName, nBoardW, nBoardH, editExisting, this);
            if (dlg.exec() == QDialog::Accepted)
            {
                QString strShape = dlg.getShape();
                pDissimilarItem->setText(strShape);

                emit updateDissimilar(m_strMaterialListName, strBarcodeNo, strShape);
            }
        }
    }
}

void MaterialTableWidget::onActionDelDissimilar(bool /*checked*/)
{
    QList<QTableWidgetItem*> itemLs = this->selectedItems();
    if (itemLs.size() >= this->columnCount())
    {
        QString strFinishedProductName;
        QString strBarcodeNo;
        QTableWidgetItem* pDissimilarItem = NULL;
        for (int i = 0; i < itemLs.size(); ++i)
        {
            QTableWidgetItem* pItem = itemLs.at(i);

            bool ok = false;
            int nItemType = pItem->data(ITEM_TYPE).toInt(&ok);
            if (!ok)
            {
                continue;
            }

            if (nItemType == MATERIAL_TABLE_DISSIMILAR_ITEM)
            {
                pDissimilarItem = pItem;
            }
            else if (nItemType == MATERIAL_TABLE_FINISHED_PRODUCT_NAME_ITEM)
            {
                strFinishedProductName = pItem->text();
            }
            else if (nItemType == MATERIAL_TABLE_BARCODE_ITEM)
            {
                strBarcodeNo = pItem->text();
            }
        }

        QString strParameters = "";
        if (pDissimilarItem != NULL && !strBarcodeNo.isEmpty())
        {
            pDissimilarItem->setText(strParameters);

            emit updateDissimilar(m_strMaterialListName, strBarcodeNo, strParameters);
        }
    }
}

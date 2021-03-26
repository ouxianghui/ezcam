#include "DialogDissimilarShapes.h"
#include "ui_DialogDissimilarShapes.h"
#include <QApplication>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QListWidgetItem>
#include <QAbstractItemView>
#include <QPixmap>
#include <QFileInfo>
#include <algorithm>
#include <QMessageBox>
#include <QRegExp>
#include <QValidator>
#include <QVariant>
#include <QTextCodec>
#include "DissimilarParametersReader.h"
#include "DissimilarParametersWriter.h"
#include "DissimilarLibraryOperator.h"
#include <QException>

DialogDissimilarShapes::DialogDissimilarShapes(const QString& strName, double dWidth, double dHeight, int specifiedID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDissimilarShapes),
    m_nPageSize(12),
    m_nCurrentPageIndex(0),
    m_nTotalPage(0),
    m_strName(strName),
    m_dWidth(dWidth),
    m_dHeight(dHeight)
{
    ui->setupUi(this);
    this->setFixedSize(1002, 596);

    createWidgets();
    readDissimilarFromLibrary();

    std::size_t nElemNum = m_vecDissimilarSt.size();

    m_nTotalPage = nElemNum/m_nPageSize;
    if ((nElemNum % m_nPageSize) > 0)
    {
        ++m_nTotalPage;
    }

    if (nElemNum > 0)
    {
        // 外部没有制定异形ID，就从第一页开始显示，外部有制定异形ID，则从该ID所在页开始显示
        if (specifiedID != -2)
        {
            int nElemIndex = -1;
            for (std::size_t i = 0; i < m_vecDissimilarSt.size(); ++i)
            {
                const DissimilarSt& st = m_vecDissimilarSt[i];
                if (st.nID == specifiedID)
                {
                    nElemIndex = i;
                    break;
                }
            }

            if (nElemIndex != -1)
            {
                m_nCurrentPageIndex = nElemIndex/m_nPageSize;
            }
        }

        std::size_t nIndex = m_nPageSize*m_nCurrentPageIndex;
        if (nIndex <= (nElemNum-1))
        {
            std::size_t nOffset = m_nPageSize > (nElemNum-nIndex) ? (nElemNum-nIndex) : m_nPageSize;

            std::vector<DissimilarSt> vecSt(m_vecDissimilarSt.begin()+nIndex, m_vecDissimilarSt.begin()+nIndex+nOffset);
            displayDisimilars2ListWidget(vecSt);
        }

        QList<QListWidgetItem*> items = m_pListWidgetDissimilar->findItems(QString("%1").arg(specifiedID), Qt::MatchFixedString);

        if (items.length() > 0)
        {
            // 弱水三千只取一瓢饮
            QListWidgetItem* item = items[0];
            m_pListWidgetDissimilar->setCurrentItem(item, QItemSelectionModel::Select);
            onDissimilarItemClicked(item);

            m_pListWidgetDissimilar->setFocus();
        }
    }

    m_pLineEditCurrentPageIndex->setText(QString("%1").arg(m_nCurrentPageIndex+1));
    m_pLabelTotalPage->setText(QString("/%1").arg(m_nTotalPage));

    //    std::map<QString, double> paramsMap;
    //    paramsMap["W"] = 0.5f;
    //    paramsMap["H"] = 0.5f;
    //    paramsMap["a"] = 0.5f;
    //    paramsMap["b"] = 0.5f;
    //    paramsMap["c"] = 0.5f;
    //    paramsMap["d"] = 0.5f;
    //    DissimilarParametersWriter::write(0, paramsMap);
    //    std::map<QString, double> paramsMap2;
    //    DissimilarParametersReader::read(0, paramsMap2);
    //    int i = 0;
    //    ++i;
}

DialogDissimilarShapes::~DialogDissimilarShapes()
{
    delete ui;
}

void DialogDissimilarShapes::createWidgets()
{
    QTextCodec *tc = QTextCodec::codecForName("GBK");

    QGroupBox* pGroupBoxParameters = new QGroupBox(this);
    pGroupBoxParameters->setTitle(tc->toUnicode("参数"));

    m_pTableWidgetParameters = new QTableWidget(pGroupBoxParameters);
    QStringList sl;
    sl << tc->toUnicode("名称") << tc->toUnicode("值");
    m_pTableWidgetParameters->setColumnCount(sl.size());
    m_pTableWidgetParameters->setHorizontalHeaderLabels(sl);
    m_pTableWidgetParameters->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_pTableWidgetParameters->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTableWidgetParameters->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(m_pTableWidgetParameters, &QTableWidget::itemChanged, this, &DialogDissimilarShapes::onParametersTableWidgetItemChanged);
    QVBoxLayout* pLayoutParameters = new QVBoxLayout(pGroupBoxParameters);
    pLayoutParameters->addWidget(m_pTableWidgetParameters);

    QGroupBox* pGroupBoxPreview = new QGroupBox(this);
    pGroupBoxPreview->setTitle(tc->toUnicode("预览"));

    m_pDissimilarPreviewer = new DissimilarPreviewer(pGroupBoxParameters);
    QVBoxLayout* pLayoutPreview = new QVBoxLayout();
    pLayoutPreview->addWidget(m_pDissimilarPreviewer);
    pGroupBoxPreview->setLayout(pLayoutPreview);

    QGroupBox* pGroupBoxDissimilar = new QGroupBox(this);
    pGroupBoxDissimilar->setTitle(tc->toUnicode("异形"));
    m_pListWidgetDissimilar = new QListWidget(pGroupBoxDissimilar);
    m_pListWidgetDissimilar->setIconSize(QSize(128, 128));
    m_pListWidgetDissimilar->setGridSize(QSize(160, 160));
    m_pListWidgetDissimilar->setResizeMode(QListView::Adjust);
    m_pListWidgetDissimilar->setViewMode(QListView::IconMode);
    m_pListWidgetDissimilar->setFlow(QListView::LeftToRight);
    m_pListWidgetDissimilar->setMovement(QListView::Static);
    m_pListWidgetDissimilar->setSelectionMode(QListWidget::ExtendedSelection);
    m_pListWidgetDissimilar->setSelectionBehavior(QListWidget::SelectRows);
    m_pListWidgetDissimilar->setVerticalScrollMode(QListWidget::ScrollPerItem);
    m_pListWidgetDissimilar->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pListWidgetDissimilar->setAutoScroll(false);
    connect(m_pListWidgetDissimilar, &QListWidget::itemClicked, this, &DialogDissimilarShapes::onDissimilarItemClicked);

    QVBoxLayout* pLayoutDissimilar = new QVBoxLayout(pGroupBoxDissimilar);
    pLayoutDissimilar->addWidget(m_pListWidgetDissimilar, 19);

    m_pToolButtonLastPage = new QToolButton(pGroupBoxDissimilar);
    m_pToolButtonLastPage->setIconSize(QSize(24, 24));
    m_pToolButtonLastPage->setIcon(QIcon(":/icons/last_page.png"));
    connect(m_pToolButtonLastPage, &QToolButton::clicked, this, &DialogDissimilarShapes::onLastPageButtonClicked);

    m_pToolButtonNextPage = new QToolButton(pGroupBoxDissimilar);
    m_pToolButtonNextPage->setIconSize(QSize(24, 24));
    m_pToolButtonNextPage->setIcon(QIcon(":/icons/next_page.png"));
    connect(m_pToolButtonNextPage, &QToolButton::clicked, this, &DialogDissimilarShapes::onNextPageButtonClicked);

    m_pToolButtonApply = new QToolButton(pGroupBoxDissimilar);
    m_pToolButtonApply->setIconSize(QSize(24, 24));
    m_pToolButtonApply->setIcon(QIcon(":/icons/accept.png"));
    connect(m_pToolButtonApply, &QToolButton::clicked, this, &DialogDissimilarShapes::onApplyButtonClicked);

    m_pLineEditCurrentPageIndex = new QLineEdit(pGroupBoxDissimilar);
    m_pLineEditCurrentPageIndex->setAlignment(Qt::AlignCenter);

    //正整数正则表达式
    QRegExp regx("(^[0-9]*[1-9][0-9]*$)");
    QValidator *validator = new QRegExpValidator(regx, this);
    m_pLineEditCurrentPageIndex->setValidator(validator);

    m_pLabelTotalPage = new QLabel(pGroupBoxDissimilar);
    m_pLabelTotalPage->setText("/");

    QHBoxLayout* pLayoutPageInfo = new QHBoxLayout();
    m_pLabelcomment = new QLabel(pGroupBoxDissimilar);
    pLayoutPageInfo->addWidget(m_pLabelcomment, 12);
    pLayoutPageInfo->addWidget(m_pToolButtonLastPage, 2);
    pLayoutPageInfo->addWidget(m_pToolButtonNextPage, 2);
    pLayoutPageInfo->addWidget(m_pLineEditCurrentPageIndex, 1);
    pLayoutPageInfo->addWidget(m_pLabelTotalPage, 1);
    pLayoutPageInfo->addWidget(m_pToolButtonApply, 2);
    pLayoutDissimilar->addLayout(pLayoutPageInfo, 1);

    QVBoxLayout* pLayoutRight = new QVBoxLayout();
    pLayoutRight->addWidget(pGroupBoxPreview, 10);
    pLayoutRight->addWidget(pGroupBoxParameters, 10);

    QHBoxLayout* pLayoutTop = new QHBoxLayout();
    pLayoutTop->addWidget(pGroupBoxDissimilar, 14);
    pLayoutTop->addLayout(pLayoutRight, 6);

    m_pPushButtonOk = new QPushButton(this);
    m_pPushButtonOk->setText(tr("Ok"));
    connect(m_pPushButtonOk, &QPushButton::clicked, this, &DialogDissimilarShapes::onOkButtonClicked);
    m_pPushButtonCancel = new QPushButton(this);
    m_pPushButtonCancel->setText(tr("Cancel"));
    connect(m_pPushButtonCancel, &QPushButton::clicked, this, &DialogDissimilarShapes::onCancelButtonClicked);

    QHBoxLayout* pLayoutBottom = new QHBoxLayout();
    pLayoutBottom->addWidget(new QLabel(this), 16);
    pLayoutBottom->addWidget(m_pPushButtonOk, 2);
    pLayoutBottom->addWidget(m_pPushButtonCancel, 2);

    QVBoxLayout* pLayoutGlobal = new QVBoxLayout(this);
    pLayoutGlobal->addLayout(pLayoutTop, 19);
    pLayoutGlobal->addLayout(pLayoutBottom, 1);
}

void DialogDissimilarShapes::readDissimilarFromLibrary()
{
    try
    {
        QStringList sl;
        DissimilarLibraryOperator::getAllDissimilarImageName(sl);

        QString strDir = QApplication::applicationDirPath()+ "/dissimilar/";
        for (int i = 0; i < sl.size(); ++i)
        {
            QString strFilePathName = strDir + sl.at(i);
            QFileInfo info(strFilePathName);
            if (info.exists() && info.isFile() && info.isReadable())
            {
                int nID = -2;
                bool ok = false;
                nID = info.baseName().toInt(&ok, 10);
                if (!ok || nID == -2)
                {
                    continue;
                }

                DissimilarSt st;
                st.nID = nID;
                st.strPathName = strFilePathName;
                m_vecDissimilarSt.push_back(st);
            }
        }

        std::sort(m_vecDissimilarSt.begin(), m_vecDissimilarSt.end());
    }
    catch (QException& e)
    {
        qCritical("DialogDissimilarShapes::readDissimilarFromLibrary() exception: %s", e.what());
    }
}

void DialogDissimilarShapes::displayDisimilars2ListWidget(const std::vector<DissimilarSt>& vecSt)
{
    try
    {
        int nCount = m_pListWidgetDissimilar->count();
        for (int i = nCount; i >= 0; --i)
        {
            QListWidgetItem* pItem = m_pListWidgetDissimilar->takeItem(i);
            delete pItem;
            pItem = NULL;
        }

        std::vector<DissimilarSt>::const_iterator it = vecSt.begin();

        for (; it != vecSt.end(); ++it)
        {
            const DissimilarSt& st = *it;

            QListWidgetItem* pItem = new QListWidgetItem(m_pListWidgetDissimilar);

            QPixmap pixmap(st.strPathName);

            pItem->setData(Qt::DecorationRole, QIcon(pixmap));
            pItem->setData(Qt::DisplayRole, st.nID);
            pItem->setData(Qt::UserRole, st.nID);

            m_pListWidgetDissimilar->addItem(pItem);
        }

        std::sort(m_vecDissimilarSt.begin(), m_vecDissimilarSt.end());
    }
    catch (QException& e)
    {
        qCritical("DialogDissimilarShapes::displayDisimilars2ListWidget() exception: %s", e.what());
    }
}

void DialogDissimilarShapes::onLastPageButtonClicked(bool /*checked*/)
{
    try
    {
        if (m_nCurrentPageIndex == 0)
        {
            return;
        }

        --m_nCurrentPageIndex;

        std::size_t nElemSize = m_vecDissimilarSt.size();
        if (nElemSize > 0)
        {
            std::size_t nIndex = m_nPageSize*m_nCurrentPageIndex;

            if (nIndex <= (nElemSize-1))
            {
                std::size_t nOffset = m_nPageSize > (nElemSize-nIndex) ? (nElemSize-nIndex) : m_nPageSize;

                std::vector<DissimilarSt> vecSt(m_vecDissimilarSt.begin()+nIndex, m_vecDissimilarSt.begin()+nIndex+nOffset);
                displayDisimilars2ListWidget(vecSt);
            }
        }

        m_pLineEditCurrentPageIndex->setText(QString("%1").arg(m_nCurrentPageIndex+1));
    }
    catch (QException& e)
    {
        qCritical("DialogDissimilarShapes::onLastPageButtonClicked() exception: %s", e.what());
    }
}

void DialogDissimilarShapes::onNextPageButtonClicked(bool /*checked*/)
{
    try
    {
        if (m_nCurrentPageIndex == m_nTotalPage-1)
        {
            return;
        }

        ++m_nCurrentPageIndex;

        std::size_t nElemSize = m_vecDissimilarSt.size();
        if (nElemSize > 0)
        {
            std::size_t nIndex = m_nPageSize*m_nCurrentPageIndex;

            if (nIndex <= (nElemSize-1))
            {
                std::size_t nOffset = m_nPageSize > (nElemSize-nIndex) ? (nElemSize-nIndex) : m_nPageSize;

                std::vector<DissimilarSt> vecSt(m_vecDissimilarSt.begin()+nIndex, m_vecDissimilarSt.begin()+nIndex+nOffset);
                displayDisimilars2ListWidget(vecSt);
            }
        }

        m_pLineEditCurrentPageIndex->setText(QString("%1").arg(m_nCurrentPageIndex+1));
    }
    catch (QException& e)
    {
        qCritical("DialogDissimilarShapes::onNextPageButtonClicked() exception: %s", e.what());
    }
}

void DialogDissimilarShapes::onApplyButtonClicked(bool /*checked*/)
{
    try
    {
        QString strPageIndex = m_pLineEditCurrentPageIndex->text();

        std::size_t nIndex = -1;
        bool ok = false;
        nIndex = strPageIndex.toInt(&ok);

        // 减1后才是在向量中的索引
        --nIndex;

        if (!ok || nIndex == -1 || nIndex < 0 || nIndex > m_nTotalPage-1)
        {
            QTextCodec *tc = QTextCodec::codecForName("GBK");
            QMessageBox::warning(this, tc->toUnicode("警告"), tc->toUnicode("无效的分页索引."), QMessageBox::Ok);
            return;
        }

        m_nCurrentPageIndex = nIndex;

        std::size_t nElemSize = m_vecDissimilarSt.size();
        if (nElemSize > 0)
        {
            std::size_t nIndex = m_nPageSize*m_nCurrentPageIndex;

            if (nIndex <= (nElemSize-1))
            {
                std::size_t nOffset = m_nPageSize > (nElemSize-nIndex) ? (nElemSize-nIndex) : m_nPageSize;

                std::vector<DissimilarSt> vecSt(m_vecDissimilarSt.begin()+nIndex, m_vecDissimilarSt.begin()+nIndex+nOffset);
                displayDisimilars2ListWidget(vecSt);
            }
        }

        m_pLineEditCurrentPageIndex->setText(QString("%1").arg(m_nCurrentPageIndex+1));
    }
    catch (QException& e)
    {
        qCritical("DialogDissimilarShapes::onApplyButtonClicked() exception: %s", e.what());
    }
}

bool DialogDissimilarShapes::getDissimilarParameters(QString& strParams)
{
    if (m_pListWidgetDissimilar)
    {
        QList<QListWidgetItem*> listItems = m_pListWidgetDissimilar->selectedItems();
        if (listItems.size() > 0)
        {
            QListWidgetItem* pItem = listItems.at(0);
            QString strID = pItem->data(Qt::UserRole).toString();
            if (!strID.isEmpty())
            {
                bool ok = false;
                int nID = -2;
                nID = strID.toInt(&ok);
                if (ok && nID != -2)
                {
                    strParams.clear();
                    std::map<QString, QVariant> paramsMap;

                    QString strKey;
                    QVariant var;
                    double dValue;
                    QString strItem = QString("ID=%1").arg(strID);
                    strParams.append(strItem);

                    DissimilarParametersReader::read(nID, paramsMap);
                    std::map<QString, QVariant>::iterator it = paramsMap.begin();
                    for (; it != paramsMap.end(); ++it)
                    {
                        strKey = it->first;
                        var = it->second;
                        bool ok = false;
                        dValue = var.toDouble(&ok);
                        if (!ok)
                        {
                            dValue = 0.0f;
                        }

                        strItem = QString(";%1=%2").arg(strKey).arg(dValue);
                        strParams.append(strItem);
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

void DialogDissimilarShapes::onOkButtonClicked(bool /*checked*/)
{
    if (m_pListWidgetDissimilar)
    {
        QList<QListWidgetItem*> listItems = m_pListWidgetDissimilar->selectedItems();
        if (listItems.size() > 0)
        {
            QDialog::accept();
            return;
        }
    }

    QTextCodec *tc = QTextCodec::codecForName("GBK");
    QMessageBox::warning(this, tc->toUnicode("警告"), tc->toUnicode("没有异形被选中."), QMessageBox::Ok);
    QDialog::reject();
}

void DialogDissimilarShapes::onCancelButtonClicked(bool /*checked*/)
{
    QDialog::reject();
}

void DialogDissimilarShapes::onDissimilarItemClicked(QListWidgetItem* pItem)
{
    try
    {
        if (pItem)
        {
            QVariant val = pItem->data(Qt::UserRole);
            bool ok = false;
            int nID = -2;
            nID = val.toInt(&ok);
            if (ok && nID != -2)
            {
                QStringList slKeys;
                DissimilarLibraryOperator::getDissimilarParametersName(nID, slKeys);
                std::map<QString, QVariant> paramsMap;
                DissimilarParametersReader::read(nID, paramsMap);
                paramsMap["Width"] = m_dWidth;
                paramsMap["Height"] = m_dHeight;
                displayParameters2TableWidget(nID, slKeys, paramsMap);
                m_pDissimilarPreviewer->drawDissimilar(nID, paramsMap);
                DissimilarParametersWriter::write(nID, paramsMap);
                showMessageOnLabel(nID);
            }
        }
    }
    catch (QException& e)
    {
        qCritical("DialogDissimilarShapes::onDissimilarItemClicked() exception: %s", e.what());
    }
}

void DialogDissimilarShapes::showMessageOnLabel(int nID)
{
    QTextCodec *tc = QTextCodec::codecForName("GBK");
    QString strMsg;

    switch(nID)
    {
    case 1:
        strMsg = tc->toUnicode("注意：此异型不能用于纯圆形工件的加工！");
        m_pLabelcomment->setText(strMsg);
        break;
    default:
        m_pLabelcomment->setText("");
        break;
    }
}

bool DialogDissimilarShapes::verrifyParameters(int nID, std::map<QString, QVariant>& paramsMap)
{
    // nID == 12酒格子,要做参数限制
    if (nID == 12)
    {
        double dBoardW = 0.0f;
        double dBoardH = 0.0f;
        double dA = 0.0f;
        double dB = 0.0f;
        double dC = 0.0f;
        int nN = 0.0f;

        QVariant var;
        bool ok = false;

        if (paramsMap.find("Width") != paramsMap.end())
        {
            var = paramsMap["Width"];
            dBoardW = var.toDouble(&ok);
            if (!ok)
            {
                dBoardW = 0.0f;
            }
        }
        else
        {
            return false;
        }

        if (paramsMap.find("Height") != paramsMap.end())
        {
            var = paramsMap["Height"];
            dBoardH = var.toDouble(&ok);
            if (!ok)
            {
                dBoardH = 0.0f;
            }
        }
        else
        {
            return false;
        }

        if (paramsMap.find("a") != paramsMap.end())
        {
            var = paramsMap["a"];
            dA = var.toDouble(&ok);
            if (!ok)
            {
                dA = 0.0f;
            }
        }
        else
        {
            return false;
        }

        if (paramsMap.find("b") != paramsMap.end())
        {
            var = paramsMap["b"];
            dB = var.toDouble(&ok);
            if (!ok)
            {
                dB = 0.0f;
            }
        }
        else
        {
            return false;
        }

        if (paramsMap.find("c") != paramsMap.end())
        {
            var = paramsMap["c"];
            dC = var.toDouble(&ok);
            if (!ok)
            {
                dC = 0.0f;
            }
        }
        else
        {
            return false;
        }

        if (paramsMap.find("n") != paramsMap.end())
        {
            var = paramsMap["n"];
            nN = var.toInt(&ok);
            if (!ok)
            {
                nN = 0;
            }
        }
        else
        {
            return false;
        }

        if (nN == 1 && (dA + dB < dBoardH - dC || dA + dB > dBoardH - dC))
        {
            QTextCodec *tc = QTextCodec::codecForName("GBK");
            QMessageBox::critical(this, tc->toUnicode("错误"), tc->toUnicode("当n=1时，必须满足a+b+c=Height"), QMessageBox::Ok);

            return false;
        }
    }

    return true;
}

void DialogDissimilarShapes::displayParameters2TableWidget(int nID, const QStringList& slKeys, std::map<QString, QVariant>& paramsMap)
{
    try
    {
        int nRow = m_pTableWidgetParameters->rowCount();
        int nCol = m_pTableWidgetParameters->columnCount();

        for (int i = 0; i < nRow; ++i)
        {
            for (int j = 0; j < nCol; ++j)
            {
                QTableWidgetItem* pItem = m_pTableWidgetParameters->item(i, j);
                delete pItem;
                pItem = NULL;
            }
            m_pTableWidgetParameters->removeRow(0);
        }

        m_pTableWidgetParameters->clearContents();

        for (int i = 0; i < slKeys.size(); ++i)
        {
            m_pTableWidgetParameters->insertRow(i); //插入新行
            m_pTableWidgetParameters->setRowHeight(i, 24);
            for (int j = 0; j < nCol; ++j)
            {
                QString strKey = slKeys.at(i);
                if (j == 0)
                {
                    QTableWidgetItem* pItem = new QTableWidgetItem(QString("%1").arg(strKey));
                    pItem->setTextAlignment(Qt::AlignCenter);

                    pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
                    m_pTableWidgetParameters->setItem(i, j, pItem);
                }
                else if (j == 1)
                {
                    QTableWidgetItem* pItem = new QTableWidgetItem();
                    pItem->setTextAlignment(Qt::AlignCenter);
                    if (strKey == "ID")
                    {
                        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
                        pItem->setText(QString("%1").arg(nID));
                        pItem->setTextColor(QColor(255, 30, 24));
                    }
                    else if (strKey == "Name")
                    {
                        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
                        pItem->setText(QString("%1").arg(m_strName));
                        pItem->setTextColor(QColor(255, 30, 24));
                    }
                    else if (strKey == "Width")
                    {
                        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
                        pItem->setText(QString("%1").arg(m_dWidth));
                        pItem->setTextColor(QColor(255, 30, 24));
                    }
                    else if (strKey == "Height")
                    {
                        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
                        pItem->setText(QString("%1").arg(m_dHeight));
                        pItem->setTextColor(QColor(255, 30, 24));
                    }
                    else
                    {
                        QVariant var = paramsMap[strKey];
                        bool ok = false;
                        double dValue = var.toDouble(&ok);
                        if (ok)
                        {
                            pItem->setText(QString("%1").arg(dValue));
                        }
                        else
                        {
                            dValue = 0.0f;
                        }
                    }

                    // 每一Item都加一个ID，后面方便获取ID
                    pItem->setData(Qt::UserRole, nID);

                    m_pTableWidgetParameters->setItem(i, j, pItem);
                }
            }
        }
    }
    catch (QException& e)
    {
        qCritical("DialogDissimilarShapes::displayParameters2TableWidget() exception: %s", e.what());
    }
}

void DialogDissimilarShapes::onParametersTableWidgetItemChanged(QTableWidgetItem* pItem)
{
    try
    {
        if (pItem && pItem->isSelected())
        {
            QTextCodec *tc = QTextCodec::codecForName("GBK");

            QVariant val = pItem->data(Qt::UserRole);
            bool ok = false;
            int nID = -2;
            nID = val.toInt(&ok);
            if (ok && nID != -2)
            {
                int nRow = m_pTableWidgetParameters->rowCount();
                int nCol = m_pTableWidgetParameters->columnCount();

                std::map<QString, QVariant> paramsMap;
                QString strKey;
                double dValue = 0.0f;

                // 跳过第0行（即ID行），ID直接作为参数传递，不放入map
                for (int i = 2; i < nRow; ++i)
                {
                    bool bAdd = false;
                    for (int j = 0; j < nCol; ++j)
                    {
                        QTableWidgetItem* pTheItem = m_pTableWidgetParameters->item(i, j);
                        if (pTheItem)
                        {
                            if (j == 0)
                            {
                                strKey = pTheItem->text();
                            }
                            else if (j == 1)
                            {
                                bool isOk = false;
                                QString strValue = pTheItem->text();
                                if (!strValue.isEmpty())
                                {
                                    dValue = strValue.toDouble(&isOk);
                                    if (!isOk)
                                    {
                                        dValue = 0.0f;
                                        bAdd = false;
                                    }
                                    else
                                    {
                                        bAdd = true;
                                    }
                                }
                                else
                                {
                                    bAdd = false;
                                }
                            }
                        }
                    }

                    if (strKey.isEmpty())
                    {
                        QString strMsg = QString(tc->toUnicode("第 %1 行: 未知的变量名称.")).arg(i+1);
                        QMessageBox::warning(this, tc->toUnicode("警告"), strMsg, QMessageBox::Ok);
                        return;
                    }

                    if (bAdd)
                    {
                        paramsMap[strKey] = dValue;
                    }
                    else
                    {
                        QString strMsg = QString(tc->toUnicode("第 %1 行: 非法的参数值.")).arg(i+1);
                        QMessageBox::warning(this, tc->toUnicode("警告"), strMsg, QMessageBox::Ok);
                        if (m_pTableWidgetParameters->columnCount() > 1)
                        {
                            QTableWidgetItem* pItem = m_pTableWidgetParameters->item(i, 1);
                            if (pItem)
                            {
                                pItem->setText("0");
                            }
                        }
                        return;
                    }
                }

                if (verrifyParameters(nID, paramsMap))
                {
                    m_pDissimilarPreviewer->drawDissimilar(nID, paramsMap);

                    // 每次参数改变都写入文件
                    DissimilarParametersWriter::write(nID, paramsMap);
                }
            }
        }
    }
    catch (QException& e)
    {
        qCritical("DialogDissimilarShapes::onParametersTableWidgetItemChanged() exception: %s", e.what());
    }
}

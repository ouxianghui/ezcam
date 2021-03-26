#include "DialogPrintPreviewer.h"
#include "ui_DialogPrintPreviewer.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QListWidgetItem>
#include <QAbstractItemView>
#include <QPixmap>
#include <QIcon>
#include <QFileInfo>
#include <algorithm>
#include <QMessageBox>
#include <QValidator>
#include <QVariant>
#include <QTextCodec>
#include <QException>
#include "SystemConfigManager.h"
#include "QR.h"
#include "Typedef.h"
#include "LabelDrawer.h"
#include "LabelPrinter.h"
#include "LabelPrinterInstance.h"


DialogPrintPreviewer::DialogPrintPreviewer(std::vector<BoardSt>& vecDataSt, QWidget *parent)
    : m_vecDataSt(vecDataSt),
      QDialog(parent),
      ui(new Ui::DialogPrintPreviewer)
{
    ui->setupUi(this);

    createWidgets();

    displayLabels2ListWidget();
}

DialogPrintPreviewer::~DialogPrintPreviewer()
{
    delete ui;
}

void DialogPrintPreviewer::createWidgets()
{
    //QTextCodec *tc = QTextCodec::codecForName("GBK");

    SystemConfigManager& smgr = SystemConfigManager::GetInstance();

    int nTemplate = smgr.getLabelTemplate();

    double dScale = 1.0;
    if (nTemplate == 0)
    {
        dScale = 3.0/5.0;
    }
    else if (nTemplate == 1)
    {
        dScale = 4.0/6.0;
    }
    else if (nTemplate == 2)
    {
        dScale = 3.0/4.0;
    }

    QSize icSize(PREVIEW_LABEL_SIZE, PREVIEW_LABEL_SIZE*dScale);
    QSize gdSize(PREVIEW_LABEL_SIZE+20, (PREVIEW_LABEL_SIZE+20)*dScale);

    QGridLayout* pGridLayout = new QGridLayout(this);
    this->setLayout(pGridLayout);

    m_pListWidget = new QListWidget(this);
    m_pListWidget->setIconSize(icSize);
    m_pListWidget->setGridSize(gdSize);
    m_pListWidget->setResizeMode(QListView::Adjust);
    m_pListWidget->setViewMode(QListView::IconMode);
    m_pListWidget->setFlow(QListView::LeftToRight);
    m_pListWidget->setMovement(QListView::Static);
    m_pListWidget->setSelectionMode(QListWidget::ExtendedSelection);
    m_pListWidget->setSelectionBehavior(QListWidget::SelectRows);
    m_pListWidget->setVerticalScrollMode(QListWidget::ScrollPerItem);
    m_pListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pListWidget->setAutoScroll(false);

    QPushButton* pButtonPrint = new QPushButton(this);
    pButtonPrint->setText(tr("打印"));
    connect(pButtonPrint, &QPushButton::clicked, this, &DialogPrintPreviewer::onPrintButtonClicked);

    QPushButton* pButtonClose = new QPushButton(this);
    pButtonClose->setText(tr("关闭"));
    connect(pButtonClose, &QPushButton::clicked, this, &DialogPrintPreviewer::onCloseButtonClicked);

    pGridLayout->addWidget(m_pListWidget, 0, 0, 19, 20);
    QHBoxLayout* pHLayout = new QHBoxLayout();

    pHLayout->addWidget(new QLabel(this), 18);
    pHLayout->addWidget(pButtonPrint, 1);
    pHLayout->addWidget(pButtonClose, 1);

    pGridLayout->addLayout(pHLayout, 19, 0, 1, 20);

}

void DialogPrintPreviewer::displayLabels2ListWidget()
{
    try
    {
        int nCount = m_pListWidget->count();
        for (int i = nCount; i >= 0; --i)
        {
            QListWidgetItem* pItem = m_pListWidget->takeItem(i);
            delete pItem;
            pItem = NULL;
        }

        SystemConfigManager& smgr = SystemConfigManager::GetInstance();
        int nTemplate = smgr.getLabelTemplate();
        int nLTContext = smgr.getLeftTopContext();

        std::vector<BoardSt>::const_iterator it = m_vecDataSt.begin();

        int nIdx = 0;
        for (; it != m_vecDataSt.end(); ++it)
        {
            const BoardSt& st = *it;

            QListWidgetItem* pItem = new QListWidgetItem(m_pListWidget);

            QImage image;
            LabelDrawer drawer(10.0);
            if (nTemplate == 0)
            {
                image = drawer.generateW5H3LabelImage(st);
            }
            else if (nTemplate == 1)
            {
                image = drawer.generateW6H4LabelImage(st, nLTContext);
            }
            else if (nTemplate == 2)
            {
                image = drawer.generateW4H3LabelImage(st);
            }

            QPixmap pixmap = QPixmap::fromImage(image);
            pItem->setData(Qt::DecorationRole, QIcon(pixmap));
            pItem->setData(Qt::DisplayRole, ++nIdx);

            m_pListWidget->addItem(pItem);
        }

    }
    catch (QException& e)
    {
        qCritical("DialogPrintPreviewer::displayLabels2ListWidget() exception: %s", e.what());
    }
}

void DialogPrintPreviewer::onPrintButtonClicked(bool /*checked*/)
{
    try
    {
        //QTextCodec *tc = QTextCodec::codecForName("GBK");

        LabelPrinterInstance& printerInstance = LabelPrinterInstance::GetInstance();

        SystemConfigManager& smgr = SystemConfigManager::GetInstance();
        int nTemplate = smgr.getLabelTemplate();
        int nLTContext = smgr.getLeftTopContext();

        LabelPrinter printer(printerInstance.getPrinter(), m_vecDataSt);

        printer.setLabelTemplate(nTemplate);

        if (nTemplate == 0)
        {
            printerInstance.getPrinter()->setPageSize(QPrinter::Custom);
            printerInstance.getPrinter()->setFullPage(true);
            printerInstance.getPrinter()->setPageSizeMM(QSizeF(50, 30));

            printer.setEdgeMargin(10.0);
            printer.setLineSpacing(1.0);
            printer.setBoardSize(44.0);
            printer.setQRCodeSize(44.0);
        }
        else if (nTemplate == 1)
        {
            printerInstance.getPrinter()->setPageSize(QPrinter::Custom);
            printerInstance.getPrinter()->setFullPage(true);
            printerInstance.getPrinter()->setPageSizeMM(QSizeF(60, 40));

            printer.setLeftTopContext(nLTContext);
        }
        else if (nTemplate == 2)
        {
            printerInstance.getPrinter()->setPageSize(QPrinter::Custom);
            printerInstance.getPrinter()->setFullPage(true);
            printerInstance.getPrinter()->setPageSizeMM(QSizeF(40, 30));

            printer.setEdgeMargin(10.0);
            printer.setLineSpacing(1.0);
            printer.setBoardSize(36.0);
            printer.setQRCodeSize(36.0);
        }

        printer.run();
    }
    catch (QException& e)
    {
        qCritical("DialogPrintPreviewer::onPrintButtonClicked() exception: %s", e.what());
    }
}

void DialogPrintPreviewer::onCloseButtonClicked(bool /*checked*/)
{
    QDialog::accept();
}

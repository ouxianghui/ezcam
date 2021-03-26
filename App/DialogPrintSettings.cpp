#include "DialogPrintSettings.h"
#include "ui_DialogPrintSettings.h"
#include <QTextCodec>
#include <QPrintDialog>
#include "SystemConfigManager.h"
#include "LabelDrawer.h"
#include "LabelPrinter.h"
#include "LabelPrinterInstance.h"

DialogPrintSettings::DialogPrintSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPrintSettings)
{
    ui->setupUi(this);

    SystemConfigManager& smgr = SystemConfigManager::GetInstance();
    int nTemplate = smgr.getLabelTemplate();
    int nLTContext = smgr.getLeftTopContext();
    ui->comboBoxTemplate->setCurrentIndex(nTemplate);
    ui->comboBoxLTContext->setCurrentIndex(nLTContext);

    QTextCodec* tc = QTextCodec::codecForName("GBK");

    m_boardSt.nBoardIndex = 2;
    m_boardSt.nSourceBoardIndex = 10;

    m_boardSt.dataSt.strBarCodeNo = "scm-150408C1-1001";
    m_boardSt.dataSt.strClientInformation = tc->toUnicode("ACEMAN");
    m_boardSt.dataSt.strDissimilarShapes = tc->toUnicode("ID=11");
    m_boardSt.dataSt.strEdgeWeight = tc->toUnicode("0.5 1.0 1.5 2.0");
    m_boardSt.dataSt.strFinishedProductName = tc->toUnicode("活动层板");
    m_boardSt.dataSt.strProductName = tc->toUnicode("A1型衣柜");
    m_boardSt.dataSt.strMaterialType = tc->toUnicode("颗粒板");
    m_boardSt.dataSt.nBoardWidth = 1220;
    m_boardSt.dataSt.nBoardHeight = 2440;
    m_boardSt.dataSt.nBoardThickness = 18;

    QImage image;
    LabelDrawer drawer(10.0);

    if (nTemplate == 0)
    {
        image = drawer.generateW5H3LabelImage(m_boardSt);
    }
    else if (nTemplate == 1)
    {
        image = drawer.generateW6H4LabelImage(m_boardSt, nLTContext);
    }
    else if (nTemplate == 2)
    {
        image = drawer.generateW4H3LabelImage(m_boardSt);
    }

    ui->drawer->drawImage(image);
}

DialogPrintSettings::~DialogPrintSettings()
{
    delete ui;
}

void DialogPrintSettings::on_pushButtonPrintTest_clicked()
{
    //QTextCodec *tc = QTextCodec::codecForName("GBK");

    LabelPrinterInstance& printerInstance = LabelPrinterInstance::GetInstance();

    std::vector<BoardSt> vecBoardSt;
    vecBoardSt.push_back(m_boardSt);
    LabelPrinter labelprinter(printerInstance.getPrinter(), vecBoardSt);

    int nTemplate = ui->comboBoxTemplate->currentIndex();
    int nLTContext = ui->comboBoxLTContext->currentIndex();

    labelprinter.setLabelTemplate(nTemplate);

    if (nTemplate == 0)
    {
        printerInstance.getPrinter()->setPageSize(QPrinter::Custom);
        printerInstance.getPrinter()->setFullPage(true);
        printerInstance.getPrinter()->setPageSizeMM(QSizeF(50, 30));

        labelprinter.setEdgeMargin(10.0);
        labelprinter.setLineSpacing(1.0);
        labelprinter.setBoardSize(44.0);
        labelprinter.setQRCodeSize(44.0);
    }
    else if (nTemplate == 1)
    {
        printerInstance.getPrinter()->setPageSize(QPrinter::Custom);
        printerInstance.getPrinter()->setFullPage(true);
        printerInstance.getPrinter()->setPageSizeMM(QSizeF(60, 40));

        labelprinter.setLeftTopContext(nLTContext);
    }
    else if (nTemplate == 2)
    {
        printerInstance.getPrinter()->setPageSize(QPrinter::Custom);
        printerInstance.getPrinter()->setFullPage(true);
        printerInstance.getPrinter()->setPageSizeMM(QSizeF(40, 30));

        labelprinter.setEdgeMargin(10.0);
        labelprinter.setLineSpacing(1.0);
        labelprinter.setBoardSize(36.0);
        labelprinter.setQRCodeSize(36.0);
    }

    labelprinter.run();
}

void DialogPrintSettings::on_pushButtonOk_clicked()
{
    SystemConfigManager& smgr = SystemConfigManager::GetInstance();
    smgr.setLabelTemplate(ui->comboBoxTemplate->currentIndex());
    smgr.setLeftTopContext(ui->comboBoxLTContext->currentIndex());

    QDialog::accept();
}

void DialogPrintSettings::on_pushButtonCancel_clicked()
{
    QDialog::reject();
}

void DialogPrintSettings::on_pushButtonSelectPrinter_clicked()
{
    LabelPrinterInstance& printerInstance = LabelPrinterInstance::GetInstance();

    QPrintDialog printDialog(printerInstance.getPrinter(), NULL);
    if (printDialog.exec() == QPrintDialog::Accepted)
    {
    }
}

void DialogPrintSettings::on_comboBoxTemplate_currentIndexChanged(int index)
{
    LabelDrawer drawer(10.0);
    if (index == 0)
    {
        QImage image = drawer.generateW5H3LabelImage(m_boardSt);
        ui->drawer->drawImage(image);
    }
    else if (index == 1)
    {
        int nLTContext = ui->comboBoxLTContext->currentIndex();
        QImage image = drawer.generateW6H4LabelImage(m_boardSt, nLTContext);
        ui->drawer->drawImage(image);
    }
    else if (index == 2)
    {
        QImage image = drawer.generateW4H3LabelImage(m_boardSt);
        ui->drawer->drawImage(image);
    }
}

void DialogPrintSettings::on_comboBoxLTContext_currentIndexChanged(int index)
{
    LabelDrawer drawer(10.0);
    if (ui->comboBoxTemplate->currentIndex() == 0)
    {
        QImage image = drawer.generateW5H3LabelImage(m_boardSt);
        ui->drawer->drawImage(image);
    }
    else if (ui->comboBoxTemplate->currentIndex() == 1)
    {
        QImage image = drawer.generateW6H4LabelImage(m_boardSt, index);
        ui->drawer->drawImage(image);
    }
    else if (ui->comboBoxTemplate->currentIndex() == 2)
    {
        QImage image = drawer.generateW4H3LabelImage(m_boardSt);
        ui->drawer->drawImage(image);
    }
}

#include "LabelPrinterInstance.h"
#include "SystemConfigManager.h"

LabelPrinterInstance& LabelPrinterInstance::GetInstance()
{
    static LabelPrinterInstance printer;
    return printer;
}

LabelPrinterInstance::LabelPrinterInstance(void)
    : m_pPrinter(NULL)
{

    m_pPrinter = new QPrinter();
    m_pPrinter->setPageSize(QPrinter::Custom);
    m_pPrinter->setFullPage(true);
    //m_pPrinter->setPageSizeMM(QSizeF(50, 30));
    m_pPrinter->setOutputFormat(QPrinter::NativeFormat);
    m_pPrinter->setOrientation(QPrinter::Portrait);
}

LabelPrinterInstance::~LabelPrinterInstance(void)
{
    if (m_pPrinter)
    {
        delete m_pPrinter;
        m_pPrinter = NULL;
    }
}

QPrinter* LabelPrinterInstance::getPrinter()
{
   return m_pPrinter;
}

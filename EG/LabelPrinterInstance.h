#ifndef LABELPRINTERINSTANCE_H
#define LABELPRINTERINSTANCE_H

#include "eg_global.h"
#include <QPrinter>

class EGSHARED_EXPORT LabelPrinterInstance
{
public:
    static LabelPrinterInstance& GetInstance();
    QPrinter* getPrinter();

private:
    LabelPrinterInstance();
    LabelPrinterInstance(const LabelPrinterInstance& printer);
    LabelPrinterInstance& operator=(const LabelPrinterInstance& printer);
    ~LabelPrinterInstance();

private:
    QPrinter* m_pPrinter;
};

#endif // LABELPRINTERINSTANCE_H

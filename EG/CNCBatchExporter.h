#ifndef CNCBATCHEXPORTER_H
#define CNCBATCHEXPORTER_H

#include "eg_global.h"

class EGSHARED_EXPORT CNCBatchExporter
{
public:
    CNCBatchExporter();
    ~CNCBatchExporter();

    static bool exportCNC(const QString& strNCExportDir);

};

#endif // CNCBATCHEXPORTER_H

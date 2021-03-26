#ifndef DissimilarParametersReader_H
#define DissimilarParametersReader_H

#include "eg_global.h"
#include <QXmlStreamReader>
#include <map>

class EGSHARED_EXPORT DissimilarParametersReader
{
public:
    DissimilarParametersReader();
    ~DissimilarParametersReader();

    static bool read(int nID, std::map<QString, QVariant>& paramsMap);
};

#endif // DissimilarParametersReader_H

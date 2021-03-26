#ifndef DISSIMILARPARAMETERSWRITER_H
#define DISSIMILARPARAMETERSWRITER_H

#include "eg_global.h"
#include <map>
#include <QXmlStreamWriter>
#include <QVariant>

class EGSHARED_EXPORT DissimilarParametersWriter
{
public:
    DissimilarParametersWriter();
    ~DissimilarParametersWriter();

    static bool write(int nID, const std::map<QString, QVariant>& paramsMap);
};

#endif // DISSIMILARPARAMETERSWRITER_H

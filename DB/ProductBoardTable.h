#ifndef PRODUCTBOARDTABLE_H
#define PRODUCTBOARDTABLE_H

#include "db_global.h"
#include <QString>
#include <vector>
#include <map>
#include "DataSt.h"

class DBSHARED_EXPORT ProductBoardTable
{
public:
    ProductBoardTable();
    ~ProductBoardTable();

    static bool updateDissimilarParametersByBarcodeNo(const QString& strBarcodeNo, const QString& strParameters);

    static bool getBoardByBarcodeNo(const QString& strBarcodeNo, DataSt& dataSt);

private:
    static std::string errorMsg(const QSqlError& error);
};

#endif // PRODUCTBOARDTABLE_H

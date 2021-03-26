#ifndef MATERIALLISTTABLE_H
#define MATERIALLISTTABLE_H

#include "db_global.h"
#include <QString>
#include <vector>
#include <map>
#include "DataSt.h"

class DBSHARED_EXPORT MaterialListTable
{
public:
    MaterialListTable();
    ~MaterialListTable();

    // 参数:strName,料单文件名;vecData,料单数据，传入后DataSt中的strMaterialListID字段会被更新
    static bool addMaterialList(const QString& strName, std::vector<DataSt>& vecData);

    static bool getMaterialLists(std::map<QString, std::vector<DataSt>>& materialMap);

    static bool deletetMaterialListByID(const QString& strID);

    static bool deletetMaterialListByName(const QString& strName);

private:
    static std::string errorMsg(const QSqlError& error);
};

#endif // MATERIALLISTTABLE_H

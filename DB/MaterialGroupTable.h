#ifndef MaterialGroupTable_H
#define MaterialGroupTable_H

#include "db_global.h"
#include <vector>

typedef struct MaterialGroupSt
{
    QString strMaterialGroupID;
    QString strMaterialGroupName;
    QString strMaterialListID;

    MaterialGroupSt()
        : strMaterialGroupID(""),
          strMaterialGroupName(""),
          strMaterialListID("")
    {

    }
}MaterialGroupSt;

class DBSHARED_EXPORT MaterialGroupTable
{
public:
    MaterialGroupTable();
    ~MaterialGroupTable();

    static bool addGroup(MaterialGroupSt& st);
    static bool addGroups(std::vector<MaterialGroupSt>& vecData);
    static bool getGroupsByMaterialListID(const QString& strMaterialListID, std::vector<MaterialGroupSt>& vecData);
    static bool deleteGroupByMaterialListID(const QString& strMaterialListID);
    static bool deleteGroupByID(const QString& strGroupID);

private:
    static std::string errorMsg(const QSqlError& error);
};

#endif // MaterialGroupTable_H

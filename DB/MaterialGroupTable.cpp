#include "MaterialGroupTable.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QException>
#include <QUuid>
#include <QSqlError>
#include "DatabaseManager.h"

MaterialGroupTable::MaterialGroupTable()
{

}

MaterialGroupTable::~MaterialGroupTable()
{

}

bool MaterialGroupTable::addGroup(MaterialGroupSt& st)
{
    try
    {
        QSqlDatabase db;
        CDatabaseManager& dbMgr = CDatabaseManager::GetInstance();

        if (QSqlDatabase::contains(dbMgr.GetConnectionName()))
        {
            db = QSqlDatabase::database(dbMgr.GetConnectionName());
        }
        else
        {
            db = QSqlDatabase::addDatabase(dbMgr.GetDriverName(), dbMgr.GetConnectionName());
        }
        db.setDatabaseName(dbMgr.GetDatabasePathName());

        if (!db.open())
        {
            qCritical("Can not open database:%s, %s.\n",
                      dbMgr.GetDatabasePathName().toStdString().c_str(),
                      errorMsg(db.lastError()).c_str());
            return false;
        }

        if (!db.isValid())
        {
            qCritical("Invalid database:%s, %s.\n",
                      dbMgr.GetDatabasePathName().toStdString().c_str(),
                      errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        // 添加料单分组
        st.strMaterialGroupID = QUuid::createUuid().toString().remove("{").remove("}");

        QSqlQuery query(db);
        QString strSQL;
        strSQL.sprintf("INSERT INTO material_group (mgid, mlid, name) VALUES ('%s', '%s', '%s');",
                       st.strMaterialGroupID.toStdString().c_str(),
                       st.strMaterialListID.toStdString().c_str(),
                       st.strMaterialGroupName.toStdString().c_str());

        if (!query.exec(strSQL))
        {
            qCritical("INSERT INTO 'material_group' failed, %s.\n", errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        db.close();

        return true;
    }
    catch(QException& e)
    {
        qCritical("数据库操作异常：%s", e.what());
        return false;
    }
}

bool MaterialGroupTable::addGroups(std::vector<MaterialGroupSt>& vecData)
{
    try
    {
        QSqlDatabase db;
        CDatabaseManager& dbMgr = CDatabaseManager::GetInstance();

        if (QSqlDatabase::contains(dbMgr.GetConnectionName()))
        {
            db = QSqlDatabase::database(dbMgr.GetConnectionName());
        }
        else
        {
            db = QSqlDatabase::addDatabase(dbMgr.GetDriverName(), dbMgr.GetConnectionName());
        }
        db.setDatabaseName(dbMgr.GetDatabasePathName());

        if (!db.open())
        {
            qCritical("Can not open database:%s, %s.\n",
                      dbMgr.GetDatabasePathName().toStdString().c_str(),
                      errorMsg(db.lastError()).c_str());
            return false;
        }

        if (!db.isValid())
        {
            qCritical("Invalid database:%s, %s.\n",
                      dbMgr.GetDatabasePathName().toStdString().c_str(),
                      errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        QSqlQuery query(db);
        QString strSQL;

        if (db.transaction())
        {
            // 批量添加料单分组
            for (std::vector<MaterialGroupSt>::iterator it = vecData.begin(); it != vecData.end(); ++it)
            {
                MaterialGroupSt& st = *it;

                st.strMaterialGroupID = QUuid::createUuid().toString().remove("{").remove("}");

                strSQL.sprintf("INSERT INTO material_group (mgid, mlid, name) VALUES ('%s', '%s', '%s');",
                               st.strMaterialGroupID.toStdString().c_str(),
                               st.strMaterialListID.toStdString().c_str(),
                               st.strMaterialGroupName.toStdString().c_str());

                if (!query.exec(strSQL))
                {
                    qCritical("INSERT INTO 'material_group' failed, %s.\n", errorMsg(db.lastError()).c_str());
                    db.close();
                    return false;
                }
            }

            if (!db.commit())
            {
                qCritical("Commit transaction failed, %s.\n", errorMsg(db.lastError()).c_str());
                db.close();
                return false;
            }
        }
        else
        {
            qCritical("Begin transaction failed, %s.\n", errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        db.close();

        return true;
    }
    catch(QException& e)
    {
        qCritical("数据库操作异常：%s", e.what());
        return false;
    }
}

bool MaterialGroupTable::getGroupsByMaterialListID(const QString& strMaterialListID, std::vector<MaterialGroupSt>& vecData)
{
    try
    {
        QSqlDatabase db;
        CDatabaseManager& dbMgr = CDatabaseManager::GetInstance();

        if (QSqlDatabase::contains(dbMgr.GetConnectionName()))
        {
            db = QSqlDatabase::database(dbMgr.GetConnectionName());
        }
        else
        {
            db = QSqlDatabase::addDatabase(dbMgr.GetDriverName(), dbMgr.GetConnectionName());
        }
        db.setDatabaseName(dbMgr.GetDatabasePathName());

        if (!db.open())
        {
            qCritical("Can not open database:%s, %s.\n",
                      dbMgr.GetDatabasePathName().toStdString().c_str(),
                      errorMsg(db.lastError()).c_str());
            return false;
        }

        if (!db.isValid())
        {
            qCritical("Invalid database:%s, %s.\n",
                      dbMgr.GetDatabasePathName().toStdString().c_str(),
                      errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        QSqlQuery query(db);
        QString strSQL;
        strSQL.sprintf("SELECT * FROM material_group WHERE mlid = '%s';", strMaterialListID.toStdString().c_str());

        MaterialGroupSt st;
        if (query.exec(strSQL))
        {
            while(query.next())
            {
                st.strMaterialGroupID = query.value("mgid").toString();
                st.strMaterialGroupName = query.value("name").toString();
                st.strMaterialListID = query.value("mlid").toString();

                vecData.push_back(st);
            }
        }
        else
        {
            qCritical("SELECT FROM 'material_group' failed, %s.\n", errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        db.close();

        return true;
    }
    catch(QException& e)
    {
        qCritical("数据库操作异常：%s", e.what());
        return false;
    }
}

bool MaterialGroupTable::deleteGroupByMaterialListID(const QString& strMaterialListID)
{
    try
    {
        QSqlDatabase db;
        CDatabaseManager& dbMgr = CDatabaseManager::GetInstance();

        if (QSqlDatabase::contains(dbMgr.GetConnectionName()))
        {
            db = QSqlDatabase::database(dbMgr.GetConnectionName());
        }
        else
        {
            db = QSqlDatabase::addDatabase(dbMgr.GetDriverName(), dbMgr.GetConnectionName());
        }
        db.setDatabaseName(dbMgr.GetDatabasePathName());

        if (!db.open())
        {
            qCritical("Can not open database:%s, %s.\n",
                      dbMgr.GetDatabasePathName().toStdString().c_str(),
                      errorMsg(db.lastError()).c_str());
            return false;
        }

        if (!db.isValid())
        {
            qCritical("Invalid database:%s, %s.\n",
                      dbMgr.GetDatabasePathName().toStdString().c_str(),
                      errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        QSqlQuery query(db);
        QString strSQL;
        strSQL.sprintf("DELETE FROM material_group WHERE mlid = '%s';",
                       strMaterialListID.toStdString().c_str());

        if (!query.exec(strSQL))
        {
            qCritical("DELETE FROM 'material_group' failed, %s.\n", errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        db.close();

        return true;
    }
    catch(QException& e)
    {
        qCritical("数据库操作异常：%s", e.what());
        return false;
    }
}

bool MaterialGroupTable::deleteGroupByID(const QString& strGroupID)
{
    try
    {
        QSqlDatabase db;
        CDatabaseManager& dbMgr = CDatabaseManager::GetInstance();

        if (QSqlDatabase::contains(dbMgr.GetConnectionName()))
        {
            db = QSqlDatabase::database(dbMgr.GetConnectionName());
        }
        else
        {
            db = QSqlDatabase::addDatabase(dbMgr.GetDriverName(), dbMgr.GetConnectionName());
        }
        db.setDatabaseName(dbMgr.GetDatabasePathName());

        if (!db.open())
        {
            qCritical("Can not open database:%s, %s.\n",
                      dbMgr.GetDatabasePathName().toStdString().c_str(),
                      errorMsg(db.lastError()).c_str());
            return false;
        }

        if (!db.isValid())
        {
            qCritical("Invalid database:%s, %s.\n",
                      dbMgr.GetDatabasePathName().toStdString().c_str(),
                      errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        QSqlQuery query(db);
        QString strSQL;
        strSQL.sprintf("DELETE FROM material_group WHERE mgid = '%s';",
                       strGroupID.toStdString().c_str());

        if (!query.exec(strSQL))
        {
            qCritical("DELETE FROM 'material_group' failed, %s.\n", errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        db.close();

        return true;
    }
    catch(QException& e)
    {
        qCritical("数据库操作异常：%s", e.what());
        return false;
    }
}

std::string MaterialGroupTable::errorMsg(const QSqlError& error)
{
    QString strMsg;
    strMsg.sprintf("error:%s, code:%s", error.text().toStdString().c_str(), error.nativeErrorCode().toStdString().c_str());
    return strMsg.toStdString();
}

#include "CuttingSolutionTable.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QException>
#include <QUuid>
#include "DatabaseManager.h"

CuttingSolutionTable::CuttingSolutionTable()
{

}

CuttingSolutionTable::~CuttingSolutionTable()
{

}

bool CuttingSolutionTable::addSolution(CuttingSolutionSt& st)
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
        st.strSolutionID = QUuid::createUuid().toString().remove("{").remove("}");

        QSqlQuery query(db);
        QString strSQL;
        strSQL.sprintf("INSERT INTO cutting_solution (csid, mgid, name, total_target_area, source_board_width, source_board_height, source_board_thickness, total_source_board_num, total_utilization_rate)"
                       "VALUES ('%s', '%s', '%s', %lf, %d, %d, %d, %d, %lf);",
                       st.strSolutionID.toStdString().c_str(),
                       st.strMateriakGroupID.toStdString().c_str(),
                       st.strSolutionName.toStdString().c_str(),
                       st.dTotalTargetArea,
                       st.nSourceBoardWidth,
                       st.nSourceBoardHeight,
                       st.nSourceBoardThickness,
                       st.nTotalSourceBoardNum,
                       st.dTotalUtilizationRate);

        if (!query.exec(strSQL))
        {
            qCritical("INSERT INTO 'cutting_solution' failed, %s.\n", errorMsg(db.lastError()).c_str());
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

bool CuttingSolutionTable::addSolutions(std::vector<CuttingSolutionSt>& vecData)
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

        if (db.transaction())
        {
            for (std::vector<CuttingSolutionSt>::iterator it = vecData.begin(); it != vecData.end(); ++it)
            {
                CuttingSolutionSt& dataSt = *it;

                dataSt.strSolutionID = QUuid::createUuid().toString().remove("{").remove("}");

                QString strSQL;
                strSQL.sprintf("INSERT INTO cutting_solution (csid, mgid, name, total_target_area, source_board_width, source_board_height, source_board_thickness, total_source_board_num, total_utilization_rate)"
                               "VALUES ('%s', '%s', '%s', %lf, %d, %d, %d, %d, %lf);",
                               dataSt.strSolutionID.toStdString().c_str(),
                               dataSt.strMateriakGroupID.toStdString().c_str(),
                               dataSt.strSolutionName.toStdString().c_str(),
                               dataSt.dTotalTargetArea,
                               dataSt.nSourceBoardWidth,
                               dataSt.nSourceBoardHeight,
                               dataSt.nSourceBoardWidth,
                               dataSt.nTotalSourceBoardNum,
                               dataSt.dTotalUtilizationRate);
                if (!query.exec(strSQL))
                {
                    qCritical("Insert into table 'cutting_solution' failed, %s.\n", errorMsg(db.lastError()).c_str());
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

bool CuttingSolutionTable::getSolutionsByMaterialGroupID(const QString& strGroupID, std::vector<CuttingSolutionSt>& vecData)
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
        strSQL.sprintf("SELECT * FROM cutting_solution WHERE mgid = '%s';", strGroupID.toStdString().c_str());

        if (query.exec(strSQL))
        {
            vecData.clear();

            CuttingSolutionSt dataSt;

            while(query.next())
            {
                dataSt.strSolutionID = query.value("csid").toString();
                dataSt.strSolutionName = query.value("name").toString();
                dataSt.strMateriakGroupID = query.value("mgid").toString();
                dataSt.dTotalTargetArea = query.value("total_target_area").toDouble();
                dataSt.nSourceBoardWidth = query.value("source_board_width").toInt();
                dataSt.nSourceBoardHeight = query.value("source_board_height").toInt();
                dataSt.nSourceBoardThickness = query.value("source_board_thickness").toInt();
                dataSt.nTotalSourceBoardNum = query.value("total_source_board_num").toInt();
                dataSt.dTotalUtilizationRate = query.value("total_utilization_rate").toDouble();
                vecData.push_back(dataSt);
            }
        }
        else
        {
            qCritical("Select from 'cutting_solution' failed, %s.\n", errorMsg(db.lastError()).c_str());
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

bool CuttingSolutionTable::deleteSolutionByMaterialGroupID(const QString& strGroupID)
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
        strSQL.sprintf("DELETE FROM cutting_solution WHERE mgid = '%s';", strGroupID.toStdString().c_str());
        if (!query.exec(strSQL))
        {
            qCritical("Delete from 'cutting_solution' failed, %s.\n", errorMsg(db.lastError()).c_str());
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

bool CuttingSolutionTable::deleteSolutionByID(const QString& strID)
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
        strSQL.sprintf("DELETE FROM cutting_solution WHERE csid = '%s';", strID.toStdString().c_str());
        if (!query.exec(strSQL))
        {
            qCritical("Delete from 'cutting_solution' failed, %s.\n", errorMsg(db.lastError()).c_str());
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

std::string CuttingSolutionTable::errorMsg(const QSqlError& error)
{
    QString strMsg;
    strMsg.sprintf("error:%s, code:%s", error.text().toStdString().c_str(), error.nativeErrorCode().toStdString().c_str());
    return strMsg.toStdString();
}

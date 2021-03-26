#include "SourceBoardTable.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QException>
#include <QUuid>
#include "DatabaseManager.h"

SourceBoardTable::SourceBoardTable()
{

}

SourceBoardTable::~SourceBoardTable()
{

}

bool SourceBoardTable::addBoards(std::vector<SourceBoardSt>& vecData)
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
            for (std::vector<SourceBoardSt>::iterator it = vecData.begin(); it != vecData.end(); ++it)
            {
                SourceBoardSt& dataSt = *it;
                dataSt.strBoardID = QUuid::createUuid().toString().remove("{").remove("}");
                QString strSQL;
                strSQL.sprintf("INSERT INTO source_board (sbid, csid, width, height, thickness, idx, num, utilization_area, utilization_rate, dxf_file_name)"
                               "VALUES ('%s', '%s', %d, %d, %d, %d, %d, %lf, %lf, '%s');",
                               dataSt.strBoardID.toStdString().c_str(),
                               dataSt.strSolutionID.toStdString().c_str(),
                               dataSt.nWidth,
                               dataSt.nHeight,
                               dataSt.nThickness,
                               dataSt.nIdx,
                               dataSt.nNum,
                               dataSt.dUtilizationArea,
                               dataSt.dUtilizationRate,
                               dataSt.strDxfFileName.toStdString().c_str());

                if (!query.exec(strSQL))
                {
                    qCritical("Insert into table 'source_board' failed, %s.\n", errorMsg(db.lastError()).c_str());
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

bool SourceBoardTable::getBoardByID(const QString& strID, SourceBoardSt& dataSt)
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
        strSQL.sprintf("SELECT * FROM source_board WHERE sbid = '%s';", strID.toStdString().c_str());

        if (query.exec(strSQL))
        {
            while(query.next())
            {
                dataSt.strBoardID = query.value("sbid").toString();
                dataSt.strSolutionID = query.value("csid").toString();
                dataSt.nWidth = query.value("width").toInt();
                dataSt.nHeight = query.value("height").toInt();
                dataSt.nThickness = query.value("thickness").toInt();
                dataSt.nIdx = query.value("idx").toInt();
                dataSt.nNum = query.value("num").toInt();
                dataSt.dUtilizationArea = query.value("utilization_area").toDouble();
                dataSt.dUtilizationRate = query.value("utilization_rate").toDouble();
                dataSt.strDxfFileName = query.value("dxf_file_name").toString();
            }
        }
        else
        {
            qCritical("Select from 'source_board' failed, %s.\n", errorMsg(db.lastError()).c_str());
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

bool SourceBoardTable::getBoardsBySolutionID(const QString& strID, std::vector<SourceBoardSt>& vecData)
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
        strSQL.sprintf("SELECT * FROM source_board WHERE csid = '%s';", strID.toStdString().c_str());

        if (query.exec(strSQL))
        {
            vecData.clear();

            SourceBoardSt dataSt;

            while(query.next())
            {
                dataSt.strBoardID = query.value("sbid").toString();
                dataSt.strSolutionID = query.value("csid").toString();
                dataSt.nWidth = query.value("width").toInt();
                dataSt.nHeight = query.value("height").toInt();
                dataSt.nThickness = query.value("thickness").toInt();
                dataSt.nIdx = query.value("idx").toInt();
                dataSt.nNum = query.value("num").toInt();
                dataSt.dUtilizationArea = query.value("utilization_area").toDouble();
                dataSt.dUtilizationRate = query.value("utilization_rate").toDouble();
                dataSt.strDxfFileName = query.value("dxf_file_name").toString();
                vecData.push_back(dataSt);
            }
        }
        else
        {
            qCritical("Select from 'source_board' failed, %s.\n", errorMsg(db.lastError()).c_str());
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


bool SourceBoardTable::deleteBoardsBySolutionID(const QString& strID)
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
        strSQL.sprintf("DELETE FROM source_board WHERE csid = '%s';", strID.toStdString().c_str());

        if (!query.exec(strSQL))
        {
            qCritical("Delete from 'source_board' failed, %s.\n", errorMsg(db.lastError()).c_str());
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

std::string SourceBoardTable::errorMsg(const QSqlError& error)
{
    QString strMsg;
    strMsg.sprintf("error:%s, code:%s", error.text().toStdString().c_str(), error.nativeErrorCode().toStdString().c_str());
    return strMsg.toStdString();
}

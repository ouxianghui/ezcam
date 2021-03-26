#include "BoardRectTable.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QException>
#include <QUuid>
#include "DatabaseManager.h"

BoardRectTable::BoardRectTable()
{

}

BoardRectTable::~BoardRectTable()
{

}

bool BoardRectTable::addBoardRects(std::vector<BoardRectSt>& vecRect)
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
            for (std::vector<BoardRectSt>::iterator it = vecRect.begin(); it != vecRect.end(); ++it)
            {
                BoardRectSt& dataSt = *it;
                dataSt.strBoardRectID = QUuid::createUuid().toString().remove("{").remove("}");
                QString strSQL;
                strSQL.sprintf("INSERT INTO board_rect (brid, sbid, name, barcode_no, idx, lt_x, lt_y, rb_x, rb_y, nc_start_pt_x, nc_start_pt_y)"
                               "VALUES ('%s', '%s', '%s', '%s', %d, %lf, %lf, %lf, %lf, %lf, %lf);",
                               dataSt.strBoardRectID.toStdString().c_str(),
                               dataSt.strSourceBoardID.toStdString().c_str(),
                               dataSt.strBoardRectName.toStdString().c_str(),
                               dataSt.strBarcodeNo.toStdString().c_str(),
                               dataSt.nIdx,
                               dataSt.dLtX,
                               dataSt.dLtY,
                               dataSt.dRbX,
                               dataSt.dRbY,
                               dataSt.dNcStartPtX,
                               dataSt.dNcStartPtY);

                if (!query.exec(strSQL))
                {
                    qCritical("Insert into table 'board_rect' failed, %s.\n", errorMsg(db.lastError()).c_str());
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

bool BoardRectTable::getBoardRectsBySourceBoardID(const QString& strSourceBoardID, std::vector<BoardRectSt>& vecRect)
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
        strSQL.sprintf("SELECT * FROM board_rect WHERE sbid = '%s';", strSourceBoardID.toStdString().c_str());

        if (query.exec(strSQL))
        {
            vecRect.clear();

            BoardRectSt dataSt;

            while(query.next())
            {
                dataSt.strBoardRectID = query.value("brid").toString();
                dataSt.strSourceBoardID = query.value("sbid").toString();
                dataSt.strBoardRectName = query.value("name").toString();
                dataSt.strBarcodeNo = query.value("barcode_no").toString();
                dataSt.nIdx = query.value("idx").toInt();
                dataSt.dLtX = query.value("lt_x").toDouble();
                dataSt.dLtY = query.value("lt_y").toDouble();
                dataSt.dRbX = query.value("rb_x").toDouble();
                dataSt.dRbY = query.value("rb_y").toDouble();
                dataSt.dNcStartPtX = query.value("nc_start_pt_x").toDouble();
                dataSt.dNcStartPtY = query.value("nc_start_pt_y").toDouble();
                vecRect.push_back(dataSt);
            }
        }
        else
        {
            qCritical("Select from 'board_rect' failed, %s.\n", errorMsg(db.lastError()).c_str());
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

bool BoardRectTable::updateBoardRectIndexByID(const QString& strBoardRectID, int nNewIndex)
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
        strSQL.sprintf("UPDATE board_rect SET idx = %d WHERE brid = '%s';",
                       nNewIndex,
                       strBoardRectID.toStdString().c_str());

        if (!query.exec(strSQL))
        {
            qCritical("UPDATE 'board_rect' failed, %s.\n", errorMsg(db.lastError()).c_str());
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

bool BoardRectTable::updateBoardCuttingPointByID(const QString& strBoardRectID, double dNcStartPtX, double dNcStartPtY)
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
        strSQL.sprintf("UPDATE board_rect SET nc_start_pt_x = %lf, nc_start_pt_y = %lf WHERE brid = '%s';",
                       dNcStartPtX,
                       dNcStartPtY,
                       strBoardRectID.toStdString().c_str());

        if (!query.exec(strSQL))
        {
            qCritical("UPDATE 'board_rect' failed, %s.\n", errorMsg(db.lastError()).c_str());
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

bool BoardRectTable::deleteBoardRectsBySourceBoardID(const QString& strSourceBoardID)
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
        strSQL.sprintf("DELETE FROM board_rect WHERE sbid = '%s';", strSourceBoardID.toStdString().c_str());

        if (!query.exec(strSQL))
        {
            qCritical("Delete from 'board_rect' failed, %s.\n", errorMsg(db.lastError()).c_str());
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

std::string BoardRectTable::errorMsg(const QSqlError& error)
{
    QString strMsg;
    strMsg.sprintf("error:%s, code:%s", error.text().toStdString().c_str(), error.nativeErrorCode().toStdString().c_str());
    return strMsg.toStdString();
}

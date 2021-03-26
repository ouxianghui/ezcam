#include "ProductBoardTable.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QException>
#include <QUuid>
#include <QSqlError>
#include "DatabaseManager.h"

ProductBoardTable::ProductBoardTable()
{

}

ProductBoardTable::~ProductBoardTable()
{

}

bool ProductBoardTable::updateDissimilarParametersByBarcodeNo(const QString& strBarcodeNo, const QString& strParameters)
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
        strSQL.sprintf("UPDATE product_board SET dissimilar_shapes = '%s' WHERE barcode_no = '%s';",
                       strParameters.toStdString().c_str(),
                       strBarcodeNo.toStdString().c_str());

        if (!query.exec(strSQL))
        {
            qCritical("UPDATE 'product_board' failed, %s.\n", errorMsg(db.lastError()).c_str());
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

std::string ProductBoardTable::errorMsg(const QSqlError& error)
{
    QString strMsg;
    strMsg.sprintf("error:%s, code:%s", error.text().toStdString().c_str(), error.nativeErrorCode().toStdString().c_str());
    return strMsg.toStdString();
}

bool ProductBoardTable::getBoardByBarcodeNo(const QString& strBarcodeNo, DataSt& dataSt)
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
        strSQL.sprintf("SELECT * FROM product_board WHERE barcode_no = '%s'", strBarcodeNo.toStdString().c_str());

        if (query.exec(strSQL))
        {
            while(query.next())
            {
                dataSt.strMaterialListID = query.value("mlid").toString();
                dataSt.strProductBoardID = query.value("pbid").toString();
                dataSt.strOrdersNo = query.value("orders_no").toString();
                dataSt.strBarCodeNo = query.value("barcode_no").toString();
                dataSt.strProductName = query.value("product_name").toString();
                dataSt.strFinishedProductName = query.value("finished_product_name").toString();
                dataSt.strMaterialType = query.value("material_type").toString();
                dataSt.nBoardWidth = query.value("board_width").toInt();
                dataSt.nBoardHeight = query.value("board_height").toInt();
                dataSt.nBoardThickness = query.value("board_thickness").toInt();
                dataSt.nBoardNum = query.value("num").toInt();
                dataSt.strEdgeWeight = query.value("edge_weight").toString();
                dataSt.nFinishedProductWidth = query.value("finished_product_width").toInt();
                dataSt.nFinishedProductHeight = query.value("finished_product_height").toInt();
                dataSt.nFinishedProductThickness = query.value("finished_product_thickness").toInt();
                dataSt.strDissimilarShapes = query.value("dissimilar_shapes").toString();
                dataSt.nRotate = query.value("rotate").toInt();
                dataSt.strClientInformation = query.value("client_information").toString();
                dataSt.strFranchiseStore = query.value("franchies_store").toString();
                dataSt.nIsRotated = query.value("is_rotated").toInt();
            }
        }
        else
        {
            qCritical("SELECT FROM 'product_board' failed, %s.\n", errorMsg(db.lastError()).c_str());
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

#include "MaterialListTable.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QException>
#include <QUuid>
#include <QSqlError>
#include "DatabaseManager.h"

MaterialListTable::MaterialListTable()
{

}

MaterialListTable::~MaterialListTable()
{

}

bool MaterialListTable::addMaterialList(const QString& strName, std::vector<DataSt>& vecData)
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

        // 添加一张料单
        QString strUUID = QUuid::createUuid().toString().remove("{").remove("}");

        QSqlQuery query(db);
        QString strSQL;
        strSQL.sprintf("INSERT INTO material_list (mlid, name) VALUES ('%s', '%s');",
                       strUUID.toStdString().c_str(), strName.toStdString().c_str());

        if (!query.exec(strSQL))
        {
            qCritical("Insert into table 'material_list' failed, %s.\n", errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        //query.clear();

        if (db.transaction())
        {
            QString strMaterialListID = strUUID;

            // 取大料单拆分后的子料单
            for (std::vector<DataSt>::iterator it = vecData.begin(); it != vecData.end(); ++it)
            {
                DataSt& st = *it;

                QString strProductBoardID = QUuid::createUuid().toString().remove("{").remove("}");
                st.strMaterialListID = strMaterialListID;
                QString strSQL;
                strSQL.sprintf("INSERT INTO product_board (pbid, mlid, orders_no, barcode_no, product_name,"
                               "finished_product_name, material_type, board_width, board_height, board_thickness,"
                               "num, edge_weight, finished_product_width, finished_product_height, finished_product_thickness, dissimilar_shapes, rotate,"
                               "client_information, franchies_store) "
                               "VALUES ('%s', '%s', '%s', '%s', '%s',"
                               "'%s', '%s', %d, %d, %d,"
                               "%d, '%s', %d, %d, %d, '%s', %d,"
                               "'%s', '%s');",
                               strProductBoardID.toStdString().c_str(), strMaterialListID.toStdString().c_str(), st.strOrdersNo.toStdString().c_str(), st.strBarCodeNo.toStdString().c_str(), st.strProductName.toStdString().c_str(),
                               st.strFinishedProductName.toStdString().c_str(), st.strMaterialType.toStdString().c_str(), st.nBoardWidth, st.nBoardHeight, st.nBoardThickness,
                               st.nBoardNum, st.strEdgeWeight.toStdString().c_str(), st.nFinishedProductWidth, st.nFinishedProductHeight, st.nFinishedProductThickness, st.strDissimilarShapes.toStdString().c_str(), st.nRotate,
                               st.strClientInformation.toStdString().c_str(), st.strFranchiseStore.toStdString().c_str());
                if (!query.exec(strSQL))
                {
                    qCritical("Insert into table 'product_board' failed, %s.\n", errorMsg(db.lastError()).c_str());
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

bool MaterialListTable::getMaterialLists(std::map<QString, std::vector<DataSt>>& materialMap)
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
        strSQL = "SELECT * FROM material_list";
        std::map<QString, QString> mlMap;
        if (query.exec(strSQL))
        {
            while(query.next())
            {
                QString strID = query.value("mlid").toString();
                QString strName = query.value("name").toString();
                mlMap[strID] = strName;
            }
        }
        else
        {
            qCritical("SELECT FROM 'material_list' failed, %s.\n", errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        for (std::map<QString, QString>::iterator it = mlMap.begin(); it != mlMap.end(); ++it)
        {
            QString strID = it->first;
            QString strSQL;
            strSQL.sprintf("SELECT * FROM product_board WHERE mlid = '%s'", strID.toStdString().c_str());
            std::vector<DataSt> vecData;
            if (query.exec(strSQL))
            {
                while(query.next())
                {
                    DataSt st;
                    st.strMaterialListID = query.value("mlid").toString();
                    st.strProductBoardID = query.value("pbid").toString();
                    st.strOrdersNo = query.value("orders_no").toString();
                    st.strBarCodeNo = query.value("barcode_no").toString();
                    st.strProductName = query.value("product_name").toString();
                    st.strFinishedProductName = query.value("finished_product_name").toString();
                    st.strMaterialType = query.value("material_type").toString();
                    st.nBoardWidth = query.value("board_width").toInt();
                    st.nBoardHeight = query.value("board_height").toInt();
                    st.nBoardThickness = query.value("board_thickness").toInt();
                    st.nBoardNum = query.value("num").toInt();
                    st.strEdgeWeight = query.value("edge_weight").toString();
                    st.nFinishedProductWidth = query.value("finished_product_width").toInt();
                    st.nFinishedProductHeight = query.value("finished_product_height").toInt();
                    st.nFinishedProductThickness = query.value("finished_product_thickness").toInt();
                    st.strDissimilarShapes = query.value("dissimilar_shapes").toString();
                    st.nRotate = query.value("rotate").toInt();
                    st.strClientInformation = query.value("client_information").toString();
                    st.strFranchiseStore = query.value("franchies_store").toString();
                    st.nIsRotated = query.value("is_rotated").toInt();

                    vecData.push_back(st);
                }

                materialMap[mlMap[strID]] = vecData;
            }
            else
            {
                qCritical("SELECT FROM 'product_board' failed, %s.\n", errorMsg(db.lastError()).c_str());
                db.close();
                return false;
            }
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


bool MaterialListTable::deletetMaterialListByID(const QString& strID)
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
        strSQL.sprintf("DELETE FROM product_board WHERE mlid = '%s'", strID.toStdString().c_str());
        if (!query.exec(strSQL))
        {
            qCritical("DELETE FROM 'product_board' failed, %s.\n", errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        strSQL.sprintf("DELETE FROM material_list WHERE mlid = '%s'", strID.toStdString().c_str());

        if (!query.exec(strSQL))
        {
            qCritical("SELECT FROM 'material_list' failed, %s.\n", errorMsg(db.lastError()).c_str());
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

bool MaterialListTable::deletetMaterialListByName(const QString& strName)
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
        strSQL.sprintf("SELECT * FROM material_list WHERE name = '%s'", strName.toStdString().c_str());

        if (query.exec(strSQL))
        {
            while(query.next())
            {
                QString strID = query.value("mlid").toString();

                strSQL.sprintf("DELETE FROM product_board WHERE mlid = '%s'", strID.toStdString().c_str());
                if (!query.exec(strSQL))
                {
                    qCritical("DELETE FROM 'product_board' failed, %s.\n", errorMsg(db.lastError()).c_str());
                    db.close();
                    return false;
                }

                strSQL.sprintf("DELETE FROM material_list WHERE mlid = '%s'", strID.toStdString().c_str());

                if (!query.exec(strSQL))
                {
                    qCritical("SELECT FROM 'material_list failed, %s.\n", errorMsg(db.lastError()).c_str());
                    db.close();
                    return false;
                }
            }
        }
        else
        {
            qCritical("SELECT FROM 'material_list failed, %s.\n", errorMsg(db.lastError()).c_str());
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

std::string MaterialListTable::errorMsg(const QSqlError& error)
{
    QString strMsg;
    strMsg.sprintf("error:%s, code:%s", error.text().toStdString().c_str(), error.nativeErrorCode().toStdString().c_str());
    return strMsg.toStdString();
}

#include "DB.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QException>
#include "DatabaseManager.h"

DB::DB()
{
}

bool DB::createDatabase()
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
            qCritical("无法打开数据库:%s.\n", dbMgr.GetDatabasePathName().toStdString().c_str());
            return false;
        }

        if (!db.isValid())
        {
            qCritical("无效的数据库:%s.\n", dbMgr.GetDatabasePathName().toStdString().c_str());
            return false;
        }

        QSqlQuery query(db);

        QString strSQL = "DROP TABLE IF EXISTS board_rect;";

        if (!query.exec(strSQL))
        {
            qCritical("Drop table 'board_rect' failed, %s\n", errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        strSQL = "DROP TABLE IF EXISTS source_board;";

        if (!query.exec(strSQL))
        {
            qCritical("Drop table 'source_board' failed, %s\n", errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        strSQL = "DROP TABLE IF EXISTS cutting_solution;";

        if (!query.exec(strSQL))
        {
            qCritical("Drop table 'cutting_solution' failed, %s.\n", errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        strSQL = "DROP TABLE IF EXISTS material_group;";

        if (!query.exec(strSQL))
        {
            qCritical("Drop table 'material_group' failed, %s\n", errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        strSQL = "DROP TABLE IF EXISTS product_board;";

        if (!query.exec(strSQL))
        {
            qCritical("Drop table 'product_board' failed, %s.\n", errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        strSQL = "DROP TABLE IF EXISTS material_list;";

        if (!query.exec(strSQL))
        {
            qCritical("Drop table 'material_list' failed, %s.\n", errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        strSQL = "CREATE TABLE material_list ("
                 "mlid CHAR (36) PRIMARY KEY,"
                 "name TEXT NOT NULL);";

        if (!query.exec(strSQL))
        {
            qCritical("Create table 'material_list' failed, %s.\n", errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        strSQL = "CREATE TABLE product_board ("
                 "pbid CHAR (36) PRIMARY KEY,"
                 "mlid CHAR(36),"
                 "orders_no TEXT,"
                 "barcode_no TEXT,"
                 "product_name TEXT,"
                 "finished_product_name TEXT,"
                 "material_type TEXT,"
                 "board_width INT,"
                 "board_height INT,"
                 "board_thickness INT,"
                 "num INT,"
                 "edge_weight TEXT,"
                 "finished_product_width INT,"
                 "finished_product_height INT,"
                 "finished_product_thickness INT,"
                 "dissimilar_shapes TEXT,"
                 "rotate INT,"
                 "is_rotated INT,"
                 "client_information TEXT,"
                 "franchies_store TEXT);";

        if (!query.exec(strSQL))
        {
            qCritical("Create table 'product_board' failed, %s.\n", errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        strSQL = "CREATE TABLE material_group ("
                 "mgid CHAR (36) PRIMARY KEY,"
                 "name TEXT NOT NULL,"
                 "mlid CHAR (36) NOT NULL,"
                 "FOREIGN KEY (mlid) REFERENCES material_list (mlid) ON DELETE CASCADE ON UPDATE CASCADE);";

        if (!query.exec(strSQL))
        {
            qCritical("Create table 'material_group' failed, %s.\n", errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        strSQL = "CREATE TABLE cutting_solution ("
                 "csid CHAR (36) PRIMARY KEY,"
                 "name TEXT,"
                 "total_target_area FLOAT,"
                 "source_board_width INT,"
                 "source_board_height INT,"
                 "source_board_thickness INT,"
                 "total_source_board_num INT,"
                 "total_utilization_rate FLOAT,"
                 "mgid CHAR (36) NOT NULL,"
                 "FOREIGN KEY (mgid) REFERENCES material_group (mgid) ON DELETE CASCADE ON UPDATE CASCADE);";

        if (!query.exec(strSQL))
        {
            qCritical("Create table cutting_solution failed, %s.\n", errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        strSQL = "CREATE TABLE source_board ("
                 "sbid CHAR (36) PRIMARY KEY,"
                 "csid CHAR (36) NOT NULL,"
                 "width INT,"
                 "height INT,"
                 "thickness INT,"
                 "idx INTEGER,"
                 "num INTEGER,"
                 "utilization_area FLOAT,"
                 "utilization_rate FLOAT,"
                 "dxf_file_name TEXT,"
                 "FOREIGN KEY (csid) REFERENCES material_list (csid) ON DELETE CASCADE ON UPDATE CASCADE);";

        if (!query.exec(strSQL))
        {
            qCritical("Create table 'source_board' failed, %s.\n", errorMsg(db.lastError()).c_str());
            db.close();
            return false;
        }

        strSQL = "CREATE TABLE board_rect ("
                "brid CHAR (36) PRIMARY KEY,"
                "sbid CHAR (36) NOT NULL,"
                "name TEXT,"
                "barcode_no TEXT,"
                "idx INTEGER,"
                "lt_x FLOAT NOT NULL,"
                "lt_y FLOAT NOT NULL,"
                "rb_x FLOAT NOT NULL,"
                "rb_y FLOAT NOT NULL,"
                "nc_start_pt_x FLOAT,"
                "nc_start_pt_y FLOAT,"
                "FOREIGN KEY (sbid) REFERENCES source_board (sbid) ON DELETE CASCADE ON UPDATE CASCADE);";

        if (!query.exec(strSQL))
        {
            qCritical("Create table 'board_rect' failed, %s.\n", errorMsg(db.lastError()).c_str());
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

std::string DB::errorMsg(const QSqlError& error)
{
    QString strMsg;
    strMsg.sprintf("error:%s, code:%s", error.text().toStdString().c_str(), error.nativeErrorCode().toStdString().c_str());
    return strMsg.toStdString();
}

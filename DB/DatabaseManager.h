#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "db_global.h"
#include <QString>

class DBSHARED_EXPORT CDatabaseManager
{
public:
    static CDatabaseManager& GetInstance();

    QString GetDriverName();
    void SetDriverName(const QString& strDriverName);

    QString GetConnectionName();
    void SetConnectionName(const QString& strConnectionName);

    QString GetDatabaseName();
    void SetDatabaseName(const QString& strDatabaseName);

    // For Sqlite
    QString GetDatabasePathName();
    void SetDatabasePathName(const QString& strDatabasePathName);

    QString GetHostName();
    void SetHostName(const QString& strHostName);

    QString GetUserName();
    void SetUserName(const QString& strUserName);

    QString GetPassowrd();
    void SetPassword(const QString& strPassword);

    QString GetConnectOptions();
    void SetConnectOptions(const QString& strOptions);

    QString GetPort();
    void SetPort(const QString& strPort);

private:
    CDatabaseManager(void);
    CDatabaseManager(const CDatabaseManager& dbMgr);
    CDatabaseManager& operator=(const CDatabaseManager& dbMgr);

protected:
    virtual ~CDatabaseManager(void);

private:
    QString m_strDriverName;
    QString m_strConnectionName;
    QString m_strDatabaseName;
    QString m_strDatabasePathName;
    QString m_strHostName;
    QString m_strUserName;
    QString m_strPassword;
    QString m_strConnectOptions;
    QString m_strPort;
};

#endif // DATABASEMANAGER_H

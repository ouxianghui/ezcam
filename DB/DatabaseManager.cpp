#include "DatabaseManager.h"

CDatabaseManager::CDatabaseManager()
{
}

CDatabaseManager::~CDatabaseManager()
{
}

CDatabaseManager& CDatabaseManager::GetInstance()
{
    static CDatabaseManager dbMgr;
    return dbMgr;
}

QString CDatabaseManager::GetDriverName()
{
    return m_strDriverName;
}

void CDatabaseManager::SetDriverName(const QString& strDriverName)
{
    m_strDriverName = strDriverName;
}

QString CDatabaseManager::GetConnectionName()
{
    if (m_strConnectionName.isEmpty())
    {
        return "video-synopsis-connection";
    }
    return m_strConnectionName;
}

void CDatabaseManager::SetConnectionName(const QString& strConnectionName)
{
    if (strConnectionName.isEmpty())
    {
        return;
    }
    m_strConnectionName = strConnectionName;
}

QString CDatabaseManager::GetDatabaseName()
{
    return m_strDatabaseName;
}

void CDatabaseManager::SetDatabaseName(const QString& strDatabaseName)
{
    if (strDatabaseName.isEmpty())
    {
        return;
    }
    m_strDatabaseName = strDatabaseName;
}

QString CDatabaseManager::GetDatabasePathName()
{
    return m_strDatabasePathName;
}

void CDatabaseManager::SetDatabasePathName(const QString& strDatabasePathName)
{
    if (strDatabasePathName.isEmpty())
    {
        return;
    }
    m_strDatabasePathName = strDatabasePathName;
}

QString CDatabaseManager::GetHostName()
{
   return m_strHostName;
}

void CDatabaseManager::SetHostName(const QString& strHostName)
{
    if (strHostName.isEmpty())
    {
        return;
    }
   m_strHostName = strHostName;
}

QString CDatabaseManager::GetUserName()
{
    return m_strUserName;
}

void CDatabaseManager::SetUserName(const QString& strUserName)
{
    if (strUserName.isEmpty())
    {
        return;
    }
    m_strUserName = strUserName;
}

QString CDatabaseManager::GetPassowrd()
{
    return m_strPassword;
}

void CDatabaseManager::SetPassword(const QString& strPassword)
{
    if (strPassword.isEmpty())
    {
        return;
    }
    m_strPassword = strPassword;
}

QString CDatabaseManager::GetConnectOptions()
{
    return m_strConnectOptions;
}

void CDatabaseManager::SetConnectOptions(const QString& strOptions)
{
    if (strOptions.isEmpty())
    {
        return;
    }
    m_strConnectOptions = strOptions;
}

QString CDatabaseManager::GetPort()
{
    return m_strPort;
}

void CDatabaseManager::SetPort(const QString& strPort)
{
    if (strPort.isEmpty())
    {
        return;
    }
    m_strPort = strPort;
}

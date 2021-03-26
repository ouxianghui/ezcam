#include "ProjectConfigManager.h"
#include <iostream>
#include <fstream>
#include <QApplication>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QTextCodec>
#include <assert.h>

ProjectConfigManager& ProjectConfigManager::GetInstance()
{
    static ProjectConfigManager mgr;

    return mgr;
}

ProjectConfigManager::ProjectConfigManager(void)
    : m_strProjectPath(""),
      m_pSettings(NULL),
      m_isOpened(false)
{
}

ProjectConfigManager::~ProjectConfigManager(void)
{
    if (m_pSettings)
    {
        delete m_pSettings;
        m_pSettings = NULL;
    }
}

int ProjectConfigManager::openConfigFile(const QString& strFilePathName)
{
    assert(!strFilePathName.isEmpty());

    QFileInfo fileInfo(strFilePathName);
    if (!fileInfo.isFile() || !fileInfo.exists())
    {
        QFile file(strFilePathName);
        file.open(QFile::ReadWrite);
        file.close();
    }

    m_strProjectPath = fileInfo.path();

    if (fileInfo.isFile() && fileInfo.exists())
    {
        if (m_pSettings)
        {
            delete m_pSettings;
            m_pSettings = NULL;
            m_isOpened = false;
        }

        m_pSettings = new QSettings(strFilePathName, QSettings::IniFormat);
        m_pSettings->setIniCodec("UTF8");

        int nRet = m_pSettings->isWritable() ? 0 : 1;

        m_isOpened = true;

        return nRet;
    }
    else
    {
        QTextCodec *tc = QTextCodec::codecForName("GBK");
        QString strMsg = QString(tc->toUnicode("无法创建工程配置文件 : %1")).arg(strFilePathName);
        QMessageBox::critical(NULL, tc->toUnicode("消息"), strMsg, tc->toUnicode("知道了"));
        return 2;
    }
}

bool ProjectConfigManager::isProjectOpened()
{
    return m_isOpened;
}

QString ProjectConfigManager::getProjectPath()
{
    return m_strProjectPath;
}

void ProjectConfigManager::addMaterialFiles(const QStringList& stringList)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginWriteArray("Project Files");

        for (int i = 0; i < stringList.size(); ++i)
        {
            m_pSettings->setArrayIndex(i);
            m_pSettings->setValue("file", stringList.at(i));
        }
        m_pSettings->endArray();
    }
}

QStringList ProjectConfigManager::getMaterialFiles()
{
    assert(m_pSettings);
    QStringList list;
    if (m_pSettings)
    {
        int nSize = m_pSettings->beginReadArray("Project Files");

        for (int i = 0; i < nSize; ++i)
        {
            m_pSettings->setArrayIndex(i);
            QString strValue = m_pSettings->value("file", "").toString();
            list.push_back(strValue);
        }

        m_pSettings->endArray();

    }
    return list;
}

void ProjectConfigManager::setProjectName(const QString& strName)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Project Name");
        m_pSettings->setValue("name", strName);
        m_pSettings->endGroup();
    }
}

QString ProjectConfigManager::getProjectName()
{
    assert(m_pSettings);
    QString strName;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Project Name");
        strName = m_pSettings->value("name").toString();
        m_pSettings->endGroup();
    }
    return strName;
}

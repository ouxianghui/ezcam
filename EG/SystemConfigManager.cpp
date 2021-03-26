#include "SystemConfigManager.h"
#include <iostream>
#include <fstream>
#include <QApplication>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QTextCodec>
#include <QDir>
#include <assert.h>

SystemConfigManager& SystemConfigManager::GetInstance()
{
    static SystemConfigManager mgr;
    return mgr;
}

SystemConfigManager::SystemConfigManager(void)
    : m_pSettings(NULL)
{
    Initialize();
}

SystemConfigManager::~SystemConfigManager(void)
{
    if (m_pSettings)
    {
        delete m_pSettings;
        m_pSettings = NULL;
    }
}

void SystemConfigManager::Initialize()
{
    QString strFilePathName = QString("%1/%2").arg(QApplication::applicationDirPath()).arg("sys.ini");

    QFileInfo fileInfo(strFilePathName);
    if (!fileInfo.isFile())
    {
        QFile file(strFilePathName);
        file.open(QFile::ReadWrite);
        file.close();
    }

    if (fileInfo.isFile())
    {
        m_pSettings = new QSettings(strFilePathName, QSettings::IniFormat);
        m_pSettings->setIniCodec("UTF8");
    }
    else
    {
        QTextCodec *tc = QTextCodec::codecForName("GBK");
        QString strMsg = QString(tc->toUnicode("无法创建系统配置文件 : %1")).arg(strFilePathName);
        QMessageBox::critical(NULL, tc->toUnicode("消息"), strMsg, tc->toUnicode("知道了"));
    }
}

void SystemConfigManager::setWorkingPath(const QString& strPath)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Working Path");
        m_pSettings->setValue("path", strPath);
        m_pSettings->endGroup();
    }
}

QString SystemConfigManager::getWorkingPath()
{
    assert(m_pSettings);
    QString strPath;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Working Path");
        strPath = m_pSettings->value("path", QDir::homePath()).toString();
        m_pSettings->endGroup();
    }
    return strPath;
}


void SystemConfigManager::setRecentlyUsedMaterialFileDir(const QString& strPath)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Recently Used Material File Path");
        m_pSettings->setValue("path", strPath);
        m_pSettings->endGroup();
    }
}

QString SystemConfigManager::getRecentlyUsedMaterialFileDir()
{
    assert(m_pSettings);
    QString strPath;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Recently Used Material File Path");
        strPath = m_pSettings->value("path", QDir::homePath()).toString();
        m_pSettings->endGroup();
    }
    return strPath;
}

void SystemConfigManager::setRecentlyUsedDxfFileDir(const QString& strPath)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Recently Used Dxf File Path");
        m_pSettings->setValue("path", strPath);
        m_pSettings->endGroup();
    }
}

QString SystemConfigManager::getRecentlyUsedDxfFileDir()
{
    assert(m_pSettings);
    QString strPath;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Recently Used Dxf File Path");
        strPath = m_pSettings->value("path", QDir::homePath()).toString();
        m_pSettings->endGroup();
    }
    return strPath;
}

void SystemConfigManager::setRecentlyUsedDExportBitmapFileDir(const QString& strPath)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Recently Used Bitmap File Path");
        m_pSettings->setValue("path", strPath);
        m_pSettings->endGroup();
    }
}

QString SystemConfigManager::getRecentlyUsedExportBitmapFileDir()
{
    assert(m_pSettings);
    QString strPath;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Recently Used Bitmap File Path");
        strPath = m_pSettings->value("path", QDir::homePath()).toString();
        m_pSettings->endGroup();
    }
    return strPath;
}

void SystemConfigManager::setRecentlyUsedDExportNCFileDir(const QString& strPath)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Recently Used NC File Path");
        m_pSettings->setValue("path", strPath);
        m_pSettings->endGroup();
    }
}

QString SystemConfigManager::getRecentlyUsedExportNCFileDir()
{
    assert(m_pSettings);
    QString strPath;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Recently Used NC File Path");
        strPath = m_pSettings->value("path", QDir::homePath()).toString();
        m_pSettings->endGroup();
    }
    return strPath;
}

void SystemConfigManager::setSplitMaterialsListByType(int nFlag)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Split Materials List By Type");
        m_pSettings->setValue("flag", nFlag);
        m_pSettings->endGroup();
    }
}

int SystemConfigManager::getSplitMaterialsListByType()
{
    assert(m_pSettings);
    int nFlag = 0;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Split Materials List By Type");
        nFlag = m_pSettings->value("flag", 0).toInt();
        m_pSettings->endGroup();
    }
    return nFlag;
}

void SystemConfigManager::setSplitMaterialsListByThickness(int nFlag)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Split Materials List By Thickness");
        m_pSettings->setValue("flag", nFlag);
        m_pSettings->endGroup();
    }
}

int SystemConfigManager::getSplitMaterialsListByThickness()
{
    assert(m_pSettings);
    int nFlag = 1;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Split Materials List By Thickness");
        nFlag = m_pSettings->value("flag", 1).toInt();
        m_pSettings->endGroup();
    }
    return nFlag;
}

void SystemConfigManager::setSourceMaterialBoardWidth(int nWidth)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Source Material Board Width");
        m_pSettings->setValue("width", nWidth);
        m_pSettings->endGroup();
    }
}

int SystemConfigManager::getSourceMaterialBoardWidth()
{
    assert(m_pSettings);
    int nWidth = 0;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Source Material Board Width");
        nWidth = m_pSettings->value("width", 0).toInt();
        m_pSettings->endGroup();
    }
    return nWidth;
}

void SystemConfigManager::setSourceMaterialBoardHeight(int nHeight)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Source Material Board Height");
        m_pSettings->setValue("height", nHeight);
        m_pSettings->endGroup();
    }
}

int SystemConfigManager::getSourceMaterialBoardHeight()
{
    assert(m_pSettings);
    int nHeight = 0;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Source Material Board Height");
        nHeight = m_pSettings->value("height", 0).toInt();
        m_pSettings->endGroup();
    }
    return nHeight;
}

void SystemConfigManager::setSourceMaterialBoardTrimming(int nTrim)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Source Material Board Trimming");
        m_pSettings->setValue("trimming", nTrim);
        m_pSettings->endGroup();
    }
}

int SystemConfigManager::getSourceMaterialBoardTrimming()
{
    assert(m_pSettings);
    int nTrim = 0;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Source Material Board Trimming");
        nTrim = m_pSettings->value("trimming", 0).toInt();
        m_pSettings->endGroup();
    }
    return nTrim;
}

void SystemConfigManager::setBoardSpacing(int nSpace)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Board Spacing");
        m_pSettings->setValue("spacing", nSpace);
        m_pSettings->endGroup();
    }
}

int SystemConfigManager::getBoardSpacing()
{
    assert(m_pSettings);
    int nSpace = 0;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Board Spacing");
        nSpace = m_pSettings->value("spacing", 0).toInt();
        m_pSettings->endGroup();
    }
    return nSpace;
}

void SystemConfigManager::setCuttingDepth(double dDepth)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Cutting Depth");
        m_pSettings->setValue("depth", dDepth);
        m_pSettings->endGroup();
    }
}

double SystemConfigManager::getCuttingDepth()
{
    assert(m_pSettings);
    double dDepth = 0;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Cutting Depth");
        dDepth = m_pSettings->value("depth", 0).toDouble();
        m_pSettings->endGroup();
    }
    return dDepth;
}

void SystemConfigManager::setMaxSolutionNum(int nNum)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Max Solution Num");
        m_pSettings->setValue("num", nNum);
        m_pSettings->endGroup();
    }
}

int SystemConfigManager::getMaxSolutionNum()
{
    assert(m_pSettings);
    int nNum = 0;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Max Solution Num");
        nNum = m_pSettings->value("num", 5).toInt();
        m_pSettings->endGroup();
    }
    return nNum;
}

void SystemConfigManager::setKinfeDiameter(double dDiameter)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Kinfe Diameter");
        m_pSettings->setValue("diameter", dDiameter);
        m_pSettings->endGroup();
    }
}

double SystemConfigManager::getKinfeDiameter()
{
    assert(m_pSettings);
    double dDiameter = 0;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Kinfe Diameter");
        dDiameter = m_pSettings->value("diameter", 0).toDouble();
        m_pSettings->endGroup();
    }
    return dDiameter;
}

void SystemConfigManager::setProcessingSpeed(int nSpeed)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Processing Speed");
        m_pSettings->setValue("speed", nSpeed);
        m_pSettings->endGroup();
    }
}
int SystemConfigManager::getProcessingSpeed()
{
    assert(m_pSettings);
    int nSpeed = 0;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Processing Speed");
        nSpeed = m_pSettings->value("speed", 0).toInt();
        m_pSettings->endGroup();
    }
    return nSpeed;
}

void SystemConfigManager::setCuttingSpeed(int nSpeed)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Cutting Speed");
        m_pSettings->setValue("speed", nSpeed);
        m_pSettings->endGroup();
    }
}

int SystemConfigManager::getCuttingSpeed()
{
    assert(m_pSettings);
    int nSpeed = 0;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Cutting Speed");
        nSpeed = m_pSettings->value("speed", 0).toInt();
        m_pSettings->endGroup();
    }
    return nSpeed;
}

void SystemConfigManager::setTerminalSpeed(int nSpeed)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Terminal Speed");
        m_pSettings->setValue("speed", nSpeed);
        m_pSettings->endGroup();
    }
}

int SystemConfigManager::getTerminalSpeed()
{
    assert(m_pSettings);
    int nSpeed = 0;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Terminal Speed");
        nSpeed = m_pSettings->value("speed", 0).toInt();
        m_pSettings->endGroup();
    }
    return nSpeed;
}

void SystemConfigManager::setSafetyHeight(double dHeight)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Safety Height");
        m_pSettings->setValue("height", dHeight);
        m_pSettings->endGroup();
    }
}

double SystemConfigManager::getSafetyHeight()
{
    assert(m_pSettings);
    double dHeight = 0;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Safety Height");
        dHeight = m_pSettings->value("height", 0).toDouble();
        m_pSettings->endGroup();
    }
    return dHeight;
}

void SystemConfigManager::setObliqueDistance(double dDistance)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Oblique Distance");
        m_pSettings->setValue("distance", dDistance);
        m_pSettings->endGroup();
    }
}

double SystemConfigManager::getObliqueDistance()
{
    assert(m_pSettings);
    double dDistance = 0;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Oblique Distance");
        dDistance = m_pSettings->value("distance", 0).toDouble();
        m_pSettings->endGroup();
    }
    return dDistance;
}

void SystemConfigManager::setNCFileHeader(const QString& strHeader)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("NC File Header");
        m_pSettings->setValue("header", strHeader);
        m_pSettings->endGroup();
    }
}

QString SystemConfigManager::getNCFileHeader()
{
    assert(m_pSettings);
    QString strHeader;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("NC File Header");
        strHeader = m_pSettings->value("header", 0).toString();
        m_pSettings->endGroup();
    }
    return strHeader;
}

void SystemConfigManager::setNCFileTail(const QString& strTail)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("NC File Tail");
        m_pSettings->setValue("tail", strTail);
        m_pSettings->endGroup();
    }
}

QString SystemConfigManager::getNCFileTail()
{
    assert(m_pSettings);
    QString strTail;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("NC File Tail");
        strTail = m_pSettings->value("tail", 0).toString();
        m_pSettings->endGroup();
    }
    return strTail;
}

void SystemConfigManager::setLabelTemplate(int nTemplate)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Label Template");
        m_pSettings->setValue("template", nTemplate);
        m_pSettings->endGroup();
    }
}

int SystemConfigManager::getLabelTemplate()
{
    assert(m_pSettings);
    int nTemplate = 0;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Label Template");
        nTemplate = m_pSettings->value("template", 1).toInt();
        m_pSettings->endGroup();
    }
    return nTemplate;
}

void SystemConfigManager::setLeftTopContext(int nContext)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Left Top Context");
        m_pSettings->setValue("context", nContext);
        m_pSettings->endGroup();
    }
}

int SystemConfigManager::getLeftTopContext()
{
    assert(m_pSettings);
    int nContext = 0;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Left Top Context");
        nContext = m_pSettings->value("context", 0).toInt();
        m_pSettings->endGroup();
    }
    return nContext;
}

void SystemConfigManager::setSlotKnifeType(const QString& type)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Slot Knife Type");
        m_pSettings->setValue("type", type);
        m_pSettings->endGroup();
    }
}

QString SystemConfigManager::getSlotKnifeType()
{
    assert(m_pSettings);
    QString type;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Slot Knife Type");
        type = m_pSettings->value("type", "T1").toString();
        m_pSettings->endGroup();
    }
    return type;
}

void SystemConfigManager::setUseDrills(int nFlag)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Use Drill");
        m_pSettings->setValue("flag", nFlag);
        m_pSettings->endGroup();
    }
}

int SystemConfigManager::getUseDrills()
{
    assert(m_pSettings);
    int nFlag = 0;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Use Drill");
        nFlag = m_pSettings->value("flag", 0).toInt();
        m_pSettings->endGroup();
    }
    return nFlag;
}

void SystemConfigManager::setDrills(const QMap<QString, QVariant>& map)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Drills");
        m_pSettings->setValue("drills", map);
        m_pSettings->endGroup();
    }
}

void SystemConfigManager::getDrills(QMap<QString, QVariant>& map)
{
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Drills");
        map = m_pSettings->value("drills", 0).toMap();
        m_pSettings->endGroup();
    }
}

void SystemConfigManager::setKnifeChangeMode(const QString& mode)
{
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Knife Change Mode");
        m_pSettings->setValue("mode", mode);
        m_pSettings->endGroup();
    }
}

QString SystemConfigManager::getKnifeChangeMode()
{
    assert(m_pSettings);
    QString type;
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Knife Change Mode");
        type = m_pSettings->value("mode").toString();
        m_pSettings->endGroup();
    }
    return type;
}

void SystemConfigManager::setHoles(const QMap<QString, QVariant>& map)
{
    assert(m_pSettings);
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Holes");
        m_pSettings->setValue("holes", map);
        m_pSettings->endGroup();
    }
}

void SystemConfigManager::getHoles(QMap<QString, QVariant>& map)
{
    if (m_pSettings)
    {
        m_pSettings->beginGroup("Holes");
        map = m_pSettings->value("holes", 0).toMap();
        m_pSettings->endGroup();
    }
}

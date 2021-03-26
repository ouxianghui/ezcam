#ifndef SYSTEMCONFIGMANAGER_H
#define SYSTEMCONFIGMANAGER_H

#include "eg_global.h"

#include <QSettings>
#include <vector>

class EGSHARED_EXPORT SystemConfigManager
{
public:
    static SystemConfigManager& GetInstance();

private:
    SystemConfigManager();
    SystemConfigManager(const SystemConfigManager& manager);
    SystemConfigManager& operator=(const SystemConfigManager& manager);
    ~SystemConfigManager();

protected:
    //在此函数里面做所有的初始化工作
    void Initialize();

public:

    void setWorkingPath(const QString& strPath);
    QString getWorkingPath();

    void setRecentlyUsedMaterialFileDir(const QString& strPath);
    QString getRecentlyUsedMaterialFileDir();

    void setRecentlyUsedDxfFileDir(const QString& strPath);
    QString getRecentlyUsedDxfFileDir();

    void setRecentlyUsedDExportBitmapFileDir(const QString& strPath);
    QString getRecentlyUsedExportBitmapFileDir();

    void setRecentlyUsedDExportNCFileDir(const QString& strPath);
    QString getRecentlyUsedExportNCFileDir();

    void setSplitMaterialsListByType(int nFlag);
    int getSplitMaterialsListByType();

    void setSplitMaterialsListByThickness(int nFlag);
    int getSplitMaterialsListByThickness();

    void setSourceMaterialBoardWidth(int nWidth);
    int getSourceMaterialBoardWidth();

    void setSourceMaterialBoardHeight(int nHeight);
    int getSourceMaterialBoardHeight();

    void setSourceMaterialBoardTrimming(int nTrim);
    int getSourceMaterialBoardTrimming();

    void setBoardSpacing(int nSpace);
    int getBoardSpacing();

    void setCuttingDepth(double dDepth);
    double getCuttingDepth();

    void setMaxSolutionNum(int nNum);
    int getMaxSolutionNum();

    void setKinfeDiameter(double dDiameter);
    double getKinfeDiameter();

    void setProcessingSpeed(int nSpeed);
    int getProcessingSpeed();

    void setCuttingSpeed(int nSpeed);
    int getCuttingSpeed();

    void setTerminalSpeed(int nSpeed);
    int getTerminalSpeed();

    void setSafetyHeight(double dHeight);
    double getSafetyHeight();

    void setObliqueDistance(double dDistance);
    double getObliqueDistance();

    void setNCFileHeader(const QString& strHeader);
    QString getNCFileHeader();

    void setNCFileTail(const QString& strTail);
    QString getNCFileTail();

    // nTemplate=0: 5cm x 3cm, nTemplate=1: 6cm x 4cm, nTemplate=2: 7cm x 5cm
    void setLabelTemplate(int nTemplate);
    int getLabelTemplate();

    // nContext=0: logo, nContext=1: client information
    void setLeftTopContext(int nContext);
    int getLeftTopContext();

    void setSlotKnifeType(const QString& type);
    QString getSlotKnifeType();

    void setUseDrills(int nFlag);
    int getUseDrills();

    void setDrills(const QMap<QString, QVariant>& map);
    void getDrills(QMap<QString, QVariant>& map);

    void setKnifeChangeMode(const QString& mode);
    QString getKnifeChangeMode();


    void setHoles(const QMap<QString, QVariant>& map);
    void getHoles(QMap<QString, QVariant>& map);

private:
    QSettings* m_pSettings;
};

#endif // SYSTEMCONFIGMANAGER_H

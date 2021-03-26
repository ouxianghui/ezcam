#ifndef PROJECTCONFIGMANAGER_H
#define PROJECTCONFIGMANAGER_H

#include "eg_global.h"

#include <QSettings>
#include <vector>
using namespace std;

class EGSHARED_EXPORT ProjectConfigManager
{
public:
    static ProjectConfigManager& GetInstance();

    // openConfigFile()函数设打开工程配置文件
    // 返回值：0，打开成功且可读写；1，打开成功但不可读写；2，打开文件失败
    int openConfigFile(const QString& strFilePathName);

    bool isProjectOpened();

    // 在openConfigFile()后调用
    QString getProjectPath();

private:
    ProjectConfigManager();
    ProjectConfigManager(const ProjectConfigManager& manager);
    ProjectConfigManager& operator=(const ProjectConfigManager& manager);
    ~ProjectConfigManager();

public:

    void setProjectName(const QString& strName);
    QString getProjectName();

    // 返回值: 0,添加成功; 1,已经存在; 2,添加失败
    void addMaterialFiles(const QStringList& stringList);

    QStringList getMaterialFiles();

private:
    QString m_strProjectPath;
    QSettings* m_pSettings;
    bool m_isOpened;
};

#endif // PROJECTCONFIGMANAGER_H

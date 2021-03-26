#include "DialogNewProject.h"
#include "ui_DialogNewProject.h"
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QException>
#include <QTextCodec>
#include "SystemConfigManager.h"
#include "ProjectConfigManager.h"
#include "DB.h"
#include "DatabaseManager.h"

DialogNewProject::DialogNewProject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewProject)
{
    ui->setupUi(this);

    SystemConfigManager& mgr = SystemConfigManager::GetInstance();
    m_strWorkingPath = mgr.getWorkingPath();
    ui->lineEditProjectPath->setText(m_strWorkingPath);

}

DialogNewProject::~DialogNewProject()
{
    delete ui;
}

QString DialogNewProject::getProjectName()
{
    return ui->lineEditProjectName->text().trimmed();
}

QString DialogNewProject::getWorkingPath()
{
    QString strProjectPath = ui->lineEditProjectPath->text().trimmed();
    if (strProjectPath.endsWith("\\") || strProjectPath.endsWith("/"))
    {
        strProjectPath = strProjectPath.remove(strProjectPath.length()-1, 1);
    }
    return strProjectPath;
}

void DialogNewProject::on_pushButtonBrowse_clicked()
{
    SystemConfigManager& mgr = SystemConfigManager::GetInstance();
    m_strWorkingPath = mgr.getWorkingPath();

    QTextCodec *tc = QTextCodec::codecForName("GBK");

    QString strPath = QFileDialog::getExistingDirectory(this, tc->toUnicode("选择工程目录"), m_strWorkingPath);
    ui->lineEditProjectPath->setText(strPath);

    if (!strPath.isEmpty())
    {
        QDir dir(strPath);
        if (dir.exists())
        {
            m_strWorkingPath = dir.absolutePath();
            mgr.setWorkingPath(m_strWorkingPath);
        }
    }
}

void DialogNewProject::on_lineEditProjectName_textChanged(const QString &/*arg1*/)
{
    testUserInput();
}

void DialogNewProject::on_lineEditProjectPath_textChanged(const QString &/*arg1*/)
{
    testUserInput();
}

bool DialogNewProject::testUserInput()
{
    QString strProjectPath(ui->lineEditProjectPath->text());
    if (!strProjectPath.isEmpty())
    {
        QDir dir(strProjectPath);
        if (!dir.exists())
        {
            ui->labelMsg->setHidden(false);
            ui->labelMsg->setText(QString(tr("The path '%1' dose not exists").arg(strProjectPath)));
            ui->labelMsg->setStyleSheet("QLabel{background:rgb(255, 0, 0);}");
        }
        else
        {
            ui->labelMsg->setHidden(true);
            if (!ui->lineEditProjectName->text().isEmpty())
            {
                QString strProjectDir = QString("%1/%2").arg(ui->lineEditProjectPath->text()).arg(ui->lineEditProjectName->text());
                QDir dir(strProjectDir);
                if (dir.exists())
                {
                    ui->labelMsg->setHidden(false);
                    ui->labelMsg->setText(QString(tr("The project '%1' has exists").arg(strProjectDir)));
                    ui->labelMsg->setStyleSheet("QLabel{background:rgb(255, 255, 0);}");
                }
                else
                {
                    ui->labelMsg->setHidden(true);
                    return true;
                }
            }
        }
    }

    return false;
}

void DialogNewProject::createProject()
{
    try
    {
        QDir dir;

        // 创建料单目录
        QString strMaterialDir = QString("%1/%2").arg(m_strProjectPath).arg(tr("material"));
        dir.mkdir(strMaterialDir);

        // 创建DXF文件目录
        QString strDxfDir = QString("%1/%2").arg(m_strProjectPath).arg(tr("dxf"));
        dir.mkdir(strDxfDir);

        // 创建异形文件目录
        QString strShapeDir = QString("%1/%2").arg(m_strProjectPath).arg(tr("shape"));
        dir.mkdir(strShapeDir);

        /*
        // 创建NC目录
        QString strNCDir = QString("%1/%2").arg(m_strProjectPath).arg(tr("nc"));
        dir.mkdir(strNCDir);

        // 创建标签存放目录
        QString strLabelDir = QString("%1/%2").arg(m_strProjectPath).arg(tr("label"));
        dir.mkdir(strLabelDir);

        // 创建图纸存放目录
        QString strDrawingDir = QString("%1/%2").arg(m_strProjectPath).arg(tr("drawing"));
        dir.mkdir(strDrawingDir);
        */

        // 创建工程文件
        QString strProjectFile = QString("%1/%2.pro").arg(m_strProjectPath).arg(ui->lineEditProjectName->text());
        QFile projectFile(strProjectFile);
        projectFile.open(QFile::ReadWrite);
        projectFile.close();

        // 创建数据库文件
        QString strDatabaseFile = QString("%1/%2.db").arg(m_strProjectPath).arg(ui->lineEditProjectName->text());

        // 重要，此后所有数据库操作都依赖GetDatabasePathName()来获取数据库文件
        CDatabaseManager& dbMgr = CDatabaseManager::GetInstance();
        dbMgr.SetDriverName("QSQLITE");
        dbMgr.SetConnectionName("EZCam");
        dbMgr.SetDatabasePathName(strDatabaseFile);

        if (!DB::createDatabase())
        {
            qCritical("Create database file failed.\n");
        }
    }
    catch (QException& e)
    {
        qCritical("create project failed: %s.\n", e.what());
    }
}

void DialogNewProject::on_pushButtonOK_clicked()
{
    if (testUserInput())
    {       
        QTextCodec *tc = QTextCodec::codecForName("GBK");

        QString strWorkingPath = getWorkingPath();

        QString strProjectDir = QString("%1/%2").arg(strWorkingPath).arg(ui->lineEditProjectName->text());
        QDir dir(strProjectDir);
        if (dir.mkdir(strProjectDir))
        {
            m_strProjectPath = strProjectDir;

            createProject();

            SystemConfigManager& sMgr = SystemConfigManager::GetInstance();
            ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();

            // 重要，新建工程或打开一个已存在的工程时必须通过openConfigFile()打开配置文件，
            // 此后对CConfigManager的所有操作才有效
            int nRet = mgr.openConfigFile(QString("%1/%2.pro").arg(strProjectDir).arg(ui->lineEditProjectName->text()));
            if (nRet == 0)
            {
                sMgr.setWorkingPath(this->getWorkingPath());
                mgr.setProjectName(this->getProjectName());
            }
            else if (nRet == 2)
            {
                QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("创建工程失败"), tc->toUnicode("知道了"));
            }
            else if (nRet == 1)
            {
                QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("工程配置文件不可写"), tc->toUnicode("知道了"));
            }
        }
        else
        {
            QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("创建工程失败"), tc->toUnicode("知道了"));
            return;
        }

        QDialog::accept();
    }
}

void DialogNewProject::on_pushButtonCancel_clicked()
{
    QDialog::reject();
}

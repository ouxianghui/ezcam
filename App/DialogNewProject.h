#ifndef DIALOGNEWPROJECT_H
#define DIALOGNEWPROJECT_H

#include <QDialog>

namespace Ui {
class DialogNewProject;
}

class DialogNewProject : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewProject(QWidget *parent = 0);
    ~DialogNewProject();

    QString getProjectName();
    QString getWorkingPath();

private slots:
    void on_pushButtonBrowse_clicked();
    void on_pushButtonOK_clicked();
    void on_pushButtonCancel_clicked();

    void on_lineEditProjectName_textChanged(const QString &arg1);

    void on_lineEditProjectPath_textChanged(const QString &arg1);

private:
    bool testUserInput();
    void createProject();

private:
    Ui::DialogNewProject *ui;
    QString m_strWorkingPath;
    QString m_strProjectPath;
};

#endif // DIALOGNEWPROJECT_H

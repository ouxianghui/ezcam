#ifndef DIALOGSYSTEMSETTINGS_H
#define DIALOGSYSTEMSETTINGS_H

#include <QDialog>
#include <vector>
#include <map>
#include <QTableWidgetItem>
#include "WorkspaceTreeWidget.h"
#include "DataSt.h"

namespace Ui {
class DialogSystemSettings;
}

class DialogSystemSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSystemSettings(WorkspaceTreeWidget* pWorkSpaceTreeWidget,
                                  std::map<QString, std::map<QString, std::vector<DataSt>>>& materialMap,
                                  QWidget *parent = 0);
    ~DialogSystemSettings();

private:
    void removeWorkspaceTree();
    void splitMaterialList(const QString& strKey, const std::vector<DataSt>& vecData);
    bool materialBoardSizeTest(int nMaxWidth, int nMaxHeight, int nSpace, const std::vector<DataSt>& vecData);

private slots:
    void on_checkBoxSplitByType_clicked();
    void on_checkBoxSplitByThickness_clicked();
    void on_pushButtonOK_clicked();
    void on_pushButtonCancel_clicked();
    void on_pushButtonBoardsSizeTest_clicked();

    void on_checkBoxUseDrills_clicked();

private:
    Ui::DialogSystemSettings *ui;
    WorkspaceTreeWidget* m_pWorkSpaceTreeWidget;
    std::map<QString, std::map<QString, std::vector<DataSt>>>& m_materialMap;
};

#endif // DIALOGSYSTEMSETTINGS_H

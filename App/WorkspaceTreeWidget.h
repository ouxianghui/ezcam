#ifndef WORKSPACETREEWIDGET_H
#define WORKSPACETREEWIDGET_H

#include <QObject>
#include <QTreeWidget>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>

class WorkspaceTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit WorkspaceTreeWidget(QWidget *parent = 0);
    ~WorkspaceTreeWidget();

signals:
    void deleteMaterialList(QString strMaterialListName);
    void deleteCuttingSolution(QString strSolutionID);

public slots:
    void onActionDeleteMaterialList(bool);
    void onActionDeleteMaterialGroup(bool);
    void onActionDeleteCuttingSolution(bool);

protected:
    void createWidgets();
    void contextMenuEvent(QContextMenuEvent* event);

private:
    QMenu* m_pMenu;
    QAction* m_pActionDeleteMaterialList;
    QAction* m_pActionDeleteMaterialGroup;
    QAction* m_pActionDeleteCuttingSolution;
};

#endif // WORKSPACETREEWIDGET_H

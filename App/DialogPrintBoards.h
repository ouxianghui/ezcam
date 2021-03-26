#ifndef DIALOGPRINTBOARDS_H
#define DIALOGPRINTBOARDS_H

#include <QDialog>
#include <QTreeWidget>
#include "Global.h"
#include "Typedef.h"
#include <map>
#include <vector>

namespace Ui {
class DialogPrintBoards;
}

class DialogPrintBoards : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPrintBoards(QWidget *parent = 0);
    ~DialogPrintBoards();

protected:
    void createWidgets();
    void readBoardsFromDatabase();
    bool readDataFromItem(QTreeWidgetItem* pItem, BoardSt& st);
    void readChildItem(QTreeWidgetItem* pItem, std::vector<BoardSt>& vecDataSt);
    void transformCheckedItem2VectorDataSt(std::vector<BoardSt>& vecDataSt);
    void sortVectorDataSt(std::vector<BoardSt>& vecDataSt);

protected slots:
    void onTreeWidgetItemChanged(QTreeWidgetItem* pItem, int column);
    void onPrintButtonClicked(bool checked = false);
    void onCloseButtonClicked(bool checked = false);

private:
    Ui::DialogPrintBoards *ui;
    QTreeWidget* m_pTreeWidget;
};

#endif // DIALOGPRINTBOARDS_H

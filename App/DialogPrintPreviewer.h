#ifndef DIALOGPRINTPREVIEWER_H
#define DIALOGPRINTPREVIEWER_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include "Typedef.h"
#include <vector>

namespace Ui {
class DialogPrintPreviewer;
}

class DialogPrintPreviewer : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPrintPreviewer(std::vector<BoardSt>& vecDataSt, QWidget *parent = 0);
    ~DialogPrintPreviewer();

protected:
    void createWidgets();
    void displayLabels2ListWidget();

protected slots:
    void onPrintButtonClicked(bool checked = false);
    void onCloseButtonClicked(bool checked = false);

private:
    Ui::DialogPrintPreviewer *ui;
    QListWidget* m_pListWidget;
    std::vector<BoardSt>& m_vecDataSt;
};

#endif // DIALOGPRINTPREVIEWER_H

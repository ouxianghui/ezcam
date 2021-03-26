#ifndef DIALOGPRINTSETTINGS_H
#define DIALOGPRINTSETTINGS_H

#include <QDialog>
#include "Typedef.h"

namespace Ui {
class DialogPrintSettings;
}

class DialogPrintSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPrintSettings(QWidget *parent = 0);
    ~DialogPrintSettings();

private slots:
    void on_pushButtonPrintTest_clicked();

    void on_pushButtonOk_clicked();

    void on_pushButtonCancel_clicked();

    void on_pushButtonSelectPrinter_clicked();

    void on_comboBoxTemplate_currentIndexChanged(int index);

    void on_comboBoxLTContext_currentIndexChanged(int index);

private:
    Ui::DialogPrintSettings *ui;
    BoardSt m_boardSt;
};

#endif // DIALOGPRINTSETTINGS_H

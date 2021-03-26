#ifndef BITMAPEXPORTDIALOG_H
#define BITMAPEXPORTDIALOG_H

#include <QDialog>
#include <QDialog>
#include <QColor>
#include <QComboBox>
#include <QRadioButton>
#include <map>
#include "RMathLineEdit.h"

namespace Ui {
class BitmapExportDialog;
}

class BitmapExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BitmapExportDialog(QWidget *parent = 0);
    ~BitmapExportDialog();

    RMathLineEdit* getWidthLineEdit();
    RMathLineEdit* getHeightLineEdit();
    QComboBox* getResolutionComboBox();
    QComboBox* getMarginComboBox();
    QRadioButton* getBlackBackgroundRadioButton();
    QRadioButton* getWriteBackgroundRadioButton();

protected slots:
    void onWidthLineEditValueChanged(double value, const QString& error);
    void onHeightLineEditValueChanged(double value, const QString& error);
    void onResolutionComboBoxEditTextChanged(const QString& text);

private:
    Ui::BitmapExportDialog *ui;
};

#endif // BITMAPEXPORTDIALOG_H

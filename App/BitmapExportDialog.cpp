#include "BitmapExportDialog.h"
#include "ui_BitmapExportDialog.h"
#include <QTextCodec>

BitmapExportDialog::BitmapExportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BitmapExportDialog)
{
    ui->setupUi(this);

    connect(ui->Width, &RMathLineEdit::valueChanged, this, &BitmapExportDialog::onWidthLineEditValueChanged);
    connect(ui->Height, &RMathLineEdit::valueChanged, this, &BitmapExportDialog::onHeightLineEditValueChanged);
    connect(ui->Resolution, &QComboBox::editTextChanged, this, &BitmapExportDialog::onResolutionComboBoxEditTextChanged);
}

BitmapExportDialog::~BitmapExportDialog()
{
    delete ui;
}

RMathLineEdit* BitmapExportDialog::getWidthLineEdit()
{
    return ui->Width;
}

RMathLineEdit* BitmapExportDialog::getHeightLineEdit()
{
    return ui->Height;
}

QComboBox* BitmapExportDialog::getResolutionComboBox()
{
    return ui->Resolution;
}

QComboBox* BitmapExportDialog::getMarginComboBox()
{
    return ui->Margin;
}

QRadioButton* BitmapExportDialog::getBlackBackgroundRadioButton()
{
    return ui->BlackBackground;
}

QRadioButton* BitmapExportDialog::getWriteBackgroundRadioButton()
{
    return ui->WhiteBackground;
}

void BitmapExportDialog::onWidthLineEditValueChanged(double /*value*/, const QString& /*error*/)
{
    ui->Resolution->setCurrentIndex(0);
}

void BitmapExportDialog::onHeightLineEditValueChanged(double /*value*/, const QString& /*error*/)
{
    ui->Resolution->setCurrentIndex(0);
}

void BitmapExportDialog::onResolutionComboBoxEditTextChanged(const QString& /*text*/)
{
}


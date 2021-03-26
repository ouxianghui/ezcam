#include "DialogAbout.h"
#include "ui_DialogAbout.h"

DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(), this->height());
}

DialogAbout::~DialogAbout()
{
    delete ui;
}

void DialogAbout::on_pushButton_OK_clicked()
{
    QDialog::accept();
}

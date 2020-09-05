#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :QDialog(parent),ui(new Ui::AboutDialog){
    ui->setupUi(this);
    setFixedSize(235,114);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_buttonBox_accepted()
{
    this->accept();
}

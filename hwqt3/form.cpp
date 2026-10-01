#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Form)
{
    ui->setupUi(this);
}

Form::~Form()
{
    delete ui;
}


void Form::on_PB_Exit_clicked()
{
    this->close();
}


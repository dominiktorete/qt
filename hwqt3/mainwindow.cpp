#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fr = new Form(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_PB_Connect_clicked()
{
    fr->show();

}


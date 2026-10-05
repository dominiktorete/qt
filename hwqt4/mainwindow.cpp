#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->RB_Radio_On->setText("Radio On");
    ui->RB_Radio_Off->setText("Radio Off");
    ui->CB_List->addItem("Получить данные");
    ui->CB_List->addItem("Получить прогноз");
    ui->CB_List->addItem("Отправить сообщение");
    ui->CB_List->addItem("Заказать еду");
    ui->CB_List->addItem("Исследовать локацию");
    ui->PB_Click->setText("Click me!");
    ui->PB_Click->setCheckable(true);
    ui->PB_Count_click->setValue(0);
    ui->PB_Count_click->setMaximum(100);
    ui->PB_Count_click->setMinimum(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_PB_Click_pressed()
{
    if(ui->PB_Count_click->value() == 100) ui->PB_Count_click->setValue(0);
    else
        ui->PB_Count_click->setValue(ui->PB_Count_click->value() + 10);
}


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QTime"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sw = new Stopwatch(this);
    timer = new QTimer(this);
    time = new QTime(0,0,0,0);
    time_Round = new QTime(0,0,0,0);
    time_for_compare = new QTime(0,0,0,0);
    current_round = 1;
    ui->tB_DataRounds->setTextColor("white");
    ui->L_Time->setText(time->toString("hh:mm:ss.z"));
    ui->L_TimeRound->setText(time_Round->toString("hh:mm:ss.z"));
    ui->pB_StartStop->setText("Старт");
    ui->pB_Round->setText("Круг");
    ui->pB_Clear->setText("Очистить");
    ui->pB_Round->setEnabled(false);
    timer->setInterval(100);
    connect(sw, &Stopwatch::sig_Start, timer, [receiver = timer]() { receiver->start(); });
    connect(sw, &Stopwatch::sig_Stop, timer, [receiver = timer]() { receiver->stop(); });
    connect(timer, &QTimer::timeout, this, &MainWindow::UpdateTimer);
    connect(sw, &Stopwatch::sig_Clear, this, &MainWindow::Clear_All);
    connect(sw, &Stopwatch::sig_Round, this, &MainWindow::Round_Update);

}
void MainWindow::UpdateTimer(){
    *time = time->addMSecs(timer->interval());
    *time_Round = time_Round->addMSecs(timer->interval());
    ui->L_Time->setText(time->toString("hh:mm:ss.z"));
    ui->L_TimeRound->setText(time_Round->toString("hh:mm:ss.z"));
}
void MainWindow::Clear_All(){
    time->setHMS(0,0,0,0);
    time_Round->setHMS(0,0,0,0);
    time_for_compare->setHMS(0,0,0,0);
    ui->L_Time->setText(time->toString("hh:mm:ss.z"));
    ui->L_TimeRound->setText(time_Round->toString("hh:mm:ss.z"));
    ui->tB_DataRounds->clear();
    current_round = 1;
}
void MainWindow::on_pB_Clear_clicked()
{
    emit sw->sig_Clear();
}
void MainWindow::on_pB_StartStop_clicked()
{
    if(ui->pB_StartStop->text() == "Старт"){
        emit sw->sig_Start();
        ui->pB_Round->setEnabled(true);
        ui->pB_StartStop->setText("Стоп");

    }
    else if(ui->pB_StartStop->text() == "Стоп"){
        emit sw->sig_Stop();
        ui->pB_Round->setEnabled(false);
        ui->pB_StartStop->setText("Старт");
    }
}
void MainWindow::Round_Update(){
    QTime temp{0,0,0,0};
    QString different{};
    if(*time_for_compare == temp){
        different = "--:--:--.-";
    }
    else {
        if(*time_for_compare < *time_Round){
            int diff_msec = time_for_compare->msecsTo(*time_Round);
            int hour = (diff_msec / 1000 / 60 / 60) % 24;
            int minute = (diff_msec / 1000 / 60) % 60;
            int second = (diff_msec / 1000) % 60;
            int msec = diff_msec % 1000 / 100;
            different = QString("+%1:%2:%3.%4").arg(hour).arg(minute).arg(second).arg(msec);
        }
        else if(*time_for_compare > *time_Round){
            int diff_msec = time_Round->msecsTo(*time_for_compare);
            int hour = (diff_msec / 1000 / 60 / 60) % 24;
            int minute = (diff_msec / 1000 / 60) % 60;
            int second = (diff_msec / 1000) % 60;
            int msec = diff_msec % 1000 / 100;
            different = QString("-%1:%2:%3.%4").arg(hour).arg(minute).arg(second).arg(msec);

        }
        else {
            different = "0:0:0.0";
        }
    }

    ui->tB_DataRounds->append(QString("Круг: %1 \tВремя: ").arg(current_round) + time_Round->toString("hh:mm:ss.z") + "\t" + different);
    *time_for_compare = *time_Round;
    time_Round->setHMS(0,0,0,0);
    ui->L_TimeRound->setText(time_Round->toString("hh:mm:ss.z"));
    ++current_round;

}

void MainWindow::on_pB_Round_clicked()
{
    sw->sig_Round();
}

MainWindow::~MainWindow()
{
    delete ui;
}








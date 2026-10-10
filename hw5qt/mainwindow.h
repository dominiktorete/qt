#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "stopwatch.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

public slots:



private slots:
    void on_pB_StartStop_clicked();
    void UpdateTimer();
    void Clear_All();
    void Round_Update();
    void on_pB_Clear_clicked();

    void on_pB_Round_clicked();

private:
    Stopwatch* sw;
    Ui::MainWindow *ui;
    QTimer* timer;
    QTime* time;
    QTime* time_Round;
    QTime* time_for_compare{};
    int current_round{};
};
#endif // MAINWINDOW_H

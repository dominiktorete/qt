#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "form.h"
#include <QMainWindow>


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

private slots:
    void on_PB_Connect_clicked();

private:

    Ui::MainWindow *ui;
    Form* fr;
};
#endif // MAINWINDOW_H

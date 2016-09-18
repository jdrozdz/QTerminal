#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_released();
    void on_read_serial();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QSerialPortInfo *ports;
    bool is_connected;
};

#endif // MAINWINDOW_H

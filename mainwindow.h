#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QShortcut>
#include <QKeySequence>

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
    QShortcut *sendCommand;
    bool send_data;

private slots:
    void send_command();
    void on_sendButton_clicked();
};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    is_connected = false;

    ui->setupUi(this);
    serial = new QSerialPort(this);

    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts()) {
        ui->_serialPort->addItem(port.portName(), port.serialNumber());
    }

    ui->_baudRate->addItem("1200", QSerialPort::Baud1200);
    ui->_baudRate->addItem("2400", QSerialPort::Baud2400);
    ui->_baudRate->addItem("4800", QSerialPort::Baud4800);
    ui->_baudRate->addItem("9600", QSerialPort::Baud9600);
    ui->_baudRate->addItem("19200", QSerialPort::Baud19200);
    ui->_baudRate->addItem("57600", QSerialPort::Baud57600);
    ui->_baudRate->addItem("115200", QSerialPort::Baud115200);

    ui->_baudRate->setCurrentText("9600");

    (void) new QShortcut(QKeySequence(Qt::Key_Return), this, SLOT(send_command()));

    // Assign "listener" on serial port
    connect(serial, SIGNAL(readyRead()), this, SLOT(on_read_serial()));

    // Assign shortcut key to send command
    // connect(sendCommand, SIGNAL(activated()), ui->sendButton, SLOT(send_command()));
    send_data = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::send_command(){
    if(ui->enterEnable->isChecked() || send_data){
        QByteArray cmd = ui->command->text().toLocal8Bit();
        qint64 resp = serial->write(cmd);
        if(resp == -1){
            QMessageBox::critical(this,"Error", QString("Błąd wysłania danych na porcie %1!\n%2").arg(serial->portName()).arg(serial->errorString()));
        }else if(resp != cmd.size()){
            QMessageBox::critical(this, "Error", QString("Nie można wysłać wszystkich danych!\n%1").arg(serial->errorString()));
        }else{
            QString user_data = QString("<div style='background: silver; color: green;margin-bottom: 3px;'>%1</div>").arg(ui->command->text());
            ui->terminal->append(user_data);
            ui->command->setText("");
        }
        send_data = false;
    }
}

void MainWindow::on_pushButton_released()
{
    if(!is_connected){
        serial->setPortName(ui->_serialPort->currentText());
        serial->setBaudRate(ui->_baudRate->currentData().toInt());
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setFlowControl(QSerialPort::NoFlowControl);
        serial->open(QIODevice::ReadWrite);
        ui->pushButton->setText("Rozłącz");
        is_connected = true;
        ui->_serialPort->setEnabled(false);
        ui->_baudRate->setEnabled(false);
    }else {
        serial->close();
        ui->_serialPort->setEnabled(true);
        ui->_baudRate->setEnabled(true);
        ui->pushButton->setText("Połącz");
        is_connected = false;
    }
}

void MainWindow::on_read_serial(){
    QString data = (QString)serial->readAll();
    data.remove(QRegExp("[\n\r]"));
    ui->terminal->append(data);
}

void MainWindow::on_sendButton_clicked()
{
    send_data = true;
    send_command();
}

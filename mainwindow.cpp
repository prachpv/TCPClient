#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket,&QTcpSocket::readyRead,this,&MainWindow::slotReadyRead);
    //connect(socket,&QTcpSocket::disconnected,this,&QTcpSocket::deleteLater);
    connect(socket,&QTcpSocket::disconnected,this,&MainWindow::slotDisonected);
    nextBlockSize=0;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connect_clicked()
{
    if(ui->lineEdit_host->text().isEmpty())
    {
        socket->connectToHost("127.0.0.1",2323);
    }else{
        socket->connectToHost(ui->lineEdit_host->text(),2323);
    }

}

void MainWindow::SendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);
    out<<quint16(0)<<QTime::currentTime()<<str;
    out.device()->seek(0);
    out<<quint16(Data.size()-sizeof(quint16));
    socket->write(Data);
    ui->message->clear();
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_10);
    if(in.status()==QDataStream::Ok)
    {
//        QString str;
//        in>>str;
//        ui->textBrowser->append(str);
        for( ; ; )
        {
            if(nextBlockSize==0)
            {
                if(socket->bytesAvailable()<2)
                {
                    break;
                }
                in>>nextBlockSize;
            }
            if(socket->bytesAvailable()<nextBlockSize)
            {
                break;
            }
            QString str;
            QTime time;
            in>>time>>str;
            nextBlockSize=0;
            ui->textBrowser->append(time.toString()+" "+str);
        }
    }else{
        ui->textBrowser->append("ready error");
    }
}

void MainWindow::slotDisonected()
{
    socket->deleteLater();
}

void MainWindow::on_send_clicked()
{
    SendToServer(ui->message->text());
}

void MainWindow::on_message_returnPressed()
{
    SendToServer(ui->message->text());
}

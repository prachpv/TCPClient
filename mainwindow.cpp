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
    bool ok;
    QString text= QInputDialog::getText(this,QString::fromUtf8("Введите имя пользователя"),QString::fromUtf8("Ваш имя:"),QLineEdit::Normal,QDir::home().dirName(),&ok);
    username=text;

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

void MainWindow::SendToServer(QString name,QString str,int _mode)
{
    Data.clear();

        QDataStream out(&Data,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_10);
        out<<quint16(0)<<name<<mode<<QTime::currentTime()<<str;
        out.device()->seek(0);
        out<<quint16(Data.size()-sizeof(quint16));
        socket->write(Data);
    SendFile();
        ui->message->clear();
}


void MainWindow::SendFile()
{
    QString filePathName = file_path_temp;
        QStringList filePath = filePathName.split("/");

        QFile file(file_path_temp);
        QByteArray q;
        int i = 0;
        QByteArray block;
        QByteArray arrBlock;
        QByteArray junk("!@#Rtasd#$%sdfs!!!)()334rrer");
        QList<QByteArray> Blocks;
//_mode=1;
        if (file.open(QIODevice::ReadOnly)) {
            QFileInfo fileInfo(file);
            QString fileName(filePath.last());

            QDataStream out(&arrBlock, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_7);
            //

//            out << quint16(0) << QTime::currentTime()
//                <<username<<fileName << fileInfo.size();

//            out.device()->seek(0);
//            out << quint16(arrBlock.size() - sizeof(quint16));
            //


            while (!file.atEnd())
            {
                q = file.readAll();
                file.close();
            }
    //        Blocks = q.split('\n');
            int pos = 0, arrsize = q.size(), sizeInArray = 512;
            while(pos<arrsize){
                QByteArray arr = q.mid(pos, sizeInArray);
                Blocks << arr;
                pos+=arr.size();
            }

            foreach (block, Blocks) {

                socket->write(block);
                socket->waitForBytesWritten();
                if (block == Blocks.last()) {
                    qDebug() << "sended!";
                    socket->write(junk);
                    socket->waitForBytesWritten();
                    block.clear();
                }
                q.clear();
                i++;
                //ui->prBr_fileSending->setValue(i/Blocks.size()*100);
            }
        }
}


void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_10);
    if(in.status()==QDataStream::Ok)
    {

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
            //
            //
            QString str;
            QTime time;
            QString name;
            int mode_sender;
            in>>name>>mode_sender>>time>>str;
            //str+="///"+QString::number(mode_sender);
            nextBlockSize=0;

                ui->textBrowser->append(time.toString()+"["+name+"]"+" "+str);

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
    SendToServer(username,ui->message->text(),mode);
}

void MainWindow::on_message_returnPressed()
{
    SendToServer(username,ui->message->text(),mode);
}

void MainWindow::on_pushButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, QString::fromUtf8("Имя файла"));

    if(!file_name.isEmpty())
    {
        mode =1;
        file_path_temp=file_name;
        //ui->message->setEnabled(false);

    }
    //ui->message->setText(file_name);
}



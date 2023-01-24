#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTime>
#include <QtGui>
#include <QInputDialog>
#include <QFileDialog>
#include <QFileInfo>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_connect_clicked();

    void on_send_clicked();

    void on_message_returnPressed();

    void on_pushButton_clicked();



private:
    QByteArray Data;
    QTcpSocket* socket;
    void SendToServer(QString name,QString str,int mode);
    quint16 nextBlockSize;
    QString username;
    QString file_path_temp;
    Ui::MainWindow *ui;
    QFile* file_;
    quint8 filesend;
    QString Me;
    int mode=0;
    void SendFile();
public slots:
    void slotReadyRead();
    void slotDisonected();

};
#endif // MAINWINDOW_H

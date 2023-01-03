#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTime>
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

private:
    QByteArray Data;
    QTcpSocket* socket;
    void SendToServer(QString str);
    quint16 nextBlockSize;
private:
    Ui::MainWindow *ui;
public slots:
    void slotReadyRead();
    void slotDisonected();
};
#endif // MAINWINDOW_H

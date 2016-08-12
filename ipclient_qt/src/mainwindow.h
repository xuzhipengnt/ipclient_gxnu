#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <cafesclient.h>
#include <QMainWindow>
#include <QLabel>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
#include <QCryptographicHash>
#include <QTimer>
#include <QTime>
#include <QHostInfo>
#include <QHostAddress>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QUdpSocket *usocket;
    void udp_init();
    void login(char *username);
    QString name;
    QString password;
    QUdpSocket *livepacket;
      QUdpSocket *ispUdp;
    QHostAddress server;
    QTimer *timer;
    QTimer *timeoutf;
    unsigned int livekey;
    int diss;
 QString myIP;
  QString myMac;
   // QLabel *information;
    ~MainWindow();
QByteArray livedata;

private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();
    void keyreceive();
    void live();
    void killtime();
    void showinfo();
    void buildLive();
    void displaytimeout();
    void on_pushButton_2_clicked();
   int ispCon(int ispNum);
    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H

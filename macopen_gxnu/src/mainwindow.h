#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
#include <QCryptographicHash>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include <QSettings>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    int ispn; //Define isp number
    int loopmode;//Deinfe circlemode  --1 loop mode --0  single mode
    QString localip;//Local ip
    QString loopmac;
    int looptime;
    QUdpSocket *ipsocket;  //ipsocket to get ipaddress
    QUdpSocket *ispudp;  //ispudp to send pre dial packet
    void Getlocalip();  //Get local ip from 202.193.160.123 because NAT will change source ip
    QHostAddress server; //Define server
    QTimer *timeout1;  //Define receive ip address timeout
    QTimer *timeout2;  //Define receive feedback timeout
    QTimer *flagtime;  //Define flag timeout
     QTimer *loopclock;  //Define loop clock
     void WriteSettings(QString mac);
    void ReadSettings();
//    QMessageBox *warning;
     int ispCon(int ispNum,QString myIP,QString myMac);
    ~MainWindow();

private slots:
    void on_unicom_clicked();
    void on_tele_clicked();
    void Recvip();  //Receive ip address from server
    void Recvfeedback();  //Receive feedback from server
    void on_mobi_clicked();
    void displaytimeout();
    void feedbacktimeout();
    void loopsend();
    void on_circ_clicked();
    void showflag();
    void on_dial_clicked();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

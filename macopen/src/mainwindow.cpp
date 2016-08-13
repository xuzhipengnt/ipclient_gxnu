#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ispn=1; //Init isp number  -1 Unicom -2 Telecom -3 Mobile
    ui->circtime->setDisabled(1);  // Loop mode time disable
    ui->circtime->setMinimum(2);   // Set loop mode min time 2 miniutes
    ui->dial->setDisabled(1); //Disable dial button if can not get correct local ip address
    ipsocket=new QUdpSocket(this);  //Define class QUdpSocket to get local ip address
    ispudp=new QUdpSocket(this);  //Define class QUdpSocket to get local ip address
    timeout1=new QTimer(this);
    timeout2=new QTimer(this);
    loopclock=new QTimer(this);
    loopmode=0;  //single mode
    server=QHostAddress("202.193.160.123"); //Define dial server
    localip="0.0.0.0";
   connect(timeout1,SIGNAL(timeout()),this,SLOT(displaytimeout())); //Receive ip address timeout
   connect(timeout2,SIGNAL(timeout()),this,SLOT(feedbacktimeout()));//Receive feedback packet timeout
    Getlocalip();//Get local ip address
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Getlocalip()
{
     ipsocket->connectToHost(server,5301);
     unsigned char data[]={0x82, 0x23, 0x05, 0x0c, 0x08, 0x09, /* Oo.#.... */
             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
             0x00, 0x38, 0xde, 0x2b, 0x41, 0x00, 0x00, 0x00, /* .8.+A... */
             0x00, 0x68, 0x16, 0x02, 0x41, 0x01, 0x00, 0x00, /* .h..A... */
             0x00, 0x61, 0x01, 0x00, 0x00, 0x00, 0x62, 0x29, /* .a....b) */
             0x00, 0x00, 0x00, 0x75, 0x48, 0x1b, 0x5b, 0x5f, /* ...uH.[_ */
             0x0d, 0x2c, 0x5a, 0x32, 0x2d, 0x34, 0x54, 0x1f, /* .,Z2-4T. */
             0x70, 0x6a, 0x25, 0x71, 0x3c, 0x62, 0x10, 0x6e, /* pj%q<b.n */
             0x3e, 0x1c, 0x51, 0x1f, 0x16, 0x13, 0x29, 0x66, /* >.Q...)f */
             0x28, 0x36, 0x67, 0x23, 0x3d, 0x10, 0x36, 0x13, /* (6g#=.6. */
             0x52, 0x52, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00, /* RRE..... */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
             0xff, 0xff, 0xff, 0xff, 0xff, 0xff              /* ...... */
             };
     QByteArray qdata; //Transform unsigned char to QByteArray
     for (int i=0;i<500;i++) qdata.append(data[i]);
     connect(ipsocket,SIGNAL(readyRead()),this,SLOT(Recvip()));
     ipsocket->write(qdata); //Senddata
     timeout1->start(2000);
}

void MainWindow::Recvip()
{
    char data[500]={0};
    ipsocket->read(data,500);
    for (int i=0;i<500;i++)
    {
        if (((unsigned char)data[i]==0x82) &&((unsigned char)data[i+1]==0x23) && ((unsigned char)data[i+2]==0x06))
        {
            localip="";
            timeout1->stop(); //Stop timer
            ui->dial->setEnabled(1);//Enable dial button
            ui->label_4->setText("Welcome");
            for (int i2=i+62;i2<500;i2++)
            {
                if (data[i2]==0x01) break;
                localip.append(data[i2]);
            }
            break;
        }
    }
     ui->lip->setText(localip);
      ipsocket->close();
}

void MainWindow::on_unicom_clicked()
{
    if (ui->unicom->isChecked()==1)
    {
        ui->tele->setChecked(0);
        ui->mobi->setChecked(0);
        ispn=1; // Set ispn
    }
}

void MainWindow::on_tele_clicked()
{
    if (ui->tele->isChecked()==1)
    {
        ui->unicom->setChecked(0);
        ui->mobi->setChecked(0);
        ispn=2;  // Set ispn
    }
}

void MainWindow::on_mobi_clicked()
{
    if (ui->mobi->isChecked()==1)
    {
        ui->tele->setChecked(0);
        ui->unicom->setChecked(0);
        ispn=3;  // Set ispn
    }
}






void MainWindow::on_circ_clicked()
{
    if (ui->circ->isChecked()==1)
    {
        ui->circtime->setEnabled(1);
        loopmode=1;
    }
    else
    {
        ui->circtime->setDisabled(1);
        loopmode=0;
    }
}



void MainWindow::on_dial_clicked()
{
    /******Before pre dial,must disconnetc all slots****/
     disconnect(ipsocket,SIGNAL(readyRead()),this,SLOT(Recvip()));
      disconnect(ispudp,SIGNAL(readyRead()),this,SLOT(Recvfeedback()));
        loopclock->stop();
  disconnect(loopclock,SIGNAL(timeout()),this,SLOT(loopsend()));
  QRegExp regexpmac("([0-9A-F]{2}:[0-9A-F]{2}:[0-9A-F]{2}:[0-9A-F]{2}:[0-9A-F]{2}:[0-9A-F]{2})");
  QString macaddr;
  int looptime=0;
  macaddr=ui->mactext->text(); //Get mac address
  macaddr=macaddr.toUpper(); //Transform to UPPER
  bool macmatch=regexpmac.exactMatch(macaddr);
  if (macmatch) //Check MAC address
  {
        if (loopmode==0)
        { //single mode
          ispCon(ispn,localip,macaddr);
        }
        else if (loopmode==1)
        { //loop mode
           ispCon(ispn,localip,macaddr);
          loopmac=macaddr;
          looptime=ui->circtime->value();
        //  disconnect(loopclock,SIGNAL(timeout()),this,SLOT(loopsend()));
          connect(loopclock,SIGNAL(timeout()),this,SLOT(loopsend()));
          loopclock->start(looptime*1000*60);
        }
   }
   else //if mac address is incorrect
   {
       ui->label_4->setText("Invalid MAC address");
   }
}


int MainWindow::ispCon(int ispNum,QString myIP,QString myMac)
{
     ispudp->connectToHost(server,20015);
     qDebug()<<myIP;
    qDebug()<<myMac;
    int ispKey=0x4e67c6a7;
        int ECX;
        int ESI;
        int EBX;
        int EAX;
        unsigned char localInfo[]={0x00,0x00,0x00,0x00,0x00,0x00,
                                  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                  0xac,0x10,0x40,0x12,0x30,0x30,0x3a,0x31,
                                  0x46,0x3a,0x31,0x36,0x3a,0x32,0x32,0x3a,
                                  0x42,0x38,0x3a,0x45,0x43,0x00,0x00,0x00,
                                  0x02,0x00};
        int nInfo=sizeof(localInfo);
        int nMac=myMac.size();
        localInfo[nInfo-2]=(unsigned char)ispNum;
        qDebug()<<myIP;
        for (int i=0;i<nMac;i++)
        {
            localInfo[i+34]=(unsigned char)myMac[i].toLatin1();
        }
        localInfo[0]='1';
        QStringList ipList = myIP.split(".");
        if (ipList.size()!=4)
        {
            return -1;
        }
        int ip[4]={0};
        for (int i=0;i<4;i++)
        {
            ip[i]=ipList.at(i).toInt();
           qDebug()<<ip[i];
        }
        for (int i=0;i<4;i++)
        {
            localInfo[i+30]=(unsigned char)ip[i];
        }
        /****************Calculating Key************/
        ECX=ispKey;
        for (int i=0;i<nInfo;i++)
        {
             ESI=ECX;
             ESI=ESI<<5;
            if (ECX>0)
             {
                 EBX=ECX;
                 EBX=EBX>>2;
             }
             else
             {
                 EBX=ECX;
                 EBX=EBX>>2;
                 EBX=EBX|(0xC0000000);
             }
             ESI=ESI+localInfo[i];
             EBX=EBX+ESI;
             ECX=ECX^EBX;
        }
        ECX=ECX&0x7FFFFFFF;
        QByteArray ispData;
        for (int i=0;i<nInfo;i++) ispData.append(localInfo[i]);
        for (int i=0;i<4;i++)
        {
            unsigned char keypart;
            keypart=(unsigned char)(ECX>>(i*8))&0x000000FF;
            ispData.append(keypart);
        }
       connect(ispudp,SIGNAL(readyRead()),this,SLOT(Recvfeedback()));
         timeout2->start(2000);
       ispudp->write(ispData);
       return 1;
}
void MainWindow::displaytimeout()
{
    ui->label_4->setText("Can't connect to server");
    ipsocket->close();
    Getlocalip();
}
void MainWindow::feedbacktimeout()
{
    ui->label_4->setText("Can't receive feedback packet");
    ispudp->close();
   // Getlocalip();
}
void MainWindow::Recvfeedback()
{

    if (ispudp->hasPendingDatagrams())
    {
          qDebug()<<"Open succeed!";
          ui->label_4->setText("Open succeed!");
          timeout2->stop();
          ispudp->close();
     }
}
void MainWindow::loopsend()
{
      ispCon(ispn,localip,loopmac);
      loopclock->start();
}

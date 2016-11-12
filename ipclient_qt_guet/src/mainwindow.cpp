#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QByteArray preDATA;
    preDATA.append("THIS IS A TEST MESSAGE TO GET LOCAL IP ADDRESS, PLEASE DO NOT RESPOND!");
    usocket=new QUdpSocket(this);
    ispUdp=new QUdpSocket(this);
    server=QHostAddress("172.16.1.1");
    usocket->connectToHost(server,5300);
    ispUdp->connectToHost(server,20015);
    usocket->write(preDATA);
    myIP=usocket->localAddress().toString();
    timer=new QTimer(this);
    timeoutf=new QTimer(this);
    livepacket=new QUdpSocket(this);
    livepacket->connectToHost(server,5301);
   ui->lineEdit_2->setEchoMode(QLineEdit::Password);
   diss=0;
}
void MainWindow::login(char *username)
{
    //char ipAddress[]="202.193.160.123";
    unsigned char tail[]={0x0b,0x00,0x00,0x00,0x21,
                 0x40,0x23,0x24,0x25,0x25,
                 0x5e,0x26,0x2a,0x28,0x29,
                 0x07,0x00,0x00,0x00,0x71,
                 0x77,0x65,0x72,0x74,0x79,
                 0x75,0x39,0x30,0x00,0x00,
                 0x01,0x00,0x00,0x00,0x06,
                 0x00,0x00,0x00,0x41,0x53,
                 0x44,0x46,0x47,0x48};

    unsigned char request[300]={0};
    /***Construct Packet Header***/
    request[0]=0x82;request[1]=0x23;request[2]=0x1f;
    for (int i=3;i<15;i++) request[i]=0x00;
    request[11]=name.size();  //get the length of string
    int charlen=name.size();
    for (int i=0;i<strlen(username);i++) request[15+i]=username[i]-10;
    for (int j=0;j<44;j++) request[15+charlen+j]=tail[j];
   QByteArray data;
   for (int i=0;i<300;i++) data.append(request[i]);

   connect(usocket,SIGNAL(readyRead()),this,SLOT(keyreceive()));
   connect(timeoutf,SIGNAL(timeout()),this,SLOT(displaytimeout()));
   usocket->write(data);
   timeoutf->start(2000);
  //keyreceive();
}



MainWindow::~MainWindow()
{

    name="12345678";
    char *user3="12345678";
    password="0000";
     diss=1;
    login(user3);
    livepacket->destroyed();
    usocket->destroyed();
    killtime();
    delete ui;
}

void MainWindow::on_pushButton_3_clicked()
{
    exit(0);
}
void MainWindow::keyreceive()
{ char key[300]={0};
  int calckey=0;
  unsigned short staus=0xff;
    usocket->read(key,300);
    int flag=0;
    for (int i=0;i<300-3;i++)
     {
        if (((unsigned char)key[i]==0x82) &&((unsigned char)key[i+1]==0x23) && ((unsigned char)key[i+2]==0x20))
         {
            //cout<<"key:"<<endl;
            //cout<<hex<<(unsigned short)(unsigned char)key[i+51]<<endl;
            //cout<<hex<<(unsigned short)(unsigned char)key[i+52]<<endl;
            calckey=(unsigned short)(unsigned char)key[i+51+(name.size()-10)]+((unsigned short)(unsigned char)key[i+52+(name.size()-10)]<<8);
          //  livekey=(unsigned short)(unsigned char)key[i+52+(name.size()-10)]+((unsigned short)(unsigned char)key[i+51+(name.size()-10)]<<8);
            calckey=calckey-3344;
            livekey=calckey+1500;
            livekey=((livekey>>8) & 0x00ff)+((livekey<<8) & 0xff00);
            cout<<hex<<livekey<<endl;
            flag=1;
            buildLive();
            timeoutf->stop();
            break;
          }
        if (((unsigned char)key[i]==0x82) &&((unsigned char)key[i+1]==0x23) && ((unsigned char)key[i+2]==0x22))
         {
            //cout<<"key:"<<endl;
            //cout<<hex<<(unsigned short)(unsigned char)key[i+51]<<endl;
            //cout<<hex<<(unsigned short)(unsigned char)key[i+52]<<endl;
            cout<<"receive result"<<endl;
            flag=2;
            staus=(unsigned short)(unsigned char)key[i+3];
            cout<<hex<<"0x"<<staus<<endl;
            if (staus==0x00)
                  {
                   ui->label_4->setText(tr("连接成功"));
                   disconnect(usocket,SIGNAL(readyRead()),this,SLOT(keyreceive()));
                   connect(timer,SIGNAL(timeout()),this,SLOT(live()));
                //   timer->start(1000);
                 timer->start(120000);
                   disconnect(timeoutf,SIGNAL(timeout()),this,SLOT(displaytimeout()));
                   timeoutf->stop();


                  }
            else if (staus==0x63)
            {
                cout<<diss<<endl;
                if (diss==1)
                   {
                    ui->label_4->setText(tr("断开成功"));
                    diss=0;
                    }
                else
                    ui->label_4->setText(tr("错误的用户名或密码"));

                timer->stop();
                disconnect(timeoutf,SIGNAL(timeout()),this,SLOT(displaytimeout()));
                timeoutf->stop();
            }
            else if (staus==0x20)
            {
                ui->label_4->setText(tr("账号已经被使用"));
                 timer->stop();
                 disconnect(timeoutf,SIGNAL(timeout()),this,SLOT(displaytimeout()));
                 timeoutf->stop();
            }
            else if (staus==0x14)
            {
                ui->label_4->setText(tr("余额不足"));
                 timer->stop();
                 disconnect(timeoutf,SIGNAL(timeout()),this,SLOT(displaytimeout()));
                 timeoutf->stop();
            }
            break;
          }
      }

    if (flag==1)  //Receive key packet and send password
    {
        QString calc=QString::number(calckey);
        calc.append(password);
        qDebug()<<calc;
        QByteArray md51;
        QString  md52;
        QByteArray  md53;
        QString  md54;
        md51=QCryptographicHash::hash(calc.toLatin1(), QCryptographicHash::Md5 );
        md52=md51.toHex();
        md52=md52.toUpper();
        md52.truncate(5);
        md52=md52.append(name);
      // qDebug()<<tr(md52.toLatin1());
        md53=QCryptographicHash::hash(md52.toLatin1(), QCryptographicHash::Md5 );
        md54=md53.toHex();
        md54=md54.toUpper();
        md54.truncate(30);
        char keysend[300]={0};
        char *mdhash;
        mdhash=md54.toLatin1().data();
        unsigned temple[]={0x82,0x23,0x21,0x00,0x00,0x00,
                       0x00,0x00,0x00,0x00,0x00,0x0e,0x00,0x00,
                       0x00,0x39,0x67,0x64,0x74,0x34,0x33,0x37,
                       0x34,0x35,0x77,0x72,0x77,0x71,0x72,0x1e,
                       0x00,0x00,0x00,0xFF,0x74,0x34,0x33,0x37,
                       0x35,0x42,0x38,0x32,0x35,0x37,0x44,0x44,
                       0x31,0x35,0x30,0x45,0xFF,0x44,0x37,0x36,
                       0x44,0x31,0x35,0x46,0x33,0x35,0x46,0x30,
                       0x44,0x11,0x00,0x00,0x00,0x31,0x31,0x3a,
                       0x32,0x32,0x3a,0x33,0x33,0x3a,0x34,0x34,
                       0x3a,0x35,0x35,0x3a,0x36,0x36,0x2d,0x1f,
                       0xd6,0x03,0xcc,0xf2,0x24,0x00,0x0a,0x00,
                       0x00,0x00,0x71,0x77,0x65,0x72,0x74,0x79,
                       0x75,0x69,0x6f,0x70};
       // qDebug()<<md54.toLatin1();
       for (int i=0;i<106;i++) keysend[i]=temple[i];
       for (int i=0;i<30;i++) keysend[i+33]=mdhash[i];
       QByteArray keydata;
       for (int i=0;i<300;i++) keydata.append(keysend[i]);
        usocket->write(keydata);
    }

}
void MainWindow::on_pushButton_clicked()
{
    char *user;
    name=ui->lineEdit->text();
    password=ui->lineEdit_2->text();
    user=name.toLatin1().data();
    login(user);
}
void MainWindow::buildLive()
{
    cout<<"live"<<endl;
    unsigned char liveframe[500]={0};
    unsigned char mid[]={0xe4,0x3e,0x86,0x02,
                         0x00,0x00,0x00,0x00,
                         0x5c,0x8f,0xc2,0xf5,
                         0xf0,0xa9,0xdf,0x40};
    liveframe[0]=0x82;liveframe[1]=0x23;liveframe[2]=0x1e;
    cout<<"live key: 0x"<<hex<<livekey<<endl;
    liveframe[3]=(unsigned char)((livekey & 0xff00)>>8);
    liveframe[4]=(unsigned char)livekey;
    for (int i=0;i<16;i++) liveframe[i+11]=mid[i];
    int pos=0;
    for (int i=0;i<name.size();i++)
    {
        liveframe[31+i]=name.toLatin1().data()[i];
        pos=31+i;
    }
    unsigned char spider[]={0x09,0x00,0x00,0x00,
                            0x53,0x70,0x69,0x64,
                            0x65,0x72,0x6d,0x61,
                            0x6e};
    for (int i=0;i<13;i++) liveframe[pos+i+1]=spider[i];
    livedata.clear();
     for (int i=0;i<500;i++) livedata.append(liveframe[i]);
}

void MainWindow::live()
{

connect(livepacket,SIGNAL(readyRead()),this,SLOT(showinfo()));
livepacket->write(livedata);
qDebug()<<"数据长度:";
qDebug()<<livedata.size();
}
void MainWindow::killtime()
{
     disconnect(timer,SIGNAL(timeout()),this,SLOT(live()));
     timer->stop();
}
void MainWindow::showinfo()
{
    char money2[500]={0};
    livepacket->read(money2,500);
    unsigned char money[500]={0};
    unsigned char *p=money;
    typedef union tagOct
    {
      double dData;
      int nData[2];
    }Oct;
     Oct monnum;
     for (int i=0;i<500;i++)
     {
           if (money2[i]!=0xff) //to filter 0xff
         {
             (*p)=(unsigned char)money2[i];
               p++;
         }
     }

   // for (int i=0;i<500;i++) cout<<hex<<(unsigned short)money[i];
        for (int i=0;i<500;i++)
        {
           if  (((unsigned char)money[i]==0x82) &&((unsigned char)money[i+1]==0x23) && ((unsigned char)money[i+2]==0x1f))
          {
              qDebug()<<"receive";
             for (int j=0;j<4;j++)
             {
              monnum.nData[0]=monnum.nData[0]|(money[i+19+j]<<(j*8));

             }
             for (int j=0;j<4;j++)
             {
                monnum.nData[1]=monnum.nData[1]|(money[i+23+j]<<(j*8));
             }

             QString moneyString=QString::number(monnum.dData);
             int pn=moneyString.indexOf(".");
             moneyString.truncate(pn+3);
             ui->label_4->setText(tr("剩余金额:")+moneyString+tr("元"));
             disconnect(timeoutf,SIGNAL(timeout()),this,SLOT(displaytimeout()));
             disconnect(livepacket,SIGNAL(readyRead()),this,SLOT(showinfo()));

              timeoutf->stop();
              break;

          }

         }

}
void MainWindow::displaytimeout()
{
    ui->label_4->setText(tr("服务器无响应"));
}

void MainWindow::on_pushButton_2_clicked()
{
    name="12345678";
    char *user2="12345678";
    password="0000";
     diss=1;
    login(user2);

}
int MainWindow::ispCon(int ispNum)
{
    CafesClient a;
    myMac=a.get_localmachine_mac(myIP);
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
       ispUdp->write(ispData);
       return 1;
}

void MainWindow::on_pushButton_4_clicked()
{
   int r= ispCon(1);
    if (r==1)
        ui->label_4->setText(tr("开放成功"));
    else
        ui->label_4->setText(tr("开放失败"));

}

void MainWindow::on_pushButton_5_clicked()
{
    int r= ispCon(2);
     if (r==1)
         ui->label_4->setText(tr("开放成功"));
     else
         ui->label_4->setText(tr("开放失败"));
}

void MainWindow::on_pushButton_6_clicked()
{
    int r= ispCon(3);
     if (r==1)
         ui->label_4->setText(tr("开放成功"));
     else
         ui->label_4->setText(tr("开放失败"));
}

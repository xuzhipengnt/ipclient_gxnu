#include "cafesclient.h"

CafesClient::CafesClient()
{
}
QString CafesClient::get_localmachine_name()
{
    QString machineName     = QHostInfo::localHostName();
    return machineName;
}
/*
 * 名称:get_localmachine_ip
 * 功能:获取本机的IP地址
 * 参数:no
 * 返回:QString
 */
QString CafesClient::get_localmachine_ip()
{
//    QString temp;
    QString ipAddress;
//    QList<QHostAddress> ipAddressesList = QNetworkInterface::allInterfaces();
//    // use the first non-localhost IPv4 address
//    for (int i = 0; i < ipAddressesList.size(); ++i) {
//        temp=ipAddressesList.at(i).toString();
//        temp=temp.left(3);

//          if (temp=="172" || temp=="202" & ipAddressesList.at(i).toIPv4Address())
//        {
//            ipAddress = ipAddressesList.at(i).toString();
//            break;
//        }
//    }
//    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = "0.0.0.0";
    return ipAddress;
}
/*
 * 名称:get_localmachine_mac
 * 功能:获取本机的MAC地址
 * 参数:no
 * 返回:void
 */
QString CafesClient::get_localmachine_mac(QString myIP)
{
    QList<QNetworkInterface> nets  = QNetworkInterface::allInterfaces();
      for (int i=0;i<nets.size();++i)
      {
        QList<QNetworkAddressEntry> entryList =nets.at(i).addressEntries();
        if (entryList.size()>0)
          {
            for (int j=0;j<entryList.size();j++)
              {
                if (entryList.at(j).ip().toString()==myIP)
                {
                    return nets.at(i).hardwareAddress();
                }
              }
          }

      }
     return "00:00:00:00:00:00";
}

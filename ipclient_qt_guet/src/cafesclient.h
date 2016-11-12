#ifndef CAFESCLIENT_H
#define CAFESCLIENT_H
#include <QHostInfo>
#include <QHostAddress>
#include <QList>
#include <QNetworkInterface>
class CafesClient
{
public:
    CafesClient();
    QString get_localmachine_name();
    QString get_localmachine_ip();
    QString get_localmachine_mac(QString);
};

#endif // CAFESCLIENT_H

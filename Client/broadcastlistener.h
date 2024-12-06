#ifndef BROADCASTLISTENER_H
#define BROADCASTLISTENER_H

#include <QUdpSocket>
#include <QTableWidget>

#include "connectingtablemodel.h"

/**
 * Класс для прослушивания broadcast UDP по порту 10001
 */
class BroadcastListener: public QObject {
    Q_OBJECT

public:
    BroadcastListener(ConnectingTableModel *model, QObject *parent = nullptr);

    void startListening();
    void stopListening();

private:
    const int BROADCAST_PORT = 10001;

    void readPendingDatagrams();
    bool isNewServer(const QString &address);

    QUdpSocket *udpSocket;
    ConnectingTableModel *model;
};

#endif // BROADCASTLISTENER_H

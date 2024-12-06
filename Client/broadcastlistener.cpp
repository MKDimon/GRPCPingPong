#include "broadcastlistener.h"

#include <QNetworkDatagram>
#include <QHostAddress>
#include <QDebug>

#include "pingclient.h"

BroadcastListener::BroadcastListener(ConnectingTableModel *model, QObject *parent)
    : QObject(parent), model(model),
    udpSocket(new QUdpSocket(this))
{}

void BroadcastListener::startListening() {
    udpSocket->bind(QHostAddress::Any, BROADCAST_PORT, QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress);
    connect(udpSocket, &QUdpSocket::readyRead, this, &BroadcastListener::readPendingDatagrams);
}

void BroadcastListener::stopListening() {
    disconnect(udpSocket, &QUdpSocket::readyRead, this, &BroadcastListener::readPendingDatagrams);
    udpSocket->close();
}

void BroadcastListener::readPendingDatagrams() {
    while (udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        QString address = QString::fromUtf8(datagram.data());

        if (isNewServer(address)) {
            qDebug() << "Found new address";;
            model->addConnection(QSharedPointer<PingConnection>::create(address));
        }
    }
}

bool BroadcastListener::isNewServer(const QString &address) {
    for (int row = 0; row < model->rowCount(); ++row) {
        if (model->data(model->index(row, 0)).toString() == address) {
            return false;
        }
    }
    return true;
}

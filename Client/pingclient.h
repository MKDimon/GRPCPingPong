#ifndef PINGCLIENT_H
#define PINGCLIENT_H

#include <QObject>
#include <QTime>
#include <QTimer>

#include "pingservice_client.grpc.qpb.h"

/**
 * Класс для подключения по адресу, который получает BroadcastListener
 * отправляет пинг, получает понг
 */
class PingConnection: public QObject
{
    Q_OBJECT

public:
    enum State {
        Offline = 0,
        Online = 1,
    };

    PingConnection(const QString &address, QObject *parent = nullptr);
    ~PingConnection();

    void start();
    void stop();

    PingConnection::State state() const;
    bool isConnected() const;
    QTime lastPingTime() const;
    QString address() const;

signals:
    void stateChanged(const PingConnection::State newState);
    void lastPingTimeChanged(const QTime& time);

private slots:
    void sendPing();

private:
    void setState(const State state);
    void setLastPingTime(const QTime& time);

    const int MAX_MISSED_PINGS = 3;
    const int PING_TIME = 2000;

    QTimer pingTimer;
    PingService::PingPongService::Client *m_client;
    PingConnection::State _state;
    QTime _lastPingTime;
    QString _address;
    int missedPings = 0;
};

#endif // PINGCLIENT_H

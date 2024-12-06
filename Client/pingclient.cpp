#include "pingclient.h"

#include <QGrpcHttp2Channel>

PingConnection::PingConnection(const QString &address, QObject *parent)
    : QObject(parent),
    _address(address),
    m_client(nullptr),
    _state(Offline)
{}

PingConnection::~PingConnection() {
    delete m_client;
}

void PingConnection::start() {
    if (_state == Online) {
        return;
    }
    qDebug() << "START CONNECT: " << _address;
    setState(Offline);

    if (!m_client) {
        qDebug() << "Create client";
        m_client = new PingService::PingPongService::Client(this);
    }
    QUrl url("http://" + _address);

    QGrpcChannelOptions channelOptions(std::move(url));
    std::shared_ptr<QAbstractGrpcChannel> channel = std::make_shared<QGrpcHttp2Channel>(std::move(channelOptions));

    if (!channel) {
        qDebug() << "Failed to create gRPC channel";
        return;
    }

    qDebug() << "Attach channel";
    m_client->attachChannel(channel);

    sendPing();
}

void PingConnection::stop() {
    qDebug() << "STOPPING";
    if (_state != Offline) {
        _state = Offline;
        emit stateChanged(_state);
    }

    if (m_client) {
        m_client->disconnect();
        delete m_client;
        m_client = nullptr;
        qDebug() << "gRPC client disconnected.";
    }

    missedPings = 0;
    qDebug() << "Connection stopped.";
}

PingConnection::State PingConnection::state() const {
    return _state;
}

bool PingConnection::isConnected() const {
    return _state == Online;
}

QTime PingConnection::lastPingTime() const {
    return _lastPingTime;
}

QString PingConnection::address() const {
    return _address;
}

void PingConnection::sendPing() {
    qDebug() << "Send ping";
    qDebug() << "Misses: " << missedPings;
    if (!m_client) {
        qWarning() << "Client is not initialized!";
        return;
    }

    PingService::PingMessage request;
    request.setMessage("Ping");
    auto reply = m_client->Ping(request);

    QObject::connect(reply.get(), &QGrpcCallReply::errorOccurred,
                     this, [this, replyPtr = reply.get()](const QGrpcStatus &status) {
                         qDebug() << "Getting error:" << status.message();
                         missedPings++;
                         if (missedPings >= MAX_MISSED_PINGS) {
                             stop();
                         } else {
                             QTimer::singleShot(PING_TIME, this, &PingConnection::sendPing);
                         }
                         disconnect(replyPtr, &QGrpcCallReply::errorOccurred, this, nullptr);
                         disconnect(replyPtr, &QGrpcCallReply::finished, replyPtr, nullptr);
                     });

    QObject::connect(reply.get(), &QGrpcCallReply::finished,
                     reply.get(), [this, replyPtr = reply.get()]() {
                         std::optional<PingService::PongMessage> message = replyPtr->read<PingService::PongMessage>();
                         if (message) {
                             qDebug() << message->message();
                             missedPings = 0;
                             _lastPingTime = QTime::currentTime();
                             emit lastPingTimeChanged(_lastPingTime);

                             if (_state != Online) {
                                 _state = Online;
                                 emit stateChanged(_state);
                             }
                             QTimer::singleShot(PING_TIME, this, &PingConnection::sendPing);
                         } else {
                             qDebug() << "Getting error?";
                         }
                         setLastPingTime(QTime::currentTime());
                         disconnect(replyPtr, &QGrpcCallReply::errorOccurred, this, nullptr);
                         disconnect(replyPtr, &QGrpcCallReply::finished, replyPtr, nullptr);
                     });
}

void PingConnection::setState(const State state) {
    _state = state;
    emit stateChanged(_state);
}

void PingConnection::setLastPingTime(const QTime& time) {
    _lastPingTime = time;
    emit lastPingTimeChanged(_lastPingTime);
}

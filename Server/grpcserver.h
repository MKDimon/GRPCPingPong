#ifndef GRPCSERVER_H
#define GRPCSERVER_H

#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include <QThread>
#include <grpcpp/grpcpp.h>

#include "generated_proto/pingservice.grpc.pb.h"

/**
 * Класс-сервер для grpc
 */
class GRPCServer : public QObject, public PingService::PingPongService::Service {
    Q_OBJECT
public:
    explicit GRPCServer(QObject *parent = nullptr);

    void runGRPCServer(const int connectPort = 50001);

    grpc::Status Ping(grpc::ServerContext *context, const PingService::PingMessage *request,
                      PingService::PongMessage *response) override;

signals:
    void gettingPing();

private slots:
    void checkTimeout();

private:
    void startBroadcast();
    void stopBroadcast();

    const QString broadcastIP = "127.0.0.1";
    const int broadcastPort = 10001;

    QString connectIP = "127.0.0.1";
    int connectPort;

    QUdpSocket *udpSocket;

    QTimer broadcastTimer;
    QTimer timeoutTimer;
    std::unique_ptr<grpc::Server> grpcServer;
};


#endif // GRPCSERVER_H

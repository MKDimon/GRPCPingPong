#include "grpcserver.h"

#include <QHostAddress>
#include <QDebug>
#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/support/async_stream.h>


GRPCServer::GRPCServer(QObject *parent)
    : QObject(parent), udpSocket(new QUdpSocket(this)) {
    timeoutTimer.setSingleShot(true);
    timeoutTimer.setInterval(15000);
    connect(&timeoutTimer, &QTimer::timeout, this, &GRPCServer::checkTimeout);
}

void GRPCServer::startBroadcast() {
    broadcastTimer.start(1000);
    connect(&broadcastTimer, &QTimer::timeout, this, [this]() {
        QByteArray message = QString("%1:%2").arg(connectIP).arg(connectPort).toUtf8();
        udpSocket->writeDatagram(message, QHostAddress(broadcastIP), broadcastPort);
        qDebug() << "Broadcasting:" << message;
    });
}

void GRPCServer::stopBroadcast() {
    broadcastTimer.stop();
    disconnect(&broadcastTimer, &QTimer::timeout, this, nullptr);
    qDebug() << "Broadcast stopped";
    timeoutTimer.start();
}

grpc::Status GRPCServer::Ping(grpc::ServerContext* context,
                              const PingService::PingMessage* request,
                              PingService::PongMessage* response) {
    QMetaObject::invokeMethod(this, &GRPCServer::stopBroadcast, Qt::QueuedConnection);
    qDebug() << "Received Ping: " << request->message();
    emit gettingPing();
    response->set_message("Pong");
    return grpc::Status::OK;
}

void GRPCServer::checkTimeout() {
    startBroadcast();
}

void GRPCServer::runGRPCServer(const int connectPort) {
    if (grpcServer) {
        timeoutTimer.stop();
        broadcastTimer.stop();
        disconnect(&broadcastTimer, &QTimer::timeout, this, nullptr);
        qDebug() << "Broadcast stopped";
        grpcServer->Shutdown();
        grpcServer->Wait();
        qDebug() << "grpcServer shutdown";
    }
    this->connectPort = connectPort;
    QString address = connectIP + ":" + QString::number(connectPort);

    grpc::ServerBuilder builder;
    builder.AddListeningPort(address.toStdString(), grpc::InsecureServerCredentials());
    builder.RegisterService(this);

    grpcServer = builder.BuildAndStart();
    qDebug() << "Server listening on " << address;
    startBroadcast();
}

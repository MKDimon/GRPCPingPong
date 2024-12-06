#include "connectingtablemodel.h"


 ConnectingTableModel::ConnectingTableModel(QObject *parent)
    : QAbstractTableModel(parent) {}

 int ConnectingTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return connections.size();
}

int ConnectingTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 4;
}

QVariant ConnectingTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= connections.size())
        return QVariant();

    const auto &connect = connections.at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0: return connect->address();
        case 1: return connect->lastPingTime().toString("HH:mm:ss");
        case 2: return connect->state() == PingConnection::Online ? "Online" : "Offline";
        case 3: return connect->isConnected() ? "Disconnect" : "Connect";
        }
    }
    return QVariant();
}

QVariant ConnectingTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    switch (section) {
    case 0: return "Address";
    case 1: return "Last Ping Time";
    case 2: return "Status";
    case 3: return "Action";
    }
    return QVariant();
}

Qt::ItemFlags ConnectingTableModel::flags(const QModelIndex &index) const {
    if (!index.isValid()){
        return Qt::NoItemFlags;
    }

    // Кнопка в 4 столбце
    if (index.column() == 3){
        return Qt::ItemIsEnabled;
    }
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void ConnectingTableModel::addConnection(QSharedPointer<PingConnection> connection) {
    if (!connection){
        return;
    }

    beginInsertRows(QModelIndex(), connections.size(), connections.size());
    connections.append(std::move(connection));
    endInsertRows();

    connect(connections.last().data(), &PingConnection::lastPingTimeChanged, this, [this](const QTime& newTime) {
        auto *senderConnection = qobject_cast<PingConnection *>(sender());
        if (!senderConnection)
            return;

        for (int i = 0; i < connections.size(); ++i) {
            if (connections[i].data() == senderConnection) {
                updatePingTime(i);
                break;
            }
        }
    });

    connect(connections.last().data(), &PingConnection::stateChanged, this, [this](const PingConnection::State newState) {
        auto *senderConnection = qobject_cast<PingConnection *>(sender());
        if (!senderConnection)
            return;

        for (int i = 0; i < connections.size(); ++i) {
            if (connections[i].data() == senderConnection) {
                updateStatus(i);
                break;
            }
        }
    });
}

void ConnectingTableModel::updatePingTime(int row) {
    if (row < 0 || row >= connections.size())
        return;

    emit dataChanged(index(row, 1), index(row, 1), {Qt::DisplayRole});
}

void ConnectingTableModel::updateStatus(int row) {
    if (row < 0 || row >= connections.size())
        return;

    emit dataChanged(index(row, 2), index(row, 2), {Qt::DisplayRole});
    emit dataChanged(index(row, 3), index(row, 3), {Qt::DisplayRole});
}

void ConnectingTableModel::toggleConnection(const QModelIndex &_index) {
    auto row = _index.row();
    if (row < 0 || row >= connections.size())
        return;

    auto connection = connections[row];
    if (connection->isConnected()) {
        connection->stop();
    } else {
        connection->start();
    }
    emit dataChanged(index(row, 3), index(row, 3), {Qt::DisplayRole});
}

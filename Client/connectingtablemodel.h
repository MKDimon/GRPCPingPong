#ifndef CONNECTINGTABLEMODEL_H
#define CONNECTINGTABLEMODEL_H

#include <QAbstractTableModel>
#include <QTimer>
#include <QVector>
#include <QSharedPointer>

#include "pingclient.h"

/**
 * Модель для подключений PingConnection
 */
class ConnectingTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit ConnectingTableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void addConnection(QSharedPointer<PingConnection> connection);

    void updatePingTime(int row);

    void updateStatus(int row);

    void toggleConnection(const QModelIndex &index);

private:
    QVector<QSharedPointer<PingConnection>> connections;
};



#endif // CONNECTINGTABLEMODEL_H

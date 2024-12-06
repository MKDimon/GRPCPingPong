#include <QApplication>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>
#include <QHeaderView>
#include "ConnectingTableModel.h"
#include "broadcastlistener.h"
#include "actionbuttondelegate.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget mainWindow;
    mainWindow.setWindowTitle("Client Connections");

    ConnectingTableModel *model = new ConnectingTableModel;
    BroadcastListener *listener = new BroadcastListener(model);

    QTableView *tableView = new QTableView(&mainWindow);
    tableView->setModel(model);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setSelectionMode(QTableView::NoSelection);
    tableView->setEditTriggers(QTableView::NoEditTriggers);

    ActionButtonDelegate *actionDelegate = new ActionButtonDelegate(tableView);
    QObject::connect(actionDelegate, &ActionButtonDelegate::actionClicked, model, &ConnectingTableModel::toggleConnection);
    tableView->setItemDelegateForColumn(3, actionDelegate);

    QVBoxLayout *mainLayout = new QVBoxLayout(&mainWindow);
    mainLayout->addWidget(tableView);

    mainWindow.resize(800, 600);
    mainWindow.show();

    listener->startListening();
    return app.exec();
}

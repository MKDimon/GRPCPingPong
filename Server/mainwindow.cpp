#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , grpcServer(new GRPCServer(this))
{
    ui->setupUi(this);
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout;

    m_logTextEdit = new QTextEdit(this);
    m_logTextEdit->setReadOnly(true);
    layout->addWidget(m_logTextEdit);

    QLabel *label = new QLabel("Port", this);
    layout->addWidget(label);

    m_portLineEdit = new QLineEdit(this);
    QRegularExpression regex("^[1-9]\\d+$");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regex, this);
    m_portLineEdit->setValidator(validator);
    layout->addWidget(m_portLineEdit);

    m_startButton = new QPushButton("Start", this);
    layout->addWidget(m_startButton);

    centralWidget->setLayout(layout);

    connect(grpcServer, &GRPCServer::gettingPing, this, [this]() {
        appendLog("Ping");
    });

    connect(m_startButton, &QPushButton::clicked, this, [this]() {
        QString port = m_portLineEdit->text();
        if (!port.isEmpty()) {
            qDebug() << "Server starting on port" << port;
            appendLog("Server starting on port " + port);
            grpcServer->runGRPCServer(port.toInt());
        } else {
            qWarning() << "Port is empty!";
            appendLog("Port is empty!");
        }
    });
}

void MainWindow::appendLog(const QString &message) {
    m_logTextEdit->append(message);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete grpcServer;
    delete m_portLineEdit;
    delete m_startButton;
    delete m_logTextEdit;
}

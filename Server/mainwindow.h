#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>

#include "grpcserver.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void appendLog(const QString &message);

private:
    Ui::MainWindow *ui;
    GRPCServer *grpcServer;
    QLineEdit *m_portLineEdit;
    QPushButton *m_startButton;
    QTextEdit *m_logTextEdit;
};
#endif // MAINWINDOW_H

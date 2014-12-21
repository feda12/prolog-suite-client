#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QTextEdit>
#include <QLabel>

#include "codeeditor.h"
#include "client.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:

private slots:
    void connectServer();
    void disconnectServer();
    void changeServerStatus(bool s);
    void writeServerOutput(const QString &str);
    void serverConnected();
    void serverDisconnected();

    void sendCmd();
    void consultFile();
private:
    QSpinBox *_port;
    QLineEdit *_host;
    QPushButton *_connectServer;
    QPushButton *_consult;
    QPushButton *_sendCmd;
    QLineEdit *_cmd;
    QTextEdit *_serverOutput;
    CodeEditor *_codeEditor;

    Client *_client;
};

#endif // MAINWINDOW_H

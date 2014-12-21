#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    _port = new QSpinBox(this);
    _port->setMaximum(99999);
    _port->setValue(8796);
    _host = new QLineEdit(this);
    _host->setFixedWidth(100);
    _connectServer = new QPushButton(tr("Connect"), this);
    _connectServer->setCheckable(true);
    _consult = new QPushButton(tr("Consult"), this);
    _sendCmd  = new QPushButton(tr("Send"), this);
    _cmd  = new QLineEdit(this);
    _serverOutput = new QTextEdit(this);
    _serverOutput->setReadOnly(true);
    _codeEditor = new CodeEditor(this);

    _consult->setEnabled(false);
    _cmd->setEnabled(false);
    _sendCmd->setEnabled(false);

    _client = new Client();

    QObject::connect(_connectServer, SIGNAL(clicked(bool)), this, SLOT(changeServerStatus(bool)));
    QObject::connect(_client, SIGNAL(connected()), this, SLOT(serverConnected()));
    QObject::connect(_client, SIGNAL(disconnected()), this, SLOT(serverDisconnected()));
    QObject::connect(_client, SIGNAL(dataReceived(QString)), this, SLOT(writeServerOutput(QString)));
    QObject::connect(_client, SIGNAL(errorSocket(QString)), this, SLOT(writeServerOutput(QString)));
    QObject::connect(_sendCmd, SIGNAL(clicked()), this, SLOT(sendCmd()));
    QObject::connect(_consult, SIGNAL(clicked()), this, SLOT(consultFile()));

    QHBoxLayout *layoutHeader = new QHBoxLayout;
    layoutHeader->setContentsMargins(10,0,0,10);
    layoutHeader->addWidget(new QLabel(tr("Host"), this));
    layoutHeader->addWidget(_host);
    layoutHeader->addSpacing(10);
    layoutHeader->addWidget(new QLabel(tr("Port"), this));
    layoutHeader->addWidget(_port);
    layoutHeader->addStretch(100);
    layoutHeader->addWidget(_connectServer);

    QSplitter *centralSplitter = new QSplitter(this);
    centralSplitter->addWidget(_codeEditor);
    centralSplitter->addWidget(_serverOutput);

    QHBoxLayout *layoutBottom = new QHBoxLayout;
    layoutBottom->addWidget(_consult, 1);
    layoutBottom->addWidget(_cmd, 100);
    layoutBottom->addWidget(_sendCmd, 1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addLayout(layoutHeader, 1);
    mainLayout->addWidget(centralSplitter);
    mainLayout->addLayout(layoutBottom, 1);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);

    this->setCentralWidget(centralWidget);
}

void MainWindow::connectServer()
{
    QString host = _host->text();
    if(host.size() != 0)
    {
        _client->connect(host, _port->value());
        _connectServer->setEnabled(false);
    }
}

void MainWindow::disconnectServer()
{
    _connectServer->setEnabled(false);
    _client->disconnect();

}

void MainWindow::writeServerOutput(const QString &str)
{
    _serverOutput->append(str);
}

void MainWindow::serverConnected()
{
    this->writeServerOutput("Server connected.");

    _connectServer->setEnabled(true);
    _connectServer->setText(tr("Disconnect"));

    _host->setReadOnly(true);
    _port->setReadOnly(true);

    _consult->setEnabled(true);
    _cmd->setEnabled(true);
    _sendCmd->setEnabled(true);
}

void MainWindow::serverDisconnected()
{
    this->writeServerOutput("Server disconnected.");

    _connectServer->setEnabled(true);
    _connectServer->setText(tr("Connect"));
    _host->setReadOnly(false);
    _port->setReadOnly(false);
    _consult->setEnabled(false);
    _cmd->setEnabled(false);
    _sendCmd->setEnabled(false);
}

void MainWindow::sendCmd()
{
    if(_cmd->text().size() == 0)
    {
        return;
    }
    _client->send(_cmd->text());
    _cmd->clear();
}

void MainWindow::consultFile()
{
    QString code = _codeEditor->toPlainText();
     _client->consult(code);
}

void MainWindow::changeServerStatus(bool s)
{
    if(s)
        connectServer();
    else
        disconnectServer();
}


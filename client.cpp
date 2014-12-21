#include "client.h"

Client::Client()
{
    _socket = new QTcpSocket(this);
    QObject::connect(_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    QObject::connect(_socket, SIGNAL(connected()), this, SLOT(socketConnected()));
    QObject::connect(_socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    QObject::connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));

    _msgSize  = 0;
}

// Tentative de connexion au serveur
void Client::connect(const QString &host, quint16 port)
{
   qDebug() << "Trying to connect to "+host+":"+QString::number(port);

    _socket->abort();
    _socket->connectToHost(host, port);
}

void Client::disconnect()
{
    _socket->abort();
}

// Envoi d'un message au serveur
void Client::send(const QString &toSend)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    QString preparedData = toSend;
    if(preparedData.right(4) != "\r\n")
         preparedData.append("\r\n");

    out << (quint16) 0;
    out << preparedData;
    out.device()->seek(0);
    out << (quint16) (packet.size() - sizeof(quint16));

    _socket->write(packet);
}

void Client::consultFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Could not open file to consult";
        return;
    }
    QString data(file.readAll());
    data.replace("\n", " ");
    this->send("consult"+data);
}

void Client::consult(QString &code)
{
    code.replace("\n", " ");
    this->send("consult"+code);
}

//Getting data back from server
void Client::readyRead()
{
    QDataStream in(_socket);

    if (_msgSize == 0)
    {
        if (_socket->bytesAvailable() < (int)sizeof(quint16))
             return;

        in >> _msgSize;
    }

    if (_socket->bytesAvailable() < _msgSize)
        return;

    QString dataBack;
    in >> dataBack;

     qDebug() << dataBack;
    emit dataReceived(dataBack);
    _msgSize = 0;
}


void Client::socketConnected()
{
    qDebug() << "Connected to server";
    emit connected();
}

void Client::socketDisconnected()
{
    qDebug() << "Disconnected from server";
    emit disconnected();
}

void Client::socketError(QAbstractSocket::SocketError error)
{
    switch(error)
    {
        case QAbstractSocket::HostNotFoundError:
            emit errorSocket("<em>Host not found. Chech hostname and port.</em>");
            break;
        case QAbstractSocket::ConnectionRefusedError:
           emit errorSocket(tr("<em>Refused connection</em>"));
            break;
        case QAbstractSocket::RemoteHostClosedError:
            emit errorSocket(tr("<em>Disconnected</em>"));
            break;
        default:
            emit errorSocket(tr("<em> ") + _socket->errorString() + tr("</em>"));
    }
}

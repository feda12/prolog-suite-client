#ifndef HEADER_CLIENT
#define HEADER_CLIENT

#include <QtGui>
#include <QtNetwork>


class Client : public QObject
{
    Q_OBJECT
    public:
        Client();

    public slots:
        void connect(const QString &host, quint16 port);
        void send(const QString &toSend);
        void consultFile(const QString &path);
        void consult(QString &code);
        void disconnect();
    private slots:
        void readyRead();
        void socketConnected();
        void socketDisconnected();
        void socketError(QAbstractSocket::SocketError error);
signals:
        void dataReceived(const QString &);
        void connected();
        void disconnected();
        void errorSocket(const QString &);
    private:
        QTcpSocket *_socket;
        quint16 _msgSize;
};

#endif

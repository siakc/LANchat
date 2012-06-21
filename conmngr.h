#ifndef CONMNGR_H
#define CONMNGR_H
#define _CRT_SECURE_NO_WARNINGS
#include <map>
#include <functional>
#include <QList>
#include <QThread>
#include <QtNetwork\QNetworkAccessManager>
#include <QtNetwork\QTcpSocket>
#include <QtNetwork\QTcpServer>
#include <QtNetwork\QUdpSocket>
#include <QtNetwork\QNetworkInterface>
#include <QStringList>


struct lessQHostAddress : public std::binary_function <QHostAddress, QHostAddress, bool>
{
    bool operator()( const QHostAddress& _Left,const QHostAddress& _Right) const
    {
        return _Right.toIPv4Address() > _Left.toIPv4Address();

    }
};

typedef std::map<QHostAddress, QString, lessQHostAddress  > NickIpPairMap;
typedef std::pair<QHostAddress, QString> NickIpPair;
typedef NickIpPairMap::const_iterator NickIpCIter;



class ConnectionManager : public QObject
{
    Q_OBJECT
    enum PacketTypes {ADZ, CON, HSH, TDC, UNN, MSG};

    QByteArray LANCHATSIG;
    static const int LISTEN_PORT = 5160;

    QUdpSocket *udpSocket;
    QTcpServer *listenSocket;
    QTcpSocket *incommingTcp;
    QTcpSocket *outgoingTcp;
    QHostAddress listenAddress;
    quint32 netmask;
    QTcpSocket *connectionFrom; //peer--->us
    QTcpSocket *connectionTo;   //us--->peer
    QString nickname;
    NickIpPairMap *nickIpPairList;

    QString readBuffer;

    void ManageConnections();
    void OnLanChatPacket(QByteArray *packet);

    QByteArray CreateADZPacket();		//UDP advertize
    QByteArray CreateCONPacket();               //Connect to advertizer, sends ip so NAT can be detected
    QByteArray CreateHSHPacket();               //Advertizer accepts connection
    QByteArray CreateTDCPacket();		//Sent after the tail coonection has been dropped
    QByteArray CreateMSGPacket(QString message);

    void OnADZPacket(QStringList *payloads);
    void OnCONPacket(QStringList *payloads);
    void OnHSHPacket(QStringList *payloads);
    void OnMSGPacket(QStringList *payloads, bool isFromTail);

    PacketTypes DisassemblePacket(QByteArray *packet, QStringList *payloads);

private Q_SLOTS:
    void OnIncommingConnection();
    void OnPendingDatagrams();
    void OnReadTcpTo();
    void OnReadTcpIncomming();
    void OnReadTcpOutgoing();
    void OnReadTcpFrom();
    void OnTcpFromDisconnect();
    void OnTcpToDisconnect();
    void OnTcpIncommingDisconnect();
    void OnTcpOutgoingDisconnect();
    void OnHandshakeOutgoing();

Q_SIGNALS:
    void NewMessage();
    void NicknamesChanged();


public:
    ~ConnectionManager();
    ConnectionManager();
    void StartNetwork(QHostAddress subnet, quint32 netmaskOnes, bool broadcast = true);
    void StopNetwork();
    void LogMessage(QString msg)
    {
        readBuffer = msg;
        emit NewMessage();
    }

    QString GetListenAddress()
    {
        return listenSocket->serverAddress().toString();
    }

    void SetMyNickname(QString name)
    {
        nickname = name;
    }

    QString GetMyNickname()
    {
        return nickname;
    }


    bool GetNicknames(QStringList *nickNames)
    {
        if(nickIpPairList == nullptr) return false;
        else
	    for(NickIpCIter itter = nickIpPairList->cbegin();itter != nickIpPairList->cend() ;++itter)
		nickNames->append( itter->second);

        return true;

    }

    bool GetNicknameIpPairs(QList<NickIpPair> *nickIpPair)
    {
        //QASSERT(nickIpPair!=nullptr);
        if(nickIpPairList == nullptr) return false;
        else
            for(NickIpCIter it = nickIpPairList->cbegin();it != nickIpPairList->cend() ;++it)
                nickIpPair->append(NickIpPair(it->first, it->second));

        return true;

    }
    QString ReadMessage();
    void WriteMessage(QString message);

};

#endif // CONMNGR_H

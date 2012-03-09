#include <conmngr.h>
/*Packet types
adz : UDP advertize
con : For tcp hanshake start
hsh : respond to con
nnc : nick name has changed, can be udp or tcp
unn : gives a name
*/
ConnectionManager::ConnectionManager()
{
    udpSocket = nullptr;
    listenSocket= nullptr;
    connectionFrom= nullptr;
    connectionTo= nullptr;
    nickIpPairList= nullptr;
    incommingTcp = nullptr;
    outgoingTcp = nullptr;
    nickname = "MyNick";
}

ConnectionManager::~ConnectionManager()
{
    StopNetwork();
}


void ConnectionManager::StartNetwork(QHostAddress subnet, quint32 netmaskOnes, bool broadcast)
{
    LogMessage("Connection Manager Started.");

    QList<QNetworkInterface> il = QNetworkInterface::allInterfaces();
    bool found = false;

    //Creating subnet mask from the number user has entered
    netmask = 0;
    netmask = ~netmask;
    netmask <<= 32 - netmaskOnes;

    for(int i=0 ; i<il.size();++i)
    {
        //LogMessage(il[i].humanReadableName() + ':');
        if( (il[i].flags() & QNetworkInterface::IsLoopBack) || !(il[i].flags() & QNetworkInterface::IsUp) || !(il[i].flags() & QNetworkInterface::CanBroadcast)) continue;
        QList<QNetworkAddressEntry> addList = il[i].addressEntries();
        for(int j = 0; j < addList.size();++j)
        {
            //LogMessage("  " + addList[j].ip().toString() + "/" + addList[j].netmask().toString() +"="+QHostAddress(addList[j].netmask().toIPv4Address() & addList[j].ip().toIPv4Address()).toString());
            if((addList[j].ip().protocol() == QAbstractSocket::IPv4Protocol )&&((subnet.toIPv4Address() & netmask)== (addList[j].netmask().toIPv4Address() & addList[j].ip().toIPv4Address())))
            {
                listenAddress = addList[j].ip();
                LogMessage("\""+il[i].humanReadableName() + "\" interface selected." );
                found = true;
                break;
            }
        }
        if(found) break;
    }
    if(!found)
    {
        LogMessage("No suitable interface found. Using whatever comes up!");
        listenAddress = QHostAddress::Any;
    }

    nickIpPairList = new NickIpPairMap;
    listenSocket = new QTcpServer(this);

    udpSocket = new QUdpSocket(this);
    if(!udpSocket->bind(listenAddress, LISTEN_PORT))
    {
        QString s("Binding of UDP socket failed.");
        throw s;
    }
    if(!listenSocket->listen(listenAddress, LISTEN_PORT))
    {
        QString s("Listening to TCP socket failed.");
        throw s;
    }

    if( !connect(listenSocket, SIGNAL(newConnection()), this, SLOT(OnIncommingConnection())))
    {
        QString s("Connecting newConnection()->OnIncommingConnection() failed.");
        throw s;
    }

    if(!connect(udpSocket, SIGNAL(readyRead()), this, SLOT(OnPendingDatagrams())))
    {
        QString s("Connecting readyRead()->OnPendingDatagrams() failed.");
        throw s;
    }


    QByteArray broadcastBytes = CreateADZPacket(nickname, listenSocket->serverAddress().toString());
    
    LogMessage("Our address is " + listenSocket->serverAddress().toString() /*+ ":" + LISTEN_PORT*/);
    //Broadcasting ourselves

    if(broadcast)
    {
        udpSocket->writeDatagram(broadcastBytes, QHostAddress(listenSocket->serverAddress().toIPv4Address() | ~netmask), LISTEN_PORT);
    }else
    {
       quint32 a = listenSocket->serverAddress().toIPv4Address() & netmask;
       for(quint32 i= 1 ; ~((i+a)|netmask) != 0; ++i)
       {
           udpSocket->writeDatagram(broadcastBytes.data(), broadcastBytes.size(), QHostAddress(a+i), LISTEN_PORT);
           //LogMessage(QHostAddress(a+i).toString());
       }
    }
    LogMessage("Advertise sent.");

}


void ConnectionManager::OnReadTcpTo()
{
    QByteArray readBytes;
    readBytes.resize(connectionTo->bytesAvailable());
    readBytes = connectionTo->read(connectionTo->bytesAvailable());
    if(!readBytes.startsWith("LANchat"))
    {
        LogMessage("TCP packet was not from LANchat, ignored.");
        return;
    }
    QStringList payloads;
    PacketTypes type = DisassemblePacket(&readBytes, &payloads);
    switch(type)
    {
    case MSG:
        if(connectionFrom != nullptr) connectionFrom->write(CreateMSGPacket(QString(payloads[1])));
        readBuffer = payloads[1];
        emit NewMessage();
        break;
    default:
        LogMessage("Unexpected packet type from acceptor.");


    }

}

void ConnectionManager::OnReadTcpFrom()
{
    QByteArray readBytes;
    readBytes.resize(connectionFrom->bytesAvailable());
    readBytes = connectionFrom->read(connectionFrom->bytesAvailable());
    if(!readBytes.startsWith("LANchat"))
    {
        LogMessage("TCP packet was not from LANchat, ignored.");
        return;
    }
    QStringList payloads;
    PacketTypes type = DisassemblePacket(&readBytes, &payloads);
    switch(type)
    {
    case UNN:
        OnUNNPacket(&payloads);
        break;
    case MSG:
        if(connectionTo != nullptr) connectionTo->write(CreateMSGPacket(QString(payloads[1])));
        readBuffer = payloads[1];
        emit NewMessage();
        break;
    default:
        LogMessage("Unexpected packet type from initiator.");

    }
}

void ConnectionManager::OnIncommingConnection()
{
    LogMessage("Incomming TCP connection...");
    if(incommingTcp != nullptr) delete incommingTcp; //We only keep track of the last TCP made and wait for CON
    incommingTcp = listenSocket->nextPendingConnection();
    connect(incommingTcp, SIGNAL(readyRead()), this, SLOT(OnReadTcpIncomming()));
    connect(incommingTcp, SIGNAL(disconnected()), this, SLOT(OnTcpIncommingDisconnect()));
}

void ConnectionManager::OnPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress udpPeer;
        quint16 peerPort;
        QStringList payloads;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), udpSocket->pendingDatagramSize(), &udpPeer, &peerPort);
        char dummy[50];
        LogMessage("UDP datagram received from " + udpPeer.toString() + ':' + QString(_itoa(peerPort, dummy, 10)));
        if(!datagram.startsWith("LANchat"))
        {
            LogMessage("UDP datagram was not from LANchat, ignored.");
            return;
        }

        PacketTypes packetType = DisassemblePacket(&datagram, &payloads);
        if(packetType == ADZ)
        {
            OnADZPacket(&payloads);
        }
    }
}

QString ConnectionManager::ReadMessage()
{
    QString temp = readBuffer;
    readBuffer.clear();
    return temp;
}

void ConnectionManager::WriteMessage(QString message)
{
    if(connectionFrom != nullptr) connectionFrom->write(CreateMSGPacket(message));
    if(connectionTo != nullptr) connectionTo->write(CreateMSGPacket(message));
}

void ConnectionManager::OnHandshakeOutgoing()
{
    outgoingTcp->write(CreateCONPacket(listenSocket->serverAddress().toString()));
    LogMessage("Sent CON packet.");
}

void ConnectionManager::OnTcpToDisconnect()
{
    connectionTo->deleteLater();
    connectionTo = nullptr;
    LogMessage("Disconnected from our acceptor.");
}

void ConnectionManager::OnTcpFromDisconnect()
{
    connectionFrom->deleteLater();
    connectionFrom = nullptr;
    LogMessage("Disconnected from our initiator.");
}

void ConnectionManager::StopNetwork()
{
    LogMessage("Disconnected.");
    this->disconnect();
    if(connectionTo != nullptr) delete connectionTo;
    if(connectionFrom != nullptr) delete connectionFrom;
    if(nickIpPairList != nullptr) delete nickIpPairList;
    if(outgoingTcp != nullptr) delete outgoingTcp;
    if(incommingTcp != nullptr) delete incommingTcp;
    if(udpSocket != nullptr) delete udpSocket;
    if(listenSocket != nullptr) delete listenSocket;
    connectionTo = nullptr;
    connectionFrom = nullptr;
    nickIpPairList = nullptr;
    outgoingTcp =nullptr;
    incommingTcp=nullptr;
    udpSocket=nullptr;
    listenSocket= nullptr;


}

void ConnectionManager::OnReadTcpIncomming()
{
    QByteArray readBytes;
    QStringList payloads;
    PacketTypes type;
    readBytes.resize(incommingTcp->bytesAvailable());
    readBytes = incommingTcp->read(incommingTcp->bytesAvailable());
    if(!readBytes.startsWith("LANchat"))
    {
        incommingTcp->deleteLater();
        incommingTcp = nullptr;
        LogMessage("Incomming TCP connection was not from LANchat, connection closed.");
        return;
    }
    else
        type = DisassemblePacket(&readBytes, &payloads);

    if(type != CON)
    {
        incommingTcp->deleteLater();
        incommingTcp = nullptr;
        LogMessage("Peer didn't start with CON, connection closed.");
        return;
    }
    else
        OnCONPacket(&payloads);
}

void ConnectionManager::OnReadTcpOutgoing()
{
    QByteArray readBytes;
    QStringList payloads;
    PacketTypes type;
    readBytes.resize(outgoingTcp->bytesAvailable());
    readBytes = outgoingTcp->read(outgoingTcp->bytesAvailable());
    if(!readBytes.startsWith("LANchat"))
    {
        outgoingTcp->deleteLater();
        outgoingTcp = nullptr;
        LogMessage("Peer respond didn't start with LANchat, connection closed.");
        return;
    }
    else
        type = DisassemblePacket(&readBytes, &payloads);


    if(type != HSH)
    {
        outgoingTcp->deleteLater();
        outgoingTcp = nullptr;
        LogMessage("Peer didn't respond with HSH, connection closed.");

        return;
    }
    else
        OnHSHPacket(&payloads);

}


void ConnectionManager::OnADZPacket(QStringList *packet)
{
    QString nick, IP;
    nick = (*packet)[1];
    IP = (*packet)[2];

    if(listenSocket->serverAddress().toString() == IP )
    {
        LogMessage("UDP datagram was ours, ignored.");
        return;
    }

    nickIpPairList->insert(NickIpPair(QHostAddress(IP), nick));
    emit NicknamesChanged();
    LogMessage("UDP datagram has advertise flag, registering >> " + nick + " (" + IP + ") ");
    if(connectionTo == nullptr)
    {
        if(outgoingTcp != nullptr) delete outgoingTcp;
        //Not sure if it is a right thing to do
        //It means we were connecting to another peer. Then we got another advertize so we let go of the first one.
        //and try to connect to new born peer and leaving first one alone
        outgoingTcp = new QTcpSocket(this);
        connect(outgoingTcp, SIGNAL(connected()), this, SLOT(OnHandshakeOutgoing()));
        connect(outgoingTcp, SIGNAL(readyRead()), this, SLOT(OnReadTcpOutgoing()));
        connect(outgoingTcp, SIGNAL(disconnected()), this, SLOT(OnTcpOutgoingDisconnect()));
        //connect(connectionTo, SIGNAL(disconnected()), this, SLOT(OnTCPInitiatorDisconnect()));
        outgoingTcp->connectToHost(QHostAddress(IP), LISTEN_PORT); //should use events to intercept

        LogMessage("Connecting to advertiser....");
    }
    else
    {
        LogMessage("Already connected to another peer, ignoring advertise datagram.");
        return; //We have already made connection to a peer
    }
}

void ConnectionManager::OnNNCPacket(QStringList *packet)
{


}

void ConnectionManager::OnUNNPacket(QStringList *packet)
{


}

void ConnectionManager::OnCONPacket(QStringList *packet)
{

    QString IP = (*packet)[1];
    incommingTcp->disconnect();
    connectionFrom = incommingTcp;
    incommingTcp = nullptr;
    //if sock ip != manifested ip client is behind a NAT
    if(connectionFrom->peerAddress().toString() != IP)
    {
        LogMessage("Peer with IP \"" + connectionFrom->peerAddress().toString() + "\" is behind NAT.");
        /* do something*/
    }
    connect(connectionFrom, SIGNAL(readyRead()), this, SLOT(OnReadTcpFrom()));
    connect(connectionFrom, SIGNAL(disconnected()), this, SLOT(OnTcpFromDisconnect()));
    connectionFrom->write(CreateHSHPacket());
    LogMessage("Accepted TCP connection from (" + IP + ")");

    int numberOfPeers = ((*packet)[2]).toInt();
    for(int i =3 ; i< (numberOfPeers+3);i+=2)
    {
        nickIpPairList->insert(NickIpPair(QHostAddress((*packet)[i]), QString((*packet)[i+1])));

    }

}

void ConnectionManager::OnHSHPacket(QStringList *packet)
{
    outgoingTcp->disconnect();
    connectionTo = outgoingTcp;
    outgoingTcp = nullptr;
    connect(connectionTo, SIGNAL(connected()), this, SLOT(OnHandshakeOutgoing()));
    connect(connectionTo, SIGNAL(readyRead()), this, SLOT(OnReadTcpTo()));
    connect(connectionTo, SIGNAL(disconnected()), this, SLOT(OnTcpToDisconnect()));
    LogMessage("Advertizer accepted the connection.");
    //connectionTo->write(CreateUNNPacket());

}

QByteArray ConnectionManager::CreateADZPacket(QString nickname, QString hostAddress)
{
    QByteArray packet;
    packet.append("LANchat");
    char dummy[2];
    _itoa(ADZ, dummy, 10);
    packet.append(QString(dummy) + "\r\n");
    packet.append(nickname + "\r\n");
    packet.append(hostAddress + "\r\n");
    return packet;
}

QByteArray ConnectionManager::CreateNNCPacket(QString nickname, QString hostAddress)
{
    QByteArray packet;
    packet.append("LANchat");
    char dummy[2];
    _itoa(NNC, dummy, 10);
    packet.append(QString(dummy) + "\r\n" );
    packet.append(nickname + "\r\n");
    packet.append(hostAddress + "\r\n");
    return packet;
}

QByteArray ConnectionManager::CreateCONPacket(QString hostAddress)
{
    QByteArray packet;
    packet.append("LANchat");
    char dummy[2];
    _itoa(CON, dummy, 10);
    packet.append(QString(dummy) + "\r\n");
    _itoa(nickIpPairList->size()+1, dummy, 10);
    packet.append(QString(dummy) + "\r\n");
    packet.append(hostAddress + "\r\n");

    for(NickIpCIter it = nickIpPairList->cbegin();it != nickIpPairList->cend() ;++it)
    {
        //We add nick/ip pairs of all we know to new peer that we have connected to
        packet.append(it->first.toString() + "\r\n");
        packet.append(it->second + "\r\n");
    }

    return packet;
}

QByteArray ConnectionManager::CreateHSHPacket()
{
    QByteArray packet;
    packet.append("LANchat");
    char dummy[2];
    _itoa(HSH, dummy, 10);
    packet.append(QString(dummy) + "\r\n");
    return packet;
}

QByteArray ConnectionManager::CreateUNNPacket()
{
    QByteArray packet;
    packet.append("LANchat");
    char dummy[2];
    _itoa(UNN, dummy, 10);
    packet.append(QString(dummy) + "\r\n");
    _itoa(nickIpPairList->size(), dummy, 10);
    packet.append(QString(dummy) + "\r\n");
    for(NickIpCIter it = nickIpPairList->cbegin();it != nickIpPairList->cend() ;++it)
    {
        packet.append(it->first.toString() + "\r\n"); //We add nick/ip pairs of all we know to new peer that we have connected to
        packet.append(it->second + "\r\n");
    }
    return packet;
}

QByteArray ConnectionManager::CreateMSGPacket(QString message)
{
    QByteArray packet;
    packet.append("LANchat");
    char dummy[2];
    _itoa(MSG, dummy, 10);
    packet.append(QString(dummy) + "\r\n");
    packet.append(message.toUtf8() + "\r\n");

    return packet;
}

ConnectionManager::PacketTypes ConnectionManager::DisassemblePacket(QByteArray *packet, QStringList *payloads)
{
    int indexOfCRLF, numberOfPayloads;
    int oldIndexOfCRLF = 7;     //Passing "LANchat"
    indexOfCRLF = packet->indexOf("\r\n", oldIndexOfCRLF);
    QString packetType = packet->mid(oldIndexOfCRLF, indexOfCRLF - oldIndexOfCRLF);
    payloads->append(packetType);

    switch(packetType.toInt())
    {
    case ADZ:
        numberOfPayloads=2;
        break;
    case CON:
        oldIndexOfCRLF = indexOfCRLF;
        indexOfCRLF = packet->indexOf("\r\n", oldIndexOfCRLF + 2);
        numberOfPayloads = packet->mid(oldIndexOfCRLF+2,indexOfCRLF-(oldIndexOfCRLF+2)).toInt()*2+1;
        break;
    case HSH:
        numberOfPayloads=0;
        break;
    case NNC:
        numberOfPayloads=1;
        break;
    case UNN:
        oldIndexOfCRLF = indexOfCRLF;
        indexOfCRLF = packet->indexOf("\r\n", oldIndexOfCRLF + 2);
        numberOfPayloads = packet->mid(oldIndexOfCRLF+2,indexOfCRLF-(oldIndexOfCRLF+2)).toInt();
        break;
    case MSG:
        numberOfPayloads=1;

    }

    for(int i=0; i < numberOfPayloads; ++i)
    {
        oldIndexOfCRLF = indexOfCRLF;
        indexOfCRLF = packet->indexOf("\r\n", oldIndexOfCRLF + 2);
        payloads->append(packet->mid(oldIndexOfCRLF+2,indexOfCRLF-(oldIndexOfCRLF+2)));
    }

    return static_cast<PacketTypes>(packetType.toInt());
}

void ConnectionManager::OnTcpIncommingDisconnect()
{
    LogMessage("Failed to establish connection from " + incommingTcp->peerAddress().toString());
    incommingTcp->deleteLater();
    incommingTcp = nullptr;
}

void ConnectionManager::OnTcpOutgoingDisconnect()
{
    LogMessage("Failed to connect to " + outgoingTcp->peerAddress().toString());
    outgoingTcp->deleteLater();
    outgoingTcp = nullptr;
}



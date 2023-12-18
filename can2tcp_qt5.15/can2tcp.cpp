#include "can_to_tcp.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <QDebug>
#include <QTimer>

#define DEBUG 0

CanToTcp::CanToTcp(QObject *parent) : QObject(parent)
{
    // Inizializza il socket TCP
    socket = new QTcpSocket(this);

    // Connessione dei segnali e slot per gestire gli eventi del socket
    connect(socket, &QTcpSocket::connected, this, &CanToTcp::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &CanToTcp::onDisconnected);

    // Avvia il timer per la connessione al server TCP
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CanToTcp::timerCallback);
}

CanToTcp::~CanToTcp()
{
    // Chiudi il socket alla fine
    if (socket->isOpen())
    {
        socket->close();
    }
}

bool CanToTcp::connectToServer(const char *ipAddress, int port)
{
    // Connessione al server TCP
    m_ipAddress = ipAddress;
    m_port = port;
    socket->connectToHost(ipAddress, port);
    if (socket->waitForConnected(5000))
    {
        qDebug() << "Connesso al server TCP";
        return true;
    }
    else
    {
        qDebug() << "Non connesso al server TCP";
        timer->start(5000);
        return false;
    }
    // Memorizza l'indirizzo IP e la porta
    // Avvia il timer per la connessione al server TCP
}

void CanToTcp::onConnected()
{
    qDebug() << "Connesso al server TCP";
    // Puoi aggiungere ulteriori azioni dopo la connessione
}

void CanToTcp::onDisconnected()
{
    qDebug() << "Disconnesso dal server TCP";
    // Puoi aggiungere ulteriori azioni dopo la disconnessione
    timer->start(5000);
}

void CanToTcp::timerCallback()
{
    qWarning() << "Timer callback";
    // qWarning() << "Non connesso al server TCP";
    socket->connectToHost(m_ipAddress, m_port);
}

void CanToTcp::sendCanFrame(const can_frame &frame)
{
    // Invia il frame sulla socket TCP
    // qWarning() << "Invio frame sulla socket TCP";
    if (socket->write((const char *)&frame, sizeof(frame)) == -1)
    {
        //qWarning() << "Errore durante l'invio del frame sulla socket TCP";
    }
    else
    {
        // qWarning() << "Frame inviato sulla socket TCP";
    }
}
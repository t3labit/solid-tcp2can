#ifndef CAN_TO_TCP_H
#define CAN_TO_TCP_H

#include <QTcpSocket>
#include <QTimer>
#include <QObject>
#include <linux/can.h>

class CanToTcp : public QObject {
    Q_OBJECT
private:
    QTcpSocket *socket;
    const char* m_ipAddress;
    int m_port;
    QTimer *timer;

private slots:
    void timerCallback();
    void onConnected();
    void onDisconnected();

public:
    explicit CanToTcp(QObject *parent = nullptr);
    ~CanToTcp();
    void sendCanFrame(const can_frame& frame);
    bool connectToServer(const char* ipAddress, int port);
};

#endif // CAN_TO_TCP_H
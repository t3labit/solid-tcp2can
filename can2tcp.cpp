#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

#define DEBUG 0

class CanToTcp
{
public:
    CanToTcp();
    ~CanToTcp();

    bool connectToServer(const char *ipAddress, int port);
    void sendCanFrame(const can_frame &frame);

private:
    int socket_fd;
    struct sockaddr_in server_address;
    const char *m_ipAddress;
    int m_port;

    void timerCallback();
};

CanToTcp::CanToTcp()
{
    // Inizializza il socket TCP
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        perror("Errore durante la creazione del socket");
        exit(EXIT_FAILURE);
    }

    // Inizializza l'indirizzo del server
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
}

CanToTcp::~CanToTcp()
{
    // Chiudi il socket alla fine
    if (close(socket_fd) == -1)
    {
        perror("Errore durante la chiusura del socket");
        exit(EXIT_FAILURE);
    }
}

bool CanToTcp::connectToServer(const char *ipAddress, int port)
{
    // Connessione al server TCP
    m_ipAddress = ipAddress;
    m_port = port;

    server_address.sin_addr.s_addr = inet_addr(ipAddress);
    server_address.sin_port = htons(port);

    if (connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == 0)
    {
        std::cout << "Connesso al server TCP" << std::endl;
        return true;
    }
    else
    {
        std::cout << "Non connesso al server TCP" << std::endl;
        // Avvia il timer per la connessione al server TCP
        timerCallback();
        return false;
    }
}

void CanToTcp::timerCallback()
{
    // Implementa la gestione del timeout se necessario
}

void CanToTcp::sendCanFrame(const can_frame &frame)
{
    // Invia il frame sulla socket TCP
    if (send(socket_fd, &frame, sizeof(frame), 0) == -1)
    {
        perror("Errore durante l'invio del frame sulla socket TCP");
    }
    else
    {
        // Frame inviato sulla socket TCP
    }
}

int main()
{
    CanToTcp canToTcp;

    const char *ipAddress = "127.0.0.1";
    int port = 1234;

    if (canToTcp.connectToServer(ipAddress, port))
    {
        // Esegui altre azioni dopo la connessione
    }

    return 0;
}

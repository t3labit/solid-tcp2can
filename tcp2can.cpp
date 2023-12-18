#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/can.h>
#include <net/if.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <csignal>

static bool isRunning = true;

class CanTcpServer
{
private:
  int tcpServerSocket;
  int vcanSocket;
  sockaddr_in serverAddress;

public:
  CanTcpServer(int port)
  {
    // Crea l'interfaccia vcan0
    createVcan();

    // Inizializza il socket TCP per il server
    tcpServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcpServerSocket == -1)
    {
      std::cerr << "Errore durante la creazione del socket TCP." << std::endl;
      // Gestisci l'errore come desideri
    }

    // Inizializza l'indirizzo del server
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    // Collega il socket all'indirizzo e alla porta
    if (bind(tcpServerSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
      std::cerr << "Errore durante il binding del socket TCP." << std::endl;
      // Gestisci l'errore come desideri
    }

    // Metti il server in ascolto
    if (listen(tcpServerSocket, 1) < 0)
    {
      std::cerr << "Errore durante l'ascolto del socket TCP." << std::endl;
      // Gestisci l'errore come desideri
    }

    // Inizializza la socket vcan
    vcanSocket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (vcanSocket == -1)
    {
      std::cerr << "Errore durante la creazione della socket vcan." << std::endl;
      // Gestisci l'errore come desideri
    }

    // Collega la socket vcan all'interfaccia vcan0
    sockaddr_can addr;
    ifreq ifr;
    std::strcpy(ifr.ifr_name, "vcan0");
    ioctl(vcanSocket, SIOCGIFINDEX, &ifr);
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(vcanSocket, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
      std::cerr << "Errore durante il binding della socket vcan." << std::endl;
      // Gestisci l'errore come desideri
    }
  }

  ~CanTcpServer()
  {
    // Chiudi i socket alla fine
    close(tcpServerSocket);
    close(vcanSocket);
  }

  // Funzione di gestione del segnale per pulire le risorse
  static void signalHandler(int signal)
  {
    isRunning = false;
  }

  void createVcan()
  {
    // Crea l'interfaccia vcan0
    system("insmod /lib/modules/5.4.3-rt1/kernel/net/can/vcan.ko");
    system("ip link add dev vcan0 type vcan");
    system("ip link set up vcan0");
  }

  void acceptAndHandleConnection()
  {
    // Accetta una connessione TCP
    int clientSocket = accept(tcpServerSocket, NULL, NULL);
    if (clientSocket < 0)
    {
      std::cerr << "Errore durante l'accettazione della connessione." << std::endl;
      // Gestisci l'errore come desideri
    }

    // Gestisci la connessione
    while (isRunning)
    {
      // Ricevi i frame dal client e inviali sulla vcan0
      can_frame frame;
      ssize_t bytesRead = recv(clientSocket, &frame, sizeof(frame), 0);
      if (bytesRead <= 0)
      {
        break;
      }


      // std::cout << "Socketcan inviato!" << std::endl;
      // Invia il frame sulla socket vcan
      sendto(vcanSocket, &frame, sizeof(frame), 0, NULL, 0);
    }

    // Chiudi la socket del client
    close(clientSocket);
  }
};

int main()
{
  // Registra la funzione di gestione del segnale per SIGINT (Ctrl+C)
  signal(SIGINT, CanTcpServer::signalHandler);
  signal(SIGTERM, CanTcpServer::signalHandler);

  // Sostituisci con la porta desiderata
  int serverPort = 3132;
  CanTcpServer canTcpServer(serverPort);

  std::cout << "In attesa di connessioni TCP..." << std::endl;

  // Accetta e gestisci connessioni finché il programma è in esecuzione
  while (isRunning)
  {
    canTcpServer.acceptAndHandleConnection();
  }

  return 0;
}

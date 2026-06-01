#include "Server.hpp"
#include "SocketUtils.hpp"
#include <iostream>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/**
 * @brief Constructor del servidor.
 *
 * Inicializa el puerto y la contraseña del servidor, y prepara el
 * descriptor `serverSocket` con valor -1 (no creado aún).
 *
 * @param port Puerto en el que el servidor escuchará conexiones.
 * @param password Contraseña requerida para autenticación (puede estar vacía).
 */
Server::Server(unsigned int port, std::string password): port(port), password(password), services(this)
{
    this->serverSocket = -1;
    char buf[256];
    if (gethostname(buf, sizeof(buf)) != 0)
        this->hostname = std::string("localhost");
    else
        this->hostname = std::string(buf);
    std::cout << "Server created" << std::endl;
}

/**
 * @brief Destructor del servidor: cierra sockets y libera recursos.
 *
 * - Cierra todos los sockets cliente almacenados en `fds` (a partir de 1).
 * - Cierra `serverSocket` si está abierto.
 * - Libera memoria de los objetos `User` y `Channel` restantes.
 */
Server::~Server()
{
    // Process client disconnections so other clients receive QUIT messages
    while (this->fds.size() > 1) {
        // always disconnect the first client entry (index 1)
        this->handleDisconnectionByIndex(1);
    }
    if (this->serverSocket >= 0)
        close(this->serverSocket);
    // clear remaining users and channels via managers
    this->services.users().clear();
    this->services.channels().clear();
    std::cout << "Server destroyed" << std::endl;
}
/** @brief Crea el socket de escucha del servidor. */
void Server::createServerSocket()
{
    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serverSocket < 0)
        throw IrcException(IRC_ERR_STARTING_SERVER, std::string("socket failed: ") + strerror(errno));
    setSocketNonBlocking(this->serverSocket);
    setSocketCloexec(this->serverSocket);
}

void Server::setSocketOptions()
{
    int opt = 1;
    if (setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        throw IrcException(IRC_ERR_STARTING_SERVER, std::string("setsockopt failed: ") + strerror(errno));
    std::cout << "Socket options set" << std::endl;
}

void Server::bindServerSocket()
{
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(this->port);

    if (bind(this->serverSocket, (struct sockaddr*)&address, sizeof(address)) < 0)
        throw IrcException(IRC_ERR_STARTING_SERVER, std::string("bind failed: ") + strerror(errno));
    std::cout << "Bind successful" << std::endl;
}

void Server::listenServerSocket()
{
    if (listen(this->serverSocket, 64) < 0)
        throw IrcException(IRC_ERR_STARTING_SERVER, std::string("listen failed: ") + strerror(errno));
    std::cout << "Server is listening on port " << this->port << "..." << std::endl;
}

void Server::setupPollFds()
{
    this->fds.clear();
    pushPollFd(this->serverSocket, POLLIN);
}

void Server::pushPollFd(int fd, short events)
{
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = events;
    pfd.revents = 0;
    this->fds.push_back(pfd);
}

void Server::startServer()
{
    try {
        createServerSocket();
        setSocketOptions();
        bindServerSocket();
        listenServerSocket();
        setupPollFds();
    }
    catch (const std::exception &e)
    {
        if(this->serverSocket >= 0) {
            close(this->serverSocket);
            this->serverSocket = -1;
        }
        std::cerr << "ERROR: " << e.what() << std::endl;
        return;
    }
    // this->services.channels().createChannel("#general", nullptr);
    // Channel* general = this->services.channels().getChannel("#general");
    // general->setTopic("Welcome to the general channel!");
    // general->setIsInviteOnly(false);
    // general->setKey("1234");
    // general->setKeyRequired(true);
    handleEvents();
}

void Server::stopServer()
{
    if (this->serverSocket >= 0) {
        close(this->serverSocket);
        this->serverSocket = -1;
    }
    std::cout << "Server stopped" << std::endl;
}

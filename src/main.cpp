#include <iostream>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Server.hpp"

int main(int argc, char* argv[]) {
    
    if(argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" <<std::endl;
        return 1;
    }
    if(std::atoi(argv[1]) <= 0 || std::atoi(argv[1]) > 65535) {
        std::cerr << "Invalid port number. Please provide a port number between 1 and 65535." << std::endl;
        return 1;
    }
    Server server(std::atoi(argv[1]), argv[2]);

    server.startServer();

    return 0;
}
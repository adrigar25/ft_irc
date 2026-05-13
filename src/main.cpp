#include <iostream>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Server.hpp"

static std::string exception_message(std::exception_ptr ep) {
        try {
                if (ep) std::rethrow_exception(ep);
        } catch (const std::exception &e) { return e.what(); }
            catch (const std::string &s) { return s; }
            catch (const char *s) { return std::string(s); }
            catch (...) { return "Unknown exception"; }
        return {};
}

static bool checkArgs(int argc, char* argv[]) {
    if(argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" <<std::endl;
        return false;
    }
    if(std::atoi(argv[1]) <= 0 || std::atoi(argv[1]) > 65535) {
        std::cerr << "Invalid port number. Please provide a port number between 1 and 65535." << std::endl;
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    
    if (!checkArgs(argc, argv))
        return 1;
    try {
        Server server(std::atoi(argv[1]), argv[2]);
        server.startServer();
    } catch (...) {
        std::cerr << "Fatal: " << exception_message(std::current_exception()) << std::endl;
        return 1;
    }
    return 0;
}
#include <iostream>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Server.hpp"

/**
 * @brief Valida los argumentos de línea de comandos esperados.
 *
 * Comprueba que se pasen exactamente 2 argumentos (puerto y contraseña)
 * y que el puerto esté en el rango válido.
 * @return `true` si los argumentos son válidos.
 */
/**
 * @brief Valida los argumentos de línea de comandos esperados.
 *
 * Comprueba que se pasen exactamente 2 argumentos (puerto y contraseña)
 * y que el puerto esté en el rango válido.
 * @param argc número de argumentos
 * @param argv vector de argumentos
 * @return `true` si los argumentos son válidos.
 */
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


/**
 * @brief Punto de entrada de la aplicación.
 *
 * Valida argumentos, crea el `Server` y arranca el loop principal. Captura
 * excepciones fatales y muestra un mensaje antes de terminar.
 */
int main(int argc, char* argv[]) {
    
    if (!checkArgs(argc, argv))
        return 1;
    try {
        Server server(std::atoi(argv[1]), argv[2]);
        server.startServer();
    } catch (const std::exception &e) {
        std::cerr << "Fatal: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Fatal: Unknown exception" << std::endl;
        return 1;
    }
    return 0;
}

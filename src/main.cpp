#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }
    unsigned int port = std::stoi(argv[1]);
    std::string password = argv[2];
    Server server(port, password);
    User *user1 = new User("Alice");
    User *user2 = new User("Bob");  
    server.addUser(user1);
    server.addUser(user2);
    server.createChannel("#general", user1);
    Channel *channel = server.getChannel("#general");
    if (channel)
    {
        user2->joinChannel(channel);
        channel->addUser(user2);
        channel->changeRole(user2, "voice");
    }
    delete user1;
    delete user2;
    return 0;
}

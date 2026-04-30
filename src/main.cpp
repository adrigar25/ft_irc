#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include <iostream>

int main()
{
    Server server(6667, "password");
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

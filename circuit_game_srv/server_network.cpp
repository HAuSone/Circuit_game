#include "server_network.hpp"
#include <iostream>

bool ServerNetwork::start(unsigned short port)
{
    if (listener.listen(port) != sf::Socket::Done)
        return false;

    std::cout << "Serveur en attente sur le port " << port << "...\n";

    // Ici on utilise 'client'
    if (listener.accept(client) != sf::Socket::Done)
        return false;

    client.setBlocking(false);
    connected = true;

    std::cout << "Client connecté\n";
    return true;
}

void ServerNetwork::update()
{
    if (!connected)
        return;

    char buffer[128];
    std::size_t received;

    // Ici on utilise 'client'
    if (client.receive(buffer, sizeof(buffer), received) == sf::Socket::Done)
    {
        lastCommand = std::string(buffer, received);
    }
}

std::string ServerNetwork::consumeCommand()
{
    std::string cmd = lastCommand;
    lastCommand.clear();
    return cmd;
}

void ServerNetwork::sendToClient(const std::string& message) {
    if (!message.empty() && connected) {

        if (client.send(message.c_str(), message.size()) != sf::Socket::Done) {
            // Erreur d'envoi
        }
    }
}
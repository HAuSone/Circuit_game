#include "client_network.hpp"
#include <iostream>

ClientNetwork::ClientNetwork() {
    // Constructeur
}

bool ClientNetwork::connect(const std::string& ip, unsigned short port) {
    if (socket.connect(ip, port) != sf::Socket::Done) {
        return false;
    }
    socket.setBlocking(false); // Mode non-bloquant pour ne pas figer le jeu
    return true;
}

void ClientNetwork::send(const std::string& message) {
    if (message.empty()) return;
    socket.send(message.c_str(), message.size());
}

std::string ClientNetwork::receive() {
    char buffer[256];
    std::size_t received;
    
    if (socket.receive(buffer, sizeof(buffer), received) == sf::Socket::Done) {
        return std::string(buffer, received);
    }
    return "";
}
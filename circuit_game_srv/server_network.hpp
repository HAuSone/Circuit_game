#ifndef SERVER_NETWORK_HPP
#define SERVER_NETWORK_HPP

#include <SFML/Network.hpp>
#include <string>

class ServerNetwork {
public:
    ServerNetwork() : connected(false) {} // Constructeur simple
    bool start(unsigned short port);
    void update();
    std::string consumeCommand();
    void sendToClient(const std::string& message);

private:
    sf::TcpListener listener;
    sf::TcpSocket client;
    bool connected;         // Pour savoir si le client est là
    std::string lastCommand; // Pour stocker la dernière touche reçue
};

#endif
#ifndef CLIENT_NETWORK_HPP
#define CLIENT_NETWORK_HPP

#include <SFML/Network.hpp>
#include <string>

class ClientNetwork {
public:
    ClientNetwork();
    
    // Se connecte au serveur (ex: "127.0.0.1", 54000)
    bool connect(const std::string& ip, unsigned short port);
    
    // Envoie une commande (ex: "LEFT")
    void send(const std::string& message);
    
    // Reçoit les données de réplication (ex: "UPDATE ...")
    std::string receive();

private:
    sf::TcpSocket socket;
};

#endif
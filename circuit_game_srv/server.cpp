#include <SFML/Network.hpp>
#include <iostream>
#include <string>

int main()
{
    sf::TcpListener listener;
    sf::TcpSocket client;

    const unsigned short PORT = 54000;

    if (listener.listen(PORT) != sf::Socket::Done)
    {
        std::cerr << "Erreur : impossible d'ouvrir le port\n";
        return -1;
    }

    std::cout << "Serveur en attente sur le port " << PORT << "...\n";

    if (listener.accept(client) != sf::Socket::Done)
    {
        std::cerr << "Erreur : connexion echouee\n";
        return -1;
    }

    std::cout << "Client connecte !\n";

    char buffer[128];
    std::size_t received;

    while (true)
    {
        sf::Socket::Status status = client.receive(buffer, sizeof(buffer), received);

        if (status == sf::Socket::Done)
        {
            std::string command(buffer, received);
            std::cout << "Commande recue : " << command << std::endl;

            // Plus tard :
            // ici tu pourras appeler vehicule.accelerate(), stop(), etc.
        }
        else if (status == sf::Socket::Disconnected)
        {
            std::cout << "Client deconnecte.\n";
            break;
        }
    }

    return 0;
}

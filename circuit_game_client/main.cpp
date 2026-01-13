#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <vector>

#include "Circuit.hpp"
#include "client_network.hpp"
#include "Tire.hpp"

using namespace std;

// --- HUD : AFFICHAGE DES DONNÉES ---
void afficherInterfaceClient(sf::RenderWindow& window, sf::Font& font, float vitesse, double pos, float essence, double distE, double distP, const vector<Tire>& pneusLocaux) {
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(1);

    stringstream ss;
    ss << fixed << setprecision(1);
    ss << "Vitesse: " << (vitesse / 10.0f) << " km/h\n";
    ss << "Position: " << setprecision(3) << pos << " / 100 km\n";
    ss << "----------------------------------------\n";


    ss << setprecision(1);
    ss << "Usure AVG: " << (pneusLocaux[0].getUsure() * 100.0f) << "%  "
       << "AVD: " << (pneusLocaux[1].getUsure() * 100.0f) << "%\n";
    ss << "Usure ARG: " << (pneusLocaux[2].getUsure() * 100.0f) << "%  "
       << "ARD: " << (pneusLocaux[3].getUsure() * 100.0f) << "%\n";
    ss << "----------------------------------------\n";
    ss << "Fleches: Accel/Voies | Bas: ralentir | Ctrl D: stop";

    text.setString(ss.str());
    text.setPosition(10, 10);
    window.draw(text);

    // Jauge essence
    sf::RectangleShape contour(sf::Vector2f(200, 20));
    contour.setPosition(550, 20);
    contour.setOutlineColor(sf::Color::White);
    contour.setOutlineThickness(2);
    contour.setFillColor(sf::Color::Transparent);
    window.draw(contour);

    float ratio = essence / 60.0f;
    sf::RectangleShape jauge(sf::Vector2f(max(0.f, 200.f * ratio), 20));
    jauge.setPosition(550, 20);
    jauge.setFillColor(ratio > 0.2f ? sf::Color::Green : sf::Color::Red);
    window.draw(jauge);

    // Radar
    sf::RectangleShape cadreRadar(sf::Vector2f(220, 80));
    cadreRadar.setPosition(550, 60);
    cadreRadar.setFillColor(sf::Color(20, 20, 20, 200));
    cadreRadar.setOutlineColor(sf::Color::Yellow);
    cadreRadar.setOutlineThickness(2);
    window.draw(cadreRadar);

    stringstream ssR;
    ssR << "   RADAR STATIONS\n";
    ssR << "ESSENCE : " << (distE > 0 ? to_string((int)(distE * 1000)) + " m" : "---") << "\n";
    ssR << "PNEUS   : " << (distP > 0 ? to_string((int)(distP * 1000)) + " m" : "---");

    sf::Text radar(ssR.str(), font, 16);
    radar.setPosition(560, 65);
    radar.setFillColor(sf::Color::Cyan);
    window.draw(radar);
}

void afficherMenuClient(sf::RenderWindow& window, sf::Font& font, bool p1Pret, bool p2Pret) {
    window.clear(sf::Color(30, 30, 30));
    sf::Text title("SELECTION DES VEHICULES", font, 30);
    title.setPosition(210, 20);
    title.setFillColor(sf::Color::Yellow);
    window.draw(title);

    sf::Text s1(p1Pret ? "P1: PRET" : "P1: CHOISIT...", font, 20);
    s1.setPosition(100, 75);
    s1.setFillColor(p1Pret ? sf::Color::Green : sf::Color::Red);
    window.draw(s1);

    sf::Text s2(p2Pret ? "P2: PRET" : "P2: CHOISIT...", font, 20);
    s2.setPosition(550, 75);
    s2.setFillColor(p2Pret ? sf::Color::Green : sf::Color::Red);
    window.draw(s2);

    string noms[] = { "1. SPORTIF", "2. LOURD", "3. PUISSANT", "4. ECO", "5. EQUILIBRE" };
    for (int i = 0; i < 5; i++) {
        sf::RectangleShape btn(sf::Vector2f(600, 60));
        btn.setPosition(100, 120 + i * 85);
        btn.setFillColor(sf::Color(40, 40, 100));
        btn.setOutlineThickness(2);
        btn.setOutlineColor(sf::Color::White);
        window.draw(btn);
        sf::Text t(noms[i], font, 22);
        t.setPosition(120, 130 + i * 85);
        window.draw(t);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "C++ Racing Client (JOUEUR 2)");
    window.setFramerateLimit(60);

    ClientNetwork clientNetwork;
    if (!clientNetwork.connect("127.0.0.1", 54000)) return -1;

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) return -1;

    Circuit circuit(100.0);
    sf::RectangleShape carShape(sf::Vector2f(40, 70));
    carShape.setOrigin(20, 35);

    vector<Tire> pneusLocaux = { Tire("AVG"), Tire("AVD"), Tire("ARG"), Tire("ARD") };
    sf::Clock localClock;
    double masseActuelle = 900.0;
    double masses[] = { 680.0, 1227.0, 900.0, 500.0, 1100.0 };

    double posP1 = 0.0, posP2 = 0.0, distE = 0.0, distP = 0.0;
    float posX1 = 300.f, posX2 = 500.f, vitesseP2 = 0.f, essence = 60.f;
    bool auMenu = true, p1Pret = false, p2Pret = false;
    int etatFinRecu = 0;

    while (window.isOpen()) {
        sf::Time dt_frame = localClock.restart();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (auMenu) {
                    int choix = -1;
                    if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num5)
                        choix = (event.key.code - sf::Keyboard::Num1) + 1;
                    else if (event.key.code >= sf::Keyboard::Numpad1 && event.key.code <= sf::Keyboard::Numpad5)
                        choix = (event.key.code - sf::Keyboard::Numpad1) + 1;

                    if (choix != -1) {
                        masseActuelle = masses[choix - 1];
                        clientNetwork.send(to_string(choix));
                    }
                }
                else if (etatFinRecu == 0) {
                    if (event.key.code == sf::Keyboard::Up)       clientNetwork.send("P2_ACCEL");
                    if (event.key.code == sf::Keyboard::Down)     clientNetwork.send("P2_DECEL");
                    if (event.key.code == sf::Keyboard::RControl) clientNetwork.send("P2_START");
                    if (event.key.code == sf::Keyboard::Left)     clientNetwork.send("P2_LEFT");
                    if (event.key.code == sf::Keyboard::Right)    clientNetwork.send("P2_RIGHT");
                }
            }
        }

        // --- CALCUL LOCAL DE L'USURE ---
        if (!auMenu && etatFinRecu == 0) {
            double dt_hour = dt_frame.asSeconds() / 3600.0;
            for (auto& pneu : pneusLocaux) {
                pneu.updateUsure(masseActuelle * 0.005, vitesseP2 * dt_hour);
            }
        }

        string data = clientNetwork.receive();
        if (!data.empty()) {
            stringstream ss(data);
            string header;
            ss >> header;

            if (header == "MENU_OPEN") {
                auMenu = true;
                ss >> p1Pret >> p2Pret;
                // Au menu, on reset l'usure pour la prochaine partie
                for(auto& p : pneusLocaux) p.resetUsure();
            }
            else if (header == "UPDATE") {
                auMenu = false;
                // Lecture synchronisée avec le serveur (9 variables)
                ss >> posP1 >> posP2 >> posX1 >> posX2 >> vitesseP2 >> essence >> distE >> distP >> etatFinRecu;

                // Reset usure si on est sur la station
                if (distP < 0.0005) {
                    for(auto& p : pneusLocaux) p.resetUsure();
                }
            }
        }

        window.clear();
        if (auMenu) {
            afficherMenuClient(window, font, p1Pret, p2Pret);
        } else {
            window.clear(sf::Color(34, 139, 34));
            circuit.dessiner(window, posP2);

            carShape.setFillColor(sf::Color::Red);
            float screenY_P1 = 500.f - (float)((posP1 - posP2) * 5000.0);
            if (screenY_P1 > -100 && screenY_P1 < 800) {
                carShape.setPosition(posX1, screenY_P1);
                window.draw(carShape);
            }

            carShape.setFillColor(sf::Color::Blue);
            carShape.setPosition(posX2, 500.f);
            window.draw(carShape);

            afficherInterfaceClient(window, font, vitesseP2, posP2, essence, distE, distP, pneusLocaux);

            // --- ÉCRAN DE FIN DE PARTIE (MÊMES CONDITIONS QUE SERVEUR) ---
            if (etatFinRecu != 0) {
                sf::RectangleShape overlay(sf::Vector2f(800, 600));
                overlay.setFillColor(sf::Color(0, 0, 0, 210));
                window.draw(overlay);

                string msg = "FIN DE COURSE";
                if (etatFinRecu == 1) msg = "VICTOIRE ATTEINTE !";
                else if (etatFinRecu == 2) msg = "GAME OVER : P1 S'EST CRASHE !";
                else if (etatFinRecu == 3) msg = "GAME OVER : P2 S'EST CRASHE !";
                else if (etatFinRecu == 4) msg = "PANNE ESSENCE P1 !";
                else if (etatFinRecu == 5) msg = "PANNE ESSENCE P2 !";
                else if (etatFinRecu == 6) msg = "P1 A DISTANCE P2 !";
                else if (etatFinRecu == 7) msg = "P2 A DISTANCE P1 !";
                else if (etatFinRecu == 8) msg = "GAME OVER : COLLISION !";

                sf::Text fin(msg + "\n\nATTENTE DU SERVEUR...", font, 30);
                fin.setOrigin(fin.getLocalBounds().width/2, fin.getLocalBounds().height/2);
                fin.setPosition(400, 300);
                fin.setFillColor(sf::Color::White);
                window.draw(fin);
            }
        }
        window.display();
    }
    return 0;
}
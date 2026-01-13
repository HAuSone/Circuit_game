#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "server_network.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <iomanip>

#include "Moteur.hpp"
#include "Tank.hpp"
#include "Tire.hpp"
#include "Vehicule.hpp"
#include "Circuit.hpp"

using namespace std;

// --- FONCTIONS UTILITAIRES ---

string toString(double value, int precision = 2) {
    stringstream ss;
    ss.precision(precision);
    ss << fixed << value;
    return ss.str();
}

sf::RectangleShape creerBouton(float x, float y, sf::Color color) {
    sf::RectangleShape rect(sf::Vector2f(600, 60));
    rect.setPosition(x, y);
    rect.setFillColor(color);
    rect.setOutlineThickness(2);
    rect.setOutlineColor(sf::Color::White);
    return rect;
}
//initialisation de l'affichage SFML
void afficherEtatSFML(sf::RenderWindow& window, const Vehicule& vehicule, double position, sf::Font& font, Circuit& circuit) {
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(1);

    stringstream ss;
    const vector<Tire>& pneus = vehicule.getTires();
    ss << "Vitesse: " << toString(vehicule.getCurrentSpeed() / 10, 1) << " km/h\n";
    ss << "Position: " << toString(position, 3) << " / 100 km\n";
    ss << "----------------------------------------\n";
    ss << "Usure AVG: " << toString(pneus[0].getUsure() * 100, 1) << "%  "
   << "AVD: " << toString(pneus[1].getUsure() * 100, 1) << "%\n";
    ss << "Usure ARG: " << toString(pneus[2].getUsure() * 100, 1) << "%  "
       << "ARD: " << toString(pneus[3].getUsure() * 100, 1) << "%\n";
    ss << "----------------------------------------\n";
    ss << "P1: Z/S (Accel/Frein) | Q/D (Voies)\n";
    ss << "P1: A (Start) | X (Stop)";

    text.setString(ss.str());
    text.setPosition(10.f, 10.f);
    window.draw(text);

    // --- JAUGE ESSENCE ---
    float ratio = vehicule.getTank().getCapacite() / 60.0f;
    sf::RectangleShape contour(sf::Vector2f(200, 20));
    contour.setPosition(550, 20);
    contour.setFillColor(sf::Color::Transparent);
    contour.setOutlineColor(sf::Color::White);
    contour.setOutlineThickness(2);
    window.draw(contour);

    sf::RectangleShape jauge(sf::Vector2f(max(0.f, 200.f * ratio), 20));
    jauge.setPosition(550, 20);
    jauge.setFillColor(ratio > 0.2f ? sf::Color::Green : sf::Color::Red);
    window.draw(jauge);

    // --- RADAR STATIONS ---
    double distEssence = circuit.getDistanceNextStation(position);
    double distPneu = circuit.getDistanceNextPneu(position);

    sf::RectangleShape cadreRadar(sf::Vector2f(220, 80));
    cadreRadar.setPosition(550, 60);
    cadreRadar.setFillColor(sf::Color(20, 20, 20, 200));
    cadreRadar.setOutlineColor(sf::Color::Yellow);
    cadreRadar.setOutlineThickness(2);
    window.draw(cadreRadar);

    stringstream ssR;
    ssR << "   RADAR STATIONS\n";
    ssR << "ESSENCE : " << (distEssence > 0 ? to_string((int)(distEssence * 1000)) + " m" : "---") << "\n";
    ssR << "PNEUS   : " << (distPneu > 0 ? to_string((int)(distPneu * 1000)) + " m" : "---");

    sf::Text radar;
    radar.setFont(font);
    radar.setCharacterSize(16);
    radar.setString(ssR.str());
    radar.setPosition(560, 65);
    radar.setFillColor(sf::Color::Cyan);
    window.draw(radar);
}

// --- MAIN ---

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "C++ Racing Simulator (SERVER - P1)");
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    ServerNetwork server;
    if (!server.start(54000)) return -1;

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) return -1;

    bool sessionActive = true;

    while (sessionActive && window.isOpen()) {

        // Données Moteurs
        Moteur moteurs[] = {
            Moteur(8.8, 680, 250, 2200),
            Moteur(6.0, 1227, 250, 2000),
            Moteur(15.0, 900, 300, 3000),
            Moteur(4.5, 500, 150, 1800),
            Moteur(10.0, 1100, 200, 2400)
        };
        string noms[] = { "1. SPORTIF", "2. LOURD", "3. PUISSANT", "4. ECO", "5. EQUILIBRE" };

        Moteur moteurP1, moteurP2;
        bool p1Pret = false, p2Pret = false;
        bool gameRunning = false;
        string reasonGameOver = "";

        // --- BOUCLE MENU ---
        while (window.isOpen() && (!p1Pret || !p2Pret)) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) { window.close(); sessionActive = false; }
                if (!p1Pret && event.type == sf::Event::KeyPressed) {
                    int idx = -1;
                    if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num5) idx = event.key.code - sf::Keyboard::Num1;
                    if (event.key.code >= sf::Keyboard::Numpad1 && event.key.code <= sf::Keyboard::Numpad5) idx = event.key.code - sf::Keyboard::Numpad1;
                    if (idx != -1) { moteurP1 = moteurs[idx]; p1Pret = true; }
                }
            }

            server.update();
            server.sendToClient("MENU_OPEN " + to_string(p1Pret) + " " + to_string(p2Pret));

            string cmd = server.consumeCommand();
            if (!p2Pret && !cmd.empty() && isdigit(cmd[0])) {
                int idx = cmd[0] - '1';
                if (idx >= 0 && idx < 5) { moteurP2 = moteurs[idx]; p2Pret = true; }
            }

            window.clear(sf::Color(30, 30, 30));
            sf::Text title("SELECTION DES VEHICULES", font, 30);
            title.setPosition(210, 20); title.setFillColor(sf::Color::Yellow);
            window.draw(title);

            sf::Text s1(p1Pret ? "P1: PRET" : "P1: CHOISIT...", font, 20);
            s1.setPosition(100, 75); s1.setFillColor(p1Pret ? sf::Color::Green : sf::Color::Red);
            window.draw(s1);

            sf::Text s2(p2Pret ? "P2: PRET" : "P2: CHOISIT...", font, 20);
            s2.setPosition(550, 75); s2.setFillColor(p2Pret ? sf::Color::Green : sf::Color::Red);
            window.draw(s2);

            for (int i = 0; i < 5; i++) {
                sf::RectangleShape btn = creerBouton(100, 120 + i * 85, sf::Color(40, 40, 100));
                window.draw(btn);
                sf::Text t(noms[i], font, 22);
                t.setPosition(120, 130 + i * 85); window.draw(t);
            }
            window.display();
        }

        // --- INITIALISATION JEU ---
        Tank tankP1(60.0, 5.0), tankP2(60.0, 5.0);
        vector<Tire> pneusP1 = { Tire("AVG"), Tire("AVD"), Tire("ARG"), Tire("ARD") };
        vector<Tire> pneusP2 = { Tire("AVG"), Tire("AVD"), Tire("ARG"), Tire("ARD") };
        Vehicule vehicule1(moteurP1, tankP1, pneusP1, moteurP1.getPoids());
        Vehicule vehicule2(moteurP2, tankP2, pneusP2, moteurP2.getPoids());

        float carPosX1 = 300.f, carPosX2 = 500.f;
        double positionP1 = 0.0, positionP2 = 0.0;
        gameRunning = true;
        sf::Clock clock, timerP2;
        Circuit circuit(100.0);
        sf::RectangleShape carShape(sf::Vector2f(40, 70));
        carShape.setOrigin(20, 35); carShape.setOutlineThickness(2);

        server.sendToClient("START_GAME");

        // --- BOUCLE DE COURSE ---
        while (window.isOpen()) {
            sf::Time elapsed = clock.restart();
            double dt_hour = elapsed.asSeconds() / 3600.0;

            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) { window.close(); sessionActive = false; }

                // Contrôles Serveur (P1)
                if (gameRunning && event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Z) vehicule1.accelerate(100);
                    if (event.key.code == sf::Keyboard::S) vehicule1.decelerate(100);
                    if (event.key.code == sf::Keyboard::Q && carPosX1 > 250) carPosX1 -= 100.f;
                    if (event.key.code == sf::Keyboard::D && carPosX1 < 550) carPosX1 += 100.f;
                    if (event.key.code == sf::Keyboard::A) vehicule1.start();
                    if (event.key.code == sf::Keyboard::X) vehicule1.stop();
                }

                // Touche de reset (Uniquement Serveur)
                if (!gameRunning && event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::R) goto reset_partie;
                    if (event.key.code == sf::Keyboard::Escape) { window.close(); sessionActive = false; }
                }
            }

            if (gameRunning) {
                server.update();
                string cmd = server.consumeCommand();
                if (!cmd.empty()) {
                    if (cmd == "P2_ACCEL") vehicule2.accelerate(100);
                    else if (cmd == "P2_DECEL") vehicule2.decelerate(100);
                    else if (cmd == "P2_START") vehicule2.start();
                    else if (timerP2.getElapsedTime().asSeconds() > 0.15f) {
                        if (cmd == "P2_LEFT" && carPosX2 > 250) { carPosX2 -= 100.f; timerP2.restart(); }
                        else if (cmd == "P2_RIGHT" && carPosX2 < 550) { carPosX2 += 100.f; timerP2.restart(); }
                    }
                }

                positionP1 += vehicule1.getCurrentSpeed() * dt_hour;
                positionP2 += vehicule2.getCurrentSpeed() * dt_hour;
                vehicule1.updateUsurePneus(dt_hour); vehicule1.updateConsommation(dt_hour);
                vehicule2.updateUsurePneus(dt_hour); vehicule2.updateConsommation(dt_hour);

                // Conditions de Fin
                float diffX = abs(carPosX1 - carPosX2);
                float diffY_px = abs((positionP1 - positionP2) * 5000.0f);

                if (diffX < 40.0f && diffY_px < 70.0f) { gameRunning = false; reasonGameOver = "GAME OVER : COLLISION !"; }
                else if (positionP1 >= 100.0 || positionP2 >= 100.0) { gameRunning = false; reasonGameOver = "VICTOIRE ATTEINTE !"; }
                else if ((carPosX1 < 250 || carPosX1 > 550) && vehicule1.getCurrentSpeed() > 10.0) { gameRunning = false; reasonGameOver = "GAME OVER : P1 S'EST CRASHE !"; }
                else if ((carPosX2 < 250 || carPosX2 > 550) && vehicule2.getCurrentSpeed() > 10.0) { gameRunning = false; reasonGameOver = "GAME OVER : P2 S'EST CRASHE !"; }
                else if (vehicule1.getTank().getCapacite() <= 0 && vehicule1.getCurrentSpeed() < 0.1) { gameRunning = false; reasonGameOver = "PANNE ESSENCE P1 !"; }
                else if (vehicule2.getTank().getCapacite() <= 0 && vehicule2.getCurrentSpeed() < 0.1) { gameRunning = false; reasonGameOver = "PANNE ESSENCE P2 !"; }
                else if (abs(positionP1 - positionP2) > 3.0) { gameRunning = false; reasonGameOver = (positionP1 > positionP2) ? "P1 A DISTANCE P2 !" : "P2 A DISTANCE P1 !"; }
            }

            // Envoi Réseau
            int etatFin = 0;
            if (!gameRunning) {
                if (reasonGameOver == "VICTOIRE ATTEINTE !") etatFin = 1;
                else if (reasonGameOver == "GAME OVER : P1 S'EST CRASHE !") etatFin = 2;
                else if (reasonGameOver == "GAME OVER : P2 S'EST CRASHE !") etatFin = 3;
                else if (reasonGameOver == "PANNE ESSENCE P1 !") etatFin = 4;
                else if (reasonGameOver == "PANNE ESSENCE P2 !") etatFin = 5;
                else if (reasonGameOver == "P1 A DISTANCE P2 !") etatFin = 6;
                else if (reasonGameOver == "P2 A DISTANCE P1 !") etatFin = 7;
                else if (reasonGameOver == "GAME OVER : COLLISION !") etatFin = 8;
            }

            stringstream ss;

            ss << "UPDATE " << fixed << setprecision(6);

            ss << positionP1 << " " << positionP2 << " "
               << carPosX1 << " " << carPosX2 << " "
               << vehicule2.getCurrentSpeed() << " "
               << vehicule2.getTank().getCapacite() << " "
               << circuit.getDistanceNextStation(positionP2) << " "
               << circuit.getDistanceNextPneu(positionP2) << " "
               << etatFin;

            server.sendToClient(ss.str());
            server.sendToClient(ss.str());

            // Dessin
            window.clear(sf::Color(34, 139, 34));
            circuit.dessiner(window, positionP1);

            carShape.setFillColor(sf::Color::Red); carShape.setPosition(carPosX1, 500.f); window.draw(carShape);
            float sY2 = 500.f - (float)((positionP2 - positionP1) * 5000.0);
            if (sY2 > -100 && sY2 < 800) { carShape.setFillColor(sf::Color::Blue); carShape.setPosition(carPosX2, sY2); window.draw(carShape); }

            afficherEtatSFML(window, vehicule1, positionP1, font, circuit);

            if (!gameRunning) {
                sf::RectangleShape overlay(sf::Vector2f(800, 600)); overlay.setFillColor(sf::Color(0, 0, 0, 200)); window.draw(overlay);
                sf::Text fin(reasonGameOver + "\n\n[R] REJOUER\n[ESC] QUITTER", font, 30);
                fin.setOrigin(fin.getLocalBounds().width/2, fin.getLocalBounds().height/2);
                fin.setPosition(400, 300); fin.setFillColor(sf::Color::White); window.draw(fin);
            }
            window.display();
        }
        reset_partie:; // Point de saut pour relancer la session
    }
    return 0;
}
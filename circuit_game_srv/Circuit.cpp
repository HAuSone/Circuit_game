#include "Circuit.hpp"
#include <cstdlib>
#include <cmath>

Circuit::Circuit(double longTotal) : longueur(longTotal), largeurVoie(100.0) {
    for (double i = 0; i < longueur; i += 0.5) {
        Segment s;
        s.positionY = i;
        int r = rand() % 45;
        s.gauche = (r == 4) ? TypeBordure::STATION_SERVICE : (r == 2 ? TypeBordure::CENTRE_PNEU : TypeBordure::VIDE);
        s.droite = (r == 1) ? TypeBordure::STATION_SERVICE : (r == 3 ? TypeBordure::CENTRE_PNEU : TypeBordure::VIDE);
        segments.push_back(s);
    }
}

bool Circuit::isCentrePneuAt(double posKM) {
    for (const auto& s : segments) {
        if (std::abs(s.positionY - posKM) < 0.1) {
            if (s.gauche == TypeBordure::CENTRE_PNEU || s.droite == TypeBordure::CENTRE_PNEU) {
                return true;
            }
        }
    }
    return false;
}

bool Circuit::isStationAt(double posKM) {
    for (const auto& s : segments) {
        if (std::abs(s.positionY - posKM) < 0.1) {
            if (s.gauche == TypeBordure::STATION_SERVICE || s.droite == TypeBordure::STATION_SERVICE) {
                return true;
            }
        }
    }
    return false;
}

void Circuit::dessiner(sf::RenderWindow& window, double voiturePosKM) {
    // 1. Chaussée
    sf::RectangleShape route(sf::Vector2f(largeurVoie * 3, 600));
    route.setFillColor(sf::Color(50, 50, 50));
    route.setPosition(250, 0);
    window.draw(route);

    // 2. Lignes en pointillés animées
    // On définit la taille d'un tiret et de l'espace entre eux
    float longueurTiret = 40.0f;
    float espaceTiret = 40.0f;
    float cycleComplet = longueurTiret + espaceTiret;

    // Calcul du décalage (offset) basé sur la position de la voiture
    // 2000 est notre facteur de conversion KM -> Pixels
    float offset = std::fmod(static_cast<float>(voiturePosKM * 2000.0), cycleComplet);

    for (int i = 1; i < 3; ++i) { // Pour les deux séparations de voies
        float posX = 250 + i * largeurVoie - 2;

        // On dessine des tirets de haut en bas de l'écran (0 à 600)
        for (float y = -cycleComplet + offset; y < 600; y += cycleComplet) {
            sf::RectangleShape tiret(sf::Vector2f(4, longueurTiret));
            tiret.setFillColor(sf::Color(200, 200, 200));
            tiret.setPosition(posX, y);
            window.draw(tiret);
        }
    }

    // 3. Dessin des Stations XXL
    for (const auto& s : segments) {
        float relativeY = 500 - (s.positionY - voiturePosKM) * 2000;

        if (relativeY > -300 && relativeY < 900) {
            sf::RectangleShape shape(sf::Vector2f(100, 200));
            shape.setOutlineThickness(4);

            // Côté GAUCHE
            if (s.gauche != TypeBordure::VIDE) {
                if (s.gauche == TypeBordure::STATION_SERVICE) {
                    shape.setFillColor(sf::Color::Blue);
                    shape.setOutlineColor(sf::Color::White);
                } else {
                    shape.setFillColor(sf::Color::Yellow);
                    shape.setOutlineColor(sf::Color::Black);
                }
                shape.setPosition(150, relativeY);
                window.draw(shape);
            }

            // Côté DROIT
            if (s.droite != TypeBordure::VIDE) {
                if (s.droite == TypeBordure::STATION_SERVICE) {
                    shape.setFillColor(sf::Color::Blue);
                    shape.setOutlineColor(sf::Color::White);
                } else {
                    shape.setFillColor(sf::Color::Yellow);
                    shape.setOutlineColor(sf::Color::Black);
                }
                shape.setPosition(550, relativeY);
                window.draw(shape);
            }
        }
    }
}

double Circuit::getDistanceNextStation(double posKM) {
    for (const auto& s : segments) {
        if (s.positionY > posKM) {
            if (s.gauche == TypeBordure::STATION_SERVICE || s.droite == TypeBordure::STATION_SERVICE) {
                return s.positionY - posKM;
            }
        }
    }
    return -1.0;
}

double Circuit::getDistanceNextPneu(double posKM) {
    for (const auto& s : segments) {
        if (s.positionY > posKM) {
            if (s.gauche == TypeBordure::CENTRE_PNEU || s.droite == TypeBordure::CENTRE_PNEU) {
                return s.positionY - posKM;
            }
        }
    }
    return -1.0;
}
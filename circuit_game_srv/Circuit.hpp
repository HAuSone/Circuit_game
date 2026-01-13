#ifndef CIRCUIT_HPP
#define CIRCUIT_HPP

#include <vector>
#include <SFML/Graphics.hpp>

enum class TypeBordure { VIDE, STATION_SERVICE, CENTRE_PNEU };

struct Segment {
    double positionY;
    TypeBordure gauche;
    TypeBordure droite;
};

class Circuit {
private:
    double longueur;
    double largeurVoie;
    std::vector<Segment> segments;

public:
    double getDistanceNextStation(double posKM);
    double getDistanceNextPneu(double posKM);
    Circuit(double longTotal);
    void dessiner(sf::RenderWindow& window, double voiturePosZ); // Garde celle-là
    bool isStationAt(double posKM); // Ajoute celle-là si elle manque
    bool isCentrePneuAt(double posKM);
};

#endif
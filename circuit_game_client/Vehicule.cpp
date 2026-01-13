#include <iostream>
#include "Vehicule.hpp"

using namespace std;

Vehicule::Vehicule()
    : vitesseActuelle(0), mass(0) {
    cout << "Constructeur par defaut du vehicule" << endl;
}

Vehicule::Vehicule(const Moteur& eng, const Tank& t, const std::vector<Tire>& tiresVec, double m)
    : engine(eng), tank(t), tires(tiresVec), vitesseActuelle(0), mass(m) {
    cout << "Constructeur surcharge du vehicule" << endl;
}

Vehicule::~Vehicule() {
    cout << "Destructeur du vehicule" << endl;
}

double Vehicule::getCurrentSpeed() const {
    return vitesseActuelle;
}

double Vehicule::getMass() const {
    return mass;
}

void Vehicule::setMass(double m) {
    mass = m;
}

const Moteur& Vehicule::getMoteur() const {
    return engine;
}

const Tank& Vehicule::getTank() const {
    return tank;
}

const std::vector<Tire>& Vehicule::getTires() const {
    return tires;
}

void Vehicule::start() {
    cout << "Vehicule demarre..." << endl;
    vitesseActuelle = 0.1;
}

void Vehicule::updateConsommation(double dt_hour) {
    double consoMoteur = engine.getConsommation();
    double distanceParcourue = vitesseActuelle * dt_hour;
    double litresConsommes = (distanceParcourue * consoMoteur) / 100.0;

    double niveauActuel = tank.getCapacite();
    if (niveauActuel - litresConsommes > 0) {
        tank.setCapacite(niveauActuel - litresConsommes);
    } else {
        tank.setCapacite(0);
        vitesseActuelle = 0;
    }
}

void Vehicule::changerPneus() {
    for (int i = 0; i < tires.size(); i++) {
        tires[i].resetUsure();
    }
    cout << "Pneus changes !" << endl;
}

void Vehicule::faireLePlein() {
    tank.setCapacite(60.0);
}

// MODIFICATION : Limitation à la vitesse max du moteur
void Vehicule::accelerate(double deltaSpeed) {
    vitesseActuelle += deltaSpeed;
    if (vitesseActuelle > engine.getVitesseMax()) {
        vitesseActuelle = engine.getVitesseMax();
    }
    cout << "Vitesse actuelle: " << vitesseActuelle << " km/h" << endl;
}

void Vehicule::decelerate(double deltaSpeed) {
    vitesseActuelle -= deltaSpeed;
    if (vitesseActuelle < 0) vitesseActuelle = 0;
}

// MODIFICATION : Vitesse de croisière = 50% de la vitesse max
void Vehicule::cruise(double targetSpeed) {
    vitesseActuelle = engine.getVitesseMax() / 2.0;
    cout << "Cruise Control : " << vitesseActuelle << " km/h (50% de Vmax)" << endl;
}

void Vehicule::stop() {
    vitesseActuelle = 0;
}

void Vehicule::updateTireLoad(double leftRatio, double rightRatio) {
    if (tires.size() != 4) return;
    double frontLeftLoad = mass * 0.005 * leftRatio;
    double frontRightLoad = mass * 0.005 * rightRatio;
    double rearLeftLoad = mass * 0.005 * leftRatio;
    double rearRightLoad = mass * 0.005 * rightRatio;

    tires[0].updateUsure(frontLeftLoad, 1.0);
    tires[1].updateUsure(frontRightLoad, 1.0);
    tires[2].updateUsure(rearLeftLoad, 1.0);
    tires[3].updateUsure(rearRightLoad, 1.0);
}

void Vehicule::updateUsurePneus(double dt) {
    for (auto& pneu : tires) {
        pneu.updateUsure(mass * 0.005, vitesseActuelle * dt);
    }
}
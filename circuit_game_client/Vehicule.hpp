#ifndef VEHICULE_HPP
#define VEHICULE_HPP

#include "Moteur.hpp"
#include "Tank.hpp"
#include "Tire.hpp"
#include <vector>
#include <string>

class Vehicule {
private:
    Moteur engine;
    Tank tank;
    std::vector<Tire> tires;
    double vitesseActuelle;
    double mass;

public:
    Vehicule();
    Vehicule(const Moteur& eng, const Tank& t, const std::vector<Tire>& tires, double m);
    ~Vehicule();

    // Actions
    void changerPneus();
    void faireLePlein();
    void updateConsommation(double dt_hour);
    void start();
    void accelerate(double deltaSpeed);
    void decelerate(double deltaSpeed);
    void cruise(double targetSpeed);
    void stop();
    void toTheLeft();
    void toTheRight();
    void straight();

    // Affichage
    void afficher() const;

    // Getters / Setters
    double getCurrentSpeed() const;
    double getMass() const;
    void setMass(double m);

    const Moteur& getMoteur() const;
    const Tank& getTank() const;
    const std::vector<Tire>& getTires() const;

    void updateUsurePneus(double dt); // met à jour l'usure des pneus
    void setMoteur(const Moteur& eng);
    void setTank(const Tank& t);
    void setTires(const std::vector<Tire>& tires);

private:
    void updateTireLoad(double leftRatio, double rightRatio); // répartir la masse sur les pneus
};

#endif

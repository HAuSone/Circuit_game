#include <iostream>
#include "moteur.hpp"

using namespace std;

// Constructeur par défaut
Moteur::Moteur() : acceleration(0), poids(0), consommation(0), max_speed(0) {
    cout << "Constructeur par defaut de moteur" << endl;
}

// Constructeur de surcharge
Moteur::Moteur(double accel, double p, double cons, double vitm) {
    acceleration = accel;
    poids = p;
    consommation = cons;
    max_speed = vitm;
    cout << "Constructeur de surcharge moteur" << endl;
}

// Destructeur
Moteur::~Moteur() {
    cout << "Destructeur moteur" << endl;
}

// --- IMPLÉMENTATION DES GETTERS ---

double Moteur::getAcceleration() const {
    return acceleration;
}

double Moteur::getPoids() const {
    return poids;
}

double Moteur::getConsommation() const {
    return consommation;
}

double Moteur::getVitesseMax() const {
    return max_speed;
}

// --- IMPLÉMENTATION DES SETTERS ---

void Moteur::setAcceleration(double accel) {
    acceleration = accel;
}

void Moteur::setPoids(double p) {
    poids = p;
}

void Moteur::setConsommation(double cons) {
    consommation = cons;
}

void Moteur::setVitesseMax(double vitm) {
    max_speed = vitm;
}

// Affichage
void Moteur::afficher() const {
    cout << "Caracteristiques du moteur :" << endl;
    cout << "- Acceleration : " << acceleration << endl;
    cout << "- Poids : " << poids << " kg" << endl;
    cout << "- Consommation : " << consommation << " L/100" << endl;
    cout << "- Vitesse Max : " << max_speed << " km/h" << endl;
}
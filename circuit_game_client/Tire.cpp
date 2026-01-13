#include <iostream>
#include "Tire.hpp"
using namespace std;

Tire::Tire() {
    Usure = 0; // initialisation
    cout << "Constructor par defaut pneu" << endl;
}

Tire::Tire(const string& pos) {
    position = pos;
    Usure = 0;
    cout << "Constructor avec position: " << position << endl;
}

Tire::~Tire() {
    cout << "Destructor du pneu" << endl;
}

void Tire::setPosition(const string& pos) {
    position = pos;
}

string Tire::getPosition() const {
    return position;
}

void Tire::updateUsure(double kg, double km) {
    Usure += 0.005 * kg * km; // accumule l'usure
}

void Tire::resetUsure() {
    Usure = 0;
}

double Tire::getUsure() const {
    return Usure;
}

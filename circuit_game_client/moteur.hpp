#ifndef MOTEUR_HPP
#define MOTEUR_HPP

class Moteur {
private:
    double acceleration;
    double poids;
    double consommation;
    double max_speed;

public:
    // Constructeurs
    Moteur();
    Moteur(double accel, double p, double cons, double vitm);
    ~Moteur();

    // --- GETTERS ---
    double getAcceleration() const;
    double getPoids() const;
    double getConsommation() const;
    double getVitesseMax() const;

    // --- SETTERS ---
    void setAcceleration(double accel);
    void setPoids(double p);
    void setConsommation(double cons);
    void setVitesseMax(double vitm);

    // Affichage
    void afficher() const;
};

#endif
#ifndef TANK_HPP
#define TANK_HPP

class Tank {
private:
    double capacite;
    double tempsRecharge;

public:
    Tank();
    Tank(double cap, double recharge);
    ~Tank();

    double getCapacite() const;
    double getTempsRecharge() const;

    void afficher() const;
    void setCapacite(double cap);
    void setTempsRecharge(double recharge);
};

#endif

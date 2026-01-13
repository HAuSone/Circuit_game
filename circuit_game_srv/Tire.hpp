#ifndef TIRE_HPP
#define TIRE_HPP
using namespace std;
#include <string>

class Tire {
private:
    std::string position;
    double Usure;

public:
    Tire();
    Tire(const string& pos);
    ~Tire();

    string getPosition() const;
    double getUsure() const;

    void updateUsure(double kg, double km);
    void resetUsure();

    void setPosition(const std::string& pos);
};

#endif

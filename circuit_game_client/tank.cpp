#include <iostream>
#include "tank.hpp"
using namespace std;

Tank::Tank(){
  cout<<"constructeur par defaut tank"<<endl;
  }
Tank::Tank(double cap,double recharge){
  capacite=cap;
  tempsRecharge=recharge;

}

Tank::~Tank(){
  cout<<"destructeur par default tank"<<endl;
}
void Tank::setCapacite(double cap){
 capacite=cap;
  }

  void Tank::setTempsRecharge(double recharge){
    tempsRecharge=recharge;
  }

  double Tank::getCapacite() const{
    return capacite;
  }
  double Tank::getTempsRecharge() const{
    return tempsRecharge;
  }

  void Tank::afficher() const {
    cout << "Capacite: " << capacite << " L, Temps de recharge: " << tempsRecharge << " h" << endl;
}
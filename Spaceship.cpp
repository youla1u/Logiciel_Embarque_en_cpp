#include "Spaceship.h"

// CONSTRUCTOR
Spaceship::Spaceship(std::string nom) : nom(nom), equipage() {}

// GETTER
std::string Spaceship::getNom(){
    return nom;
}

std::vector<std::weak_ptr<Personnage>> Spaceship::getEquipage() {
    return equipage;
}

// METHOD

void Spaceship::deleteHabitant(std::string personnage) {
    //Recherche de la position du weak_ptr dans le vecteur
    for (auto it = equipage.begin(); it != equipage.end();) {
        auto sp = it->lock();
        if (sp && sp->getNom() == personnage) {
            it = equipage.erase(it);
        } else {
            ++it;
        }
    }
}
template <typename T>
void Spaceship::ajouteEquipage(const std::shared_ptr<T> &membre) {
    membre->setLocation(nom);
    equipage.push_back(membre);
}
template void Spaceship::ajouteEquipage<Personnage>(const std::shared_ptr<Personnage>& habitant);
template void Spaceship::ajouteEquipage<Enemy>(const std::shared_ptr<Enemy>& habitant);


bool Spaceship::estDansEquipage(std::shared_ptr<Enemy>& element){
    for (auto i = equipage.begin(); i != equipage.end();){
        if (auto sharedHabitant = i->lock()){
            if (sharedHabitant == element){
                return true;
            }
            ++i;
        }
        else{
            i = equipage.erase(i);
        }
    }
    return false;
}

bool Spaceship::estDansEquipage(std::shared_ptr<Personnage>& element){
    for (auto i = equipage.begin(); i != equipage.end();){
        if (auto sharedHabitant = i->lock()){
            if (sharedHabitant == element){
                return true;
            }
            ++i;
        }
        else{
            i = equipage.erase(i);
        }
    }
    return false;
}

void Spaceship::majEquipage(){
    for (auto i = equipage.begin(); i != equipage.end();){
        if (auto sharedHabitant = i->lock()){
            ++i;
        } else {
            i = equipage.erase(i);
        }
    }
}

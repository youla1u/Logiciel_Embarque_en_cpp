#include "Planet.h"

// CONSTRUCTOR
Planet::Planet(std::string nom, std::string description): nom(nom), description(description) {}

// GETTER
std::string Planet::getNom() const{
    return nom;
}

std::string Planet::getDescription() const{
    return description;
}

std::vector<std::weak_ptr<Personnage>> Planet::getHabitants(){
    return habitants;
}

// SETTER

void Planet::deleteHabitant(std::string personnage) {
    //Recherche de la position du weak_ptr dans le vecteur
    for (auto it = habitants.begin(); it != habitants.end();) {
        auto sp = it->lock();
        if (sp && sp->getNom() == personnage) {
            it = habitants.erase(it);
        } else {
            ++it;
        }
    }
}

template <typename T>
void Planet::setHabitant(const std::shared_ptr<T>& habitant) {
    habitant->setLocation(nom);
    habitants.push_back(habitant);
}
// Explicit instantiations for Personnage and Enemy
template void Planet::setHabitant<Personnage>(const std::shared_ptr<Personnage>& habitant);
template void Planet::setHabitant<Enemy>(const std::shared_ptr<Enemy>& habitant);

// METHOD

bool Planet::estHabitant(std::shared_ptr<Personnage>& habitant){
    for (auto i = habitants.begin(); i != habitants.end();){
        if (auto sharedHabitant = i->lock()){
            if (sharedHabitant == habitant){
                return true;
            }
            ++i;
        }
        else{
            i = habitants.erase(i);
        }
    }
    return false;
}



bool Planet::estHabitant(std::shared_ptr<Enemy>& habitant){
    for (auto i = habitants.begin(); i != habitants.end();){
        if (auto sharedHabitant = i->lock()){
            if (sharedHabitant == habitant){
                return true;
            }
            ++i;
        }
        else{
            i = habitants.erase(i);
        }
    }
    return false;
}



void Planet::majHabitants(){
    for (auto i = habitants.begin(); i != habitants.end();){
        if (auto sharedHabitant = i->lock()){
            ++i;
        }
        else {
            i = habitants.erase(i);
        }
    }
}

// DESTRUCTOR
Planet::~Planet(){}

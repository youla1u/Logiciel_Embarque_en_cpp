#include "Mission.h"

// CONSTRUCTOR
Mission::Mission(std::string nom, std::string description) : nom(nom), description(description), complete(false) {}

// GETTER
std::string Mission::getNom() const{
    return nom;
}

std::string Mission::getDescription() const{
    return description;
}

bool Mission::getComplete(){
    return complete;
}

// SETTER
void Mission::setComplete(bool b){
    complete = b;
}

// METHOD
void Mission::afficherMission(){
    std::cout << getNom() << " | " << getDescription() << (getComplete() ? " | mission effectuee" : " | mission non effectuee") << std::endl;
}

// DESTRUCTOR
Mission::~Mission(){
}

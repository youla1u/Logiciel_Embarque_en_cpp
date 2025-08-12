#include "Personnage.h"

// CONSTRUCTOR
Personnage::Personnage(std::string nom, std::string description, int hp, int ap, int dp){
    this->nom = nom;
    this->description = description;
    this->hp = hp;
    this->ap = ap;
    this->dp = dp;
    this->Nb_added = 0; // le nombre d'items qu'il à mis dans son sac (la map)
    this->Nb_used  = 0; // le nombre d'items qu'il à utilisés
}

// GETTER
std::string Personnage::getNom() {
    return nom;
}

std::string Personnage::getDescription() {
    return description;
}

int Personnage::getHp() {
    return hp;
}

int Personnage::getAp() {
    return ap;
}

int Personnage::getDp() {
    return dp;
}

std::string Personnage::getLocation(){
    return location;
}

std::map<std::string, std::unique_ptr<Item>>& Personnage::getItems(){
    return items;
}

int Personnage::get_nb_Items_used(){
    return Nb_used;
}

int Personnage::get_nb_Items_added(){
    return Nb_added;
}

// SETTER
void Personnage::setHp(int newHp) {
    hp = newHp;
}

void Personnage::setAp(int newAp) {
    ap = newAp;
}

void Personnage::setDp(int newDp) {
    dp = newDp;
}

void Personnage::addItem(std::unique_ptr<Item> item){
    items.insert(std::make_pair(item->getName(), std::move(item)));
    Nb_added++;
}


void Personnage::useItem(int item_effect){
     hp += item_effect;
     Nb_used++;
}

void Personnage::setLocation(std::string newLocation){
    location = newLocation;
}

// METHOD
bool Personnage::attack(std::shared_ptr<Personnage> personnage) {
    // Calcul des degats potentiels infliges par l'attaquant
    int potentialDmg = ap - personnage->getDp();

    if (potentialDmg > 0) {
        // Infliger les degats au personnage attaque
        int newHp = personnage->getHp() - potentialDmg;
        if (newHp < 0) {
            newHp = 0;
        }
        personnage->setHp(newHp);

        // Verifier si le personnage attaque est vaincu
        if (newHp == 0) {
            return true;
        }
    }
    return false;
}


void Personnage::presentation(){
    #ifdef DEBUG
    std::cout << nom << " " << description << std::endl;
    std::cout << hp << " " << ap << " " << dp << std::endl;
    #endif // DEBUG
}

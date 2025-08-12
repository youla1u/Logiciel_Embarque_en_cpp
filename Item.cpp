#include "Item.h"

Item::Item(std::string name, std::string description, int effect){
    this->name = name;
    this->description = description;
    this->effect = effect;
}

//GETTER
std::string Item::getName(){
    return name;
}

std::string Item::getDescription(){
    return description;
}

int Item::getEffect(){
    return effect;
}


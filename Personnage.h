#ifndef PERSONNAGE_H
#define PERSONNAGE_H

#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <map>
#include "Item.h"

class Personnage {
    public:

        // CONSTRUCTOR
        Personnage(std::string nom, std::string description, int hp, int ap, int dp);

        // GETTER
        std::string getNom();
        std::string getDescription();
        int getHp();
        int getAp();
        int getDp();
        std::string getLocation();
        std::map<std::string, std::unique_ptr<Item>>& getItems();
        
        int get_nb_Items_added();
        int get_nb_Items_used();

        // SETTER
        void setHp(int newHp);
        void setAp(int newAp);
        void setDp(int newDp);
        void setLocation(std::string location);

        // METHOD
        bool attack(std::shared_ptr<Personnage> personnage);
        void presentation();
        void addItem(std::unique_ptr<Item> item);
        void useItem(int item_effect);

    private:
        std::string nom;
        std::string description;
        std::string location;
        int hp;
        int ap;
        int dp;
        std::map<std::string, std::unique_ptr<Item>> items;
        int Nb_added;
        int Nb_used;
};

#endif // PERSONNAGE_H

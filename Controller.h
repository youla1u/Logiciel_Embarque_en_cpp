#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <memory>
#include <map>
#include <cassert>

#include "Mission.h"
#include "Personnage.h"
#include "Planet.h"
#include "Spaceship.h"
#include "Enemy.h"
#include "Item.h"

class Controller
{
    public:

        // CONTROLLER
        Controller();

        // GETTER
        std::map<std::string, std::shared_ptr<Personnage>> getPersonnages();
        std::map<std::string, std::shared_ptr<Enemy>> getEnemy();
        std::map<std::string, std::shared_ptr<Planet>> getPlanetes();
        std::map<std::string, std::shared_ptr<Spaceship>> getSpaceships();
        std::map<std::string, std::shared_ptr<Mission>> getMissions();
        std::map<std::string, std::unique_ptr<Item>>& getItems();
        /*int get_nb_Items_retants(std::string perso_name);
        int get_nb_Items_used(std::string perso_name);
        int get_nb_Items_added(std::string perso_name);*/

        // METHOD
        void menu();
        void initalisation();
        std::string save();
        void addPersonnage(std::shared_ptr<Personnage> personnage);
        void addPlanete(std::shared_ptr<Planet> planete);
        void addSpaceship(std::shared_ptr<Spaceship> spaceship);
        void addMission(std::shared_ptr<Mission> mission);
        void addItem(std::string name, std::string description, int effect);
        void deletePersonnage(std::string personnage);
        void deletePlanete(std::string planete);
        void deletePlanete(int planete_id);
        void deleteSpaceship(std::string spaceship);
        void deleteMission(std::string mission);
        void deleteItem(std::string item);
        bool attack(std::string attackCharacter, std::string defCharacter);
        void addUnusedItem(std::string personage_name, std::string item_name);
        void useItem(std::string personage_name, std::string item_name);
        void move(std::string personnage, std::string place);
        virtual ~Controller();

    protected:

    private:

        std::map<std::string, std::shared_ptr<Planet>> planetMap;
        std::map<std::string, std::shared_ptr<Spaceship>> spaceshipMap;
        std::map<std::string, std::shared_ptr<Personnage>> personnageMap;
        std::map<std::string, std::shared_ptr<Mission>> missionMap;
        std::map<std::string, std::shared_ptr<Enemy>> enemyMap;
        std::map<std::string, std::unique_ptr<Item>> itemMap;

        void handleAction(int action);
        void displayMenu();
        void importPlanete();
        void importSpaceship();
        void importPersonnage();
        void importMission();
        void importItem();
        void savePlanete();
        void saveSpaceship();
        void savePersonnage();
        void saveMission();
        void saveItem();
        void deletePersonnage(std::shared_ptr<Personnage> personnage);
        void deletePlanete(std::shared_ptr<Planet> planete);
        void deleteSpaceship(std::shared_ptr<Spaceship> spaceship);
        void deleteMission(std::shared_ptr<Mission> mission);
        bool attack(std::shared_ptr<Personnage> attackCharacter, std::shared_ptr<Personnage> defCharacter);
        std::vector<std::string> split(const std::string& phrase, char delimiter);

};

#endif // CONTROLLER_H

#define DEBUG
#include "Controller.h"

Controller::Controller(){}

// GETTER

std::map<std::string, std::shared_ptr<Personnage>> Controller::getPersonnages(){
    return personnageMap;
}

std::map<std::string, std::shared_ptr<Enemy>> Controller::getEnemy(){
    return enemyMap;
}

std::map<std::string, std::shared_ptr<Planet>> Controller::getPlanetes(){
    return planetMap;
}

std::map<std::string, std::shared_ptr<Spaceship>> Controller::getSpaceships(){
    return spaceshipMap;
}

std::map<std::string, std::shared_ptr<Mission>> Controller::getMissions(){
    return missionMap;
}

std::map<std::string, std::unique_ptr<Item>>& Controller::getItems(){
    return itemMap;
}


// Game
bool Controller::attack(std::shared_ptr<Personnage> attackCharacter, std::shared_ptr<Personnage> defCharacter){
    bool result = attackCharacter->attack(defCharacter);
    if(result){
        std::cout  << defCharacter->getNom() << " est mort" << std::endl;
        deletePersonnage(defCharacter);
    }
    return result;

}

bool Controller::attack(std::string attackCharacter, std::string defCharacter) {
    bool attackerFind = false;
    bool defenderFind = false;

    // Recherche de l'attaquant
    std::shared_ptr<Personnage> attackerPtr;
    auto attackerPersonnage = personnageMap.find(attackCharacter);
    if (attackerPersonnage != personnageMap.end()) {
        attackerPtr = attackerPersonnage->second;
        attackerFind = true;
    } else {
        auto attackerEnemy = enemyMap.find(attackCharacter);
        if (attackerEnemy != enemyMap.end()) {
            attackerPtr = std::static_pointer_cast<Personnage>(attackerEnemy->second);
            attackerFind = true;
        }
    }

    // Recherche du defenseur
    std::shared_ptr<Personnage> defenderPtr;
    auto defenderPersonnage = personnageMap.find(defCharacter);
    if (defenderPersonnage != personnageMap.end()) {
        defenderPtr = defenderPersonnage->second;
        defenderFind = true;
    } else {
        auto defenderEnemy = enemyMap.find(defCharacter);
        if (defenderEnemy != enemyMap.end()) {
            defenderPtr = std::static_pointer_cast<Personnage>(defenderEnemy->second);
            defenderFind = true;
        }
    }

    if (attackerFind && defenderFind) {
        attack(attackerPtr, defenderPtr);
        return true;
    }
    return false;
}

void Controller::move(std::string personnage, std::string place){
    auto personnageFind = personnageMap.find(personnage); // chercher un personnage das personnageMap qui correspond  à  personnage
    if (personnageFind != personnageMap.end()) { // si on le trouve
        //personnageFind->second->setLocation("Terre");
        auto planetIt = planetMap.find(place); // chercher un nom de planette dans planetMap qui correspond  à  place
        if (planetIt != planetMap.end()) {    // si on le trouve
            auto planetBaseIt = planetMap.find(personnageFind->second->getLocation()); // chercher  localisation de personnage
            if (planetBaseIt != planetMap.end()){ // si on la trouve dans une autre planette dans  planeteMap
                planetBaseIt->second->deleteHabitant(personnage);  // le supprimer de cette plannette
                planetIt->second->setHabitant(personnageFind->second); // puis la mettre dans la nouvelle planntte passée en paramettre
                personnageFind->second->setLocation(place); // enfin modifier le nom de sa locatilisation
                return;
            }
          auto spaceshipBaseIt = spaceshipMap.find(personnageFind->second->getLocation()); // chercher un nom vaiseau dans spaceshipMap qui correspond  à  place
            if (spaceshipBaseIt != spaceshipMap.end()) {  // si on le trouve
                spaceshipBaseIt->second->deleteHabitant(personnage);  // le supprimer de ce vaisseau
                planetIt->second->setHabitant(personnageFind->second); // puis la mettre dans la nouvelle planntte passée en paramettre
                personnageFind->second->setLocation(place); // enfin modifier le nom de sa locatilisation
                return;
            }
        }

        //Vérification dans spaceshipMap
        auto spaceshipIt = spaceshipMap.find(place); //chercher un personnage das spaceshipMap qui correspond  à  personnage
        if (spaceshipIt != spaceshipMap.end()){  // si on le trouve
            auto planetBaseIt = planetMap.find(personnageFind->second->getLocation()); // chercher  localisation de personnage
            if (planetBaseIt != planetMap.end()){ // si on la trouve dans une planette dans  planeteMap
                planetBaseIt->second->deleteHabitant(personnage);  // le supprimer de cette plannette
                spaceshipIt->second->ajouteEquipage(personnageFind->second); // puis la mettre dans la nouvelle planntte passée en paramettre
                personnageFind->second->setLocation(place); // enfin modifier le nom de sa locatilisation
                return;
            }
            auto spaceshipBaseIt = spaceshipMap.find(personnageFind->second->getLocation()); // chercher un nom vaiseau dans spaceshipMap qui correspond  à  place
            if (spaceshipBaseIt != spaceshipMap.end()) {  // si on le trouve
                spaceshipBaseIt->second->deleteHabitant(personnage);  // le supprimer de ce vaisseau
                spaceshipIt->second->ajouteEquipage(personnageFind->second); // puis la mettre dans le nouveau vaisseau passé en paramettre
                personnageFind->second->setLocation(place); // enfin modifier le nom de sa locatilisation
                return;
            }
        }
    }
}


void Controller::addUnusedItem(std::string personage_name, std::string item_name){

    auto it =  itemMap.find(item_name);

    if (it != itemMap.end()){
        for(auto& pair1 : personnageMap){
            if(pair1.second->getNom() == personage_name ){
               for(auto& pair2 : itemMap){
                   if(pair2.second->getName() == item_name){
                      pair1.second->addItem(std::move(pair2.second));
                      itemMap.erase(item_name);
                      break;
                    }
                }
             }
        }
     }
}



void Controller::useItem(std::string personage_name, std::string item_name){
       for(auto& pair : personnageMap){
          if(pair.second->getNom() == personage_name ){
             for(auto& pair0 : pair.second->getItems()){
                 if(pair0.second->getName() == item_name){
                    pair.second->useItem(pair0.second->getEffect());
                    pair.second->getItems().erase(item_name);
                    break;
                  }
              }
           }
       }
}




void Controller::initalisation(){

    // On initialise les objets
    //std::cout << "Initalisation en cours..." << std::endl;
    importPlanete();
    importSpaceship();
    importPersonnage();
    importMission();
    importItem();
    //std::cout << "Initalisation termine" << std::endl;

}

std::string Controller::save(){

    // Sauvegarde des objets
    //std::cout << "Sauvegarde en cours..." << std::endl;
    savePlanete();
    saveSpaceship();
    savePersonnage();
    saveMission();
    saveItem();
    return "Sauvegarde termine";
}

void Controller::importPlanete(){

    std::ifstream fichier("planete.txt");
    std::string ligne;

    #ifdef DEBUG
    assert(fichier.is_open() && "Le fichier doit exiter");
    #endif

    if (fichier.is_open()) {
        while (std::getline(fichier, ligne)) {
            std::vector<std::string> elements = split(ligne, ':');
            std::shared_ptr<Planet> planete = std::make_shared<Planet>(elements[0], elements[1]);
            planetMap[planete->getNom()] = planete;
        }
        fichier.close();
    } else {
        std::cout << "Impossible d'ouvrir le fichier" << std::endl;
    }
}

void Controller::importSpaceship(){

    std::ifstream fichier("Spaceship.txt");
    std::string ligne;

    #ifdef DEBUG
    assert(fichier.is_open() && "Le fichier doit exiter");
    #endif

    if (fichier.is_open()) {
        while (std::getline(fichier, ligne)) {
            std::vector<std::string> elements = split(ligne, ':');
            std::shared_ptr<Spaceship> spaceship = std::make_shared<Spaceship>(elements[0]);
            spaceshipMap[spaceship->getNom()] = spaceship;
        }
        fichier.close();
    } else {
        std::cout << "Impossible d'ouvrir le fichier" << std::endl;
    }
}

void Controller::importPersonnage(){

    std::ifstream fichier("Personnage.txt");
    std::string ligne;

    #ifdef DEBUG
    assert(fichier.is_open() && "Le fichier doit exiter");
    #endif

    if (fichier.is_open()) {
        while (std::getline(fichier, ligne)) {

            std::vector<std::string> elements = split(ligne, ':');

            //On creer le personnage selon ou il est
            if("Planet" == elements[0]){
                for (auto& pair : planetMap) {
                    std::string planeteName = pair.first;
                    std::shared_ptr<Planet> planetePtr = pair.second;
                    if(planeteName == elements[1]){
                        if ("Personnage" == elements[2]) {
                            std::shared_ptr<Personnage> personnage = std::make_shared<Personnage>(elements[3], elements[4], std::stoi(elements[5]), std::stoi(elements[6]), std::stoi(elements[7]));
                            planetePtr->setHabitant(personnage);
                            personnage->setLocation(elements[1]);
                            personnageMap[personnage->getNom()] = personnage;
                            break;
                        }
                        else if ("Enemy" == elements[2]) {
                            std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(elements[3], elements[4], std::stoi(elements[5]), std::stoi(elements[6]), std::stoi(elements[7]));
                            planetePtr->setHabitant(enemy);
                            enemy->setLocation(elements[1]);
                            enemyMap[enemy->getNom()] = enemy;
                        }
                    }
                }
            }
            if("Spaceship" == elements[0]){
                for (auto& pair : spaceshipMap) {
                    std::string spaceshipName = pair.first;
                    std::shared_ptr<Spaceship> spaceshipPtr = pair.second;
                    if(spaceshipName == elements[1]){
                        if ("Personnage" == elements[2]) {
                            std::shared_ptr<Personnage> personnage = std::make_shared<Personnage>(elements[3], elements[4], std::stoi(elements[5]), std::stoi(elements[6]), std::stoi(elements[7]));
                            spaceshipPtr->ajouteEquipage(personnage);
                            personnage->setLocation(elements[1]);
                            personnageMap[personnage->getNom()] = personnage;
                            break;
                        }
                        else if ("Enemy" == elements[2]) {
                            std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(elements[3], elements[4], std::stoi(elements[5]), std::stoi(elements[6]), std::stoi(elements[7]));
                            spaceshipPtr->ajouteEquipage(enemy);
                            enemy->setLocation(elements[1]);
                            enemyMap[enemy->getNom()] = enemy;
                        }
                    }
                }
            }
        }
        fichier.close();
    } else {
        std::cout << "Impossible d'ouvrir le fichier" << std::endl;
    }
}

void Controller::importMission(){

    std::ifstream fichier("Mission.txt");
    std::string ligne;

    #ifdef DEBUG
    assert(fichier.is_open() && "Le fichier doit exiter");
    #endif

    if (fichier.is_open()) {
        while (std::getline(fichier, ligne)) {
            std::vector<std::string> elements = split(ligne, ':');
            std::shared_ptr<Mission> mission = std::make_shared<Mission>(elements[0], elements[1]);
            mission->setComplete(std::stoi(elements[2]));
            missionMap[mission->getNom()] = mission;
        }
        fichier.close();
    } else {
        std::cout << "Impossible d'ouvrir le fichier" << std::endl;
    }
}

void Controller::importItem(){

    std::ifstream fichier("Item.txt");
    std::string ligne;

    #ifdef DEBUG
    assert(fichier.is_open() && "Le fichier doit exiter");
    #endif

    if (fichier.is_open()) {
        while (std::getline(fichier, ligne)) {

            std::vector<std::string> elements = split(ligne, ':');


            if(elements.size() > 3){
                personnageMap[elements[4]]->addItem(std::make_unique<Item>(elements[0], elements[1], std::stoi(elements[2])));
            } else{
                itemMap[elements[0]] = std::make_unique<Item>(elements[0], elements[1], std::stoi(elements[2]));
            }
        }
        fichier.close();
    } else {
        std::cout << "Impossible d'ouvrir le fichier" << std::endl;
    }
}

void Controller::saveSpaceship() {
    std::ofstream fichier("Spaceship.txt");

    #ifdef DEBUG
    assert(fichier.is_open() && "Le fichier doit exiter");
    #endif

    if (fichier.is_open()) {
        for(auto& pair : spaceshipMap){
            fichier << pair.second->getNom() << "\n";
        }
        fichier.close();
        //std::cout << "Les spaceships ont ete sauvegardes dans le fichier Spaceship.txt." << std::endl;
    } else {
        std::cout << "Impossible d'ouvrir le fichier." << std::endl;
    }
}

void Controller::savePlanete() {
    std::ofstream fichier("planete.txt");

    #ifdef DEBUG
    assert(fichier.is_open() && "Le fichier doit exiter");
    #endif

    if (fichier.is_open()) {
        for(auto& pair : planetMap){
            fichier << pair.second->getNom() << ":" << pair.second->getDescription() << "\n";
        }
        fichier.close();
        //std::cout << "Les planetes ont ete sauvegardees dans le fichier planete.txt." << std::endl;
    } else {
        std::cout << "Impossible d'ouvrir le fichier." << std::endl;
    }
}

void Controller::savePersonnage() {
    std::ofstream fichier("Personnage.txt");

    #ifdef DEBUG
    assert(fichier.is_open() && "Le fichier doit exiter");
    #endif

    if (fichier.is_open()) {
        //Sauvegarder les personnages
        for(auto& pairPersonnage : personnageMap){
            bool personnageTrouve = false;
            for(auto& pairPlanet : planetMap){
                if (pairPlanet.second->estHabitant(pairPersonnage.second)) {
                    fichier << "Planet:" << pairPlanet.second->getNom() << ":Personnage:" << pairPersonnage.second->getNom() << ":" << pairPersonnage.second->getDescription() << ":" << pairPersonnage.second->getHp() << ":" << pairPersonnage.second->getAp() << ":" << pairPersonnage.second->getDp() << "\n";
                    personnageTrouve = true;
                    break;
                }
            }
            if (!personnageTrouve) {
                for(auto& pairSpaceship : spaceshipMap) {
                    if (pairSpaceship.second->estDansEquipage(pairPersonnage.second)) {
                        fichier << "Spaceship:" << pairSpaceship.second->getNom() << ":Personnage:" << pairPersonnage.second->getNom() << ":" << pairPersonnage.second->getDescription() << ":" << pairPersonnage.second->getHp() << ":" << pairPersonnage.second->getAp() << ":" << pairPersonnage.second->getDp() << "\n";
                        break;
                    }
                }
            }
        }
        // Sauvegarder les ennemis
        for(auto& pairEnemy : enemyMap){
            bool ennemiTrouve = false;
            for(auto& pairPlanet : planetMap){
                if (pairPlanet.second->estHabitant(pairEnemy.second)) {
                    fichier << "Planet:" << pairPlanet.second->getNom() << ":Enemy:" << pairEnemy.second->getNom() << ":" << pairEnemy.second->getDescription() << ":" << pairEnemy.second->getHp() << ":" << pairEnemy.second->getAp() << ":" << pairEnemy.second->getDp() << "\n";
                    ennemiTrouve = true;
                    break;
                }
            }
            if (!ennemiTrouve) {
                for(auto& pairSpaceship : spaceshipMap) {
                    if (pairSpaceship.second->estDansEquipage(pairEnemy.second)) {
                        fichier << "Spaceship:" << pairSpaceship.second->getNom() << ":Enemy:" << pairEnemy.second->getNom() << ":" << pairEnemy.second->getDescription() << ":" << pairEnemy.second->getHp() << ":" << pairEnemy.second->getAp() << ":" << pairEnemy.second->getDp() << "\n";
                        break;
                    }
                }
            }
        }
        fichier.close();
        //std::cout << "Les personnages ont ete sauvegardes dans le fichier Personnage.txt." << std::endl;
    } else {
        std::cout << "Impossible d'ouvrir le fichier." << std::endl;
    }

}

void Controller::saveMission() {
    std::ofstream fichier("Mission.txt");

    #ifdef DEBUG
    assert(fichier.is_open() && "Le fichier doit exiter");
    #endif

    if (fichier.is_open()) {
        for(auto& pair : missionMap){
            fichier << pair.second->getNom() << ":" << pair.second->getDescription() << ":" << pair.second->getComplete() << "\n";
        }
        fichier.close();
        //std::cout << "Les missions ont ete sauvegardees dans le fichier Mission.txt." << std::endl;
    } else {
        std::cout << "Impossible d'ouvrir le fichier." << std::endl;
    }
}

void Controller::saveItem() {
    
    std::ofstream fichier("Item.txt");

    #ifdef DEBUG
    assert(fichier.is_open() && "Le fichier doit exiter");
    #endif

    if (fichier.is_open()) {
        for(auto& pair : itemMap){
            fichier << pair.second->getName() << ":" << pair.second->getDescription() << ":" << pair.second->getEffect() << "\n";
        }
        for(auto& pair : personnageMap){
            if(!pair.second->getItems().empty()){
                for(auto& result : pair.second->getItems()){
                    fichier << result.second->getName() << ":" << result.second->getDescription() << ":" << result.second->getEffect()<< ":Personnage:" << pair.second->getNom() <<"\n";
                }
            }
        }
        fichier.close();
        //std::cout << "Les items ont ete sauvegardees dans le fichier Items.txt." << std::endl;
    } else {
        std::cout << "Impossible d'ouvrir le fichier." << std::endl;
    }
}

void Controller::addPersonnage(std::shared_ptr<Personnage> personnage){
    personnageMap[personnage->getNom()] = personnage;
}

void Controller::addPlanete(std::shared_ptr<Planet> planete){
    planetMap[planete->getNom()] = planete;
}

void Controller::addSpaceship(std::shared_ptr<Spaceship> spaceship){
    spaceshipMap[spaceship->getNom()] = spaceship;
}

void Controller::addMission(std::shared_ptr<Mission> mission){
    missionMap[mission->getNom()] = mission;
}

void Controller::addItem(std::string name, std::string description, int effect){
    itemMap[name] = std::make_unique<Item>(name, description, effect);
}

void Controller::deletePersonnage(std::shared_ptr<Personnage> personnage){
    // Supprimer le personnage du std::map personnageMap
    for (auto it = personnageMap.begin(); it != personnageMap.end(); ++it) {
        if (it->second == personnage) {
            personnageMap.erase(it);
            break;
        }
    }
}

void Controller::deletePersonnage(std::string personnage){
    // Supprimer le personnage du std::map personnageMap
    auto it =  personnageMap.find(personnage);
    if (it != personnageMap.end()) {
        deletePersonnage(it->second);
    }
}

void Controller::deletePlanete(std::string planete){
    auto it =  planetMap.find(planete);
    if (it != planetMap.end()) {
        deletePlanete(it->second);
    }
}

void Controller::deletePlanete(std::shared_ptr<Planet> planete){
    for(auto& habitantWeakPtr : planete->getHabitants()){
        if (auto habitantSharedPtr = habitantWeakPtr.lock()){
            deletePersonnage(habitantSharedPtr);
        }
    }

    // Supprimer la planete du std::map planetMap
    for (auto it = planetMap.begin(); it != planetMap.end(); ++it) {
        if (it->second == planete) {
            planetMap.erase(it);
            break;
        }
    }
}

void Controller::deleteSpaceship(std::string spaceship){
    auto it =  spaceshipMap.find(spaceship);
    if (it != spaceshipMap.end()){
        deleteSpaceship(it->second);
    }
}

void Controller::deleteSpaceship(std::shared_ptr<Spaceship> spaceship){
    for(auto& habitantWeakPtr : spaceship->getEquipage()){
        if (auto habitantSharedPtr = habitantWeakPtr.lock()){
            deletePersonnage(habitantSharedPtr);
        }
    }

    // Supprimer le spaceship du std::map spaceshipMap
    for (auto it = spaceshipMap.begin(); it != spaceshipMap.end(); ++it) {
        if (it->second == spaceship) {
            spaceshipMap.erase(it);
            break;
        }
    }
}


void Controller::deleteMission(std::string mission){
    auto it =  missionMap.find(mission);
    if (it != missionMap.end()) {
        deleteMission(it->second);
    }
}

void Controller::deleteMission(std::shared_ptr<Mission> mission){
    for (auto it = missionMap.begin(); it != missionMap.end(); ++it) {
        if (it->second == mission) {
            missionMap.erase(it);
            break;
        }
    }
}


void Controller::deleteItem(std::string item){
    auto it =  itemMap.find(item);
    if (it != itemMap.end()) {
        itemMap.erase(it);
    }
}


std::vector<std::string> Controller::split(const std::string& phrase, char delimiter) {
    std::vector<std::string> mots;
    std::stringstream ss(phrase);
    std::string mot;

    while (std::getline(ss, mot, delimiter)) {
        mots.push_back(mot);
    }

    return mots;
}

Controller::~Controller() {
}


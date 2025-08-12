#include "Serveur.h"
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <json/json.h>
#include <time.h>
#include <iomanip>

#define ADDRESS "0.0.0.0"
#define PORT 8047
#define CONCURRENT_CONNECTION 10
#define QUEUE_CONNECTION 20
#define BUFFER_SIZE 1024
#define THREAD_STACK_SIZE 65536

Serveur::Serveur(Controller* controller){
    this->controller = controller;
}

int connection = 0;

void *connection_handler(void *);
Json::Value action(std::string key, Controller* controller, Json::Value root);

int Serveur::run() {

    pthread_t thread_id;
    pthread_attr_t attr;

    if (pthread_attr_init(&attr) != 0) {
        std::cout << "[ERROR][THREAD][INIT] " << strerror(errno) << "\n";
        return -1;
    }

    if (pthread_attr_setstacksize(&attr, THREAD_STACK_SIZE) != 0) {
        std::cout << "[ERROR][THREAD][STACK] " << strerror(errno) << "\n";
        return -1;
    }

    if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0) {
        std::cout << "[ERROR][THREAD][DETACH] " << strerror(errno) << "\n";
        return -1;
    }

    int master_socket, conn_id;
    struct sockaddr_in server, client;

    memset(&server, 0, sizeof(server));
    memset(&client, 0, sizeof(client));

    signal(SIGPIPE, SIG_IGN);

    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cout << "[ERROR] CAN'T CREATE SOCKET\n";
        return -1;
    } else {
        std::cout << "[NOTE] SOCKET CREATED\n";
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ADDRESS);
    server.sin_port = htons(PORT);

    socklen_t addrlen = sizeof(struct sockaddr_in);

    if (bind(master_socket, (struct sockaddr *)&server, sizeof(server)) == -1) {
        std::cout << "[ERROR][BIND] " << strerror(errno) << "\n";
        return -1;
    } else {
        std::cout << "[NOTE] BIND " << ADDRESS << ":" << PORT << "\n";
    }

    if (listen(master_socket, QUEUE_CONNECTION) == -1) {
        std::cout << "[ERROR][LISTEN] " << strerror(errno) << "\n";
        return -1;
    } else {
        std::cout << "[INFO] WAITING FOR INCOMING CONNECTIONS\n";
    }

    while (true) {
        conn_id = accept(master_socket, (struct sockaddr *)&client, (socklen_t *)&addrlen);

        if (conn_id == -1) {
            std::cout << "[WARNING] CAN'T ACCEPT NEW CONNECTION\n";
        } else {
            if (connection >= CONCURRENT_CONNECTION) {
                std::cout << "[WARNING] CONNECTION LIMIT REACHED\n";
                close(conn_id);
            } else {
                std::cout << "[INFO] NEW CONNECTION ACCEPTED FROM " << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << "\n";
                ConnectionData data;
                data.conn_id = new int(conn_id);
                data.controller = controller;
                if (pthread_create(&thread_id, &attr, connection_handler, static_cast<void*>(&data)) == -1) {
                    std::cout << "[WARNING] CAN'T CREATE NEW THREAD\n";
                    close(conn_id);
                } else {
                    std::cout << "[INFO] NEW THREAD CREATED\n";
                    connection++;
                }
            }
        }
    }

    return 0;
}

void *connection_handler(void *data_ptr) {
    ConnectionData* data = static_cast<ConnectionData*>(data_ptr);

    // Accéder au descripteur de socket et à l'instance du contrôleur réelle
    int conn_id = *(int*)data->conn_id;
    Controller* controller = data->controller;

    clock_t start, end;
    start = clock();

    int read_byte = 0;
    char buffer[BUFFER_SIZE] = {0};

    while ((read_byte = recv(conn_id, buffer, BUFFER_SIZE, 0)) > 0) {
        std::cout << "[RECEIVED] " << buffer << "\n";

        Json::Value root;
        Json::CharReaderBuilder readerBuilder;
        Json::CharReader *reader = readerBuilder.newCharReader();
        JSONCPP_STRING err;

        if (!reader->parse(buffer, buffer + read_byte, &root, &err)) {
            std::cout << "[ERROR] Failed to parse JSON: " << err << std::endl;
            delete reader;
            break;
        }

        delete reader;

        std::cout << "[INFO] Parsing JSON...\n";

        if (!root.isObject()) {
            std::cout << "[ERROR] Invalid JSON format: root is not an object\n";
            break;
        }

        // Parcourir toutes les clés de l'objet JSON
        for (Json::Value::iterator it = root.begin(); it != root.end(); ++it) {
            std::string key = it.key().asString();
            Json::Value response = action(key, controller, root); // Stockez le résultat de l'action dans la variable "response"

            // Envoyer la réponse JSON pour chaque clé
            Json::StreamWriterBuilder writerBuilder;
            std::string jsonResponse = Json::writeString(writerBuilder, response);

            if (send(conn_id, jsonResponse.c_str(), jsonResponse.length(), 0) > 0) {
                std::cout << "[SEND] " << jsonResponse << "\n";
            } else {
                std::cout << "[WARNING][SEND] " << strerror(errno) << "\n";
            }
        }

        // Réinitialiser le tampon après l'envoi de la réponse
        memset(buffer, 0, BUFFER_SIZE);
    }

    close(conn_id);
    std::cout << "[INFO] CONNECTION CLOSED\n";

    connection--;
    std::cout << "[INFO] THREAD TERMINATED" << std::endl;

    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);

    std::cout << "[TIME] PROCESS COMPLETE IN " << std::fixed << time_taken;
    std::cout << " SEC\n";

    std::cout << "------------------------\n";

    pthread_exit(NULL);
}


Json::Value action(std::string key, Controller* controller, Json::Value root){
    Json::Value response;

    if(key == "Attack"){
        std::string attacker = root["Attack"]["attacker"].asString();
        std::string defender = root["Attack"]["defender"].asString();

        bool result = controller->attack(attacker, defender);
        response["successAttack"] = result;
        return response;
    } else if(key == "Afficher"){
        if(root["Afficher"] == "Personnages"){
            Json::Value personnagesJson(Json::arrayValue);
            for (const auto& pair : controller->getPersonnages()) {
                Json::Value personnageJson;
                personnageJson["Nom"] = pair.second->getNom();
                personnageJson["HP"] = pair.second->getHp();
                personnageJson["AP"] = pair.second->getAp();
                personnageJson["DP"] = pair.second->getDp();
                personnageJson["Description"] = pair.second->getDescription();
                personnagesJson.append(personnageJson);
            }
            response["Personnages"] = personnagesJson;
            return response;
        } else if(root["Afficher"] == "Ennemis"){
            Json::Value ennemisJson(Json::arrayValue);
            for (const auto& pair : controller->getEnemy()) {
                Json::Value ennemiJson;
                ennemiJson["Nom"] = pair.second->getNom();
                ennemiJson["HP"] = pair.second->getHp();
                ennemiJson["AP"] = pair.second->getAp();
                ennemiJson["DP"] = pair.second->getDp();
                ennemisJson.append(ennemiJson);
            }
            response["Ennemis"] = ennemisJson;
            return response;
        } else if(root["Afficher"] == "Planetes"){
            Json::Value planetesJson(Json::arrayValue);
            for (const auto& pair : controller->getPlanetes()) {
                Json::Value planeteJson;
                planeteJson["Nom"] = pair.second->getNom();
                planeteJson["Description"] = pair.second->getDescription();
                planetesJson.append(planeteJson);
            }
            response["Planetes"] = planetesJson;
            return response;
        } else if (root["Afficher"] == "Spaceships") {
            Json::Value spaceshipsJson(Json::arrayValue);
            for (const auto& pair : controller->getSpaceships()) {
                Json::Value spaceshipJson;
                spaceshipJson["Nom"] = pair.second->getNom();
                spaceshipsJson.append(spaceshipJson);
            }
            response["Spaceships"] = spaceshipsJson;
            return response;
        } else if (root["Afficher"] == "Missions") {
            Json::Value missionsJson(Json::arrayValue);
            for (const auto& pair : controller->getMissions()) {
                Json::Value missionJson;
                if (!pair.second->getComplete()) {
                    missionJson["Nom"] = pair.second->getNom();
                    missionJson["Description"] = pair.second->getDescription();
                }
                missionsJson.append(missionJson);
            }
            response["AvailableMissions"] = missionsJson;
            return response;
        } else if (root["Afficher"] == "Items") {
            Json::Value itemsJson(Json::arrayValue);
            for (const auto& pair : controller->getItems()) {
                Json::Value itemJson;
                itemJson["Nom"] = pair.second->getName();
                itemJson["Description"] = pair.second->getDescription();
                itemJson["Effect"] = pair.second->getEffect();
                itemsJson.append(itemJson);
            }
            response["Items"] = itemsJson;
            return response;
        }
    } else if(key == "Creer"){
        if(root["Creer"] == "Personnages"){
            if (root.isMember("personnages") && root["personnages"].isArray()) {
                for (const auto& personnageData : root["personnages"]) {
                    std::string nom = personnageData["nom"].asString();
                    std::string description = personnageData["description"].asString();
                    int hp = personnageData["hp"].asInt();
                    int ap = personnageData["ap"].asInt();
                    int dp = personnageData["dp"].asInt();

                    std::shared_ptr<Personnage> personnage = std::make_shared<Personnage>(nom, description, hp, ap, dp);
                    personnage->setLocation(personnageData["nomLieu"].asString());
                    if(personnageData["lieu"].asString() == "Spaceship"){
                        controller->getSpaceships()[personnageData["nomLieu"].asString()]->ajouteEquipage(personnage);
                    } else if(personnageData["lieu"].asString() == "Planete"){
                        controller->getPlanetes()[personnageData["nomLieu"].asString()]->setHabitant(personnage);
                    }
                    controller->addPersonnage(personnage);
                }
            }
            response["Data"] = "Received";
            return response;
        } else if(root["Creer"] == "Planetes"){
            if (root.isMember("Planetes") && root["Planetes"].isArray()) {
                for (const auto& planeteData : root["Planetes"]) {
                    std::string nom = planeteData["nom"].asString();
                    std::string description = planeteData["description"].asString();

                    std::shared_ptr<Planet> planete = std::make_shared<Planet>(nom, description);
                    controller->addPlanete(planete);
                }
            }
            response["Data"] = "Received";
            return response;
        } else if(root["Creer"] == "Spaceships"){
            if (root.isMember("Spaceships") && root["Spaceships"].isArray()) {
                for (const auto& spaceshipData : root["Spaceships"]) {
                    std::string nom = spaceshipData["nom"].asString();
                    std::shared_ptr<Spaceship> spaceship = std::make_shared<Spaceship>(nom);
                    controller->addSpaceship(spaceship);
                }
            }
            response["Data"] = "Received";
            return response;
        } else if(root["Creer"] == "Missions"){
            if (root.isMember("Missions") && root["Missions"].isArray()) {
                for (const auto& missionData : root["Missions"]) {
                    std::string nom = missionData["nom"].asString();
                    std::string description = missionData["description"].asString();
                    std::shared_ptr<Mission> mission = std::make_shared<Mission>(nom, description);
                    controller->addMission(mission);
                }
            }
            response["Data"] = "Received";
            return response;
        } else if(root["Creer"] == "Items"){
            if (root.isMember("Items") && root["Items"].isArray()) {
                for (const auto& itemData : root["Items"]) {
                    std::string nom = itemData["nom"].asString();
                    std::string description = itemData["description"].asString();
                    int effect = itemData["effect"].asInt();
                    controller->addItem(nom, description, effect);
                }
            }
            response["Data"] = "Received";
            return response;
        }   
    } else if(key == "Localisation"){
        std::string personnage = root["Localisation"].asString();
        response["Place"] = controller->getPersonnages()[personnage]->getLocation();
        return response;
    } else if(key == "Deplacement"){
        std::string personnage = root["Deplacement"]["Personnage"].asString();
        std::string place = root["Deplacement"]["Place"].asString();
        
        controller->move(personnage, place);
        response["Place"] = controller->getPersonnages()[personnage]->getLocation();
        return response;
    } else if(key == "UseItem"){
        std::string personnage = root["UseItem"]["personnage"].asString();
        std::string item = root["UseItem"]["item"].asString();

        controller->useItem(personnage, item);
        
        response["personnage"] = personnage;
        response["hp"] = controller->getPersonnages()[personnage]->getHp();
        return response;
    } else if(key == "PrendreItem"){
        std::string personnage = root["PrendreItem"]["personnage"].asString();
        std::string item = root["PrendreItem"]["item"].asString(); 
        
        controller->addUnusedItem(personnage, item);
        Json::Value itemsJson;
        for (const auto& item : controller->getPersonnages()[personnage]->getItems()) {
            itemsJson[item.first] = item.second->getName();
        }
        response["Items"] = itemsJson;
        return response;
    } else if (key == "AfficherItems") {
        std::string personnage = root["AfficherItems"].asString();
        Json::Value itemsJson;
        for (const auto& item : controller->getPersonnages()[personnage]->getItems()) {
            itemsJson[item.first] = item.second->getName();
        }
        response["Items"] = itemsJson;
        return response;
    } else if (key == "Save") {
        response["response"] = controller->save();
        return response;
    } else if (key == "AfficherPersonnage") {
        std::string personnage = root["AfficherPersonnage"].asString();
        response["Nom"] = controller->getPersonnages()[personnage]->getNom();
        response["HP"] = controller->getPersonnages()[personnage]->getHp();
        response["AP"] = controller->getPersonnages()[personnage]->getAp();
        response["DP"] = controller->getPersonnages()[personnage]->getDp();
        response["Localisation"] = controller->getPersonnages()[personnage]->getLocation();
        response["Nombre items utilises"] = controller->getPersonnages()[personnage]->get_nb_Items_used();
        response["Nombre items ramases"] = controller->getPersonnages()[personnage]->get_nb_Items_added();
        return response;
    }

    response["error"] = "Error no corresponding JSON key";
    return response;
}

Serveur::~Serveur() {
}













/*

{
    "Deplacement":{
        "Personnage": "Alex Starborn",
        "Place": "Terre"
    }
}

{
  "Localisation": "Alex Starborn"
}

*/
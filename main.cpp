#include "Serveur.h"

#include <iostream>

int main() {

    Controller controller;

    controller.initalisation();
    
    Serveur server(&controller);
    server.run();

    return 0;
}

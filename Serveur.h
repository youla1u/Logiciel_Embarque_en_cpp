#ifndef SERVEUR_H
#define SERVEUR_H

#include "Controller.h"

struct ConnectionData {
    int* conn_id;
    Controller* controller;
};

class Serveur
{
    public:

        // CONSTRUCTOR
        Serveur(Controller* controller);


        // METHOD
        int run();

        
        virtual ~Serveur();

    protected:

    private:
        Controller* controller;
};

#endif // SERVEUR_H

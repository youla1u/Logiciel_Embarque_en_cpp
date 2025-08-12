#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include "Personnage.h"
#include "Enemy.h"

class Spaceship
{
    public:

        // CONSTRUCTOR
        Spaceship(std::string nom);

        // GETTER
        std::string getNom();
        std::vector<std::weak_ptr<Personnage>> getEquipage();

        // METHOD
        template <typename T>
        void ajouteEquipage(const std::shared_ptr<T> &membre);
        bool estDansEquipage(std::shared_ptr<Personnage>& element);
        bool estDansEquipage(std::shared_ptr<Enemy>& element);
        void majEquipage();
        void deleteHabitant(std::string personnage);

    protected:

    private:
        std::string nom;
        std::vector<std::weak_ptr<Personnage>> equipage;
};

#endif // SPACESHIP_H

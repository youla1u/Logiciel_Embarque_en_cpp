#ifndef PLANET_H
#define PLANET_H

#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include "Personnage.h"
#include "Enemy.h"

class Planet
{
    public:

        // CONSTRUCTOR
        Planet(std::string nom, std::string description);

        // GETTER
        std::string getNom() const;
        std::string getDescription() const;
        std::vector<std::weak_ptr<Personnage>> getHabitants();

        //SETTER
        template <typename T>
        void setHabitant(const std::shared_ptr<T>& habitant);

        // METHOD
        void majHabitants();
        bool estHabitant(std::shared_ptr<Personnage>& habitant);
        bool estHabitant(std::shared_ptr<Enemy>& habitant);
        void deleteHabitant(std::string personnage);

        virtual ~Planet();

    protected:

    private:
        std::string nom;
        std::string description;
        std::vector<std::weak_ptr<Personnage>> habitants;
};

#endif // PLANET_H

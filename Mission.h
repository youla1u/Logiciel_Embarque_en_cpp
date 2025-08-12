#ifndef MISSION_H
#define MISSION_H

#include <string>
#include <iostream>

class Mission
{
    public:

        // CONSTRUCTOR
        Mission(std::string nom, std::string description);

        // GETTER
        std::string getNom() const;
        std::string getDescription() const;
        bool getComplete();

        // SETTER
        void setComplete(bool);

        // METHOD
        void afficherMission();

        virtual ~Mission();

    protected:

    private:
        std::string nom;
        std::string description;
        bool complete;
};

#endif // MISSION_H

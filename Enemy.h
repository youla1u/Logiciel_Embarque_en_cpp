#ifndef ENEMY_H
#define ENEMY_H

#include <memory>

#include "Personnage.h"

class Enemy : public Personnage {
public:
    // CONSTRUCTOR
    Enemy(std::string nom, std::string description, int hp, int ap, int dp);

    // METHOD

};

#endif // ENEMY_H

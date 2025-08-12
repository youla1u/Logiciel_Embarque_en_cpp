#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <memory>

class Item
{
    public:

        Item(std::string name, std::string description, int effect);

        //GETTER
        std::string getName();
        std::string getDescription();
        int getEffect();

    protected:

    private:
        std::string name;
        std::string description;
        int effect; // Un chiffre repr�sentant l'effet de l'objet (par exemple, les points de vie r�cup�r�s)
};

#endif // ITEM_H

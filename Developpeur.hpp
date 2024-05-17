#pragma once
#include <string>
#include <iostream>
#include "ListeJeux.hpp"
#include "Jeu.hpp"

class Developpeur
{
public:
    Developpeur(const std::string& nomDev) : paireNomJeux_(nomDev, ListeJeux{}) {}
    ~Developpeur() { delete[] paireNomJeux_.second.elements; }

    std::string getNom() const { return paireNomJeux_.first; }

    size_t compterJeux(const ListeJeux& listeJeux) const
    {
        size_t count = 0;
        for (size_t i = 0; i < listeJeux.nElements; ++i)
        {
            if (listeJeux.elements[i]->developpeur == paireNomJeux_.first)
            {
                count++;
            }
        }
        return count;
    }

    void mettreAJourListeJeux(const ListeJeux& tousLesJeux)
    {
        delete[] paireNomJeux_.second.elements;
        paireNomJeux_.second = ListeJeux{};

        size_t count = compterJeux(tousLesJeux);
        paireNomJeux_.second.elements  = new Jeu * [count];
        paireNomJeux_.second.nElements = 0;
        paireNomJeux_.second.capacite  = count;

        for (size_t i = 0; i < tousLesJeux.nElements; ++i)
        {
            if (tousLesJeux.elements[i]->developpeur == paireNomJeux_.first)
            {
                if (paireNomJeux_.second.nElements < paireNomJeux_.second.capacite)
                {
                    paireNomJeux_.second.elements[paireNomJeux_.second.nElements++] = tousLesJeux.elements[i];
                }
            }
        }
    }

    void afficherJeux() const
    {
        for (size_t i = 0; i < paireNomJeux_.second.nElements; ++i)
        {
            std::cout << paireNomJeux_.second.elements[i]->titre << std::endl;
        }
    }

private:
    std::pair<std::string, ListeJeux> paireNomJeux_;
};
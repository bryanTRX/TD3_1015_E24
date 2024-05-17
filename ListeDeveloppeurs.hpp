#pragma once
#include <cstddef>
#include <string>
#include "Developpeur.hpp"

class ListeDeveloppeurs
{
public:

    ListeDeveloppeurs() : nElements(0), capacite(1), elements(new Developpeur* [1]) {}
    ~ListeDeveloppeurs()
    {
        for (size_t i = 0; i < nElements; ++i)
        {
            delete elements[i];
        }
        delete[] elements;
    }

    void afficher() const
    {
        for (size_t i = 0; i < nElements; ++i)
        {
            elements[i]->afficherJeux();
        }
    }

    void ajouterDeveloppeur(Developpeur* developpeur)
    {
        if (trouverDeveloppeur(developpeur->getNom()) == nullptr)
        {
            if (nElements >= capacite)
            {
                augmenterCapacite();
            }
            elements[nElements++] = developpeur;
        }
    }

    void retirerDeveloppeur(Developpeur* developpeur)
    {
        for (size_t i = 0; i < nElements; ++i)
        {
            if (elements[i] == developpeur)
            {
                elements[i] = elements[--nElements];
                return;
            }
        }
    }

    Developpeur* trouverDeveloppeur(const std::string& nom) const
    {
        for (size_t i = 0; i < nElements; ++i)
        {
            if (elements[i]->getNom() == nom)
            {
                return elements[i];
            }
        }
        return nullptr;
    }

    void augmenterCapacite()
    {
        capacite *= 2;
        Developpeur** nouvelleListe = new Developpeur * [capacite];

        for (size_t i = 0; i < nElements; ++i)
        {
            nouvelleListe[i] = elements[i];
        }
        delete[] elements;
        elements = nouvelleListe;
    }

private:
    size_t nElements, capacite;
    Developpeur** elements;
};
#pragma once
#include <cstddef>
#include <string>
#include "Developpeur.hpp"
#include <memory> // Ajout pour les pointeurs intelligents

using namespace std;
using namespace gsl;

class ListeDeveloppeurs
{
public:
    ListeDeveloppeurs() : nElements(0), capacite(1), elements(new std::shared_ptr<Developpeur>[1]) {}
    ~ListeDeveloppeurs() = default; // Pas besoin de définir le destructeur explicitement

    void afficher() const
    {
        for (size_t i = 0; i < nElements; ++i)
        {
            elements[i]->afficherJeux();
        }
    }

    void ajouterDeveloppeur(const std::shared_ptr<Developpeur>& developpeur)
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

    void retirerDeveloppeur(const std::shared_ptr<Developpeur>& developpeur)
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

    std::shared_ptr<Developpeur> trouverDeveloppeur(const std::string& nom) const
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
        auto nouvelleListe = std::make_unique<std::shared_ptr<Developpeur>[]>(capacite); // Utilisation de make_unique

        for (size_t i = 0; i < nElements; ++i)
        {
            nouvelleListe[i] = elements[i];
        }
        elements = std::move(nouvelleListe); // Utilisation de move pour transférer la propriété du pointeur
    }

private:
    size_t nElements, capacite;
    std::unique_ptr<std::shared_ptr<Developpeur>[]> elements; // Utilisation de unique_ptr pour la gestion automatique de la mémoire
};
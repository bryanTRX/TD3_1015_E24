﻿#pragma once

#include <string>
#include <utility>
#include "Jeu.hpp"
#include "Liste.hpp"

class Jeu;

using ListeJeux = Liste<Jeu>;

class Developpeur
{
public:
    Developpeur(const std::string& nom = "");
    ~Developpeur();

    void ajouterJeux(Liste<Jeu>& listeJeux);
    const std::string& getNom() const { return paireNomJeux_.first; }

    void afficher() const;

private:
    unsigned compterJeuxDeveloppes(Liste<Jeu>& listeJeux);

    std::pair<std::string, ListeJeux> paireNomJeux_;
};

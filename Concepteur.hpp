#ifndef CONCEPTEUR_HPP
#define CONCEPTEUR_HPP

#include <string>
#include <vector>
#include <memory>

// Déclaration avancée de la structure Jeu
struct Jeu;

struct Concepteur
{
    std::string nom;
    int anneeNaissance;
    std::string pays;
    std::vector<std::shared_ptr<Jeu>> jeuxConcus;
};

#endif // CONCEPTEUR_HPP
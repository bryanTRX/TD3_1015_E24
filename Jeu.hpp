#ifndef JEU_HPP
#define JEU_HPP

#include <string>
#include <vector>
#include <memory>
#include "Concepteur.hpp"

struct Jeu
{
    std::string titre;
    int anneeSortie;
    std::string developpeur;
    std::vector<std::shared_ptr<Concepteur>> concepteurs;
};

#endif // JEU_HPP

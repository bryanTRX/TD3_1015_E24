#pragma once
#include <string>
#include "Concepteur.hpp"
#include "Liste.hpp"

struct Concepteur;

struct Jeu
{
	std::string titre;
	int anneeSortie;
	std::string developpeur;
	Liste<Concepteur> concepteurs;
};
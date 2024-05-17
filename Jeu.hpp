#pragma once
#include <string>
#include "ListeConcepteurs.hpp"

struct Jeu
{
	std::string titre;
	int anneeSortie;
	std::string developpeur;
	ListeConcepteurs concepteurs;
};

#pragma once
#include <string>
#include "Jeu.hpp"
#include "Liste.hpp"

class Jeu;

struct Concepteur
{
	std::string nom;
	int anneeNaissance;
	std::string pays;
	Liste<Jeu> jeuxConcus;

	friend ostream& operator<<(ostream& os, const Concepteur& concepteur)
	{
		os << "Nom: " << concepteur.nom << ", Année de naissance: " << concepteur.anneeNaissance << ", Pays: " << concepteur.pays;
		return os;
	}
};

#pragma once

#include <string>
#include "Jeu.hpp"
#include "Liste.hpp"

class Jeu;

// --------------------------------------------- Question 3 : Changer ListeJeux par Liste<Jeu> ---------------------------------------------

using ListeJeux = Liste<Jeu>;

class Concepteur
{
public:
	friend ostream& operator<<(ostream& os, const Concepteur& concepteur);

	std::string nom;
	int anneeNaissance;
	std::string pays;
	ListeJeux jeuxConcus;
};

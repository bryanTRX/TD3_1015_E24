#pragma once
#include <string>
#include "Jeu.hpp"
#include "Liste.hpp"

struct Jeu;

struct Concepteur
{
	std::string nom;
	int anneeNaissance;
	std::string pays;
	Liste<Jeu> jeuxConcus;
};

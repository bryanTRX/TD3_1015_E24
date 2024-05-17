#pragma once
#include <string>
#include "ListeJeux.hpp"

struct Concepteur
{
	std::string nom;
	int anneeNaissance;
	std::string pays;
	ListeJeux jeuxConcus;
};

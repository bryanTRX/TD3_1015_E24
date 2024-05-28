#pragma once

#include <string>
#include "Concepteur.hpp"
#include "Liste.hpp"

class Concepteur;

class Jeu
{
public:
	std::string titre;
	int anneeSortie;
	std::string developpeur;
	Liste<Concepteur> concepteurs;

	shared_ptr<Concepteur> chercherConcepteur(const std::function<bool(const shared_ptr<Concepteur>&)>& critere) const 
	{
		return concepteurs.chercherElement(critere);
	}

	friend ostream& operator<<(ostream& os, const Jeu& jeu);
};
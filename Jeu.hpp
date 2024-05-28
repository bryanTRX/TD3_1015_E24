#pragma once

#include <string>
#include "Concepteur.hpp"
#include "Liste.hpp"

class Concepteur;

// --------------------------------------------- Question 3 : Changer ListeConcepteur par Liste<Concepteur> ---------------------------------------------

using ListeConcepteurs = Liste<Concepteur>;

class Jeu
{
public:
	// Constructeurs ----------------------------------------------------------------------------

	shared_ptr<Concepteur> chercherConcepteur(const std::function<bool(const shared_ptr<Concepteur>&)>& critere) const
	{
		if (concepteurs.nElements_ == 0) {
			return nullptr; // Si la liste est vide, retourne nullptr
		}
		return concepteurs.chercherElement(critere);
	}

	friend ostream& operator<<(ostream& os, const Jeu& jeu);

	std::string titre;
	int anneeSortie;
	std::string developpeur;
	ListeConcepteurs concepteurs;
};

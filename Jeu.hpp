#pragma once
#include <string>
#include "Concepteur.hpp"
#include "Liste.hpp"

struct Concepteur;

class Jeu
{
public:
	std::string titre;
	int anneeSortie;
	std::string developpeur;
	Liste<Concepteur> concepteurs;

	template <typename Critere>
	shared_ptr<Concepteur> chercherConcepteur(Critere critere) const
	{
		return concepteurs.chercherElement(critere);
	}

	friend ostream& operator<<(ostream& os, const Jeu& jeu)
	{
		os << "Titre           : " << jeu.titre << endl;
		os << "Developpeur     : " << jeu.developpeur << endl;
		os << "Annee de sortie : " << jeu.anneeSortie << endl;

		os << "Concepteur : " << endl; 
		for (const auto& c : jeu.concepteurs.enSpan())
		{
			os << *c << endl;
		}
		return os;
	}
};
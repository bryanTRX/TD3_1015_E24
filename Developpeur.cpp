#include "Developpeur.hpp"
#include "Jeu.hpp"
#include "cppitertools/range.hpp"
#include <iostream>
#include "debogage_memoire.hpp"  //NOTE: Incompatible avec le "placement new", ne pas utiliser cette entête si vous utilisez ce type de "new" dans les lignes qui suivent cette inclusion.

using namespace std;

Developpeur::Developpeur(const string& nom) : paireNomJeux_{ nom, Liste<Jeu>() } {}

Developpeur::~Developpeur() {}

void Developpeur::ajouterJeux(Liste<Jeu>& listeJeux)
{
	paireNomJeux_.second.changerCapacite(compterJeuxDeveloppes(listeJeux));
	for (shared_ptr<Jeu>& j : spanListeJeux(listeJeux))
	{
		if (j->developpeur == getNom())
		{
			paireNomJeux_.second.ajouter(j);
		}
	}
}

void Developpeur::afficher() const
{
	cout << "\n" << getNom() << " a développé les jeux suivants :" << endl;
	if (paireNomJeux_.second.taille() > 0)
	{
		for (shared_ptr<Jeu>& j : spanListeJeux(paireNomJeux_.second))
			cout << "\t\033[33m" << j->titre << "\033[0m" << endl;
	}
	else
		cout << "\t\033[31m" << "Aucun jeu trouvé, réessayez" << "\033[0m" << endl;
}

unsigned int Developpeur::compterJeuxDeveloppes(Liste<Jeu>& listeJeux)
{
	unsigned int n = 0;
	for (shared_ptr<Jeu>& j : spanListeJeux(listeJeux))
	{
		if (j->developpeur == getNom())
			n++;
	}
	return n;
}

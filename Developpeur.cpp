#include "Developpeur.hpp"
#include "Jeu.hpp"
#include "cppitertools/range.hpp"
#include <iostream>
#include "debogage_memoire.hpp"  

using namespace std;

Developpeur::Developpeur(const string& nom) : paireNomJeux_{ nom, Liste<Jeu>() } {}

Developpeur::~Developpeur() {}

void Developpeur::ajouterJeux(Liste<Jeu>& listeJeux)
{
	paireNomJeux_.second.changerCapacite(compterJeuxDeveloppes(listeJeux));
	for (shared_ptr<Jeu>& j : listeJeux.enSpan())
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
	if (paireNomJeux_.second.nElements_ > 0)
	{
		for (shared_ptr<Jeu>& j : paireNomJeux_.second.enSpan()) 
			cout << "\t\033[33m" << j->titre << "\033[0m" << endl;
	}
	else
		cout << "\t\033[31m" << "Aucun jeu trouvé, réessayez" << "\033[0m" << endl;
}

unsigned int Developpeur::compterJeuxDeveloppes(Liste<Jeu>& listeJeux)
{
	unsigned int n = 0;
	for (shared_ptr<Jeu>& j : listeJeux.enSpan())
	{
		if (j->developpeur == getNom())
			n++;
	}
	return n;
}

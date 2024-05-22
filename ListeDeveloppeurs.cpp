#include "ListeDeveloppeurs.hpp"
#include <iostream>
#include <algorithm>
#include "cppitertools/range.hpp"
#include "debogage_memoire.hpp"  //NOTE: Incompatible avec le "placement new", ne pas utiliser cette entête si vous utilisez ce type de "new" dans les lignes qui suivent cette inclusion.
using namespace std;

ListeDeveloppeurs::~ListeDeveloppeurs()
{
	delete[] elements_;
}

void ListeDeveloppeurs::afficher() const
{
	for (const Developpeur* d : enSpan())
		d->afficher();
}

void ListeDeveloppeurs::ajouter(Developpeur* d)
{
	if (nElements_ == capacite_)
		changerCapacite(max(size_t(1), capacite_ * 2));
	elements_[nElements_++] = d;
}

void ListeDeveloppeurs::retirer(const Developpeur* aRetirer)
{
	for (Developpeur*& d : enSpan()) {
		if (d == aRetirer) {
			if (nElements_ > 1)
				d = elements_[nElements_ - 1];
			nElements_--;
		}
	}
}

void ListeDeveloppeurs::changerCapacite(size_t nouvelleCapacite)
{
	// Copie du code de changerTailleListeJeux, ajusté pour la classe.
	assert(nouvelleCapacite >= nElements_); // On ne demande pas de supporter les réductions de nombre d'éléments.
	Developpeur** nouvelleListe = new Developpeur * [nouvelleCapacite];
	// Pas nécessaire de tester si liste.elements est nullptr puisque si c'est le cas, nElements est nécessairement 0.
	for (size_t i : iter::range(nElements_))
		nouvelleListe[i] = elements_[i];
	delete[] elements_;

	elements_ = nouvelleListe;
	capacite_ = nouvelleCapacite;
}

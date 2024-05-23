//#pragma once
//#include <cstddef>
//#include "Developpeur.hpp"
//#include "gsl/span"  // On utilise encode gsl::span au lieu de std::span car c'est la version de span disponible à l'examen.
//
//class ListeDeveloppeurs
//{
//	//TODO: En faire une classe qui suit les principes OO.
//	//TODO: On veut pouvoir ajouter et enlever un Developpeur* de la liste, avec réallocation dynamique tel que faite pour ListeJeux.
//	//NOTE: Le code sera principalement copié de certaines fonctions écrites pour la partie 1, mais mises dans une classe.
//public:
//	ListeDeveloppeurs() = default;
//	~ListeDeveloppeurs();
//
//	void ajouter(Developpeur* d);
//	void retirer(const Developpeur* aRetirer);
//
//	void afficher() const;
//
//private:
//	void changerCapacite(std::size_t nouvelleCapacite);  // Pas dit si ça doit être public ou non.
//	gsl::span<Developpeur*> enSpan() const { return { elements_, nElements_ }; }  // Pourrait être public.
//
//	std::size_t nElements_ = 0, capacite_ = 0;  // Pas besoin de déclarer explicitement un corps de constructeur avec ces initialisations.
//	Developpeur** elements_ = nullptr;
//};

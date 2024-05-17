/**
* Programme contenant les exercices du TD2.
* \file		td2.cpp
* \author	Iliass Khider et Bryan Alexandre Tavares 
* \date		14 mai 2024
* Créé le	19 mai 2024
*/

#include "Jeu.hpp"
#include "ListeDeveloppeurs.hpp"
#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <limits>
#include <algorithm>

#include "cppitertools/range.hpp"
#include "gsl/span"
#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp"
#include "debogage_memoire.hpp"  //NOTE: Incompatible avec le "placement new", ne pas utiliser cette entête si vous utilisez ce type de "new" dans les lignes qui suivent cette inclusion.

using namespace std;
using namespace iter;
using namespace gsl;

#pragma region "Fonctions de base pour vous aider"
template <typename T>
T lireType(istream& fichier)
{
	T valeur{};
	fichier.read(reinterpret_cast<char*>(&valeur), sizeof(valeur));
	return valeur;
}
#define erreurFataleAssert(message) assert(false&&(message)),terminate()
static const uint8_t enteteTailleVariableDeBase = 0xA0;
size_t lireUintTailleVariable(istream& fichier)
{
	uint8_t entete = lireType<uint8_t>(fichier);
	switch (entete) {
	case enteteTailleVariableDeBase+0: return lireType<uint8_t>(fichier);
	case enteteTailleVariableDeBase+1: return lireType<uint16_t>(fichier);
	case enteteTailleVariableDeBase+2: return lireType<uint32_t>(fichier);
	default:
		erreurFataleAssert("Tentative de lire un entier de taille variable alors que le fichier contient autre chose à cet emplacement.");
	}
}

string lireString(istream& fichier)
{
	string texte;
	texte.resize(lireUintTailleVariable(fichier));
	fichier.read((char*)&texte[0], streamsize(sizeof(texte[0])) * texte.length());
	return texte;
}
gsl::span<Jeu*> spanListeJeux(const ListeJeux& liste)
{
	return gsl::span(liste.elements, liste.nElements);
}
gsl::span<Concepteur*> spanListeConcepteurs(const ListeConcepteurs& liste)
{
	return gsl::span(liste.elements, liste.nElements);
}
#pragma endregion

Concepteur* trouverConcepteurParNom(const string& nom, const ListeJeux& listeJeux)
{
	for (const Jeu* jeu : spanListeJeux(listeJeux)) 
	{
		for (Concepteur* concepteur : spanListeConcepteurs(jeu->concepteurs)) 
		{
			if (concepteur->nom == nom) {
				return concepteur;
			}
		}
	}
	return nullptr;
}

Concepteur* lireConcepteur(istream& fichier, const ListeJeux& listeJeux)
{
	Concepteur concepteur     = {}; 
	concepteur.nom            = lireString(fichier);
	concepteur.anneeNaissance = int(lireUintTailleVariable(fichier));
	concepteur.pays           = lireString(fichier);

	Concepteur* concepteurExistant = trouverConcepteurParNom(concepteur.nom, listeJeux);
	Concepteur* nouveauConcepteur  = new Concepteur(concepteur);
	
	if (concepteurExistant == nullptr)
	{
		cout << "Nouveau Concepteur  : " << concepteur.nom << endl;
		return nouveauConcepteur;
	}

	else
	{
		cout << "Concepteur Existant : " << concepteur.nom << endl;
		delete nouveauConcepteur;
		return concepteurExistant;
	}
}

void changerTailleListeJeux(size_t nouvelleCapacite, ListeJeux& listeJeux)
{
    Jeu** nouveauTableau = new Jeu * [nouvelleCapacite];
	copy(listeJeux.elements, listeJeux.elements + listeJeux.nElements, nouveauTableau);

    delete[] listeJeux.elements;

    listeJeux.elements = nouveauTableau;
    listeJeux.capacite = nouvelleCapacite;
}

void ajouterJeuListeJeux(Jeu* jeu, ListeJeux& listeJeux)
{
	if (listeJeux.nElements >= listeJeux.capacite)
	{
		size_t nouvelleCapacite = (listeJeux.capacite == 0) ? 1 : listeJeux.capacite * 2;
		changerTailleListeJeux(nouvelleCapacite, listeJeux);
	}

	listeJeux.elements[listeJeux.nElements++] = jeu;
}

void enleverJeuListeJeux(Jeu* jeu, ListeJeux& listeJeux)
{
	auto it = find(listeJeux.elements, listeJeux.elements + listeJeux.nElements, jeu);

	if (it != listeJeux.elements + listeJeux.nElements)
	{
		*it = listeJeux.elements[listeJeux.nElements - 1];
		listeJeux.nElements--;
		return;
	}

	else
	{
		cout << "Le jeu n'a pas été trouvé dans la liste." << endl;
	}
}

Jeu* lireJeu(istream& fichier, const ListeJeux& listeJeux)
{
	Jeu jeu = {}; 
	jeu.titre = lireString(fichier);
	jeu.anneeSortie = int(lireUintTailleVariable(fichier));
	jeu.developpeur = lireString(fichier);
	jeu.concepteurs.nElements = lireUintTailleVariable(fichier);
	jeu.concepteurs.elements = new Concepteur * [jeu.concepteurs.nElements];
	Jeu* nouveauJeu = new Jeu(jeu);

	cout << "\n\033[32m═════════════════\033[0m " << nouveauJeu->titre << " \033[32m═════════════════\033[0m\n";
	cout << "Nouvel jeu créé     : " << nouveauJeu->titre << endl;
	
	for ([[maybe_unused]] size_t i : iter::range(jeu.concepteurs.nElements))
	{
		Concepteur* concepteur = lireConcepteur(fichier, listeJeux); 
		jeu.concepteurs.elements[i] = concepteur;
		ajouterJeuListeJeux(nouveauJeu, concepteur->jeuxConcus);
	}

	return nouveauJeu;
}

ListeJeux creerListeJeux(const string& nomFichier)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);
	size_t nElements = lireUintTailleVariable(fichier);
	ListeJeux listeJeux = {};

	for([[maybe_unused]] size_t n : iter::range(nElements))
	{
		ajouterJeuListeJeux(lireJeu(fichier, listeJeux), listeJeux);
		changerTailleListeJeux(listeJeux.nElements, listeJeux);
	}

	return listeJeux;
}

void detruireConcepteur(Concepteur* concepteur)
{
	if (concepteur != nullptr)
	{
		cout << "Concepteur detruit : " << concepteur->nom << endl;
		delete[] concepteur->jeuxConcus.elements;
		delete concepteur;
	}

	else
	{
		cout << "Erreur : Tentative de destruction d'un concepteur nul." << endl;
	}
}

bool concepteurParticipeJeu(Concepteur* concepteur, Jeu* jeu) 
{
	for (size_t i = 0; i < concepteur->jeuxConcus.nElements; ++i) 
	{
		if (concepteur->jeuxConcus.elements[i] == jeu)
		{
			return true;
		}
	}
	return false;
}

void detruireJeu(Jeu* jeu)
{
	if (jeu != nullptr) 
	{
		cout << "\n\033[31m═════════════════\033[0m " << jeu->titre << " \033[31m═════════════════\033[0m\n";
		cout << "Jeu supprime       : " << jeu->titre << endl;

		for (size_t i = 0; i < jeu->concepteurs.nElements; ++i)
		{
			Concepteur* concepteur = jeu->concepteurs.elements[i];
			if (concepteur != nullptr)
			{
				enleverJeuListeJeux(jeu, concepteur->jeuxConcus);
			}
		}

		for (size_t j = 0; j < jeu->concepteurs.nElements; ++j)
		{
			Concepteur* concepteur = jeu->concepteurs.elements[j];
			if (concepteur != nullptr && !concepteurParticipeJeu(concepteur, jeu) && concepteur->jeuxConcus.nElements == 0)
			{
				detruireConcepteur(concepteur);
			}
		}

		delete[] jeu->concepteurs.elements;
		jeu->concepteurs.elements  = nullptr;
		jeu->concepteurs.nElements = 0;

		delete jeu;
	}
}

void detruireListeJeux(ListeJeux& listeJeux)
{
	for (size_t i = 0; i < listeJeux.nElements; ++i)
	{
		detruireJeu(listeJeux.elements[i]);
	}

	delete[] listeJeux.elements;
	listeJeux.elements = nullptr;
	listeJeux.nElements = 0;
}

void afficherConcepteur(const Concepteur& concepteur)
{
	cout << "\t" << concepteur.nom << ", " << concepteur.anneeNaissance << ", " << concepteur.pays << endl;
}

void afficherInfosJeu(const Jeu& jeu)
{
	cout << "Titre           : " << jeu.titre << endl;
	cout << "Developpeur     : " << jeu.developpeur << endl;
	cout << "Annee de sortie : " << jeu.anneeSortie << endl;

	cout << "Concepteurs : " << endl;
	for (size_t i = 0; i < jeu.concepteurs.nElements; ++i)
	{
		afficherConcepteur(*(jeu.concepteurs.elements[i]));
	}
}

void afficherTousJeux(const ListeJeux& listeJeux)
{
	static const string ligneDeSeparation = "\n\033[34m════════════════════════════════════════════════════════\033[0m\n";
	cout << ligneDeSeparation;
	
	for (auto&& jeu : spanListeJeux(listeJeux))
	{
		afficherInfosJeu(*jeu);
		cout << ligneDeSeparation;
	}
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
#pragma region "Bibliothèque du cours"
	// Permet sous Windows les "ANSI escape code" pour changer de couleur
	// https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac
	// les supportent normalement par défaut.
	bibliotheque_cours::activerCouleursAnsi();
#pragma endregion

	int* fuite = new int;

	static const string lignePartie1Separation = "\n\033[35m════════════════════════ PARTIE 1 ═════════════════════════\033[0m";
	static const string lignePartie2Separation = "\n\033[35m════════════════════════ PARTIE 2 ═════════════════════════\033[0m";

	cout << lignePartie1Separation << endl;
	cout << endl << "Création de la liste de jeux:";
	ListeJeux listeJeux = creerListeJeux("jeux.bin");

	static const string ligneDeSeparation = "\n\033[34m════════════════════════════════════════════════════════\033[0m\n";
	cout << endl << "Premier jeu de la liste : ";
	cout << ligneDeSeparation;
	afficherInfosJeu(*listeJeux.elements[0]);

	cout << endl << "Les jeux sont:";
	afficherTousJeux(listeJeux);

	cout << lignePartie2Separation << endl;
	ListeDeveloppeurs listeDeveloppeurs;

	Developpeur* dev1 = new Developpeur("FromSoftware");
	Developpeur* dev2 = new Developpeur("MicroProse");

	listeDeveloppeurs.ajouterDeveloppeur(dev1);
	listeDeveloppeurs.ajouterDeveloppeur(dev2);

	dev1->mettreAJourListeJeux(listeJeux);
	dev2->mettreAJourListeJeux(listeJeux);

	cout << "\nJeux du développeur FromSoftware : ";
	dev1->afficherJeux();
	cout << "Jeux du développeur MicroProse : ";
	dev2->afficherJeux();

	cout << ligneDeSeparation;
	cout << "Liste de leurs jeux : " << endl;
	listeDeveloppeurs.afficher();
	cout << ligneDeSeparation;

	listeDeveloppeurs.retirerDeveloppeur(dev1);
	cout << "Liste des développeurs après suppression de FromSoftware : " << endl;
	listeDeveloppeurs.afficher();
	
	cout << ligneDeSeparation;
	cout << "Vérification de l'existence des développeurs dans la liste : \n";
	cout << "FromSoftware est " << (listeDeveloppeurs.trouverDeveloppeur("FromSoftware") ? "présent" : "absent") << " dans la liste." << endl;
	cout << "MicroProse est " << (listeDeveloppeurs.trouverDeveloppeur("MicroProse") ? "présent" : "absent") << " dans la liste." << endl;
	
	cout << lignePartie1Separation << endl;
	cout << endl << "Destruction de la liste de jeux:";
	detruireListeJeux(listeJeux);

	delete dev1; 

	delete fuite;
}
/**
* Programme contenant le projet du TD3.
* \file		main.cpp
* \author	Iliass Khider et Bryan Alexandre Tavares
* \date		21 mai 2024
* Créé le	27 mai 2024
*/

#include "Jeu.hpp"
#include "Developpeur.hpp"
#include "Concepteur.hpp"
#include "Liste.hpp"
#include <iostream>
#include <fstream>
#include <cstdint>
#include <cassert>
#include "cppitertools/range.hpp"
#include "gsl/span"
#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp"
#include "ListeDeveloppeurs.hpp"
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
	case enteteTailleVariableDeBase + 0: return lireType<uint8_t>(fichier);
	case enteteTailleVariableDeBase + 1: return lireType<uint16_t>(fichier);
	case enteteTailleVariableDeBase + 2: return lireType<uint32_t>(fichier);
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
#pragma endregion

shared_ptr<Concepteur> trouverConcepteur(const Liste<Jeu>& listeJeux, const string& nom)
{
	auto critere = [&](const shared_ptr<Concepteur>& concepteur) {
		return concepteur->nom == nom;
		};

	for (const shared_ptr<Jeu>& jeu : listeJeux.enSpan())
	{
		auto concepteur = jeu->chercherConcepteur(critere);
		if (concepteur)
		{
			return concepteur;
		}
	}

	return nullptr;
}

shared_ptr<Concepteur> lireConcepteur(istream& fichier, const  Liste<Jeu>& listeJeux)
{
	Concepteur concepteur = {};
	concepteur.nom = lireString(fichier);
	concepteur.anneeNaissance = int(lireUintTailleVariable(fichier));
	concepteur.pays = lireString(fichier);

	shared_ptr<Concepteur> concepteurExistant = trouverConcepteur(listeJeux, concepteur.nom);
	shared_ptr<Concepteur> nouveauConcepteur = make_shared<Concepteur>(concepteur);

	if (!concepteurExistant)
	{
		cout << "Nouveau Concepteur  : " << concepteur.nom << endl;
		return nouveauConcepteur;
	}
	else
	{
		cout << "Concepteur Existant : " << concepteur.nom << endl;
		return concepteurExistant;
	}
}

shared_ptr<Jeu> lireJeu(istream& fichier, Liste<Jeu>& listeJeux)
{
	Jeu jeu = {};
	jeu.titre = lireString(fichier);
	jeu.anneeSortie = int(lireUintTailleVariable(fichier));
	jeu.developpeur = lireString(fichier);
	size_t nConcepteurs = lireUintTailleVariable(fichier);

	shared_ptr<Jeu> nouveauJeu = make_shared<Jeu>(jeu);

	cout << "\n\033[32m═════════════════\033[0m " << "\033[32m" << nouveauJeu->titre << "\033[0m" << " \033[32m═════════════════\033[0m\n";
	cout << "Nouveau jeu créé    : " << nouveauJeu->titre << endl;

	nouveauJeu->concepteurs.changerCapacite(nConcepteurs); 
	for (size_t i = 0; i < nConcepteurs; ++i)
	{
		shared_ptr<Concepteur> concepteur = lireConcepteur(fichier, listeJeux);
		nouveauJeu->concepteurs.ajouter(concepteur);
		concepteur->jeuxConcus.ajouter(nouveauJeu);
	}
	return nouveauJeu; 
}

Liste<Jeu> creerListeJeux(const string& nomFichier)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);
	size_t nElements = lireUintTailleVariable(fichier);
	Liste<Jeu> listeJeux;

	for ([[maybe_unused]] size_t n : iter::range(nElements))
	{
		listeJeux.ajouter(lireJeu(fichier, listeJeux));
	}

	return listeJeux;
}

void detruireConcepteur(shared_ptr<Concepteur> concepteur)
{
	if (concepteur)
	{
		cout << "Concepteur détruit : " << concepteur->nom << endl;
	}

	else
	{
		cout << "Erreur : Tentative de destruction d'un concepteur nul." << endl;
	}
}

bool encorePresentDansUnJeu(shared_ptr<Concepteur> concepteur)
{
	return concepteur->jeuxConcus.taille() != 0;
}

void detruireJeu(shared_ptr<Jeu> jeu)
{
	cout << "\n\033[31m═════════════════\033[0m " << "\033[31m" << jeu->titre << "\033[0m" << " \033[31m═════════════════\033[0m\n";
	cout << "Jeu supprime       : " << jeu->titre << endl;

	for (shared_ptr<Concepteur> c : jeu->concepteurs.enSpan())
	{
		c->jeuxConcus.retirer(jeu);
		if (!encorePresentDansUnJeu(c))
		{
			detruireConcepteur(c);
		}
	}
}

void detruireListeJeux(Liste<Jeu>& liste)
{
	for (shared_ptr<Jeu> j : liste.enSpan())
	{
		detruireJeu(j);
	}
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
	for (const shared_ptr<Concepteur> c : jeu.concepteurs.enSpan())
	{
		afficherConcepteur(*c);
	}
}

void afficherListeJeux(const Liste<Jeu>& listeJeux)
{
	static const string ligneDeSeparation = "\n\033[34m════════════════════════════════════════════════════════\033[0m\n";
	cout << ligneDeSeparation;

	for (const shared_ptr<Jeu> j : listeJeux.enSpan())
	{
		afficherInfosJeu(*j);
		cout << ligneDeSeparation;
	}
}

template<typename T>
ostream& operator<<(ostream& os, const Liste<T>& liste)
{
    for (const auto& element : liste.enSpan())
    {
        os << *element << endl;
    }
    return os;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
#pragma region "Bibliothèque du cours"
	// Permet sous Windows les "ANSI escape code" pour changer de couleur
	// https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac
	// les supportent normalement par défaut.
	bibliotheque_cours::activerCouleursAnsi();
#pragma endregion

	static const string ligneDeSeparation = "\n\033[96m════════════════════════════════════════════════════════\033[0m\n";

	static const string ligneSeparationCreation = "\n\033[35m═══════════════════ Creation de ma liste de jeu ═════════════════════\033[0m\n";
	cout << ligneSeparationCreation;

	Liste<Jeu> listeJeux = creerListeJeux("jeux.bin"); 

	// --------------------------------------------- Affichage de ma liste ---------------------------------------------
	
	static const string ligneSeparationContenu= "\n\033[35m═══════════════════ Affichage du contenu de ma liste de jeu ═════════════════════\033[0m\n";
	cout << ligneSeparationContenu;

	afficherListeJeux(listeJeux);

	// --------------------------------------------- Test pour la surchage de [] ---------------------------------------------

	static const string ligneSeparationSurchage = "\n\033[35m═══════════════════ Test surchage operator[] ═════════════════════\033[0m\n";
	cout << ligneSeparationSurchage;

	cout << ligneDeSeparation;
	afficherInfosJeu(*listeJeux[2]);
	cout << ligneDeSeparation << endl;

	// --------------------------------------------- Test pour la fonction trouverConcepteur ---------------------------------------------

	static const string ligneSeparationTestFonction = "\n\033[35m═══════════════════ Test de la fonction trouverConcepteur ═════════════════════\033[0m\n";
	cout << ligneSeparationTestFonction;

	auto concepteurTrouve = trouverConcepteur(listeJeux, "Yoshinori Kitase");

	if (concepteurTrouve)
	{
		cout << ligneDeSeparation;
		cout << "Nom du concepteur rechercher : " << concepteurTrouve->nom << " et puis son année de naissance : " << concepteurTrouve->anneeNaissance << endl;
		
		if (listeJeux[0]->concepteurs[0] == listeJeux[1]->concepteurs[0])
		{
			cout << "Les deux jeux renvoient un pointeur vers la même adresse pour le concepteur.";
			cout << endl <<ligneDeSeparation;
		}
		else
		{
			cout << "Les deux jeux ne renvoient pas un pointeur vers la même adresse pour le concepteur.";
			cout << endl << ligneDeSeparation;
		}
	}
	
	else
	{
		cout << "Concepteur non trouvé.";
		cout << endl << ligneDeSeparation;
	}

	// --------------------------------------------- Affichage à l'aide du cout ---------------------------------------------

	static const string ligneSeparationCout = "\n\033[35m═══════════════════ Affichage à l'aide de la surchage operator<< ═════════════════════\033[0m\n";
	cout << endl << ligneSeparationCout;

	cout << ligneDeSeparation << listeJeux << ligneDeSeparation;

	// --------------------------------------------- Test de la copie de jeu ---------------------------------------------

	static const string ligneSeparationCopie = "\n\033[35m═══════════════════ Test de la copie de jeu ═════════════════════\033[0m\n";
	cout << endl << ligneSeparationCopie;

	shared_ptr<Jeu> jeuOriginal = listeJeux[2]; 
	shared_ptr<Jeu> copieJeu = make_shared<Jeu>(*jeuOriginal); 

	shared_ptr<Concepteur> nouveauConcepteur = listeJeux[0]->concepteurs[1];

	copieJeu->concepteurs[1] = nouveauConcepteur;

	cout << "Jeu original à l'indice 2 :" << endl;
	afficherInfosJeu(*jeuOriginal);
	cout << endl;

	cout << "Copie du jeu modifiée :" << endl;
	afficherInfosJeu(*copieJeu);
	cout << endl;

	if (jeuOriginal->concepteurs[0] == copieJeu->concepteurs[0])
	{
		cout << "Les adresses du premier concepteur dans les deux jeux sont les mêmes." << endl;
	}

	else
	{
		cout << "Les adresses du premier concepteur dans les deux jeux ne sont pas les mêmes." << endl;
	}

	// --------------------------------------------- Destruction de ma liste de jeu ---------------------------------------------

	static const string ligneSeparationDestruction = "\n\033[35m═══════════════════ Destruction de ma liste de jeu ═════════════════════\033[0m\n";
	cout << ligneSeparationDestruction;

	detruireListeJeux(listeJeux);
}

// Je n'ai pas trop compris ce que vous voulez dire dans 7) 
// Lorsqu'on fait l'affichage avec le cout est-ce vous voulez exactement le meme modele que celui du TP2 
// Les surchages d'operator<< de Jeu et Concepteur doivent egalement etre dans le main ou on peut les laisser dans le hpp 
// 
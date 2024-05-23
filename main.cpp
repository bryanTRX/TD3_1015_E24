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

shared_ptr<Concepteur> trouverConcepteur(const Liste<Jeu>& listeJeux, const string nom)
{
	for (const shared_ptr<Jeu> j : spanListeJeux(listeJeux))
	{
		for (shared_ptr<Concepteur> d : spanListeConcepteurs(j->concepteurs))
		{
			if (d->nom == nom)
				return d;
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

	cout << "\033[96m" << "Allocation en mémoire du jeu " << nouveauJeu->titre << "\033[0m" << endl;

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
	for (shared_ptr<Concepteur> c : spanListeConcepteurs(jeu->concepteurs))
	{
		c->jeuxConcus.retirer(jeu);
		if (!encorePresentDansUnJeu(c))
		{
			detruireConcepteur(c);
		}
	}

	cout << "\033[31m" << "Destruction du jeu " << jeu->titre << "\033[0m" << endl;
}

void detruireListeJeux(Liste<Jeu>& liste)
{
	for (shared_ptr<Jeu> j : spanListeJeux(liste))
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
	cout << "Titre           : " << "\033[94m" << jeu.titre << "\033[0m" << endl;
	cout << "Developpeur     : " << "\033[94m" << jeu.developpeur << "\033[0m" << endl;
	cout << "Annee de sortie : " << "\033[94m" << jeu.anneeSortie << "\033[0m" << endl;
	
	cout << "Concepteurs : " << "\033[94m" << endl;
	for (const shared_ptr<Concepteur> c : spanListeConcepteurs(jeu.concepteurs))
	{
		afficherConcepteur(*c);
	}
	cout << "\033[0m";
}

void afficherListeJeux(const Liste<Jeu>& listeJeux)
{
	static const string ligneSeparation = "\n\033[95m""══════════════════════════════════════════════════════════════════════════""\033[0m\n";
	cout << ligneSeparation;

	for (const shared_ptr<Jeu> j : spanListeJeux(listeJeux))
	{
		afficherInfosJeu(*j);
		cout << ligneSeparation;
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

	// Pour vérifier que la détection de fuites fonctionne; un message devrait dire qu'il y a une fuite à cette ligne.

	Liste<Jeu> lj = creerListeJeux("jeux.bin"); //TODO: Appeler correctement votre fonction de création de la liste de jeux.

	static const string ligneSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";
	cout << ligneSeparation << endl;

	cout << "\033[94m Test surchage operator[] : \033[0m" << endl;
	//TODO: Afficher le premier jeu de la liste (en utilisant la fonction).  Devrait être Chrono Trigger.
	afficherInfosJeu(*lj[2]);
	cout << ligneSeparation << endl;

	//TODO: Appel à votre fonction d'affichage de votre liste de jeux.
	afficherListeJeux(lj);

	//TODO: Faire les appels à toutes vos fonctions/méthodes pour voir qu'elles fonctionnent et avoir 0% de lignes non exécutées dans le programme (aucune ligne rouge dans la couverture de code; c'est normal que les lignes de "new" et "delete" soient jaunes).  Vous avez aussi le droit d'effacer les lignes du programmes qui ne sont pas exécutée, si finalement vous pensez qu'elle ne sont pas utiles.

	//Liste<Developpeur> ld;

	//// Création des développeurs avec shared_ptr
	//auto nintendo = make_shared<Developpeur>("Nintendo");
	//auto square   = make_shared<Developpeur>("Square");
	//auto konami   = make_shared<Developpeur>("Konami");
	//auto bidon    = make_shared<Developpeur>("Bidon");

	//// On ajoute les jeux respectifs de ListeJeux développé par le développeur
	//nintendo->ajouterJeux(lj);
	//square->ajouterJeux(lj);
	//konami->ajouterJeux(lj);

	//// On ajoute les développeurs à la ListeDeveloppeur car ils sont externes
	//ld.ajouter(nintendo);
	//ld.ajouter(square);
	//ld.ajouter(konami);
	//ld.ajouter(bidon);

	//// On affiche la liste des développeurs, leurs jeux sont aussi affichés; Bidon ne devrait avoir aucun jeu.
	//ld.afficher();

	//cout << endl << "On retire " << bidon->getNom() << endl;
	//ld.retirer(bidon); // Retire sans détruire.
	//ld.afficher();
	//cout << "Il existe encore: " << bidon->getNom() << endl;

	//TODO: Détruire tout avant de terminer le programme.  Devrait afficher "Aucune fuite detectee." a la sortie du programme; il affichera "Fuite detectee:" avec la liste des blocs, s'il manque des delete.
	detruireListeJeux(lj);
}
#pragma once
#include <string>
#include <iostream>
#include "Liste.hpp"
#include "Jeu.hpp"

class Developpeur
{
public:
    Developpeur(const std::string& nomDev) : nomDev_(nomDev), listeJeux_() {}
    ~Developpeur() {}

    std::string getNom() const { return nomDev_; }

    size_t compterJeux(const Liste<Jeu>& listeJeux) const
    {
        size_t count = 0;
        for (size_t i = 0; i < listeJeux.taille(); ++i)
        {
            if (listeJeux[i]->developpeur == nomDev_)
            {
                ++count;
            }
        }
        return count;
    }

    void mettreAJourListeJeux(const Liste<Jeu>& listeJeux)
    {
        for (size_t i = 0; i < listeJeux.taille(); ++i)
        {
            if (listeJeux[i]->developpeur == nomDev_)
            {
                listeJeux_.ajouter(listeJeux[i]);
            }
        }
    }

    void afficherJeux() const
    {
        for (size_t i = 0; i < listeJeux_.taille(); ++i)
        {
            std::cout << listeJeux_[i]->titre << std::endl;
        }
    }

private:
    std::string nomDev_;
    Liste<Jeu> listeJeux_;
};

//class Developpeur
//{
//public:
//    Developpeur(const std::string& nomDev) : nomDev_(nomDev) {}
//
//    std::string getNom() const { return nomDev_; }
//
//    size_t compterJeux() const { return listeJeux_.size(); }
//
//    void ajouterJeu(const std::shared_ptr<Jeu>& jeu) { listeJeux_.push_back(jeu); }
//
//    void supprimerJeu(const std::shared_ptr<Jeu>& jeu)
//    {
//        auto it = std::find(listeJeux_.begin(), listeJeux_.end(), jeu);
//        if (it != listeJeux_.end())
//        {
//            listeJeux_.erase(it);
//        }
//    }
//
//    void afficherJeux() const
//    {
//        for (const auto& jeu : listeJeux_)
//        {
//            std::cout << jeu->getTitre() << std::endl;
//        }
//    }
//
//private:
//    std::string nomDev_;
//    std::vector<std::shared_ptr<Jeu>> listeJeux_;
//};
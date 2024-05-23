#pragma once 

#include <memory>
#include <cstddef>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include "cppitertools/range.hpp"
#include "gsl/span"

using namespace std;

template <typename T>
class Liste
{
public:
	// Constructeurs ----------------------------------------------------------------------------

	Liste() : capacite_(0), nElements_(0), elements_(nullptr) {}
	Liste(size_t nElementsInitiales, size_t capaciteInitiale) : nElements_(nElementsInitiales), capacite_(capaciteInitiale), elements_(new shared_ptr<T>[capaciteInitiale]) {}
	Liste(const Liste& other) : capacite_(other.capacite_), nElements_(other.nElements_), elements_(new shared_ptr<T>[other.capacite_])
	{
		if (capacite_ != 0 && nElements_ != 0)
		{
			for (size_t elem = 0; elem < nElements_; ++elem)
			{
				elements_[elem] = other.elements_[elem];
			}
		}
	}
	~Liste() = default;

	// Methodes ---------------------------------------------------------------------------------

	void afficher() const
	{
		for (auto& element : enSpan())
		{
			element->afficher();
		}
	}

	void ajouter(const shared_ptr<T>& element)
	{
		if (nElements_ >= capacite_)
		{
			changerCapacite(max(size_t(1), capacite_ * 2));
		}
		elements_[nElements_++] = element;
	}

	void retirer(const shared_ptr<T>& aRetirer)
	{
		bool elementTrouve = false;
		for (size_t i = 0; i < nElements_; ++i)
		{
			if (elements_[i] == aRetirer)
			{
				elementTrouve = true;
				elements_[i] = elements_[nElements_ - 1];
				elements_[nElements_ - 1] = nullptr;
				--nElements_;
				break; 
			}
		}

		if (!elementTrouve)
		{
			throw runtime_error("Element non trouvé dans la liste");
		}
	}

	void changerCapacite(size_t nouvelleCapacite)
	{
		assert(nouvelleCapacite >= nElements_);
		unique_ptr<shared_ptr<T>[]> nouvelleListe(new shared_ptr<T>[nouvelleCapacite]);

		for (size_t i : iter::range(nElements_))
		{
			nouvelleListe[i] = elements_[i];
		}
		elements_ = move(nouvelleListe);
		capacite_ = nouvelleCapacite;
	}

	size_t taille() const
	{
		return nElements_;
	}

	shared_ptr<T> operator[](size_t index) const
	{
		if (index >= nElements_)
		{
			throw out_of_range("Index hors limite");
		}
		return elements_[index];
	}

	template<typename Function>
	shared_ptr<T> chercherElement(Function critere) const
	{
		auto it = find_if(elements_.get(), elements_.get() + nElements_, critere);
		return it != elements_.get() + nElements_ ? *it : nullptr;
	}

	template<typename Function>
	shared_ptr<T> chercherElement(Function critere, Liste& autreListe)
	{
		auto element = chercherElement(critere);
		if (element)
		{
			autreListe.ajouter(element);
		}
		return element;
	}

	template<typename U>
	friend ostream& operator<<(ostream& os, const Liste<U>& liste);

	gsl::span<shared_ptr<T>> enSpan() const { return gsl::span<shared_ptr<T>>(elements_.get(), nElements_); }

private:
	size_t nElements_ = 0, capacite_ = 0;
	unique_ptr<shared_ptr<T>[]> elements_;
};

template<typename T>
ostream& operator<<(ostream& os, const Liste<T>& liste)
{
	for (size_t i = 0; i < liste.taille(); ++i)
	{
		os << *(liste.elements_[i]) << endl;
	}
	return os;
}

template <typename T>
gsl::span<shared_ptr<T>> spanListeJeux(const Liste<T>& liste)
{
	return liste.enSpan();
}

template <typename T>
gsl::span<shared_ptr<T>> spanListeConcepteurs(const Liste<T>& liste)
{
	return liste.enSpan();
}
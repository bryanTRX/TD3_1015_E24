#pragma once 

#include <memory>
#include <cstddef>
#include <iostream>
#include "cppitertools/range.hpp"
#include "gsl/span"

using namespace std;

template <typename T>
class Liste
{
public:
	// Constructeurs ----------------------------------------------------------------------------

	Liste() : capacite_(0), nElements_(0), elements_(nullptr) {}
	Liste(size_t nElementsInitiales, size_t capaciteInitiale) : nElements_(nElementsInitiales), capacite_(capaciteInitiale), elements_(make_unique<shared_ptr<T>[]>(capaciteInitiale)) {}
	Liste(const Liste& other) : capacite_(other.capacite_), nElements_(other.nElements_), elements_(make_unique<shared_ptr<T>[]>(other.capacite_))
	{
		if (capacite_ != 0 && nElements_ != 0)
		{
			for (size_t elem = 0; elem < nElements_; ++elem)
			{
				elements_[elem] = other.elements_[elem];
			}
		}
	}
	Liste(Liste<T>&& other) = default;

	// Destructeurs -----------------------------------------------------------------------------

	~Liste() = default;

	// Methodes ---------------------------------------------------------------------------------

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
		for (size_t i = 0; i < nElements_; ++i)
		{
			if (elements_[i] == aRetirer)
			{
				elements_[i] = elements_[nElements_ - 1];
				elements_[nElements_ - 1] = nullptr;
				--nElements_;
				break;
			}
		}
	}

	void changerCapacite(size_t nouvelleCapacite)
	{
		assert(nouvelleCapacite >= nElements_);
		unique_ptr<shared_ptr<T>[]> nouvelleListe(make_unique<shared_ptr<T>[]>(nouvelleCapacite));

		for (size_t i : iter::range(nElements_))
		{
			nouvelleListe[i] = elements_[i];
		}
		elements_ = move(nouvelleListe);
		capacite_ = nouvelleCapacite;
	}

	shared_ptr<T> operator[](size_t index) const
	{
		assert(0 <= index && index < nElements_);
		return elements_[index];
	}

	template <typename Critere>
	shared_ptr<T> chercherElement(Critere critere) const
	{
		for (const auto& element : enSpan())
		{
			if (critere(element))
			{
				return element;
			}
		}
		return nullptr;
	}

	template<typename Critere>
	shared_ptr<T> chercherElement(Critere critere, Liste& autreListe)
	{
		auto element = chercherElement(critere);
		if (element)
		{
			autreListe.ajouter(element);
		}
		return element;
	}

	template<typename T>
	friend ostream& operator<<(ostream& os, const Liste<T>& liste);

	Liste& operator=(const Liste& other)
	{
		if (this != &other)
		{
			capacite_ = other.capacite_;
			nElements_ = other.nElements_;
			elements_ = make_unique<shared_ptr<T>[]>(capacite_);
			for (size_t i = 0; i < nElements_; ++i) {
				elements_[i] = make_shared<T>(*other.elements_[i]);
			}
		}
		return *this;
	}

	gsl::span<shared_ptr<T>> enSpan() const { return gsl::span<shared_ptr<T>>(elements_.get(), nElements_); }

	size_t nElements_ = 0, capacite_ = 0;
	unique_ptr<shared_ptr<T>[]> elements_;
};

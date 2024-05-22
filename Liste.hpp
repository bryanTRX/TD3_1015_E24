#pragma once

#ifndef LISTE_HPP
#define LISTE_HPP

#include <memory>
#include <string>
#include <cstddef>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include "gsl/span"

using namespace std;

template <typename T>
class Liste
{
public:
	// Constructeurs ----------------------------------------------------------------------------
	
	Liste() : capacite(0), nElements(0), elements(nullptr) {}
	Liste(size_t capaciteInitiale, size_t ElementsInitial) : capacite(capaciteInitiale), nElements(ElementsInitial), elements(new shared_ptr<T>[capaciteInitiale]) {}
	Liste(const Liste& other) : capacite(other.capacite), nElements(other.nElements), elements(new shared_ptr<T>[other.capacite])
	{
		if (capacite != 0 && nElements != 0)
		{
			for (size_t elem = 0; elem < nElements; ++elem)
			{
				elements[elem] = other.elements[elem];
			}
		}
	}

	~Liste() = default;

	// Methodes ---------------------------------------------------------------------------------

	/*gsl::span<std::shared_ptr<T>> getSpan() const
	{
		return gsl::span<std::shared_ptr<T>>(elements.get(), taille());
	}*/

	template<typename U>
    friend gsl::span<std::shared_ptr<U>> spanListe(const Liste<U>& liste);

	/*void ajouter(const shared_ptr<T>& element)
	{
		if (std::find(elements.get(), elements.get() + nElements, element) == elements.get() + nElements)
		{
			if (nElements >= capacite)
			{
				augmenterCapacite();
			}
			elements[nElements++] = element;
		}
	}*/

	void ajouter(const shared_ptr<T>& element)
	{
		if (trouver(element->getNom()))
		{
			return;
		}

		if (nElements >= capacite)
		{
			augmenterCapacite();
		}
		elements[nElements++] = element;
	}

	void retirer(const shared_ptr<T>& element)
	{
		auto it = std::find(elements.get(), elements.get() + nElements, element);
		if (it != elements.get() + nElements)
		{
			*it = elements[--nElements];
		}
	}

	shared_ptr<T> trouver(const string& nom) const
	{
		auto it = find_if(elements.get(), elements.get() + nElements, [&nom](const shared_ptr<T>& element) { return element->getNom() == nom; });
		return it != elements.get() + nElements ? *it : nullptr;
	}

	void augmenterCapacite()
	{
		capacite = capacite ? capacite * 2 : 1;
		unique_ptr<shared_ptr<T>[]> nouvelleListe(new shared_ptr<T>[capacite]);
		for (size_t i = 0; i < nElements; ++i)
		{
			nouvelleListe[i] = elements[i];
		}
		elements.swap(nouvelleListe);
	}

	size_t taille() const
	{
		return nElements;
	}

	shared_ptr<T> operator[](size_t index) const
	{
		if (index >= nElements)
		{
			throw out_of_range("Index hors limite");
		}
		return elements[index];
	}

	Liste& operator=(const Liste& other)
	{
		if (this == &other) { return *this; }

		capacite = other.capacite;
		nElements = other.nElements;

		elements.reset(new shared_ptr<T>[capacite]);

		if (nElements > 0)
		{
			copy(other.elements.get(), other.elements.get() + nElements, elements.get());
		}
		return *this;
	}

	template<typename Function>
	shared_ptr<T> chercherElement(Function critere) const
	{
		auto it = find_if(elements.get(), elements.get() + nElements, critere);
		return it != elements.get() + nElements ? *it : nullptr;
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

private:
	size_t capacite, nElements;
	unique_ptr<shared_ptr<T>[]> elements;
};

template<typename T>
ostream& operator<<(ostream& os, const Liste<T>& liste)
{
	for (size_t i = 0; i < liste.taille(); ++i)
	{
		os << *(liste.elements[i]) << endl;
	}
	return os;
}

#endif 
#pragma once
#include <cstddef>
#include "Concepteur.hpp"

struct ListeConcepteurs
{
	std::size_t nElements, capacite;
	Concepteur** elements;
};

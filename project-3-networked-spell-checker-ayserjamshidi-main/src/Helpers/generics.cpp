//
// Created by lmfaoown on 11/12/21.
//
#include <random>

#include "helpers.h"

int Helpers::Generics::rng(int min, int max) {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(min, max); // distribution in range [1, 6]

	return dist(rng);
}

bool Helpers::Generics::is_integer(const std::string &s) {
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}
//
// Created by ayserjamshidi on 11/18/21.
//

#include <random>
#include <thread>
#include <csignal>

#include "../includes/helpers.h"

void Helpers::Generics::sleep() {
	std::this_thread::sleep_for(std::chrono::milliseconds(rng(10, 100)));
}

int Helpers::Generics::rng(int min, int max) {
	return (int) rng((unsigned long)min, (unsigned long)max);
}

static std::random_device rd;
static std::mt19937 gen(rd());

unsigned long Helpers::Generics::rng(unsigned long min, unsigned long max) {
	std::uniform_int_distribution<> dist(min,max);

	return dist(gen);
}

const char * Helpers::Generics::SigusrText(int num) {
	if (num != SIGUSR1 && num != SIGUSR2)
		return "*** Unknown number pushed to SigusrText";

	return (num == SIGUSR1) ? "SIGUSR1" : "SIGUSR2";
}
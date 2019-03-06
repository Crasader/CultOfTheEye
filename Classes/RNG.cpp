#include "RNG.h"

default_random_engine RNG::mRandomGenerator(chrono::high_resolution_clock::now().time_since_epoch().count());

int RNG::getUniformInteger(int a, int b)
{
	return uniform_int_distribution<int>(a, b)(mRandomGenerator);
}

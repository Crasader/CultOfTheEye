#pragma once
#include <random>
#include <chrono>
using namespace std;

class RNG
{
	static default_random_engine mRandomGenerator;	
public:

	static int getUniformInteger(int a, int b);
};


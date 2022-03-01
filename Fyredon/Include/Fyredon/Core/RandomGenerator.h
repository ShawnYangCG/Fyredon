#pragma once
#ifndef RANDOM_GENERATOR
#define RANDOM_GENERATOR

#include<climits>

class RandomGenerator {
public:
    RandomGenerator(int seed);

    int getInt(int min, int max);

    double getDouble(double min = 0, double max = 1);

private:
	int prngState;

    int rand();
};

#endif // !RANDOM_GENERATOR

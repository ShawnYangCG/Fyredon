#pragma once
#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include<climits>
#include <iostream>



class RandomGenerator {
public:
    RandomGenerator(int seed);

    int getInt(int min, int max);

    double getDouble(double min = 0, double max = 1);
    void printDouble() const;

private:
	int prngState;

    int rand();
};

#endif // !RANDOM_GENERATOR

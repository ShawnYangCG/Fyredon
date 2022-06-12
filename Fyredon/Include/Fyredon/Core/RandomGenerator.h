// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
    RandomGenerator.h: Basic PRNG Generation : Zechen
=============================================================================*/
#pragma once


namespace Fyredon {

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

}



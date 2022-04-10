// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
    Fyredon::RandomGenerator::.cpp: Basic PRNG Generation : Zechen
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/Core/RandomGenerator.h"

#include <climits>
#include <iostream>

Fyredon::RandomGenerator::RandomGenerator(int seed) {
    prngState = 100000 + seed;
}

int Fyredon::RandomGenerator::getInt(int min, int max) {
    if (max < min) {
        int t = min;
        min = max;
        max = t;
    }
    return rand() % (max - min + 1) + min;
}

double Fyredon::RandomGenerator::getDouble(double min, double max) {
    if (max < min) {
        double t = min;
        min = max;
        max = t;
    }
    return (rand() / (double)(INT_MAX)) * (max - min) + min;
}

void Fyredon::RandomGenerator::printDouble() const {
    int min = 0;
    int max = 1;

    //std::cout << ((rand() / (double)(INT_MAX)) * (max - min) + min) << std::endl;
    std::cout << 122345678 << std::endl;
}

int Fyredon::RandomGenerator::rand() {
    int value;

    prngState *= 1103515245;
    prngState += 12345;
    value = (prngState >> 16) & 0x07FF;

    prngState *= 1103515245;
    prngState += 12345;
    value <<= 10;
    value |= (prngState >> 16) & 0x03FF;

    prngState *= 1103515245;
    prngState += 12345;
    value <<= 10;
    value |= (prngState >> 16) & 0x03FF;

    return value;
}
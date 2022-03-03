#include "Fyredon/Other/pch.h"

#include <Fyredon/Core/RandomGenerator.h>

RandomGenerator::RandomGenerator(int seed) {
    prngState = 100000 + seed;
}

int RandomGenerator::getInt(int min, int max) {
    if (max < min) {
        int t = min;
        min = max;
        max = t;
    }
    return rand() % (max - min + 1) + min;
}

double RandomGenerator::getDouble(double min, double max) {
    if (max < min) {
        int t = min;
        min = max;
        max = t;
    }
    return (rand() / (double)(INT_MAX)) * (max - min) + min;
}

int RandomGenerator::rand() {
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
#include <iostream>
#include <random>

using namespace std;

int random_int(int min, int max) {
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> distrib(min, max);

    return distrib(gen);
}
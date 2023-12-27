#include "simulation.hpp"

void simulate(Factory& factory, TimeOffset d, std::function<void(Factory&, TimeOffset)>&& rf) {
    rf(factory, d);
};
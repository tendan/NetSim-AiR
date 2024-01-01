#ifndef NETSIM_SIMULATION_HPP
#define NETSIM_SIMULATION_HPP

#include "factory.hpp"

class IntervalReportNotifier {
private:
    TimeOffset to_;
public:
    explicit IntervalReportNotifier(TimeOffset to) : to_(to) {};

    bool should_generate_report(Time t) { return t == 1 || t % to_ == 1; };
};

class SpecificTurnsReportNotifier {
private:
    std::set<Time> turns_;
public:
    explicit SpecificTurnsReportNotifier(const std::set<Time>& turns) : turns_(turns) {};

    bool should_generate_report(Time t) { return turns_.find(t) != turns_.cend(); };
};

void simulate(Factory& factory, TimeOffset d, std::function<void(Factory&, TimeOffset)>&& rf);

#endif //NETSIM_SIMULATION_HPP

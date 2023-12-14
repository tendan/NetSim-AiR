#ifndef NETSIM_TYPES_HPP
#define NETSIM_TYPES_HPP

#include <functional>

using ElementID = unsigned;

using Time = unsigned;

using TimeOffset = Time;

using ProbabilityGenerator = std::function<double()>;

#endif //NETSIM_TYPES_HPP

#ifndef NETSIM_FACTORY_H
#define NETSIM_FACTORY_H

#include <bits/stdc++.h>

using Factory = std::map<int, int>;

enum ElementType {
    RAMP, WORKER, STOREHOUSE, LINK
};

struct ParsedLineData {
    ElementType element_type;
    std::map<std::string, std::string> parameters;
};

ParsedLineData parse_line(std::string& line);

Factory load_factory_structure(std::istream& is);

void save_factory_structure(Factory& factory, std::ostream& os);

#endif //NETSIM_FACTORY_H

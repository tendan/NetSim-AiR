//
// Created by Oskar Słomion on 01/12/2023.
//

#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP

#include "types.hpp"

class Package {
public:
    Package();
    Package(ElementID elementId) : id_(elementId) {};
    Package(Package&& moved) = default;
    ~Package() = default;

    Package& operator=(Package&& moved) = default;
    ElementID get_id() const {
        return id_;
    };

private:
    ElementID id_;
};

#endif //NETSIM_PACKAGE_HPP

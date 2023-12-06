#include "package.hpp"

std::set<ElementID> Package::assigned_IDs = {};
std::set<ElementID> Package::freed_IDs = {};

Package &Package::operator=(Package &&package)  noexcept {
    if (this == &package)
        return *this;
    assigned_IDs.erase(this->ID_);
    freed_IDs.insert(this->ID_);
    this->ID_ = package.ID_;
    assigned_IDs.insert(this->ID_);
    return *this;
}

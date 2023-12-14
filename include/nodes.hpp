//
// Created by Oskar Słomion on 14/12/2023.
//

#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include "package.hpp"
#include "config.hpp"
#include "types.hpp"
#include "storage_types.hpp"
#include "helpers.hpp"

class Storehouse{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::FIFO)) : id_(id), d_(std::move(d)) {}
};

#endif //NETSIM_NODES_HPP

//
// Created by Oskar Słomion on 01/12/2023.
//
#include "package.hpp"

std::set<ElementID> Package::assigned_IDs = {};
std::set<ElementID> Package::freed_IDs = {};

Package::Package() {
    if (freed_IDs.empty()){
        if (assigned_IDs.empty()){
            ID_ = 1;
        }
        else {
            ID_ = *assigned_IDs.end() + 1;
        }
    }
    else{
        ID_ = *freed_IDs.begin();
        freed_IDs.erase(*freed_IDs.begin());
    }
    assigned_IDs.insert(ID_);
}

Package::~Package() {
    freed_IDs.insert(ID_);
    assigned_IDs.erase(ID_);
}
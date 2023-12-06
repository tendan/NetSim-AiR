//
// Created by Oskar Słomion on 01/12/2023.
//

#include "storage_types.hpp"

Package PackageQueue::pop() {
    Package package;

    if (this->package_queue_type_ == PackageQueueType::LIFO) {
        package = std::move(this->package_list_.back());
        this->package_list_.pop_back();
    } else {
        package = std::move(this->package_list_.front());
        this->package_list_.pop_front();
    }

    return package;
}
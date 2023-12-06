//
// Created by Oskar Słomion on 01/12/2023.
//

#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP

#include "package.hpp"
#include <iostream>
#include <list>

enum PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {
public:
    using const_iterator = std::list<Package>::const_iterator;

    virtual ~IPackageStockpile() = default;

    virtual void push(Package&& moved) = 0;
    virtual bool empty() const = 0;

    virtual size_t size() const = 0;

    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;
};

class IPackageQueue : public IPackageStockpile {
public:
    ~IPackageQueue() override = default;

    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() const = 0;
};

class PackageQueue : public IPackageQueue {
public:
    PackageQueue() = delete;
    explicit PackageQueue(PackageQueueType type) : package_queue_type_(type), package_list_() {}

    ~PackageQueue() override = default;

    void push(Package&& moved) override {
        this->package_list_.emplace_back(std::move(moved));
    }

    bool empty() const override {
        return this->package_list_.empty();
    }

    size_t size() const override {
        return this->package_list_.size();
    }

    const_iterator begin() const override {
        return this->package_list_.cbegin();
    }

    const_iterator end() const override {
        return this->package_list_.cend();
    }

    const_iterator cbegin() const override {
        return this->package_list_.cbegin();
    }

    const_iterator cend() const override {
        return this->package_list_.cend();
    }

    Package pop() override;

    PackageQueueType get_queue_type() const override {
        return this->package_queue_type_;
    };

private:
    PackageQueueType package_queue_type_;
    std::list<Package> package_list_;
};



#endif //NETSIM_STORAGE_TYPES_HPP

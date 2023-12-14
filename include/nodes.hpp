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

class Storehouse : public IPackageReceiver {
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::FIFO)) : id_(id), d_(std::move(d)) {}

    void receive_package(Package &&p) override;

    ElementID get_id() const override {return id_;}

    IPackageStockpile::const_iterator cbegin() const override {return d_->cbegin();}

    IPackageStockpile::const_iterator cend() const override {return d_->cend();}

    IPackageStockpile::const_iterator begin() const override {return d_->begin();}

    IPackageStockpile::const_iterator end() const override {return d_->end();}

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};


class Worker : public IPackageReceiver, public PackageSender {
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) : PackageSender(), id_(id), pd_(pd), q_(std::move(q)) {}

    void do_work(Time t);

    TimeOffset get_processing_duration() const {return pd_;}

    Time get_package_processing_start_time() const {return t_;}

    void receive_package(Package &&p) override;

    ElementID get_id() const override {return id_;}

    IPackageStockpile::const_iterator cbegin() const override {return q_->cbegin();}

    IPackageStockpile::const_iterator cend() const override {return q_->cend();}

    IPackageStockpile::const_iterator begin() const override {return q_->begin();}

    IPackageStockpile::const_iterator end() const override {return q_->end();}

private:
    ElementID id_;
    TimeOffset pd_;
    Time t_;
    std::unique_ptr<IPackageQueue> q_;
    std::optional<Package> bufor_ = std::nullopt;
};

#endif //NETSIM_NODES_HPP

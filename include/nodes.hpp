//
// Created by Oskar Słomion on 14/12/2023.
//

#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include "types.hpp"
#include "package.hpp"
#include "helpers.hpp"
#include "storage_types.hpp"
#include <optional>
#include <memory>
#include <map>
#include <utility>

class IPackageReceiver {
};

class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences() : ReceiverPreferences(probability_generator) {}

    ReceiverPreferences(ProbabilityGenerator pg) : generate_probability_(std::move(pg)) {};

    void add_receiver(IPackageReceiver* receiver);

    void remove_receiver(IPackageReceiver* receiver);

    IPackageReceiver* choose_receiver();

    const preferences_t& get_preferences() const {
        return this->preferences_;
    };

    const_iterator cbegin() const noexcept { return preferences_.cbegin(); };

    const_iterator cend() const noexcept { return preferences_.cend(); };

    const_iterator begin() const noexcept { return preferences_.begin(); };

    const_iterator end() const noexcept { return preferences_.end(); };

private:
    preferences_t preferences_;
    ProbabilityGenerator generate_probability_;
};

class PackageSender {
public:
    PackageSender() = default;

    PackageSender(PackageSender&& movable) = default;

    void send_package();

    const std::optional<Package>& get_sending_buffer() const { return buffer_; };

    ReceiverPreferences receiver_preferences_;
protected:
    void push_package(Package&& moved_package) { buffer_.emplace(moved_package.get_id()) };

    std::optional<Package> buffer_;
};


class Storehouse : public IPackageReceiver {
public:
    Storehouse(ElementID id,
               std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::FIFO)) : id_(id),
                                                                                                                d_(std::move(
                                                                                                                        d)) {}

    void receive_package(Package&& p) override;

    ElementID get_id() const override { return id_; }

    IPackageStockpile::const_iterator cbegin() const override { return d_->cbegin(); }

    IPackageStockpile::const_iterator cend() const override { return d_->cend(); }

    IPackageStockpile::const_iterator begin() const override { return d_->begin(); }

    IPackageStockpile::const_iterator end() const override { return d_->end(); }

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};


class Worker : public IPackageReceiver, public PackageSender {
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q)
            : PackageSender(), id_(id), pd_(pd), q_(std::move(q)) {}

    void do_work(Time t);

    TimeOffset get_processing_duration() const { return pd_; }

    Time get_package_processing_start_time() const { return t_; }

    void receive_package(Package&& p) override;

    ElementID get_id() const override { return id_; }

    IPackageStockpile::const_iterator cbegin() const override { return q_->cbegin(); }

    IPackageStockpile::const_iterator cend() const override { return q_->cend(); }

    IPackageStockpile::const_iterator begin() const override { return q_->begin(); }

    IPackageStockpile::const_iterator end() const override { return q_->end(); }

private:
    ElementID id_;
    TimeOffset pd_;
    Time t_;
    std::unique_ptr<IPackageQueue> q_;
    std::optional<Package> buffer_ = std::nullopt;
};

#endif //NETSIM_NODES_HPP

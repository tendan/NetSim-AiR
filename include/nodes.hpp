//
// Created by Oskar Słomion on 14/12/2023.
//

#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include "types.hpp"
#include "package.hpp"
#include "helpers.hpp"
#include <optional>
#include <map>
#include <utility>

class IPackageReceiver {};

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
    void push_package(Package&& moved_package) { buffer_.emplace(moved_package.get_id())};

    std::optional<Package> buffer_;
};

#endif //NETSIM_NODES_HPP

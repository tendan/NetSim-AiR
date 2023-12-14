//
// Created by Oskar Słomion on 14/12/2023.
//

#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include "types.hpp"
#include "package.hpp"
#include <optional>
#include <map>
#include <utility>

class IPackageReceiver {};

class ReceiverPreferences {
private:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    preferences_t preferences_;
    ProbabilityGenerator pg_;
public:
    ReceiverPreferences(ProbabilityGenerator pg) : pg_(std::move(pg)) {};

    void add_receiver(IPackageReceiver* r);

    void remove_receiver(IPackageReceiver* r);

    IPackageReceiver* choose_receiver();

    preferences_t& get_preferences();

    const_iterator cbegin() const noexcept { return preferences_.cbegin(); };
    const_iterator cend() const noexcept { return preferences_.cend(); };
    const_iterator begin() const noexcept { return preferences_.begin(); };
    const_iterator end() const noexcept { return preferences_.end(); };
};

class PackageSender {
public:
    PackageSender(PackageSender&& movable) = default;

    void send_package(void);

    std::optional<Package>& get_sending_buffer(void);

    ReceiverPreferences receiver_preferences_;
protected:
    void push_package(Package&& movable);
};

#endif //NETSIM_NODES_HPP

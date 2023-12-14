#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include "package.hpp"
#include "config.hpp"
#include "types.hpp"
#include "storage_types.hpp"
#include <memory>
#include <map>
#include <optional>
#include <utility>

class IPackageReceiver {
public:
    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;
    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;

#if defined(EXERCISE_ID) && EXERCISE_ID != EXERCISE_ID_NODES
    virtual ReceiverType get_receiver_type() const = 0;
#endif

    virtual ~IPackageReceiver() = default;
};

class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset di)
            : PackageSender(), id_(id), di_(di), t_(), bufor_(std::nullopt) {}

    void deliver_goods(Time t);

    TimeOffset get_delivery_interval() const { return di_; }

    ElementID get_id() const { return id_; }

private:
    ElementID id_;
    TimeOffset di_;
    Time t_;
    std::optional<Package> bufor_;
};
#endif //NETSIM_NODES_HPP
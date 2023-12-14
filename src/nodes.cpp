#include "nodes.hpp"

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    auto prob = pg_();
    if (prob >= 0 && prob <= 1) {
        double distribution = 0.0;
        for (const auto& rec : preferences_) {
            distribution += rec.second;
            if (distribution < 0 || distribution > 1) {
                return nullptr;
            }
            if (prob <= distribution) {
                return rec.first;
            }
        }
    }
    return nullptr;
}


void Ramp::deliver_goods(Time t) {
    if (!bufor_) {
        push_package(Package());
        bufor_.emplace(id_);
        t_ = t;
    } else if (t - di_ == t_) {
        push_package(Package());
    }
}
#include "nodes.hpp"

using ReceiverPair = std::pair<IPackageReceiver* const, double>;

void ReceiverPreferences::add_receiver(IPackageReceiver* r) {
    auto num_of_receivers = preferences_.size();
    if (num_of_receivers == 0) preferences_[r] = 1.;
    else {
        auto denominator = static_cast<double>(num_of_receivers + 1);
        std::for_each(preferences_.begin(), preferences_.end(), [=](ReceiverPair& receiver) {
            receiver.second = 1 / denominator;
        });
        preferences_[r] = 1. / denominator;
    }
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {
    auto num_of_receivers = preferences_.size();
    if (num_of_receivers <= 1) return;
    preferences_.erase(r);
    std::for_each(preferences_.begin(), preferences_.end(), [=](ReceiverPair& receiver) {
        /*receiver.second = (receiver.first != r)
                ? 1 / static_cast<double>(num_of_receivers + 1)
                : receiver.second;*/
        receiver.second = 1 / static_cast<double>(num_of_receivers - 1);
    });
}

void Storehouse::receive_package(Package &&p) {
    d_->push(std::move(p));
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    /*double probability = generate_probability_();
    if (probability < 0 || probability > 1) return nullptr;

    double distribution = 0.;
    auto found = std::find_if(preferences_.begin(), preferences_.end(), [&](const ReceiverPair& receiver) {
        distribution += receiver.second;
        return (distribution >= 0 && distribution <= 1) && (probability <= distribution);
    });

    return found != std::end(preferences_) ? found->first : nullptr;*/
    auto prob = generate_probability_();
    if (prob >= 0 && prob <= 1) {
        double distribution = 0.0;
        for (auto &rec: preferences_) {
            distribution = distribution + rec.second;
            if (distribution < 0 || distribution > 1) {
                return nullptr;
            }
            if (prob <= distribution) {
                return rec.first;
            }
        }
        return nullptr;
    }
    return nullptr;
}

void PackageSender::send_package() {
    if (!buffer_.has_value()) { return; }
    IPackageReceiver* receiver = receiver_preferences_.choose_receiver();
    receiver->receive_package(std::move(*buffer_));
    buffer_.reset();
}


void Worker::receive_package(Package &&p) {
    q_->push(std::move(p));
}

void Worker::do_work(Time t) {
    if (!buffer_ && !q_->empty()) {
        buffer_.emplace(q_->pop());
        t_ = t;
    } else {
        if (t - t_ + 1 == pd_) {
            push_package(Package(buffer_.value().get_id()));
            buffer_.reset();
            if (!q_->empty()) {
                buffer_.emplace(q_->pop());
            }
        }
    }
}

void Ramp::deliver_goods(Time t) {
    if (!buffer_) {
        push_package(Package());
        buffer_.emplace(id_);
        t_ = t;
    } else if (t - di_ == t_) {
        push_package(Package());
    }
}

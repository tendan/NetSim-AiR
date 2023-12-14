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
    preferences_.erase(r);
    if (num_of_receivers <= 1) return;
    std::for_each(preferences_.begin(), preferences_.end(), [=](ReceiverPair& receiver) {
        /*receiver.second = (receiver.first != r)
                ? 1 / static_cast<double>(num_of_receivers + 1)
                : receiver.second;*/
        receiver.second = 1 / static_cast<double>(num_of_receivers + 1);
    });
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    double probability = generate_probability_();
    if (probability < 0 || probability > 1) return nullptr;

    double distribution = 0.;
    auto found = std::find_if(preferences_.begin(), preferences_.end(), [&](const ReceiverPair& receiver) {
        distribution += receiver.second;
        return (distribution >= 0 && distribution <= 1) && (probability <= distribution);
    });

    return found != std::end(preferences_) ? found->first : nullptr;
}

void PackageSender::send_package() {
    if (buffer_.has_value()) {
        IPackageReceiver* receiver = receiver_preferences_.choose_receiver();
        //receiver->receive_package(std::move(*buffer_)); // TODO
        //buffer_.reset();
    }
}
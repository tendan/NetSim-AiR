#include "nodes.hpp"

void ReceiverPreferences::add_receiver(IPackageReceiver* r) {

}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {

}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    return nullptr;
}

ReceiverPreferences::preferences_t& ReceiverPreferences::get_preferences() {
    return this->preferences_;
}

void PackageSender::send_package(void) {

}

std::optional<Package>& PackageSender::get_sending_buffer(void) {

}

void PackageSender::push_package(Package&& movable) {

}
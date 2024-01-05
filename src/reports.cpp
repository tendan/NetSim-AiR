//
// Created by Oskar Słomion on 22/12/2023.
//
#include "reports.hpp"

void generate_structure_report(const Factory& f, std::ostream& os) {

    os << "\n== LOADING RAMPS ==\n\n";

    std::set<ElementID> workers;
    std::set<ElementID> storehouses;

    for (auto it = f.ramp_cbegin(); it != f.ramp_cend(); it++) {
        os << "LOADING RAMP #" << std::to_string(it->get_id()) << "\n  Delivery interval: "
           << std::to_string(it->get_delivery_interval()) << "\n  Receivers:\n";
        for (auto iterator = it->receiver_preferences_.cbegin();
             iterator != it->receiver_preferences_.cend(); iterator++) {
            if (iterator->first->get_receiver_type() == ReceiverType::WORKER) {
                workers.insert(iterator->first->get_id());
            } else if (iterator->first->get_receiver_type() == ReceiverType::STOREHOUSE) {
                storehouses.insert(iterator->first->get_id());
            }
        }
        for (auto i: storehouses) {
            os << "    storehouse #" << std::to_string(i) << "\n";
        }
        for (auto i: workers) {
            os << "    worker #" << std::to_string(i) << "\n";
        }
        os << "\n";
    }

    workers.clear();
    storehouses.clear();

    os << "\n== WORKERS ==\n\n";

    std::string queue_type;

    for (auto it = f.worker_cbegin(); it != f.worker_cend(); it++) {
        switch (it->get_queue()->get_queue_type()) {
            case PackageQueueType::LIFO: {
                queue_type = "LIFO";
                break;
            }
            case PackageQueueType::FIFO: {
                queue_type = "FIFO";
                break;
            }
            default:
                break;
        }
        os << "WORKER #" << std::to_string(it->get_id()) << "\n  Processing time: "
           << std::to_string(it->get_processing_duration()) << "\n  Queue type: " << queue_type << "\n  Receivers:\n";
        for (auto iterator = it->receiver_preferences_.cbegin();
             iterator != it->receiver_preferences_.cend(); iterator++) {
            if (iterator->first->get_receiver_type() == ReceiverType::WORKER) {
                workers.insert(iterator->first->get_id());
            } else if (iterator->first->get_receiver_type() == ReceiverType::STOREHOUSE) {
                storehouses.insert(iterator->first->get_id());
            }
        }
        for (auto i: storehouses) {
            os << "    storehouse #" << std::to_string(i) << "\n";
        }
        for (auto i: workers) {
            os << "    worker #" << std::to_string(i) << "\n";
        }
        os << "\n";
    }
    os << "\n== STOREHOUSES ==\n\n";
    for (auto it = f.storehouse_cbegin(); it != f.storehouse_cend(); it++) {
        os << "STOREHOUSE #" << std::to_string(it->get_id()) << "\n\n";
    }
}

void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t) {

    os << "=== [ Turn: " << std::to_string(t) << " ] ===\n\n";
    os << "== WORKERS ==\n";

    if (f.worker_cend() != f.worker_cbegin()) {
        os << "\n";
    }
    std::set<ElementID> workers;
    for (auto it = f.worker_cbegin(); it != f.worker_cend(); it++) {
        workers.insert(it->get_id());
    }
    for (auto it: workers) {
        auto iterator = f.find_worker_by_id(it);
        os << "WORKER #" << std::to_string(it) << "\n";
        if (iterator->get_processing_buffer().has_value()) {
            os << "  PBuffer: #" << iterator->get_processing_buffer()->get_id() << " (pt = "
               << std::to_string(t - iterator->get_package_processing_start_time() + 1) << ")\n";
        } else {
            os << "  PBuffer: (empty)\n";
        }
        if (iterator->get_queue()->empty()) {
            os << "  Queue: (empty)\n";
        } else {
            os << "  Queue:";
            for (auto i = iterator->get_queue()->cbegin(); i != iterator->get_queue()->cend(); i++) {
                if (i == iterator->get_queue()->cbegin()) {
                    os << " #" << i->get_id();
                } else {
                    os << ", #" << i->get_id();
                }
            }
            os << "\n";
        }
        if (iterator->get_sending_buffer().has_value()) {
            os << "  SBuffer: #" << iterator->get_sending_buffer()->get_id() << "\n";
        } else {
            os << "  SBuffer: (empty)\n";
        }

    }
    os << "\n\n== STOREHOUSES ==\n\n";
    std::set<ElementID> stores;
    for (auto it = f.storehouse_cbegin(); it != f.storehouse_cend(); it++) {
        stores.insert(it->get_id());
    }
    for (auto it: stores) {
        auto iterator = f.find_storehouse_by_id(it);
        os << "STOREHOUSE #" << std::to_string(it) << "\n";
        ///Queue
        if (iterator->get_queue()->empty()) {
            os << "  Stock: (empty)\n\n";
        } else {
            os << "  Stock:";
            for (auto i = iterator->get_queue()->cbegin(); i != iterator->get_queue()->cend(); i++) {
                if (i == iterator->get_queue()->cbegin()) {
                    os << " #" << i->get_id();
                } else {
                    os << ", #" << i->get_id();
                }
            }
            os << "\n\n";
        }
    }
}
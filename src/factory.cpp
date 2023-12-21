#include "factory.hpp"
#include "nodes.hpp"
#include <istream>
#include <string>
#include <sstream>

bool has_reachable_storehouse(PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) {
    if (node_colors[sender] == NodeColor::VERIFIED) {
        return true;
    }

    node_colors[sender] = NodeColor::VISITED;

    if (sender->receiver_preferences_.get_preferences().empty()) {
        throw std::logic_error("Sender does not have any receivers");
    }

    for (const auto& receiver : sender->receiver_preferences_.get_preferences()) {
        if (receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE) {
            return true;
        } else if (receiver.first->get_receiver_type() == ReceiverType::WORKER) {
            PackageSender* sendrecv_ptr = dynamic_cast<PackageSender*>(dynamic_cast<class Worker*>(receiver.first));

            if (sendrecv_ptr == sender) {
                continue;
            }

            if (node_colors[sendrecv_ptr] == NodeColor::UNVISITED && has_reachable_storehouse(sendrecv_ptr, node_colors)) {
                return true;
            }
        }
    }

    node_colors[sender] = NodeColor::VERIFIED;
    throw std::logic_error("Error");
}

void Factory::remove_worker(ElementID id){
    Worker* node = &(*cont_w.find_by_id(id));
    std::for_each(cont_r.begin(), cont_r.end(), [&node](Ramp& ramp) {
        ramp.receiver_preferences_.remove_receiver(node);
    });

    std::for_each(cont_w.begin(), cont_w.end(), [&node](Worker& worker) {
        worker.receiver_preferences_.remove_receiver(node);
    });
    cont_w.remove_by_id(id);
}

void Factory::remove_storehouse(ElementID id)
{
    Storehouse* node = &(*cont_s.find_by_id(id));
    std::for_each(cont_w.begin(), cont_w.end(), [&node](Worker& ramp) {
        ramp.receiver_preferences_.remove_receiver(node);
    });

    std::for_each(cont_w.begin(), cont_w.end(), [&node](Worker& worker) {
        worker.receiver_preferences_.remove_receiver(node);
    });
    cont_s.remove_by_id(id);
}

void Factory::do_deliveries(Time t) {
    for (auto &ramp : cont_r)
        ramp.deliver_goods(t);
}

void Factory::do_work(Time t ) {
    for (auto& worker : cont_w)
        worker.do_work(t);
}

void Factory::do_package_passing() {
    for (auto &ramp : cont_r)
        ramp.send_package();

    for (auto &worker : cont_w)
        worker.send_package();
}

bool Factory::is_consistent() {
    std::map<const PackageSender*, NodeColor> kolor;

    auto set_unvisited_colors = [&kolor](const auto& container) {
        for (const auto& item : container) {
            auto sender = dynamic_cast<PackageSender*>(&item);
            kolor[sender] = NodeColor::UNVISITED;
        }
    };

    set_unvisited_colors(cont_w);
    set_unvisited_colors(cont_r);

    try {
        for (const auto& ramp : cont_r) {
            auto sender = dynamic_cast<PackageSender*>(&ramp);
            has_reachable_storehouse(sender, kolor);
        }
    } catch (const std::logic_error&) {
        return false;
    }

    return true;
}
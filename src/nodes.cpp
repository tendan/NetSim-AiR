//
// Created by Oskar Słomion on 14/12/2023.
//
#include "nodes.hpp"

void Storehouse::receive_package(Package &&p) {
    d_->push(std::move(p));
}

void Worker::receive_package(Package &&p) {
    q_->push(std::move(p));
}

void Worker::do_work(Time t) {
    if (!bufor_ && !q_->empty()) {
        bufor_.emplace(q_->pop());
        t_ = t;
    }
    else {
        if (t - t_ + 1 == pd_) {
            push_package(Package(bufor_.value().get_id()));
            bufor_.reset();
            if (!q_->empty()) {
                bufor_.emplace(q_->pop());
            }
        }
    }
}
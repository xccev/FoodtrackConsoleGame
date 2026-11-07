#include "Order.hpp"

Order::Order(const std::string &description, const std::vector<int> &recipe)
    : description_(description), recipe_(recipe) {}
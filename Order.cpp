#include "Order.hpp"

Order::Order(const std::string &description, const std::vector<int> &recipe,
             const float price)
    : description_(description),
      recipe_(recipe),
      price_(price <= 0 ? 0.01 : price) {}
#pragma once

#include <string>
#include <vector>

struct Order {
 public:
  const std::string description_;
  const float price_;
  const std::vector<int> recipe_;

  Order(const std::string &description, const std::vector<int> &recipe,
        const float price);
};
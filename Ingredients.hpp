#pragma once

#include <string>

enum Ingredient {
  roll,
  butter,
  meat,
  oil,
  lettuce,
  tomato,
  cucumber,
  ketchup,
  mustard
};

struct IngredientStaticInfo {
 public:
  const std::string name_;
  const int efficiency_;

  IngredientStaticInfo(const std::string& name, float recommendedPrice,
                       int efficiency);

  double getLastPrice() const;

  void setLastPrice(float price);

 private:
  float lastPrice_;
};

struct IngredientInfo {
 public:
  Ingredient ingredient_;

  IngredientInfo(Ingredient ingredient, int amount);

  float addPieces(float amount);

  float getAmount() const;

  void setAmount(float amount);

 private:
  float amount_;
};

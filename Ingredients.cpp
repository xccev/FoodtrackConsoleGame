#include "Ingredients.hpp"

IngredientStaticInfo::IngredientStaticInfo(const std::string &name,
                                           float recommendedPrice,
                                           int efficiency)
    : name_(name), lastPrice_(recommendedPrice), efficiency_(efficiency) {}

double IngredientStaticInfo::getLastPrice() const { return lastPrice_; }

void IngredientStaticInfo::setLastPrice(float price) {
  if (price <= 0)
    lastPrice_ = 0.01;
  else
    lastPrice_ = price;
}


IngredientInfo::IngredientInfo(Ingredient ingredient, int amount)
    : ingredient_(ingredient) {
  setAmount(amount);
}

float IngredientInfo::addPieces(float amount) {
  amount_ += amount;
  if (amount_ < 0) amount_ = 0;
  return amount_;
}

float IngredientInfo::getAmount() const { return amount_; }

void IngredientInfo::setAmount(float amount) {
  if (amount < 0)
    amount_ = 0;
  else
    amount_ = amount;
}

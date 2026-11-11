#include "Ingredients.hpp"
#include "Client.hpp"
#include "Order.hpp"

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <limits>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <iomanip>

const std::array<IngredientStaticInfo, 9> INGREDIENTS_STATIC_INFO = {
    IngredientStaticInfo("bulka", 5, 2),
    IngredientStaticInfo("maslo", 4, 5),
    IngredientStaticInfo("mieso", 20, 8),
    IngredientStaticInfo("olej", 10, 20),
    IngredientStaticInfo("salata", 8.5, 5),
    IngredientStaticInfo("pomidor", 6, 5),
    IngredientStaticInfo("ogorek", 8.75, 4),
    IngredientStaticInfo("ketchup", 14, 18),
    IngredientStaticInfo("musztarda", 8, 14)};

const std::array<std::string, 10> CLIENTS_NAMES = {
    "Patryk",     "Robert", "Henryk",    "Marek",    "Norbert",
    "Malgorzata", "Oliwia", "Anastazja", "Genowefa", "Anna"};

const std::vector<Order> STATIC_ORDERS = {
    Order("Bulka z miesem",
          {Ingredient::roll, Ingredient::butter, Ingredient::meat,
           Ingredient::roll},
          13.5),
    Order("Salatka",
          {Ingredient::lettuce, Ingredient::tomato, Ingredient::cucumber,
           Ingredient::oil},
          12)};

double budget = 0;
int reputation = 100;
int day = 0;

std::vector<IngredientInfo> foodSupplies;

void init();
bool startNewDay();
void gameMenu();
void shop();
void showSupplies(bool showIndexes);
void showSupplies(const std::vector<IngredientInfo> &supplies,
                  bool showIndexes);
void showAvailableFoodtrackOfferts();
void openFoodtrack();
void shopDuringWork(bool hasShoppedEarlier, int chance, int reputationDecrease);
void serveClient();
void takeOrder(Client &client);
void doOrder(Client &client);
void serveDish(Client &client, std::vector<int> &dish);
void showReputation();
void showDaySummary();
void showPlaySummary();
void changeReputation(int change);

int main() {
  std::ios_base::sync_with_stdio(false);

  init();
  while (true) {
    if (!startNewDay()) {
      showPlaySummary();
      break;
    }
    gameMenu();
  }

  return 0;
}

void init() {
  srand(std::time(nullptr));

  do {
    std::cout << "Podaj budzet swojego foodtracka: ";
    std::cin >> budget;
  } while (budget < 0);

  foodSupplies.reserve(9);
  for (int i = 0; i < 9; i++)
    foodSupplies.push_back(IngredientInfo((Ingredient)i, 0));
}

bool startNewDay() {
  day++;
  if (day > 10) return false;
  std::cout << "----------DZIEN " << day << "----------\n";
  // changin ingredients' shop prices (random raise or discount) and checking is selling price profitable
  return true;
}

void gameMenu() {
  int temp = 0;
  while (temp != 6) {
    std::cout << "  *****BUDZET: " << budget << "zl*****\n";
    std::cout << "Wybierz opcje: \n";
    std::cout << "  1. Zrob zakupy.\n";
    std::cout << "  2. Sprawdz zapasy.\n";
    std::cout << "  3. Wyswietl dostepne oferty foodtracka.\n";
    std::cout << "  4. Otworz foodtracka.\n";
    std::cout << "  5. Sprawdz reputacje.\n";
    std::cout << "  6. Zakoncz sesje gry.\n";
    std::cin >> temp;
    switch (temp) {
      case 1:
        shop();
        break;
      case 2:
        showSupplies(false);
        break;
      case 3:
        showAvailableFoodtrackOfferts();
        break;
      case 4:
        openFoodtrack();
        temp = 5;
        break;
      case 5:
        showReputation();
        break;
      case 6:
        // session save
        std::exit(0);
        break;
    }

    std::cout << "//WCISNIJ ENTER, ABY PRZEJSC DALEJ\\\\\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
  }
}

void shop() {
  int index, amount;
  std::cout << "Twoj budzet wynosi " << budget << "zl.\n";
  std::cout << "Asortyment sklepu:\n";
  std::cout << "    lp. | NAZWA | CENA | WYDAJNOSC\n";
  for (int i = 0; i < std::size(INGREDIENTS_STATIC_INFO); i++) {
    std::cout << "    " << i + 1 << ". "
              << (char)std::toupper(INGREDIENTS_STATIC_INFO[i].name_[0])
              << INGREDIENTS_STATIC_INFO[i].name_.substr(1) << " - "
              << INGREDIENTS_STATIC_INFO[i].getLastPrice() << "zl - "
              << INGREDIENTS_STATIC_INFO[i].efficiency_ << " uzyc\n";
  }
  do {
    std::cout << "Co chcesz kupic? Podaj index: ";
    std::cin >> index;
  } while (index < 1 || index > std::size(INGREDIENTS_STATIC_INFO));

  do {
    std::cout << "Podaj ilosc: ";
    std::cin >> amount;
  } while (amount <= 0);

  float orderValue = INGREDIENTS_STATIC_INFO[index - 1].getLastPrice() * amount;
  if (index > 0 && index - 1 < std::size(INGREDIENTS_STATIC_INFO) &&
      orderValue <= budget) {
    char c;
    std::cout << "Czy chcesz sfinalizowac zakup \"" << amount << " * "
              << INGREDIENTS_STATIC_INFO[index - 1].name_ << "\" za "
              << orderValue << "zl? (T/n) ";
    std::cin >> c;
    if (std::tolower(c) == 't') {
      foodSupplies[index - 1].addPieces(amount);
      budget -= orderValue;
    } else
      std::cout << "Zakup anulowano.\n";
  } else {
    std::cout << "Nie posiadasz wystarczajacej kwoty(" << orderValue
              << "zl).\n";
  }
}

void showSupplies(bool showIndexes) { showSupplies(foodSupplies, showIndexes); }

void showSupplies(const std::vector<IngredientInfo> &supplies,
                  bool showIndexes) {
  std::cout << "Zapasy:\n";

  for (int i = 0; i < supplies.size(); i++)
    std::cout << "  " << (showIndexes ? std::to_string(i + 1) + ". " : "")
              << supplies[i].getAmount() << " * "
              << (char)std::toupper(
                     INGREDIENTS_STATIC_INFO[supplies[i].ingredient_].name_[0])
              << INGREDIENTS_STATIC_INFO[supplies[i].ingredient_].name_.substr(
                     1)
              << '\n';
}

void showAvailableFoodtrackOfferts() {
  std::cout << "****FOODTRACK MENU****\n";
  for (Order order : STATIC_ORDERS) {
    std::cout << "  ■ " << order.description_ << "\n";
    for (int i : order.recipe_)
      std::cout << "    ● " << INGREDIENTS_STATIC_INFO[i].name_ << '\n';
    std::cout << "  ~~ Cena: " << order.price_ << '\n';
  }
  std::cout << '\n';
}

void openFoodtrack() {
  std::cout << "//////////FOODTRACK OTWARTY\\\\\\\\\\\\\\\\\\\\\n";
  bool hasShopped = false;

  int temp = 0;
  while (temp != 5) {
    std::cout << "Wybierz opcje:\n";
    std::cout << "  1. Obsluz klienta.\n";
    std::cout << "  2. Sprawdz zapasy.\n";
    std::cout << "  3. Udaj sie do sklepu.\n";
    std::cout << "  4. Sprawdz reputacje.\n";
    std::cout << "  5. Zamknij foodtrack.\n";
    std::cin >> temp;

    switch (temp) {
      case 1:
        serveClient();
        break;
      case 2:
        showSupplies(false);
        break;
      case 3: {
        shopDuringWork(hasShopped, 7, 10);
        hasShopped = true;
        break;
      }
      case 4:
        showReputation();
        break;
      case 5:
        showDaySummary();
        break;
    }
  }
}

void shopDuringWork(bool hasShoppedEarlier, int chance,
                    int reputationDecrease) {
  if (hasShoppedEarlier) {
    shop();
    return;
  }

  if (chance <= 0)
    chance = 1;
  else if (chance > 10)
    chance = 10;

  char answer;
  std::cout << "Wyjscie do sklepu w czasie pracy moze spowodowac spadek "
               "reputacji. Czy mimo to chcesz wyjsc? (t/N) ";
  std::cin >> answer;
  if (std::tolower(answer) == 't') {
    if (rand() % 10 >= 10 - chance) {
      changeReputation(-reputationDecrease);
    }
    shop();
  }
}

void serveClient() {
  Client client(std::size(CLIENTS_NAMES), std::size(STATIC_ORDERS));
  std::cout << "~~~NOWY KLIENT~~~\n";
  bool hasShopped = false;

  int temp = 0;
  while (temp != 6) {
    std::cout << "Wybierz opcje:\n";
    std::cout << "  1. Odbierz zamowienie.\n";
    std::cout << "  2. Przejdz do wykonania.\n";
    std::cout << "  3. Sprawdz zapasy.\n";
    std::cout << "  4. Wyjdz do sklepu.\n";
    std::cout << "  5. Sprawdz reputacje.\n";
    std::cout << "  6. Odmow wykonania.\n";
    std::cin >> temp;

    switch (temp) {
      case 1:
        takeOrder(client);
        break;
      case 2:
        doOrder(client);
        break;
      case 3:
        showSupplies(false);
        break;
      case 4: {
        shopDuringWork(hasShopped, 9, 30);
        hasShopped = true;
        break;
      }
      case 5:
        showReputation();
        break;
      case 6:
        char c;
        std::cout << "Odmowa wykonania moze spowodowac ZNACZNY spadek "
                     "reputacji. Czy mimo to kontynuowac? (t/N) ";
        std::cin >> c;
        if (std::tolower(c) == 't') {
          if (rand() % 100 <= 94) changeReputation(-42);
        } else {
          temp = 5;
        }
        break;
    }
  }
}

void takeOrder(Client &client) {
  std::cout << "———ZAMOWIENIE———\n";
  std::cout << CLIENTS_NAMES[client.getNameIndex()] << ": "
            << STATIC_ORDERS[client.getOrderIndex()].description_ << "\n";
  for (const int ingredient : STATIC_ORDERS[client.getOrderIndex()].recipe_) {
    std::cout << "  ● " << INGREDIENTS_STATIC_INFO[ingredient].name_ << '\n';
  }
  std::cout << "\n";
}

void doOrder(Client &client) {
  std::cout << "Rozpoczeto przygotowywanie zamowienia. Pamietaj: liczy sie "
               "kolejnosc skladnikow!\n";
  int index, temp = 0;
  std::vector<IngredientInfo> suppliesCopy = foodSupplies;
  std::vector<int> work;
  while (temp != 3) {
    showSupplies(suppliesCopy, true);
    std::cout << '\n';
    for (int i : work)
      std::cout << INGREDIENTS_STATIC_INFO[suppliesCopy[i].ingredient_].name_
                << "<<<";
    do {
      std::cout << "\n    Czego uzyc? (index) ";
      std::cin >> index;
    } while (index < 1 || index > suppliesCopy.size());

    if (suppliesCopy[index - 1].getAmount() <= 0)
      std::cout
          << "Nie masz tego skladnika. ("
          << INGREDIENTS_STATIC_INFO[suppliesCopy[index - 1].ingredient_].name_
          << ")\n";
    else {
      work.push_back(index - 1);
      suppliesCopy[index - 1].addPieces(
          (float)-1 /
          INGREDIENTS_STATIC_INFO[suppliesCopy[index - 1].ingredient_]
              .efficiency_);
    }
    if (work.size()) {
      std::cout << "Wybierz opcje:\n"
                << "  1. Podaj danie.\n"
                << "  2. Usun ostatni skladnik.\n"
                << "  3. Rozloz wszystkie uzyte skladniki.\n"
                << "  [Inna opcja]. KONTYNUUJ.\n";
      std::cin >> temp;

      switch (temp) {
        case 1: {
          serveDish(client, work);
          foodSupplies = suppliesCopy;
          return;
        }
        case 2: {
          suppliesCopy[work.back()].addPieces(
              (float)1 /
              INGREDIENTS_STATIC_INFO[suppliesCopy[work.back()].ingredient_]
                  .efficiency_);
          work.pop_back();
          break;
        }
        case 3:
          std::cout << "Skladniki rozlozono.\n";
          break;
      }
    }
  }
}

void serveDish(Client &client, std::vector<int> &dish) {
  int reputationChange = 0, r = rand() % (20 + client.getMood()) + 1;
  float moneyIncome = 0;
  Order order = STATIC_ORDERS[client.getOrderIndex()];
  if (order.recipe_ == dish) {
    reputationChange = 20;
    moneyIncome = order.price_ * (1 + (float)r / 100);
  } else if (std::is_permutation(order.recipe_.begin(), order.recipe_.end(),
                                 dish.begin(), dish.end())) {
    reputationChange = 10;
    moneyIncome = order.price_ * (1 - (float)r / 100);
    r *= -1;
  } else {
    float correct = 0;
    std::sort(dish.begin(), dish.end());
    for (int i : STATIC_ORDERS[client.getOrderIndex()].recipe_) {
      auto it = std::lower_bound(dish.begin(), dish.end(), i);
      if (it != dish.end() && *it == i) {
        dish.erase(it);
        correct++;
      }
    }
    if (dish.size())
      correct /= (float)dish.size();
    else
      correct = 0;
    reputationChange = -20 * (1 - correct);
    moneyIncome = order.price_ * (correct + (float)r / 100) / 2;

    moneyIncome = std::round(moneyIncome * 100) / 100;
  }

  changeReputation(reputationChange);
  budget += moneyIncome;
  // clang-format off
  std::cout << "\n+++PODSUMOWANIE KLIENTA+++\n"
  << "  " << CLIENTS_NAMES[client.getNameIndex()] << ": "<< order.description_
  << "\n  Reputacja: " << std::to_string(reputation) << " (" << (reputationChange >= 0 ? "+" : "") << reputationChange << ')'
  << "\n  Budzet: " << std::fixed << std::setprecision(2) << budget << " (" << (moneyIncome >= 0 ? "+" : "") << moneyIncome << ")\n";
  // clang-format on
}

void showReputation() {
  std::cout << ">>>REPUTACJA<<<\n  ";
  for (int i = 0; i < reputation / 10; i++) {
    std::cout << "⬢";
  }
  for (int i = 0; i < 10 - reputation / 10; i++) {
    std::cout << "⬡";
  }
  std::cout << "\n>>>>" << reputation << "/100<<<<\n";
}

void showDaySummary() {
  std::cout << "//////////FOODTRACK ZAMKNIETY\\\\\\\\\\\\\\\\\\\\\n";
}

void showPlaySummary() {}

void changeReputation(int change) {
  reputation += change;

  if (reputation < 0)
    reputation = 0;
  else if (reputation > 100)
    reputation = 100;
}
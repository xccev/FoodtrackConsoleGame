#include <iostream>
#include <string>
#include <vector>
#include <limits>

enum Ingredient {
    roll, butter, meat, oil, lettuce, tomato, cucumber, ketchup, mustard
};

struct IngredientStaticInfo {
    public:
    const std::string name_;
    const int efficiency_;
    
    IngredientStaticInfo(std::string name, float recommendedPrice, int efficiency) : name_(name), lastPrice_(recommendedPrice), efficiency_(efficiency) {}
    
    double getLastPrice() const {
        return lastPrice_;
    }
    
    void setLastPrice(float price) {
        if (price <= 0)
        lastPrice_ = 0.01;
        else
        lastPrice_ = price;
    }
    private:
    float lastPrice_;
};
const IngredientStaticInfo INGREDIENT_STATIC_INFO[] = {IngredientStaticInfo("bulka", 5, 1), IngredientStaticInfo("maslo", 4, 5), IngredientStaticInfo("mieso", 20, 8), IngredientStaticInfo("olej", 10, 20), IngredientStaticInfo("salata", 8.5, 5), IngredientStaticInfo("pomidor", 6, 5), IngredientStaticInfo("ogorek", 8.75, 4), IngredientStaticInfo("ketchup", 14, 18), IngredientStaticInfo("musztarda", 8, 14)};

struct IngredientInfo {
    public:
        Ingredient ingredient_;

        IngredientInfo(Ingredient ingredient, int amount) : ingredient_(ingredient) {
            setAmount(amount);
        }

        int addPieces(int amount) {
            return amount_ += amount;
        }

        int getAmount() const {
            return amount_;
        }

        void setAmount(int amount) {
            if (amount <= 0)
                amount_ = 0;
            else
                amount_ = amount;
        }
    private:
        int amount_;
};

double budget = 0;
int day = 0;

std::vector<IngredientInfo> foodSupplies;

void init();
bool startNewDay();
void gameMenu();
void shop();
void showSupplies();
void showPlaySummary();

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
    do {
        std::cout << "Podaj budzet swojego foodtracka: ";
        std::cin >> budget;
    } while (budget < 0);

    foodSupplies.reserve(9);
    for (int i = 0; i < 9; i++)
        foodSupplies.push_back(IngredientInfo((Ingredient) i, 0));
}

bool startNewDay() {
    day++;
    if (day > 10)
        return false;
    std::cout << "----------DZIEN " << day << "----------\n";
    return true;
}

void gameMenu() {
    int temp;
    while (temp != 5) {
        std::cout << "  *****BUDZET: " << budget << "zl*****\n";
        std::cout << "Wybierz opcje: \n";
        std::cout << "  1. Zrob zakupy.\n";
        std::cout << "  2. Sprawdz zapasy.\n";
        std::cout << "  3. Otworz foodtracka.\n";
        std::cout << "  4. Sprawdz reputacje.\n";
        std::cout << "  5. Zakoncz sesje gry.\n";
        std::cin >> temp;
        switch (temp) {
            case 1:
                shop();
                break;
            case 2:
                showSupplies();
                break;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
}

void shop() {
    int index, amount;
    std::cout << "Twoj budzet wynosi " << budget << "zl.\n";
    std::cout << "Asortyment sklepu:\n";
    std::cout << "\tlp. | NAZWA | CENA | WYDAJNOSC\n";
    for (int i = 0; i < std::size(INGREDIENT_STATIC_INFO); i++) {
        std::cout << '\t' << i+1 << ". " << (char) std::toupper(INGREDIENT_STATIC_INFO[i].name_[0]) << INGREDIENT_STATIC_INFO[i].name_.substr(1) << " - " << INGREDIENT_STATIC_INFO[i].getLastPrice() << "zl - " << INGREDIENT_STATIC_INFO[i].efficiency_ << " uzyc\n";
    }
    std::cout << "Co chcesz kupic? Podaj index: ";
    std::cin >> index;

    std::cout << "Podaj ilosc: ";
    std::cin >> amount;

    float orderValue = INGREDIENT_STATIC_INFO[index-1].getLastPrice() * amount;
    if (index > 0 && index-1 < std::size(INGREDIENT_STATIC_INFO) && orderValue <= budget) {
        char c;
        std::cout << "Czy chcesz sfinalizowac zakup \"" << amount << " * " << INGREDIENT_STATIC_INFO[index-1].name_ << "\" za " << orderValue << "zl? (T/n) ";
        std::cin >> c;
        if (std::tolower(c) == 't') {
            foodSupplies[index-1].addPieces(amount);
            budget -= orderValue;
        } else
            std::cout << "Zakup anulowano.\n";
    } else {
        std::cout << "Nie posiadasz wystarczajacej kwoty(" << orderValue << "zl).\n";
    }
}

void showSupplies() {
    std::cout << "Zapasy:\n";

    for (const IngredientInfo &i : foodSupplies)
        std::cout << '\t' << i.getAmount() << " * " << (char) std::toupper(INGREDIENT_STATIC_INFO[i.ingredient_].name_[0]) << INGREDIENT_STATIC_INFO[i.ingredient_].name_.substr(1) << '\n';
}

void showPlaySummary() {}
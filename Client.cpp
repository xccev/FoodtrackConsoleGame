#include "Client.hpp"

Client::Client(int maxNameIndex, int maxOrderIndex) {
    srand(std::time(nullptr));
    nameIndex_ = rand() % maxNameIndex;
    orderIndex_ = rand() % maxOrderIndex;
    mood_ = (rand() % 10) + 1;
}

int Client::getNameIndex() const {
    return nameIndex_;
}

int Client::getOrderIndex() const {
    return orderIndex_;
}

int Client::getMood() const {
    return mood_;
}
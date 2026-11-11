#pragma once

#include <iostream>
#include <ctime>

class Client {
    public:
        Client(int maxNameIndex, int maxOrderIndex);

        int getNameIndex() const;
        int getOrderIndex() const;
        int getMood() const;
    private:
        int nameIndex_, orderIndex_, mood_; // mood (1-10)
};
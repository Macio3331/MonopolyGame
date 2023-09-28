#ifndef DICE_H
#define DICE_H

#include <random>
#include <thread>

class Dice
{
private:
    int actual_number = 1;
    std::mt19937 engine{ static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()) };
public:
    Dice();
    int get_number() { return actual_number; }
    void generate_number();
};

#endif // DICE_H

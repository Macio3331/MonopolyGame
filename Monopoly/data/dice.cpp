#include "dice.h"

Dice::Dice()
{

}

void Dice::generate_number()
{
    actual_number = engine() % 6 + 1;
}


#include "player.h"

Player::Player(QString name, QColor color, int number) : player_name(name), player_color(color), player_number(number)
{

}

void Player::delete_field(Field *field)
{
    auto it_field = vector_players_fields.begin();
    bool found = false;
    for(auto it = vector_players_fields.begin(); it < vector_players_fields.end(); it++)
    {
        if(field == *it)
        {
            it_field = it;
            found = true;
            break;
        }
    }
    if(!found) return;
    vector_players_fields.erase(it_field);
}


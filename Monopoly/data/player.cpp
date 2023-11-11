#include "player.h"

Player::Player(QString name, QColor color, int number) : player_name(name), player_color(color), player_number(number)
{

}

void Player::delete_field(std::shared_ptr<Field> field)
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

std::shared_ptr<Field> Player::find_field(QString field_name)
{
    auto it_field = vector_players_fields.begin();
    bool found = false;
    for(auto it = vector_players_fields.begin(); it < vector_players_fields.end(); it++)
    {
        if(field_name == (*it)->get_name())
        {
            it_field = it;
            found = true;
            break;
        }
    }
    if(!found) return nullptr;
    else return *it_field;
}

std::shared_ptr<Offer> Player::find_offer(int number)
{
    return vector_players_offers[number];
}

void Player::delete_all_offers_received(QString property_name)
{
    std::vector<int> offers_to_delete;
    for(size_t i = 0; i < vector_players_offers.size(); i++)
        if(vector_players_offers[i]->get_property_name_receiving_player() == property_name)
            offers_to_delete.push_back(i - offers_to_delete.size());
    for(size_t i = 0; i < offers_to_delete.size(); i++)
        vector_players_offers.erase(vector_players_offers.begin() + offers_to_delete[i]);
}

void Player::delete_all_offers_sent(QString property_name)
{
    std::vector<int> offers_to_delete;
    for(size_t i = 0; i < vector_players_offers.size(); i++)
        if(vector_players_offers[i]->get_property_name_sending_player() == property_name)
            offers_to_delete.push_back(i - offers_to_delete.size());
    for(size_t i = 0; i < offers_to_delete.size(); i++)
        vector_players_offers.erase(vector_players_offers.begin() + offers_to_delete[i]);
}

void Player::delete_offer(std::shared_ptr<Offer> offer)
{
    auto it_offer = vector_players_offers.begin();
    bool found = false;
    for(auto it = vector_players_offers.begin(); it < vector_players_offers.end(); it++)
    {
        if(offer == *it)
        {
            it_offer = it;
            found = true;
            break;
        }
    }
    if(!found) return;
    vector_players_offers.erase(it_offer);
}


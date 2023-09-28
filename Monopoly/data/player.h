#ifndef PLAYER_H
#define PLAYER_H

#include "field.h"
#include <vector>
#include <QString>
#include <QColor>

class Player
{
private:
    QString player_name;
    QColor player_color;
    int player_number;
    int player_money = 1500;
    int player_position = 0;
    std::vector<Field*> vector_players_fields;
    int player_number_of_stations = 0;
    int player_number_of_special = 0;
    bool player_bankrupt = false;
    bool player_prisoned = false;
    int player_prison_turn = 0;
public:
    Player() {}
    Player(QString name, QColor color, int number);
    ~Player() {}
    void set_player_name(QString name) { player_name = name; }
    void set_player_color(QColor color) { player_color = color; }
    void set_player_position(int pos) { player_position = pos; }
    void set_bankrupt(bool ban) { player_bankrupt = ban; }
    void set_prisoned(bool pris) { player_prisoned = pris; }
    void change_money(int m) { player_money += m; }
    void add_number_of_stations() { player_number_of_stations++; }
    void substract_number_of_stations() { player_number_of_stations--; }
    void add_number_of_special() { player_number_of_special++; }
    void substract_number_of_special() { player_number_of_special--; }
    void add_prison_turn() { player_prison_turn++; }
    void reset_prison_turn() { player_prison_turn = 0; }
    void add_field(Field* field) { vector_players_fields.push_back(field); }
    void delete_field(Field* field);
    QString get_player_name() { return player_name; }
    QColor get_player_color() { return player_color; }
    int get_player_number() { return player_number; }
    int get_player_money() { return player_money; }
    int get_player_position() { return player_position; }
    std::vector<Field*> get_vector_players_fields() { return vector_players_fields; }
    int get_player_number_of_stations() { return player_number_of_stations; }
    int get_player_number_of_special() { return player_number_of_special; }
    bool get_player_bankrupt() { return player_bankrupt; }
    bool get_player_prisoned() { return player_prisoned; }
    int get_player_prison_turn() { return player_prison_turn; }
};

#endif // PLAYER_H

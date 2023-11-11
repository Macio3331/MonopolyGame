#ifndef OFFER_H
#define OFFER_H

#include <QString>

class Offer
{
private:
    int number_sending_player;
    int money_sending_player;
    QString property_name_sending_player;
    int money_receiving_player;
    QString property_name_receiving_player;
public:
    Offer() {}
    Offer(int number_sender, int money_sender, QString property_name_sender, int money_receiver, QString property_name_receiver);
    ~Offer() {}
    int get_number_sending_player() { return number_sending_player; }
    int get_money_sending_player() { return money_sending_player; }
    QString get_property_name_sending_player() { return property_name_sending_player; }
    int get_money_receiving_player() { return money_receiving_player; }
    QString get_property_name_receiving_player() { return property_name_receiving_player; }
    void set_number_sending_player(int number) { number_sending_player = number; }
    void set_money_sending_player(int money) { money_sending_player = money; }
    void set_property_name_sending_player(QString name) { property_name_sending_player = name; }
    void set_money_receiving_player(int money) { money_receiving_player = money; }
    void set_property_name_receiving_player(QString name) { property_name_receiving_player = name; }
};

#endif // OFFER_H

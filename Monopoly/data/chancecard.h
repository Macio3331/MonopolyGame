#ifndef CHANCECARD_H
#define CHANCECARD_H
#include <QString>

class ChanceCard
{
    int id_number;
    QString description;
    bool one_button;
public:
    ChanceCard(int number, QString desc, bool one);
    ~ChanceCard() {}
    void set_id_number(int number) { id_number = number; }
    void set_description(QString desc) { description = desc; }
    void set_one_button(bool one) { one_button = one; }
    int get_id_number() { return id_number; }
    QString get_description() { return description; }
    bool get_one_button() { return one_button; }
};

#endif // CHANCECARD_H

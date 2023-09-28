#ifndef FIELD_H
#define FIELD_H

#include <string>
#include <vector>
#include <QString>

class Field
{
private:
    QString name;
    QString description;
    QString owner = "Bank";
    int number_owner = -1;
protected:
    QString classname = "field";
public:
    Field() {}
    Field(QString name, QString description);
    ~Field() {}
    void set_name(QString n) { name = n; }
    void set_description(QString desc) { description = desc; }
    void set_owner(QString name) { owner = name; }
    void set_number_owner(int number) { number_owner = number; }
    QString get_name() { return name; }
    QString get_description() { return description; }
    QString get_owner() { return owner; }
    QString get_classname() { return classname; }
    int get_number_owner() { return number_owner; }
    virtual bool get_popped() { return false; }
    virtual int get_building_cost() { return 0; }
    virtual int get_number_of_houses() { return 0; }
    virtual int get_payment(int number) { return 0 * number; }
    virtual int get_buying_cost() { return 0; }
};

class FieldProperty : public Field
{
private:
    int number_of_houses = 0;
    std::vector<int> vector_payments;
    int buying_cost;
    int building_cost;
    bool popped = false;
public:
    FieldProperty() { classname = "field_property"; }
    FieldProperty(QString name, QString desc, std::vector<int> v, int buy, int build);
    ~FieldProperty() {}
    virtual int get_number_of_houses() override { return number_of_houses; }
    virtual int get_payment(int number) override { return vector_payments[number]; }
    virtual int get_buying_cost() override { return buying_cost; }
    virtual int get_building_cost() override { return building_cost; }
    virtual bool get_popped() override { return popped; }
};

class FieldChance : public Field
{
private:
    bool choice;
public:
    FieldChance(QString name, QString desc);
    FieldChance() { classname = "field_station"; }
    ~FieldChance() {}
    bool get_choice() { return choice; }
};

class FieldStation : public Field
{
private:
    bool popped = false;
    int buying_cost = 200;
public:
    FieldStation() { classname = "field_station"; }
    FieldStation(QString name, QString desc);
    ~FieldStation() {}
    virtual int get_buying_cost() override { return buying_cost; }
    virtual bool get_popped() override { return popped; }
};

class FieldSpecial : public Field
{
private:
    bool popped = false;
    int buying_cost = 150;
public:
    FieldSpecial() { classname = "field_special"; }
    FieldSpecial(QString name, QString desc);
    ~FieldSpecial() {}
    virtual int get_buying_cost() override { return buying_cost; }
    virtual bool get_popped() override { return popped; }
};

#endif // FIELD_H

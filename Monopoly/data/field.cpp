#include "field.h"

Field::Field(QString name, QString description) : name(name), description(description)
{

}

FieldProperty::FieldProperty(QString name, QString desc, std::vector<int> v, int buy, int build) : Field(name, desc), vector_payments(v), buying_cost(buy), building_cost(build)
{
    classname = "field_property";
}

FieldChance::FieldChance(QString name, QString desc) : Field(name, desc)
{
    classname = "field_chance";
}

FieldStation::FieldStation(QString name, QString desc) : Field(name, desc)
{
    classname = "field_station";
}

FieldSpecial::FieldSpecial(QString name, QString desc) : Field(name, desc)
{
    classname = "field_special";
}

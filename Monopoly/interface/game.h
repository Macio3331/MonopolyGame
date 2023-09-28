#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPainter>
#include <QPushButton>
#include <QTimer>
#include "data/field.h"
#include "data/player.h"
#include "data/dice.h"

class Game : public QWidget
{
    Q_OBJECT
public:
    explicit Game(QWidget *parent, QVector<QString> vector_name_players, int number_of_players, bool auction);
    ~Game();
private:
    //interface
    //center
    QLabel *board;
    QLabel *dice1_label;
    QLabel *dice2_label;
    QVector<QPushButton*> vector_board_buttons;
    QVector<QLabel*> vector_chequers;
    QVector<QVector<QRect>> chequers_position_table;
    QPushButton* dice1_button;
    QPushButton* dice2_button;
    //left
    QLabel *player_label;
    QLabel *color_player_label;
    QLabel *money_label;
    QLabel *action_label;
    QLabel *action_name_label;
    QLabel *action_description_label;
    QPushButton *build_house_button;
    QPushButton *sell_house_button;
    QPushButton *exchange_button;
    QPushButton *offers_button;
    QPushButton *hypothecation_button;
    QPushButton *end_turn_button;
    QPushButton *accept_button;
    QPushButton *deny_button;
    QPushButton *ok_button;
    QLabel *message;
    //right
    QLabel *player_cards_label;
    QLabel *card_label;
    QLabel *card_name_label;
    QLabel *card_description_label;
    //
    QLabel* create_pixmap_label(QString path, QRect geometry, bool scaled = false, int scale_width = 0, int scale_height = 0, bool fill = false, QColor color = Qt::white);
    QLabel* create_label(QString text, QFont font, QString stylesheet, QRect geometry, Qt::Alignment align = Qt::AlignLeft);
    QPushButton* create_pushbutton(QString text, QFont font, QString stylesheet, QRect geometry, bool enabled = true);
    //
    void initialize_interface();
    void initialize_field_buttons();
    void initialize_dice_buttons();
    void layer_fields(QVector<Field*>& vector_fields_chance_help, QVector<Field*>& vector_fields_help, QVector<Field*>& vector_fields_station_help, QVector<Field*>& vector_fields_special_help, QVector<Field*>& vector_fields_properties_help);
    void change_dices_labels(int dice_number, QLabel* dice_label);
    void initialize_chequers();
    void initialize_chequers_table();
    //logic
    QVector<QString> vector_name_players;
    int number_of_players;
    bool auction;
    bool dublet;
    int dublet_turn;
    int current_player;
    std::vector<Field*> vector_fields;
    std::vector<Player*> vector_players;
    std::vector<std::vector<Field*>> vector_estates;
    Dice* dice1;
    Dice* dice2;
    //
    void create_vector_fields();  
    void initialize_players();
    void initialize_dices();
    void initialize_vector_estates();
    void check_position(Field* actual_field);
    int find_entrance_cost(Field* actual_field);
    std::pair<bool, bool> find_estate(Field* actual_field, int number_player_property);
    void show_message(QString text);
    //
    Field* create_field(QString n, QString desc);
    Field* create_field_chance(QString n, QString desc);
    Field* create_field_special(QString n, QString desc);
    Field* create_field_station(QString n, QString desc);
    Field* create_field_property(QString n, QString desc, std::vector<int> v, int buy, int build);
    Player* create_player(QString name, QColor color, int number);
    Dice* create_dice();
private slots:
    void on_build_house_button_clicked();
    void on_sell_house_button_clicked();
    void on_exchange_button_clicked();
    void on_offers_button_clicked();
    void on_hypothecation_button_clicked();
    void on_end_turn_button_clicked();
    void on_accept_button_clicked();
    void on_ok_button_clicked();
    void on_deny_button_clicked();
    void on_field_button_clicked();
    void on_dice_button_clicked();
};

#endif // GAME_H

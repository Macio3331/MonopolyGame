#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPainter>
#include <QPushButton>
#include <QTimer>
#include <QDialog>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QListWidget>
#include <QSpinBox>
#include <QPointer>
#include <QThread>
#include <QFuture>
#include <regex>
#include <algorithm>
#include <queue>
#include <memory>
#include <thread>
#include <future>
#include <condition_variable>
#include <mutex>
#include <ranges>
#include "data/field.h"
#include "data/player.h"
#include "data/dice.h"
#include "data/chancecard.h"
#include "data/windialog.h"
#include "data/exchangedialog.h"
#include "data/offersdialog.h"
#include "data/auctiondialog.h"

class Game : public QWidget
{
    Q_OBJECT
public:
    explicit Game(QWidget *parent, QVector<QString> vector_name_players, QVector<QColor> vector_color_players, int number_of_players, bool auction);
    ~Game();
private:
    QWidget* parent;
    QPointer<QListWidget> players_offers;
    //interface
    //center
    QPointer<QLabel> board;
    QPointer<QLabel> dice1_label;
    QPointer<QLabel> dice2_label;
    QVector<QPointer<QPushButton>> vector_board_buttons;
    QVector<QPointer<QLabel>> vector_chequers;
    QVector<QVector<QRect>> chequers_position_table;
    QVector<QVector<QPointer<QLabel>>> buildings_labels_table;
    QPointer<QPushButton> dice1_button;
    QPointer<QPushButton> dice2_button;
    //left
    QPointer<QLabel> player_label;
    QPointer<QLabel> color_player_label;
    QPointer<QLabel> money_label;
    QPointer<QLabel> action_label;
    QPointer<QLabel> action_name_label;
    QPointer<QLabel> action_description_label;
    QPointer<QLabel> exclamation_mark_label;
    QPointer<QPushButton> build_house_button;
    QPointer<QPushButton> sell_house_button;
    QPointer<QPushButton> exchange_button;
    QPointer<QPushButton> offers_button;
    QPointer<QPushButton> hypothecation_button;
    QPointer<QPushButton> buyback_button;
    QPointer<QPushButton> end_turn_button;
    QPointer<QPushButton> accept_button;
    QPointer<QPushButton> deny_button;
    QPointer<QPushButton> ok_button;
    QPointer<QLabel> message;
    //right
    QPointer<QLabel> player_cards_label;
    QVector<QPointer<QLabel>> vector_cards_label;
    QPointer<QLabel> card_label;
    QPointer<QLabel> card_name_label;
    QPointer<QLabel> card_description_label;
    //
    QPointer<QLabel> create_pixmap_label(QString path, QRect geometry, QString stylesheet = "background-color: transparent", bool hidden = true);
    QPointer<QLabel> create_pixmap_label(QString path, QRect geometry, QColor color);
    QPointer<QLabel> create_pixmap_label(QString path, QRect geometry, int scale_width, int scale_height);
    QPointer<QLabel> create_pixmap_label(QString path, QRect geometry, int scale_width, int scale_height, QColor color);
    QPointer<QLabel> create_pixmap_label(QString path, QRect geometry, int scale_width, int scale_height, QString stylesheet, bool hidden);
    QPointer<QLabel> create_label(QString text, QFont font, QString stylesheet, QRect geometry = QRect(0, 0, 0, 0), Qt::Alignment align = Qt::AlignLeft);
    QPointer<QPushButton> create_pushbutton(QString text, QFont font, QString stylesheet, QRect geometry = QRect(0, 0, 0, 0), bool enabled = true);
    //
    void initialize_interface();
    bool initialize_field_buttons();
    bool initialize_vector_cards_label();
    bool initialize_dice_buttons();
    void layer_fields(QVector<std::shared_ptr<Field>>& vector_fields_chance_help, QVector<std::shared_ptr<Field>>& vector_fields_help, QVector<std::shared_ptr<Field>>& vector_fields_station_help, QVector<std::shared_ptr<Field>>& vector_fields_special_help, QVector<std::shared_ptr<Field>>& vector_fields_properties_help);
    void change_dices_labels(int dice_number, QPointer<QLabel> dice_label);
    bool initialize_chequers();
    void initialize_chequers_table();
    bool initialize_buildings_labels_table();
    void refresh_button_states();
    void get_button_states();
    void enable_six_buttons();
    void enable_next_throw();
    void disable_next_throw();
    void disable_next_turn();
    void restore_button_states();
    void create_center_interface();
    void create_left_interface();
    void create_right_interface();
    void create_left_buttons_interface();
    void hide_interface();
    void disable_reaction_buttons();
    void disable_interface_buttons();
    void show_action_labels();
    void show_card_labels();
    void restore_card_labels();
    void show_two_buttons();
    void change_action_labels(QString action_name, QString action_description);
    void clear_card_labels();
    void repeat_action(bool& enough);
    void repeat_action();
    void change_one_money_label(int money);
    void change_money_start_label();

    //logic
    QVector<QString> vector_name_players;
    QVector<QColor> vector_color_players;
    int number_of_players;
    bool auction;
    bool dublet;
    bool comm_card;
    int dublet_turn;
    std::shared_ptr<Player> current_player;
    int chance_card_index;
    int community_chest_card_index;
    int view_number;

    int number_field_button_clicked;
    int fixing_cost;
    struct
    {
        bool dice_buttons_enabled;
        bool end_turn_button_enabled;
        bool action_hidden;
        bool action_name_hidden;
        QString action_name_text;
        bool action_desc_hidden;
        QString action_desc_text;
        bool ok_button_hidden;
        bool accept_button_hidden;
        bool deny_button_hidden;
    } last_state;

    std::vector<std::shared_ptr<Field>> vector_fields;
    std::vector<std::shared_ptr<Player>> vector_players;
    std::vector<std::vector<std::shared_ptr<Field>>> vector_estates;
    std::vector<std::shared_ptr<ChanceCard>> vector_chance_cards;
    std::vector<std::shared_ptr<ChanceCard>> vector_community_chest_cards;
    std::shared_ptr<Dice> dice1;
    std::shared_ptr<Dice> dice2;
    //
    void create_vector_fields();
    void initialize_players();
    void initialize_dices();
    void initialize_vector_estates();
    void initialize_vector_chance_cards();
    void initialize_vector_community_chest_cards();
    void shuffle_cards(bool chance_cards);
    void check_position(std::shared_ptr<Field> actual_field);
    bool check_for_prison_card(bool chance_card);
    void react_card_chance(int id_number, bool& second_button, bool& enough);
    void react_card_community_chest(int id_number, bool& second_button, bool& enough);
    void change_player_position(int new_position, bool& second_button, bool check_for_money = true);
    void prison_player();
    void add_prison_card(bool chance_card);
    std::shared_ptr<Field> find_prison_card(bool chance_card);
    int find_fixing_cost(int house_cost, int hotel_cost);
    int find_entrance_cost(std::shared_ptr<Field> actual_field);
    std::pair<bool, bool> find_estate(std::shared_ptr<Field> actual_field, int number_player_property);
    std::vector<std::shared_ptr<Field>> get_estate(std::shared_ptr<Field> actual_field);
    void build_house();
    void sell_house();
    void hyphothecate_property();
    void buyback_property();
    bool check_bankruptcy(std::shared_ptr<Player> player, int cost);
    void player_bankrupted(std::shared_ptr<Player> player, int owner_number, int money_to_pay, bool actual_player = true);
    void organize_bankrupt_player(std::shared_ptr<Player> bankrupt_player, int player_property_number, int money_to_pay, bool actual_player = true);
    void get_money_automatically(std::shared_ptr<Player> player);
    void exchange_money(std::shared_ptr<Player> player_to_give, std::shared_ptr<Player> player_to_get, int money);
    void exchange_money_label(std::shared_ptr<Player> player_to_give, std::shared_ptr<Player> player_to_get, int money);
    void change_ownership(std::shared_ptr<Field> actual_field, QString owner, int owner_number);
    void prepare_before_next_player();
    void prepare_next_player();
    void find_next_player();
    bool check_win();
    void win_procedure();
    void change_page();
    //
    std::shared_ptr<Field> create_field(QString n, QString desc);
    std::shared_ptr<Field> create_field_chance(QString n, QString desc);
    std::shared_ptr<Field> create_field_special(QString n, QString desc);
    std::shared_ptr<Field> create_field_station(QString n, QString desc);
    std::shared_ptr<Field> create_field_property(QString n, QString desc, std::vector<int> v, int buy, int build);
    std::shared_ptr<Player> create_player(QString name, QColor color, int number);
    std::shared_ptr<Dice> create_dice();
    std::shared_ptr<ChanceCard> create_chance_card(int number, QString desc, bool one = true);
public:
    void show_message(QString text);
    void change_vector_cards_label();
    void actualize_field_state(std::shared_ptr<Field> actual_field);
    void hide_exclamation_mark() { exclamation_mark_label->hide(); }
    void actualize_money_label() { money_label->setText("Finanse:  " + QString::number(current_player->get_player_money()) + "$"); }
private slots:
    void on_build_house_button_clicked();
    void on_sell_house_button_clicked();
    void on_exchange_button_clicked();
    void on_offers_button_clicked();
    void on_hypothecation_button_clicked();
    void on_buyback_button_clicked();
    void on_end_turn_button_clicked();
    void on_accept_button_clicked();
    void on_ok_button_clicked();
    void on_deny_button_clicked();
    void on_field_button_clicked();
    void on_dice_button_clicked();
};

#endif // GAME_H

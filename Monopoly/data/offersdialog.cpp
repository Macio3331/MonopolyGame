#include "offersdialog.h"
#include "interface/game.h"

OffersDialog::OffersDialog(QWidget* parent, std::vector<std::shared_ptr<Player>> vector_players, std::shared_ptr<Player> current_player, QPointer<QListWidget> players_offers)
    : parent(parent), vector_players(vector_players), current_player(current_player), players_offers(players_offers)
{

}

void OffersDialog::execute()
{
    QPointer<QDialog> offers_dialog = new QDialog();
    offers_dialog->setStyleSheet("background-color: #323236");
    offers_dialog->setWindowFlags(Qt::Window | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    offers_dialog->setMinimumSize(400, 500);
    QPointer<QVBoxLayout> v_layout = new QVBoxLayout(this);
    QPointer<QLabel> offers_label = create_label("Oferty", QFont("Open Sans", 30), "color: white");
    offers_label->setAlignment(Qt::AlignCenter);
    players_offers = new QListWidget(this);
    players_offers->setStyleSheet("color: white; background-color: #484852");
    std::vector<std::shared_ptr<Offer>> vector_players_offers = current_player->get_vector_players_offers();
    for(size_t i = 0; i < vector_players_offers.size(); i++)
    {
        players_offers->addItem("Oferta od gracza nr " + QString::number(vector_players_offers[i]->get_number_sending_player() + 1) + ":"
                                "\nPłacisz: " + QString::number(vector_players_offers[i]->get_money_receiving_player()) + "$" + (vector_players_offers[i]->get_property_name_receiving_player() != "" ? " i oddajesz nieruchomość " + vector_players_offers[i]->get_property_name_receiving_player() : "") +
                                "\nZa: "+ QString::number(vector_players_offers[i]->get_money_sending_player()) + "$" + (vector_players_offers[i]->get_property_name_sending_player() != "" ? " i otrzymujesz nieruchomość " + vector_players_offers[i]->get_property_name_sending_player() : ""));
        players_offers->item(i)->setForeground(Qt::white);
    }
    players_offers->setCurrentItem(players_offers->item(0));

    QPointer<QHBoxLayout> h_layout = new QHBoxLayout(this);
    QPointer<QPushButton> yes_button = create_pushbutton("Przyjmij", QFont("Open Sans", 20), "color: white");
    QPointer<QPushButton> no_button = create_pushbutton("Odrzuć", QFont("Open Sans", 20), "color: white");
    connect(yes_button, &QPushButton::clicked, this, &OffersDialog::on_yes_button_clicked);
    connect(no_button, &QPushButton::clicked, this, &OffersDialog::on_no_button_clicked);
    h_layout->addWidget(yes_button);
    h_layout->addWidget(no_button);

    QPointer<QDialogButtonBox> offers_button_box = new QDialogButtonBox(QDialogButtonBox::Cancel);
    offers_button_box->setStyleSheet("color: white; background-color: #484852");
    offers_button_box->setCenterButtons(true);
    connect(offers_button_box, &QDialogButtonBox::rejected, offers_dialog, &QDialog::reject);

    v_layout->addWidget(offers_label);
    v_layout->addWidget(players_offers);
    v_layout->addLayout(h_layout);
    v_layout->addWidget(offers_button_box);

    offers_dialog->setLayout(v_layout);

    bool next = false;
    do
    {
        offers_dialog->exec();
        if(!offers_dialog->result())
            next = true;
    } while(!next);
    offers_dialog->close();
    delete offers_dialog;
    offers_dialog = nullptr;
}

QPointer<QLabel> OffersDialog::create_label(QString text, QFont font, QString stylesheet, QRect geometry, Qt::Alignment align)
{
    QPointer<QLabel> label = new QLabel(this);
    label->setText(text);
    label->setFont(font);
    label->setStyleSheet(stylesheet);
    label->setGeometry(geometry);
    label->setAlignment(align);
    return label;
}

QPointer<QPushButton> OffersDialog::create_pushbutton(QString text, QFont font, QString stylesheet, QRect geometry, bool enabled)
{
    QPointer<QPushButton> button = new QPushButton(this);
    button->setText(text);
    button->setFont(font);
    button->setStyleSheet(stylesheet);
    button->setGeometry(geometry);
    if(!enabled) button->setEnabled(enabled);
    return button;
}

void OffersDialog::on_yes_button_clicked()
{
    if(current_player->get_vector_players_offers().size())
    {
        int number = players_offers->indexFromItem(players_offers->currentItem()).row();
        std::shared_ptr<Offer> offer = current_player->find_offer(number);
        if(offer->get_money_sending_player() > vector_players[offer->get_number_sending_player()]->get_player_money())
            reinterpret_cast<Game*>(parent)->show_message("Gracz " + QString::number(offer->get_number_sending_player() + 1) + " ma za mało funduszy!");
        else if(offer->get_money_receiving_player() > current_player->get_player_money())
            reinterpret_cast<Game*>(parent)->show_message("Masz za mało funduszy do wykonania akcji!");
        else
        {
            if(offer->get_money_receiving_player() > 0)
            {
                current_player->change_money(-1 * offer->get_money_receiving_player());
                vector_players[offer->get_number_sending_player()]->change_money(offer->get_money_receiving_player());
            }
            else if(offer->get_money_sending_player() > 0)
            {
                vector_players[offer->get_number_sending_player()]->change_money(-1 * offer->get_money_sending_player());
                current_player->change_money(offer->get_money_sending_player());
            }

            if(offer->get_property_name_receiving_player() != "")
            {
                std::shared_ptr<Field> field = current_player->find_field(offer->get_property_name_receiving_player());
                current_player->delete_field(field);
                vector_players[offer->get_number_sending_player()]->add_field(field);
                field->set_number_owner(offer->get_number_sending_player());
                reinterpret_cast<Game*>(parent)->actualize_field_state(field);
                if(offer->get_property_name_receiving_player() == "Szansa")
                {
                    vector_players[offer->get_number_sending_player()]->set_player_chance_prison_card(true);
                    current_player->set_player_chance_prison_card(false);
                }
                else if(offer->get_property_name_receiving_player() == "Kasa społeczna")
                {
                    vector_players[offer->get_number_sending_player()]->set_player_comm_prison_card(true);
                    current_player->set_player_comm_prison_card(false);
                }
                if(field->get_classname() == "field_station" && !field->get_popped())
                {
                    vector_players[offer->get_number_sending_player()]->add_number_of_stations();
                    current_player->substract_number_of_stations();
                }
                else if(field->get_classname() == "field_special" && !field->get_popped())
                {
                    vector_players[offer->get_number_sending_player()]->add_number_of_special();
                    current_player->substract_number_of_special();
                }

                QString current_player_property_name = offer->get_property_name_receiving_player();
                for(const auto& player : vector_players)
                {
                    if(player == current_player) continue;
                    player->delete_all_offers_sent(current_player_property_name);
                }
                current_player->delete_all_offers_received(current_player_property_name);
            }
            if(offer->get_property_name_sending_player() != "")
            {
                std::shared_ptr<Field> field = vector_players[offer->get_number_sending_player()]->find_field(offer->get_property_name_sending_player());
                vector_players[offer->get_number_sending_player()]->delete_field(field);
                current_player->add_field(field);
                field->set_number_owner(current_player->get_player_number());
                reinterpret_cast<Game*>(parent)->actualize_field_state(field);
                if(offer->get_property_name_receiving_player() == "Szansa")
                {
                    current_player->set_player_chance_prison_card(true);
                    vector_players[offer->get_number_sending_player()]->set_player_chance_prison_card(false);
                }
                else if(offer->get_property_name_receiving_player() == "Kasa społeczna")
                {
                    current_player->set_player_comm_prison_card(true);
                    vector_players[offer->get_number_sending_player()]->set_player_comm_prison_card(false);
                }
                if(field->get_classname() == "field_station" && !field->get_popped())
                {
                    current_player->add_number_of_stations();
                    vector_players[offer->get_number_sending_player()]->substract_number_of_stations();
                }
                else if(field->get_classname() == "field_special" && !field->get_popped())
                {
                    current_player->add_number_of_special();
                    vector_players[offer->get_number_sending_player()]->substract_number_of_special();
                }

                QString sending_player_property_name = offer->get_property_name_sending_player();
                for(const auto& player : vector_players)
                {
                    if(player == vector_players[offer->get_number_sending_player()]) continue;
                    player->delete_all_offers_sent(sending_player_property_name);
                }
                vector_players[offer->get_number_sending_player()]->delete_all_offers_received(sending_player_property_name);
            }

            reinterpret_cast<Game*>(parent)->change_vector_cards_label();
            reinterpret_cast<Game*>(parent)->actualize_money_label();

            std::vector<std::shared_ptr<Offer>> vector_players_offers = current_player->get_vector_players_offers();
            players_offers->hide();
            players_offers->clear();
            for(size_t i = 0; i < vector_players_offers.size(); i++)
            {
                players_offers->addItem("Oferta od gracza nr " + QString::number(vector_players_offers[i]->get_number_sending_player() + 1) + ":"
                                        "\nPłacisz: " + QString::number(vector_players_offers[i]->get_money_receiving_player()) + "$" + (vector_players_offers[i]->get_property_name_receiving_player() != "" ? " i oddajesz nieruchomość " + vector_players_offers[i]->get_property_name_receiving_player() : "") +
                                        "\nZa: "+ QString::number(vector_players_offers[i]->get_money_sending_player()) + "$" + (vector_players_offers[i]->get_property_name_sending_player() != "" ? " i otrzymujesz nieruchomość " + vector_players_offers[i]->get_property_name_sending_player() : ""));
                players_offers->item(i)->setForeground(Qt::white);
            }
            if(vector_players_offers.size())
                players_offers->setCurrentItem(players_offers->item(0));
            else
            {
                reinterpret_cast<Game*>(parent)->show_message("Brak ofert do wyświetlenia!");
                reinterpret_cast<Game*>(parent)->hide_exclamation_mark();
            }
            players_offers->show();
        }
    }
}

void OffersDialog::on_no_button_clicked()
{
    if(current_player->get_vector_players_offers().size())
    {
        int number = players_offers->indexFromItem(players_offers->currentItem()).row();
        std::shared_ptr<Offer> offer = current_player->find_offer(number);
        current_player->delete_offer(offer);
        std::vector<std::shared_ptr<Offer>> vector_players_offers = current_player->get_vector_players_offers();
        players_offers->hide();
        players_offers->clear();
        for(size_t i = 0; i < vector_players_offers.size(); i++)
        {
            players_offers->addItem("Oferta od gracza nr " + QString::number(vector_players_offers[i]->get_number_sending_player() + 1) + ":"
                                    "\nPłacisz: " + QString::number(vector_players_offers[i]->get_money_receiving_player()) + "$" + (vector_players_offers[i]->get_property_name_receiving_player() != "" ? " i oddajesz nieruchomość " + vector_players_offers[i]->get_property_name_receiving_player() : "") +
                                    "\nZa: "+ QString::number(vector_players_offers[i]->get_money_sending_player()) + "$" + (vector_players_offers[i]->get_property_name_sending_player() != "" ? " i otrzymujesz nieruchomość " + vector_players_offers[i]->get_property_name_sending_player() : ""));
            players_offers->item(i)->setForeground(Qt::white);
        }
        if(vector_players_offers.size())
            players_offers->setCurrentItem(players_offers->item(0));
        else
        {
            reinterpret_cast<Game*>(parent)->show_message("Brak ofert do wyświetlenia!");
            reinterpret_cast<Game*>(parent)->hide_exclamation_mark();
        }
        players_offers->show();
    }
}

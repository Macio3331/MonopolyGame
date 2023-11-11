#include "auctiondialog.h"
#include "interface/game.h"

AuctionDialog::AuctionDialog(QWidget* parent, std::vector<std::shared_ptr<Player>> vector_players, std::shared_ptr<Player> current_player, std::shared_ptr<Field> actual_field)
    : parent(parent), vector_players(vector_players), current_player(current_player), actual_field(actual_field)
{

}

void AuctionDialog::execute()
{
    int current_payment = actual_field->get_buying_cost() * 1.1 - 1;
    int current_buyer = -1;
    int proposed_player = -1;
    std::vector<bool> vector_bool_players;
    for(const auto& player : vector_players)
        if(player == current_player || player->get_player_bankrupt()) vector_bool_players.push_back(false);
        else vector_bool_players.push_back(true);
    for(int i = 0; i < vector_players.size(); i++)
        if(vector_bool_players[i])
        {
            proposed_player = i;
            break;
        }

    QPointer<QDialog> auction_dialog = new QDialog();
    auction_dialog->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    auction_dialog->setStyleSheet("background-color: #323236");
    QPointer<QVBoxLayout> v_layout = new QVBoxLayout(this);

    QPointer<QLabel> auction_label = create_label("Licytacja\n", QFont("Open Sans", 30), "color: white");
    auction_label->setAlignment(Qt::AlignCenter);
    QPointer<QLabel> proposed_player_label = create_label("Aktualny gracz: " + QString::number(proposed_player + 1) + "  " + vector_players[proposed_player]->get_player_name(), QFont("Open Sans", 20), "color: white");
    proposed_player_label->setAlignment(Qt::AlignCenter);
    QPointer<QLabel> proposed_player_finance_label = create_label("Finanse gracza: " + QString::number(vector_players[proposed_player]->get_player_money()) + "$", QFont("Open Sans", 20), "color: white");
    proposed_player_finance_label->setAlignment(Qt::AlignCenter);
    QPointer<QLabel> current_leader_label = create_label("Aktualny lider licytacji: Bank", QFont("Open Sans", 20), "color: white");
    current_leader_label->setAlignment(Qt::AlignCenter);
    QPointer<QLabel> current_payment_label = create_label("Aktualna stawka: " + QString::number(current_payment) + "$\n", QFont("Open Sans", 20), "color: white");
    current_payment_label->setAlignment(Qt::AlignCenter);

    QPointer<QSpinBox> current_players_spin = new QSpinBox(this);
    current_players_spin->setAlignment(Qt::AlignCenter);
    current_players_spin->setStyleSheet("color: white; background-color: #484852");
    current_players_spin->setMinimum(0);
    current_players_spin->setMaximum(10000);
    current_players_spin->setMinimumSize(50, 30);

    QPointer<QLabel> new_line_label = create_label("\n", QFont("Open Sans", 20), "color: white");

    QPointer<QDialogButtonBox> auction_button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    auction_button->setStyleSheet("color: white; background-color: #484852");
    auction_button->setCenterButtons(true);
    connect(auction_button, &QDialogButtonBox::accepted, auction_dialog, &QDialog::accept);
    connect(auction_button, &QDialogButtonBox::rejected, auction_dialog, &QDialog::reject);

    v_layout->addWidget(auction_label);
    v_layout->addWidget(proposed_player_label);
    v_layout->addWidget(proposed_player_finance_label);
    v_layout->addWidget(current_leader_label);
    v_layout->addWidget(current_payment_label);
    v_layout->addWidget(current_players_spin);
    v_layout->addWidget(new_line_label);
    v_layout->addWidget(auction_button);
    auction_dialog->setLayout(v_layout);

    bool end = false;
    bool bid = false;
    do
    {
        bool change = true;
        auction_dialog->exec();
        if(!auction_dialog->result())
            vector_bool_players[proposed_player] = false;
        else
        {
            if(current_players_spin->value() > vector_players[proposed_player]->get_player_money())
            {
                change = false;
                reinterpret_cast<Game*>(parent)->show_message("Masz za mało funduszy do wykonanania akcji!");
            }
            else if(current_players_spin->value() <= current_payment)
            {
                change = false;
                reinterpret_cast<Game*>(parent)->show_message("Wpisano wartość mniejszą niż aktualna stawka!");
            }
            else
            {
                bid = true;
                current_payment = current_players_spin->value();
                current_buyer = proposed_player;
            }
        }
        if(number_auction_players(vector_bool_players) == 0 || (number_auction_players(vector_bool_players) == 1 && bid))
            end = true;
        if(change && !end)
        {
            int proposed_player_copy = proposed_player;
            for(size_t i = proposed_player_copy + 1; i < vector_bool_players.size(); i++)
                if(vector_bool_players[i])
                {
                    proposed_player_copy = i;
                    break;
                }
            if(proposed_player == proposed_player_copy)
                for(int i = 0; i < proposed_player; i++)
                    if(vector_bool_players[i])
                    {
                        proposed_player_copy = i;
                        break;
                    }
            proposed_player = proposed_player_copy;

            proposed_player_label->setText("Aktualny gracz: " + QString::number(proposed_player + 1) + "  " + vector_players[proposed_player]->get_player_name());
            proposed_player_finance_label->setText("Finanse gracza: " + QString::number(vector_players[proposed_player]->get_player_money()) + "$");
            current_leader_label->setText("Aktualny lider licytacji: " + (current_buyer == -1 ? "Bank" : QString::number(current_buyer + 1) + "  " + vector_players[current_buyer]->get_player_name()));
            current_payment_label->setText("Aktualna stawka: " + QString::number(current_payment) + "$\n");
            current_players_spin->setValue(0);
        }
    }
    while(!end);
    auction_dialog->close();

    if(current_buyer != -1)
    {
        vector_players[current_buyer]->change_money(-1 * current_payment);
        vector_players[current_buyer]->add_field(actual_field);
        if(actual_field->get_classname() == "field_station")
            vector_players[current_buyer]->add_number_of_stations();
        if(actual_field->get_classname() == "field_special")
            vector_players[current_buyer]->add_number_of_special();
        actual_field->set_number_owner(current_buyer);
        actual_field->set_owner("Player");
        reinterpret_cast<Game*>(parent)->actualize_field_state(actual_field);
    }
}

int AuctionDialog::number_auction_players(std::vector<bool> vector_bool_players)
{
    int result = 0;
    for(size_t i = 0; i < vector_bool_players.size(); i++)
        if(vector_bool_players[i]) result++;
    return result;
}

QPointer<QLabel> AuctionDialog::create_label(QString text, QFont font, QString stylesheet, QRect geometry, Qt::Alignment align)
{
    QPointer<QLabel> label = new QLabel(this);
    label->setText(text);
    label->setFont(font);
    label->setStyleSheet(stylesheet);
    label->setGeometry(geometry);
    label->setAlignment(align);
    return label;
}

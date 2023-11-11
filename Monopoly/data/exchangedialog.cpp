#include "exchangedialog.h"
#include "interface/game.h"

ExchangeDialog::ExchangeDialog(QWidget* parent, std::vector<std::shared_ptr<Player>> vector_players, std::shared_ptr<Player> current_player, QPointer<QListWidget> players_offers)
    : parent(parent), vector_players(vector_players), current_player(current_player), players_offers(players_offers)
{

}

void ExchangeDialog::execute()
{
    QPointer<QDialog> exchange_dialog = new QDialog();
    exchange_dialog->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    exchange_dialog->setStyleSheet("background-color: #323236");

    QPointer<QVBoxLayout> v_layout = new QVBoxLayout(this);
    QPointer<QLabel> exchange_label = create_label("Wymiana kart\nWybierz numer gracza do wymiany:", QFont("Open Sans", 30), "color: white");
    exchange_label->setAlignment(Qt::AlignCenter);
    QPointer<QDialogButtonBox> exchange_button_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    exchange_button_box->setStyleSheet("color: white; background-color: #484852");
    exchange_button_box->setCenterButtons(true);
    connect(exchange_button_box, &QDialogButtonBox::accepted, exchange_dialog, &QDialog::accept);
    connect(exchange_button_box, &QDialogButtonBox::rejected, exchange_dialog, &QDialog::reject);
    exchange_combo = new QComboBox(this);
    exchange_combo->setStyleSheet("color: white; background-color: #484852");
    exchange_combo->addItem("");
    for(const auto& player : vector_players)
    {
        if(player == current_player || player->get_player_bankrupt()) continue;
        exchange_combo->addItem(QString::number(player->get_player_number() + 1) + "  " + player->get_player_name());
    }
    connect(exchange_combo, &QComboBox::currentTextChanged, this, &ExchangeDialog::chosen_player_changed);

    QPointer<QHBoxLayout> h_layout = new QHBoxLayout(this);
    QPointer<QLabel> player_label = create_label("Twoje finanse: " + QString::number(current_player->get_player_money()) + "$", QFont("Open Sans", 20), "color: white");
    player_label->setAlignment(Qt::AlignCenter);
    chosen_player_label = create_label("", QFont("Open Sans", 20), "color: white");
    chosen_player_label->setAlignment(Qt::AlignCenter);
    chosen_player_label->hide();
    h_layout->addWidget(player_label);
    h_layout->addWidget(chosen_player_label);

    QPointer<QHBoxLayout> h_layout2 = new QHBoxLayout(this);
    QPointer<QListWidget> current_players_list = new QListWidget(this);
    current_players_list->addItem("");
    std::vector<std::shared_ptr<Field>> current_player_properties = current_player->get_vector_players_fields();
    for(size_t i = 0; i < current_player_properties.size(); i++)
    {
        if(!(current_player_properties[i]->get_classname() == "field_property" && current_player_properties[i]->get_number_of_houses()))
        {
            current_players_list->addItem(current_player_properties[i]->get_name());
            current_players_list->item(i + 1)->setForeground(Qt::white);
        }
    }
    current_players_list->setCurrentItem(current_players_list->item(0));
    QPointer<QSpinBox> current_players_spin = new QSpinBox(this);
    current_players_spin->setStyleSheet("color: white; background-color: #484852");
    current_players_spin->setMinimum(0);
    current_players_spin->setMaximum(10000);
    current_players_spin->setMinimumSize(50, 30);

    other_players_list = new QListWidget(this);
    other_players_list->hide();
    other_players_spin = new QSpinBox(this);
    other_players_spin->setStyleSheet("color: white; background-color: #484852");
    other_players_spin->setMinimum(0);
    other_players_spin->setMaximum(10000);
    other_players_spin->setMinimumSize(50, 30);
    other_players_spin->hide();

    h_layout2->addWidget(current_players_list);
    h_layout2->addWidget(current_players_spin);
    h_layout2->addWidget(other_players_list);
    h_layout2->addWidget(other_players_spin);

    QPointer<QLabel> exchange_message = create_label("", QFont("Open Sans", 15), "color: white");

    v_layout->addWidget(exchange_label);
    v_layout->addWidget(exchange_combo);
    v_layout->addLayout(h_layout);
    v_layout->addLayout(h_layout2);
    v_layout->addWidget(exchange_message);
    v_layout->addWidget(exchange_button_box);
    exchange_dialog->setLayout(v_layout);

    bool next = false;
    do
    {
        current_players_spin->setValue(0);
        other_players_spin->setValue(0);
        exchange_dialog->exec();
        next = true;
        if(exchange_dialog->result())
        {
            next = false;
            if(exchange_combo->currentText() == "")
                exchange_message->setText("Brak wybranego gracza! Wybierz gracza!");
            else if(current_players_list->currentItem()->text() == "" && other_players_list->currentItem()->text() == "")
                exchange_message->setText("Brak wybranej nieruchomości! Wybierz nieruchomość!");
            else
            {
                int current_players_money = 0;
                int other_players_money = 0;
                if(current_players_spin->value() > other_players_spin->value())
                    current_players_money = current_players_spin->value() - other_players_spin->value();
                else if(current_players_spin->value() < other_players_spin->value())
                    other_players_money = other_players_spin->value() - current_players_spin->value();
                std::shared_ptr<Offer> offer = std::make_shared<Offer>(Offer(current_player->get_player_number(), current_players_money, current_players_list->currentItem()->text(), other_players_money, other_players_list->currentItem()->text()));
                vector_players[exchange_chosen_player]->add_offer(offer);
                reinterpret_cast<Game*>(parent)->show_message("Złożono ofertę!");
            }
        }
    } while(!next);

    exchange_dialog->close();
    delete exchange_dialog;
    exchange_dialog = nullptr;
}

QPointer<QLabel> ExchangeDialog::create_label(QString text, QFont font, QString stylesheet, QRect geometry, Qt::Alignment align)
{
    QPointer<QLabel> label = new QLabel(this);
    label->setText(text);
    label->setFont(font);
    label->setStyleSheet(stylesheet);
    label->setGeometry(geometry);
    label->setAlignment(align);
    return label;
}

void ExchangeDialog::chosen_player_changed()
{
    if(exchange_combo->currentText() == "")
    {
        exchange_chosen_player = -1;
        other_players_list->clear();
        chosen_player_label->hide();
        other_players_list->hide();
        other_players_spin->hide();
    }
    else
    {
        QString line = exchange_combo->currentText();
        exchange_chosen_player = int(line.toStdString()[0]) - 49; //-48 ASCII number
        other_players_list->clear();
        other_players_list->addItem("");
        std::vector<std::shared_ptr<Field>> other_player_properties = vector_players[exchange_chosen_player]->get_vector_players_fields();
        for(size_t i = 0; i < other_player_properties.size(); i++)
        {
            if(!(other_player_properties[i]->get_classname() == "field_property" && other_player_properties[i]->get_number_of_houses() > 0))
            {
                other_players_list->addItem(other_player_properties[i]->get_name());
                other_players_list->item(i + 1)->setForeground(Qt::white);
            }
        }
        other_players_list->setCurrentItem(other_players_list->item(0));
        chosen_player_label->setText("Finanse rywala: " + QString::number(vector_players[exchange_chosen_player]->get_player_money()) + "$");
        chosen_player_label->show();
        other_players_list->show();
        other_players_spin->show();
    }
}

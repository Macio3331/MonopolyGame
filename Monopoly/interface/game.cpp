#include "game.h"

Game::Game(QWidget *parent, QVector<QString> vector_name_players, int number_of_players, bool auction)
    : QWidget{parent}, vector_name_players(vector_name_players), number_of_players(number_of_players), auction(auction)
{
    create_vector_fields();
    initialize_vector_estates();
    initialize_players();
    initialize_dices();
    initialize_chequers_table();
    initialize_interface();
    initialize_chequers();
    initialize_field_buttons();
    initialize_dice_buttons();
}

Game::~Game()
{

}

QLabel *Game::create_pixmap_label(QString path, QRect geometry, bool scaled, int scale_width, int scale_height, bool fill, QColor color)
{
    QLabel *label = new QLabel(this);
    QPixmap p = QPixmap(path);
    if(fill)
        p.fill(color);
    if(scaled)
         p = p.scaled(scale_width, scale_height);
    label->setPixmap(p);
    label->setPixmap(QPixmap(p));
    label->setGeometry(geometry);
    return label;
}

QLabel *Game::create_label(QString text, QFont font, QString stylesheet, QRect geometry, Qt::Alignment align)
{
    QLabel *label = new QLabel(this);
    label->setText(text);
    label->setFont(font);
    label->setStyleSheet(stylesheet);
    label->setGeometry(geometry);
    label->setAlignment(align);
    return label;
}

QPushButton *Game::create_pushbutton(QString text, QFont font, QString stylesheet, QRect geometry, bool enabled)
{
    QPushButton *button = new QPushButton(this);
    button->setText(text);
    button->setFont(font);
    button->setStyleSheet(stylesheet);
    button->setGeometry(geometry);
    if(!enabled) button->setEnabled(enabled);
    return button;
}

void Game::initialize_interface()
{
    board = create_pixmap_label(":/board/board/board.jpg", QRect(460, 0, 1000, 1000), true, 1000, 1000);
    player_label = create_label(QString::number(vector_players[0]->get_player_number() + 1) + "  " + vector_players[0]->get_player_name(), QFont("Open Sans", 30), "color: white", QRect(0, 0, 360, 60));
    color_player_label = create_pixmap_label(":/board/board/checker.png", QRect(390, 0, 60, 60), false, 0, 0, true, vector_players[0]->get_player_color());
    money_label = create_label("Finanse:  " + QString::number(vector_players[0]->get_player_money()) + "$", QFont("Open Sans", 30), "color: white", QRect(0, 60, 450, 60));

    dice1_label = create_pixmap_label(":/board/board/dice_1.png", QRect(620, 790, 50, 50), true, 50, 50);
    dice2_label = create_pixmap_label(":/board/board/dice_1.png", QRect(680, 790, 50, 50), true, 50, 50);

    build_house_button = create_pushbutton("Postaw budynek", QFont("Open Sans", 30), "color: white; background-color: #484852", QRect(0, 140, 450, 60));
    connect(build_house_button, SIGNAL(clicked()), this, SLOT(on_build_house_button_clicked()));
    sell_house_button = create_pushbutton("Sprzedaj budynek", QFont("Open Sans", 30), "color: white; background-color: #484852", QRect(0, 210, 450, 60));
    connect(sell_house_button, SIGNAL(clicked()), this, SLOT(on_sell_house_button_clicked()));
    exchange_button = create_pushbutton("Zaoferuj wymianę", QFont("Open Sans", 30), "color: white; background-color: #484852", QRect(0, 280, 450, 60));
    connect(exchange_button, SIGNAL(clicked()), this, SLOT(on_exchange_button_clicked()));
    offers_button = create_pushbutton("Lista ofert", QFont("Open Sans", 30), "color: white; background-color: #484852", QRect(0, 350, 450, 60));
    connect(offers_button, SIGNAL(clicked()), this, SLOT(on_offers_button_clicked()));
    hypothecation_button = create_pushbutton("Zastaw nieruchomość", QFont("Open Sans", 30), "color: white; background-color: #484852", QRect(0, 420, 450, 60));
    connect(hypothecation_button, SIGNAL(clicked()), this, SLOT(on_hypothecation_button_clicked()));
    end_turn_button = create_pushbutton("Zakończ turę", QFont("Open Sans", 30), "color: white; background-color: #484852", QRect(0, 490, 450, 60), false);
    connect(end_turn_button, SIGNAL(clicked()), this, SLOT(on_end_turn_button_clicked()));

    action_label = create_label("Zdarzenie:", QFont("Open Sans", 30), "color: white", QRect(0, 600, 450, 60), Qt::AlignCenter);
    action_name_label = create_label("", QFont("Open Sans", 25), "color: white", QRect(0, 660, 450, 50), Qt::AlignCenter);
    action_description_label = create_label("", QFont("Open Sans", 16), "color: white", QRect(0, 710, 450, 150), Qt::AlignCenter);
    action_description_label->setWordWrap(true);

    accept_button = create_pushbutton("Tak", QFont("Open Sans", 25), "color: white; background-color: #484852", QRect(90, 880, 90, 40));
    connect(accept_button, SIGNAL(clicked()), this, SLOT(on_accept_button_clicked())); 
    ok_button = create_pushbutton("Ok", QFont("Open Sans", 25), "color: white; background-color: #484852", QRect(180, 880, 90, 40));
    connect(ok_button, SIGNAL(clicked()), this, SLOT(on_ok_button_clicked()));
    deny_button = create_pushbutton("Nie", QFont("Open Sans", 25), "color: white; background-color: #484852", QRect(270, 880, 90, 40));
    connect(deny_button, SIGNAL(clicked()), this, SLOT(on_deny_button_clicked()));

    message = create_label("", QFont("Open Sans", 16), "color: white", QRect(0, 920, 450, 80), Qt::AlignCenter);
    message->setWordWrap(true);

    player_cards_label = create_label("Karty gracza:", QFont("Open Sans", 30), "color: white", QRect(1470, 0, 450, 60), Qt::AlignCenter);
    card_label = create_label("Karta:", QFont("Open Sans", 30), "color: white", QRect(1470, 650, 450, 60), Qt::AlignCenter);
    card_name_label = create_label("Lipiny Bukowego", QFont("Open Sans", 25), "color: white", QRect(1470, 710, 450, 50), Qt::AlignCenter);
    card_description_label = create_label("Cena zakupu: 200$"
                                            "\n1 dom: 120$"
                                            "\n2 domy: 310$"
                                            "\n3 domy: 520$"
                                            "\n4 domy: 680$"
                                            "\nHotel: 900$"
                                            "\nCena za 1 dom: 100$",
                                            QFont("Open Sans", 16), "color: white", QRect(1470, 760, 450, 200), Qt::AlignCenter);
    card_description_label->setWordWrap(true);

    action_label->hide();
    action_name_label->hide();
    action_description_label->hide();
    card_label->hide();
    card_name_label->hide();
    card_description_label->hide();
    accept_button->hide();
    ok_button->hide();
    deny_button->hide();
}

void Game::initialize_field_buttons()
{
    int width = 0, height = 0, offset_width = 0, offset_height = 0;
    for(int i = 0; i < 40; i++)
    {
         if(i % 10 == 0)
         {
            width = 133;
            height = 133;
         }
         else if(i < 10 || (i > 20 && i < 30))
         {
            width = 81;
            height = 133;
         }
         else
         {
            width = 133;
            height = 81;
         }

         if(i == 0)
         {
            offset_width = 1329;
            offset_height = 868;
         }
         else if(i < 10)
         {
            offset_width = 1329 - 82 * i;
            offset_height = 868;
         }
         else if(i == 10)
         {
            offset_width = 459;
            offset_height = 868;
         }
         else if(i > 10 && i < 20)
         {
            offset_width = 459;
            offset_height = 869 - 82 * (i - 10);
         }
         else if(i == 20)
         {
            offset_width = 459;
            offset_height = -1;
         }
         else if(i > 20 && i < 30)
         {
            offset_width = 591 + 82 * (i - 21);
            offset_height = -1;
         }
         else if(i == 30)
         {
            offset_width = 1329;
            offset_height = -1;
         }
         else if(i > 30)
         {
            offset_width = 1329;
            offset_height = 131 + 82 * (i - 31);
         }

         QPushButton *field_button = create_pushbutton("", QFont(), "background: transparent", QRect(offset_width, offset_height, width, height));
         connect(field_button, SIGNAL(clicked()), this, SLOT(on_field_button_clicked()));
         vector_board_buttons.push_back(field_button);
    }
}

void Game::create_vector_fields()
{
    QVector<QString> vector_names_chance_fields = {"Kasa społeczna", "Szansa"};
    QVector<Field*> vector_fields_chance_help;
    for(int i = 0; i < 6; i++)
        if(i % 2 == 0)
             vector_fields_chance_help.push_back(create_field_chance(vector_names_chance_fields[0], "Weź kartę z dodatkowej puli kart"));
        else
             vector_fields_chance_help.push_back(create_field_chance(vector_names_chance_fields[1], "Weź kartę z dodatkowej puli kart"));

    QVector<std::pair<QString, QString>> vector_names_fields = {{"Pole startowe", "W chwili przejścia przez start pobierz 200$"},
                                                                            {"Podatek dochodowy", "Zapłać 200$"},
                                                                            {"Więzienie", "Wyjdź z więzienia przez wyrzucenie dubletu"
                                                                                          "\nlub zapłatę kaucji w wysokości 50$"},
                                                                            {"Bezpłatny parking", "Darmowy postój na parkingu"},
                                                                            {"Idź do więzienia", "Przejdź prosto do więzienia"
                                                                                                 "\nNie przechodź przez start"
                                                                                                 "\nNie pobieraj 200$"},
                                                                            {"Domiar podatkowy", "Zapłać 100$"}};
    QVector<Field*> vector_fields_help;
    for(int i = 0; i < 6; i++)
        vector_fields_help.push_back(create_field(vector_names_fields[i].first, vector_names_fields[i].second));

    QVector<QString> vector_names_station_fields = {"Dworzec Reading", "Dworzec Pensylvania", "Dworzec B. &. O", "Dworzec Short Line"};
    QVector<Field*> vector_fields_station_help;
    for(int i = 0; i < 4; i++)
        vector_fields_station_help.push_back(create_field_station(vector_names_station_fields[i], "Właściciel podanej ilości dworców pobiera:"
                                                                                                  "\n1 dworzec - 25$"
                                                                                                  "\n2 dworce - 50$"
                                                                                                  "\n3 dworce - 100$"
                                                                                                  "\n4 dworce - 200$"
                                                                                                  "\nKoszt zakupu: 200$"));

    QVector<QString> vector_names_special_fields = {"Elektrownia", "Wodociągi"};
    QVector<Field*> vector_fields_special_help;
    for(int i = 0; i < 2; i++)
        vector_fields_special_help.push_back(create_field_special(vector_names_special_fields[i], "Właściciel jednego obiektu"
                                                                                                  "\nużyteczności publicznej pobiera"
                                                                                                  "\nczterokrotność sumy oczek;"
                                                                                                  "\nWłaściciel obu obiektów"
                                                                                                  "\nużyteczności publicznej pobiera"
                                                                                                  "\ndziesięciokrotną sumę oczek"
                                                                                                  "\nCena zakupu: 150$"));

    QVector<QString> vector_names_properties_fields = {"Mediter-Ranean Avenue", "Baltic Avenue", "Oriental Avenue", "Vermont Avenue", "Connecticut Avenue",
                                                    "St. Charles Place", "States Avenue", "Virginia Avenue", "St. James Place", "Tennessee Avenue",
                                                    "New York Avenue", "Kentucky Avenue", "Indiana Avenue", "Illinois Avenue", "Atlantic Avenue",
                                                    "Ventnor Avenue", "Marvin Gardens", "Pacific Avenue", "North Carolina Avenue", "Pensylvania Avenue",
                                                    "Park Place", "Boardwalk"};
    QVector<std::vector<int>> vector_of_payments = {{2, 10, 30, 90, 160, 250}, {4, 20, 60, 180, 320, 450}, {6, 30, 90, 270, 400, 550}, {6, 30, 90, 270, 400, 550}, {8, 40, 100, 300, 450, 600},
                                                        {10, 50, 150, 450, 625, 750}, {10, 50, 150, 450, 625, 750}, {12, 60, 180, 500, 700, 900}, {14, 70, 200, 550, 750, 950}, {14, 70, 200, 550, 750, 950},
                                                        {16, 80, 220, 600, 800, 1000}, {18, 90, 250, 700, 875, 1050}, {18, 90, 250, 700, 875, 1050}, {20, 100, 300, 750, 925, 1100}, {22, 110, 330, 800, 975, 1150},
                                                        {22, 110, 330, 800, 975, 1150}, {24, 120, 360, 850, 1025, 1200}, {26, 130, 390, 900, 1100, 1275}, {26, 130, 390, 900, 1100, 1275}, {28, 150, 450, 1000, 1200, 1400},
                                                        {35, 175, 500, 1100, 1300, 1500}, {50, 200, 600, 1400, 1700, 2000}};
    QVector<int> vector_buying_costs = {60, 60, 100, 100, 120, 140, 140, 160, 180, 180, 200, 220, 220, 240, 260, 260, 280, 300, 300, 320, 350, 400};
    QVector<int> vector_building_costs = {50, 50, 50, 50, 50, 100, 100, 100, 100, 100, 100, 150, 150, 150, 150, 150, 150, 200, 200, 200, 200, 200};
    QVector<Field*> vector_fields_properties_help;
    for(int i = 0; i < 22; i++)
        vector_fields_properties_help.push_back(
            create_field_property(vector_names_properties_fields[i],
                                  "Cena zakupu: " + QString::number(vector_buying_costs[i]) + "$" +
                                  "\nObszar niezabudowany: " + QString::number(vector_of_payments[i][0]) + "$" +
                                  "\n1 dom: " + QString::number(vector_of_payments[i][1]) + "$" +
                                  "\n2 domy: " + QString::number(vector_of_payments[i][2]) + "$" +
                                  "\n3 domy: " + QString::number(vector_of_payments[i][3]) + "$" +
                                  "\n4 domy: " + QString::number(vector_of_payments[i][4]) + "$" +
                                  "\nHotel: " + QString::number(vector_of_payments[i][5]) + "$" +
                                  "\nCena za 1 dom: " + QString::number(vector_building_costs[i]) + "$",
                                  vector_of_payments[i], vector_buying_costs[i], vector_building_costs[i])
            );

    layer_fields(vector_fields_chance_help, vector_fields_help, vector_fields_station_help, vector_fields_special_help, vector_fields_properties_help);
}

void Game::layer_fields(QVector<Field*>& vector_fields_chance_help, QVector<Field*>& vector_fields_help, QVector<Field*>& vector_fields_station_help, QVector<Field*>& vector_fields_special_help, QVector<Field*>& vector_fields_properties_help)
{
    //down
    vector_fields.push_back(vector_fields_help[0]);
    vector_fields.push_back(vector_fields_properties_help[0]);
    vector_fields.push_back(vector_fields_chance_help[0]);
    vector_fields.push_back(vector_fields_properties_help[1]);
    vector_fields.push_back(vector_fields_help[1]);
    vector_fields.push_back(vector_fields_station_help[0]);
    vector_fields.push_back(vector_fields_properties_help[2]);
    vector_fields.push_back(vector_fields_chance_help[1]);
    vector_fields.push_back(vector_fields_properties_help[3]);
    vector_fields.push_back(vector_fields_properties_help[4]);
    //left
    vector_fields.push_back(vector_fields_help[2]);
    vector_fields.push_back(vector_fields_properties_help[5]);
    vector_fields.push_back(vector_fields_special_help[0]);
    vector_fields.push_back(vector_fields_properties_help[6]);
    vector_fields.push_back(vector_fields_properties_help[7]);
    vector_fields.push_back(vector_fields_station_help[1]);
    vector_fields.push_back(vector_fields_properties_help[8]);
    vector_fields.push_back(vector_fields_chance_help[2]);
    vector_fields.push_back(vector_fields_properties_help[9]);
    vector_fields.push_back(vector_fields_properties_help[10]);
    //up
    vector_fields.push_back(vector_fields_help[3]);
    vector_fields.push_back(vector_fields_properties_help[11]);
    vector_fields.push_back(vector_fields_chance_help[3]);
    vector_fields.push_back(vector_fields_properties_help[12]);
    vector_fields.push_back(vector_fields_properties_help[13]);
    vector_fields.push_back(vector_fields_station_help[2]);
    vector_fields.push_back(vector_fields_properties_help[14]);
    vector_fields.push_back(vector_fields_properties_help[15]);
    vector_fields.push_back(vector_fields_special_help[1]);
    vector_fields.push_back(vector_fields_properties_help[16]);
    //right
    vector_fields.push_back(vector_fields_help[4]);
    vector_fields.push_back(vector_fields_properties_help[17]);
    vector_fields.push_back(vector_fields_properties_help[18]);
    vector_fields.push_back(vector_fields_chance_help[4]);
    vector_fields.push_back(vector_fields_properties_help[19]);
    vector_fields.push_back(vector_fields_station_help[3]);
    vector_fields.push_back(vector_fields_chance_help[3]);
    vector_fields.push_back(vector_fields_properties_help[20]);
    vector_fields.push_back(vector_fields_help[5]);
    vector_fields.push_back(vector_fields_properties_help[21]);

    for(int i = 0; i < vector_fields_help.size(); i++)
        vector_fields_help[i] = nullptr;
    for(int i = 0; i < vector_fields_chance_help.size(); i++)
        vector_fields_chance_help[i] = nullptr;
    for(int i = 0; i < vector_fields_station_help.size(); i++)
        vector_fields_station_help[i] = nullptr;
    for(int i = 0; i < vector_fields_special_help.size(); i++)
        vector_fields_special_help[i] = nullptr;
    for(int i = 0; i < vector_fields_properties_help.size(); i++)
        vector_fields_properties_help[i] = nullptr;
}

void Game::change_dices_labels(int dice_number, QLabel *dice_label)
{
    switch(dice_number)
    {
        case 1:
            dice_label->setPixmap(QPixmap(":/board/board/dice_1.png").scaled(50, 50));
            break;
        case 2:
            dice_label->setPixmap(QPixmap(":/board/board/dice_2.png").scaled(50, 50));
            break;
        case 3:
            dice_label->setPixmap(QPixmap(":/board/board/dice_3.png").scaled(50, 50));
            break;
        case 4:
            dice_label->setPixmap(QPixmap(":/board/board/dice_4.png").scaled(50, 50));
            break;
        case 5:
            dice_label->setPixmap(QPixmap(":/board/board/dice_5.png").scaled(50, 50));
            break;
        case 6:
            dice_label->setPixmap(QPixmap(":/board/board/dice_6.png").scaled(50, 50));
            break;
    }
}

void Game::initialize_chequers()
{
    for(int i = 0; i < number_of_players; i++)
            vector_chequers.push_back(create_pixmap_label(":/board/board/checker.png", chequers_position_table[0][i], true, 15, 15, true, vector_players[i]->get_player_color()));
}

void Game::initialize_chequers_table()
{
    chequers_position_table = {
                                { QRect(1370, 891, 15, 15), QRect(1405, 891, 15, 15), QRect(1352, 926, 15, 15), QRect(1387, 926, 15, 15), QRect(1422, 926, 15, 15), QRect(1352, 961, 15, 15), QRect(1387, 961, 15, 15), QRect(1422, 961, 15, 15) },
                                //bottom
                                { QRect(1267, 903, 15, 15), QRect(1292, 903, 15, 15), QRect(1267, 928, 15, 15), QRect(1292, 928, 15, 15), QRect(1267, 953, 15, 15), QRect(1292, 953, 15, 15), QRect(1267, 978, 15, 15), QRect(1292, 978, 15, 15) },
                                { QRect(1185, 903, 15, 15), QRect(1210, 903, 15, 15), QRect(1185, 928, 15, 15), QRect(1210, 928, 15, 15), QRect(1185, 953, 15, 15), QRect(1210, 953, 15, 15), QRect(1185, 978, 15, 15), QRect(1210, 978, 15, 15) },
                                { QRect(1103, 903, 15, 15), QRect(1128, 903, 15, 15), QRect(1103, 928, 15, 15), QRect(1128, 928, 15, 15), QRect(1103, 953, 15, 15), QRect(1128, 953, 15, 15), QRect(1103, 978, 15, 15), QRect(1128, 978, 15, 15) },
                                { QRect(1021, 903, 15, 15), QRect(1046, 903, 15, 15), QRect(1021, 928, 15, 15), QRect(1046, 928, 15, 15), QRect(1021, 953, 15, 15), QRect(1046, 953, 15, 15), QRect(1021, 978, 15, 15), QRect(1046, 978, 15, 15) },
                                { QRect(939, 903, 15, 15), QRect(964, 903, 15, 15), QRect(939, 928, 15, 15), QRect(964, 928, 15, 15), QRect(939, 953, 15, 15), QRect(964, 953, 15, 15), QRect(939, 978, 15, 15), QRect(964, 978, 15, 15) },
                                { QRect(857, 903, 15, 15), QRect(882, 903, 15, 15), QRect(857, 928, 15, 15), QRect(882, 928, 15, 15), QRect(857, 953, 15, 15), QRect(882, 953, 15, 15), QRect(857, 978, 15, 15), QRect(882, 978, 15, 15) },
                                { QRect(775, 903, 15, 15), QRect(800, 903, 15, 15), QRect(775, 928, 15, 15), QRect(800, 928, 15, 15), QRect(775, 953, 15, 15), QRect(800, 953, 15, 15), QRect(775, 978, 15, 15), QRect(800, 978, 15, 15) },
                                { QRect(693, 903, 15, 15), QRect(718, 903, 15, 15), QRect(693, 928, 15, 15), QRect(718, 928, 15, 15), QRect(693, 953, 15, 15), QRect(718, 953, 15, 15), QRect(693, 978, 15, 15), QRect(718, 978, 15, 15) },
                                { QRect(611, 903, 15, 15), QRect(636, 903, 15, 15), QRect(611, 928, 15, 15), QRect(636, 928, 15, 15), QRect(611, 953, 15, 15), QRect(636, 953, 15, 15), QRect(611, 978, 15, 15), QRect(636, 978, 15, 15) },
                                //
                                { QRect(471, 873, 15, 15), QRect(471, 898, 15, 15), QRect(471, 923, 15, 15), QRect(471, 948, 15, 15), QRect(498, 973, 15, 15), QRect(523, 973, 15, 15), QRect(548, 973, 15, 15), QRect(573, 973, 15, 15) },
                                //left
                                { QRect(539, 808, 15, 15), QRect(539, 833, 15, 15), QRect(514, 808, 15, 15), QRect(514, 833, 15, 15), QRect(489, 808, 15, 15), QRect(489, 833, 15, 15), QRect(464, 808, 15, 15), QRect(464, 833, 15, 15) },
                                { QRect(539, 726, 15, 15), QRect(539, 751, 15, 15), QRect(514, 726, 15, 15), QRect(514, 751, 15, 15), QRect(489, 726, 15, 15), QRect(489, 751, 15, 15), QRect(464, 726, 15, 15), QRect(464, 751, 15, 15) },
                                { QRect(539, 644, 15, 15), QRect(539, 669, 15, 15), QRect(514, 644, 15, 15), QRect(514, 669, 15, 15), QRect(489, 644, 15, 15), QRect(489, 669, 15, 15), QRect(464, 644, 15, 15), QRect(464, 669, 15, 15) },
                                { QRect(539, 562, 15, 15), QRect(539, 587, 15, 15), QRect(514, 562, 15, 15), QRect(514, 587, 15, 15), QRect(489, 562, 15, 15), QRect(489, 587, 15, 15), QRect(464, 562, 15, 15), QRect(464, 587, 15, 15) },
                                { QRect(539, 480, 15, 15), QRect(539, 505, 15, 15), QRect(514, 480, 15, 15), QRect(514, 505, 15, 15), QRect(489, 480, 15, 15), QRect(489, 505, 15, 15), QRect(464, 480, 15, 15), QRect(464, 505, 15, 15) },
                                { QRect(539, 398, 15, 15), QRect(539, 423, 15, 15), QRect(514, 398, 15, 15), QRect(514, 423, 15, 15), QRect(489, 398, 15, 15), QRect(489, 423, 15, 15), QRect(464, 398, 15, 15), QRect(464, 423, 15, 15) },
                                { QRect(539, 316, 15, 15), QRect(539, 341, 15, 15), QRect(514, 316, 15, 15), QRect(514, 341, 15, 15), QRect(489, 316, 15, 15), QRect(489, 341, 15, 15), QRect(464, 316, 15, 15), QRect(464, 341, 15, 15) },
                                { QRect(539, 234, 15, 15), QRect(539, 259, 15, 15), QRect(514, 234, 15, 15), QRect(514, 259, 15, 15), QRect(489, 234, 15, 15), QRect(489, 259, 15, 15), QRect(464, 234, 15, 15), QRect(464, 259, 15, 15) },
                                { QRect(539, 152, 15, 15), QRect(539, 177, 15, 15), QRect(514, 152, 15, 15), QRect(514, 177, 15, 15), QRect(489, 152, 15, 15), QRect(489, 177, 15, 15), QRect(464, 152, 15, 15), QRect(464, 177, 15, 15) },
                                //
                                { QRect(552, 38, 15, 15), QRect(552, 73, 15, 15), QRect(517, 23, 15, 15), QRect(517, 58, 15, 15), QRect(517, 93, 15, 15), QRect(482, 23, 15, 15), QRect(482, 58, 15, 15), QRect(482, 93, 15, 15) },
                                //up
                                { QRect(637, 79, 15, 15), QRect(612, 79, 15, 15), QRect(637, 54, 15, 15), QRect(612, 54, 15, 15), QRect(637, 29, 15, 15), QRect(612, 29, 15, 15), QRect(637, 4, 15, 15), QRect(612, 4, 15, 15) },
                                { QRect(719, 79, 15, 15), QRect(694, 79, 15, 15), QRect(719, 54, 15, 15), QRect(694, 54, 15, 15), QRect(719, 29, 15, 15), QRect(694, 29, 15, 15), QRect(719, 4, 15, 15), QRect(694, 4, 15, 15) },
                                { QRect(801, 79, 15, 15), QRect(776, 79, 15, 15), QRect(801, 54, 15, 15), QRect(776, 54, 15, 15), QRect(801, 29, 15, 15), QRect(776, 29, 15, 15), QRect(801, 4, 15, 15), QRect(776, 4, 15, 15) },
                                { QRect(883, 79, 15, 15), QRect(858, 79, 15, 15), QRect(883, 54, 15, 15), QRect(858, 54, 15, 15), QRect(883, 29, 15, 15), QRect(858, 29, 15, 15), QRect(883, 4, 15, 15), QRect(858, 4, 15, 15) },
                                { QRect(965, 79, 15, 15), QRect(940, 79, 15, 15), QRect(965, 54, 15, 15), QRect(940, 54, 15, 15), QRect(965, 29, 15, 15), QRect(940, 29, 15, 15), QRect(965, 4, 15, 15), QRect(940, 4, 15, 15) },
                                { QRect(1047, 79, 15, 15), QRect(1022, 79, 15, 15), QRect(1047, 54, 15, 15), QRect(1022, 54, 15, 15), QRect(1047, 29, 15, 15), QRect(1022, 29, 15, 15), QRect(1047, 4, 15, 15), QRect(1022, 4, 15, 15) },
                                { QRect(1129, 79, 15, 15), QRect(1104, 79, 15, 15), QRect(1129, 54, 15, 15), QRect(1104, 54, 15, 15), QRect(1129, 29, 15, 15), QRect(1104, 29, 15, 15), QRect(1129, 4, 15, 15), QRect(1104, 4, 15, 15) },
                                { QRect(1211, 79, 15, 15), QRect(1186, 79, 15, 15), QRect(1211, 54, 15, 15), QRect(1186, 54, 15, 15), QRect(1211, 29, 15, 15), QRect(1186, 29, 15, 15), QRect(1211, 4, 15, 15), QRect(1186, 4, 15, 15) },
                                { QRect(1293, 79, 15, 15), QRect(1268, 79, 15, 15), QRect(1293, 54, 15, 15), QRect(1268, 54, 15, 15), QRect(1293, 29, 15, 15), QRect(1268, 29, 15, 15), QRect(1293, 4, 15, 15), QRect(1268, 4, 15, 15) },
                                //
                                { QRect(1405, 92, 15, 15), QRect(1370, 92, 15, 15), QRect(1422, 57, 15, 15), QRect(1387, 57, 15, 15), QRect(1352, 57, 15, 15), QRect(1422, 22, 15, 15), QRect(1387, 22, 15, 15), QRect(1352, 22, 15, 15) },
                                //right
                                { QRect(1364, 177, 15, 15), QRect(1364, 152, 15, 15), QRect(1389, 177, 15, 15), QRect(1389, 152, 15, 15), QRect(1414, 177, 15, 15), QRect(1414, 152, 15, 15), QRect(1439, 177, 15, 15), QRect(1439, 152, 15, 15) },
                                { QRect(1364, 259, 15, 15), QRect(1364, 234, 15, 15), QRect(1389, 259, 15, 15), QRect(1389, 234, 15, 15), QRect(1414, 259, 15, 15), QRect(1414, 234, 15, 15), QRect(1439, 259, 15, 15), QRect(1439, 234, 15, 15) },
                                { QRect(1364, 341, 15, 15), QRect(1364, 316, 15, 15), QRect(1389, 341, 15, 15), QRect(1389, 316, 15, 15), QRect(1414, 341, 15, 15), QRect(1414, 316, 15, 15), QRect(1439, 341, 15, 15), QRect(1439, 316, 15, 15) },
                                { QRect(1364, 423, 15, 15), QRect(1364, 398, 15, 15), QRect(1389, 423, 15, 15), QRect(1389, 398, 15, 15), QRect(1414, 423, 15, 15), QRect(1414, 398, 15, 15), QRect(1439, 423, 15, 15), QRect(1439, 398, 15, 15) },
                                { QRect(1364, 505, 15, 15), QRect(1364, 480, 15, 15), QRect(1389, 505, 15, 15), QRect(1389, 480, 15, 15), QRect(1414, 505, 15, 15), QRect(1414, 480, 15, 15), QRect(1439, 505, 15, 15), QRect(1439, 480, 15, 15) },
                                { QRect(1364, 587, 15, 15), QRect(1364, 562, 15, 15), QRect(1389, 587, 15, 15), QRect(1389, 562, 15, 15), QRect(1414, 587, 15, 15), QRect(1414, 562, 15, 15), QRect(1439, 587, 15, 15), QRect(1439, 562, 15, 15) },
                                { QRect(1364, 669, 15, 15), QRect(1364, 644, 15, 15), QRect(1389, 669, 15, 15), QRect(1389, 644, 15, 15), QRect(1414, 669, 15, 15), QRect(1414, 644, 15, 15), QRect(1439, 669, 15, 15), QRect(1439, 644, 15, 15) },
                                { QRect(1364, 751, 15, 15), QRect(1364, 726, 15, 15), QRect(1389, 751, 15, 15), QRect(1389, 726, 15, 15), QRect(1414, 751, 15, 15), QRect(1414, 726, 15, 15), QRect(1439, 751, 15, 15), QRect(1439, 726, 15, 15) },
                                { QRect(1364, 833, 15, 15), QRect(1364, 808, 15, 15), QRect(1389, 833, 15, 15), QRect(1389, 808, 15, 15), QRect(1414, 833, 15, 15), QRect(1414, 808, 15, 15), QRect(1439, 833, 15, 15), QRect(1439, 808, 15, 15) },
                                //prison
                                { QRect(525, 883, 15, 15), QRect(550, 883, 15, 15), QRect(513, 908, 15, 15), QRect(538, 908, 15, 15), QRect(563, 908, 15, 15), QRect(513, 933, 15, 15), QRect(538, 933, 15, 15), QRect(563, 933, 15, 15) }
                              };
}

void Game::initialize_players()
{
    current_player = 0;
    QVector<QColor> color_players_help = { Qt::white, Qt::black, Qt::red, Qt::green, Qt::yellow, Qt::lightGray, Qt::blue, Qt::magenta };
    for(int i = 0; i < number_of_players; i++)
        vector_players.push_back(create_player(vector_name_players[i], color_players_help[i], i));
}

void Game::initialize_dices()
{
    dublet = false;
    dublet_turn = 0;
    dice1 = new Dice();
    dice2 = new Dice();
}

void Game::initialize_vector_estates()
{
    std::vector<Field*> estate;
    estate.push_back(vector_fields[1]);
    estate.push_back(vector_fields[3]);
    vector_estates.push_back(estate);
    estate.clear();
    estate.push_back(vector_fields[6]);
    estate.push_back(vector_fields[8]);
    estate.push_back(vector_fields[9]);
    vector_estates.push_back(estate);
    estate.clear();
    estate.push_back(vector_fields[11]);
    estate.push_back(vector_fields[13]);
    estate.push_back(vector_fields[14]);
    vector_estates.push_back(estate);
    estate.clear();
    estate.push_back(vector_fields[16]);
    estate.push_back(vector_fields[18]);
    estate.push_back(vector_fields[19]);
    vector_estates.push_back(estate);
    estate.clear();
    estate.push_back(vector_fields[21]);
    estate.push_back(vector_fields[23]);
    estate.push_back(vector_fields[24]);
    vector_estates.push_back(estate);
    estate.clear();
    estate.push_back(vector_fields[26]);
    estate.push_back(vector_fields[27]);
    estate.push_back(vector_fields[29]);
    vector_estates.push_back(estate);
    estate.clear();
    estate.push_back(vector_fields[31]);
    estate.push_back(vector_fields[32]);
    estate.push_back(vector_fields[34]);
    vector_estates.push_back(estate);
    estate.clear();
    estate.push_back(vector_fields[37]);
    estate.push_back(vector_fields[39]);
    vector_estates.push_back(estate);
    estate.clear();
}

void Game::check_position(Field* actual_field)
{
    dice1_button->setEnabled(false);
    dice2_button->setEnabled(false);
    end_turn_button->setEnabled(false);
    action_label->show();
    action_name_label->show();
    action_description_label->show();
    QString class_type = actual_field->get_classname();
    int entrance_cost = find_entrance_cost(actual_field);
    if(class_type == "field_property" || class_type == "field_station" || class_type == "field_special")
    {
        if(actual_field->get_owner() == "Bank")
        {
            action_name_label->setText("Zakup pola");
            action_description_label->setText("Czy chcesz kupić " + actual_field->get_name() + " za kwotę " + ( actual_field->get_popped() ? QString::number(actual_field->get_buying_cost() * 0.55) : QString::number(actual_field->get_buying_cost()))  + "$?");
            accept_button->show();
            deny_button->show();
        }
        else if(actual_field->get_number_owner() == vector_players[current_player]->get_player_number())
        {
            action_name_label->setText("Wejście na nieruchomość");
            action_description_label->setText("Nieruchomość " + actual_field->get_name() + " należy do Ciebie, nie musisz wnosić żadnych opłat");
            ok_button->show();
        }
        else
        {
            action_name_label->setText("Wejście na nieruchomość");
            if(actual_field->get_popped())
                action_description_label->setText("Nieruchomość " + actual_field->get_name() + " jest zastawiona, nie musisz wnosić żadnych opłat");
            else
                action_description_label->setText("Wszedłeś na nieruchomość " + actual_field->get_name() + ", opłata wynosi " + QString::number(entrance_cost) + "$");
            ok_button->show();
        }
    }
    else if(class_type == "field_chance")
    {
        action_name_label->setText("Szansa");
        action_description_label->setText("Tutaj będzie obsługa karty szansa/kasa społeczna"); // TODO
        ok_button->show();
    }
    else
    {
        action_name_label->setText(actual_field->get_name());
        if(actual_field->get_name() == "Pole startowe")
            action_description_label->setText("Wszedłeś na pole startowe, otrzymujesz 200$");
        else if(actual_field->get_name() == "Bezpłatny parking")
            action_description_label->setText("Wszedłeś na bezpłatny parking, możesz odpocząć");
        else if(actual_field->get_name() == "Więzienie")
            action_description_label->setText("Odwiedzasz więzienie, możesz odpocząć");
        else if(actual_field->get_name() == "Domiar podatkowy" || actual_field->get_name() == "Podatek dochodowy")
            action_description_label->setText("Musisz zapłacić podatek o wartości " + QString::number(entrance_cost) + "$");
        ok_button->show();
    }
}

int Game::find_entrance_cost(Field *actual_field)
{
    int result = 0;
    if(actual_field->get_classname() == "field")
    {
        if(actual_field->get_name() == "Podatek dochodowy")
            result = 200;
        if(actual_field->get_name() == "Domiar podatkowy")
            result = 100;
    }
    else if(actual_field->get_owner() != "Bank")
    {
        int number_player_property = actual_field->get_number_owner();
        if(actual_field->get_classname() == "field_station")
        {
            if(vector_players[number_player_property]->get_player_number_of_stations() == 1)
                result = 25;
            if(vector_players[number_player_property]->get_player_number_of_stations() == 2)
                result = 50;
            if(vector_players[number_player_property]->get_player_number_of_stations() == 3)
                result = 100;
            if(vector_players[number_player_property]->get_player_number_of_stations() == 4)
                result = 200;
        }
        else if(actual_field->get_classname() == "field_special")
        {
            if(vector_players[number_player_property]->get_player_number_of_special() == 1)
                result = (dice1->get_number() + dice2->get_number()) * 4;
            if(vector_players[number_player_property]->get_player_number_of_special() == 2)
                result = (dice1->get_number() + dice2->get_number()) * 10;
        }
        else if(actual_field->get_classname() == "field_property")
        {
            std::pair<bool, bool> estate = find_estate(actual_field, number_player_property);
            if(estate.first && !estate.second)
                result = actual_field->get_payment(0) * 2;
            else
                result = actual_field->get_payment(actual_field->get_number_of_houses());
        }
    }
    return result;
}

std::pair<bool, bool> Game::find_estate(Field *actual_field, int number_player_property)
{
    std::vector<bool> found_field = { false, false, false };
    bool found_field_houses = false;
    std::vector<Field*> vector_player_fields = vector_players[number_player_property]->get_vector_players_fields();
    bool found = false;
    std::vector<Field*> estate;
    for(size_t i = 0; i < vector_estates.size(); i++)
        for(size_t j = 0; j < vector_estates[i].size(); j++)
        {
            if(vector_estates[i][j] == actual_field)
            {
                estate = vector_estates[i];
                found = true;
                break;
            }
            if(found) break;
        }
    if(estate.size() == 2) found_field[2] = true;
    for(size_t i = 0; i < vector_player_fields.size(); i++)
    {
        for(size_t j = 0; j < estate.size(); j++)
        {
            if(!found_field[j] && vector_player_fields[i] == estate[j] && !vector_player_fields[i]->get_popped())
            {
                found_field[j] = true;
                if(estate[j]->get_number_of_houses() != 0) found_field_houses = true;
            }
            if(found_field[0] && found_field[1] && found_field[2]) break;
        }
        if(found_field[0] && found_field[1] && found_field[2]) break;
    }
    if(found_field[0] && found_field[1] && found_field[2]) return { true, found_field_houses };
    else return { false, false };
}

void Game::show_message(QString text)
{
    QTimer::singleShot(1, [=](){ message->setText(text); });
    QTimer::singleShot(2500, [=](){ message->setText(""); });
}

void Game::initialize_dice_buttons()
{
    dice1_button = create_pushbutton("", QFont(), "background: transparent", QRect(620, 790, 50, 50));
    connect(dice1_button, SIGNAL(clicked()), this, SLOT(on_dice_button_clicked()));
    dice2_button = create_pushbutton("", QFont(), "background: transparent", QRect(680, 790, 50, 50));
    connect(dice2_button, SIGNAL(clicked()), this, SLOT(on_dice_button_clicked()));
}

Field* Game::create_field(QString n, QString desc)
{
    Field* f = new Field(n, desc);
    return f;
}

Field* Game::create_field_chance(QString n, QString desc)
{
    Field* f = new FieldChance(n, desc);
    return f;
}

Field *Game::create_field_special(QString n, QString desc)
{
    Field* f = new FieldSpecial(n, desc);
    return f;
}

Field *Game::create_field_station(QString n, QString desc)
{
    Field* f = new FieldStation(n, desc);
    return f;
}

Field *Game::create_field_property(QString n, QString desc, std::vector<int> v, int buy, int build)
{
    Field* f = new FieldProperty(n, desc, v, buy, build);
    return f;
}

Player *Game::create_player(QString name, QColor color, int number)
{
    Player* p = new Player(name, color, number);
    return p;
}

Dice *Game::create_dice()
{
    Dice* d = new Dice();
    return d;
}

void Game::on_build_house_button_clicked()
{

}

void Game::on_sell_house_button_clicked()
{

}

void Game::on_exchange_button_clicked()
{

}

void Game::on_offers_button_clicked()
{

}

void Game::on_hypothecation_button_clicked()
{

}

void Game::on_end_turn_button_clicked()
{
    if(vector_players[current_player]->get_player_prisoned()) vector_players[current_player]->add_prison_turn();
    do
    {
        current_player++;
        if(current_player == number_of_players) current_player = 0;
    }
    while(vector_players[current_player]->get_player_bankrupt());
    player_label->setText(QString::number(vector_players[current_player]->get_player_number() + 1) + "  " + vector_players[current_player]->get_player_name());
    QPixmap p(":/board/board/checker.png");
    p.fill(vector_players[current_player]->get_player_color());
    color_player_label->setPixmap(p);
    money_label->setText("Finanse:  " + QString::number(vector_players[current_player]->get_player_money()) + "$");

    dice1_button->setEnabled(true);
    dice2_button->setEnabled(true);
    end_turn_button->setEnabled(false);
    action_label->hide();
    action_name_label->hide();
    action_description_label->hide();
    card_label->hide();
    card_name_label->hide();
    card_description_label->hide();

    if(vector_players[current_player]->get_player_prisoned())
    {
        if(vector_players[current_player]->get_player_prison_turn() < 4)
        {
            action_label->show();
            action_name_label->show();
            action_description_label->show();
            action_name_label->setText("Wyjdź z więzienia");
            action_description_label->setText("Numer tury spędzonej w więzieniu: " + QString::number(vector_players[current_player]->get_player_prison_turn()) +
                                              "\nZapłać 50$ lub rzuć kością");
            ok_button->show();
        }
        if(vector_players[current_player]->get_player_prison_turn() == 4)
        {
            vector_players[current_player]->change_money(-50);
            vector_players[current_player]->set_prisoned(false);
            vector_players[current_player]->reset_prison_turn();
            money_label->setText("Finanse:  " + QString::number(vector_players[current_player]->get_player_money()) + "$");
        }
    }
    dublet_turn = 0;
}

void Game::on_accept_button_clicked()
{
    action_label->hide();
    action_name_label->hide();
    action_description_label->hide();
    accept_button->hide();
    deny_button->hide();

    Field* actual_field = vector_fields[vector_players[current_player]->get_player_position()];
    QString class_type = actual_field->get_classname();
    if(class_type == "field_property" || class_type == "field_station" || class_type == "field_special")
    {
        vector_players[current_player]->change_money(-1 * actual_field->get_buying_cost());
        if(class_type == "field_station") vector_players[current_player]->add_number_of_stations();
        if(class_type == "field_special") vector_players[current_player]->add_number_of_special();
        actual_field->set_owner("Player");
        actual_field->set_number_owner(current_player);
        vector_players[current_player]->add_field(actual_field);
        money_label->setText("Finanse:  " + QString::number(vector_players[current_player]->get_player_money()) + "$");
    }
    else if(class_type == "field_chance")
    {
        /*
        action_name_label->setText("Szansa");
        action_description_label->setText("Tutaj będzie obsługa karty szansa/kasa społeczna"); // TODO
        ok_button->show();
        */
    }

    if(dublet)
    {
        dice1_button->setEnabled(true);
        dice2_button->setEnabled(true);
        end_turn_button->setEnabled(false);
        dublet = false;
    }
    else
    {
        dice1_button->setEnabled(false);
        dice2_button->setEnabled(false);
        end_turn_button->setEnabled(true);
    }
}

void Game::on_ok_button_clicked()
{
    action_label->hide();
    action_name_label->hide();
    action_description_label->hide();
    ok_button->hide();
    bool paid = false;

    if(vector_players[current_player]->get_player_prisoned() && vector_players[current_player]->get_player_prison_turn() != 0)
    {
        vector_players[current_player]->change_money(-50);
        vector_players[current_player]->set_prisoned(false);
        vector_players[current_player]->reset_prison_turn();
        money_label->setText("Finanse:  " + QString::number(vector_players[current_player]->get_player_money()) + "$");
        dice1_button->setEnabled(true);
        dice2_button->setEnabled(true);
        end_turn_button->setEnabled(false);
        paid = true;
    }
    else
    {
        Field* actual_field = vector_fields[vector_players[current_player]->get_player_position()];
        QString class_type = actual_field->get_classname();
        int entrance_cost = find_entrance_cost(actual_field);
        if(class_type == "field_property" || class_type == "field_station" || class_type == "field_special")
        {
            if(actual_field->get_owner() != "Bank" && actual_field->get_number_owner() != vector_players[current_player]->get_player_number() && !actual_field->get_popped())
            {
                vector_players[current_player]->change_money(-1 * entrance_cost);
                vector_players[actual_field->get_number_owner()]->change_money(entrance_cost);
                money_label->setText("Finanse:  " + QString::number(vector_players[current_player]->get_player_money()) + "$");
            }
        }
        else if(class_type == "field_chance")
        {
            /*
            action_name_label->setText("Szansa");
            action_description_label->setText("Tutaj będzie obsługa karty szansa/kasa społeczna"); // TODO
            ok_button->show();
            */
        }
        else
        {
            if(actual_field->get_name() == "Domiar podatkowy" || actual_field->get_name() == "Podatek dochodowy")
            {
                vector_players[current_player]->change_money(-1 * entrance_cost);
                money_label->setText("Finanse:  " + QString::number(vector_players[current_player]->get_player_money()) + "$");
            }
        }
    }

    if(!paid)
    {
        if(vector_players[current_player]->get_player_prisoned() || !dublet)
        {
            dice1_button->setEnabled(false);
            dice2_button->setEnabled(false);
            end_turn_button->setEnabled(true);
        }
        else
        {
            dice1_button->setEnabled(true);
            dice2_button->setEnabled(true);
            end_turn_button->setEnabled(false);
        }
    }
    dublet = false;
}

void Game::on_deny_button_clicked()
{
    action_label->hide();
    action_name_label->hide();
    action_description_label->hide();
    accept_button->hide();
    deny_button->hide();

    if(dublet)
    {
        dice1_button->setEnabled(true);
        dice2_button->setEnabled(true);
        end_turn_button->setEnabled(false);
        dublet = false;
    }
    else
    {
        dice1_button->setEnabled(false);
        dice2_button->setEnabled(false);
        end_turn_button->setEnabled(true);
    }
}

void Game::on_field_button_clicked()
{
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    int number_button_clicked = 0;
    for(int i = 0; i < vector_board_buttons.size(); i++)
    {
        if(vector_board_buttons[i] == buttonSender)
        {
            number_button_clicked = i;
            break;
        }
    }
    buttonSender = nullptr;

    if(card_label->isHidden()) card_label->show();
    if(card_name_label->isHidden()) card_name_label->show();
    if(card_description_label->isHidden()) card_description_label->show();
    card_name_label->setText(vector_fields[number_button_clicked]->get_name());
    card_description_label->setText(vector_fields[number_button_clicked]->get_description());
}

void Game::on_dice_button_clicked()
{
    dice1->generate_number();
    dice2->generate_number();
    change_dices_labels(dice1->get_number(), dice1_label);
    change_dices_labels(dice2->get_number(), dice2_label);
    if(dice1->get_number() == dice2->get_number())
    {
        dublet = true;
        if(vector_players[current_player]->get_player_prisoned())
        {
            vector_players[current_player]->set_prisoned(false);
            ok_button->hide();
        }
        dublet_turn++;
    }

    if(vector_players[current_player]->get_player_prisoned() && !dublet)
    {
        dice1_button->setEnabled(false);
        dice2_button->setEnabled(false);
        end_turn_button->setEnabled(true);
        action_label->hide();
        action_name_label->hide();
        action_description_label->hide();
        ok_button->hide();
    }
    else
    {
        int actual_position = vector_players[current_player]->get_player_position();
        int calculated_position = (actual_position + dice1->get_number() + dice2->get_number()) % 40;

        if(dublet_turn == 3 || calculated_position == 30)
        {
            vector_players[current_player]->set_player_position(10);
            vector_chequers[current_player]->setGeometry(chequers_position_table[40][current_player]);
            vector_players[current_player]->set_prisoned(true);

            dice1_button->setEnabled(false);
            dice2_button->setEnabled(false);
            end_turn_button->setEnabled(false);
            action_label->show();
            action_name_label->show();
            action_description_label->show();
            action_name_label->setText("Idź do więzienia");
            action_description_label->setText("Popełniłeś wykroczenie!"
                                              "\nIdziesz do więzienia!");
            ok_button->show();
        }
        else
        {
            vector_players[current_player]->set_player_position(calculated_position);
            vector_chequers[current_player]->setGeometry(chequers_position_table[calculated_position][current_player]);

            if((actual_position + dice1->get_number() + dice2->get_number()) >= 40)
            {
                vector_players[current_player]->change_money(200);
                money_label->setText("Finanse:  " + QString::number(vector_players[current_player]->get_player_money()) + "$");
                show_message("Otrzymałeś 200$ za przejście przez start");
            }
            check_position(vector_fields[calculated_position]);
        }
    }
}




#include "game.h"
#include "mainwidget.h"

Game::Game(QWidget *parent, QVector<QString> vector_name_players, QVector<QColor> vector_color_players, int number_of_players, bool auction)
    : QWidget{parent}, parent(parent), vector_name_players(vector_name_players), vector_color_players(vector_color_players), number_of_players(number_of_players), auction(auction)
{
    create_vector_fields();
    initialize_vector_estates();

    std::thread players_thread(&Game::initialize_players, this);
    std::thread dices_thread(&Game::initialize_dices, this);
    std::thread chance_thread(&Game::initialize_vector_chance_cards, this);
    std::thread community_chest_thread(&Game::initialize_vector_community_chest_cards, this);
    initialize_chequers_table();
    players_thread.join();
    dices_thread.join();
    chance_thread.join();
    community_chest_thread.join();

    initialize_interface();
    initialize_chequers();
    initialize_field_buttons();
    initialize_dice_buttons();
    initialize_vector_cards_label();
    initialize_buildings_labels_table();
    hide_interface();
}

Game::~Game()
{

}

QPointer<QLabel> Game::create_pixmap_label(QString path, QRect geometry, QColor color)
{
    QPointer<QLabel> label = new QLabel(this);
    QPixmap p(path);
    p.fill(color);
    label->setPixmap(p);
    label->setGeometry(geometry);
    return label;
}

QPointer<QLabel> Game::create_pixmap_label(QString path, QRect geometry, int scale_width, int scale_height, QColor color)
{
    QPointer<QLabel> label = new QLabel(this);
    QPixmap p(path);
    p.fill(color);
    label->setPixmap(p.scaled(scale_width, scale_height));
    label->setGeometry(geometry);
    return label;
}

QPointer<QLabel> Game::create_pixmap_label(QString path, QRect geometry, int scale_width, int scale_height, QString stylesheet, bool hidden)
{
    QPointer<QLabel> label = new QLabel(this);
    QPixmap p(path);
    label->setPixmap(p.scaled(scale_width, scale_height));
    label->setGeometry(geometry);
    if(stylesheet != "") label->setStyleSheet(stylesheet);
    if(hidden) label->hide();
    return label;
}

QPointer<QLabel> Game::create_pixmap_label(QString path, QRect geometry, QString stylesheet, bool hidden)
{
    QPointer<QLabel> label = new QLabel(this);
    QPixmap p(path);
    label->setPixmap(p);
    label->setGeometry(geometry);
    label->setStyleSheet(stylesheet);
    if(hidden) label->hide();
    return label;
}

QPointer<QLabel> Game::create_pixmap_label(QString path, QRect geometry, int scale_width, int scale_height)
{
    QPointer<QLabel> label = new QLabel(this);
    QPixmap p(path);
    label->setPixmap(p.scaled(scale_width, scale_height));
    label->setGeometry(geometry);
    return label;
}

QPointer<QLabel> Game::create_label(QString text, QFont font, QString stylesheet, QRect geometry, Qt::Alignment align)
{
    QPointer<QLabel> label = new QLabel(this);
    label->setText(text);
    label->setFont(font);
    label->setStyleSheet(stylesheet);
    label->setGeometry(geometry);
    label->setAlignment(align);
    return label;
}

QPointer<QPushButton> Game::create_pushbutton(QString text, QFont font, QString stylesheet, QRect geometry, bool enabled)
{
    QPointer<QPushButton> button = new QPushButton(this);
    button->setText(text);
    button->setFont(font);
    button->setStyleSheet(stylesheet);
    button->setGeometry(geometry);
    if(!enabled) button->setEnabled(enabled);
    return button;
}

void Game::initialize_interface()
{
    view_number = 0;
    refresh_button_states();
    create_center_interface();
    create_left_interface();
    create_right_interface();
}

bool Game::initialize_field_buttons()
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

        QPointer<QPushButton> field_button = create_pushbutton("", QFont(), "background: transparent", QRect(offset_width, offset_height, width, height));
        connect(field_button, SIGNAL(clicked()), this, SLOT(on_field_button_clicked()));
        vector_board_buttons.push_back(field_button);
    }
    return true;
}

bool Game::initialize_vector_cards_label()
{
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 6; j++)
            vector_cards_label.push_back(create_pixmap_label("", QRect(1468 + j * 72, 65 + i * 115, 70, 112), "", false));
    return true;
}


bool Game::initialize_dice_buttons()
{
    dice1_button = create_pushbutton("", QFont(), "background: transparent", QRect(620, 790, 50, 50));
    connect(dice1_button, SIGNAL(clicked()), this, SLOT(on_dice_button_clicked()));
    dice2_button = create_pushbutton("", QFont(), "background: transparent", QRect(680, 790, 50, 50));
    connect(dice2_button, SIGNAL(clicked()), this, SLOT(on_dice_button_clicked()));
    return true;
}

void Game::create_vector_fields()
{
    number_field_button_clicked = -1;

    QVector<QString> vector_names_chance_fields = {"Kasa społeczna", "Szansa"};
    QVector<std::shared_ptr<Field>> vector_fields_chance_help;
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
    QVector<std::shared_ptr<Field>> vector_fields_help;
    for(int i = 0; i < 6; i++)
        vector_fields_help.push_back(create_field(vector_names_fields[i].first, vector_names_fields[i].second));

    QVector<QString> vector_names_station_fields = {"Dworzec Reading", "Dworzec Pennsylvania", "Dworzec B. &. O", "Dworzec Short Line"};
    QVector<std::shared_ptr<Field>> vector_fields_station_help;
    for(int i = 0; i < 4; i++)
        vector_fields_station_help.push_back(create_field_station(vector_names_station_fields[i], "Właściciel podanej ilości dworców pobiera:"
                                                                                                  "\n1 dworzec - 25$"
                                                                                                  "\n2 dworce - 50$"
                                                                                                  "\n3 dworce - 100$"
                                                                                                  "\n4 dworce - 200$"
                                                                                                  "\nKoszt zakupu: 200$"
                                                                                                  "\nWłaściciel/Numer właściciela: Bank"
                                                                                                  "\nZastaw: Nie"));

    QVector<QString> vector_names_special_fields = {"Elektrownia", "Wodociągi"};
    QVector<std::shared_ptr<Field>> vector_fields_special_help;
    for(int i = 0; i < 2; i++)
        vector_fields_special_help.push_back(create_field_special(vector_names_special_fields[i], "Właściciel jednego obiektu"
                                                                                                  "\nużyteczności publicznej pobiera"
                                                                                                  "\nczterokrotność sumy oczek;"
                                                                                                  "\nWłaściciel obu obiektów"
                                                                                                  "\nużyteczności publicznej pobiera"
                                                                                                  "\ndziesięciokrotną sumę oczek"
                                                                                                  "\nCena zakupu: 150$"
                                                                                                  "\nWłaściciel/Numer właściciela: Bank"
                                                                                                  "\nZastaw: Nie"));

    QVector<QString> vector_names_properties_fields = {"Mediter-Ranean Avenue", "Baltic Avenue", "Oriental Avenue", "Vermont Avenue", "Connecticut Avenue",
                                                    "St. Charles Place", "States Avenue", "Virginia Avenue", "St. James Place", "Tennessee Avenue",
                                                    "New York Avenue", "Kentucky Avenue", "Indiana Avenue", "Illinois Avenue", "Atlantic Avenue",
                                                    "Ventnor Avenue", "Marvin Gardens", "Pacific Avenue", "North Carolina Avenue", "Pennsylvania Avenue",
                                                    "Park Place", "Boardwalk"};
    QVector<std::vector<int>> vector_of_payments = {{2, 10, 30, 90, 160, 250}, {4, 20, 60, 180, 320, 450}, {6, 30, 90, 270, 400, 550}, {6, 30, 90, 270, 400, 550}, {8, 40, 100, 300, 450, 600},
                                                        {10, 50, 150, 450, 625, 750}, {10, 50, 150, 450, 625, 750}, {12, 60, 180, 500, 700, 900}, {14, 70, 200, 550, 750, 950}, {14, 70, 200, 550, 750, 950},
                                                        {16, 80, 220, 600, 800, 1000}, {18, 90, 250, 700, 875, 1050}, {18, 90, 250, 700, 875, 1050}, {20, 100, 300, 750, 925, 1100}, {22, 110, 330, 800, 975, 1150},
                                                        {22, 110, 330, 800, 975, 1150}, {24, 120, 360, 850, 1025, 1200}, {26, 130, 390, 900, 1100, 1275}, {26, 130, 390, 900, 1100, 1275}, {28, 150, 450, 1000, 1200, 1400},
                                                        {35, 175, 500, 1100, 1300, 1500}, {50, 200, 600, 1400, 1700, 2000}};
    QVector<int> vector_buying_costs = {60, 60, 100, 100, 120, 140, 140, 160, 180, 180, 200, 220, 220, 240, 260, 260, 280, 300, 300, 320, 350, 400};
    QVector<int> vector_building_costs = {50, 50, 50, 50, 50, 100, 100, 100, 100, 100, 100, 150, 150, 150, 150, 150, 150, 200, 200, 200, 200, 200};
    QVector<std::shared_ptr<Field>> vector_fields_properties_help;
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
                                  "\nCena za 1 dom: " + QString::number(vector_building_costs[i]) + "$"
                                  "\nWłaściciel/Numer właściciela: Bank"
                                  "\nZastaw: Nie",
                                  vector_of_payments[i], vector_buying_costs[i], vector_building_costs[i])
            );

    layer_fields(vector_fields_chance_help, vector_fields_help, vector_fields_station_help, vector_fields_special_help, vector_fields_properties_help);
}

void Game::layer_fields(QVector<std::shared_ptr<Field>>& vector_fields_chance_help, QVector<std::shared_ptr<Field>>& vector_fields_help, QVector<std::shared_ptr<Field>>& vector_fields_station_help, QVector<std::shared_ptr<Field>>& vector_fields_special_help, QVector<std::shared_ptr<Field>>& vector_fields_properties_help)
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
}

void Game::change_dices_labels(int dice_number, QPointer<QLabel> dice_label)
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

void Game::change_vector_cards_label()
{
    for(int i = 0; i < vector_cards_label.size(); i++)
        vector_cards_label[i]->setPixmap(QPixmap());
    std::vector<std::shared_ptr<Field>> vector_players_fields = current_player->get_vector_players_fields();
    std::vector<std::regex> vector_regex = { std::regex(" |-"), std::regex("ą"), std::regex("ć"), std::regex("ę"), std::regex("ł"), std::regex("ń"), std::regex("ó"), std::regex("ś"), std::regex("ź|ż"), std::regex("[.]")};
    std::vector<std::string> vector_regex_results = { "_", "a", "c", "e", "l", "n", "o", "s", "z", ""};
    for(size_t i = 0; i < vector_players_fields.size(); i++)
    {
        std::string field_name = vector_players_fields[i]->get_name().toStdString();
        for(size_t i = 0; i < vector_regex.size(); i++)
            field_name = std::regex_replace(field_name, vector_regex[i], vector_regex_results[i]);
        std::transform(field_name.begin(), field_name.end(), field_name.begin(), [](unsigned char c){ return std::tolower(c); });
        if(vector_players_fields[i]->get_classname() != "field_chance")
            if(vector_players_fields[i]->get_popped()) field_name += "_popped";
        QString path = QString::fromStdString(field_name) + ".jpg";
        vector_cards_label[i]->setPixmap(QPixmap(":/fields/fields/" + path));
    }
}

bool Game::initialize_chequers()
{
    for(int i = 0; i < number_of_players; i++)
        vector_chequers.push_back(create_pixmap_label(":/board/board/checker.png", chequers_position_table[0][i], 15, 15, vector_players[i]->get_player_color()));
    return true;
}

void Game::initialize_chequers_table()
{
    chequers_position_table = {
                                { QRect(1370, 891, 15, 15), QRect(1405, 891, 15, 15), QRect(1352, 926, 15, 15), QRect(1387, 926, 15, 15),
                                  QRect(1422, 926, 15, 15), QRect(1352, 961, 15, 15), QRect(1387, 961, 15, 15), QRect(1422, 961, 15, 15) },
                                //bottom
                                { QRect(1267, 903, 15, 15), QRect(1292, 903, 15, 15), QRect(1267, 928, 15, 15), QRect(1292, 928, 15, 15),
                                  QRect(1267, 953, 15, 15), QRect(1292, 953, 15, 15), QRect(1267, 978, 15, 15), QRect(1292, 978, 15, 15) },
                                { QRect(1185, 903, 15, 15), QRect(1210, 903, 15, 15), QRect(1185, 928, 15, 15), QRect(1210, 928, 15, 15),
                                  QRect(1185, 953, 15, 15), QRect(1210, 953, 15, 15), QRect(1185, 978, 15, 15), QRect(1210, 978, 15, 15) },
                                { QRect(1103, 903, 15, 15), QRect(1128, 903, 15, 15), QRect(1103, 928, 15, 15), QRect(1128, 928, 15, 15),
                                  QRect(1103, 953, 15, 15), QRect(1128, 953, 15, 15), QRect(1103, 978, 15, 15), QRect(1128, 978, 15, 15) },
                                { QRect(1021, 903, 15, 15), QRect(1046, 903, 15, 15), QRect(1021, 928, 15, 15), QRect(1046, 928, 15, 15),
                                  QRect(1021, 953, 15, 15), QRect(1046, 953, 15, 15), QRect(1021, 978, 15, 15), QRect(1046, 978, 15, 15) },
                                { QRect(939, 903, 15, 15), QRect(964, 903, 15, 15), QRect(939, 928, 15, 15), QRect(964, 928, 15, 15),
                                  QRect(939, 953, 15, 15), QRect(964, 953, 15, 15), QRect(939, 978, 15, 15), QRect(964, 978, 15, 15) },
                                { QRect(857, 903, 15, 15), QRect(882, 903, 15, 15), QRect(857, 928, 15, 15), QRect(882, 928, 15, 15),
                                  QRect(857, 953, 15, 15), QRect(882, 953, 15, 15), QRect(857, 978, 15, 15), QRect(882, 978, 15, 15) },
                                { QRect(775, 903, 15, 15), QRect(800, 903, 15, 15), QRect(775, 928, 15, 15), QRect(800, 928, 15, 15),
                                  QRect(775, 953, 15, 15), QRect(800, 953, 15, 15), QRect(775, 978, 15, 15), QRect(800, 978, 15, 15) },
                                { QRect(693, 903, 15, 15), QRect(718, 903, 15, 15), QRect(693, 928, 15, 15), QRect(718, 928, 15, 15),
                                  QRect(693, 953, 15, 15), QRect(718, 953, 15, 15), QRect(693, 978, 15, 15), QRect(718, 978, 15, 15) },
                                { QRect(611, 903, 15, 15), QRect(636, 903, 15, 15), QRect(611, 928, 15, 15), QRect(636, 928, 15, 15),
                                  QRect(611, 953, 15, 15), QRect(636, 953, 15, 15), QRect(611, 978, 15, 15), QRect(636, 978, 15, 15) },
                                //
                                { QRect(471, 873, 15, 15), QRect(471, 898, 15, 15), QRect(471, 923, 15, 15), QRect(471, 948, 15, 15),
                                  QRect(498, 973, 15, 15), QRect(523, 973, 15, 15), QRect(548, 973, 15, 15), QRect(573, 973, 15, 15) },
                                //left
                                { QRect(539, 808, 15, 15), QRect(539, 833, 15, 15), QRect(514, 808, 15, 15), QRect(514, 833, 15, 15),
                                  QRect(489, 808, 15, 15), QRect(489, 833, 15, 15), QRect(464, 808, 15, 15), QRect(464, 833, 15, 15) },
                                { QRect(539, 726, 15, 15), QRect(539, 751, 15, 15), QRect(514, 726, 15, 15), QRect(514, 751, 15, 15),
                                  QRect(489, 726, 15, 15), QRect(489, 751, 15, 15), QRect(464, 726, 15, 15), QRect(464, 751, 15, 15) },
                                { QRect(539, 644, 15, 15), QRect(539, 669, 15, 15), QRect(514, 644, 15, 15), QRect(514, 669, 15, 15),
                                  QRect(489, 644, 15, 15), QRect(489, 669, 15, 15), QRect(464, 644, 15, 15), QRect(464, 669, 15, 15) },
                                { QRect(539, 562, 15, 15), QRect(539, 587, 15, 15), QRect(514, 562, 15, 15), QRect(514, 587, 15, 15),
                                  QRect(489, 562, 15, 15), QRect(489, 587, 15, 15), QRect(464, 562, 15, 15), QRect(464, 587, 15, 15) },
                                { QRect(539, 480, 15, 15), QRect(539, 505, 15, 15), QRect(514, 480, 15, 15), QRect(514, 505, 15, 15),
                                  QRect(489, 480, 15, 15), QRect(489, 505, 15, 15), QRect(464, 480, 15, 15), QRect(464, 505, 15, 15) },
                                { QRect(539, 398, 15, 15), QRect(539, 423, 15, 15), QRect(514, 398, 15, 15), QRect(514, 423, 15, 15),
                                  QRect(489, 398, 15, 15), QRect(489, 423, 15, 15), QRect(464, 398, 15, 15), QRect(464, 423, 15, 15) },
                                { QRect(539, 316, 15, 15), QRect(539, 341, 15, 15), QRect(514, 316, 15, 15), QRect(514, 341, 15, 15),
                                  QRect(489, 316, 15, 15), QRect(489, 341, 15, 15), QRect(464, 316, 15, 15), QRect(464, 341, 15, 15) },
                                { QRect(539, 234, 15, 15), QRect(539, 259, 15, 15), QRect(514, 234, 15, 15), QRect(514, 259, 15, 15),
                                  QRect(489, 234, 15, 15), QRect(489, 259, 15, 15), QRect(464, 234, 15, 15), QRect(464, 259, 15, 15) },
                                { QRect(539, 152, 15, 15), QRect(539, 177, 15, 15), QRect(514, 152, 15, 15), QRect(514, 177, 15, 15),
                                  QRect(489, 152, 15, 15), QRect(489, 177, 15, 15), QRect(464, 152, 15, 15), QRect(464, 177, 15, 15) },
                                //
                                { QRect(552, 38, 15, 15), QRect(552, 73, 15, 15), QRect(517, 23, 15, 15), QRect(517, 58, 15, 15),
                                  QRect(517, 93, 15, 15), QRect(482, 23, 15, 15), QRect(482, 58, 15, 15), QRect(482, 93, 15, 15) },
                                //up
                                { QRect(637, 79, 15, 15), QRect(612, 79, 15, 15), QRect(637, 54, 15, 15), QRect(612, 54, 15, 15),
                                  QRect(637, 29, 15, 15), QRect(612, 29, 15, 15), QRect(637, 4, 15, 15), QRect(612, 4, 15, 15) },
                                { QRect(719, 79, 15, 15), QRect(694, 79, 15, 15), QRect(719, 54, 15, 15), QRect(694, 54, 15, 15),
                                  QRect(719, 29, 15, 15), QRect(694, 29, 15, 15), QRect(719, 4, 15, 15), QRect(694, 4, 15, 15) },
                                { QRect(801, 79, 15, 15), QRect(776, 79, 15, 15), QRect(801, 54, 15, 15), QRect(776, 54, 15, 15),
                                  QRect(801, 29, 15, 15), QRect(776, 29, 15, 15), QRect(801, 4, 15, 15), QRect(776, 4, 15, 15) },
                                { QRect(883, 79, 15, 15), QRect(858, 79, 15, 15), QRect(883, 54, 15, 15), QRect(858, 54, 15, 15),
                                  QRect(883, 29, 15, 15), QRect(858, 29, 15, 15), QRect(883, 4, 15, 15), QRect(858, 4, 15, 15) },
                                { QRect(965, 79, 15, 15), QRect(940, 79, 15, 15), QRect(965, 54, 15, 15), QRect(940, 54, 15, 15),
                                  QRect(965, 29, 15, 15), QRect(940, 29, 15, 15), QRect(965, 4, 15, 15), QRect(940, 4, 15, 15) },
                                { QRect(1047, 79, 15, 15), QRect(1022, 79, 15, 15), QRect(1047, 54, 15, 15), QRect(1022, 54, 15, 15),
                                  QRect(1047, 29, 15, 15), QRect(1022, 29, 15, 15), QRect(1047, 4, 15, 15), QRect(1022, 4, 15, 15) },
                                { QRect(1129, 79, 15, 15), QRect(1104, 79, 15, 15), QRect(1129, 54, 15, 15), QRect(1104, 54, 15, 15),
                                  QRect(1129, 29, 15, 15), QRect(1104, 29, 15, 15), QRect(1129, 4, 15, 15), QRect(1104, 4, 15, 15) },
                                { QRect(1211, 79, 15, 15), QRect(1186, 79, 15, 15), QRect(1211, 54, 15, 15), QRect(1186, 54, 15, 15),
                                  QRect(1211, 29, 15, 15), QRect(1186, 29, 15, 15), QRect(1211, 4, 15, 15), QRect(1186, 4, 15, 15) },
                                { QRect(1293, 79, 15, 15), QRect(1268, 79, 15, 15), QRect(1293, 54, 15, 15), QRect(1268, 54, 15, 15),
                                  QRect(1293, 29, 15, 15), QRect(1268, 29, 15, 15), QRect(1293, 4, 15, 15), QRect(1268, 4, 15, 15) },
                                //
                                { QRect(1405, 92, 15, 15), QRect(1370, 92, 15, 15), QRect(1422, 57, 15, 15), QRect(1387, 57, 15, 15),
                                  QRect(1352, 57, 15, 15), QRect(1422, 22, 15, 15), QRect(1387, 22, 15, 15), QRect(1352, 22, 15, 15) },
                                //right
                                { QRect(1364, 177, 15, 15), QRect(1364, 152, 15, 15), QRect(1389, 177, 15, 15), QRect(1389, 152, 15, 15),
                                  QRect(1414, 177, 15, 15), QRect(1414, 152, 15, 15), QRect(1439, 177, 15, 15), QRect(1439, 152, 15, 15) },
                                { QRect(1364, 259, 15, 15), QRect(1364, 234, 15, 15), QRect(1389, 259, 15, 15), QRect(1389, 234, 15, 15),
                                  QRect(1414, 259, 15, 15), QRect(1414, 234, 15, 15), QRect(1439, 259, 15, 15), QRect(1439, 234, 15, 15) },
                                { QRect(1364, 341, 15, 15), QRect(1364, 316, 15, 15), QRect(1389, 341, 15, 15), QRect(1389, 316, 15, 15),
                                  QRect(1414, 341, 15, 15), QRect(1414, 316, 15, 15), QRect(1439, 341, 15, 15), QRect(1439, 316, 15, 15) },
                                { QRect(1364, 423, 15, 15), QRect(1364, 398, 15, 15), QRect(1389, 423, 15, 15), QRect(1389, 398, 15, 15),
                                  QRect(1414, 423, 15, 15), QRect(1414, 398, 15, 15), QRect(1439, 423, 15, 15), QRect(1439, 398, 15, 15) },
                                { QRect(1364, 505, 15, 15), QRect(1364, 480, 15, 15), QRect(1389, 505, 15, 15), QRect(1389, 480, 15, 15),
                                  QRect(1414, 505, 15, 15), QRect(1414, 480, 15, 15), QRect(1439, 505, 15, 15), QRect(1439, 480, 15, 15) },
                                { QRect(1364, 587, 15, 15), QRect(1364, 562, 15, 15), QRect(1389, 587, 15, 15), QRect(1389, 562, 15, 15),
                                  QRect(1414, 587, 15, 15), QRect(1414, 562, 15, 15), QRect(1439, 587, 15, 15), QRect(1439, 562, 15, 15) },
                                { QRect(1364, 669, 15, 15), QRect(1364, 644, 15, 15), QRect(1389, 669, 15, 15), QRect(1389, 644, 15, 15),
                                  QRect(1414, 669, 15, 15), QRect(1414, 644, 15, 15), QRect(1439, 669, 15, 15), QRect(1439, 644, 15, 15) },
                                { QRect(1364, 751, 15, 15), QRect(1364, 726, 15, 15), QRect(1389, 751, 15, 15), QRect(1389, 726, 15, 15),
                                  QRect(1414, 751, 15, 15), QRect(1414, 726, 15, 15), QRect(1439, 751, 15, 15), QRect(1439, 726, 15, 15) },
                                { QRect(1364, 833, 15, 15), QRect(1364, 808, 15, 15), QRect(1389, 833, 15, 15), QRect(1389, 808, 15, 15),
                                  QRect(1414, 833, 15, 15), QRect(1414, 808, 15, 15), QRect(1439, 833, 15, 15), QRect(1439, 808, 15, 15) },
                                //prison
                                { QRect(525, 883, 15, 15), QRect(550, 883, 15, 15), QRect(513, 908, 15, 15), QRect(538, 908, 15, 15),
                                  QRect(563, 908, 15, 15), QRect(513, 933, 15, 15), QRect(538, 933, 15, 15), QRect(563, 933, 15, 15) }
    };
}

bool Game::initialize_buildings_labels_table()
{
    buildings_labels_table = {  //
                                {},
                                //down
                                { create_pixmap_label(":/board/board/house_1.png", QRect(1250, 875, 18, 18)),
                                  create_pixmap_label(":/board/board/house_1.png", QRect(1269, 875, 18, 18)),
                                  create_pixmap_label(":/board/board/house_1.png", QRect(1288, 875, 18, 18)),
                                  create_pixmap_label(":/board/board/house_1.png", QRect(1307, 875, 18, 18)),
                                  create_pixmap_label(":/board/board/hotel_1.png", QRect(1276, 870, 24, 26)) },
                                {},
                                { create_pixmap_label(":/board/board/house_1.png", QRect(1086, 875, 18, 18)),
                                  create_pixmap_label(":/board/board/house_1.png", QRect(1105, 875, 18, 18)),
                                  create_pixmap_label(":/board/board/house_1.png", QRect(1124, 875, 18, 18)),
                                  create_pixmap_label(":/board/board/house_1.png", QRect(1143, 875, 18, 18)),
                                  create_pixmap_label(":/board/board/hotel_1.png", QRect(1112, 870, 24, 26)) },
                                {},
                                {},
                                { create_pixmap_label(":/board/board/house_1.png", QRect(840, 875, 18, 18)),
                                  create_pixmap_label(":/board/board/house_1.png", QRect(859, 875, 18, 18)),
                                  create_pixmap_label(":/board/board/house_1.png", QRect(878, 875, 18, 18)),
                                  create_pixmap_label(":/board/board/house_1.png", QRect(897, 875, 18, 18)),
                                  create_pixmap_label(":/board/board/hotel_1.png", QRect(866, 870, 24, 26)) },
                                {},
                                { create_pixmap_label(":/board/board/house_1.png", QRect(676, 875, 18, 18)),
                                  create_pixmap_label(":/board/board/house_1.png", QRect(695, 875, 18, 18)),
                                  create_pixmap_label(":/board/board/house_1.png", QRect(714, 875, 18, 18)),
                                  create_pixmap_label(":/board/board/house_1.png", QRect(733, 875, 18, 18)),
                                  create_pixmap_label(":/board/board/hotel_1.png", QRect(702, 870, 24, 26)) },
                                { create_pixmap_label(":/board/board/house_1.png", QRect(594, 875, 18, 18)),
                                  create_pixmap_label(":/board/board/house_1.png", QRect(613, 875, 18, 18)),
                                  create_pixmap_label(":/board/board/house_1.png", QRect(632, 875, 18, 18)),
                                  create_pixmap_label(":/board/board/house_1.png", QRect(651, 875, 18, 18)),
                                  create_pixmap_label(":/board/board/hotel_1.png", QRect(620, 870, 24, 26)) },
                                //
                                {},
                                //left
                                { create_pixmap_label(":/board/board/house_2.png", QRect(569, 790, 18, 18)),
                                  create_pixmap_label(":/board/board/house_2.png", QRect(569, 809, 18, 18)),
                                  create_pixmap_label(":/board/board/house_2.png", QRect(569, 828, 18, 18)),
                                  create_pixmap_label(":/board/board/house_2.png", QRect(569, 847, 18, 18)),
                                  create_pixmap_label(":/board/board/hotel_2.png", QRect(564, 816, 26, 24)) },
                                {},
                                { create_pixmap_label(":/board/board/house_2.png", QRect(569, 626, 18, 18)),
                                  create_pixmap_label(":/board/board/house_2.png", QRect(569, 645, 18, 18)),
                                  create_pixmap_label(":/board/board/house_2.png", QRect(569, 664, 18, 18)),
                                  create_pixmap_label(":/board/board/house_2.png", QRect(569, 683, 18, 18)),
                                  create_pixmap_label(":/board/board/hotel_2.png", QRect(564, 652, 26, 24)) },
                                { create_pixmap_label(":/board/board/house_2.png", QRect(569, 544, 18, 18)),
                                  create_pixmap_label(":/board/board/house_2.png", QRect(569, 563, 18, 18)),
                                  create_pixmap_label(":/board/board/house_2.png", QRect(569, 582, 18, 18)),
                                  create_pixmap_label(":/board/board/house_2.png", QRect(569, 601, 18, 18)),
                                  create_pixmap_label(":/board/board/hotel_2.png", QRect(564, 570, 26, 24)) },
                                {},
                                { create_pixmap_label(":/board/board/house_2.png", QRect(569, 380, 18, 18)),
                                  create_pixmap_label(":/board/board/house_2.png", QRect(569, 399, 18, 18)),
                                  create_pixmap_label(":/board/board/house_2.png", QRect(569, 418, 18, 18)),
                                  create_pixmap_label(":/board/board/house_2.png", QRect(569, 437, 18, 18)),
                                  create_pixmap_label(":/board/board/hotel_2.png", QRect(564, 406, 26, 24)) },
                                {},
                                { create_pixmap_label(":/board/board/house_2.png", QRect(569, 216, 18, 18)),
                                  create_pixmap_label(":/board/board/house_2.png", QRect(569, 235, 18, 18)),
                                  create_pixmap_label(":/board/board/house_2.png", QRect(569, 254, 18, 18)),
                                  create_pixmap_label(":/board/board/house_2.png", QRect(569, 273, 18, 18)),
                                  create_pixmap_label(":/board/board/hotel_2.png", QRect(564, 242, 26, 24)) },
                                { create_pixmap_label(":/board/board/house_2.png", QRect(569, 134, 18, 18)),
                                  create_pixmap_label(":/board/board/house_2.png", QRect(569, 153, 18, 18)),
                                  create_pixmap_label(":/board/board/house_2.png", QRect(569, 172, 18, 18)),
                                  create_pixmap_label(":/board/board/house_2.png", QRect(569, 191, 18, 18)),
                                  create_pixmap_label(":/board/board/hotel_2.png", QRect(564, 160, 26, 24)) },
                                //
                                {},
                                //up
                                { create_pixmap_label(":/board/board/house_3.png", QRect(651, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/house_3.png", QRect(632, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/house_3.png", QRect(613, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/house_3.png", QRect(594, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/hotel_3.png", QRect(620, 104, 24, 26)) },
                                {},
                                { create_pixmap_label(":/board/board/house_3.png", QRect(814, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/house_3.png", QRect(795, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/house_3.png", QRect(776, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/house_3.png", QRect(757, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/hotel_3.png", QRect(783, 104, 24, 26)) },
                                { create_pixmap_label(":/board/board/house_3.png", QRect(897, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/house_3.png", QRect(878, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/house_3.png", QRect(859, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/house_3.png", QRect(840, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/hotel_3.png", QRect(866, 104, 24, 26)) },
                                {},
                                { create_pixmap_label(":/board/board/house_3.png", QRect(1061, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/house_3.png", QRect(1042, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/house_3.png", QRect(1023, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/house_3.png", QRect(1004, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/hotel_3.png", QRect(1030, 104, 24, 26)) },
                                { create_pixmap_label(":/board/board/house_3.png", QRect(1143, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/house_3.png", QRect(1124, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/house_3.png", QRect(1105, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/house_3.png", QRect(1086, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/hotel_3.png", QRect(1112, 104, 24, 26)) },
                                {},
                                { create_pixmap_label(":/board/board/house_3.png", QRect(1307, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/house_3.png", QRect(1288, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/house_3.png", QRect(1269, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/house_3.png", QRect(1250, 108, 18, 18)),
                                  create_pixmap_label(":/board/board/hotel_3.png", QRect(1276, 104, 24, 26)) },
                                //
                                {},
                                //right
                                { create_pixmap_label(":/board/board/house_4.png", QRect(1334, 191, 18, 18)),
                                  create_pixmap_label(":/board/board/house_4.png", QRect(1334, 172, 18, 18)),
                                  create_pixmap_label(":/board/board/house_4.png", QRect(1334, 153, 18, 18)),
                                  create_pixmap_label(":/board/board/house_4.png", QRect(1334, 134, 18, 18)),
                                  create_pixmap_label(":/board/board/hotel_4.png", QRect(1330, 160, 26, 24)) },
                                { create_pixmap_label(":/board/board/house_4.png", QRect(1334, 273, 18, 18)),
                                  create_pixmap_label(":/board/board/house_4.png", QRect(1334, 254, 18, 18)),
                                  create_pixmap_label(":/board/board/house_4.png", QRect(1334, 235, 18, 18)),
                                  create_pixmap_label(":/board/board/house_4.png", QRect(1334, 216, 18, 18)),
                                  create_pixmap_label(":/board/board/hotel_4.png", QRect(1330, 242, 26, 24)) },
                                {},
                                { create_pixmap_label(":/board/board/house_4.png", QRect(1334, 437, 18, 18)),
                                  create_pixmap_label(":/board/board/house_4.png", QRect(1334, 418, 18, 18)),
                                  create_pixmap_label(":/board/board/house_4.png", QRect(1334, 399, 18, 18)),
                                  create_pixmap_label(":/board/board/house_4.png", QRect(1334, 380, 18, 18)),
                                  create_pixmap_label(":/board/board/hotel_4.png", QRect(1330, 406, 26, 24)) },
                                {},
                                {},
                                { create_pixmap_label(":/board/board/house_4.png", QRect(1334, 683, 18, 18)),
                                  create_pixmap_label(":/board/board/house_4.png", QRect(1334, 664, 18, 18)),
                                  create_pixmap_label(":/board/board/house_4.png", QRect(1334, 645, 18, 18)),
                                  create_pixmap_label(":/board/board/house_4.png", QRect(1334, 626, 18, 18)),
                                  create_pixmap_label(":/board/board/hotel_4.png", QRect(1330, 652, 26, 24)) },
                                {},
                                { create_pixmap_label(":/board/board/house_4.png", QRect(1334, 847, 18, 18)),
                                  create_pixmap_label(":/board/board/house_4.png", QRect(1334, 828, 18, 18)),
                                  create_pixmap_label(":/board/board/house_4.png", QRect(1334, 809, 18, 18)),
                                  create_pixmap_label(":/board/board/house_4.png", QRect(1334, 790, 18, 18)),
                                  create_pixmap_label(":/board/board/hotel_4.png", QRect(1330, 816, 26, 24)) }
    };
    return true;
}

void Game::refresh_button_states()
{
    last_state.dice_buttons_enabled = false;
    last_state.end_turn_button_enabled = false;
    last_state.action_hidden = false;
    last_state.action_name_hidden = false;
    last_state.action_desc_hidden = false;
    last_state.ok_button_hidden = false;
    last_state.accept_button_hidden = false;
    last_state.deny_button_hidden = false;
}

void Game::get_button_states()
{
    last_state.dice_buttons_enabled = dice1_button->isEnabled();
    last_state.end_turn_button_enabled = end_turn_button->isEnabled();
    last_state.action_hidden = action_label->isHidden();
    last_state.action_name_hidden = action_name_label->isHidden();
    last_state.action_desc_hidden = action_description_label->isHidden();
    last_state.ok_button_hidden = ok_button->isHidden();
    last_state.accept_button_hidden = accept_button->isHidden();
    last_state.deny_button_hidden = deny_button->isHidden();

    last_state.action_name_text = action_name_label->text();
    last_state.action_desc_text = action_description_label->text();
}

void Game::enable_six_buttons()
{
    build_house_button->setEnabled(true);
    sell_house_button->setEnabled(true);
    exchange_button->setEnabled(true);
    offers_button->setEnabled(true);
    hypothecation_button->setEnabled(true);
    buyback_button->setEnabled(true);
}

void Game::enable_next_throw()
{
    dice1_button->setEnabled(true);
    dice2_button->setEnabled(true);
    end_turn_button->setEnabled(false);
}

void Game::disable_next_throw()
{
    dice1_button->setEnabled(false);
    dice2_button->setEnabled(false);
    end_turn_button->setEnabled(true);
}

void Game::disable_next_turn()
{
    dice1_button->setEnabled(false);
    dice2_button->setEnabled(false);
    end_turn_button->setEnabled(false);
}

void Game::restore_button_states()
{
    dice1_button->setEnabled(last_state.dice_buttons_enabled);
    dice2_button->setEnabled(last_state.dice_buttons_enabled);
    end_turn_button->setEnabled(last_state.end_turn_button_enabled);
    enable_six_buttons();

    action_name_label->setText(last_state.action_name_text);
    action_description_label->setText(last_state.action_desc_text);
    if(last_state.action_hidden) action_label->hide();
    else action_label->show();
    if(last_state.action_name_hidden) action_name_label->hide();
    else action_name_label->show();
    if(last_state.action_desc_hidden) action_description_label->hide();
    else action_description_label->show();

    if(last_state.accept_button_hidden) accept_button->hide();
    else accept_button->show();
    if(last_state.ok_button_hidden) ok_button->hide();
    else ok_button->show();
    if(last_state.deny_button_hidden) deny_button->hide();
    else deny_button->show();
}

void Game::create_center_interface()
{
    board = create_pixmap_label(":/board/board/board.jpg", QRect(460, 0, 1000, 1000), 1000, 1000);
    dice1_label = create_pixmap_label(":/board/board/dice_1.png", QRect(620, 790, 50, 50), 50, 50);
    dice2_label = create_pixmap_label(":/board/board/dice_1.png", QRect(680, 790, 50, 50), 50, 50);
}

void Game::create_left_interface()
{
    player_label = create_label(QString::number(vector_players[0]->get_player_number() + 1) + "  " + vector_players[0]->get_player_name(), QFont("Open Sans", 30), "color: white", QRect(0, 0, 360, 60));
    color_player_label = create_pixmap_label(":/board/board/checker.png", QRect(390, 0, 60, 60), vector_players[0]->get_player_color());
    money_label = create_label("Finanse:  " + QString::number(vector_players[0]->get_player_money()) + "$", QFont("Open Sans", 30), "color: white", QRect(0, 60, 450, 60));

    create_left_buttons_interface();

    exclamation_mark_label = create_pixmap_label(":images/images/exclamation_mark.png", QRect(435, 305, 30, 30), 30, 30, "background-color: transparent", true);
    action_label = create_label("Zdarzenie", QFont("Open Sans", 30), "color: white", QRect(0, 600, 450, 60), Qt::AlignCenter);
    action_name_label = create_label("", QFont("Open Sans", 25), "color: white", QRect(0, 660, 450, 50), Qt::AlignCenter);
    action_description_label = create_label("", QFont("Open Sans", 16), "color: white", QRect(0, 710, 450, 150), Qt::AlignCenter);
    action_description_label->setWordWrap(true);

    message = create_label("", QFont("Open Sans", 16), "color: white", QRect(0, 920, 450, 80), Qt::AlignCenter);
    message->setWordWrap(true);
}

void Game::create_right_interface()
{
    player_cards_label = create_label("Karty gracza", QFont("Open Sans", 30), "color: white", QRect(1470, 0, 450, 60), Qt::AlignCenter);
    card_label = create_label("", QFont("Open Sans", 30), "color: white", QRect(1470, 650, 450, 50), Qt::AlignCenter);
    card_name_label = create_label("", QFont("Open Sans", 25), "color: white", QRect(1470, 700, 450, 50), Qt::AlignCenter);
    card_description_label = create_label("", QFont("Open Sans", 16), "color: white", QRect(1470, 750, 450, 250), Qt::AlignCenter);
    card_description_label->setWordWrap(true);
}

void Game::create_left_buttons_interface()
{
    build_house_button = create_pushbutton("Postaw budynek", QFont("Open Sans", 30), "color: white; background-color: #484852", QRect(0, 140, 450, 50));
    connect(build_house_button, SIGNAL(clicked()), this, SLOT(on_build_house_button_clicked()));
    sell_house_button = create_pushbutton("Sprzedaj budynek", QFont("Open Sans", 30), "color: white; background-color: #484852", QRect(0, 200, 450, 50));
    connect(sell_house_button, SIGNAL(clicked()), this, SLOT(on_sell_house_button_clicked()));
    exchange_button = create_pushbutton("Zaoferuj wymianę", QFont("Open Sans", 30), "color: white; background-color: #484852", QRect(0, 260, 450, 50));
    connect(exchange_button, SIGNAL(clicked()), this, SLOT(on_exchange_button_clicked()));
    offers_button = create_pushbutton("Lista ofert", QFont("Open Sans", 30), "color: white; background-color: #484852", QRect(0, 320, 450, 50));
    connect(offers_button, SIGNAL(clicked()), this, SLOT(on_offers_button_clicked()));
    hypothecation_button = create_pushbutton("Zastaw nieruchomość", QFont("Open Sans", 30), "color: white; background-color: #484852", QRect(0, 380, 450, 50));
    connect(hypothecation_button, SIGNAL(clicked()), this, SLOT(on_hypothecation_button_clicked()));
    buyback_button = create_pushbutton("Wykup nieruchomość", QFont("Open Sans", 30), "color: white; background-color: #484852", QRect(0, 440, 450, 50));
    connect(buyback_button, SIGNAL(clicked()), this, SLOT(on_buyback_button_clicked()));
    end_turn_button = create_pushbutton("Zakończ turę", QFont("Open Sans", 30), "color: white; background-color: #484852", QRect(0, 500, 450, 50), false);
    connect(end_turn_button, SIGNAL(clicked()), this, SLOT(on_end_turn_button_clicked()));

    accept_button = create_pushbutton("Tak", QFont("Open Sans", 25), "color: white; background-color: #484852", QRect(90, 880, 90, 40));
    connect(accept_button, SIGNAL(clicked()), this, SLOT(on_accept_button_clicked()));
    ok_button = create_pushbutton("Ok", QFont("Open Sans", 25), "color: white; background-color: #484852", QRect(180, 880, 90, 40));
    connect(ok_button, SIGNAL(clicked()), this, SLOT(on_ok_button_clicked()));
    deny_button = create_pushbutton("Nie", QFont("Open Sans", 25), "color: white; background-color: #484852", QRect(270, 880, 90, 40));
    connect(deny_button, SIGNAL(clicked()), this, SLOT(on_deny_button_clicked()));
}

void Game::hide_interface()
{
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

void Game::disable_reaction_buttons()
{
    dice1_button->setEnabled(false);
    dice2_button->setEnabled(false);
    end_turn_button->setEnabled(false);
}

void Game::disable_interface_buttons()
{
    dice1_button->setEnabled(false);
    dice2_button->setEnabled(false);
    end_turn_button->setEnabled(false);
    build_house_button->setEnabled(false);
    sell_house_button->setEnabled(false);
    exchange_button->setEnabled(false);
    offers_button->setEnabled(false);
    hypothecation_button->setEnabled(false);
    buyback_button->setEnabled(false);
}

void Game::show_action_labels()
{
    action_label->show();
    action_name_label->show();
    action_description_label->show();
}

void Game::show_card_labels()
{
    card_label->show();
    card_name_label->show();
    card_description_label->show();
}

void Game::restore_card_labels()
{
    bool show = false;
    if(!card_label->isHidden()) show = true;
    hide_interface();
    if(show) show_card_labels();
}

void Game::show_two_buttons()
{
    accept_button->show();
    deny_button->show();
}

void Game::change_action_labels(QString action_name, QString action_description)
{
    action_name_label->setText(action_name);
    action_description_label->setText(action_description);
}

void Game::clear_card_labels()
{
    card_label->setText("");
    card_name_label->setText("");
    card_description_label->setText("");
}

void Game::repeat_action(bool& enough)
{
    enough = false;
    repeat_action();
}

void Game::repeat_action()
{
    action_label->show();
    action_name_label->show();
    action_description_label->show();
    ok_button->show();
    show_message("Zbyt mało funduszy do wykonania akcji!");
}

void Game::change_one_money_label(int money)
{
    current_player->change_money(money);
    money_label->setText("Finanse:  " + QString::number(current_player->get_player_money()) + "$");
}

void Game::change_money_start_label()
{
    current_player->change_money(200);
    money_label->setText("Finanse:  " + QString::number(current_player->get_player_money()) + "$");
    show_message("Otrzymałeś 200$ za przejście przez start");
}

void Game::initialize_players()
{
    for(int i = 0; i < number_of_players; i++)
        vector_players.push_back(create_player(vector_name_players[i], vector_color_players[i], i));
    current_player = vector_players[0];
}

void Game::initialize_dices()
{
    dublet = false;
    dublet_turn = 0;
    dice1 = create_dice();
    dice2 = create_dice();
}

void Game::initialize_vector_estates()
{
    std::vector<std::shared_ptr<Field>> estate;
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

void Game::initialize_vector_chance_cards()
{
    fixing_cost = 0;
    chance_card_index = 0;

    std::vector<QString> vector_descriptions = { "Grzywna - zapłać 20$",
                                                 "Zapłać za szkołę - 150$",
                                                 "Otrzymałeś kredyt budowlany;"
                                                    "\nPobierz 150$",
                                                 "Cofnij się o trzy pola",
                                                 "Mandat za przekroczenie prędkości;"
                                                    "\nZapłać 15$",
                                                 "Przejdź na Boardwalk",
                                                 "Idź do więzienia;"
                                                    "\nPrzejdź prosto do więzienia;"
                                                    "\nNie przechodź przez start;"
                                                    "\nNie pobieraj 200$",
                                                 "Wyjdź bezpłatnie z więzienia;"
                                                    "\nTę kartę możesz wykorzystać podczas pobytu w więzieniu",
                                                 "Bank wypłaca Ci dywidendę w wysokości 50$",
                                                 "Wygrałeś konkurs krzyżówkowy - pobierz 100$",
                                                 "Przejdź na start",
                                                 "Zostałeś obciążony kosztami napraw - zapłać:"
                                                    "\n40$ za każdy dom"
                                                    "\n115$ za każdy hotel",
                                                 "Przejdź na dworzec Pennsylvania;"
                                                    "\nJeśli miniesz po drodze start, pobierz 200$",
                                                 "Przejdź na Illinois Avenue"
                                                    "\nJeśli miniesz po drodze start, pobierz 200$",
                                                 "Przeprowadź generalny remont wszystkich budynków;"
                                                    "\nZapłać po 25$ za każdy dom"
                                                    "\nZapłać po 100$ za każdy hotel",
                                                 "Przejdź na St. Charles Place;"
                                                    "\nJeśli miniesz po drodze start, pobierz 200$"
    };
    for(int i = 0; i < 16; i++)
        vector_chance_cards.push_back(create_chance_card(i, vector_descriptions[i]));
    shuffle_cards(true);
}

void Game::initialize_vector_community_chest_cards()
{
    community_chest_card_index = 0;
    comm_card = false;

    std::vector<QString> vector_descriptions = { "Odziedziczyłeś w spadku 100$",
                                                 "Zapłać rachunek za szpital - 100$",
                                                 "Otrzymałeś zwrot podatku dochodowego;"
                                                    "\nPobierz 20$",
                                                 "Otrzymujesz 50$ za sprzedane akcje",
                                                 "Zapłać składkę ubezpieczeniową - 50$",
                                                 "Idź do więzienia;"
                                                    "\nPrzejdź prosto do więzienia;"
                                                    "\nNie przechodź przez start;"
                                                    "\nNie pobieraj 200$",
                                                 "Wyjdź bezpłatnie z więzienia;"
                                                    "\nTę kartę możesz wykorzystać podczas pobytu w więzieniu",
                                                 "Honorarium lekarza - zapłać 50$",
                                                 "Sprzedałeś obligacje - pobierz 100$",
                                                 "Przejdź na start",
                                                 "Wygrałeś drugą nagrodę w konkursie piękności;"
                                                    "\nPobierz 10$",
                                                 "Otrzymałeś odsetki od lokaty terminowej;"
                                                    "\nPobierz 25$",
                                                 "Błąd bankowy na Twoją korzyść - pobierz 200$",
                                                 "Dziś są Twoje urodziny;"
                                                    "\nPobierz po 10$ od każdego gracza",
                                                 "Wróć na Mediter-Ranean Avenue",
                                                 "Zapłać 10$ lub weź kartę Szansa;"
                                                    "\nCzy chcesz wziąć kartę Szansa?"
    };
    for(int i = 0; i < 15; i++)
        vector_community_chest_cards.push_back(create_chance_card(i, vector_descriptions[i]));
    vector_community_chest_cards.push_back(create_chance_card(15, vector_descriptions[15], false));
    shuffle_cards(false);
}

void Game::shuffle_cards(bool chance_cards)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);
    if(chance_cards)
    {
        std::ranges::shuffle(vector_chance_cards, rng);
        std::ranges::shuffle(vector_chance_cards | std::views::take(8), rng);
        std::ranges::shuffle(vector_chance_cards | std::views::reverse | std::views::take(8), rng);
        auto vector_chance_cards_view = vector_chance_cards | std::views::all;
        for(const auto& el : vector_chance_cards_view)
            qDebug() << el->get_description() << "\n";
    }
    else
    {
        std::ranges::shuffle(vector_community_chest_cards, rng);
        std::ranges::shuffle(vector_community_chest_cards | std::views::take(8), rng);
        std::ranges::shuffle(vector_community_chest_cards | std::views::reverse | std::views::take(8), rng);
        auto vector_community_chest_cards_view = vector_community_chest_cards | std::views::all;
        for(const auto& el : vector_community_chest_cards_view)
            qDebug() << el->get_description() << "\n";
    }
}

void Game::check_position(std::shared_ptr<Field> actual_field)
{
    disable_reaction_buttons();
    show_action_labels();

    QString class_type = actual_field->get_classname();
    int entrance_cost = find_entrance_cost(actual_field);
    if(class_type == "field_property" || class_type == "field_station" || class_type == "field_special")
    {
        if(actual_field->get_owner() == "Bank")
        {
            change_action_labels("Zakup pola", "Czy chcesz kupić " + actual_field->get_name() + " za kwotę " + ( actual_field->get_popped() ? QString::number(int(actual_field->get_buying_cost() * 0.55)) : QString::number(actual_field->get_buying_cost()))  + "$?");
            show_two_buttons();
        }
        else if(actual_field->get_number_owner() == current_player->get_player_number())
        {
            change_action_labels("Wejście na nieruchomość", "Nieruchomość " + actual_field->get_name() + " należy do Ciebie, nie musisz wnosić żadnych opłat");
            ok_button->show();
        }
        else
        {
            if(actual_field->get_popped()) change_action_labels("Wejście na nieruchomość", "Nieruchomość " + actual_field->get_name() + " jest zastawiona, nie musisz wnosić żadnych opłat");
            else change_action_labels("Wejście na nieruchomość", "Wszedłeś na nieruchomość " + actual_field->get_name() + ", opłata wynosi " + QString::number(entrance_cost) + "$");
            ok_button->show();
        }
    }
    else if(class_type == "field_chance")
    {
        if(current_player->get_player_position() == 7 || current_player->get_player_position() == 22 || current_player->get_player_position() == 36)
        {
            chance_card_index++;
            if(chance_card_index == 16) chance_card_index = 0;
            if(vector_chance_cards[chance_card_index]->get_id_number() == 7 && check_for_prison_card(true))
            {
                chance_card_index++;
                if(chance_card_index == 16) chance_card_index = 0;
            }
            change_action_labels("Szansa", vector_chance_cards[chance_card_index]->get_description());
            if(chance_card_index == 11) fixing_cost = find_fixing_cost(40, 115);
            if(chance_card_index == 14) fixing_cost = find_fixing_cost(25, 100);
            ok_button->show();
        }
        else
        {
            community_chest_card_index++;
            if(community_chest_card_index == 16) community_chest_card_index = 0;
            if(vector_chance_cards[community_chest_card_index]->get_id_number() == 6 && check_for_prison_card(false))
            {
                community_chest_card_index++;
                if(community_chest_card_index == 16) community_chest_card_index = 0;
            }
            change_action_labels("Kasa społeczna", vector_community_chest_cards[community_chest_card_index]->get_description());
            if(vector_community_chest_cards[community_chest_card_index]->get_one_button()) ok_button->show();
            else show_two_buttons();
        }
    }
    else
    {
        if(actual_field->get_name() == "Pole startowe") change_action_labels(actual_field->get_name(), "Wszedłeś na pole startowe, otrzymujesz 200$");
        else if(actual_field->get_name() == "Bezpłatny parking") change_action_labels(actual_field->get_name(), "Wszedłeś na bezpłatny parking, możesz odpocząć");
        else if(actual_field->get_name() == "Więzienie") change_action_labels(actual_field->get_name(), "Odwiedzasz więzienie, możesz odpocząć");
        else change_action_labels(actual_field->get_name(), "Musisz zapłacić podatek o wartości " + QString::number(entrance_cost) + "$");
        ok_button->show();
    }
}

bool Game::check_for_prison_card(bool chance_card)
{
    bool found = false;
    if(chance_card)
        for(const auto& player : vector_players)
        {
            if(player->get_player_chance_prison_card()) found = true;
            if(found) break;
        }
    else
        for(const auto& player : vector_players)
        {
            if(player->get_player_comm_prison_card()) found = true;
            if(found) break;
        }
    return found;
}

void Game::react_card_chance(int id_number, bool& second_button, bool& enough)
{
    int actual_position = current_player->get_player_position();
    int calculated_position = 0;
    switch(id_number)
    {
        case 0:
            if(check_bankruptcy(current_player, 20)) player_bankrupted(current_player, -1, 20);
            else if(20 > current_player->get_player_money()) repeat_action(enough);
            else change_one_money_label(-20);
            break;
        case 1:
            if(check_bankruptcy(current_player, 150)) player_bankrupted(current_player, -1, 150);
            else if(150 > current_player->get_player_money()) repeat_action(enough);
            else change_one_money_label(-150);
            break;
        case 2:
            change_one_money_label(150);
            break;
        case 3:
            calculated_position = actual_position - 3;
            if(calculated_position < 0) calculated_position += 40;
            change_player_position(calculated_position, second_button, false);
            break;
        case 4:
            if(check_bankruptcy(current_player, 15)) player_bankrupted(current_player, -1, 15);
            else if(15 > current_player->get_player_money()) repeat_action(enough);
            else change_one_money_label(-15);
            break;
        case 5:
            change_player_position(39, second_button);
            break;
        case 6:
            prison_player();
            break;
        case 7:
            add_prison_card(true);
            break;
        case 8:
            change_one_money_label(50);
            break;
        case 9:
            change_one_money_label(-100);
            break;
        case 10:
            change_player_position(39, second_button, false);
            change_money_start_label();
            break;
        case 11:
            if(check_bankruptcy(current_player, fixing_cost)) player_bankrupted(current_player, -1, fixing_cost);
            else if(fixing_cost > current_player->get_player_money()) repeat_action(enough);
            else change_one_money_label(-1 * fixing_cost);
            break;
        case 12:
            change_player_position(15, second_button);
            break;
        case 13:
            change_player_position(24, second_button);
            break;
        case 14:
            if(check_bankruptcy(current_player, fixing_cost)) player_bankrupted(current_player, -1, fixing_cost);
            else if(fixing_cost > current_player->get_player_money()) repeat_action(enough);
            else change_one_money_label(-1 * fixing_cost);
            break;
        case 15:
            change_player_position(11, second_button);
            break;
    }
}

void Game::react_card_community_chest(int id_number, bool& second_button, bool& enough)
{
    switch(id_number)
    {
        case 0:
            change_one_money_label(100);
            break;
        case 1:
            if(check_bankruptcy(current_player, 100)) player_bankrupted(current_player, -1, 100);
            else if(100 > current_player->get_player_money()) repeat_action(enough);
            else change_one_money_label(-100);
            break;
        case 2:
            change_one_money_label(20);
            break;
        case 3:
            change_one_money_label(50);
            break;
        case 4:
            if(check_bankruptcy(current_player, 50)) player_bankrupted(current_player, -1, 50);
            else if(50 > current_player->get_player_money()) repeat_action(enough);
            else change_one_money_label(-50);
            break;
        case 5:
            prison_player();
            break;
        case 6:
            add_prison_card(false);
            break;
        case 7:
            if(check_bankruptcy(current_player, 50)) player_bankrupted(current_player, -1, 50);
            else if(50 > current_player->get_player_money()) repeat_action(enough);
            else change_one_money_label(-50);
            break;
        case 8:
            change_one_money_label(100);
            break;
        case 9:
            change_player_position(0, second_button, false);
            change_money_start_label();
            break;
        case 10:
            change_one_money_label(10);
            break;
        case 11:
            change_one_money_label(25);
            break;
        case 12:
            change_one_money_label(200);
            break;
        case 13:
            for(const auto& player : vector_players)
            {
                if(player->get_player_bankrupt() || player == current_player) continue;
                if(check_bankruptcy(player, 10))
                {
                    player_bankrupted(player, -1, 50, false);
                    show_message("Gracz " + QString::number(player->get_player_number() + 1) + " posiadał zbyt mało funduszy do wykonania akcji!");
                }
                else if(10 > player->get_player_money())
                {
                    get_money_automatically(player);
                    exchange_money(player, current_player, 10);
                }
                else exchange_money(player, current_player, 10);
            }
            money_label->setText("Finanse:  " + QString::number(current_player->get_player_money()) + "$");
            break;
        case 14:
            change_player_position(1, second_button, false);
            break;
        case 15:
            break;
        }
}

void Game::change_player_position(int new_position, bool &second_button, bool check_for_money)
{
    if(check_for_money)
        if(current_player->get_player_position() >= new_position) change_money_start_label();
    current_player->set_player_position(new_position);
    vector_chequers[current_player->get_player_number()]->setGeometry(chequers_position_table[new_position][current_player->get_player_number()]);
    check_position(vector_fields[new_position]);
    second_button = true;
}

void Game::prison_player()
{
    current_player->set_player_position(10);
    vector_chequers[current_player->get_player_number()]->setGeometry(chequers_position_table[40][current_player->get_player_number()]);
    current_player->set_prisoned(true);
    dublet = false;
    dice1_button->setEnabled(false);
    dice2_button->setEnabled(false);
    end_turn_button->setEnabled(true);
}

void Game::add_prison_card(bool chance_card)
{
    if(chance_card)
    {
        current_player->set_player_chance_prison_card(true);
        current_player->add_field(vector_fields[current_player->get_player_position()]);
    }
    else
    {
        current_player->set_player_comm_prison_card(true);
        current_player->add_field(vector_fields[current_player->get_player_position()]);
    }
    change_vector_cards_label();
}

std::shared_ptr<Field> Game::find_prison_card(bool chance_card)
{
    std::shared_ptr<Field> chance_field = nullptr;
    std::vector<std::shared_ptr<Field>> vector_player_fields = current_player->get_vector_players_fields();
    if(chance_card)
    {
        for(size_t i = 0; i < vector_player_fields.size(); i++)
            if(vector_player_fields[i]->get_name() == "Szansa")
                chance_field = vector_player_fields[i];
    }
    else
        for(size_t i = 0; i < vector_player_fields.size(); i++)
            if(vector_player_fields[i]->get_name() == "Kasa społeczna")
                chance_field = vector_player_fields[i];
    return chance_field;
}

int Game::find_fixing_cost(int house_cost, int hotel_cost)
{
    int result = 0;
    int number_of_houses = 0;
    int number_of_hotels = 0;
    std::vector<std::shared_ptr<Field>> vector_players_field = current_player->get_vector_players_fields();
    for(size_t i = 0; i < vector_players_field.size(); i++)
    {
        if(vector_players_field[i]->get_classname() == "field_property")
        {
            if(vector_players_field[i]->get_number_of_houses() == 5) number_of_hotels++;
            else number_of_houses += vector_players_field[i]->get_number_of_houses();
        }
    }
    result = house_cost * number_of_houses + hotel_cost * number_of_hotels;
    return result;
}

int Game::find_entrance_cost(std::shared_ptr<Field> actual_field)
{
    int result = 0;
    if(actual_field->get_classname() == "field")
    {
        if(actual_field->get_name() == "Podatek dochodowy") result = 200;
        if(actual_field->get_name() == "Domiar podatkowy") result = 100;
    }
    else if(actual_field->get_owner() != "Bank")
    {
        int number_player_property = actual_field->get_number_owner();
        if(actual_field->get_classname() == "field_station")
        {
            if(vector_players[number_player_property]->get_player_number_of_stations() == 1) result = 25;
            if(vector_players[number_player_property]->get_player_number_of_stations() == 2) result = 50;
            if(vector_players[number_player_property]->get_player_number_of_stations() == 3) result = 100;
            if(vector_players[number_player_property]->get_player_number_of_stations() == 4) result = 200;
        }
        else if(actual_field->get_classname() == "field_special")
        {
            if(vector_players[number_player_property]->get_player_number_of_special() == 1) result = (dice1->get_number() + dice2->get_number()) * 4;
            if(vector_players[number_player_property]->get_player_number_of_special() == 2) result = (dice1->get_number() + dice2->get_number()) * 10;
        }
        else if(actual_field->get_classname() == "field_property")
        {
            std::pair<bool, bool> estate = find_estate(actual_field, number_player_property);
            if(estate.first && !estate.second) result = actual_field->get_payment(0) * 2;
            else result = actual_field->get_payment(actual_field->get_number_of_houses());
        }
    }
    return result;
}

std::pair<bool, bool> Game::find_estate(std::shared_ptr<Field> actual_field, int number_player_property)
{
    std::vector<bool> found_field = { false, false, false };
    bool found_field_houses = false;
    std::vector<std::shared_ptr<Field>> vector_player_fields = vector_players[number_player_property]->get_vector_players_fields();
    bool found = false;
    std::vector<std::shared_ptr<Field>> estate;
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

std::vector<std::shared_ptr<Field>> Game::get_estate(std::shared_ptr<Field> actual_field)
{
    bool found = false;
    std::vector<std::shared_ptr<Field>> estate;
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
    return estate;
}

void Game::build_house()
{
    vector_fields[number_field_button_clicked]->add_number_of_houses();
    int money_to_pay = 0;
    if(vector_fields[number_field_button_clicked]->get_number_of_houses() == 5) money_to_pay = vector_fields[number_field_button_clicked]->get_building_cost() * 5;
    else money_to_pay = vector_fields[number_field_button_clicked]->get_building_cost();
    current_player->change_money(money_to_pay * -1);
    money_label->setText("Finanse:  " + QString::number(current_player->get_player_money()) + "$");
    for(int i = 0; i < buildings_labels_table[number_field_button_clicked].size(); i++)
        buildings_labels_table[number_field_button_clicked][i]->hide();
    if(vector_fields[number_field_button_clicked]->get_number_of_houses() == 5) buildings_labels_table[number_field_button_clicked][4]->show();
    else
        for(int i = 0; i < vector_fields[number_field_button_clicked]->get_number_of_houses(); i++)
            buildings_labels_table[number_field_button_clicked][i]->show();
}

void Game::sell_house()
{
    vector_fields[number_field_button_clicked]->substract_number_of_houses();
    int money_to_receive = 0;
    if(vector_fields[number_field_button_clicked]->get_number_of_houses() == 4) money_to_receive = vector_fields[number_field_button_clicked]->get_building_cost() * 2.5;
    else money_to_receive = vector_fields[number_field_button_clicked]->get_building_cost() * 0.5;
    current_player->change_money(money_to_receive);
    money_label->setText("Finanse:  " + QString::number(current_player->get_player_money()) + "$");
    for(int i = 0; i < buildings_labels_table[number_field_button_clicked].size(); i++)
        buildings_labels_table[number_field_button_clicked][i]->hide();
    for(int i = 0; i < vector_fields[number_field_button_clicked]->get_number_of_houses(); i++)
        buildings_labels_table[number_field_button_clicked][i]->show();
}

void Game::hyphothecate_property()
{
    vector_fields[number_field_button_clicked]->set_popped(true);
    current_player->change_money(vector_fields[number_field_button_clicked]->get_buying_cost() * 0.5);
    if(vector_fields[number_field_button_clicked]->get_classname() == "field_special") current_player->substract_number_of_special();
    else if(vector_fields[number_field_button_clicked]->get_classname() == "field_station") current_player->substract_number_of_stations();
    money_label->setText("Finanse:  " + QString::number(current_player->get_player_money()) + "$");
    actualize_field_state(vector_fields[number_field_button_clicked]);
    card_description_label->setText(vector_fields[number_field_button_clicked]->get_description());
    change_vector_cards_label();
}

void Game::buyback_property()
{
    vector_fields[number_field_button_clicked]->set_popped(false);
    current_player->change_money(vector_fields[number_field_button_clicked]->get_buying_cost() * 0.55 * -1);
    if(vector_fields[number_field_button_clicked]->get_classname() == "field_special") current_player->add_number_of_special();
    else if(vector_fields[number_field_button_clicked]->get_classname() == "field_station") current_player->add_number_of_stations();
    money_label->setText("Finanse:  " + QString::number(current_player->get_player_money()) + "$");
    actualize_field_state(vector_fields[number_field_button_clicked]);
    card_description_label->setText(vector_fields[number_field_button_clicked]->get_description());
    change_vector_cards_label();
}

void Game::actualize_field_state(std::shared_ptr<Field> actual_field)
{
    QString class_type = actual_field->get_classname();
    if(class_type == "field_property")
        actual_field->set_description(
            "Cena zakupu: " + QString::number(actual_field->get_buying_cost()) + "$" +
            "\nObszar niezabudowany: " + QString::number(actual_field->get_payment(0)) + "$" +
            "\n1 dom: " + QString::number(actual_field->get_payment(1)) + "$" +
            "\n2 domy: " + QString::number(actual_field->get_payment(2)) + "$" +
            "\n3 domy: " + QString::number(actual_field->get_payment(3)) + "$" +
            "\n4 domy: " + QString::number(actual_field->get_payment(4)) + "$" +
            "\nHotel: " + QString::number(actual_field->get_payment(5)) + "$" +
            "\nCena za 1 dom: " + QString::number(actual_field->get_building_cost()) + "$"
            "\nWłaściciel/Numer właściciela: " + (actual_field->get_owner() == "Bank" ? "Bank" : QString::number(actual_field->get_number_owner() + 1)) +
            "\nZastaw: " + (actual_field->get_popped() ? "Tak" : "Nie")
            );
    else if(class_type == "field_station")
        actual_field->set_description(
            "Właściciel podanej ilości dworców pobiera:"
            "\n1 dworzec - 25$"
            "\n2 dworce - 50$"
            "\n3 dworce - 100$"
            "\n4 dworce - 200$"
            "\nKoszt zakupu: 200$"
            "\nWłaściciel/Numer właściciela: " + (actual_field->get_owner() == "Bank" ? "Bank" : QString::number(actual_field->get_number_owner() + 1)) +
            "\nZastaw: " + (actual_field->get_popped() ? "Tak" : "Nie")
            );
    else if(class_type == "field_special")
        actual_field->set_description(
            "Właściciel jednego obiektu"
            "\nużyteczności publicznej pobiera"
            "\nczterokrotność sumy oczek;"
            "\nWłaściciel obu obiektów"
            "\nużyteczności publicznej pobiera"
            "\ndziesięciokrotną sumę oczek"
            "\nCena zakupu: 150$"
            "\nWłaściciel/Numer właściciela: " + (actual_field->get_owner() == "Bank" ? "Bank" : QString::number(actual_field->get_number_owner() + 1)) +
            "\nZastaw: " + (actual_field->get_popped() ? "Tak" : "Nie")
            );
}

bool Game::check_bankruptcy(std::shared_ptr<Player> player, int cost)
{
    int finance = 0;
    finance += player->get_player_money();
    std::vector<std::shared_ptr<Field>> vector_players_fields = player->get_vector_players_fields();
    for(size_t i = 0; i < vector_players_fields.size(); i++)
        if(vector_players_fields[i]->get_classname() != "field" && vector_players_fields[i]->get_classname() != "field_chance")
            if(!vector_players_fields[i]->get_popped())
                finance += 0.5 * (vector_players_fields[i]->get_buying_cost() + vector_players_fields[i]->get_building_cost() * vector_players_fields[i]->get_number_of_houses());
    if(cost > finance) return true;
    else return false;
}

void Game::player_bankrupted(std::shared_ptr<Player> player, int owner_number, int money_to_pay, bool actual_player)
{
    player->set_bankrupt(true);
    if(actual_player) organize_bankrupt_player(player, owner_number, money_to_pay);
    else organize_bankrupt_player(player, owner_number, money_to_pay, false);
}

void Game::organize_bankrupt_player(std::shared_ptr<Player> bankrupt_player, int player_property_number, int money_to_pay, bool actual_player)
{
    std::vector<std::shared_ptr<Field>> vector_players_fields = bankrupt_player->get_vector_players_fields();
    if(player_property_number == -1)
    {
        for(size_t i = 0; i < vector_players_fields.size(); i++)
        {
            vector_players_fields[i]->set_number_owner(-1);
            vector_players_fields[i]->set_owner("Bank");
            if(vector_players_fields[i]->get_classname() == "field_property") vector_players_fields[i]->reset_number_of_houses();
            if(!vector_players_fields[i]->get_popped()) vector_players_fields[i]->set_popped(true);
        }
        if(bankrupt_player->get_player_chance_prison_card()) bankrupt_player->set_player_chance_prison_card(false);
        if(bankrupt_player->get_player_comm_prison_card()) bankrupt_player->set_player_comm_prison_card(false);
    }
    else
    {
        std::shared_ptr<Player> creditor = vector_players[player_property_number];
        for(size_t i = 0; i < vector_players_fields.size(); i++)
        {
            vector_players_fields[i]->set_number_owner(player_property_number);
            vector_players_fields[i]->set_owner("Player");
            creditor->add_field(vector_players_fields[i]);
            if(vector_players_fields[i]->get_classname() == "field_station" && !vector_players_fields[i]->get_popped()) creditor->add_number_of_stations();
            if(vector_players_fields[i]->get_classname() == "field_special" && !vector_players_fields[i]->get_popped()) creditor->add_number_of_special();
            if(vector_players_fields[i]->get_classname() == "field_property") vector_players_fields[i]->reset_number_of_houses();
            if(!vector_players_fields[i]->get_popped()) vector_players_fields[i]->set_popped(true);
            actualize_field_state(vector_players_fields[i]);
        }
        if(bankrupt_player->get_player_chance_prison_card())
        {
            bankrupt_player->set_player_chance_prison_card(false);
            creditor->set_player_chance_prison_card(true);
        }
        if(bankrupt_player->get_player_comm_prison_card())
        {
            bankrupt_player->set_player_comm_prison_card(false);
            creditor->set_player_comm_prison_card(true);
        }
        creditor->change_money(money_to_pay);
    }

    bankrupt_player->reset_vector_players_fields();
    bankrupt_player->reset_number_of_stations();
    bankrupt_player->reset_number_of_special();
    bankrupt_player->reset_money();

    if(actual_player)
    {
        money_label->setText("Finanse:  " + QString::number(current_player->get_player_money()) + "$");
        change_vector_cards_label();
        show_action_labels();
        change_action_labels("Bankructwo!", "Zbankrutowałeś!"
                                                "\nNaciśnij OK by zakończyć swoją grę!");
        ok_button->show();
    }
}

void Game::get_money_automatically(std::shared_ptr<Player> player)
{
    bool found = false;
    std::vector<std::shared_ptr<Field>> vector_players_fields = player->get_vector_players_fields();
    for(size_t j = 0; j < vector_players_fields.size(); j++)
    {
        if(vector_players_fields[j]->get_classname() == "field_property")
        {
            if(vector_players_fields[j]->get_number_of_houses() != 0)
            {
                int number_of_houses = vector_players_fields[j]->get_number_of_houses();
                int max_number_of_houses = 0;
                std::vector<std::shared_ptr<Field>> estate = get_estate(vector_players_fields[j]);
                for(size_t i = 0; i < estate.size(); i++)
                    if(estate[i]->get_number_of_houses() > max_number_of_houses) max_number_of_houses = estate[i]->get_number_of_houses();
                if(number_of_houses == max_number_of_houses && number_of_houses > 0)
                {
                    vector_players_fields[j]->substract_number_of_houses();
                    player->change_money(vector_players_fields[j]->get_building_cost() * 0.5);
                    for(int i = 0; i < buildings_labels_table[j].size(); i++)
                        buildings_labels_table[j][i]->hide();
                    for(int i = 0; i < vector_players_fields[j]->get_number_of_houses(); i++)
                        buildings_labels_table[j][i]->show();
                    found = true;
                    break;
                }
            }
        }
    }
    if(!found)
    {
        for(size_t j = 0; j < vector_players_fields.size(); j++)
        {
            if(vector_players_fields[j]->get_classname() != "field_chance")
            {
                if(!vector_players_fields[j]->get_popped())
                {
                    if(vector_players_fields[j]->get_classname() == "field_station" || vector_players_fields[j]->get_classname() == "field_special")
                    {
                        vector_players_fields[j]->set_popped(true);
                        player->change_money(vector_players_fields[j]->get_buying_cost() * 0.5);
                        if(vector_players_fields[j]->get_classname() == "field_special") vector_players[vector_players_fields[j]->get_number_owner()]->substract_number_of_special();
                        else vector_players[vector_players_fields[j]->get_number_owner()]->substract_number_of_stations();
                        actualize_field_state(vector_players_fields[j]);
                        change_vector_cards_label();
                        break;
                    }
                    std::pair<bool, bool> found_estate = find_estate(vector_players_fields[j], player->get_player_number());
                    if(vector_players_fields[j]->get_classname() == "field_property" && !found_estate.second)
                    {
                        vector_players_fields[j]->set_popped(true);
                        player->change_money(vector_players_fields[j]->get_buying_cost() * 0.5);
                        actualize_field_state(vector_players_fields[j]);
                        change_vector_cards_label();
                        break;
                    }
                }
            }
        }
    }
}

void Game::exchange_money(std::shared_ptr<Player> player_to_give, std::shared_ptr<Player> player_to_get, int money)
{
    player_to_give->change_money(-1 * money);
    player_to_get->change_money(money);
}

void Game::exchange_money_label(std::shared_ptr<Player> player_to_give, std::shared_ptr<Player> player_to_get, int money)
{
    player_to_give->change_money(-1 * money);
    player_to_get->change_money(money);
    money_label->setText("Finanse:  " + QString::number(current_player->get_player_money()) + "$");
}

void Game::change_ownership(std::shared_ptr<Field> actual_field, QString owner, int owner_number)
{
    actual_field->set_owner(owner);
    actual_field->set_number_owner(owner_number);
    vector_players[owner_number]->add_field(actual_field);
    actualize_field_state(actual_field);
    change_vector_cards_label();
}

void Game::prepare_before_next_player()
{
    dublet = false;
    dublet_turn = 0;
    fixing_cost = 0;
    current_player->delete_all_offers();
    exclamation_mark_label->hide();
}

void Game::prepare_next_player()
{
    player_label->setText(QString::number(current_player->get_player_number() + 1) + "  " + current_player->get_player_name());
    QPixmap p(":/board/board/checker.png");
    p.fill(current_player->get_player_color());
    color_player_label->setPixmap(p);
    change_vector_cards_label();
    message->setText("");
    if(current_player->get_vector_players_offers().size()) exclamation_mark_label->show();
    enable_next_throw();
    enable_six_buttons();
    hide_interface();

    if(current_player->get_player_prisoned())
    {
        if(current_player->get_player_prison_turn() < 4)
        {
            show_action_labels();
            change_action_labels("Wyjdź z więzienia", "Numer tury spędzonej w więzieniu: " + QString::number(current_player->get_player_prison_turn()) +
                                                        "\nZapłać 50$ lub rzuć kością");
            ok_button->show();
        }
        if(current_player->get_player_prison_turn() == 4)
        {
            if(check_bankruptcy(current_player, 50)) player_bankrupted(current_player, -1, 50);
            else if(50 > current_player->get_player_money())
            {
                get_money_automatically(current_player);
                current_player->change_money(-50);
            }
            else
            {
                current_player->change_money(-50);
                current_player->set_prisoned(false);
                current_player->reset_prison_turn();
            }
        }
    }
    money_label->setText("Finanse:  " + QString::number(current_player->get_player_money()) + "$");
}

void Game::find_next_player()
{
    int current_player_number = current_player->get_player_number();
    if(current_player->get_player_prisoned()) current_player->add_prison_turn();
    do
    {
        current_player_number++;
        if(current_player_number == number_of_players) current_player_number = 0;
        current_player = vector_players[current_player_number];
    }
    while(current_player->get_player_bankrupt());
}

bool Game::check_win()
{
    bool win = false;
    int number_of_bankrupts = 0;
    for(int i = 0; i < number_of_players; i++)
        if(vector_players[i]->get_player_bankrupt()) number_of_bankrupts++;
    if(number_of_players - 1 == number_of_bankrupts) win = true;
    return win;
}

void Game::win_procedure()
{
    WinDialog(this, vector_players).execute();
    change_page();
}

void Game::show_message(QString text)
{
    QTimer::singleShot(1, [=](){ message->setText(text); });
    QTimer::singleShot(2500, [=]()
    {
        if(view_number == 0)
            message->setText("");
        if(view_number == 1)
            message->setText("Tryb budowania");
        if(view_number == 2)
            message->setText("Tryb wyburzania");
        if(view_number == 3)
            message->setText("Tryb zastawiania");
        if(view_number == 4)
            message->setText("Tryb wykupywania");
    });
}

std::shared_ptr<Field> Game::create_field(QString n, QString desc)
{
    std::shared_ptr<Field> f = std::make_shared<Field>(Field(n, desc));
    return f;
}

std::shared_ptr<Field> Game::create_field_chance(QString n, QString desc)
{
    std::shared_ptr<Field> f = std::make_shared<FieldChance>(FieldChance(n, desc));
    return f;
}

std::shared_ptr<Field> Game::create_field_special(QString n, QString desc)
{
    std::shared_ptr<Field> f = std::make_shared<FieldSpecial>(FieldSpecial(n, desc));
    return f;
}

std::shared_ptr<Field> Game::create_field_station(QString n, QString desc)
{
    std::shared_ptr<Field> f = std::make_shared<FieldStation>(FieldStation(n, desc));
    return f;
}

std::shared_ptr<Field> Game::create_field_property(QString n, QString desc, std::vector<int> v, int buy, int build)
{
    std::shared_ptr<Field> f = std::make_shared<FieldProperty>(FieldProperty(n, desc, v, buy, build));
    return f;
}

std::shared_ptr<Player> Game::create_player(QString name, QColor color, int number)
{
    std::shared_ptr<Player> p = std::make_shared<Player>(Player(name, color, number));
    return p;
}

std::shared_ptr<Dice> Game::create_dice()
{
    std::shared_ptr<Dice> d = std::make_shared<Dice>(Dice());
    return d;
}

std::shared_ptr<ChanceCard> Game::create_chance_card(int number, QString desc, bool one)
{
    std::shared_ptr<ChanceCard> c = std::make_shared<ChanceCard>(ChanceCard(number, desc, one));
    return c;
}

void Game::on_build_house_button_clicked()
{
    if(view_number == 0)
    {
        get_button_states();
        hide_interface();
        disable_interface_buttons();
        show_card_labels();
        clear_card_labels();
        build_house_button->setEnabled(true);
        build_house_button->setText("Powrót");
        message->setText("Tryb budowania");
        view_number = 1;
    }
    else
    {
        restore_button_states();
        show_card_labels();
        clear_card_labels();
        build_house_button->setText("Postaw budynek");
        message->setText("");
        view_number = 0;
    }
}

void Game::on_sell_house_button_clicked()
{
    if(view_number == 0)
    {
        get_button_states();
        hide_interface();
        disable_interface_buttons();
        show_card_labels();
        clear_card_labels();
        sell_house_button->setEnabled(true);
        sell_house_button->setText("Powrót");
        message->setText("Tryb wyburzania");
        view_number = 2;
    }
    else
    {
        restore_button_states();
        show_card_labels();
        clear_card_labels();
        sell_house_button->setText("Sprzedaj budynek");
        message->setText("");
        view_number = 0;
    }
}

void Game::on_exchange_button_clicked()
{
    ExchangeDialog(this, vector_players, current_player, players_offers).execute();
}

void Game::on_offers_button_clicked()
{
    if(current_player->get_vector_players_offers().size()) OffersDialog(this, vector_players, current_player, players_offers).execute();
    else show_message("Brak ofert do wyświetlenia!");
}

void Game::on_hypothecation_button_clicked()
{
    if(view_number == 0)
    {
        get_button_states();
        hide_interface();
        disable_interface_buttons();
        show_card_labels();
        clear_card_labels();
        hypothecation_button->setEnabled(true);
        hypothecation_button->setText("Powrót");
        message->setText("Tryb zastawiania");
        view_number = 3;
    }
    else
    {
        restore_button_states();
        show_card_labels();
        clear_card_labels();
        hypothecation_button->setText("Zastaw nieruchomość");
        message->setText("");
        view_number = 0;
    }
}

void Game::on_buyback_button_clicked()
{
    if(view_number == 0)
    {
        get_button_states();
        hide_interface();
        disable_interface_buttons();
        show_card_labels();
        clear_card_labels();
        buyback_button->setEnabled(true);
        buyback_button->setText("Powrót");
        message->setText("Tryb wykupywania");
        view_number = 4;
    }
    else
    {
        restore_button_states();
        show_card_labels();
        clear_card_labels();
        buyback_button->setText("Wykup nieruchomość");
        message->setText("");
        view_number = 0;
    }
}

void Game::on_end_turn_button_clicked()
{
    if(check_win()) win_procedure();
    else
    {
        prepare_before_next_player();
        find_next_player();
        prepare_next_player();
    }
}

void Game::on_accept_button_clicked()
{
    std::shared_ptr<Field> actual_field = vector_fields[current_player->get_player_position()];
    QString class_type = actual_field->get_classname();
    bool second_button = false;
    bool enough = true;
    if(class_type == "field_property" || class_type == "field_station" || class_type == "field_special")
    {
        if(actual_field->get_buying_cost() > current_player->get_player_money())
        {
            enough = false;
            show_message("Zbyt mało funduszy do wykonania akcji!");
        }
        else
        {
            restore_card_labels();
            if(actual_field->get_popped()) change_one_money_label(-1 * actual_field->get_buying_cost() * 0.55);
            else change_one_money_label(-1 * actual_field->get_buying_cost());
            if(class_type == "field_station") current_player->add_number_of_stations();
            if(class_type == "field_special") current_player->add_number_of_special();
            change_ownership(actual_field, "Player", current_player->get_player_number());
        }
    }
    else if(class_type == "field_chance")
    {
        accept_button->hide();
        deny_button->hide();
        chance_card_index++;
        if(chance_card_index == 16) chance_card_index = 0;
        if(vector_chance_cards[chance_card_index]->get_id_number() == 7 && check_for_prison_card(true))
        {
            chance_card_index++;
            if(chance_card_index == 16) chance_card_index = 0;
        }
        change_action_labels("Szansa", vector_chance_cards[chance_card_index]->get_description());
        if(chance_card_index == 11) fixing_cost = find_fixing_cost(40, 115);
        if(chance_card_index == 14) fixing_cost = find_fixing_cost(25, 100);
        comm_card = true;
        second_button = true;
        ok_button->show();
    }

    if(enough)
    {
        if(dublet) enable_next_throw();
        else disable_next_throw();
        if(second_button) disable_next_turn();
    }
}

void Game::on_ok_button_clicked()
{
    restore_card_labels();

    if(current_player->get_player_bankrupt())
    {
        ok_button->hide();
        on_end_turn_button_clicked();
    }
    else
    {
        if(view_number == 0)
        {
            bool paid = false;
            bool second_button = false;
            bool enough = true;

            if(current_player->get_player_prisoned() && current_player->get_player_prison_turn() != 0)
            {
                if(!current_player->get_player_chance_prison_card() && !current_player->get_player_comm_prison_card())
                {
                    if(50 > current_player->get_player_money()) repeat_action(enough);
                    else change_one_money_label(-50);
                }
                else if(current_player->get_player_chance_prison_card())
                {
                    current_player->set_player_chance_prison_card(false);
                    std::shared_ptr<Field> chance_field = find_prison_card(true);
                    current_player->delete_field(chance_field);
                    for(const auto& player : vector_players)
                    {
                        if(player == current_player) continue;
                        player->delete_all_offers_sent("Szansa");
                    }
                    current_player->delete_all_offers_received("Szansa");
                    change_vector_cards_label();
                }
                else if(current_player->get_player_comm_prison_card())
                {
                    current_player->set_player_comm_prison_card(false);
                    std::shared_ptr<Field> community_chest_field = find_prison_card(false);
                    current_player->delete_field(community_chest_field);
                    for(const auto& player : vector_players)
                    {
                        if(player == current_player) continue;
                        player->delete_all_offers_sent("Kasa społeczna");
                    }
                    current_player->delete_all_offers_received("Kasa społeczna");
                    change_vector_cards_label();
                }
                current_player->set_prisoned(false);
                current_player->reset_prison_turn();
                enable_next_throw();
                paid = true;
            }
            else
            {
                std::shared_ptr<Field> actual_field = vector_fields[current_player->get_player_position()];
                QString class_type = actual_field->get_classname();
                int entrance_cost = find_entrance_cost(actual_field);
                if(class_type == "field_property" || class_type == "field_station" || class_type == "field_special")
                {
                    if(actual_field->get_owner() != "Bank" && actual_field->get_number_owner() != current_player->get_player_number() && !actual_field->get_popped())
                    {
                        if(check_bankruptcy(current_player, entrance_cost)) player_bankrupted(current_player, actual_field->get_number_owner(), entrance_cost);
                        else if(entrance_cost > current_player->get_player_money()) repeat_action(enough);
                        else exchange_money_label(current_player, vector_players[actual_field->get_number_owner()], entrance_cost);
                    }
                }
                else if(class_type == "field_chance")
                {
                    if(current_player->get_player_position() == 7 || current_player->get_player_position() == 22 || current_player->get_player_position() == 36 || comm_card)
                    {
                        comm_card = false;
                        std::shared_ptr<ChanceCard> card = vector_chance_cards[chance_card_index];
                        react_card_chance(card->get_id_number(), second_button, enough);
                    }
                    else
                    {
                        std::shared_ptr<ChanceCard> card = vector_community_chest_cards[community_chest_card_index];
                        react_card_community_chest(card->get_id_number(), second_button, enough);
                    }
                }
                else
                {
                    if(actual_field->get_name() == "Domiar podatkowy" || actual_field->get_name() == "Podatek dochodowy")
                    {
                        if(check_bankruptcy(current_player, entrance_cost)) player_bankrupted(current_player, -1, entrance_cost);
                        else if(entrance_cost > current_player->get_player_money()) repeat_action(enough);
                        else change_one_money_label(-1 * entrance_cost);
                    }
                }
            }

            if(current_player->get_player_bankrupt()) disable_next_turn();
            else if(enough)
                if(!paid)
                {
                    if(current_player->get_player_prisoned() || !dublet) disable_next_throw();
                    else enable_next_throw();
                    if(second_button) disable_next_turn();
                }
        }
        else if(view_number == 1)
        {
            if(vector_fields[number_field_button_clicked]->get_number_of_houses() < 4)
            {
                if(vector_fields[number_field_button_clicked]->get_building_cost() > current_player->get_player_money()) repeat_action();
                else build_house();
            }
            else if(vector_fields[number_field_button_clicked]->get_building_cost() * 5 > current_player->get_player_money()) repeat_action();
            else build_house();
        }
        else if(view_number == 2) sell_house();
        else if(view_number == 3) hyphothecate_property();
        else
        {
            if(vector_fields[number_field_button_clicked]->get_buying_cost() * 0.55 > current_player->get_player_money()) repeat_action();
            else buyback_property();
        }
    }
}

void Game::on_deny_button_clicked()
{
    restore_card_labels();

    bool enough = true;
    std::shared_ptr<Field> actual_field = vector_fields[current_player->get_player_position()];
    QString class_type = actual_field->get_classname();
    if(class_type == "field_chance")
    {
        if(10 > current_player->get_player_money())
        {
            repeat_action(enough);
            ok_button->hide();
            show_two_buttons();
        }
        else change_one_money_label(-10);
    }
    else if(auction) AuctionDialog(this, vector_players, current_player, actual_field).execute();

    if(enough)
    {
        if(dublet) enable_next_throw();
        else disable_next_throw();
    }
}

void Game::on_field_button_clicked()
{
    QPointer<QPushButton> buttonSender = qobject_cast<QPushButton*>(sender());
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
    number_field_button_clicked = number_button_clicked;

    show_card_labels();
    card_label->setText("Karta");
    card_name_label->setText(vector_fields[number_button_clicked]->get_name());
    card_description_label->setText(vector_fields[number_button_clicked]->get_description());

    if(view_number != 0)
    {
        restore_card_labels();

        if(view_number == 1)
        {
            if(vector_fields[number_button_clicked]->get_classname() == "field_property")
                if(vector_fields[number_button_clicked]->get_number_owner() == current_player->get_player_number())
                {
                    std::pair<bool, bool> has_estate = find_estate(vector_fields[number_button_clicked], current_player->get_player_number());
                    if(has_estate.first)
                    {
                        int number_of_houses = vector_fields[number_button_clicked]->get_number_of_houses();
                        int min_number_of_houses = 6;
                        std::vector<std::shared_ptr<Field>> estate = get_estate(vector_fields[number_button_clicked]);
                        for(size_t i = 0; i < estate.size(); i++)
                            if(estate[i]->get_number_of_houses() < min_number_of_houses) min_number_of_houses = estate[i]->get_number_of_houses();
                        if(number_of_houses == min_number_of_houses && number_of_houses < 5)
                        {
                            show_action_labels();
                            change_action_labels("Stawianie budynku", "Czy chcesz postawić budynek na " + vector_fields[number_button_clicked]->get_name() + "?");
                            ok_button->show();
                        }
                    }
                }
        }
        else if(view_number == 2)
        {
            if(vector_fields[number_button_clicked]->get_classname() == "field_property")
                if(vector_fields[number_button_clicked]->get_number_owner() == current_player->get_player_number())
                    if(vector_fields[number_button_clicked]->get_number_of_houses() != 0)
                    {
                        int number_of_houses = vector_fields[number_button_clicked]->get_number_of_houses();
                        int max_number_of_houses = 0;
                        std::vector<std::shared_ptr<Field>> estate = get_estate(vector_fields[number_button_clicked]);
                        for(size_t i = 0; i < estate.size(); i++)
                            if(estate[i]->get_number_of_houses() > max_number_of_houses) max_number_of_houses = estate[i]->get_number_of_houses();
                        if(number_of_houses == max_number_of_houses && number_of_houses > 0)
                        {
                            show_action_labels();
                            change_action_labels("Wyburzanie budynku", "Czy chcesz wyburzyć budynek na " + vector_fields[number_button_clicked]->get_name() + "?");
                            ok_button->show();
                        }
                    }
        }
        else if(view_number == 3)
        {
            if(vector_fields[number_button_clicked]->get_classname() == "field_property" || vector_fields[number_button_clicked]->get_classname() == "field_station" || vector_fields[number_button_clicked]->get_classname() == "field_special")
                if(vector_fields[number_button_clicked]->get_number_owner() == current_player->get_player_number())
                    if(!vector_fields[number_button_clicked]->get_popped())
                    {
                        if(vector_fields[number_button_clicked]->get_classname() == "field_station" || vector_fields[number_button_clicked]->get_classname() == "field_special")
                        {
                            show_action_labels();
                            change_action_labels("Zastawianie posiadłości", "Czy chcesz zastawić posiadłość " + vector_fields[number_button_clicked]->get_name() + "?");
                            ok_button->show();
                        }
                        std::pair<bool, bool> found_estate = find_estate(vector_fields[number_button_clicked], current_player->get_player_number());
                        if(vector_fields[number_button_clicked]->get_classname() == "field_property" && !found_estate.second)
                        {
                            show_action_labels();
                            change_action_labels("Zastawianie posiadłości", "Czy chcesz zastawić posiadłość " + vector_fields[number_button_clicked]->get_name() + "?");
                            ok_button->show();
                        }
                    }
        }
        else
            if(vector_fields[number_button_clicked]->get_classname() == "field_property" || vector_fields[number_button_clicked]->get_classname() == "field_station" || vector_fields[number_button_clicked]->get_classname() == "field_special")
                if(vector_fields[number_button_clicked]->get_number_owner() == current_player->get_player_number())
                    if(vector_fields[number_button_clicked]->get_popped())
                    {
                        show_action_labels();
                        change_action_labels("Wykupienie posiadłości", "Czy chcesz wykupić posiadłość " + vector_fields[number_button_clicked]->get_name() + "?");
                        ok_button->show();
                    }
    }
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
        if(current_player->get_player_prisoned())
        {
            current_player->set_prisoned(false);
            current_player->reset_prison_turn();
            ok_button->hide();
        }
        dublet_turn++;
    } else dublet = false;

    if(current_player->get_player_prisoned() && !dublet)
    {
        disable_next_throw();
        restore_card_labels();
    }
    else
    {
        int actual_position = current_player->get_player_position();
        int calculated_position = (actual_position + dice1->get_number() + dice2->get_number()) % 40;

        if(dublet_turn == 3 || calculated_position == 30)
        {
            prison_player();
            disable_next_turn();
            show_action_labels();
            change_action_labels("Idź do więzienia", "Popełniłeś wykroczenie!"
                                                        "\nIdziesz do więzienia!");
            ok_button->show();
        }
        else
        {
            current_player->set_player_position(calculated_position);
            vector_chequers[current_player->get_player_number()]->setGeometry(chequers_position_table[calculated_position][current_player->get_player_number()]);
            if((actual_position + dice1->get_number() + dice2->get_number()) >= 40) change_money_start_label();
            check_position(vector_fields[calculated_position]);
        }
    }
}

void Game::change_page()
{
    MainWidget* widget = reinterpret_cast<MainWidget*>(parent);
    widget->change_page();
}

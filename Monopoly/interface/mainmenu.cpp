#include "mainmenu.h"

MainMenu::MainMenu(QWidget *parent)
    : QWidget{parent}
{
    layout_menu_base = new QVBoxLayout(this);
    layout_menu_base->setGeometry(QRect(0, 0, 1920, 1080));
    layout_menu_base->setAlignment(Qt::AlignHCenter);

    layout_menu = create_layoutmenu();

    layout_menu_base->addLayout(layout_menu);

    setLayout(layout_menu_base);
}

MainMenu::~MainMenu()
{

}

QLabel *MainMenu::create_label(QString text, QFont font, QString stylesheet)
{
    QLabel* label = new QLabel();
    label->setText(text);
    label->setFont(font);
    label->setStyleSheet(stylesheet);
    return label;
}

QLabel *MainMenu::create_pixmap_label(QColor color)
{
    QLabel* label = new QLabel(this);
    QPixmap p(":/board/board/checker.png");
    p.fill(color);
    label->setPixmap(p);
    return label;
}

QCheckBox *MainMenu::create_checkbox(QString text, QFont font, QString stylesheet)
{
    QCheckBox *checkbox = new QCheckBox();
    checkbox->setText(text);
    checkbox->setFont(font);
    checkbox->setStyleSheet(stylesheet);
    return checkbox;
}

QPushButton *MainMenu::create_pushbutton(QString text, QFont font, QString stylesheet, int width, int height)
{
    QPushButton *pushbutton = new QPushButton();
    pushbutton->setText(text);
    pushbutton->setFont(font);
    pushbutton->setStyleSheet(stylesheet);
    if(width != 0) pushbutton->setFixedWidth(width);
    if(height != 0) pushbutton->setFixedHeight(height);
    return pushbutton;
}

QLineEdit *MainMenu::create_lineedit(QString text, QFont font, QString stylesheet, int width, int height)
{
    QLineEdit *lineedit = new QLineEdit();
    lineedit->setText(text);
    lineedit->setFont(font);
    lineedit->setStyleSheet(stylesheet);
    if(width != 0) lineedit->setFixedWidth(width);
    if(height != 0) lineedit->setFixedHeight(height);
    return lineedit;
}

QHBoxLayout *MainMenu::create_hlayout(bool first_or_second)
{
    QHBoxLayout *hlayout = new QHBoxLayout();

    QLabel* color_label = create_pixmap_label(Qt::black);
    color_label->setFixedSize(30, 30);
    vector_color_labels_players.push_back(color_label);

    QLineEdit *lineedit = create_lineedit("Nazwa", QFont("Open Sans", 16), "color: white; background-color: #484852"); //color: black; background-color: white
    vector_name_players.push_back("Nazwa");
    vector_lineedit_players.push_back(lineedit);

    QPushButton *button_color = create_pushbutton("Zmień kolor", QFont("Open Sans", 16), "color: white; background-color: #484852", 150, 30); //color: black; background-color: white
    vector_color_button_players.push_back(button_color);
    connect(button_color, SIGNAL(clicked()), this, SLOT(on_button_color_clicked()));
    vector_color_players.push_back(Qt::black);

    hlayout->addWidget(color_label);
    hlayout->addWidget(lineedit);
    hlayout->addWidget(button_color);

    if(!first_or_second)
    {
        QPushButton *button_delete = create_pushbutton("Usuń", QFont("Open Sans", 16), "color: white; background-color: #484852", 150, 30); //color: black; background-color: white
        vector_delete_button_players.push_back(button_delete);
        connect(button_delete, SIGNAL(clicked()), this, SLOT(on_button_delete_clicked()));
        hlayout->addWidget(button_delete);
    }
    return hlayout;
}

QVBoxLayout *MainMenu::create_layoutmenu()
{
    layout_menu = new QVBoxLayout(this);
    layout_menu->setAlignment(Qt::AlignTop);

    label_logo = new QLabel(this);
    label_logo->setPixmap(QPixmap(":/images/images/monopoly_logo.png").scaled(1280, 384));
    label_logo->setAlignment(Qt::AlignHCenter);

    label_options = create_label("Opcje rozgrywki:", QFont("Open Sans", 35), "color: white");
    checkbox_auction = create_checkbox("Licytacja", QFont("Open Sans", 30), "color: white"); //20
    connect(checkbox_auction, SIGNAL(stateChanged(int)), this, SLOT(on_checkbox_auction_checked()));

    layout_menu_players = new QHBoxLayout(this);
    label_players = create_label("Liczba graczy: " + QString::number(number_of_players), QFont("Open Sans", 30), "color: white"); //white
    button_add_player = create_pushbutton("+", QFont("Open Sans", 30), "color: white; background-color: #484852", 60, 40); //color: black; background-color: white
    connect(button_add_player, SIGNAL(clicked()), this, SLOT(on_button_add_player_clicked()));
    message = create_label("", QFont("Open Sans", 30), "color: white");
    layout_menu_players->addWidget(label_players);
    layout_menu_players->addWidget(message);
    layout_menu_players->addWidget(button_add_player);

    layout_menu_list_players = new QVBoxLayout(this);
    QHBoxLayout* layout_menu_first_player = create_hlayout(true);
    vector_layout_players.push_back(layout_menu_first_player);
    QHBoxLayout* layout_menu_second_player = create_hlayout(true);
    vector_layout_players.push_back(layout_menu_second_player);
    layout_menu_list_players->addLayout(layout_menu_first_player);
    layout_menu_list_players->addLayout(layout_menu_second_player);

    layout_menu_buttons = new QHBoxLayout(this);
    button_play = create_pushbutton("Zagraj!", QFont("Open Sans", 30), "color: white; background-color: #484852"); //color: black; background-color: white
    connect(button_play, SIGNAL(clicked()), this, SLOT(on_button_play_clicked()));
    connect(button_play, SIGNAL(clicked()), parent(), SLOT(change_page()));
    button_exit = create_pushbutton("Wyjdź :-(", QFont("Open Sans", 30), "color: white; background-color: #484852"); //color: black; background-color: white
    connect(button_exit, SIGNAL(clicked()), this, SLOT(on_button_exit_clicked()));
    connect(button_exit, SIGNAL(clicked()), parent(), SLOT(change_page()));
    layout_menu_buttons->addWidget(button_play);
    layout_menu_buttons->addWidget(button_exit);

    layout_menu->addWidget(label_logo);
    layout_menu->addWidget(label_options);
    layout_menu->addSpacing(30);
    layout_menu->addWidget(checkbox_auction);
    layout_menu->addSpacing(20);
    layout_menu->addLayout(layout_menu_players);
    layout_menu->addSpacing(20);
    layout_menu->addLayout(layout_menu_list_players);
    layout_menu->addSpacing(30);
    layout_menu->addStretch(0);
    layout_menu->addLayout(layout_menu_buttons);

    return layout_menu;
}

void MainMenu::show_message(QString text)
{
    QTimer::singleShot(1, [=](){ message->setText(text); });
    QTimer::singleShot(1000, [=](){ message->setText(""); });
}

void MainMenu::on_button_add_player_clicked()
{
    if(number_of_players < 8)
    {
        number_of_players++;
        label_players->setText("Liczba graczy: " + QString::number(number_of_players));
        QHBoxLayout* hlayout = create_hlayout(false);
        layout_menu_list_players->addLayout(hlayout);
        vector_layout_players.push_back(hlayout);
    }
}

void MainMenu::on_checkbox_auction_checked()
{
    auction = !auction;
}

void MainMenu::on_button_delete_clicked()
{
    number_of_players--;
    label_players->setText("Liczba graczy: " + QString::number(number_of_players));

    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    int number_button_clicked = 0;
    for(int i = 0; i < vector_delete_button_players.size(); i++)
    {
        if(vector_delete_button_players[i] == buttonSender)
        {
            number_button_clicked = i;
            break;
        }
    }
    buttonSender = nullptr;

    vector_name_players.removeAt(number_button_clicked + 2);
    vector_color_players.removeAt(number_button_clicked + 2);

    delete vector_color_labels_players[number_button_clicked + 2];
    vector_color_labels_players[number_button_clicked + 2] = nullptr;
    vector_color_labels_players.removeAt(number_button_clicked + 2);

    delete vector_lineedit_players[number_button_clicked + 2];
    vector_lineedit_players[number_button_clicked + 2] = nullptr;
    vector_lineedit_players.removeAt(number_button_clicked + 2);

    delete vector_color_button_players[number_button_clicked + 2];
    vector_color_button_players[number_button_clicked + 2] = nullptr;
    vector_color_button_players.removeAt(number_button_clicked + 2);

    delete vector_delete_button_players[number_button_clicked];
    vector_delete_button_players[number_button_clicked] = nullptr;    
    vector_delete_button_players.removeAt(number_button_clicked);

    layout_menu_list_players->removeItem(vector_layout_players[number_button_clicked + 2]);
    vector_layout_players[number_button_clicked + 2] = nullptr;
    vector_layout_players.removeAt(number_button_clicked + 2);

}

void MainMenu::on_button_color_clicked()
{
    QColorDialog color_dialog;

    color_dialog.setStyleSheet("QColorDialog { color: white; }");
    //color_dialog.setBackgroundRole(Qt::white);
    QColor color = color_dialog.getColor(Qt::black, this, "Wybierz kolor pionka");

    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    int number_button_clicked = 0;
    for(int i = 0; i < vector_color_button_players.size(); i++)
    {
        if(vector_color_button_players[i] == buttonSender)
        {
            number_button_clicked = i;
            break;
        }
    }
    buttonSender = nullptr;

    vector_color_players[number_button_clicked] = color;
    QPixmap p(":/board/board/checker.png");
    p.fill(color);
    vector_color_labels_players[number_button_clicked]->setPixmap(p);
}

void MainMenu::on_button_play_clicked()
{
    for(int i = 0; i < number_of_players; i++)
        vector_name_players[i] = vector_lineedit_players[i]->text();
    play = true;
    close();
}

void MainMenu::on_button_exit_clicked()
{
    exit = true;
    close();
}

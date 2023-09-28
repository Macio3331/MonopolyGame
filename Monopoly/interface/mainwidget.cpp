#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Monopoly");
    setWindowIcon(QIcon(":images/images/icon.ico"));
    setStyleSheet("background-color: #323236");
    setMinimumSize(1920, 1000);

    pages = new QStackedWidget(this);
    main_menu = new MainMenu(this);

    pages->addWidget(main_menu);

    pages->setCurrentIndex(0);

    main_layout = new QVBoxLayout(this);
    main_layout->addWidget(pages);
    setLayout(main_layout);
}

MainWidget::~MainWidget()
{
    /*
    delete pages;
    delete main_menu;
    delete main_layout;
    delete game_menu;
    pages = nullptr;
    main_menu = nullptr;
    main_layout = nullptr;
    game_menu = nullptr;
    */
}

void MainWidget::change_page()
{
    vector_name_players = main_menu->get_vector_name_players();
    number_of_players = main_menu->get_number_of_players();
    auction = main_menu->get_auction();
    exit = main_menu->get_exit();
    play = main_menu->get_play();
    if(!first)
    {
        pages->removeWidget(game_menu);
        delete game_menu;
        first = false;
    }
    game_menu = new Game(this, vector_name_players, number_of_players, auction);
    pages->addWidget(game_menu);

    if(exit) close();
    if(pages->currentIndex() == 0) pages->setCurrentIndex(1);
    else pages->setCurrentIndex(0);
}



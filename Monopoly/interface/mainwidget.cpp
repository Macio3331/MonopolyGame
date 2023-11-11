#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Monopoly");
    setWindowIcon(QIcon(":images/images/icon.ico"));
    setStyleSheet("color: white; background-color: #323236");
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

}

void MainWidget::change_page()
{
    if(pages->currentIndex() == 0)
    {
        if(!first)
        {
            pages->removeWidget(game_menu);
            delete game_menu;
        }
        first = false;
        game_menu = new Game(this, main_menu->get_vector_name_players(), main_menu->get_vector_color_players(), main_menu->get_number_of_players(), main_menu->get_auction());
        pages->addWidget(game_menu);

        if(main_menu->get_exit()) close();
        pages->setCurrentIndex(1);
    }
    else pages->setCurrentIndex(0);
}



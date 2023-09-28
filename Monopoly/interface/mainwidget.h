#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include "interface/mainmenu.h"
#include "interface/game.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    /*
    void add_player(int number) { number_of_players += number; }
    void add_player_name(QString name) { vector_name_players.push_back(name); }
    void change_auction() { auction = !auction; }
    void change_exit() { exit = !exit; }
    void change_play() { play = !play; }
    int get_number_of_players() { return number_of_players; }
    QVector<QString> get_vector_name_players() { return vector_name_players; }
    bool get_auction() { return auction; }
    bool get_exit() { return exit; }
    bool get_play() { return play; }
    */
private:
    QStackedWidget *pages;
    MainMenu *main_menu;
    Game *game_menu;
    QVBoxLayout *main_layout;

    QVector<QString> vector_name_players;
    int number_of_players = 2;
    bool auction = false;
    bool exit = false;
    bool play = false;
    bool first = true;
public slots:
    void change_page();
};

#endif // MAINWIDGET_H

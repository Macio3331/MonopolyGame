#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QString>
#include <QColor>
#include "interface/mainmenu.h"
#include "interface/game.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
private:
    QStackedWidget *pages;
    MainMenu *main_menu;
    Game *game_menu;
    QVBoxLayout *main_layout;
    bool first = true;
public slots:
    void change_page();
};

#endif // MAINWIDGET_H

#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QVector>
#include <QLineEdit>
#include <QTimer>
#include <QColor>
#include <QColorDialog>

class MainMenu : public QWidget
{
    Q_OBJECT
public:
    explicit MainMenu(QWidget *parent);
    ~MainMenu();
    int get_number_of_players() { return number_of_players; }
    QVector<QString> get_vector_name_players() { return vector_name_players; }
    QVector<QColor> get_vector_color_players() { return vector_color_players; }
    bool get_auction() { return auction; }
    bool get_exit() { return exit; }
    bool get_play() { return play; }
private:
    QVector<QHBoxLayout*> vector_layout_players;
    QVector<QLineEdit*> vector_lineedit_players;
    QVector<QPushButton*> vector_delete_button_players;
    QVector<QPushButton*> vector_color_button_players;
    QVector<QLabel*> vector_color_labels_players;
    //
    QVector<QString> vector_name_players;
    QVector<QColor> vector_color_players;
    int number_of_players = 2;
    bool auction = false;
    bool exit = false;
    bool play = false;
    //
    QVBoxLayout *layout_menu_base;
    QVBoxLayout *layout_menu;
    QVBoxLayout *layout_menu_list_players;
    QHBoxLayout *layout_menu_players;
    QHBoxLayout *layout_menu_buttons;
    QLabel *label_logo;
    QLabel *label_options;
    QCheckBox *checkbox_auction;
    QLabel *label_players;
    QLabel *message;
    QPushButton *button_add_player;
    QPushButton *button_play;
    QPushButton *button_exit;

    QLabel* create_label(QString text, QFont font, QString stylesheet);
    QLabel* create_pixmap_label(QColor color);
    QCheckBox* create_checkbox(QString text, QFont font, QString stylesheet);
    QPushButton* create_pushbutton(QString text, QFont font, QString stylesheet, int width = 0, int height = 0);
    QLineEdit* create_lineedit(QString text, QFont font, QString stylesheet, int width = 0, int height = 0);
    QHBoxLayout* create_hlayout(bool first_or_second);
    QVBoxLayout* create_layoutmenu();
    void show_message(QString text);

private slots:
    void on_button_add_player_clicked();
    void on_checkbox_auction_checked();
    void on_button_delete_clicked();
    void on_button_color_clicked();
    void on_button_play_clicked();
    void on_button_exit_clicked();
};

#endif // MAINMENU_H

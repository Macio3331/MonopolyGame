#include "windialog.h"

WinDialog::WinDialog(QWidget* parent, std::vector<std::shared_ptr<Player>> vector_players) : parent(parent), vector_players(vector_players)
{

}

void WinDialog::execute()
{
    int not_bankrupt = 0;
    for(size_t i = 0; i < vector_players.size(); i++)
        if(!vector_players[i]->get_player_bankrupt())
        {
            not_bankrupt = i;
            break;
        }

    QDialog win_dialog;
    win_dialog.setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    win_dialog.setStyleSheet("background-color: #323236");
    QPointer<QVBoxLayout> layout = new QVBoxLayout(this);
    QPointer<QLabel> win_label = create_label("Koniec gry!\nZwyciężył gracz " + QString::number(not_bankrupt + 1) + "  " + vector_players[not_bankrupt]->get_player_name() + "!", QFont("Open Sans", 20), "color: white");
    win_label->setAlignment(Qt::AlignCenter);
    QDialogButtonBox win_button(QDialogButtonBox::Ok);
    win_button.setStyleSheet("color: white; background-color: #484852");
    win_button.setCenterButtons(true);
    connect(&win_button, &QDialogButtonBox::accepted, &win_dialog, &QDialog::accept);
    layout->addWidget(win_label);
    layout->addWidget(&win_button);
    win_dialog.setLayout(layout);
    do win_dialog.exec();
    while(!win_dialog.result());
    win_dialog.close();
}

QPointer<QLabel> WinDialog::create_label(QString text, QFont font, QString stylesheet, QRect geometry, Qt::Alignment align)
{
    QPointer<QLabel> label = new QLabel(this);
    label->setText(text);
    label->setFont(font);
    label->setStyleSheet(stylesheet);
    label->setGeometry(geometry);
    label->setAlignment(align);
    return label;
}

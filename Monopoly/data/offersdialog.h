#ifndef OFFERSDIALOG_H
#define OFFERSDIALOG_H

#include <QDialog>
#include <QObject>
#include <QPointer>
#include <QListWidget>
#include <QLabel>
#include <QVBoxLayout>

#include <memory>
#include "data/player.h"

class OffersDialog : public QDialog
{
    QWidget* parent;
    std::vector<std::shared_ptr<Player>> vector_players;
    std::shared_ptr<Player> current_player;
    QPointer<QListWidget> players_offers;
public:
    OffersDialog(QWidget* parent, std::vector<std::shared_ptr<Player>> vector_players, std::shared_ptr<Player> current_player, QPointer<QListWidget> players_offers);
    void execute();
    QPointer<QLabel> create_label(QString text, QFont font, QString stylesheet, QRect geometry = QRect(0, 0, 0, 0), Qt::Alignment align = Qt::AlignLeft);
    QPointer<QPushButton> create_pushbutton(QString text, QFont font, QString stylesheet, QRect geometry = QRect(0, 0, 0, 0), bool enabled = true);
private slots:
    void on_yes_button_clicked();
    void on_no_button_clicked();
};

#endif // OFFERSDIALOG_H

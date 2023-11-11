#ifndef AUCTIONDIALOG_H
#define AUCTIONDIALOG_H

#include <QDialog>
#include <QObject>
#include <QPointer>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <memory>
#include "data/player.h"

class AuctionDialog : public QDialog
{
    QWidget* parent;
    std::vector<std::shared_ptr<Player>> vector_players;
    std::shared_ptr<Player> current_player;
    std::shared_ptr<Field> actual_field;
public:
    AuctionDialog(QWidget* parent, std::vector<std::shared_ptr<Player>> vector_players, std::shared_ptr<Player> current_player, std::shared_ptr<Field> actual_field);
    void execute();
    int number_auction_players(std::vector<bool> vector_bool_players);
    QPointer<QLabel> create_label(QString text, QFont font, QString stylesheet, QRect geometry = QRect(0, 0, 0, 0), Qt::Alignment align = Qt::AlignCenter);
};

#endif // AUCTIONDIALOG_H

#ifndef EXCHANGEDIALOG_H
#define EXCHANGEDIALOG_H

#include <QDialog>
#include <QObject>
#include <QPointer>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QListWidget>
#include <QSpinBox>
#include <memory>
#include "data/player.h"

class ExchangeDialog : public QDialog
{
    QWidget* parent;
    std::vector<std::shared_ptr<Player>> vector_players;
    std::shared_ptr<Player> current_player;
    QPointer<QListWidget> players_offers;
    QPointer<QComboBox> exchange_combo;
    QPointer<QLabel> chosen_player_label;
    QPointer<QListWidget> other_players_list;
    QPointer<QSpinBox> other_players_spin;
    int exchange_chosen_player = -1;
public:
    ExchangeDialog(QWidget* parent, std::vector<std::shared_ptr<Player>> vector_players, std::shared_ptr<Player> current_player, QPointer<QListWidget> players_offers);
    void execute();
    QPointer<QLabel> create_label(QString text, QFont font, QString stylesheet, QRect geometry = QRect(0, 0, 0, 0), Qt::Alignment align = Qt::AlignCenter);
private slots:
    void chosen_player_changed();
};

#endif // EXCHANGEDIALOG_H

#ifndef WINDIALOG_H
#define WINDIALOG_H

#include <QDialog>
#include <QObject>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPointer>
#include <memory>
#include "data/player.h"

class WinDialog : public QDialog
{
    QWidget* parent;
    std::vector<std::shared_ptr<Player>> vector_players;
public:
    WinDialog(QWidget* parent, std::vector<std::shared_ptr<Player>> vector_players);
    void execute();
    QPointer<QLabel> create_label(QString text, QFont font, QString stylesheet, QRect geometry = QRect(0, 0, 0, 0), Qt::Alignment align = Qt::AlignCenter);
};

#endif // WINDIALOG_H

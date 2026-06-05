#ifndef MINIGAMESWIDGET_H
#define MINIGAMESWIDGET_H

#include "progressservice.h"

#include <QWidget>

class QFrame;
class QGridLayout;

class MiniGamesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MiniGamesWidget(QWidget *parent = nullptr);

    void setProgress(const ProgressData &progress);

signals:
    void backRequested();
    void mazeAdventureRequested();

private:
    void rebuildCards();
    QFrame *createMazeCard();
    QFrame *createLockedCard(const QString &title, const QString &description, const QString &condition);

    ProgressData m_progress;
    QGridLayout *m_cardsLayout = nullptr;
};

#endif

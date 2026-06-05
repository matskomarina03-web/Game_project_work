#ifndef ROUND_H
#define ROUND_H

#include <QStringList>
#include <QVector>


struct Round {
    int number;
    int size;
    QString difficulty;
    QString paintColor;

    bool unlocked;
    bool completed;
    int bestSeconds;
    int bestStars;
    int coinsEarned;

    QVector<QVector<int>> solution;
};


QVector<Round> createRounds();
QStringList rowHints(const Round &round, int row);
QStringList columnHints(const Round &round, int column);

#endif

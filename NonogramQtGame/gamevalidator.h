#ifndef GAMEVALIDATOR_H
#define GAMEVALIDATOR_H

#include <QVector>

enum CellState {
    Empty,
    Filled
};

class GameValidator
{
public:
    static bool isCorrect(
        const QVector<QVector<int>>& solution,
        const QVector<QVector<CellState>>& board
        );
};

#endif // GAMEVALIDATOR_H

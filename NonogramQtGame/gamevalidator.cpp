#include "gamevalidator.h"

bool GameValidator::isCorrect(
    const QVector<QVector<int>>& solution,
    const QVector<QVector<CellState>>& board)
{
    for (int i = 0; i < solution.size(); i++) {
        for (int j = 0; j < solution[i].size(); j++) {
            bool expected = solution[i][j] == 1;
            bool actual = (board[i][j] == Filled);
            if (expected != actual) return false;
        }
    }
    return true;
}

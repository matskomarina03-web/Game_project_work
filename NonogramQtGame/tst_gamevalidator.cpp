#include "tst_gamevalidator.h"

void TestGameValidator::correctSolution_returnsTrue()
{
    QVector<QVector<int>> solution = {{1,0},{0,1}};
    QVector<QVector<CellState>> board = {{Filled,Empty},{Empty,Filled}};

    QVERIFY(GameValidator::isCorrect(solution, board));
}

void TestGameValidator::wrongSolution_returnsFalse()
{
    QVector<QVector<int>> solution = {{1,1},{0,0}};
    QVector<QVector<CellState>> board = {{Filled,Empty},{Empty,Filled}};

    QVERIFY(!GameValidator::isCorrect(solution, board));
}

void TestGameValidator::emptyBoard_returnsFalse()
{
    QVector<QVector<int>> solution = {{1,0},{0,1}};
    QVector<QVector<CellState>> board = {{Empty,Empty},{Empty,Empty}};

    QVERIFY(!GameValidator::isCorrect(solution, board));
}

void TestGameValidator::allFilledWrong_returnsFalse()
{
    QVector<QVector<int>> solution = {{1,0},{0,1}};
    QVector<QVector<CellState>> board = {{Filled,Filled},{Filled,Filled}};

    QVERIFY(!GameValidator::isCorrect(solution, board));
}

void TestGameValidator::emptySolution_edgeCase()
{
    QVector<QVector<int>> solution;
    QVector<QVector<CellState>> board;

    QVERIFY(GameValidator::isCorrect(solution, board));
}

QTEST_MAIN(TestGameValidator)
#include "tst_gamevalidator.moc"

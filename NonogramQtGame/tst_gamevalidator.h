#ifndef TST_GAMEVALIDATOR_H
#define TST_GAMEVALIDATOR_H

#include <QObject>
#include <QtTest/QtTest>
#include "gamevalidator.h"

class TestGameValidator : public QObject
{
    Q_OBJECT

private slots:
    void correctSolution_returnsTrue();
    void wrongSolution_returnsFalse();
    void emptyBoard_returnsFalse();
    void allFilledWrong_returnsFalse();
    void emptySolution_edgeCase();
};

#endif

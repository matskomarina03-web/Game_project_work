#include "round.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

namespace {
QVector<QVector<int>> readPattern(const QJsonArray &rows)
{
    QVector<QVector<int>> result;
    for (const auto &row : rows) {
        QVector<int> values;
        for (const auto &value : row.toArray()) {
            values.append(value.toInt());
        }
        result.append(values);
    }
    return result;
}

bool isValidPattern(const Round &round)
{
    if (round.size <= 0 || round.solution.size() != round.size) {
        return false;
    }

    for (const auto &row : round.solution) {
        if (row.size() != round.size) {
            return false;
        }
    }

    return true;
}

QString roundsFilePath()
{
    const QString workingPath = QDir::current().filePath("rounds.json");
    if (QFile::exists(workingPath) && QFile::exists(QDir::current().filePath("CMakeLists.txt"))) {
        return workingPath;
    }

    QDir directory(QCoreApplication::applicationDirPath());
    while (!directory.isRoot()) {
        const QString projectPath = directory.filePath("rounds.json");
        if (QFile::exists(projectPath) && QFile::exists(directory.filePath("CMakeLists.txt"))) {
            return projectPath;
        }
        directory.cdUp();
    }

    const QString appPath = QDir(QCoreApplication::applicationDirPath()).filePath("rounds.json");
    if (QFile::exists(appPath)) {
        return appPath;
    }

    return appPath;
}
}

QVector<Round> createRounds()
{
    QFile file(roundsFilePath());
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open rounds file:" << file.fileName();
        return {};
    }

    QJsonParseError error;
    const QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &error);
    if (error.error != QJsonParseError::NoError || !document.isArray()) {
        qWarning() << "Invalid rounds JSON:" << error.errorString();
        return {};
    }

    QVector<Round> rounds;
    const QJsonArray array = document.array();
    for (const auto &value : array) {
        const QJsonObject object = value.toObject();
        Round round;
        round.number = object.value("number").toInt(rounds.size() + 1);
        round.size = object.value("size").toInt();
        round.solution = readPattern(object.value("solution").toArray());
        round.paintColor = object.value("paintColor").toString("#111827");
        round.difficulty = object.value("difficulty").toString(round.size <= 5 ? "Easy" : "Hard");
        round.unlocked = object.value("unlocked").toBool(rounds.isEmpty());
        round.completed = false;
        round.bestSeconds = 0;
        round.bestStars = 0;
        round.coinsEarned = 0;

        if (isValidPattern(round)) {
            rounds.append(round);
        } else {
            qWarning() << "Skipped invalid round" << round.number;
        }
    }

    if (!rounds.isEmpty()) {
        rounds[0].unlocked = true;
    }
    return rounds;
}

QStringList rowHints(const Round &round, int row)
{
    QStringList hints;
    int run = 0;
    for (int value : round.solution[row]) {
        if (value == 1) {
            ++run;
        } else if (run > 0) {
            hints.append(QString::number(run));
            run = 0;
        }
    }
    if (run > 0) {
        hints.append(QString::number(run));
    }
    if (hints.isEmpty()) {
        hints.append("0");
    }
    return hints;
}

QStringList columnHints(const Round &round, int column)
{
    QStringList hints;
    int run = 0;
    for (int row = 0; row < round.size; ++row) {
        if (round.solution[row][column] == 1) {
            ++run;
        } else if (run > 0) {
            hints.append(QString::number(run));
            run = 0;
        }
    }
    if (run > 0) {
        hints.append(QString::number(run));
    }
    if (hints.isEmpty()) {
        hints.append("0");
    }
    return hints;
}

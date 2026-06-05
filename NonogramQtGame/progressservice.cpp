#include "progressservice.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QtGlobal>

ProgressService::ProgressService(int mazeLevelCount)
    : m_mazeLevelCount(qMax(1, mazeLevelCount))
{
}

ProgressData ProgressService::load() const
{
    QFile file(progressFilePath());
    if (!file.open(QIODevice::ReadOnly)) {
        return recalculate(ProgressData());
    }

    const QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    const QJsonObject object = document.object();

    ProgressData data;
    data.completedNonogramLevels = object.value("completedNonogramLevels").toInt(0);
    data.unlockedMazeLevel = object.value("unlockedMazeLevel").toInt(1);
    data.totalProgress = object.value("totalProgress").toInt(0);

    const QJsonArray completedMazeLevels = object.value("completedMazeLevels").toArray();
    for (const QJsonValue &value : completedMazeLevels) {
        data.completedMazeLevels.append(value.toInt());
    }

    return recalculate(data);
}

void ProgressService::save(const ProgressData &data) const
{
    const ProgressData normalized = recalculate(data);
    QJsonArray completedMazeLevels;
    const QVector<int> levels = normalized.completedMazeLevels;
    for (int level : levels) {
        if (level >= 1 && level <= m_mazeLevelCount) {
            completedMazeLevels.append(level);
        }
    }

    QJsonObject object;
    object.insert("completedNonogramLevels", normalized.completedNonogramLevels);
    object.insert("unlockedMazeLevel", normalized.unlockedMazeLevel);
    object.insert("completedMazeLevels", completedMazeLevels);
    object.insert("totalProgress", normalized.totalProgress);

    QFile file(progressFilePath());
    QDir().mkpath(QFileInfo(file).absolutePath());
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.write(QJsonDocument(object).toJson(QJsonDocument::Indented));
    }
}

ProgressData ProgressService::syncNonogramProgress(int completedNonogramLevels) const
{
    ProgressData data = load();
    data.completedNonogramLevels = qMax(0, completedNonogramLevels);
    data = recalculate(data);
    save(data);
    return data;
}

ProgressData ProgressService::markMazeLevelCompleted(int level) const
{
    ProgressData data = load();
    if (level >= 1 && level <= data.unlockedMazeLevel) {
        data.completedMazeLevels.push_back(level);
    }
    data = recalculate(data);
    save(data);
    return data;
}

int ProgressService::unlockedMazeLevel(int completedNonogramLevels) const
{
    return qMax(1, completedNonogramLevels / 3 + 1);
}

int ProgressService::missingNonogramLevelsForMazeLevel(int level, int completedNonogramLevels) const
{
    if (level <= unlockedMazeLevel(completedNonogramLevels)) {
        return 0;
    }
    const int required = (level - 1) * 3;
    return qMax(0, required - completedNonogramLevels);
}

ProgressData ProgressService::recalculate(ProgressData data) const
{
    data.completedNonogramLevels = qMax(0, data.completedNonogramLevels);
    data.unlockedMazeLevel = unlockedMazeLevel(data.completedNonogramLevels);

    QSet<int> normalizedCompleted;
    for (int level : std::as_const(data.completedMazeLevels)) {
        if (level >= 1 && level <= m_mazeLevelCount) {
            normalizedCompleted.insert(level);
        }
    }
    data.completedMazeLevels = QVector<int>(normalizedCompleted.begin(), normalizedCompleted.end());

    const int completedUnits = data.completedNonogramLevels + data.completedMazeLevels.size();
    const int totalUnits = data.completedNonogramLevels + m_mazeLevelCount;
    data.totalProgress = totalUnits == 0 ? 0 : qBound(0, completedUnits * 100 / totalUnits, 100);
    return data;
}

QString ProgressService::progressFilePath() const
{
    QString directory = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (directory.isEmpty()) {
        directory = QDir::currentPath();
    }
    return QDir(directory).filePath("progress.json");
}

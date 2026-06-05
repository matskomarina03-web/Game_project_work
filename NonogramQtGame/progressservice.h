#ifndef PROGRESSSERVICE_H
#define PROGRESSSERVICE_H

#include <QSet>
#include <QString>

struct ProgressData {
    int completedNonogramLevels = 0;
    int unlockedMazeLevel = 1;
    QVector<int> completedMazeLevels;
    int totalProgress = 0;
};

class ProgressService
{
public:
    explicit ProgressService(int mazeLevelCount = 50);

    ProgressData load() const;
    void save(const ProgressData &data) const;
    ProgressData syncNonogramProgress(int completedNonogramLevels) const;
    ProgressData markMazeLevelCompleted(int level) const;

    int unlockedMazeLevel(int completedNonogramLevels) const;
    int missingNonogramLevelsForMazeLevel(int level, int completedNonogramLevels) const;

private:
    ProgressData recalculate(ProgressData data) const;
    QString progressFilePath() const;

    int m_mazeLevelCount;
};

#endif

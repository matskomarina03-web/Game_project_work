#ifndef MAZEADVENTUREWIDGET_H
#define MAZEADVENTUREWIDGET_H

#include "progressservice.h"

#include <QFrame>
#include <QPoint>
#include <QStackedWidget>
#include <QTimer>
#include <QWidget>

class QLabel;
class QPushButton;
class QGridLayout;

struct MazeLevel {
    int number = 1;
    QString title;
    QVector<QString> rows;
};

class MazePreviewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MazePreviewWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

class MazeSchemePreviewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MazeSchemePreviewWidget(const MazeLevel &level, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    MazeLevel m_level;
};

class MazeGameView : public QWidget
{
    Q_OBJECT

public:
    explicit MazeGameView(QWidget *parent = nullptr);

    void setLevel(const MazeLevel &level);

signals:
    void levelCompleted();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void tryMove(const QPoint &delta);
    void animateTo(const QPoint &nextCell);
    QRectF cellRect(const QPoint &cell) const;
    QPoint findCell(QChar marker) const;
    bool isWall(const QPoint &cell) const;
    void drawMiniMap(QPainter &p);

    MazeLevel m_level;
    QPoint m_playerCell;
    QPointF m_playerOffset;
    QTimer m_animationTimer;
    QPointF m_animationStart;
    QPointF m_animationEnd;
    int m_animationStep = 0;
};

class MazeAdventureWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MazeAdventureWidget(QWidget *parent = nullptr);

    void setProgress(const ProgressData &progress);

signals:
    void backRequested();
    void progressChanged(const ProgressData &progress);

private:
    void buildLevelSelection();
    void startLevel(int level);
    void finishCurrentLevel();
    void showLevelSelection();
    QPushButton *createBackButton(QWidget *parent) const;
    QFrame *createLevelCard(int level, bool unlocked);
    QVector<MazeLevel> loadMazeLevels() const;

    QVector<MazeLevel> m_levels;
    ProgressService m_progressService;
    ProgressData m_progress;
    QStackedWidget *m_stack = nullptr;
    QWidget *m_levelPage = nullptr;
    QWidget *m_gamePage = nullptr;
    QGridLayout *m_levelGrid = nullptr;
    QLabel *m_titleLabel = nullptr;
    QLabel *m_statusLabel = nullptr;
    MazeGameView *m_gameView = nullptr;
    int m_currentLevel = 1;

};

#endif

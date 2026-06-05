#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "round.h"
#include "progressservice.h"

#include <QElapsedTimer>
#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <QVector>
#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startSelectedRound();
    void openRound(int index);
    void verifyBoard();
    void useHint();
    void checkBoard();
    void resetBoard();
    void showMenu();
    void showLevelBrowser();
    void showCompletedPictures();
    void showMiniGames();
    void showMazeAdventure();
    void repeatRound();
    void nextRound();
    void updateTimerLabel();



private:
    enum CellState {
        Unknown = 0,
        Filled = 1,
        Marked = 2
    };

    void setupConnections();
    void addMenuActions();
    void buildRoundButtons();
    void refreshMenu();
    void loadRound(int index);
    QWidget *createPicturePreview(const Round &round, int cellSize, QWidget *parent) const;
    void renderBoard();
    void cycleCell(int row, int column);
    void applyCellStyle(QPushButton *button, CellState state);
    void restoreBoardStyles();
    void revealSolvedPicture();
    void setGameControlsEnabled(bool enabled);
    void finishRound(bool won);
    void unlockNextRound();
    int starsForResult() const;
    int coinsForResult(int stars) const;
    QString starText(int stars) const;
    void updateCoinLabels();
    void completeDailyMission();
    int completedNonogramLevels() const;
    void syncBonusProgress();
    void loadProgress();
    void saveProgress() const;
    QString formatTime(int seconds) const;


    QScrollArea *roundScrollArea = nullptr;
    QWidget *roundContainer = nullptr;
    QGridLayout *roundGrid = nullptr;

    QScrollArea *gameScroll = nullptr;
    QWidget *gameContainer = nullptr;
    QGridLayout *boardLayout = nullptr;

    Ui::MainWindow *ui;
    class MiniGamesWidget *miniGamesWidget;
    class MazeAdventureWidget *mazeAdventureWidget;
    ProgressService progressService;
    ProgressData bonusProgress;
    QVector<Round> rounds;
    QVector<QVector<CellState>> board;
    QVector<QPushButton *> roundButtons;
    QVector<QVector<QPushButton *>> cellButtons;
    QVector<Round> loadRoundsFromFile();

    QTimer timer;
    QElapsedTimer elapsedTimer;
    int selectedRound;
    int currentRound;
    int lives;
    int errors;
    int hintsUsed;
    int coins;
    int elapsedSeconds;
    bool dailyMissionDone;


};

#endif

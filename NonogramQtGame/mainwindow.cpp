#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mazeadventurewidget.h"
#include "minigameswidget.h"

#include <QApplication>
#include <QDate>
#include <QDialog>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayoutItem>
#include <QScrollArea>
#include <QSettings>
#include <QSizePolicy>
#include <QVBoxLayout>

namespace {
const int MaxLives = 3;
const int HintCost = 10;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      miniGamesWidget(nullptr),
      mazeAdventureWidget(nullptr),
      progressService(50),
      rounds(createRounds()),
      selectedRound(0),
      currentRound(0),
      lives(MaxLives),
      errors(0),
      hintsUsed(0),
      coins(30),
      elapsedSeconds(0),
      dailyMissionDone(false)
{


    ui->setupUi(this);
    setStyleSheet(R"(


QMainWindow {
    background: #000000;
}

QLabel {
    color: #ffffff;
    font-size: 14px;
}

QPushButton {
    background: #111c33;
    color: #6e737a;
    border: 1px solid #22304a;
    border-radius: 12px;
    padding: 10px 14px;
    font-size: 14px;
    font-weight: 600;
}

QPushButton:hover {
    background: #1b2a4a;
    border-color: #7c3aed;
}

QPushButton:pressed {
    background: #7c3aed;
    color: white;
}

QPushButton:disabled {
    background: #0b1220;
    color: #64748b;
    border-color: #1e293b;
}
)");


    loadProgress();
    syncBonusProgress();
    addMenuActions();
    buildRoundButtons();
    setupConnections();
    updateCoinLabels();
    refreshMenu();
    ui->stackedWidget->setCurrentWidget(ui->menuPage);
    ui->nextButton->setStyleSheet("background-color: #7c3aed;color: white;border-radius: 10px;");
    ui->repeatButton->setStyleSheet("background-color: #7c3aed;color: white;border-radius: 10px;");
    ui->menuButton->setStyleSheet("background-color: #7c3aed;color: white;border-radius: 10px;");

    roundScrollArea->setStyleSheet(R"(
QScrollArea {
    background: transparent;
    border: none;
}

QScrollBar:vertical {
    width: 10px;
    background: transparent;
}

QScrollBar::handle:vertical {
    background: #64748b;
    border-radius: 5px;
}
)");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections()
{
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startSelectedRound);
    connect(ui->exitMenuButton, &QPushButton::clicked, qApp, &QApplication::quit);
    connect(ui->hintButton, &QPushButton::clicked, this, &MainWindow::useHint);
    connect(ui->checkButton, &QPushButton::clicked, this, &MainWindow::checkBoard);
    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::resetBoard);
    connect(ui->exitGameButton, &QPushButton::clicked, this, &MainWindow::showMenu);
    connect(ui->nextButton, &QPushButton::clicked, this, &MainWindow::nextRound);
    connect(ui->repeatButton, &QPushButton::clicked, this, &MainWindow::repeatRound);
    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::showMenu);
    connect(&timer, &QTimer::timeout, this, &MainWindow::updateTimerLabel);
}

void MainWindow::addMenuActions()
{
    auto *actionsLayout = new QHBoxLayout();
    actionsLayout->setSpacing(12);

    auto *levelsButton = new QPushButton("Рівні за складністю", ui->menuPage);
    levelsButton->setMinimumSize(190, 48);
    levelsButton->setCursor(Qt::PointingHandCursor);
    connect(levelsButton, &QPushButton::clicked, this, &MainWindow::showLevelBrowser);

    auto *picturesButton = new QPushButton("Готові фотографії", ui->menuPage);
    picturesButton->setMinimumSize(190, 48);
    picturesButton->setCursor(Qt::PointingHandCursor);
    connect(picturesButton, &QPushButton::clicked, this, &MainWindow::showCompletedPictures);

    auto *miniGamesButton = new QPushButton("Міні гра", ui->menuPage);
    miniGamesButton->setMinimumSize(190, 48);
    miniGamesButton->setCursor(Qt::PointingHandCursor);
    connect(miniGamesButton, &QPushButton::clicked, this, &MainWindow::showMiniGames);

    actionsLayout->addStretch();
    actionsLayout->addWidget(levelsButton);
    actionsLayout->addWidget(picturesButton);
    actionsLayout->addWidget(miniGamesButton);
    actionsLayout->addStretch();
    ui->menuLayout->insertLayout(4, actionsLayout);
}

void MainWindow::buildRoundButtons()
{
    roundContainer = new QWidget();
    roundGrid = new QGridLayout(roundContainer);

    roundGrid->setSpacing(12);
    roundGrid->setContentsMargins(10, 10, 10, 10);

    for (int i = 0; i < rounds.size(); ++i) {

        auto *button = new QPushButton(roundContainer);
        button->setMinimumSize(116, 82);
        button->setCursor(Qt::PointingHandCursor);

        roundButtons.append(button);

        int row = i / 5;
        int col = i % 5;
        roundGrid->addWidget(button, row, col);

        connect(button, &QPushButton::clicked, this, [this, i]() {
            openRound(i);
        });
    }

    roundScrollArea = new QScrollArea(ui->menuPage);
    roundScrollArea->setWidgetResizable(true);
    roundScrollArea->setFrameShape(QFrame::NoFrame);

    roundScrollArea->setWidget(roundContainer);

    ui->menuLayout->insertWidget(3, roundScrollArea);
}
void MainWindow::refreshMenu()
{
    int completed = 0;
    for (const Round &round : rounds) {
        if (round.completed) {
            ++completed;
        }
    }
    ui->progressLabel->setText(QString("Вибір раунду | Пройдено: %1/%2").arg(completed).arg(rounds.size()));
    ui->missionLabel->setText(dailyMissionDone
                                  ? "Місія дня виконана: +20 🪙"
                                  : "Місія дня: пройди 1 раунд і отримай +20 🪙");

    for (int i = 0; i < roundButtons.size(); ++i) {
        QPushButton *button = roundButtons[i];
        const Round &round = rounds[i];
        QString text = QString("Раунд %1").arg(round.number);
        if (!round.unlocked) {
            text += "\n🔒 Закрито";
        } else if (round.completed) {
            text += QString("\n%1  +%2 🪙").arg(starText(round.bestStars)).arg(round.coinsEarned);
        } else if (i == selectedRound) {
            text += "\n▶ Обрано";
        } else {
            text += "\n🔓 Відкрито";
        }

        button->setText(text);
        button->setEnabled(round.unlocked);

        if (!round.unlocked) {
            button->setStyleSheet(
                "font-size: 14px; font-weight: 800;"
                "color: white;"
                "background: #1e293b;"
                "border: 2px solid #334155;"
                "border-radius: 14px;"
                );
        } else if (i == selectedRound) {
            button->setStyleSheet("font-size: 14px; font-weight: 800; color: white; background: #7c3aed;border: #a78bfa; border-radius: 14px;");
        } else if (round.completed) {
            button->setStyleSheet("font-size: 14px; font-weight: 800; background: #0f172a;border: #475569;color: #f8fafc; border-radius: 14px;");
        } else {
            button->setStyleSheet(
                "font-size: 14px;"
                "color: #475569;"
                "background: #0a0f1a;"
                "border: 1px solid #1f2937;"
                "border-radius: 14px;"
                );
        }
    }
}

void MainWindow::openRound(int index)
{
    if (index < 0 || index >= rounds.size()) {
        return;
    }
    if (!rounds[index].unlocked) {
        return;
    }
    selectedRound = index;
    refreshMenu();
}

void MainWindow::startSelectedRound()
{
    if (rounds.isEmpty()) {
        return;
    }
    loadRound(selectedRound);
}

void MainWindow::loadRound(int index)
{
    if (index < 0 || index >= rounds.size()) {
        return;
    }

    currentRound = index;
    lives = MaxLives;
    errors = 0;
    hintsUsed = 0;
    elapsedSeconds = 0;

    const int size = rounds[index].size;
    board = QVector<QVector<CellState>>(size, QVector<CellState>(size, Unknown));

    ui->gameTitleLabel->setText(QString("Раунд %1 - поле %2x%2").arg(rounds[index].number).arg(size));

    ui->gameTitleLabel->setStyleSheet(R"(
    color: #a78bfa;
    font-size: 22px;
    font-weight: 900;
    letter-spacing: 1px;
)");
    ui->statusLabel->setText("Натискай клітинки: зафарбовано → X → порожньо.");
    setGameControlsEnabled(true);
    renderBoard();

    elapsedTimer.restart();
    timer.start(1000);
    updateTimerLabel();
    updateCoinLabels();
    ui->stackedWidget->setCurrentWidget(ui->gamePage);
}

QWidget *MainWindow::createPicturePreview(const Round &round, int cellSize, QWidget *parent) const
{
    auto *frame = new QFrame(parent);
    auto *layout = new QGridLayout(frame);
    layout->setSpacing(1);
    layout->setContentsMargins(0, 0, 0, 0);

    for (int row = 0; row < round.size; ++row) {
        for (int column = 0; column < round.size; ++column) {
            auto *cell = new QLabel(frame);
            cell->setMinimumSize(cellSize, cellSize);
            cell->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            const bool filled = round.solution[row][column] == 1;
            cell->setStyleSheet(QString("background: %1; border: 1px solid %2;")
                                    .arg(filled ? round.paintColor : "#f8fafc")
                                    .arg(filled ? "#111827" : "#e2e8f0"));
            layout->addWidget(cell, row, column);
        }
    }

    return frame;
}

void MainWindow::renderBoard()
{
    while (QLayoutItem *item = ui->boardLayout->takeAt(0)) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
    cellButtons.clear();

    const Round &round = rounds[currentRound];
    const int cellSize = round.size <= 5 ? 48 : 34;

    ui->boardLayout->setSpacing(0);
    ui->boardLayout->setAlignment(Qt::AlignCenter);
    ui->boardLayout->addWidget(new QLabel("", ui->gamePage), 0, 0);

    for (int column = 0; column < round.size; ++column) {
        auto *label = new QLabel(columnHints(round, column).join("\n"), ui->gamePage);
        label->setAlignment(Qt::AlignCenter | Qt::AlignBottom);
        label->setMinimumWidth(cellSize);
        label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        label->setMinimumHeight(78);
        label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
        label->setStyleSheet("font-size: 13px; color: #334155;");
        ui->boardLayout->addWidget(label, 0, column + 1);
        ui->boardLayout->setColumnMinimumWidth(column + 1, cellSize);
        ui->boardLayout->setColumnStretch(column + 1, 0);
    }

    for (int row = 0; row < round.size; ++row) {
        auto *label = new QLabel(rowHints(round, row).join(" "), ui->gamePage);
        label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        label->setMinimumWidth(96);
        label->setFixedHeight(cellSize);
        label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        label->setStyleSheet("font-size: 13px; color: #334155;");
        ui->boardLayout->addWidget(label, row + 1, 0);
        ui->boardLayout->setRowMinimumHeight(row + 1, cellSize);
        ui->boardLayout->setRowStretch(row + 1, 0);

        QVector<QPushButton *> rowButtons;
        for (int column = 0; column < round.size; ++column) {
            auto *cell = new QPushButton(ui->gamePage);
            cell->setFixedSize(cellSize, cellSize);
            cell->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            cell->setCursor(Qt::PointingHandCursor);
            applyCellStyle(cell, Unknown);
            ui->boardLayout->addWidget(cell, row + 1, column + 1);
            rowButtons.append(cell);
            connect(cell, &QPushButton::clicked, this, [this, row, column]() { cycleCell(row, column); });
        }
        cellButtons.append(rowButtons);
    }
}

void MainWindow::cycleCell(int row, int column)
{
    int next = (static_cast<int>(board[row][column]) + 1) % 3;
    board[row][column] = static_cast<CellState>(next);
    applyCellStyle(cellButtons[row][column], board[row][column]);
}

void MainWindow::applyCellStyle(QPushButton *button, CellState state)
{
    const QString cellBox = "min-width: 0px; min-height: 0px; max-width: 999px; max-height: 999px; padding: 0px;";
    if (state == Filled) {
        button->setText("");
        button->setStyleSheet(cellBox + "background: #111827; border: 1px solid #334155;");
    } else if (state == Marked) {
        button->setText("X");
        button->setStyleSheet(cellBox + "font-weight: 800; color: #dc2626; background: #f8fafc; border: 1px solid #cbd5e1;");
    } else {
        button->setText("");
        button->setStyleSheet(cellBox + "background: white; border: 1px solid #cbd5e1;");
    }
}

void MainWindow::restoreBoardStyles()
{
    const int size = rounds[currentRound].size;
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column) {
            applyCellStyle(cellButtons[row][column], board[row][column]);
        }
    }
}

void MainWindow::revealSolvedPicture()
{
    const Round &round = rounds[currentRound];
    for (int row = 0; row < round.size; ++row) {
        for (int column = 0; column < round.size; ++column) {
            QPushButton *cell = cellButtons[row][column];
            if (round.solution[row][column] == 1) {
                cell->setText("");
                cell->setStyleSheet(QString("min-width: 0px; min-height: 0px; padding: 0px; background: %1; border: 1px solid #111827;").arg(round.paintColor));
            } else {
                cell->setText("");
                cell->setStyleSheet("min-width: 0px; min-height: 0px; padding: 0px; background: #f8fafc; border: 1px solid #e2e8f0;");
            }
        }
    }
}

void MainWindow::setGameControlsEnabled(bool enabled)
{
    ui->hintButton->setEnabled(enabled);
    ui->checkButton->setEnabled(enabled);
    ui->resetButton->setEnabled(enabled);
}

void MainWindow::useHint()
{
    if (coins < HintCost) {
        ui->coinLabel->setStyleSheet(
            "color: #fbbf24;"
            "font-size: 16px;"
            "font-weight: 800;"
            );
        ui->statusLabel->setText(QString("Недостатньо монет. Підказка коштує %1 🪙.").arg(HintCost));
        return;
    }

    const Round &round = rounds[currentRound];
    for (int row = 0; row < round.size; ++row) {
        for (int column = 0; column < round.size; ++column) {
            if (round.solution[row][column] == 1 && board[row][column] != Filled) {
                coins -= HintCost;
                ++hintsUsed;
                board[row][column] = Filled;
                cellButtons[row][column]->setStyleSheet(QString("min-width: 0px; min-height: 0px; padding: 0px; background: %1; border: 2px solid #f59e0b;").arg(round.paintColor));
                updateCoinLabels();
                saveProgress();
                ui->statusLabel->setText(QString("Підказка використана: відкрита правильна клітинка. Підказок: %1").arg(hintsUsed));
                QTimer::singleShot(700, this, &MainWindow::restoreBoardStyles);
                return;
            }
        }
    }

    ui->statusLabel->setText("Підказка не потрібна: усі потрібні клітинки вже відкриті.");
}

void MainWindow::verifyBoard()
{
    const Round &round = rounds[currentRound];
    int placed = 0;
    int correctPlaced = 0;
    int wrongPlaced = 0;

    for (int row = 0; row < round.size; ++row) {
        for (int column = 0; column < round.size; ++column) {
            const bool expectedFilled = round.solution[row][column] == 1;
            const CellState state = board[row][column];
            QPushButton *cell = cellButtons[row][column];

            if (state == Filled) {
                ++placed;
                if (expectedFilled) {
                    ++correctPlaced;
                    cell->setStyleSheet("min-width: 0px; min-height: 0px; padding: 0px; background: #22c55e; border: 2px solid #15803d;");
                } else {
                    ++wrongPlaced;
                    cell->setStyleSheet("min-width: 0px; min-height: 0px; padding: 0px; background: #ef4444; border: 2px solid #991b1b;");
                }
            } else if (state == Marked && expectedFilled) {
                ++wrongPlaced;
                cell->setStyleSheet("min-width: 0px; min-height: 0px; padding: 0px; font-weight: 800; color: white; background: #ef4444; border: 2px solid #991b1b;");
            }
        }
    }

    const int accuracy = placed == 0 ? 0 : (correctPlaced * 100 / placed);
    ui->statusLabel->setText(QString("Перевірка: правильних клітинок %1, помилок %2, точність %3%.")
                                 .arg(correctPlaced)
                                 .arg(wrongPlaced)
                                 .arg(accuracy));

    QTimer::singleShot(900, this, &MainWindow::restoreBoardStyles);
}

void MainWindow::checkBoard()
{
    const Round &round = rounds[currentRound];
    bool correct = true;
    for (int row = 0; row < round.size; ++row) {
        for (int column = 0; column < round.size; ++column) {
            const bool expectedFilled = round.solution[row][column] == 1;
            const bool actualFilled = board[row][column] == Filled;
            if (expectedFilled != actualFilled) {
                correct = false;
                break;
            }
        }
        if (!correct) {
            break;
        }
    }

    if (correct) {
        timer.stop();
        elapsedSeconds = elapsedTimer.elapsed() / 1000;
        setGameControlsEnabled(false);
        revealSolvedPicture();
        ui->statusLabel->setText("Правильно! Малюнок відкрито.");
        QTimer::singleShot(1200, this, [this]() { finishRound(true); });
        return;
    }

    --lives;
    ++errors;
    if (lives <= 0) {
        finishRound(false);
    } else {
        ui->statusLabel->setText("Є помилка. Перевір підказки та спробуй ще раз.");
        updateTimerLabel();
    }
}

void MainWindow::resetBoard()
{
    const int size = rounds[currentRound].size;
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column) {
            board[row][column] = Unknown;
            applyCellStyle(cellButtons[row][column], Unknown);
        }
    }
    ui->statusLabel->setText("Поле очищено. Таймер і життя залишаються.");
}

void MainWindow::finishRound(bool won)
{
    if (timer.isActive()) {
        timer.stop();
        elapsedSeconds = elapsedTimer.elapsed() / 1000;
    }

    if (won) {
        rounds[currentRound].completed = true;
        if (rounds[currentRound].bestSeconds == 0 || elapsedSeconds < rounds[currentRound].bestSeconds) {
            rounds[currentRound].bestSeconds = elapsedSeconds;
        }
        const int stars = starsForResult();
        const int earnedCoins = coinsForResult(stars);
        rounds[currentRound].bestStars = qMax(rounds[currentRound].bestStars, stars);
        rounds[currentRound].coinsEarned = qMax(rounds[currentRound].coinsEarned, earnedCoins);
        coins += earnedCoins;
        completeDailyMission();
        unlockNextRound();
        syncBonusProgress();
        ui->resultTitleLabel->setText("Раунд пройдено!");
        ui->ratingLabel->setText(starText(stars));
    } else {
        ui->resultTitleLabel->setText("Спробуй ще раз");
        ui->ratingLabel->setText("Без зірок");
    }

    const int stars = won ? starsForResult() : 0;
    const int earnedCoins = won ? coinsForResult(stars) : 0;
    ui->resultStatsLabel->setText(QString("Час: %1\nПомилки: %2\nПідказки: %3\nЗароблено: +%4 🪙\nУсього монет: %5 🪙")
                                      .arg(formatTime(elapsedSeconds))
                                      .arg(errors)
                                      .arg(hintsUsed)
                                      .arg(earnedCoins)
                                      .arg(coins));

    ui->nextButton->setEnabled(won && currentRound + 1 < rounds.size());
    updateCoinLabels();
    saveProgress();
    ui->stackedWidget->setCurrentWidget(ui->resultPage);
}

void MainWindow::unlockNextRound()
{
    if (currentRound + 1 < rounds.size()) {
        rounds[currentRound + 1].unlocked = true;
    }
}

void MainWindow::showMenu()
{
    timer.stop();
    syncBonusProgress();
    refreshMenu();
    ui->stackedWidget->setCurrentWidget(ui->menuPage);
}

void MainWindow::showLevelBrowser()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Choose level");
    dialog.resize(720, 520);

    auto *mainLayout = new QVBoxLayout(&dialog);
    auto *title = new QLabel("Choose difficulty and level", &dialog);
    title->setStyleSheet("font-size: 22px; font-weight: 800; color: #0f172a;");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    auto *difficultyLayout = new QHBoxLayout();
    auto *allButton = new QPushButton("All", &dialog);
    auto *easyButton = new QPushButton("Easy 5x5", &dialog);
    auto *hardButton = new QPushButton("Hard 10x10", &dialog);
    difficultyLayout->addStretch();
    difficultyLayout->addWidget(allButton);
    difficultyLayout->addWidget(easyButton);
    difficultyLayout->addWidget(hardButton);
    difficultyLayout->addStretch();
    mainLayout->addLayout(difficultyLayout);

    auto *scrollArea = new QScrollArea(&dialog);
    scrollArea->setWidgetResizable(true);
    auto *content = new QWidget(scrollArea);


    auto *grid = new QGridLayout(content);
    for (int i = 0; i < rounds.size(); ++i) {
        grid->setRowStretch(i, 1);
        grid->setColumnStretch(i, 1);
    }

    grid->setSpacing(2);
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setAlignment(Qt::AlignCenter);
    grid->setSpacing(12);
    scrollArea->setWidget(content);
    mainLayout->addWidget(scrollArea);

    auto rebuild = [this, grid, content, &dialog](const QString &difficulty) {
        while (QLayoutItem *item = grid->takeAt(0)) {
            if (item->widget()) {
                item->widget()->deleteLater();
            }
            delete item;
        }

        int visibleIndex = 0;
        for (int i = 0; i < rounds.size(); ++i) {
            const Round &round = rounds[i];
            if (!difficulty.isEmpty() && round.difficulty != difficulty) {
                continue;
            }

            auto *button = new QPushButton(content);
            button->setMinimumSize(150, 92);
            button->setCursor(round.unlocked ? Qt::PointingHandCursor : Qt::ForbiddenCursor);
            button->setEnabled(round.unlocked);
            button->setText(QString("Round %1\n%2\n%3")
                                .arg(round.number)
                                .arg(round.difficulty)
                                .arg(round.completed ? starText(round.bestStars) : (round.unlocked ? "Open" : "Locked")));
            button->setStyleSheet(round.unlocked
                                      ? "font-size: 14px; font-weight: 800; background: #ffffff; border: 2px solid #38bdf8; border-radius: 12px;"
                                      : "font-size: 14px; color: #64748b; background: #e2e8f0; border: 2px solid #cbd5e1; border-radius: 12px;");
            connect(button, &QPushButton::clicked, this, [this, i, &dialog]() {
                openRound(i);
                dialog.accept();
                startSelectedRound();
            });
            grid->addWidget(button, visibleIndex / 3, visibleIndex % 3);
            ++visibleIndex;
        }

        if (visibleIndex == 0) {
            auto *emptyLabel = new QLabel("No levels for this difficulty.", content);
            emptyLabel->setAlignment(Qt::AlignCenter);
            emptyLabel->setStyleSheet("font-size: 16px; color: #64748b;");
            grid->addWidget(emptyLabel, 0, 0);
        }
    };

    connect(allButton, &QPushButton::clicked, &dialog, [rebuild]() { rebuild(""); });
    connect(easyButton, &QPushButton::clicked, &dialog, [rebuild]() { rebuild("Easy"); });
    connect(hardButton, &QPushButton::clicked, &dialog, [rebuild]() { rebuild("Hard"); });

    rebuild("");
    dialog.exec();
}

void MainWindow::showCompletedPictures()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Completed pictures");
    dialog.resize(760, 560);

    auto *mainLayout = new QVBoxLayout(&dialog);
    auto *title = new QLabel("Completed pictures", &dialog);
    title->setStyleSheet("font-size: 22px; font-weight: 800; color: #0f172a;");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    auto *scrollArea = new QScrollArea(&dialog);
    scrollArea->setWidgetResizable(true);
    auto *content = new QWidget(scrollArea);
    auto *grid = new QGridLayout(content);

    grid->setSpacing(2);
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setAlignment(Qt::AlignCenter);
    grid->setSpacing(18);
    scrollArea->setWidget(content);
    mainLayout->addWidget(scrollArea);

    int visibleIndex = 0;
    for (const Round &round : rounds) {
        if (!round.completed) {
            continue;
        }

        auto *card = new QFrame(content);
        card->setStyleSheet("QFrame { background: #ffffff; border: 1px solid #cbd5e1; border-radius: 8px; } QLabel { border: none; }");
        auto *cardLayout = new QVBoxLayout(card);
        cardLayout->setSpacing(8);

        auto *name = new QLabel(QString("Round %1 - %2").arg(round.number).arg(round.difficulty), card);
        name->setAlignment(Qt::AlignCenter);
        name->setStyleSheet("font-size: 15px; font-weight: 800; color: #0f172a;");
        cardLayout->addWidget(name);
        cardLayout->addWidget(createPicturePreview(round, round.size <= 5 ? 18 : 10, card), 0, Qt::AlignCenter);

        auto *stats = new QLabel(QString("%1 | Best: %2 | +%3 coins")
                                     .arg(starText(round.bestStars))
                                     .arg(formatTime(round.bestSeconds))
                                     .arg(round.coinsEarned), card);
        stats->setAlignment(Qt::AlignCenter);
        stats->setStyleSheet("font-size: 13px; color: #475569;");
        cardLayout->addWidget(stats);

        grid->addWidget(card, visibleIndex / 3, visibleIndex % 3);
        ++visibleIndex;
    }

    if (visibleIndex == 0) {
        auto *emptyLabel = new QLabel("No completed levels yet.", content);
        emptyLabel->setAlignment(Qt::AlignCenter);
        emptyLabel->setStyleSheet("font-size: 16px; color: #64748b;");
        grid->addWidget(emptyLabel, 0, 0);
    }

    auto *closeButton = new QPushButton("Close", &dialog);
    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    mainLayout->addWidget(closeButton, 0, Qt::AlignCenter);
    dialog.exec();
}

void MainWindow::showMiniGames()
{
    syncBonusProgress();
    if (!miniGamesWidget) {
        miniGamesWidget = new MiniGamesWidget(ui->stackedWidget);
        ui->stackedWidget->addWidget(miniGamesWidget);
        connect(miniGamesWidget, &MiniGamesWidget::backRequested, this, &MainWindow::showMenu);
        connect(miniGamesWidget, &MiniGamesWidget::mazeAdventureRequested, this, &MainWindow::showMazeAdventure);
    }
    miniGamesWidget->setProgress(bonusProgress);
    ui->stackedWidget->setCurrentWidget(miniGamesWidget);
}

void MainWindow::showMazeAdventure()
{
    syncBonusProgress();
    if (!mazeAdventureWidget) {
        mazeAdventureWidget = new MazeAdventureWidget(ui->stackedWidget);
        ui->stackedWidget->addWidget(mazeAdventureWidget);
        connect(mazeAdventureWidget, &MazeAdventureWidget::backRequested,
                this, &MainWindow::showMenu);
        connect(mazeAdventureWidget, &MazeAdventureWidget::progressChanged, this, [this](const ProgressData &progress) {
            bonusProgress = progress;
            if (miniGamesWidget) {
                miniGamesWidget->setProgress(bonusProgress);
            }
        });
    }
    mazeAdventureWidget->setProgress(bonusProgress);
    ui->stackedWidget->setCurrentWidget(mazeAdventureWidget);
}

void MainWindow::repeatRound()
{
    loadRound(currentRound);
}

void MainWindow::nextRound()
{
    if (currentRound + 1 < rounds.size() && rounds[currentRound + 1].unlocked) {
        selectedRound = currentRound + 1;
        loadRound(selectedRound);
    } else {
        showMenu();
    }
}

void MainWindow::updateTimerLabel()
{
    elapsedSeconds = timer.isActive() ? elapsedTimer.elapsed() / 1000 : elapsedSeconds;
    QString hearts;
    for (int i = 0; i < lives; ++i) {
        hearts += QChar(0x2665);
    }
    for (int i = lives; i < MaxLives; ++i) {
        hearts += QChar(0x2661);
    }
    ui->lifeLabel->setText(QString("Життя: %1").arg(hearts));
    ui->timerLabel->setText(QString("Час: %1").arg(formatTime(elapsedSeconds)));
    ui->mistakesLabel->setText(QString("Помилки: %1 | Підказки: %2").arg(errors).arg(hintsUsed));
}

int MainWindow::starsForResult() const
{
    const int targetSeconds = rounds[currentRound].size <= 5 ? 90 : 180;
    if (errors == 0 && hintsUsed == 0 && elapsedSeconds <= targetSeconds) {
        return 3;
    }
    if (hintsUsed == 0 && errors <= 2) {
        return 2;
    }
    return 1;
}

int MainWindow::coinsForResult(int stars) const
{
    const int targetSeconds = rounds[currentRound].size <= 5 ? 90 : 180;
    const int speedBonus = qMax(0, (targetSeconds - elapsedSeconds) / 10);
    return stars * 10 + speedBonus;
}

QString MainWindow::starText(int stars) const
{
    if (stars <= 0) {
        return "Без зірок";
    }
    return QString(stars, QChar(0x2605)) + QString(3 - stars, QChar(0x2606));
}

void MainWindow::updateCoinLabels()
{
    ui->coinLabel->setText(QString("💰 %1").arg(coins));
    ui->gameCoinLabel->setStyleSheet(
        "color: #7c3aed;"
        "font-weight: 700;"
        );
    ui->gameCoinLabel->setText(QString("%1 🪙 | 💡 %2").arg(coins).arg(HintCost));
}

void MainWindow::completeDailyMission()
{
    if (dailyMissionDone) {
        return;
    }
    dailyMissionDone = true;
    coins += 20;
}

int MainWindow::completedNonogramLevels() const
{
    int completed = 0;
    for (const Round &round : rounds) {
        if (round.completed) {
            ++completed;
        }
    }
    return completed;
}

void MainWindow::syncBonusProgress()
{
    bonusProgress = progressService.syncNonogramProgress(completedNonogramLevels());
    if (miniGamesWidget) {
        miniGamesWidget->setProgress(bonusProgress);
    }
    if (mazeAdventureWidget) {
        mazeAdventureWidget->setProgress(bonusProgress);
    }
}

void MainWindow::loadProgress()
{
    QSettings settings("StudentProject", "NonogramGame");
    coins = settings.value("coins", 30).toInt();

    const QString today = QDate::currentDate().toString(Qt::ISODate);
    dailyMissionDone = settings.value("dailyDate").toString() == today
        && settings.value("dailyDone", false).toBool();

    for (int i = 0; i < rounds.size(); ++i) {
        const QString key = QString("rounds/%1/").arg(i);
        rounds[i].unlocked = settings.value(key + "unlocked", i == 0).toBool();
        rounds[i].completed = settings.value(key + "completed", false).toBool();
        rounds[i].bestSeconds = settings.value(key + "bestSeconds", 0).toInt();
        rounds[i].bestStars = settings.value(key + "bestStars", 0).toInt();
        rounds[i].coinsEarned = settings.value(key + "coinsEarned", 0).toInt();
    }
    if (!rounds.isEmpty()) {
        rounds[0].unlocked = true;
        for (int i = 1; i < rounds.size(); ++i) {
            if (rounds[i - 1].completed) {
                rounds[i].unlocked = true;
            }
        }
        if (selectedRound >= rounds.size() || !rounds[selectedRound].unlocked) {
            selectedRound = 0;
        }
    }
}

void MainWindow::saveProgress() const
{
    QSettings settings("StudentProject", "NonogramGame");
    settings.setValue("coins", coins);
    settings.setValue("dailyDate", QDate::currentDate().toString(Qt::ISODate));
    settings.setValue("dailyDone", dailyMissionDone);

    for (int i = 0; i < rounds.size(); ++i) {
        const QString key = QString("rounds/%1/").arg(i);
        settings.setValue(key + "unlocked", rounds[i].unlocked);
        settings.setValue(key + "completed", rounds[i].completed);
        settings.setValue(key + "bestSeconds", rounds[i].bestSeconds);
        settings.setValue(key + "bestStars", rounds[i].bestStars);
        settings.setValue(key + "coinsEarned", rounds[i].coinsEarned);
    }
}

QString MainWindow::formatTime(int seconds) const
{
    const int minutes = seconds / 60;
    const int rest = seconds % 60;
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(rest, 2, 10, QChar('0'));
}

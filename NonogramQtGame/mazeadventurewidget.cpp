#include "mazeadventurewidget.h"

#include <QPainter>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QStackedLayout>
#include <QScrollArea>
// =========================
// PREVIEW WIDGET
// =========================


MazeAdventureWidget::MazeAdventureWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(R"(
        QWidget {
            background: #f4f6fb;
            color: #111827;
            font-family: Arial;
        }
    )");

    m_stack = new QStackedWidget(this);

    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->addWidget(m_stack);

    m_levelPage = new QWidget();
    m_gamePage = new QWidget();

    m_gameView = new MazeGameView();

    auto gameLayout = new QVBoxLayout(m_gamePage);
    gameLayout->setContentsMargins(10, 10, 10, 10);
    gameLayout->setSpacing(10);

    // ===== TOP BAR (EXIT BUTTON) =====
    auto *topBar = new QWidget;
    auto *topLayout = new QHBoxLayout(topBar);
    topLayout->setContentsMargins(0, 0, 0, 0);

    auto *exitBtn = new QPushButton("← Exit");
    exitBtn->setFixedSize(100, 36);

    exitBtn->setStyleSheet(R"(
    QPushButton {
        background: #ef4444;
        color: white;
        border-radius: 8px;
        font-weight: bold;
    }
    QPushButton:hover {
        background: #dc2626;
    }
)");

    connect(exitBtn, &QPushButton::clicked, this, [this]() {
        m_stack->setCurrentWidget(m_levelPage);
    });

    topLayout->addWidget(exitBtn);
    topLayout->addStretch();

    gameLayout->addWidget(topBar);

    // ===== GAME VIEW =====
    gameLayout->addWidget(m_gameView, 1);

    m_stack->addWidget(m_levelPage);
    m_stack->addWidget(m_gamePage);

    m_levels = loadMazeLevels();
    buildLevelSelection();
}

MazePreviewWidget::MazePreviewWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(200, 120);
}

void MazePreviewWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    QLinearGradient grad(rect().topLeft(), rect().bottomRight());
    grad.setColorAt(0, QColor(15, 15, 30));
    grad.setColorAt(1, QColor(5, 5, 10));

    p.fillRect(rect(), grad);

    p.fillRect(rect(), QColor(0, 0, 0, 140));

    QFont f = p.font();
    f.setPointSize(20);
    f.setBold(true);
    p.setFont(f);

    p.setPen(Qt::white);
    p.drawText(rect(), Qt::AlignCenter, "Лабіринт");
}

// =========================
// SCHEME PREVIEW
// =========================

MazeSchemePreviewWidget::MazeSchemePreviewWidget(const MazeLevel &level, QWidget *parent)
    : QWidget(parent), m_level(level)
{
    setMinimumSize(160, 100);
}

void MazeSchemePreviewWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(rect(),QColor(17, 17, 17));

    int cell = 6;

    for (int y = 0; y < m_level.rows.size(); y++) {
        for (int x = 0; x < m_level.rows[y].size(); x++) {

            QRect r(x * cell, y * cell, cell, cell);

            QChar c = m_level.rows[y][x];

            if (c == '#')
                p.fillRect(r, Qt::darkGray);
            else
                p.fillRect(r, Qt::black);

            if (c == 'E')
                p.fillRect(r, Qt::green);

            if (c == 'S')
                p.fillRect(r, Qt::yellow);
        }
    }
}

// =========================
// GAME VIEW
// =========================

MazeGameView::MazeGameView(QWidget *parent)
    : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
}

void MazeGameView::setLevel(const MazeLevel &level)
{
    m_level = level;
    m_playerCell = findCell('S');
    update();
}

QPoint MazeGameView::findCell(QChar marker) const
{
    for (int y = 0; y < m_level.rows.size(); y++)
        for (int x = 0; x < m_level.rows[y].size(); x++)
            if (m_level.rows[y][x] == marker)
                return QPoint(x, y);

    return QPoint(1, 1);
}

bool MazeGameView::isWall(const QPoint &c) const
{
    if (c.y() < 0 || c.y() >= m_level.rows.size())
        return true;

    if (c.x() < 0 || c.x() >= m_level.rows[c.y()].size())
        return true;

    return m_level.rows[c.y()][c.x()] == '#';
}

void MazeGameView::tryMove(const QPoint &d)
{
    QPoint next = m_playerCell + d;

    if (isWall(next))
        return;

    m_playerCell = next;
    update();

    if (m_level.rows[next.y()][next.x()] == 'E')
        emit levelCompleted();
}

void MazeGameView::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Left) tryMove(QPoint(-1,0));
    if (e->key() == Qt::Key_Right) tryMove(QPoint(1,0));
    if (e->key() == Qt::Key_Up) tryMove(QPoint(0,-1));
    if (e->key() == Qt::Key_Down) tryMove(QPoint(0,1));
}

void MazeGameView::drawMiniMap(QPainter &p)
{
    if (m_level.rows.isEmpty()) return;

    int mapW = m_level.rows[0].size();
    int mapH = m_level.rows.size();

    int maxSize = 90;
    int cell = qMin(maxSize / mapW, maxSize / mapH);
    cell = qMax(cell, 3);

    int ox = width() - mapW * cell - 12;
    int oy = 12;

    // фон
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(0, 0, 0, 40));
    p.drawRoundedRect(
        QRect(ox - 4, oy - 4, mapW * cell + 8, mapH * cell + 8),
        6, 6
        );

    for (int y = 0; y < mapH; y++) {
        for (int x = 0; x < mapW; x++) {

            QRect r(ox + x * cell, oy + y * cell, cell, cell);

            char c = m_level.rows[y][x].toLatin1();

            if (c == '#')
                p.fillRect(r, QColor(209, 213, 219));
            else if (c == 'E')
                p.fillRect(r, QColor(34, 197, 94));
            else if (c == 'S')
                p.fillRect(r, QColor(234, 179, 8));
            else
                p.fillRect(r, QColor(241, 245, 249));
        }
    }

    // player
    QRect player(
        ox + m_playerCell.x() * cell,
        oy + m_playerCell.y() * cell,
        cell, cell
        );

    p.fillRect(player, QColor(59, 130, 246));
}

void MazeGameView::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(rect(), QColor(248, 250, 252));

    int size = qMin(width() / 14, height() / 14);
    size = qMax(size, 40);
    for (int y = 0; y < m_level.rows.size(); y++) {
        for (int x = 0; x < m_level.rows[y].size(); x++) {

            QRect r(x * size, y * size, size, size);

            if (m_level.rows[y][x] == '#')
                p.fillRect(r, QColor(209, 213, 219)); // світлі стіни

            if (m_level.rows[y][x] == 'E')
                p.fillRect(r, QColor(34, 197, 94));   // зелений фініш
        }
    }

    // player
    QRect player(
        m_playerCell.x() * size,
        m_playerCell.y() * size,
        size, size
        );

    p.fillRect(player, QColor(59, 130, 246)); // синій гравець

    drawMiniMap(p);
}

// =========================
// MAIN WIDGET
// =========================


// =========================
// LOAD JSON LEVELS
// =========================

QVector<MazeLevel> MazeAdventureWidget::loadMazeLevels() const
{
    QVector<MazeLevel> levels;

    QFile file("maze_levels.json");
    if (!file.open(QIODevice::ReadOnly))
        return levels;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray arr = doc["levels"].toArray();

    for (const auto &v : arr) {
        QJsonObject o = v.toObject();

        MazeLevel l;
        l.number = o["number"].toInt();
        l.title = o["title"].toString();

        for (const auto &row : o["scheme"].toArray())
            l.rows.push_back(row.toString());

        levels.push_back(l);
    }

    return levels;
}

// =========================
// LEVEL MENU
// =========================

void MazeAdventureWidget::buildLevelSelection()
{
    m_levels = loadMazeLevels();

    // =========================
    // CLEAN OLD LAYOUT
    // =========================
    QLayout *old = m_levelPage->layout();

    if (old) {
        QLayoutItem *item;
        while ((item = old->takeAt(0)) != nullptr) {
            if (item->widget()) {
                item->widget()->deleteLater();
            }
            delete item;
        }
        delete old;
    }

    m_levelPage->setLayout(nullptr);

    auto layout = new QVBoxLayout(m_levelPage);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(16);

    // =========================
    // HEADER (DRAWN, NO IMAGE)
    // =========================
    QWidget *header = new QWidget();
    header->setFixedHeight(160);

    header->setStyleSheet(R"(
        QWidget {
            background: qlineargradient(
                x1:0, y1:0, x2:1, y2:1,
                stop:0 #6366f1,
                stop:1 #3b82f6
            );
            border-radius: 14px;
        }
    )");

    auto *headerLayout = new QVBoxLayout(header);
    headerLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *title = new QLabel("Лабіринт");
    title->setAlignment(Qt::AlignCenter);

    title->setStyleSheet(R"(
        color: white;
        font-size: 28px;
        font-weight: 700;
        letter-spacing: 2px;
    )");

    headerLayout->addStretch();
    headerLayout->addWidget(title);
    headerLayout->addStretch();

    layout->addWidget(header);
    layout->addSpacing(12);



    // =========================
    // EXIT BUTTON (BACK TO NONOGRAM)
    // =========================
    auto *exitBtn = new QPushButton("← Back to Nonogram");
    exitBtn->setFixedHeight(40);
    exitBtn->setCursor(Qt::PointingHandCursor);

    exitBtn->setStyleSheet(R"(
    QPushButton {
        background: #ef4444;
        color: white;
        border-radius: 10px;
        font-weight: bold;
    }
    QPushButton:hover {
        background: #dc2626;
    }
)");

    connect(exitBtn, &QPushButton::clicked, this, [this]() {
        emit backRequested();
    });

    layout->addWidget(exitBtn);
    layout->addSpacing(12);

    // =========================
    // UNLOCK LOGIC
    // =========================
    int unlocked = qMin(m_progress.unlockedMazeLevel, m_levels.size());

    // =========================
    // GRID
    // =========================

    QWidget *gridContainer = new QWidget();
    auto gridLayout = new QGridLayout(gridContainer);

    gridLayout->setSpacing(14);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    for (int i = 0; i < m_levels.size(); i++) {

        bool open = (i + 1) <= unlocked;

        auto card = createLevelCard(i + 1, open);
        gridLayout->addWidget(card, i / 3, i % 3);
    }

    layout->addSpacing(10);

    auto *scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scroll->setWidget(gridContainer);

    layout->addWidget(scroll);

    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);

    scroll->setStyleSheet(R"(
    QScrollArea {
        background: transparent;
        border: none;
    }

    QScrollBar:vertical {
        width: 10px;
        background: transparent;
    }

    QScrollBar::handle:vertical {
        background: #cbd5e1;
        border-radius: 5px;
        min-height: 30px;
    }

    QScrollBar::handle:vertical:hover {
        background: #94a3b8;
    }

    QScrollBar::add-line:vertical,
    QScrollBar::sub-line:vertical {
        height: 0px;
    }
)");

    // =========================
    // STACK HANDLING
    // =========================
    if (m_stack->indexOf(m_levelPage) == -1)
        m_stack->addWidget(m_levelPage);

    m_stack->setCurrentWidget(m_levelPage);
}

// =========================
// CARD UI
// =========================

QFrame *MazeAdventureWidget::createLevelCard(int level, bool unlocked)
{
    auto *card = new QFrame;
    card->setFixedSize(210, 240);

    card->setStyleSheet(R"(
        QFrame {
            background: white;
            border-radius: 14px;
            border: 1px solid #e5e7eb;
        }
    )");

    auto *layout = new QVBoxLayout(card);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(10);

    QLabel *title = new QLabel(QString("Level %1").arg(level));
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-weight: bold; color:#111827;");
    layout->addWidget(title);

    // preview
    auto *previewFrame = new QFrame;
    previewFrame->setFixedHeight(120);
    previewFrame->setStyleSheet("background:#f9fafb; border-radius:10px;");

    auto *previewLayout = new QStackedLayout(previewFrame);
    previewLayout->setStackingMode(QStackedLayout::StackAll);

    auto *preview = new MazeSchemePreviewWidget(m_levels[level - 1]);
    preview->setAttribute(Qt::WA_TransparentForMouseEvents);
    previewLayout->addWidget(preview);

    layout->addWidget(previewFrame);

    QPushButton *btn = new QPushButton(unlocked ? "Play" : "Locked");
    btn->setEnabled(unlocked);

    btn->setStyleSheet(R"(
        QPushButton {
            background:#3b82f6;
            color:white;
            border-radius:8px;
            padding:6px;
        }
        QPushButton:disabled {
            background:#e5e7eb;
            color:#9ca3af;
        }
    )");

    connect(btn, &QPushButton::clicked, this, [this, level, unlocked]() {
        if (unlocked) startLevel(level);
    });

    layout->addWidget(btn);

    return card;
}
// =========================
// START LEVEL
// =========================

void MazeAdventureWidget::startLevel(int level)
{
    if (level <= 0 || level > m_levels.size())
        return;

    m_currentLevel = level;

    m_gameView->setLevel(m_levels[level - 1]);

    disconnect(m_gameView, nullptr, this, nullptr);

    connect(m_gameView, &MazeGameView::levelCompleted,
            this, &MazeAdventureWidget::finishCurrentLevel);

    m_stack->setCurrentWidget(m_gamePage);
}

// =========================
// FINISH LEVEL + SAVE
// =========================

void MazeAdventureWidget::finishCurrentLevel()
{
    if (!m_progress.completedMazeLevels.contains(m_currentLevel))
        m_progress.completedMazeLevels.push_back(m_currentLevel);

    m_progress.unlockedMazeLevel =
        m_progress.completedNonogramLevels / 3 + 1;

    m_progressService.save(m_progress);

    QMessageBox::information(this, "Win", "Level completed!");

    buildLevelSelection();
}

void MazeAdventureWidget::setProgress(const ProgressData &progress)
{
    m_progress = progress;

    m_progress.unlockedMazeLevel =
        m_progress.completedNonogramLevels / 3 + 1;

    buildLevelSelection();
}

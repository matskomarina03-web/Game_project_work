#include "minigameswidget.h"

#include "mazeadventurewidget.h"

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include <QVBoxLayout>

#include <functional>

namespace {
class ClickableFrame : public QFrame
{
public:
    explicit ClickableFrame(QWidget *parent = nullptr)
        : QFrame(parent)
    {
    }

    std::function<void()> onClick;

protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton && onClick) {
            onClick();
        }
        QFrame::mousePressEvent(event);
    }
};

QString cardStyle(bool unlocked)
{
    return unlocked
        ? "QFrame { background: #f8fafc; border: 1px solid #14b8a6; border-radius: 8px; } QLabel { border: none; }"
          "QFrame:hover { background: #ecfeff; border: 2px solid #0f766e; }"
        : "QFrame { background: #d1d5db; border: 1px solid #94a3b8; border-radius: 8px; } QLabel { border: none; color: #475569; }";
}

QString playButtonStyle()
{
    return "QPushButton { background: #0f766e; color: white; border: 1px solid #115e59; border-radius: 10px; padding: 9px 16px; font-weight: 900; }"
           "QPushButton:hover { background: #115e59; }";
}
}

MiniGamesWidget::MiniGamesWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(28, 24, 28, 24);
    root->setSpacing(18);

    auto *top = new QHBoxLayout();
    auto *back = new QPushButton("Назад", this);
    back->setCursor(Qt::PointingHandCursor);
    connect(back, &QPushButton::clicked, this, &MiniGamesWidget::backRequested);
    top->addWidget(back);
    top->addStretch();

    auto *title = new QLabel("Міні-ігри", this);
    title->setStyleSheet("font-size: 28px; font-weight: 900; color: #0f172a;");
    top->addWidget(title);
    top->addStretch();
    root->addLayout(top);

    auto *subtitle = new QLabel("Бонусні ігри відкриваються під час проходження рівнів Нонограми.", this);
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet("font-size: 15px; color: #475569;");
    root->addWidget(subtitle);

    m_cardsLayout = new QGridLayout();
    m_cardsLayout->setSpacing(16);
    root->addLayout(m_cardsLayout);
    root->addStretch();

    rebuildCards();
}

void MiniGamesWidget::setProgress(const ProgressData &progress)
{
    m_progress = progress;
    rebuildCards();
}

void MiniGamesWidget::rebuildCards()
{
    if (!m_cardsLayout) {
        return;
    }
    while (QLayoutItem *item = m_cardsLayout->takeAt(0)) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }

    m_cardsLayout->addWidget(createMazeCard(), 0, 0);
}

QFrame *MiniGamesWidget::createMazeCard()
{
    auto *card = new QFrame(this);
    card->setMinimumSize(300, 390);
    card->setStyleSheet(cardStyle(true));
    auto *layout = new QVBoxLayout(card);
    layout->setSpacing(10);

    auto *preview = new MazePreviewWidget(card);
    layout->addWidget(preview);

    auto *title = new QLabel("Лабіринтова пригода", card);
    title->setStyleSheet("font-size: 21px; font-weight: 900; color: #0f172a;");
    layout->addWidget(title);

    auto *description = new QLabel("Пройди лабіринт і знайди вихід. Нові рівні відкриваються під час проходження Нонограми.", card);
    description->setWordWrap(true);
    description->setStyleSheet("font-size: 14px; color: #475569;");
    layout->addWidget(description);

    auto *status = new QLabel(QString("Відкрито | Доступно рівнів: %1").arg(m_progress.unlockedMazeLevel), card);
    status->setStyleSheet("font-size: 14px; color: #0f766e; font-weight: 800;");
    layout->addWidget(status);
    layout->addStretch();

    auto *play = new QPushButton("Грати", card);
    play->setCursor(Qt::PointingHandCursor);
    play->setStyleSheet(playButtonStyle());
    connect(play, &QPushButton::clicked, this, &MiniGamesWidget::mazeAdventureRequested);
    layout->addWidget(play, 0, Qt::AlignLeft);
    return card;
}


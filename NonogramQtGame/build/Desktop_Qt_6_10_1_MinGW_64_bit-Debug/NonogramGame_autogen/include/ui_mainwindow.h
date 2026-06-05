/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *centralLayout;
    QStackedWidget *stackedWidget;
    QWidget *menuPage;
    QVBoxLayout *menuLayout;
    QGridLayout *gridLayout;
    QLabel *titleLabel;
    QLabel *progressLabel;
    QLabel *coinLabel;
    QLabel *missionLabel;
    QGridLayout *roundGridLayout;
    QPushButton *startButton;
    QPushButton *exitMenuButton;
    QWidget *gamePage;
    QVBoxLayout *gameLayout;
    QHBoxLayout *topBarLayout;
    QLabel *gameTitleLabel;
    QSpacerItem *topBarSpacer;
    QLabel *lifeLabel;
    QLabel *timerLabel;
    QLabel *gameCoinLabel;
    QLabel *statusLabel;
    QHBoxLayout *boardWrapLayout;
    QSpacerItem *boardLeftSpacer;
    QGridLayout *boardLayout;
    QSpacerItem *boardRightSpacer;
    QHBoxLayout *controlsLayout;
    QSpacerItem *controlsLeftSpacer;
    QPushButton *hintButton;
    QPushButton *checkButton;
    QPushButton *resetButton;
    QPushButton *exitGameButton;
    QSpacerItem *controlsRightSpacer;
    QLabel *mistakesLabel;
    QWidget *resultPage;
    QVBoxLayout *resultLayout;
    QSpacerItem *resultTopSpacer;
    QLabel *resultTitleLabel;
    QLabel *ratingLabel;
    QLabel *resultStatsLabel;
    QHBoxLayout *resultButtonsLayout;
    QSpacerItem *resultLeftSpacer;
    QPushButton *nextButton;
    QPushButton *repeatButton;
    QPushButton *menuButton;
    QSpacerItem *resultRightSpacer;
    QSpacerItem *resultBottomSpacer;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(980, 720);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setStyleSheet(QString::fromUtf8("background: #f1f5f9;"));
        centralLayout = new QVBoxLayout(centralwidget);
        centralLayout->setObjectName("centralLayout");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setStyleSheet(QString::fromUtf8("background: #f8fafc;\n"
"box-shadow: 0 1px 6px rgba(0, 0, 0, 0.06);\n"
"border-radius: 12px;"));
        menuPage = new QWidget();
        menuPage->setObjectName("menuPage");
        menuPage->setStyleSheet(QString::fromUtf8("background: #f8fafc;"));
        menuLayout = new QVBoxLayout(menuPage);
        menuLayout->setSpacing(22);
        menuLayout->setObjectName("menuLayout");
        menuLayout->setContentsMargins(48, 36, 48, 36);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        titleLabel = new QLabel(menuPage);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setStyleSheet(QString::fromUtf8("font-size: 38px;\n"
"font-weight: 900;\n"
"color: #1e293b;\n"
"letter-spacing: 2px;"));
        titleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(titleLabel, 0, 0, 1, 2);


        menuLayout->addLayout(gridLayout);

        progressLabel = new QLabel(menuPage);
        progressLabel->setObjectName("progressLabel");
        progressLabel->setStyleSheet(QString::fromUtf8("font-size: 18px;\n"
"font-weight: 800;\n"
"color: #1e40af;\n"
"background: #dbeafe;\n"
"border: 2px solid #3b82f6;\n"
"border-radius: 12px;\n"
"padding: 8px;"));
        progressLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        menuLayout->addWidget(progressLabel);

        coinLabel = new QLabel(menuPage);
        coinLabel->setObjectName("coinLabel");
        coinLabel->setStyleSheet(QString::fromUtf8("font-size: 18px;\n"
"font-weight: 800;\n"
"color: #16a34a;\n"
"background: #f1f5f9;\n"
"border: 1px solid #86efac;\n"
"border-radius: 12px;\n"
"padding: 8px 12px;"));
        coinLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        menuLayout->addWidget(coinLabel);

        missionLabel = new QLabel(menuPage);
        missionLabel->setObjectName("missionLabel");
        QPalette palette;
        QBrush brush(QColor(15, 118, 110, 255));
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush);
        QBrush brush1(QColor(241, 245, 249, 255));
        brush1.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush1);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Text, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ButtonText, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush1);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush1);
        QBrush brush2(QColor(15, 118, 110, 128));
        brush2.setStyle(Qt::BrushStyle::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::PlaceholderText, brush2);
#endif
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush1);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Text, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ButtonText, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush1);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush1);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::PlaceholderText, brush2);
#endif
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::WindowText, brush);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Text, brush);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ButtonText, brush);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush1);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::PlaceholderText, brush2);
#endif
        missionLabel->setPalette(palette);
        missionLabel->setStyleSheet(QString::fromUtf8("font-size: 16px;\n"
"font-weight: 700;\n"
"color: #0f766e;\n"
"\n"
"background: #f1f5f9;\n"
"border-left: 3px solid #14b8a6;\n"
"border-radius: 12px;\n"
"padding: 8px 12px;"));
        missionLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        menuLayout->addWidget(missionLabel);

        roundGridLayout = new QGridLayout();
        roundGridLayout->setSpacing(14);
        roundGridLayout->setObjectName("roundGridLayout");

        menuLayout->addLayout(roundGridLayout);

        startButton = new QPushButton(menuPage);
        startButton->setObjectName("startButton");
        startButton->setMinimumSize(QSize(0, 48));
        startButton->setStyleSheet(QString::fromUtf8("font-size: 18px; font-weight: 800; background: #2563eb; color: white; border-radius: 14px; padding: 10px;"));

        menuLayout->addWidget(startButton);

        exitMenuButton = new QPushButton(menuPage);
        exitMenuButton->setObjectName("exitMenuButton");
        exitMenuButton->setMinimumSize(QSize(0, 42));

        menuLayout->addWidget(exitMenuButton);

        stackedWidget->addWidget(menuPage);
        gamePage = new QWidget();
        gamePage->setObjectName("gamePage");
        gamePage->setStyleSheet(QString::fromUtf8("background: #f8fafc;"));
        gameLayout = new QVBoxLayout(gamePage);
        gameLayout->setSpacing(14);
        gameLayout->setObjectName("gameLayout");
        gameLayout->setContentsMargins(28, 24, 28, 24);
        topBarLayout = new QHBoxLayout();
        topBarLayout->setObjectName("topBarLayout");
        gameTitleLabel = new QLabel(gamePage);
        gameTitleLabel->setObjectName("gameTitleLabel");
        gameTitleLabel->setStyleSheet(QString::fromUtf8("font-size: 22px; font-weight: 700;"));

        topBarLayout->addWidget(gameTitleLabel);

        topBarSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        topBarLayout->addItem(topBarSpacer);

        lifeLabel = new QLabel(gamePage);
        lifeLabel->setObjectName("lifeLabel");
        lifeLabel->setStyleSheet(QString::fromUtf8("font-size: 18px; color: #dc2626;"));

        topBarLayout->addWidget(lifeLabel);

        timerLabel = new QLabel(gamePage);
        timerLabel->setObjectName("timerLabel");
        timerLabel->setStyleSheet(QString::fromUtf8("font-size: 18px; color: #334155;"));

        topBarLayout->addWidget(timerLabel);

        gameCoinLabel = new QLabel(gamePage);
        gameCoinLabel->setObjectName("gameCoinLabel");
        gameCoinLabel->setStyleSheet(QString::fromUtf8("font-size: 16px; font-weight: 700; color: #ca8a04;"));

        topBarLayout->addWidget(gameCoinLabel);


        gameLayout->addLayout(topBarLayout);

        statusLabel = new QLabel(gamePage);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setStyleSheet(QString::fromUtf8("font-size: 15px; color: #64748b;"));
        statusLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gameLayout->addWidget(statusLabel);

        boardWrapLayout = new QHBoxLayout();
        boardWrapLayout->setObjectName("boardWrapLayout");
        boardLeftSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        boardWrapLayout->addItem(boardLeftSpacer);

        boardLayout = new QGridLayout();
        boardLayout->setSpacing(2);
        boardLayout->setObjectName("boardLayout");

        boardWrapLayout->addLayout(boardLayout);

        boardRightSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        boardWrapLayout->addItem(boardRightSpacer);


        gameLayout->addLayout(boardWrapLayout);

        controlsLayout = new QHBoxLayout();
        controlsLayout->setObjectName("controlsLayout");
        controlsLeftSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        controlsLayout->addItem(controlsLeftSpacer);

        hintButton = new QPushButton(gamePage);
        hintButton->setObjectName("hintButton");
        hintButton->setMinimumSize(QSize(0, 42));

        controlsLayout->addWidget(hintButton);

        checkButton = new QPushButton(gamePage);
        checkButton->setObjectName("checkButton");
        checkButton->setMinimumSize(QSize(0, 42));

        controlsLayout->addWidget(checkButton);

        resetButton = new QPushButton(gamePage);
        resetButton->setObjectName("resetButton");
        resetButton->setMinimumSize(QSize(0, 42));

        controlsLayout->addWidget(resetButton);

        exitGameButton = new QPushButton(gamePage);
        exitGameButton->setObjectName("exitGameButton");
        exitGameButton->setMinimumSize(QSize(0, 42));

        controlsLayout->addWidget(exitGameButton);

        controlsRightSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        controlsLayout->addItem(controlsRightSpacer);


        gameLayout->addLayout(controlsLayout);

        mistakesLabel = new QLabel(gamePage);
        mistakesLabel->setObjectName("mistakesLabel");
        mistakesLabel->setStyleSheet(QString::fromUtf8("font-size: 15px; color: #475569;"));
        mistakesLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gameLayout->addWidget(mistakesLabel);

        stackedWidget->addWidget(gamePage);
        resultPage = new QWidget();
        resultPage->setObjectName("resultPage");
        resultPage->setStyleSheet(QString::fromUtf8("background: #111827;"));
        resultLayout = new QVBoxLayout(resultPage);
        resultLayout->setSpacing(18);
        resultLayout->setObjectName("resultLayout");
        resultLayout->setContentsMargins(52, 48, 52, 48);
        resultTopSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        resultLayout->addItem(resultTopSpacer);

        resultTitleLabel = new QLabel(resultPage);
        resultTitleLabel->setObjectName("resultTitleLabel");
        resultTitleLabel->setStyleSheet(QString::fromUtf8("font-size: 34px; font-weight: 800; color: #f8fafc;"));
        resultTitleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        resultLayout->addWidget(resultTitleLabel);

        ratingLabel = new QLabel(resultPage);
        ratingLabel->setObjectName("ratingLabel");
        ratingLabel->setStyleSheet(QString::fromUtf8("font-size: 52px; font-weight: 800; color: #facc15;"));
        ratingLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        resultLayout->addWidget(ratingLabel);

        resultStatsLabel = new QLabel(resultPage);
        resultStatsLabel->setObjectName("resultStatsLabel");
        resultStatsLabel->setStyleSheet(QString::fromUtf8("font-size: 18px; color: #dbeafe;"));
        resultStatsLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        resultLayout->addWidget(resultStatsLabel);

        resultButtonsLayout = new QHBoxLayout();
        resultButtonsLayout->setObjectName("resultButtonsLayout");
        resultLeftSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        resultButtonsLayout->addItem(resultLeftSpacer);

        nextButton = new QPushButton(resultPage);
        nextButton->setObjectName("nextButton");
        nextButton->setMinimumSize(QSize(0, 44));
        QPalette palette1;
        QBrush brush3(QColor(17, 24, 39, 255));
        brush3.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush3);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush3);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush3);
        QBrush brush4(QColor(39, 191, 115, 255));
        brush4.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Highlight, brush4);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush3);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush3);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush3);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Highlight, brush4);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush3);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush3);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush3);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Highlight, brush4);
        nextButton->setPalette(palette1);

        resultButtonsLayout->addWidget(nextButton);

        repeatButton = new QPushButton(resultPage);
        repeatButton->setObjectName("repeatButton");
        repeatButton->setMinimumSize(QSize(0, 44));

        resultButtonsLayout->addWidget(repeatButton);

        menuButton = new QPushButton(resultPage);
        menuButton->setObjectName("menuButton");
        menuButton->setMinimumSize(QSize(0, 44));

        resultButtonsLayout->addWidget(menuButton);

        resultRightSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        resultButtonsLayout->addItem(resultRightSpacer);


        resultLayout->addLayout(resultButtonsLayout);

        resultBottomSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        resultLayout->addItem(resultBottomSpacer);

        stackedWidget->addWidget(resultPage);

        centralLayout->addWidget(stackedWidget);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Nonogram / Picross", nullptr));
        titleLabel->setText(QCoreApplication::translate("MainWindow", "NONOGRAM", nullptr));
        progressLabel->setText(QCoreApplication::translate("MainWindow", "\320\222\320\270\320\261\321\226\321\200 \321\200\320\260\321\203\320\275\320\264\321\203 | \320\237\321\200\320\276\320\271\320\264\320\265\320\275\320\276: 0/10", nullptr));
        coinLabel->setText(QCoreApplication::translate("MainWindow", " \320\234\320\276\320\275\320\265\321\202\320\270: 30 \360\237\252\231", nullptr));
        missionLabel->setText(QCoreApplication::translate("MainWindow", "\320\234\321\226\321\201\321\226\321\217 \320\264\320\275\321\217: \320\277\321\200\320\276\320\271\320\264\320\270 1 \321\200\320\260\321\203\320\275\320\264 \321\226 \320\276\321\202\321\200\320\270\320\274\320\260\320\271 +20 \360\237\252\231", nullptr));
        startButton->setText(QCoreApplication::translate("MainWindow", "\342\226\266 \320\237\320\276\321\207\320\260\321\202\320\270", nullptr));
        exitMenuButton->setText(QCoreApplication::translate("MainWindow", "\320\222\320\270\321\205\321\226\320\264", nullptr));
        gameTitleLabel->setText(QCoreApplication::translate("MainWindow", "\320\223\321\200\320\260", nullptr));
        lifeLabel->setText(QCoreApplication::translate("MainWindow", "\320\226\320\270\321\202\321\202\321\217: \342\231\245\342\231\245\342\231\245", nullptr));
        timerLabel->setText(QCoreApplication::translate("MainWindow", "\320\247\320\260\321\201: 00:00", nullptr));
        gameCoinLabel->setText(QCoreApplication::translate("MainWindow", "\360\237\244\221 30 \360\237\252\231 | \360\237\222\241 10", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\321\202\320\270\321\201\320\272\320\260\320\271 \320\272\320\273\321\226\321\202\320\270\320\275\320\272\320\270: \320\267\320\260\321\204\320\260\321\200\320\261\320\276\320\262\320\260\320\275\320\276 \342\206\222 X \342\206\222 \320\277\320\276\321\200\320\276\320\266\320\275\321\214\320\276.", nullptr));
        hintButton->setText(QCoreApplication::translate("MainWindow", "\360\237\222\241 \320\237\321\226\320\264\320\272\320\260\320\267\320\272\320\260", nullptr));
        checkButton->setText(QCoreApplication::translate("MainWindow", "\342\234\205 \320\223\320\276\321\202\320\276\320\262\320\276", nullptr));
        resetButton->setText(QCoreApplication::translate("MainWindow", "\342\206\272 \320\241\320\272\320\270\320\275\321\203\321\202\320\270", nullptr));
        exitGameButton->setText(QCoreApplication::translate("MainWindow", "\320\234\320\265\320\275\321\216", nullptr));
        mistakesLabel->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\274\320\270\320\273\320\272\320\270: 0 | \320\237\321\226\320\264\320\272\320\260\320\267\320\272\320\270: 0", nullptr));
        resultTitleLabel->setText(QCoreApplication::translate("MainWindow", "\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202", nullptr));
        ratingLabel->setText(QCoreApplication::translate("MainWindow", "\342\230\205\342\230\205\342\230\205", nullptr));
        resultStatsLabel->setText(QCoreApplication::translate("MainWindow", "\320\247\320\260\321\201: 00:00\n"
"\320\237\320\276\320\274\320\270\320\273\320\272\320\270: 0\n"
"\320\227\320\260\321\200\320\276\320\261\320\273\320\265\320\275\320\276: +0 \360\237\252\231", nullptr));
        nextButton->setText(QCoreApplication::translate("MainWindow", "\320\224\320\260\320\273\321\226 \342\226\266", nullptr));
        repeatButton->setText(QCoreApplication::translate("MainWindow", "\342\206\272 \320\237\320\276\320\262\321\202\320\276\321\200\320\270\321\202\320\270", nullptr));
        menuButton->setText(QCoreApplication::translate("MainWindow", "\320\234\320\265\320\275\321\216", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

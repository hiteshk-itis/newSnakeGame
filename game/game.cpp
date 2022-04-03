#include "game.h"
#include "ui_game.h"
#include <QDebug>
#include <QLCDNumber>

Game::Game(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Game)
{
    ui->setupUi(this);
    ui->scoreLcdNumber->setSegmentStyle(QLCDNumber::Flat);

    connect(ui->startPushButton, &QPushButton::clicked,
            ui->graphicsView, &SnakeGameView::viewSnakeLengthIncrease);

    connect(ui->graphicsView, SIGNAL(scoreDisplay(qreal)),
            ui->scoreLcdNumber, SLOT(display(qreal)));


}

Game::~Game()
{
    delete ui;
}

qreal Game::gameWidth() const
{
    return width();
}


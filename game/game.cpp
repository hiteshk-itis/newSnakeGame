#include "game.h"
#include "ui_game.h"
#include <QDebug>
#include <QDialogButtonBox>
#include <QLCDNumber>
#include <qdialog.h>
#include "chatclient.h"
#include "multiplayersnakegameview.h"

Game::Game(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Game)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->mainPage);

    QDialog *nameWin = new QDialog(this);
    QVBoxLayout* vlayout = new QVBoxLayout(nameWin);
    QLineEdit *userName = new QLineEdit(nameWin);

    userName->setPlaceholderText("Username");

    ChatClient *widget_mpChatClient =  ui->multiplayerPage->findChild<ChatClient*>("widget_mpChatClient");
    MultiPlayerSnakeGameView* graphicsView_multiPlayer = ui->multiplayerPage->findChild<MultiPlayerSnakeGameView*>("graphicsView_multiPlayer");

QDialogButtonBox* btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, nameWin);

    connect(userName, &QLineEdit::returnPressed, [=]{
       ui->playerNameLabel->setText(userName->text());
    });

connect(btn_box, &QDialogButtonBox::accepted, nameWin, &QDialog::accept);
connect(btn_box, &QDialogButtonBox::rejected, nameWin, &QDialog::reject);

vlayout->addWidget(userName);
vlayout->addWidget(btn_box);
nameWin->exec();


    connect(widget_mpChatClient, &ChatClient::disableCreateRoom, [=](bool state){
        ui->pushButton_createRoom->setEnabled(!state);
    });
    connect(ui->startPushButton, &QPushButton::clicked,
            ui->graphicsView, &SnakeGameView::viewSnakeLengthIncrease);

//    MainWindow* mw = ui->aiPage->findChild<MainWindow*>("widget");





    connect(ui->pushButton_ai, &QPushButton::clicked,[=]{

        ui->stackedWidget->setCurrentWidget(ui->aiPage);

    });

    connect(ui->graphicsView, SIGNAL(scoreDisplay(qreal)),
            ui->scoreLcdNumber, SLOT(display(qreal)));

    connect(this, &Game::PlayerName, widget_mpChatClient,
            &ChatClient::getPlayerName);

    connect(widget_mpChatClient, &ChatClient::generatePlayers, graphicsView_multiPlayer, &MultiPlayerSnakeGameView::generateOtherSnakes);
    connect(widget_mpChatClient, &ChatClient::directionChanged, graphicsView_multiPlayer, &MultiPlayerSnakeGameView::directionChanged);


    connect(ui->pushButton_singlePlayer, &QPushButton::clicked,[=]{
        ui->stackedWidget->setCurrentWidget(ui->singlPlayerPage);
    });
    connect(ui->pushButton_createRoom, &QPushButton::clicked,[=]{
        ui->stackedWidget->setCurrentWidget(ui->multiplayerPage);
    });

    connect(ui->pushButton_createRoom, &QPushButton::clicked,
            widget_mpChatClient, &ChatClient::createRoomClicked);

    connect(widget_mpChatClient, &ChatClient::allPlayersJoined,
            graphicsView_multiPlayer, &MultiPlayerSnakeGameView::allPlayersJoinedEmitted);

    connect(graphicsView_multiPlayer, &MultiPlayerSnakeGameView::sendDirection,
            widget_mpChatClient, &ChatClient::sendDirection);

    connect(graphicsView_multiPlayer, &MultiPlayerSnakeGameView::sendInitialPos,
            widget_mpChatClient, &ChatClient::sendInitialPos);

    connect(this, &Game::PlayerName, graphicsView_multiPlayer, &MultiPlayerSnakeGameView::getPlayerName);

    QString n = ui->playerNameLabel->text();
    QString &name = n;
    emit PlayerName(name);

    ui->scoreLcdNumber->setSegmentStyle(QLCDNumber::Flat);
}

Game::~Game()
{
    delete ui;
}

void Game::displayTextInChat(QString &msg)
{
    Q_UNUSED(msg);
//    ui->plainTextEdit_chat->appendPlainText(msg);
}

qreal Game::gameWidth() const
{
    return width();
}


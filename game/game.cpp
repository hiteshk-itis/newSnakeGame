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


    // displaying name window
    QDialog *nameWin = new QDialog(this);
    QVBoxLayout* vlayout = new QVBoxLayout(nameWin);
    QLineEdit *userName = new QLineEdit(nameWin);

    userName->setPlaceholderText("Username");

    ChatClient *widget_mpChatClient =  ui->multiplayerPage->findChild<ChatClient*>("widget_mpChatClient");

    MultiPlayerSnakeGameView* graphicsView_multiPlayer = ui->multiplayerPage->findChild<MultiPlayerSnakeGameView*>("graphicsView_multiPlayer");

    QDialogButtonBox* btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, nameWin);

    connect(userName, &QLineEdit::returnPressed, [=]{
        d_user = userName->text();
    });

connect(nameWin, &QDialog::accepted, userName, &QLineEdit::returnPressed);
connect(btn_box, &QDialogButtonBox::accepted, nameWin, &QDialog::accept);
connect(btn_box, &QDialogButtonBox::rejected, nameWin, &QDialog::reject);

vlayout->addWidget(userName);
vlayout->addWidget(btn_box);
nameWin->exec();

QString &name = d_user;

//========================================================================================================================

ui->graphicsView->move(QPoint((ui->singlPlayerPage->width()/2)-200, 20));

QPixmap mPlayers(":/img/res/undraw_game_day_ucx9.svg");
QPixmap gConsole(":/img/res/undraw_Gaming_re_cma2.png");


    // connecting home btns with go to home page action
    connect(ui->actionHome, &QAction::triggered, [=]{
        ui->graphicsView->HomeBtnClicked();
        ui->stackedWidget->setCurrentWidget(ui->mainPage);
    });
    QPushButton* multiPlayerHomeBtn =ui->multiplayerPage->findChild<QPushButton*>("home_btn");
    QPushButton* aiHomeBtn =ui->aiPage->findChild<QPushButton*>("home_btn");

    connect(ui->home_btn, &QPushButton::clicked, ui->actionHome, &QAction::trigger);
    connect(multiPlayerHomeBtn, &QPushButton::clicked, ui->actionHome, &QAction::trigger);
    connect(aiHomeBtn, &QPushButton::clicked, ui->actionHome, &QAction::trigger);

    //  connecting menu buttons with respective pages
    connect(ui->pushButton_singlePlayer, &QPushButton::clicked,[=]{
        ui->stackedWidget->setCurrentWidget(ui->singlPlayerPage);
    });
    connect(ui->pushButton_createRoom, &QPushButton::clicked,[=]{
        ui->stackedWidget->setCurrentWidget(ui->multiplayerPage);
    });

    connect(ui->pushButton_ai, &QPushButton::clicked,[=]{
        ui->stackedWidget->setCurrentWidget(ui->aiPage);
    });

//=====================================================================================================================
    connect(widget_mpChatClient, &ChatClient::disableCreateRoom, [=](bool state){
        ui->pushButton_createRoom->setEnabled(!state);
    });

    connect(ui->startPushButton, &QPushButton::clicked,
            ui->graphicsView, &SnakeGameView::viewSnakeLengthIncrease);


//    connect(ui->graphicsView, SIGNAL(scoreDisplay(qreal)),
//            ui->scoreLcdNumber, SLOT(display(qreal)));

// send the player name to all the classes
    connect(this, &Game::PlayerName, widget_mpChatClient,
            &ChatClient::getPlayerName);

    connect(ui->graphicsView, &SnakeGameView::callPlayerName, this, &Game::PlayerNameCalled);
    connect(this, &Game::PlayerName, ui->graphicsView,
            &SnakeGameView::getPlayerName);

    connect(this, &Game::PlayerName, graphicsView_multiPlayer,
            &MultiPlayerSnakeGameView::getPlayerName);
//============================================================================================

    connect(widget_mpChatClient, &ChatClient::generatePlayers, graphicsView_multiPlayer, &MultiPlayerSnakeGameView::generateOtherSnakes);
    connect(widget_mpChatClient, &ChatClient::directionChanged, graphicsView_multiPlayer, &MultiPlayerSnakeGameView::directionChanged);




    connect(ui->pushButton_createRoom, &QPushButton::clicked,
            widget_mpChatClient, &ChatClient::createRoomClicked);

    connect(widget_mpChatClient, &ChatClient::allPlayersJoined,
            graphicsView_multiPlayer, &MultiPlayerSnakeGameView::allPlayersJoinedEmitted);

    connect(graphicsView_multiPlayer, &MultiPlayerSnakeGameView::sendDirection,
            widget_mpChatClient, &ChatClient::sendDirection);

    connect(graphicsView_multiPlayer, &MultiPlayerSnakeGameView::sendInitialPos,
            widget_mpChatClient, &ChatClient::sendInitialPos);


// food eaten purpose
    connect(graphicsView_multiPlayer, &SnakeGameView::mpFoodEaten,
            widget_mpChatClient, &ChatClient::sendFoodEaten);

    connect(widget_mpChatClient, &ChatClient::otherUserFoodEaten,
            graphicsView_multiPlayer, &MultiPlayerSnakeGameView::otherPlayerFoodEaten);
//===================================================================================


    emit PlayerName(name);

//    ui->scoreLcdNumber->setSegmentStyle(QLCDNumber::Flat);
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

void Game::PlayerNameCalled()
{
    emit PlayerName(d_user);
}


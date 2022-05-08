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
    , dd_timer(new QTimer(this))
    , lbl_showTimer(new QTimer(this))

{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->mainPage);
    QFont defaultFont;
    defaultFont.setFamily("helvetica");
    defaultFont.setLetterSpacing(QFont::PercentageSpacing, 300);
    setFont(defaultFont);


    ui->userName->setFocus();
    setTabOrder(ui->userName, ui->pushButton_singlePlayer);
    setTabOrder(ui->pushButton_singlePlayer, ui->pushButton_createRoom);
    setTabOrder(ui->pushButton_createRoom, ui->pushButton_ai);

    ChatClient *widget_mpChatClient =  ui->multiplayerPage->findChild<ChatClient*>("widget_mpChatClient");

    MultiPlayerSnakeGameView* graphicsView_multiPlayer = ui->multiplayerPage->findChild<MultiPlayerSnakeGameView*>("graphicsView_multiPlayer");

    NewMainWindow* aiView =  ui->aiPage->findChild<NewMainWindow*>("widget");


    connect(ui->userName, &QLineEdit::editingFinished, [=]{
        d_user = ui->userName->text();
        emit PlayerName(d_user);
    });

//=======================================================================================================================
    //pictures
    ui->graphicsView->move(QPoint((ui->singlPlayerPage->width()/2)-200, 20));

    QPixmap mPlayers(":/img/res/undraw_game_day_ucx9.svg");
    QPixmap gConsole(":/img/res/undraw_Gaming_re_cma2.png");
    QPixmap icon(":/img/res/pngkit_snake-cartoon-png_1033742.png");
    QPixmap userIcon(":/img/res/user-1.svg");
//    ui->label_userIcon->setPixmap(userIcon.scaled(ui->label_userIcon->width(), ui->label_userIcon->height(), Qt::KeepAspectRatio));
    QString side_pic_frame = "background-image: url(:/img/res/drawio.svg);"
                             "position: relative;"
                             "background-repeat:no-repeat;";
//                             "background-position: center";

    ui->label_icon->setPixmap(icon.scaled(ui->label_icon->width(), ui->label_icon->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->frame_btns->setStyleSheet("background: none");
    ui->side_pic_frame->setStyleSheet(side_pic_frame);
    setStyleSheet("background-color: #e1f5e6");
//====================================================================================================
    // connecting home btns with go to home page action
    connect(ui->actionHome, &QAction::triggered, [=]{

        ui->stackedWidget->setCurrentWidget(ui->mainPage);
    });
    connect(widget_mpChatClient, &ChatClient::goToHome, ui->actionHome, &QAction::trigger);
//    QPushButton* multiPlayerHomeBtn =ui->multiplayerPage->findChild<QPushButton*>("home_btn");
//    QPushButton* aiHomeBtn =ui->aiPage->findChild<QPushButton*>("home_btn");

    QLabel* lbl_timer = ui->multiplayerPage->findChild<QLabel*>("timer_label");


    connect(graphicsView_multiPlayer, &MultiPlayerSnakeGameView::timerStarts,[=]{
        dd_timer->start(60000);
        dd_timer->setSingleShot(true);
        lbl_showTimer->start(100);
        lbl_timer->setNum(dd_timer->remainingTime());
    });
    connect(lbl_showTimer, &QTimer::timeout, [=]{
            lbl_timer->setNum(dd_timer->remainingTime());
    });
    connect(dd_timer, &QTimer::timeout, [=]{
        emit mpTimerStop();
    });

connect(this, &Game::mpTimerStop, graphicsView_multiPlayer,
        &MultiPlayerSnakeGameView::mpTimerStops);

    connect(ui->home_btn, &QPushButton::clicked, ui->graphicsView, &SnakeGameView::HomeBtnClicked);
    connect(ui->graphicsView, &SnakeGameView::goToHome, ui->actionHome, &QAction::trigger);

    connect(graphicsView_multiPlayer, &SnakeGameView::goToHome, ui->actionHome, &QAction::trigger);
    connect(aiView, &NewMainWindow::goToHome, ui->actionHome, &QAction::trigger);

    //  connecting menu buttons with respective pages
    connect(ui->pushButton_singlePlayer, &QPushButton::clicked,[=]{
        if(d_user.isEmpty()){

            ui->statusbar->showMessage("UserName field is Empty", 5000);
            return;
        }
        ui->statusbar->clearMessage();
        ui->stackedWidget->setCurrentWidget(ui->singlPlayerPage);
    });
    connect(ui->pushButton_createRoom, &QPushButton::clicked,[=]{
        if(d_user.isEmpty()){
            ui->statusbar->showMessage("UserName field is Empty");
            return;
        }

        ui->stackedWidget->setCurrentWidget(ui->multiplayerPage);
        widget_mpChatClient->createRoomClicked();
    });

    connect(ui->pushButton_ai, &QPushButton::clicked,[=]{
        if(d_user.isEmpty()){
            ui->statusbar->showMessage("UserName field is Empty");
            return;
        }

        ui->stackedWidget->setCurrentWidget(ui->aiPage);
    });
//=====================================================================================================================
//    connect(widget_mpChatClient, &ChatClient::disableCreateRoom, [=](bool state){
//        ui->pushButton_createRoom->setEnabled(!state);
//    });
    connect(ui->multiplayerPage, &MultiPlayerPage::mpCreateOrJoinRoomClicked, widget_mpChatClient, &ChatClient::createRoomClicked);

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



//    connect(graphicsView_multiPlayer, &SnakeGameView::sendFoodPos,
//            widget_mpChatClient, &ChatClient::sendFoodPos);


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


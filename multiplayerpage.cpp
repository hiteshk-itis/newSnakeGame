#include "multiplayerpage.h"
#include "ui_multiplayerpage.h"

MultiPlayerPage::MultiPlayerPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MultiPlayerPage)  
{
    ui->setupUi(this);
    d_view = ui->graphicsView_multiPlayer;
    d_scene = ui->graphicsView_multiPlayer->scene();
    d_snake = ui->graphicsView_multiPlayer->snake();
    d_chatClient =  ui->widget_mpChatClient;

    connect(ui->home_btn, &QPushButton::clicked, ui->graphicsView_multiPlayer, &MultiPlayerSnakeGameView::homeBtnClicked);

}

MultiPlayerPage::~MultiPlayerPage()
{
    delete ui;
}


void MultiPlayerPage::on_pushButton_clicked()
{
    emit mpCreateOrJoinRoomClicked();
}

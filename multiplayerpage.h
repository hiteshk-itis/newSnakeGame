#ifndef MULTIPLAYERPAGE_H
#define MULTIPLAYERPAGE_H

#include <QWidget>
#include <QDebug>

#include "snake.h"
#include "snakegameview.h"
#include "chatclient.h"

namespace Ui {
class MultiPlayerPage;
}

class MultiPlayerPage : public QWidget
{
    Q_OBJECT

public:
    explicit MultiPlayerPage(QWidget *parent = nullptr);
    ~MultiPlayerPage();


private:
    Ui::MultiPlayerPage *ui;
    SnakeGameView* d_view;
    QGraphicsScene* d_scene;
    Snake* d_snake;
    ChatClient* d_chatClient;
    QString d_user;



};

#endif // MULTIPLAYERPAGE_H

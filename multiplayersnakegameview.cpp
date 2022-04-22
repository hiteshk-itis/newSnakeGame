#include "multiplayersnakegameview.h"

#include <QTimer>

MultiPlayerSnakeGameView::MultiPlayerSnakeGameView(QWidget* parent)
    :SnakeGameView(parent)
{
    connect(d_timer, &QTimer::timeout,[=]{

        for(Snake* sn : d_snakes.values()){

            sn->moveTheSnake();
        }
    });
}

MultiPlayerSnakeGameView::~MultiPlayerSnakeGameView()
{

}

void MultiPlayerSnakeGameView::allPlayersJoinedEmitted(qreal &n)
{
    d_players = n;
    emit sendInitialPos(d_snake->pos());
}

void MultiPlayerSnakeGameView::generateOtherSnakes(QHash<QString, QPointF> snakesAndPos)
{


    for(QString name: snakesAndPos.keys()){
        Snake* &temp_snake = d_snakes[name];
        Snake::Direction &dir = d_snakesAndDirections[name];

        temp_snake = new Snake(snakesAndPos.value(name).x(), snakesAndPos.value(name).y());
        dir = Snake::Direction::None;

        d_scene->addItem(temp_snake);
    }

}

void MultiPlayerSnakeGameView::directionChanged(QString name, int d)
{
//    if(d_user == name){
//        return;
//    }
    if(!d_snakes.keys().contains(name)){
        qDebug() << "d_snakes does not contain the room name.";
        return;
    }
    Snake* &snake = d_snakes[name];

    qDebug() << "dir: "<<snake->direction();
    snake->setDirection(Snake::Direction(d));
    qDebug() << "i am in the slot";
}

void MultiPlayerSnakeGameView::getPlayerName(const QString name)
{
    d_user = name;
}



void MultiPlayerSnakeGameView::keyPressEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat() == true){
        return;
    }
    Snake::Direction dir = Snake::Direction::None;

    bool singleSnakePart = (d_snake->childItems().count() == 1);

    switch(event->key()){
        case Qt::Key_Left:
            dir = (singleSnakePart)?Snake::Direction::Left :

                    (d_snake->direction() != Snake::Direction::Right)
                    ?Snake::Direction::Left
                   :d_snake->direction();
            break;

        case Qt::Key_Right:
        dir =  (singleSnakePart)?Snake::Direction::Right :
                (d_snake->direction() != Snake::Direction::Left
                && !(d_snake->childItems()[0]->pos().x() > d_fieldWidth-2))
                ?Snake::Direction::Right
               :d_snake->direction();
            break;
        case Qt::Key_Up:
            dir = (singleSnakePart)?Snake::Direction::Up:
                    (d_snake->direction() != Snake::Direction::Down
                    && !(d_snake->childItems()[0]->pos().y() <0+5))
                ?Snake::Direction::Up
               :d_snake->direction();
            break;
        case Qt::Key_Down:

        dir = (singleSnakePart)?Snake::Direction::Down :
                (d_snake->direction() != Snake::Direction::Up
                && !(d_snake->childItems()[0]->pos().y() > d_fieldHeight-5))
                ?Snake::Direction::Down
               :d_snake->direction();
            break;
        default:
            break;
    }

    d_snake->setDirection(dir);
    if(d_players > 1){
        emit sendDirection(dir);
    }

}



#include "multiplayersnakegameview.h"

#include <QMessageBox>
#include <QTimer>

MultiPlayerSnakeGameView::MultiPlayerSnakeGameView(QWidget* parent)
    :SnakeGameView(parent)
    , dd_timer(new QTimer(this))
{
//    setDisabled(true);
//    connect(d_timer, &QTimer::timeout,
//            this, &MultiPlayerSnakeGameView::checkCollidingwSnakeAndFood);

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
emit timerStarts();
//    generateManyFoods(15);
//    setDisabled(false);

}

void MultiPlayerSnakeGameView::directionChanged(QString name, int d)
{
    if(d_user == name){
        return;
    }
    if(!(d_snakes.keys().contains(name))){
        qDebug() << "d_snakes does not contain the snake "<< name;
        return;
    }
    Snake* &snake = d_snakes[name];

    snake->setDirection(Snake::Direction(d));

}

void MultiPlayerSnakeGameView::getPlayerName(const QString name)
{
    d_user = name;
}

void MultiPlayerSnakeGameView::otherPlayerFoodEaten(QString playerName)
{
    if(d_user == playerName){
        return;
    }


   Snake* &snk = d_snakes[playerName];

   snk->increaseScore();
   snk->increaseSnakeLength();
//   displayNameAndScore(true);
   displayAllNamesAndScores();

}

void MultiPlayerSnakeGameView::homeBtnClicked()
{
    emit goToHome();
}

void MultiPlayerSnakeGameView::displayAllNamesAndScores()
{
    QFont nameAndScoreFont;
    nameAndScoreFont.setFamily("Helvetica");
    nameAndScoreFont.setCapitalization(QFont::Capitalize);
    nameAndScoreFont.setItalic(true);
    nameAndScoreFont.setPointSize(15);
    nameAndScoreFont.setBold(true);

    QString nameAndScoreHtml;

    for(auto &snkName : d_snakes.keys()){
        Snake* &snk = d_snakes[snkName];

        nameAndScoreHtml = QStringLiteral("<table>"
                               "<tr><td>Name: </td><td>%1</td></tr>"
                                "<tr><td>Score: </td><td>%2</td></tr>"
                               "</table>").arg(snkName).arg(snk->score());
    }
    nameAndScoreHtml.append(QStringLiteral("<table>"
                                          "<tr><td>Name: </td><td>%1</td></tr>"
                                           "<tr><td>Score: </td><td>%2</td></tr>"
                                          "</table>").arg(d_user).arg(d_snake->score()));

    d_nameAndScore->setFont(nameAndScoreFont);
    d_nameAndScore->setDefaultTextColor(Qt::white);
    d_scene->removeItem(d_nameAndScore);
    d_nameAndScore->setHtml(nameAndScoreHtml);
    d_scene->addItem(d_nameAndScore);
}

void MultiPlayerSnakeGameView::mpTimerStops()
{
    d_timer->stop();
    QMessageBox::information(this, "Game", "Timer Finished");
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

void MultiPlayerSnakeGameView::generateManyFoods(int n)
{
    for(int i =0; i<n; i++){
        generateFood();
    }
}

//void MultiPlayerSnakeGameView::checkCollidingwSnakeAndFood()
//{
//    for(QGraphicsItem* item: d_scene->collidingItems(d_snake->childItems()[0])){
//        if(qgraphicsitem_cast<Food*>(item)){;
//            Food* fd = qgraphicsitem_cast<Food*>(item);
//            fd->implodeOnEating();
//            foodEaten();
//        }
//    }
//}

//void MultiPlayerSnakeGameView::foodEaten()
//{
//    d_snake->increaseScore();
//    emit scoreDisplay(d_snake->score());
//    emit mpFoodEaten(d_user, d_snake->score());

//    displayNameAndScore();
//    d_snake->increaseSnakeLength();
//}



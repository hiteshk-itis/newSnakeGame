#include "snakegameview.h"
#include "game/game.h"


#include <QGraphicsItem>

#include <QDebug>

#include <QMessageBox>
#include <QStringLiteral>
#include <QTimer>
QString SnakeGameView::d_user;
SnakeGameView::SnakeGameView(QWidget* parent)
    :QGraphicsView(parent)
    ,d_snakeSpeed(3)
    , d_scene(new QGraphicsScene())
    ,d_fieldWidth(700)
    ,d_fieldHeight(550)
    ,d_snake(new Snake(300,70))
    ,d_food(new Food(d_fieldWidth,d_fieldHeight))
    ,d_timer(new QTimer)
    ,d_nameAndScore(new QGraphicsTextItem())
    , bestScore(0)


{

    d_scene->setSceneRect(QRectF(0,0, d_fieldWidth, d_fieldHeight));
    setFixedSize(d_scene->sceneRect().size().toSize());
//    fitInView(QRectF(0,0, d_fieldWidth, d_fieldHeight));
    setScene(d_scene);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    d_scene->setBackgroundBrush(QColor("#c3e58d"));
    d_snake->setPos(100, 50);
    connect(d_timer, &QTimer::timeout,[=]{
        d_snake->moveTheSnake();
    });

d_timer->setInterval(150);
emit callPlayerName();
//connect(d_timer, &QTimer::timeout,
//        this, &SnakeGameView::isInsideBoundary);



connect(d_timer, &QTimer::timeout,
        this, &SnakeGameView::checkCollidingwBoundary);
connect(d_timer, &QTimer::timeout,
        this, &SnakeGameView::checkCollidingwSnake);
d_timer->start();
    d_snake->setZValue(1);
    d_food->setZValue(2);
    d_scene->addItem(d_snake);
    d_scene->addItem(d_food);
    d_snake->setFlag(QGraphicsItem::ItemIsFocusable);
    d_snake->setFocus();

    // set border lines
    QPen mypen = QPen(Qt::red);
    mypen.setWidth(6);

    QLineF TopLine(d_scene->sceneRect().topLeft(), d_scene->sceneRect().topRight());
    QLineF LeftLine(d_scene->sceneRect().topLeft(), d_scene->sceneRect().bottomLeft());
    QLineF RightLine(d_scene->sceneRect().topRight(), d_scene->sceneRect().bottomRight());
    QLineF BottomLine(d_scene->sceneRect().bottomLeft(), d_scene->sceneRect().bottomRight());

   topLine = d_scene->addLine(TopLine, mypen);
    bottomLine = d_scene->addLine(BottomLine, mypen);
    rightLine = d_scene->addLine(RightLine, mypen);
    leftLine = d_scene->addLine(LeftLine, mypen);


}

SnakeGameView::~SnakeGameView()
{

}

qreal SnakeGameView::fieldWidth() const
{
    return d_fieldWidth;
}

qreal SnakeGameView::fieldHeight() const
{
    return d_fieldHeight;
}


void SnakeGameView::setFieldHeight(const qreal &fieldWidth)
{
    if(d_fieldWidth == fieldWidth){
        return;
    }

    d_fieldWidth = fieldWidth;
    d_scene->setSceneRect(QRectF(0,0, d_fieldWidth, d_fieldHeight));
    setFixedSize(d_scene->sceneRect().size().toSize());

}

qreal SnakeGameView::snakeSpeed() const
{
    return d_snakeSpeed;
}

void SnakeGameView::setSnakeSpeed(const qreal &snakeSpeed)
{
    d_snakeSpeed = snakeSpeed;
}

void SnakeGameView::viewSnakeLengthIncrease()
{
    d_snake->increaseSnakeLength();
}

void SnakeGameView::checkCollidingwSnake()
{


    for(QGraphicsItem* item: d_scene->collidingItems(d_food)){
        if(qgraphicsitem_cast<Snake*>(item)){;
            d_food->implodeOnEating();
            foodEaten();
        }
    }
}

void SnakeGameView::checkCollidingwBoundary()
{
    for(QGraphicsItem* item: d_scene->collidingItems(d_snake->childItems()[0])){
        if(qgraphicsitem_cast<QGraphicsLineItem*>(item)){;
            gameOver();

        }
    }
}

void SnakeGameView::foodEaten()
{
    d_snake->increaseScore();
    emit scoreDisplay(d_snake->score());
    emit mpFoodEaten(d_user, d_snake->score());

    displayNameAndScore(false);
    d_snake->increaseSnakeLength();

    generateFood();
}

void SnakeGameView::generateFood()
{
    d_food = new Food(d_fieldWidth-5, d_fieldHeight-5);
    d_scene->addItem(d_food);
//    emit sendFoodPos(d_food->pos());
}

void SnakeGameView::getPlayerName(const QString name)
{
    d_user = name;
    d_snake->setObjectName(d_user);

    displayNameAndScore();
}



Snake *SnakeGameView::snake()
{
    return d_snake;
}

void SnakeGameView::displayNameAndScore(bool multiPlayer)
{
    if(!multiPlayer){
        QFont nameAndScoreFont;
        nameAndScoreFont.setFamily("Helvetica");
        nameAndScoreFont.setCapitalization(QFont::Capitalize);
        nameAndScoreFont.setItalic(true);
        nameAndScoreFont.setPointSize(15);
        nameAndScoreFont.setBold(true);


        QString nameAndScoreHtml = QStringLiteral("<table>"
                                   "<tr><td>Name: </td><td>%1</td></tr>"
                                    "<tr><td>Score: </td><td>%2</td></tr>"
                                   "</table>").arg(d_user).arg(d_snake->score());


        d_nameAndScore->setFont(nameAndScoreFont);
        d_nameAndScore->setDefaultTextColor(Qt::white);
        d_scene->removeItem(d_nameAndScore);
        d_nameAndScore->setHtml(nameAndScoreHtml);
        d_scene->addItem(d_nameAndScore);
    }
}

/*bool SnakeGameView::isInsideBoundary()
{
    int offset = 4;
   bool a = (d_snake->childItems()[0]->pos().x() <0+offset) || (d_snake->childItems()[0]->pos().y() <0);

   bool b = (d_snake->childItems()[0]->pos().x() > d_fieldWidth) || (d_snake->childItems()[0]->pos().y() > d_fieldHeight);

   if((a || b)){
       d_timer->stop();
       return false;
   }
   else {

       return true;
   }

//        qDebug() << d_scene->sceneRect().contains( mapToParent(d_snake->childItems()[0]->pos().toPoint()));
}*/


void SnakeGameView::keyPressEvent(QKeyEvent *event)
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

//    moveSnake();

}

void SnakeGameView::moveSnake()
{

    if((d_snake->direction() == Snake::Direction::None)){
        d_timer->stop();
    }
    else if(!(d_timer->isActive())){
            d_timer->start();
    }
}

void SnakeGameView::gameOver()
{
    if((d_timer->isActive()) && (d_snake->direction() != Snake::Direction::None)){

        d_timer->stop();
        QMessageBox::warning(this, "Game Over", tr("GAME OVER\n%1").arg(d_snake->score()));
    }

}

void SnakeGameView::HomeBtnClicked()
{
    QMessageBox msgBox;
    msgBox.setText("Are you Sure?");
    if((d_timer->isActive()) && (d_snake->direction() != Snake::Direction::None)){
        msgBox.setDetailedText("Neither the game session will be paused nor the score will be saved.");
    }
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Yes:
        gameOver();
        emit goToHome();
        break;
    case QMessageBox::No:
        msgBox.close();
        break;
    default:
        break;
    }

}







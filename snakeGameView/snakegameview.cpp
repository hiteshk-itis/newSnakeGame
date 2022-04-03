#include "snakegameview.h"
#include "game/game.h"


#include <QGraphicsItem>

#include <QDebug>
#include <QTimer>

SnakeGameView::SnakeGameView(QWidget* parent)
    :QGraphicsView(parent)
    , d_scene(new QGraphicsScene())
    ,d_fieldWidth((parentWidget()->size().width()/10)*8)
    ,d_fieldHeight(500)
    ,d_snakeSpeed(3)
    ,d_snake(new Snake(300,70))
    ,d_food(new Food(d_fieldWidth,d_fieldHeight))
    ,d_timer(new QTimer)


{

    d_scene->setSceneRect(QRectF(0,0, d_fieldWidth, d_fieldHeight));
    setFixedSize(d_scene->sceneRect().size().toSize());
//    fitInView(QRectF(0,0, d_fieldWidth, d_fieldHeight));
    setScene(d_scene);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    d_scene->setBackgroundBrush(QColor("#c3e58d"));
    d_snake->setPos(100, 50);
d_timer->setInterval(150);
//connect(d_timer, &QTimer::timeout,
//        this, &SnakeGameView::isInsideBoundary);


connect(d_timer, &QTimer::timeout,
        d_snake, &Snake::moveTheSnake);

connect(d_timer, &QTimer::timeout,
        this, &SnakeGameView::checkColliding);
d_timer->start();
    d_snake->setZValue(1);
    d_food->setZValue(2);
    d_scene->addItem(d_snake);
    d_scene->addItem(d_food);
    d_snake->setFlag(QGraphicsItem::ItemIsFocusable);
    d_snake->setFocus();

    // set border lines
    QPen mypen = QPen(Qt::red);
    mypen.setWidth(0);

    QLineF TopLine(d_scene->sceneRect().topLeft(), d_scene->sceneRect().topRight());
    QLineF LeftLine(d_scene->sceneRect().topLeft(), d_scene->sceneRect().bottomLeft());
    QLineF RightLine(d_scene->sceneRect().topRight(), d_scene->sceneRect().bottomRight());
    QLineF BottomLine(d_scene->sceneRect().bottomLeft(), d_scene->sceneRect().bottomRight());

    d_scene->addLine(TopLine, mypen);
    d_scene->addLine(BottomLine, mypen);
    d_scene->addLine(RightLine, mypen);
    d_scene->addLine(LeftLine, mypen);



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

void SnakeGameView::checkColliding()
{


    for(QGraphicsItem* item: d_scene->collidingItems(d_food)){
        if(qgraphicsitem_cast<Snake*>(item)){;
            d_food->implodeOnEating();
            foodEaten();
        }
    }
}

void SnakeGameView::foodEaten()
{
    d_snake->increaseScore();
    emit scoreDisplay(d_snake->score());
    d_snake->increaseSnakeLength();
    generateFood();

}

void SnakeGameView::generateFood()
{
    d_food = new Food(d_fieldWidth, d_fieldHeight);
    d_scene->addItem(d_food);
}

bool SnakeGameView::isInsideBoundary()
{/*
    int offset = 4;
   bool a = (d_snake->childItems()[0]->pos().x() <0+offset) || (d_snake->childItems()[0]->pos().y() <0);

   bool b = (d_snake->childItems()[0]->pos().x() > d_fieldWidth) || (d_snake->childItems()[0]->pos().y() > d_fieldHeight);

   if((a || b)){
       d_timer->stop();
       return false;
   }
   else {

       return true;
   }*/

        qDebug() << d_scene->sceneRect().contains( mapToParent(d_snake->childItems()[0]->pos().toPoint()));
}


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

    moveSnake();

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







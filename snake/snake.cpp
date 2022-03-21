#include "snake.h"
#include<QGraphicsScene>

#include <QDebug>
Snake::Snake(qreal x , qreal y, QGraphicsItem *parent)
    :QGraphicsRectItem(parent)
    , d_snakeWidth(20)
    , d_snakeHeight(20)
    , d_score(0)
    , d_direction(Direction::None)
{


    if(!parentItem()){
        setRect(x, y, 0, 0);

        increaseSnakeLength();
    }
    else if(parentItem()){
        setRect(x, y, d_snakeWidth, d_snakeHeight);
        setTransformOriginPoint(this->rect().x()+this->rect().width()/2,this->rect().y()+this->rect().height()/2);
        setBrush(Qt::red);
    }
    else {
        qDebug() << "something went wrong!!";
    }

}

Snake::Direction Snake::direction() const
{
    return d_direction;
}

void Snake::setDirection(const Snake::Direction &dir)
{
    if(d_direction == dir){

        return;
    }
    d_direction = dir;
    if(!childItems().isEmpty()){

        Snake* firstPart = qgraphicsitem_cast<Snake*>(childItems().at(0));
        firstPart->setDirection(dir);
    }
}

void Snake::organizeDirections()
{
    if(!childItems().isEmpty()){
        for(int i = childItems().size()-1; i >=0; i--){

            Snake *lastPart = qgraphicsitem_cast<Snake*>(childItems().at(i));
            if(i ==0){
                lastPart->setDirection(direction());
                return;
            }
            Snake *secondLastPart = qgraphicsitem_cast<Snake*>(childItems().at(i-1));
            lastPart->setDirection(secondLastPart->direction());
        }
    }

}

void Snake::moveTheSnake()
{

     Snake *part;
    QPointF tempPos;
    for(int i = 0; i <=childItems().size()-1; i++){
        part = qgraphicsitem_cast<Snake*>(childItems().at(i));


        switch (part->direction()) {
            case Direction::Left:
            tempPos = QPointF(part->pos().x()-d_snakeWidth, part->pos().y());
               break;
            case Direction::Right:
            tempPos = QPointF(part->pos().x()+d_snakeWidth, part->pos().y());
                break;
            case Direction::Up:
            tempPos= QPointF(part->pos().x(), part->pos().y()-d_snakeHeight);

                break;
            case Direction::Down:
            tempPos= QPointF(part->pos().x(), part->pos().y()+d_snakeHeight);
                break;
            default:
                tempPos = part->pos();
                break;
        }
        if(i==0){
            QGraphicsScene* sce = scene();
            if(!sce->sceneRect().contains(mapToParent(tempPos))){
                return;
            }

        }

        part->setPos(tempPos);
    }

    organizeDirections();

}

void Snake::increaseSnakeLength()
{

        Snake* lastPart;

        if(childItems().isEmpty()){
            lastPart = this;
        }
        else{
            lastPart = qgraphicsitem_cast<Snake*>(childItems().last());
        }
        QPointF point;


    Snake *snakePart = new Snake(point.x(), point.y(),this);

    switch (lastPart->d_direction) {
    case Direction::Up:
        snakePart->setPos(lastPart->pos().x(),lastPart->pos().y()+d_snakeHeight);
        break;

    case Direction::Down:
        snakePart->setPos(lastPart->pos().x(),lastPart->pos().y()-d_snakeHeight);
        break;

    case Direction::Left:
        snakePart->setPos(lastPart->pos().x()+d_snakeWidth, lastPart->pos().y());
        break;

    case Direction::Right:
        snakePart->setPos(lastPart->pos().x()-d_snakeWidth, lastPart->pos().y());
        break;

    default:
        snakePart->setPos(lastPart->pos().x(), lastPart->pos().y());
        break;
    }


if(!(childItems().size()==1)){
    snakePart->setBrush(Qt::white);
}
snakePart->setDirection(lastPart->direction());
snakePart->setTransformOriginPoint(rect().x()+rect().width()/2, rect().y()+rect().height()/2);


}

qreal Snake::score() const
{
    return  d_score;
}

void Snake::increaseScore()
{
    d_score += 1;

}

void Snake::setScore(const qreal &score)
{
    if(d_score == score)
        return;
    d_score = score;
}


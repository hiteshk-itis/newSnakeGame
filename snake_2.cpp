#include "snake.h"


Snake::Snake(qreal x , qreal y, QGraphicsItem *parent)
    :QGraphicsRectItem(parent)
    , d_snakeWidth(30)
    , d_snakeHeight(30)
    , group (new QParallelAnimationGroup())
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

//     QPropertyAnimation *anim = new QPropertyAnimation(this, "pos");
//     anim->setDuration(150);
//     anim->setStartValue(part->pos());

    for(int i = 0; i <=childItems().size()-1; i++){
        part = qgraphicsitem_cast<Snake*>(childItems().at(i));
        QPropertyAnimation *anim = new QPropertyAnimation(part, "pos");
//anim->setTargetObject(part);
//anim->setPropertyName("pos");
anim->setDuration(150);
anim->setStartValue(part->pos());
QPointF *pt;
        switch (part->direction()) {
            case Direction::Left:
//                part->setPos(part->pos().x()-d_snakeWidth, part->pos().y());
                  pt = new QPointF(part->pos().x()-d_snakeWidth, part->pos().y());
               break;
            case Direction::Right:
//                    part->setPos(part->pos().x()+d_snakeWidth, part->pos().y());
                                      pt = new QPointF(part->pos().x()+d_snakeWidth, part->pos().y());
                break;
            case Direction::Up:
//                    part->setPos(part->pos().x(), part->pos().y()-d_snakeHeight);
                    pt = new QPointF(part->pos().x(), part->pos().y()-d_snakeHeight);
                break;
            case Direction::Down:
//                part->setPos(part->pos().x(), part->pos().y()+d_snakeHeight);
                pt = new QPointF(part->pos().x(), part->pos().y()+d_snakeHeight);
                break;
            default:
//                part->setPos(part->pos());
                pt = new QPointF(part->pos());
                break;
        }
//        qDebug() << *pt;
        anim->setEndValue(*pt);
        anim->setEasingCurve(QEasingCurve::Linear);
        group->addAnimation(anim);
//        anim->start();
    }
    group->start();
    organizeDirections();

}

void Snake::increaseSnakeLength()
{
    qDebug() <<"afd"<< group->state();
    group->stop();

    //checking if its the starting state of the game
    if(d_direction == Direction::None){

        //giving the d_direction a value sot that the first child item could be drawn on screen
        if(childItems().isEmpty()){
            d_direction = Direction::Up;
        }
        else{
            return;
        }
    }

        Snake* lastPart;

        if(childItems().isEmpty()){
            lastPart = this;
        }
        else{
            lastPart = qgraphicsitem_cast<Snake*>(childItems().last());
        }
        QPointF point;


    Snake *snakePart = new Snake(point.x(), point.y(),this);

    QPointF *pt;
    switch (lastPart->d_direction) {
    case Direction::Up:
        snakePart->setPos(lastPart->pos().x(),lastPart->pos().y()+d_snakeHeight);
        pt = new QPointF(lastPart->pos().x(),lastPart->pos().y()+d_snakeHeight);
        break;

    case Direction::Down:
        snakePart->setPos(lastPart->pos().x(),lastPart->pos().y()-d_snakeHeight);
        pt = new QPointF(lastPart->pos().x(),lastPart->pos().y()-d_snakeHeight);
        break;

    case Direction::Left:
        snakePart->setPos(lastPart->pos().x()+d_snakeWidth, lastPart->pos().y());
        pt = new QPointF(lastPart->pos().x()+d_snakeWidth, lastPart->pos().y());
        break;

    case Direction::Right:
        snakePart->setPos(lastPart->pos().x()-d_snakeWidth, lastPart->pos().y());
        pt = new QPointF(lastPart->pos().x()-d_snakeWidth, lastPart->pos().y());
        break;

    default:
        snakePart->setPos(lastPart->pos().x(), lastPart->pos().y());
        pt = new QPointF(lastPart->pos().x(), lastPart->pos().y());
        break;

    }



if(!(childItems().size()==1)){
    snakePart->setBrush(Qt::white);
}
snakePart->setDirection(lastPart->direction());
snakePart->setTransformOriginPoint(rect().x()+rect().width()/2, rect().y()+rect().height()/2);
if(group){

    group->start();
}

}


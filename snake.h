#ifndef SNAKE_H
#define SNAKE_H


#include<QObject>
#include <QBrush>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>

#include <QDebug>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

class Snake : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)// NOTIFY posChanged)
public:
    Snake(qreal x, qreal y, QGraphicsItem *parent = nullptr);

    enum class Direction{
        Left,
        Right,
        Up,
        Down,
        None};
    Q_ENUM(Direction);

    Direction direction() const;
    void setDirection(const Direction &dir);
    void organizeDirections();

    void moveTheSnake();

    void increaseSnakeLength();
    Snake* makeSnakePart();
    qreal score() const;
    void increaseScore();


private:
    qreal d_snakeWidth;
    qreal d_snakeHeight;
    QParallelAnimationGroup *group;
    Snake* d_firstPart;
    qreal d_score;

    Direction d_direction;

protected:
    void setScore(const qreal &score);

};

#endif // SNAKE_H

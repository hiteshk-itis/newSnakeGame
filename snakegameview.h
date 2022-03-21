#ifndef SNAKEGAMEVIEW_H
#define SNAKEGAMEVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWidget>

#include <QKeyEvent>
#include <QPropertyAnimation>

#include "food.h"
#include "snake.h"

class SnakeGameView : public QGraphicsView
{
    Q_OBJECT
public:
    SnakeGameView(QWidget* parent = 0);

    inline qreal fieldWidth() const;
    qreal setFieldWidth();

    inline qreal fieldHeight() const;
    void  setFieldHeight(const qreal &fieldWidth);

    inline qreal snakeSpeed() const;
    void setSnakeSpeed(const qreal &snakeSpeed);

    void viewSnakeLengthIncrease();
    void checkColliding();
    void foodEaten();
    void generateFood();
    bool isInsideBoundary();
signals:
    void scoreDisplay(qreal score);


private:
    QGraphicsScene* d_scene;
    qreal d_fieldWidth;
    qreal d_fieldHeight;
    qreal d_snakeSpeed;

    Snake *d_snake;
    Food *d_food;

    qreal d_score;

//    QPropertyAnimation* move_anim;
    QTimer* d_timer;

//    QGraphicsEllipseItem* food;

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event);
    void moveSnake();
};

#endif // SNAKEGAMEVIEW_H

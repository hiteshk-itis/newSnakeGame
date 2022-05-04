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
    ~SnakeGameView();
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

public slots:
    void getPlayerName(const QString name);

    Snake* snake();

signals:
    void scoreDisplay(qreal score);
    void mpFoodEaten(const QString d_user, const int score);
    void callPlayerName();

private:
    qreal d_snakeSpeed;
    qreal d_score;


//    QPropertyAnimation* move_anim;



//    QGraphicsEllipseItem* food;

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event);
    QGraphicsScene* d_scene;
    qreal d_fieldWidth;
    qreal d_fieldHeight;
    Snake *d_snake;
    Food *d_food;
    QTimer* d_timer;
    static QString d_user;
    void moveSnake();

};
#endif // SNAKEGAMEVIEW_H

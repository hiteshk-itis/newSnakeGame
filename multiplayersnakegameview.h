#ifndef MULTIPLAYERSNAKEGAMEVIEW_H
#define MULTIPLAYERSNAKEGAMEVIEW_H

#include <QJsonObject>
#include <QWidget>
#include "snakegameview.h"

class MultiPlayerSnakeGameView : public SnakeGameView
{
    Q_OBJECT
signals:

public:

    explicit MultiPlayerSnakeGameView(QWidget* parent = nullptr);
    ~MultiPlayerSnakeGameView();
    void allPlayersJoinedEmitted(qreal &n);
    void generateOtherSnakes(QHash<QString, QPointF>);
    void directionChanged(QString, int);
    void getPlayerName(const QString name);
    void otherPlayerFoodEaten(QString playerName);
    void homeBtnClicked();
    void checkCollidingwSnakeAndFood();
    void foodEaten();
    void displayAllNamesAndScores();
    void mpTimerStops();

signals:
    void sendDirection(Snake::Direction);
    void sendInitialPos(QPointF);
    void timerStarts();
private:

    qreal d_players;
    QHash<QString, Snake*> d_snakes;
    QHash<QString, Snake::Direction> d_snakesAndDirections;
//    QString d_user;
    QTimer* dd_timer;



    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void generateManyFoods(int n);
};

#endif // MULTIPLAYERSNAKEGAMEVIEW_H

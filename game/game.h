#ifndef GAME_H
#define GAME_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Game; }
QT_END_NAMESPACE

class Game : public QMainWindow
{
    Q_OBJECT

public:
    Game(QWidget *parent = nullptr);
    ~Game();
    void displayTextInChat(QString &msg);
    qreal gameWidth() const;
signals:
    void PlayerName(QString &name);
private:
    Ui::Game *ui;
};
#endif // GAME_H

#ifndef NEWMAINWINDOW_H
#define NEWMAINWINDOW_H

#include "square.h"
#include "pathfinding.h"
#include "dialog.h"
#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>
#include <random>
#include <QWidget>


namespace Ui {
class NewMainWindow;
}

class NewMainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit NewMainWindow(QWidget *parent = nullptr);
    ~NewMainWindow();

signals:
    void game_over(QString msg);
    void sendCentralWidget(QWidget*);
private slots:
    void on_btnRestart_clicked();

    void on_btnAutoManual_clicked();

    void on_game_over(QString msg);

    void on_actionQuit_triggered();

    void on_actionAbout_triggered();

    void on_actionSpeed_Up_triggered();

    void on_actionSpeed_Down_triggered();

    void on_timeout_linkActivated(const QString &link);

    void on_timeout_linkHovered(const QString &link);

    void on_timeout_windowIconTextChanged(const QString &iconText);

protected:
   void keyPressEvent(QKeyEvent* e);

private:
      Ui::NewMainWindow *ui;
  const int kSize;
  Square* food;
  std::list<Square*> world;
  std::list<Square*> playerSnake;
  std::list<Square*> snake;
  std::list<Square*> vsnake;
  std::list<Point> real_path;
  std::random_device rd;
  //std::mt19937 gen;
  bool is_manual;
  PathFinding aStar;
  QTimer* timer;
  Dialog* dialog;
  int timeout;

  void init();

  bool isBlock(const Point& pos);

  void setUpFoodorHead(Square::Id);

  int genRand(int n);

  bool isReachFood(Square* head);

  Square* findPoint(const Point& p);

  bool moveAround(const Point &target, const Square::Id &id);

  bool moveUp(const Square::Id &id);

  bool moveDown(const Square::Id &id);

  bool moveLeft(const Square::Id &id);

  bool moveRight(const Square::Id &id);

  bool randomMove(Square *vfood);

  bool vMove();

  void autoMove();

  void updateTimeout();

};

#endif // NEWMAINWINDOW_H

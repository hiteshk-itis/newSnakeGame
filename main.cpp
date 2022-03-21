#include "game.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Game w;

    w.show();
    return a.exec();
}

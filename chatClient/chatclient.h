#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <qformlayout.h>
#include <QTcpSocket>
#include <QWidget>
#include <QAbstractSocket>
#include <QThread>

#include "snake.h"
class ChatClient : public QWidget
{
    Q_OBJECT
public:
    explicit ChatClient(QWidget *parent = nullptr);
    void createRoomClicked();
    void getPlayerName(QString &name);
    QString playerName();
~ChatClient();

signals:
    void disableCreateRoom(bool state);
    void allPlayersJoined(qreal &n);
    void generatePlayers(QHash<QString, QPointF>);
    void directionChanged(QString, int);
    void otherUserFoodEaten(QString playerName);
    void goToHome();
public slots:
    void sendDirection(Snake::Direction);
    void sendInitialPos(QPointF);
    void sendFoodEaten(const QString name, const int score);

private slots:
    void errorOnConn(QAbstractSocket::SocketError);

private:

    QTcpSocket* d_socket;
    QByteArray d_receivedMsg;
    QString d_user;
    QLabel* lbl;
    QLineEdit* chat_text;


    QGridLayout *glayout;
    QPlainTextEdit *chat;
    QPlainTextEdit *status;

    QPushButton *send;
    QPushButton *disconnect;
    int d_port;
    QString d_address;

    QString roomInfo;
    QString roomPassword;

    qreal players;
    QHash<QString, QPointF> d_snakesAndInitialPos;
    bool d_roomEnrolled;

    void connectToServer();
    void disconnectFromServer();
    void roomInfoWindow();
    void updateGui(QAbstractSocket::SocketState state);
    void readyRead();
    void sendChatText();
    void sendRoomInfo(const QString&, const QString&, const QString&, const int&);


};

#endif // CHATCLIENT_H

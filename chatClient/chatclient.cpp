#include "chatclient.h"


#include <QBitArray>
#include <QCommandLinkButton>
#include <QDialog>
#include <QDialogButtonBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaEnum>
#include <QNetworkConfigurationManager>
#include <QNetworkInterface>
#include <QRadioButton>
#include <QSpinBox>
#include <QStringLiteral>
#include <QTime>

#include <QMetaObject>

ChatClient::ChatClient(QWidget* parent) :
    QWidget(parent)
    ,d_socket(new QTcpSocket(this))
  , lbl(new QLabel("Chat: "))
  , chat_text(new QLineEdit())
  , glayout(new QGridLayout(this))
  , chat(new QPlainTextEdit)
  , send(new QPushButton("send"))
  , disconnect(new QPushButton("disconnect"))
  , d_port(52693)
  , d_address("127.0.0.1")
{
QNetworkConfigurationManager mgr;
QNetworkConfiguration cfg = mgr.defaultConfiguration();
if(cfg.state() == QNetworkConfiguration::Active){

    for(QHostAddress &adr : QNetworkInterface::allAddresses()){
        if(adr.protocol() == QAbstractSocket::IPv4Protocol){
            d_address = adr.toString();
        }
    }
}
    
    connect(d_socket, &QTcpSocket::connected, [=]{
        status->appendPlainText("== Connected to server");
        chat->appendPlainText("== Connected to server");
        emit disableCreateRoom(true);
        updateGui(d_socket->state());
    });
    connect(d_socket, &QTcpSocket::disconnected, [=]{
        status->appendPlainText("== disconnected");
        chat->appendPlainText("== disconnected");
        updateGui(d_socket->state());
    });
connect(d_socket, SIGNAL(error(QAbstractSocket::SocketError)),
        this, SLOT(errorOnConn(QAbstractSocket::SocketError)));

    connect(d_socket, &QTcpSocket::readyRead, this, &ChatClient::readyRead);
    connect(chat_text, &QLineEdit::returnPressed, this, &ChatClient::sendChatText);
    connect(send, &QPushButton::clicked, this, &ChatClient::sendChatText);

    // laying out the widgets in the chat box
    glayout->addWidget(chat, 0, 0, 2, 3);
    glayout->addWidget(lbl, 2, 0);
    glayout->addWidget(chat_text, 2, 1);
    glayout->addWidget(send, 2, 2);

    updateGui(d_socket->state());



}

void ChatClient::createRoomClicked()
{

    QDialog *roomInfoWin = new QDialog(this);

    QLabel* lbl_ipAddr = new QLabel("Server's IP Address", roomInfoWin);
    QLineEdit* ipAddr = new QLineEdit(roomInfoWin);

    QLabel* lbl_roomName = new QLabel("room Name: ", roomInfoWin);
    QLineEdit* room_name= new QLineEdit(roomInfoWin);


    QLabel *lbl_roomPasswd = new QLabel("room password: ", roomInfoWin);
    QLineEdit *room_password= new QLineEdit(roomInfoWin);
    room_password->setEchoMode(QLineEdit::Password);


    QDialogButtonBox *btn_box = new     QDialogButtonBox(QDialogButtonBox::Ok |     QDialogButtonBox::Cancel ,roomInfoWin);

    QRadioButton* createRoom = new QRadioButton("Create Room", roomInfoWin);
    createRoom->setChecked(true);

    QRadioButton* joinRoom = new QRadioButton("Join Room", roomInfoWin);

    QLabel* lbl_players = new QLabel("No. of Players: ", roomInfoWin);
    QSpinBox *no_of_players = new QSpinBox(roomInfoWin);
    no_of_players->setValue(1);

    status = new QPlainTextEdit(roomInfoWin);
    status->appendPlainText("\tStatus here");
    status->setEnabled(false);
//    connectToServer();

    QDialogButtonBox* btn_boxConn = new QDialogButtonBox(roomInfoWin);
    QPushButton* connectBtn = btn_boxConn->addButton("connect", QDialogButtonBox::AcceptRole);

    QPushButton* disconnectBtn =  btn_boxConn->addButton("disconnect", QDialogButtonBox::RejectRole);

connect(connectBtn, &QPushButton::clicked, [=]{
    d_address = ipAddr->text();
    connectToServer();
    const QString &roomName = room_name->text();

    const QString &roomPassword = room_password->text();
    if(roomName.isEmpty() || roomPassword.isEmpty()){
        status->appendPlainText(tr("Error: room_name or room_password is empty"));
        return;
    }
    const QString &purpose = (createRoom->isChecked())?"Create Room": "Join Room";
    const int &players = no_of_players->value();

    sendRoomInfo(roomName, roomPassword, purpose, players);
});

    connect(disconnectBtn, &QPushButton::clicked, [this]{
        disconnectFromServer();
    });

    connect(btn_box, &QDialogButtonBox::accepted, roomInfoWin, &QDialog::accept);

    connect(btn_box, &QDialogButtonBox::rejected, roomInfoWin, &QDialog::reject);

    QGridLayout *glayoutRoomInfo = new QGridLayout(roomInfoWin);

    glayoutRoomInfo->addWidget(lbl_ipAddr, 0, 0);
    glayoutRoomInfo->addWidget(ipAddr, 0, 1);

    glayoutRoomInfo->addWidget(lbl_roomName, 1, 0);
    glayoutRoomInfo->addWidget(room_name, 1, 1);

    glayoutRoomInfo->addWidget(lbl_roomPasswd, 2, 0);
    glayoutRoomInfo->addWidget(room_password, 2, 1);

    glayoutRoomInfo->addWidget(createRoom, 3, 0);
    glayoutRoomInfo->addWidget(joinRoom, 3, 1);

    glayoutRoomInfo->addWidget(lbl_players, 4,0);
    glayoutRoomInfo->addWidget(no_of_players, 4,1);

    glayoutRoomInfo->addWidget(btn_boxConn, 5,1);


    glayoutRoomInfo->addWidget(status, 6, 0, 1,2);
    glayoutRoomInfo->addWidget(btn_box, 7, 1);


    roomInfoWin->setTabOrder(room_password, createRoom);
    roomInfoWin->setTabOrder(createRoom, no_of_players);
    roomInfoWin->setTabOrder(no_of_players, connectBtn);
    roomInfoWin->setTabOrder(connectBtn, disconnectBtn);

    roomInfoWin->setWindowTitle("Server Info");
    roomInfoWin->setModal(true);
    roomInfoWin->exec();




}

void ChatClient::getPlayerName(QString &name)
{
    if(name.isEmpty()){
        return;
    }
    d_user = name;
}

QString ChatClient::playerName()
{
    return d_user;
}

ChatClient::~ChatClient()
{
}

void ChatClient::sendDirection(Snake::Direction dir)
{
 QVariant v;
 v.setValue(dir);

    QJsonObject dirObj;
    dirObj.insert("purpose",tr("set Direction"));
    dirObj.insert("playerName", d_user);
    dirObj.insert("snakeDirection", v.toInt());

    if(d_socket->state() != QAbstractSocket::ConnectedState){
        return;
    }

    QJsonDocument dirDoc(dirObj);
    QByteArray messageArray = dirDoc.toJson(QJsonDocument::Indented);
    messageArray.append(23);
    d_socket->write(messageArray);

}

void ChatClient::sendInitialPos(QPointF pt)
{
    QJsonObject posObj;
    posObj.insert("purpose",tr("set Initial Position"));
    posObj.insert("playerName",d_user);
    posObj.insert("initialPos", tr("%1, %2").arg(pt.x()).arg( pt.y()));


    QJsonDocument posDoc(posObj);

    QByteArray messageArray = posDoc.toJson(QJsonDocument::Indented);
    messageArray.append(23);
    d_socket->write(messageArray);
}

void ChatClient::sendFoodEaten(const QString name, const int score)
{
    qDebug() << "sendFoodEaten";
    QJsonObject obj;
    obj.insert("purpose", tr("food Eaten"));
    obj.insert("playerName", name);
    obj.insert("score", score);

    QJsonDocument foodDoc(obj);

    QByteArray messageArray = foodDoc.toJson(QJsonDocument::Indented);
    messageArray.append(23);
    d_socket->write(messageArray);
}

void ChatClient::connectToServer()
{

    if(d_user.isEmpty() || d_user == "pl. Name"){

        status->appendPlainText(tr("== Unable to connect to server. You must define a user name"));
        return;
    }

    if(d_socket->state()!= QAbstractSocket::ConnectedState){
        if(!d_address.isEmpty() && d_port !=0){

            d_socket->connectToHost(d_address, d_port); // asynchronous call
            status->appendPlainText(tr("== Connecting..."));
            d_socket->waitForConnected(3000);
            updateGui(QAbstractSocket::ConnectingState);
        }
    }
}

void ChatClient::disconnectFromServer()
{
 if(d_socket->state() == QAbstractSocket::ConnectingState){
    status->appendPlainText("== Abort Connecting");
}

    //abort socket
    d_socket->disconnectFromHost();
    d_socket->abort();

    // update the gui
    updateGui(d_socket->state());
}

void ChatClient::updateGui(QAbstractSocket::SocketState state)
{

    chat_text->setEnabled(state == QAbstractSocket::ConnectedState);
    lbl->setEnabled(state == QAbstractSocket::ConnectedState);
    send->setEnabled(state == QAbstractSocket::ConnectedState);
}

void ChatClient::readyRead()
{

    // read all the incoming message
    d_receivedMsg.append(d_socket->readAll());


    while(true){
        // get the index of 1st occurence of end of message
        int endIndex = d_receivedMsg.indexOf(23);

        // check if the message has ended
        if(endIndex < 0){
            break;
        }

        //store the received message, which is left of endIndex into local variable in QString from ByteArray
        QString message = QString::fromUtf8(d_receivedMsg.left(endIndex));
        QJsonDocument doc = QJsonDocument::fromJson(d_receivedMsg.left(endIndex));

        if(!doc.isNull()){
            QJsonObject obj = doc.object();
            QJsonValue purpose = obj.value(tr("purpose"));

            if(purpose.toString() == "set Initial Position"){

                //extract QPointF
                QJsonValue initialPosJson = obj.value(tr("initialPos"));
                QJsonValue snakePlayerName = obj.value(tr("playerName"));

                if(d_user != snakePlayerName.toString()){
                    QPointF initialPos(initialPosJson.toString().split(",")[0].toInt(), initialPosJson.toString().split(",")[1].toInt());

                    QPointF &pos = d_snakesAndInitialPos[snakePlayerName.toString()];
                    pos = initialPos;

                    if(d_snakesAndInitialPos.size() == players-1){

                        emit generatePlayers(d_snakesAndInitialPos);
                    }
                }
            }
            if(purpose.toString() == "set Direction"){

                QJsonValue snakePlayerName = obj.value(tr("playerName"));
                int snakeDirection = obj.value(tr("snakeDirection")).toInt();



                emit directionChanged(snakePlayerName.toString(), snakeDirection);



            }
            if(purpose.toString() == "All Players Joined"){
                qreal num = obj.value(tr("players")).toInt();
                emit allPlayersJoined(num);
                players = num;
            }

            if(purpose.toString() == "food Eaten"){
                QString playerName = obj.value("playerName").toString();
                qDebug() << "food eaten" << obj;
                emit otherUserFoodEaten(playerName);
            }
        }

        //remove the message from the byteArray till the endIndex
        d_receivedMsg.remove(0, endIndex+1);

        // display the text message
        chat->appendPlainText(message);
    }
}

void ChatClient::sendChatText()
{
    // store the text
    const QString text = chat_text->text().simplified();

    // check if text is empty or socket is not connected
    if(text.isEmpty() || d_socket->state() != QAbstractSocket::ConnectedState){
        return;
    }

    // append the playerName along with the text
    QString message = QStringLiteral("%1: %2")
            .arg(d_user)
            .arg(chat_text->text());

    // change into byteArray
    QByteArray messageArray = message.toUtf8();

    // to denote the end of message
    messageArray.append(23);

    d_socket->write(messageArray);

    chat_text->clear();
}

void ChatClient::sendRoomInfo(const QString &roomName, const QString &roomPassword, const QString &purpose, const int &players)
{
    QJsonObject obj;
    obj.insert("roomName", roomName);
    obj.insert("roomPassword", roomPassword);
    obj.insert("roomCreator", d_user);
    obj.insert("purpose", purpose);
    obj.insert("players", players);
    QJsonDocument doc(obj);
    QByteArray roomInfo = doc.toJson(QJsonDocument::Indented);
    roomInfo.append(23);

    d_socket->write(roomInfo);
    chat->appendPlainText(doc.toJson());
    status->appendPlainText(doc.toJson());
}

void ChatClient::errorOnConn(QAbstractSocket::SocketError err)
{
    Q_UNUSED(err);
    QString error = QStringLiteral("Error: %1")
            .arg(d_socket->errorString());
    status->appendPlainText(error);
}




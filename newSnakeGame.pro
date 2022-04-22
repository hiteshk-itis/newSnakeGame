QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


CONFIG += c++11
INCLUDEPATH += $$PWD/food $$PWD/game $$PWD/snake $$PWD/snakeGameView $$PWD/chatClient
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    food/food.cpp \
    main.cpp \
    game/game.cpp \
    multiplayerpage.cpp \
    multiplayersnakegameview.cpp \
    newmainwindow.cpp \
    snake/snake.cpp \
    snakeGameView/snakegameview.cpp \
    chatClient/chatclient.cpp \
    dialog.cpp \
    pathfinding.cpp \
    square.cpp


HEADERS += \
    food/food.h \
    game/game.h \
    multiplayerpage.h \
    multiplayersnakegameview.h \
    newmainwindow.h \
    snake/snake.h \
    snakeGameView/snakegameview.h \
    chatClient/chatclient.h \
    dialog.h \
    pathfinding.h \
    square.h

FORMS += \
    game/game.ui \
    multiplayerpage.ui \
    dialog.ui \
    newmainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

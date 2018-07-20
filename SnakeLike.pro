TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

LIBS += -lncurses
INCLUDEPATH += "./SFML-2.4.2-osx-clang/include"
DEPENDPATH  += "./SFML-2.4.2-osx-clang/lib"
DEPENDPATH  += "./SFML-2.4.2-osx-clang/extlibs"
DEPENDPATH  += "./SFML-2.4.2-osx-clang/Frameworks"

LIBS += -L"./SFML-2.4.2-osx-clang/lib"
LIBS += -L"./SFML-2.4.2-osx-clang/extlibs"
LIBS += -L"./SFML-2.4.2-osx-clang/Frameworks"

CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-audio.2.4 -lsfml-audio.2.4.2 -lsfml-graphics -lsfml-graphics.2.4 -lsfml-graphics.2.4.2  -lsfml-network -lsfml-network.2.4 -lsfml-network.2.4.2 -lsfml-window -lsfml-window.2.4 -lsfml-window.2.4.2 -lsfml-system -lsfml-system.2.4 -lsfml-system.2.4.2
CONFIG(debug, debug|release):   LIBS += -lsfml-audio -lsfml-audio.2.4 -lsfml-audio.2.4.2 -lsfml-graphics -lsfml-graphics.2.4 -lsfml-graphics.2.4.2 -lsfml-network -lsfml-network.2.4 -lsfml-network.2.4.2 -lsfml-window -lsfml-window.2.4 -lsfml-window.2.4.2 -lsfml-system -lsfml-system.2.4 -lsfml-system.2.4.2


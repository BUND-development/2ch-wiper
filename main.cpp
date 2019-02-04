#include "setup.h"
#include "gui.h"
#include <QApplication>
#include <QSplashScreen>

int main (int argc, char * argv[]) {
    QApplication a(argc, argv);
    QSplashScreen splash(QPixmap(QDir::currentPath()+"/gui/splash.png"));
    splash.show();
    GUI w;

    if (argc == 2) {
        if (argv[1] == "-d" || argv[1] == "--debug") w.debug(2);
        else if (argv[1] == "-l" || argv[2] == "--log") w.debug(1);
    }

    w.show();
    splash.finish(&w);

    return a.exec();
}

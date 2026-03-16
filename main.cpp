#include <QApplication>
#include "Window.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Window okno;
    okno.show();

    return app.exec();
}

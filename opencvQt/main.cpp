#include "opencvQt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    opencvQt w;
    w.show();
    return a.exec();
}

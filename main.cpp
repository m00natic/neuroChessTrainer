#include <QtGui/QApplication>
#include "ntrain.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    nTrain w;
    w.show();
    return a.exec();
}

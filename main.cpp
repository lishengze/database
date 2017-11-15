#include <QCoreApplication>
#include "basicfunc.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    testConnect ();
//    testFunc ();
    return a.exec();
}

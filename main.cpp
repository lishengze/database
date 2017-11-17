#include <QCoreApplication>
#include "basicfunc.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    testAdo ();
//    testFunc ();
//    outputDatabaseDrivers();
    testODBC ();
    return a.exec();
}

#include <QCoreApplication>
#include "worker.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Worker w(a.parent());

    QMetaObject::invokeMethod(&w, ([&w, &argc, argv]() {w.run(argc, argv);}), Qt::QueuedConnection);
    QObject::connect(&w, SIGNAL(close()), &a, SLOT(quit()));
    return a.exec();
}

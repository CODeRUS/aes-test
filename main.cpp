#include <QCoreApplication>
#include "aestest.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    new AesTest(&a);

    return a.exec();
}

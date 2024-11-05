#include <QDebug>
#include "interface.h"
#include "wall.h"
#include <QApplication>
#include <pthread.h>
#include "generator.h"
#include "calculate.h"
#include "analytical.h"


int main(int argc, char *argv[])
{
     QApplication a(argc, argv);
     Interface w;
     w.move(400, 400);
     w.show();
     return a.exec();
}

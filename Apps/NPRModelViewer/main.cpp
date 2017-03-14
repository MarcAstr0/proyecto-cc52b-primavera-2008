#include "aplicacion_cc52b.h"

#include <QtGui>
#include <QApplication>
#include <locale.h>


int main(int argc, char *argv[]) {
	setlocale(LC_NUMERIC, "C");
    QApplication a(argc, argv);
    AplicacionCC52B w;
    w.show();
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();
}

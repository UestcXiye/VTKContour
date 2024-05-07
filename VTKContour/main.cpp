#include "VTKContour.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VTKContour w;
    w.show();
    return a.exec();
}

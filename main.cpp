///@brief The main file used to build the sprite editor
///Reviewed by: Peyton Jensen

#include "view.h"
#include "model.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Model m;
    View w(m);
    w.show();
    return a.exec();
}

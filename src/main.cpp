#include <QtGui/QApplication>

#include <presenters/MainPresenter.h>

int main (int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainPresenter mainPresenter;
    return app.exec();
}

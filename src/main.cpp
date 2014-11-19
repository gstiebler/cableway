#include <QApplication>

#include <presenters/MainPresenter.h>
#include <MainExecution.h>

int main (int argc, char *argv[])
{
    MainExecution::exeFileName = argv[0];

    QApplication app(argc, argv);

    MainPresenter mainPresenter;
    return app.exec();
}

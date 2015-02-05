#include <QApplication>

#include <presenters/MainPresenter.h>
#include <MainExecution.h>

int main (int argc, char *argv[])
{
	printf("%s\n", argv[0]);
    MainExecution::exeFileName = argv[0];

    QApplication app(argc, argv);

    MainPresenter mainPresenter;
    return app.exec();
}

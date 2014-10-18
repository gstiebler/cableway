 #include <QtGui/QApplication>
 #include <QtGui/QPushButton>

int main (int argc, char *argv[])
{
    QApplication app(argc, argv);

    QPushButton hello("Hello world!");
    hello.resize(100, 30);

    hello.show();
    return app.exec();
}

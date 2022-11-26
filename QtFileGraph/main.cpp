#include "MainWindow.hpp"

#include <QApplication>
#include <QMainWindow>


int main(int argc, char* argv[])
{
    QApplication app{ argc, argv };

    MainWindow window{};
    window.show();

    return app.exec();
}

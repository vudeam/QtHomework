#include "QtFileGraph.hpp"

#include <QApplication>
#include <QMainWindow>


int main(int argc, char* argv[])
{
    QApplication app{ argc, argv };

    auto widget = new QtFileGraph{};

    QMainWindow window{};
    window.setCentralWidget(widget);

    window.show();

    return app.exec();
}

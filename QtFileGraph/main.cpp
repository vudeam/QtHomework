#include "QtFileGraph.hpp"

#include <QApplication>


int main(int argc, char* argv[])
{
    QApplication app{ argc, argv };

    QtFileGraph window{};
    window.show();

    return app.exec();
}

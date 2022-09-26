#include "QtFileCRUD.hpp"

#include <QApplication>


int main(int argc, char* argv[])
{
    QApplication app{argc, argv};

    QtFileCRUD window{};
    window.show();

    return app.exec();
}

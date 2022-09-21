#include "QtFileCRUD.hpp"
#include "StudentModel.hpp"

// DELETE
#include <QDebug>
#include <QTableView>
// DELETE

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app{argc, argv};

    // QtFileCRUD window{};
    // window.show();

    Student s;
    s.m_name = "Name";
    s.m_surname = "Surname";
    s.m_lastName = "LastName";
    s.m_course = 3;
    s.m_enroll = QDate{2020, 2, 3};
    s.m_institute = "IKB";
    s.m_chair = "Kb";
    s.m_isBudget = false;

    QTableView v{};
    StudentModel m{{s, s, s}};
    v.setModel(&m);
    v.show();

    return app.exec();
}

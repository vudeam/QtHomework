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
    /*
    QFile test{"D:\\Temp\\student.json"};
    if (test.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        Student s;
        s.m_name = "Name";
        s.m_surname = "jgkf";
        s.m_lastName = "jgkf";
        s.m_course = 123;
        s.m_enroll = QDate{2020, 2, 3};
        s.m_institute = "jgkf";
        s.m_chair = "jgkf";
        s.m_isBudget = false;

        QJsonObject obj{};
        s.writeJson(obj);

        test.write(QJsonDocument{obj}.toJson(QJsonDocument::Indented));
        test.close();
    }
    if (test.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Student fromFile{};

        QJsonDocument doc{QJsonDocument::fromJson(test.readAll())};
        fromFile.readJson(doc.object());

        test.close();
        qDebug() << fromFile.m_enroll << fromFile.m_name;
        qDebug() << doc;
    }
    */

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

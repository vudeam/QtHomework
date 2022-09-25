#include "QtFileCRUD.hpp"
#include "DateEditDelegate.hpp"
#include "SpinBoxDelegate.hpp"
#include "StudentModel.hpp"
#include "ui_QtFileCRUD.h"

#include <QTableView>


QtFileCRUD::QtFileCRUD(QWidget* parent)
    : QMainWindow{parent}
    , ui{new Ui::QtFileCRUD}
{
    ui->setupUi(this);

    Student s;
    s.m_name = "Name";
    s.m_surname = "Surname";
    s.m_lastName = "LastName";
    s.m_course = 3;
    s.m_enroll = QDate{2020, 2, 3};
    s.m_institute = "IKB";
    s.m_chair = "Kb";
    s.m_isBudget = false;

    auto v{ new QTableView{} };
    auto m{ new StudentModel{{s, s, s}} };

    v->setModel(m);

    auto d1{ new SpinBoxDelegate{} };
    v->setItemDelegateForColumn(Student::Course, d1);

    auto d2{ new DateEditDelegate{} };
    v->setItemDelegateForColumn(Student::Enroll, d2);

    setCentralWidget(v);
}

QtFileCRUD::~QtFileCRUD()
{
    delete ui;
}

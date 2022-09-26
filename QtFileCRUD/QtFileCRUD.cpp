#include "DateEditDelegate.hpp"
#include "SpinBoxDelegate.hpp"
#include "QtFileCRUD.hpp"
// #include "ui_QtFileCRUD.h"

#include <QMenuBar>
#include <QMessageBox>
#include <QTableView>


QtFileCRUD::QtFileCRUD(QWidget* parent)
    : QMainWindow{parent}
    // , ui{new Ui::QtFileCRUD}
{
    // ui->setupUi(this);

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

    createMenus();

    setWindowTitle(QStringLiteral("QtFileCRUD"));
}

QtFileCRUD::~QtFileCRUD()
{
    // delete ui;
}

void
QtFileCRUD::createMenus()
{
    auto fileMenu{ menuBar()->addMenu(tr("&File")) };

    auto openAct{ new QAction{tr("&Open...")} };
    openAct->setShortcut(Qt::CTRL | Qt::Key_O);
    fileMenu->addAction(openAct);
    connect(openAct, &QAction::triggered,
            this, &QtFileCRUD::openFile);

    auto saveAct{ new QAction{tr("&Save As...")} };
    saveAct->setShortcut(Qt::CTRL | Qt::Key_S);
    fileMenu->addAction(saveAct);
    connect(saveAct, &QAction::triggered,
            this, &QtFileCRUD::saveFile);

    auto editMenu{ menuBar()->addMenu(tr("&Edit")) };

    auto removeAct{ new QAction{tr("&Remove row(s)")} };
    removeAct->setShortcut(Qt::Key_Delete);
    editMenu->addAction(removeAct);
    connect(removeAct, &QAction::triggered,
            this, [this](){ QMessageBox::information(this, "Info", "Remove"); });

    menuBar()->addSeparator();
}

void
QtFileCRUD::openFile()
{
    QMessageBox::information(this, "Info", "Open file");
}

void
QtFileCRUD::saveFile()
{
    QMessageBox::information(this, "Info", "Save file");
}

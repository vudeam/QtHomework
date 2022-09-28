#include "DateEditDelegate.hpp"
#include "SpinBoxDelegate.hpp"
#include "QtFileCRUD.hpp"

#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMenuBar>
#include <QMessageBox>

#include <QDebug>


QtFileCRUD::QtFileCRUD(QWidget* parent)
    : QMainWindow{parent}
    // , studentModel{new StudentModel{{}}}
    , studentView{new QTableView}
{
    Student s;
    s.m_name = "Name";
    s.m_surname = "Surname";
    s.m_lastName = "LastName";
    s.m_course = 3;
    s.m_enroll = QDate{2020, 2, 3};
    s.m_institute = "IKB";
    s.m_chair = "Kb";
    s.m_isBudget = false;

    studentModel = new StudentModel{{s, s, s}};

    studentView->setModel(studentModel);

    auto d1{ new SpinBoxDelegate{} };
    studentView->setItemDelegateForColumn(Student::Course, d1);

    auto d2{ new DateEditDelegate{} };
    studentView->setItemDelegateForColumn(Student::Enroll, d2);

    setCentralWidget(studentView);

    createMenus();

    setWindowTitle(QStringLiteral("QtFileCRUD"));
}

void
QtFileCRUD::createMenus()
{
    /* File */
    auto fileMenu{ menuBar()->addMenu(tr("&File")) };

    /* File -> Open... */
    auto openAct{ new QAction{tr("&Open...")} };
    openAct->setShortcut(Qt::CTRL | Qt::Key_O);
    fileMenu->addAction(openAct);
    connect(openAct, &QAction::triggered,
            this, &QtFileCRUD::openFile);

    /* File -> Save As... */
    auto saveAct{ new QAction{tr("&Save As...")} };
    saveAct->setShortcut(Qt::CTRL | Qt::Key_S);
    fileMenu->addAction(saveAct);
    connect(saveAct, &QAction::triggered,
            this, &QtFileCRUD::saveFile);

    fileMenu->addSeparator();

    /* File -> Quit */
    auto quitAct{ new QAction{tr("&Quit")} };
    quitAct->setShortcut(Qt::CTRL | Qt::Key_Q);
    fileMenu->addAction(quitAct);
    connect(quitAct, &QAction::triggered,
            this, &QtFileCRUD::close);

    /* Edit */
    auto editMenu{ menuBar()->addMenu(tr("&Edit")) };

    /* Edit -> Add row */
    addAct = new QAction{tr("&Add row")};
    addAct->setShortcut(Qt::CTRL | Qt::Key_N);
    editMenu->addAction(addAct);
    connect(addAct, &QAction::triggered,
            this, [this]() { QMessageBox::information(this, "Info", "Add"); });

    /* Edit -> Remove row(s) */
    removeAct = new QAction{tr("&Remove row(s)")};
    removeAct->setShortcut(Qt::Key_Delete);
    editMenu->addAction(removeAct);
    connect(removeAct, &QAction::triggered,
            this, [this](){ QMessageBox::information(this, "Info", "Remove"); });

    menuBar()->addSeparator();

    connect(studentView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &QtFileCRUD::updateActions);
}

void
QtFileCRUD::openFile()
{
    auto fileName{
        QFileDialog::getOpenFileName(this,
                                     tr("Open file"),
                                     QDir::currentPath(),
                                     tr("Student (*.json *.JSON)"))
    };

    if (fileName.isEmpty())
    {
        return;
    }

    QFile studentsFile{ fileName };

    if (!studentsFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(this,
                                 tr("Unable to open file"),
                                 studentsFile.errorString());

        return;
    }

    auto jsonDoc{ QJsonDocument::fromJson(studentsFile.readAll()) };
    studentsFile.close();

    if (!jsonDoc.isArray() || jsonDoc.array().size() <= 0)
    {
        QMessageBox::critical(this,
                              tr("Incorrect format"),
                              tr("File must contain valid non-empty JSON array"));

        return;
    }

    /* clear table (remove all rows from 0 to end) */
    studentModel->removeRows(0,
                             studentModel->rowCount());

    /**
     * for each entry in array
     * construct Student & pass it to be inserted in the table
     */
    for (const auto& item : jsonDoc.array())
    {
        auto stud{ Student::fromJson(item.toObject()) };

        addStudentEntry(stud);
    }
}

void
QtFileCRUD::saveFile()
{
    QMessageBox::information(this, "Info", "Save file");
}

void
QtFileCRUD::addStudentEntry(const Student& stud)
{
    using Field = Student::StudentField;

    const auto insertRow{ studentModel->rowCount() };

    studentModel->insertRows(insertRow,
                             1);

    /* set each column with corrensponding field */
    for (auto field{0}; field <= Student::IsBudget; field++)
    {
        auto idx{ studentModel->index(insertRow, field) };
        QVariant val{};

        switch (field)
        {
        case Field::Name:      val = stud.m_name; break;
        case Field::Surname:   val = stud.m_surname; break;
        case Field::LastName:  val = stud.m_lastName; break;
        case Field::Course:    val = stud.m_course; break;
        case Field::Enroll:    val = stud.m_enroll; break;
        case Field::Institute: val = stud.m_institute; break;
        case Field::Chair:     val = stud.m_chair; break;
        case Field::IsBudget:  val = stud.m_isBudget; break;
        }

        studentModel->setData(idx,
                              val);
    }
}

void
QtFileCRUD::updateActions(const QItemSelection& sel)
{
    auto indexes{ sel.indexes() };

    if (indexes.isEmpty())
    {
        addAct->setEnabled(false);
        removeAct->setEnabled(false);
    }
    else
    {
        addAct->setEnabled(true);
        addAct->setEnabled(true);
    }
}

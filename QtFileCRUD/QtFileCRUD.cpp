#include "DateEditDelegate.hpp"
#include "SpinBoxDelegate.hpp"
#include "QtFileCRUD.hpp"

#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMenuBar>
#include <QMessageBox>


QtFileCRUD::QtFileCRUD(QWidget* parent)
    : QMainWindow{parent}
    , studentModel{new StudentModel}
    , studentView{new QTableView}
{
    resize(850, 400);

    studentView->setModel(studentModel);

    auto d1{ new SpinBoxDelegate{} };
    studentView->setItemDelegateForColumn(Student::Course, d1);

    auto d2{ new DateEditDelegate{} };
    studentView->setItemDelegateForColumn(Student::Enroll, d2);

    setCentralWidget(studentView);

    connect(studentModel, &QAbstractTableModel::rowsRemoved,
            this, &QtFileCRUD::updateActions);
    connect(studentModel, &QAbstractTableModel::rowsInserted,
            this, &QtFileCRUD::updateActions);

    createMenus();
    updateActions();

    setWindowTitle(QStringLiteral("QtFileCRUD"));
}

void
QtFileCRUD::createMenus()
{
    /* File */
    auto fileMenu{ menuBar()->addMenu(tr("&File")) };

    /* File -> Open... */
    openAct = new QAction{ tr("&Open..."), this };
    openAct->setShortcut(Qt::CTRL | Qt::Key_O);
    fileMenu->addAction(openAct);
    connect(openAct, &QAction::triggered,
            this, &QtFileCRUD::openFile);

    /* File -> Save As... */
    saveAct = new QAction{ tr("&Save As..."), this };
    saveAct->setShortcut(Qt::CTRL | Qt::Key_S);
    fileMenu->addAction(saveAct);
    connect(saveAct, &QAction::triggered,
            this, &QtFileCRUD::saveFile);

    fileMenu->addSeparator();

    /* File -> Quit */
    quitAct = new QAction{ tr("&Quit"), this };
    quitAct->setShortcut(Qt::CTRL | Qt::Key_Q);
    fileMenu->addAction(quitAct);
    connect(quitAct, &QAction::triggered,
            this, &QtFileCRUD::close);

    /* Edit */
    auto editMenu{ menuBar()->addMenu(tr("&Edit")) };

    /* Edit -> Add row */
    addAct = new QAction{ tr("&Add row"), this };
    addAct->setShortcut(Qt::CTRL | Qt::Key_N);
    editMenu->addAction(addAct);
    connect(addAct,
            &QAction::triggered,
            this,
            [this]()
            {
                studentModel->insertRows(studentModel->rowCount(), 1);
            });

    /* Edit -> Remove row(s) */
    removeAct = new QAction{ tr("&Remove row(s)"), this };
    removeAct->setShortcut(Qt::Key_Delete);
    editMenu->addAction(removeAct);
    connect(removeAct,
            &QAction::triggered,
            this,
            [this]()
            {
                auto rows{ studentView->selectionModel()->selectedRows() };
                if (!rows.empty())
                {
                    studentModel->removeRows(rows[0].row(), rows.size());
                }
            });

    menuBar()->addSeparator();
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

    if (studentModel->rowCount() > 0)
    {
        /* clear table (remove all rows from 0 to end) */
        studentModel->removeRows(0, studentModel->rowCount());
    }

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
    if (studentModel->rowCount() <= 0)
    {
        QMessageBox::information(this,
                                 tr("Empty table"),
                                 tr("Table must contain at least one entry"));

        return;
    }

    auto fileName{
        QFileDialog::getSaveFileName(this,
                                     tr("Save to"),
                                     QDir::currentPath(),
                                     tr("Student (*.json *.JSON)"))
    };

    QJsonArray arr{};
    for (const auto& stud : studentModel->students())
    {
        QJsonObject entry{};
        stud.writeJson(entry);

        arr.push_back(entry);
    }

    QFile studentsFile{ fileName };

    if (!studentsFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::information(this,
                                 tr("Unable to open file"),
                                 studentsFile.errorString());

        return;
    }

    studentsFile.write(QJsonDocument{arr}.toJson(QJsonDocument::Indented));
    studentsFile.close();
}

void
QtFileCRUD::addStudentEntry(const Student& stud)
{
    using Field = Student::StudentField;

    const auto insertRow{ studentModel->rowCount() };

    studentModel->insertRows(insertRow, 1);

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

        studentModel->setData(idx, val);
    }
}

void
QtFileCRUD::updateActions() const {
    if (studentModel->rowCount() <= 0)
    {
        removeAct->setEnabled(false);
        saveAct->setEnabled(false);
    }
    else
    {
        removeAct->setEnabled(true);
        saveAct->setEnabled(true);
    }
}

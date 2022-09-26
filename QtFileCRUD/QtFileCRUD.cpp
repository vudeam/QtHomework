#include "DateEditDelegate.hpp"
#include "SpinBoxDelegate.hpp"
#include "QtFileCRUD.hpp"

#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>


QtFileCRUD::QtFileCRUD(QWidget* parent)
    : QMainWindow{parent}
    , model{new StudentModel{{}, this}}
    , view{new QTableView}
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

    // auto v{ new QTableView{} };
    auto m{ new StudentModel{{s, s, s}} };

    view->setModel(m);

    auto d1{ new SpinBoxDelegate{} };
    view->setItemDelegateForColumn(Student::Course, d1);

    auto d2{ new DateEditDelegate{} };
    view->setItemDelegateForColumn(Student::Enroll, d2);

    setCentralWidget(view);

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

    // connect(table, &QItemSelectionModel::selectionChanged,
    //         this, &QtFileCRUD::updateActions);
}

void
QtFileCRUD::openFile()
{
    auto fileName{
        QFileDialog::getOpenFileName(this,
                                     tr("Open file"),
                                     QDir::currentPath(),
                                     tr("Student (*.json *.JSON"))
    };

    if (fileName.isEmpty())
    {
        return;
    }

    QFile studentsFile{fileName};

    if (!studentsFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(this,
                                 tr("Unable to open file"),
                                 studentsFile.errorString());

        return;
    }
}

void
QtFileCRUD::saveFile()
{
    QMessageBox::information(this, "Info", "Save file");
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

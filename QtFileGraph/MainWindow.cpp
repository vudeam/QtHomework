#include "MainWindow.hpp"

#include <QMenuBar>
#include <QMessageBox>


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}
    , m_graph{new QtFileGraph{}}
{
    setCentralWidget(m_graph);

    createMenus();

    setWindowTitle(QStringLiteral("HPP Graph"));
}

void
MainWindow::createMenus()
{
    /* File */
    auto fileMenu{ menuBar()->addMenu(tr("&File")) };

    /* File -> Open */
    m_openAct = new QAction{ tr("&Open..."), this };
    m_openAct->setShortcut(Qt::CTRL | Qt::Key_O);
    fileMenu->addAction(m_openAct);
    connect(m_openAct, &QAction::triggered,
            m_graph, &QtFileGraph::openFile);

    fileMenu->addSeparator();

    /* File -> Quit */
    m_quitAct = new QAction{ tr("&Quit"), this };
    m_quitAct->setShortcut(Qt::CTRL | Qt::Key_Q);
    fileMenu->addAction(m_quitAct);
    connect(m_quitAct, &QAction::triggered,
            this, &MainWindow::close);
}

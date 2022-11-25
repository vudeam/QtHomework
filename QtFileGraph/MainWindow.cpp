#include "MainWindow.hpp"

#include <QFile>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QPixmap>


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}
    , m_graph{new QtFileGraph{}}
{
    setCentralWidget(m_graph);

    createMenus();

    setWindowTitle(QStringLiteral("HPP Graph"));
}

MainWindow::~MainWindow()
{
    delete m_graph;
    delete m_clearAct;
    delete m_openAct;
    delete m_quitAct;
    delete m_screenshotAct;
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

    /* Edit */
    auto editMenu{ menuBar()->addMenu(tr("&Edit")) };

    /* Edit -> Clear scene */
    m_clearAct = new QAction{ tr("&Clear scene"), this };
    m_clearAct->setShortcut(Qt::CTRL | Qt::Key_R);
    editMenu->addAction(m_clearAct);
    connect(m_clearAct, &QAction::triggered,
            m_graph->scene(), &QGraphicsScene::clear);

    /* Edit -> Take screenshot */
    m_screenshotAct = new QAction{ tr("&Take screenshot"), this };
    m_screenshotAct->setShortcut(Qt::CTRL | Qt::Key_S);
    editMenu->addAction(m_screenshotAct);
    connect(m_screenshotAct, &QAction::triggered,
            this, &MainWindow::saveScreenshot);
    {
        auto shot{ m_graph->grab() };
        QFile out("D:\\Temp\\screenshot.png");
        shot.save(&out, "PNG");
    }
}

void
MainWindow::saveScreenshot()
{
    auto fileName
    {
        QFileDialog::getSaveFileName(this,
                                     tr("Save Screenshot"),
                                     QDir::currentPath(),
                                     "Image (*.png *.PNG)")
    };

    if (fileName.isEmpty())
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("No file selected"));

        return;
    }

    auto screenshot{ m_graph->grab() };
    QFile outFile{ fileName };

    if (!outFile.open(QFile::WriteOnly))
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Cannot read file"));

        return;
    }

    screenshot.save(&outFile, "PNG");

    outFile.close();
}

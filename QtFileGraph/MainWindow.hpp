#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "QtFileGraph.hpp"

#include <QAction>
#include <QMainWindow>


class MainWindow
    : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget* parent = nullptr);

    ~MainWindow();

private slots:

    void
    saveScreenshot();

private:

    void
    createMenus();

    QtFileGraph* m_graph;
    QAction* m_clearAct;
    QAction* m_openAct;
    QAction* m_quitAct;
    QAction* m_screenshotAct;
};

#endif /* MAINWINDOW_HPP */

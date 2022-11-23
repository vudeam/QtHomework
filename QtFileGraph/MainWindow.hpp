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

private:

    void
    createMenus();

    QtFileGraph* m_graph;
    QAction* m_openAct;
    QAction* m_quitAct;
};

#endif /* MAINWINDOW_HPP */

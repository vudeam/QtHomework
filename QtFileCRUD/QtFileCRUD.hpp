#ifndef QTFILECRUD_HPP
#define QTFILECRUD_HPP

#include "StudentModel.hpp"

#include <QItemSelection>
#include <QMainWindow>
#include <QTableView>


class QtFileCRUD
    : public QMainWindow
{
    Q_OBJECT

public:

    QtFileCRUD(QWidget* parent = nullptr);

private slots:

    void
    updateActions(const QItemSelection& selection);

    void
    openFile();

    void
    saveFile();

private:

    void
    createMenus();

    QAction* addAct;
    QAction* removeAct;

    StudentModel* model;
    QTableView* view;
};
#endif /* QTFILECRUD_HPP */

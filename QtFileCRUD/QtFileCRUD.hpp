#ifndef QTFILECRUD_HPP
#define QTFILECRUD_HPP

#include "StudentModel.hpp"

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
    updateActions() const;

    /*!
     * \brief Read list of Students from JSON file.
     *
     * This methods gets open file name via dialog, reads JSON array from it
     * and calls another method to fill new row in the table with data.
     *
     * \sa QFileDialog
     */
    void
    openFile();

    void
    saveFile();

private:

    void
    createMenus();

    void
    addStudentEntry(const Student& stud);

    QAction* addAct;
    QAction* openAct;
    QAction* removeAct;
    QAction* saveAct;
    QAction* quitAct;

    StudentModel* studentModel;
    QTableView* studentView;
};
#endif /* QTFILECRUD_HPP */

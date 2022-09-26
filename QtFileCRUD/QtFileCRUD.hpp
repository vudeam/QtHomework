#ifndef QTFILECRUD_HPP
#define QTFILECRUD_HPP

#include "StudentModel.hpp"

#include <QMainWindow>


// QT_BEGIN_NAMESPACE
// namespace Ui { class QtFileCRUD; }
// QT_END_NAMESPACE

class QtFileCRUD
    : public QMainWindow
{
    Q_OBJECT

public:

    using ModelType = StudentModel;
    using ModelPtr = ModelType*;

    QtFileCRUD(QWidget* parent = nullptr);
    ~QtFileCRUD();

private slots:

    void
    openFile();

    void
    saveFile();

private:

    void
    createMenus();

    ModelPtr table;
    // Ui::QtFileCRUD* ui;
};
#endif /* QTFILECRUD_HPP */

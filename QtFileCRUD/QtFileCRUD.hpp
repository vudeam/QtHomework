#ifndef QTFILECRUD_HPP
#define QTFILECRUD_HPP

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class QtFileCRUD; }
QT_END_NAMESPACE

class QtFileCRUD
    : public QMainWindow
{
    Q_OBJECT

public:
    QtFileCRUD(QWidget* parent = nullptr);
    ~QtFileCRUD();

private:
    Ui::QtFileCRUD* ui;
};
#endif /* QTFILECRUD_HPP */

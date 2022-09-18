#include "QtFileCRUD.hpp"
#include "ui_QtFileCRUD.h"

QtFileCRUD::QtFileCRUD(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QtFileCRUD)
{
    ui->setupUi(this);
}

QtFileCRUD::~QtFileCRUD()
{
    delete ui;
}


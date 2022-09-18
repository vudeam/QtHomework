#ifndef STUDENTMODEL_HPP
#define STUDENTMODEL_HPP

#include "Student.hpp"

#include <QAbstractTableModel>
#include <QVector>


/*!
 * \brief Model class for Student
 */
class StudentModel
    : public QAbstractTableModel
{
    Q_OBJECT

public:

    using DataType = QVector<Student>;

    StudentModel(QObject* parent = nullptr);
    explicit StudentModel(const DataType& data,
                          QObject* parent = nullptr);

    int
    rowCount(const QModelIndex& parentIdx = QModelIndex{}) const override;

    int
    columnCount(const QModelIndex& parentIdx = QModelIndex{}) const override;

    Qt::ItemFlags
    flags(const QModelIndex& idx) const override;

    QVariant
    headerData(int section,
               Qt::Orientation orientation,
               int role = Qt::DisplayRole) const override;

    QVariant
    data(const QModelIndex& idx,
         int role) const override;

    bool
    setData(const QModelIndex& idx,
            const QVariant& value,
            int role = Qt::EditRole) override;

    bool
    insertRows(int pos,
               int rows,
               const QModelIndex& idx = QModelIndex{}) override;

    bool
    removeRows(int pos,
               int rows,
               const QModelIndex& idx = QModelIndex{}) override;

private:

    DataType studentData;
};

#endif /* STUDENTMODEL_HPP */

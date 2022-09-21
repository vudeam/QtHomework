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

    using value_type = Student;
    using Container = QVector<value_type>;

    StudentModel(QObject* parent = nullptr);
    explicit StudentModel(const Container& data,
                          QObject* parent = nullptr);

    int
    rowCount(const QModelIndex& parentIdx = QModelIndex{}) const override;

    /*!
     * \brief Number of fields in underlying class
     *
     * Number of fields is calculated using enum
     * containing names of fields.
     */
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
         int role = Qt::DisplayRole) const override;

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

    const Container&
    getStudents() const;

private:

    Container studentData;
};

#endif /* STUDENTMODEL_HPP */

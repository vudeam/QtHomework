#include "StudentModel.hpp"


using Field = StudentModel::value_type::StudentField;

static const QStringList headerNames{
    QStringLiteral("Name"),
    QStringLiteral("Surname"),
    QStringLiteral("Last name"),
    QStringLiteral("Course"),
    QStringLiteral("Enroll year"),
    QStringLiteral("Institute"),
    QStringLiteral("Chair"),
    QStringLiteral("Is budget"),
};

StudentModel::StudentModel(QObject* parent)
    : QAbstractTableModel{parent}
{
}

StudentModel::StudentModel(const StudentModel::Container& data,
                           QObject* parent)
    : QAbstractTableModel{parent}
    , studentData{data}
{
}

int
StudentModel::rowCount(const QModelIndex& parentIdx) const
{
    // return parentIdx.isValid() ? studentData.size() : 0;
    return !parentIdx.isValid() ? studentData.size() : 0;
}

int
StudentModel::columnCount(const QModelIndex& parentIdx) const
{
    // return parentIdx.isValid() ? value_type::IsBudget + 1 : 0;
    return !parentIdx.isValid() ? value_type::IsBudget + 1 : 0;
}

Qt::ItemFlags
StudentModel::flags(const QModelIndex& idx) const
{
    if (!idx.isValid())
    {
        return Qt::ItemIsEnabled;
    }

    return QAbstractTableModel::flags(idx) | Qt::ItemIsEditable;
}

QVariant
StudentModel::headerData(int section,
                         Qt::Orientation orientation,
                         int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant{};
    }

    if (orientation == Qt::Horizontal)
    {
        /* return corresponding field name */
        if (section >= Field::Name && section <= Field::IsBudget)
        {
            return headerNames[section];
        }
    } else if (orientation == Qt::Vertical)
    {
        /* return row number */
        return QString::number(section);
    }

    return QVariant{};
}

QVariant
StudentModel::data(const QModelIndex& idx,
                   int role) const
{
    if (!idx.isValid())
    {
        return QVariant{};
    }

    if (idx.row() >= studentData.size() || idx.row() < 0)
    {
        return QVariant{};
    }

    // if (idx.column() >= (Field::IsBudget + 1) || idx.column() < 0)
    // {
    //     return QVariant{};
    // }

    switch (role)
    {
    case Qt::DisplayRole:
    {
        const auto& student = studentData.at(idx.row());

        switch (idx.column())
        {
        case Field::Name: return student.m_name;
        case Field::Surname: return student.m_surname;
        case Field::LastName: return student.m_lastName;
        case Field::Course: return QString::number(student.m_course);
        case Field::Enroll: return student.m_enroll.toString();
        case Field::Institute: return student.m_institute;
        case Field::Chair: return student.m_chair;
        case Field::IsBudget: return student.m_isBudget ? QStringLiteral("Yes") : QStringLiteral("No");
        }

    }
    }

    return QVariant{};
}

const StudentModel::Container&
StudentModel::getStudents() const
{
    return studentData;
}

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
    }
    else if (orientation == Qt::Vertical)
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

    const auto& student{ studentData.at(idx.row()) };

    switch (role)
    {
    case Qt::DisplayRole:

        switch (idx.column())
        {
        case Field::Name:      return student.m_name;
        case Field::Surname:   return student.m_surname;
        case Field::LastName:  return student.m_lastName;
        case Field::Course:    return QString::number(student.m_course);
        case Field::Enroll:    return student.m_enroll.toString(JSON_DATE_FORMAT);
        case Field::Institute: return student.m_institute;
        case Field::Chair:     return student.m_chair;
        case Field::IsBudget:  return student.m_isBudget ? QStringLiteral("Yes") : QStringLiteral("No");
        }
        break;

    case Qt::EditRole:

        switch (idx.column())
        {
        case Field::Course: return student.m_course;
        case Field::Enroll: return student.m_enroll;
        }
        break;

    }

    return QVariant{};
}

bool
StudentModel::setData(const QModelIndex& idx,
                      const QVariant& val,
                      int role)
{
    if (!idx.isValid())
    {
        return false;
    }

    if (idx.row() >= studentData.size() || idx.row() < 0)
    {
        return false;
    }

    if (role == Qt::EditRole)
    {
        auto stud{ studentData.value(idx.row()) };

        switch (idx.column())
        {
        case Field::Name:
            // studentData[idx.column()].m_name = val.toString();
            stud.m_name = val.toString();
            break;
        case Field::Surname:
            // studentData[idx.column()].m_surname = val.toString();
            stud.m_surname = val.toString();
            break;
        case Field::LastName:
            // studentData[idx.column()].m_lastName = val.toString();
            stud.m_lastName = val.toString();
            break;
        case Field::Course:
            // studentData[idx.column()].m_course = val.toInt();
            stud.m_course = val.toInt();
            break;
        case Field::Enroll:
            // studentData[idx.column()].m_enroll = QDate::fromString(val.toString(), JSON_DATE_FORMAT);
            // stud.m_enroll = QDate::fromString(val.toString(), JSON_DATE_FORMAT);
            stud.m_enroll = val.toDate();
            break;
        case Field::Institute:
            // studentData[idx.column()].m_institute = val.toString();
            stud.m_institute = val.toString();
            break;
        case Field::Chair:
            // studentData[idx.column()].m_chair = val.toString();
            stud.m_chair = val.toString();
            break;
        case Field::IsBudget:
            // studentData[idx.column()].m_isBudget = val.toBool();
            stud.m_isBudget = val.toBool();
            break;
        default:
            return false;
        }

        studentData.replace(idx.row(), stud);
        emit dataChanged(idx,
                         idx,
                         { Qt::DisplayRole, Qt::EditRole });

        return true;
    }

    return false;
}

bool
StudentModel::insertRows(int pos,
                         int nrows,
                         const QModelIndex& parentIdx)
{
    Q_UNUSED(parentIdx);

    beginInsertRows(QModelIndex{},
                    pos,
                    pos + nrows - 1);

    for (auto row{0}; row < nrows; row++)
    {
        studentData.insert(pos, Student{});
    }

    endInsertRows();

    return true;
}

bool
StudentModel::removeRows(int pos,
                         int nrows,
                         const QModelIndex&)
{
    beginRemoveRows(QModelIndex{},
                    pos,
                    pos + nrows - 1);

    for (auto row{0}; row < nrows; row++)
    {
        studentData.removeAt(pos);
    }

    endRemoveRows();

    return true;
}

const StudentModel::Container&
StudentModel::students() const
{
    return studentData;
}

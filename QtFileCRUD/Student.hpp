#ifndef STUDENT_HPP
#define STUDENT_HPP

#define JSON_DATE_FORMAT "dd.MM.yyyy"
#define EMPTY_STR QStringLiteral("")

#include <QDate>
#include <QJsonObject>


class Student
{
public:

    Student() = default;

    /*!
     * \brief Write all members to passed object
     */
    void
    writeJson(QJsonObject& json) const;

    /*!
     * \brief Construct Student object from passed object
     * \param json object to take member values from
     */
    static Student
    fromJson(const QJsonObject& json);

    static constexpr auto COURSE_MIN { 1 };
    static constexpr auto COURSE_MAX { 6 };

    enum StudentField
    {
        Name,
        Surname,
        LastName,
        Course,
        Enroll,
        Institute,
        Chair,
        IsBudget
    };

    QString m_name;
    QString m_surname;
    QString m_lastName;
    int m_course;
    QDate m_enroll;
    QString m_institute;
    QString m_chair;
    bool m_isBudget;
};

#endif /* STUDENT_HPP */

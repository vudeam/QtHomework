#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <QDate>
#include <QJsonObject>


class Student
{
public:

    Student() = default;

    /*!
     * \brief Memberwise comparison
     * \param other
     * \return
     */
    bool
    operator ==(const Student& other) const;

    /*!
     * \brief writeJson
     * \param json
     */
    void
    writeJson(QJsonObject& json) const;

    /*!
     * \brief readJson
     * \param json
     */
    void
    readJson(const QJsonObject& json);

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

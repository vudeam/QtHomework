#include "Student.hpp"


void
Student::writeJson(QJsonObject& json) const
{
    json["name"] = m_name;
    json["surname"] = m_surname;
    json["last_name"] = m_lastName;
    json["course"] = m_course;
    json["enroll"] = m_enroll.toString(QStringLiteral(JSON_DATE_FORMAT));
    json["institute"] = m_institute;
    json["chair"] = m_chair;
    json["is_budget"] = m_isBudget;
}

/**
 * TODO: maybe rewrite with list of keys
 */
Student
Student::fromJson(const QJsonObject& json)
{
    Student result{};

    if (json.contains(QStringLiteral("name")))
    {
        result.m_name = json["name"].toString(EMPTY_STR);
    }
    if (json.contains("surname"))
    {
        result.m_surname = json["surname"].toString(EMPTY_STR);
    }
    if (json.contains("last_name"))
    {
        result.m_lastName = json["last_name"].toString(EMPTY_STR);
    }
    if (json.contains("course"))
    {
        result.m_course = json["course"].toDouble(0);
    }
    if (json.contains("enroll"))
    {
        result.m_enroll = QDate::fromString(json["enroll"].toString(), JSON_DATE_FORMAT);
    }
    if (json.contains("institute"))
    {
        result.m_institute = json["institute"].toString(EMPTY_STR);
    }
    if (json.contains("chair"))
    {
        result.m_chair = json["chair"].toString(EMPTY_STR);
    }
    if (json.contains("is_budget"))
    {
        result.m_isBudget = json["is_budget"].toBool(false);
    }

    return result;
}

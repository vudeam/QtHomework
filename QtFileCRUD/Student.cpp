#include "Student.hpp"


bool
Student::operator ==(const Student& other) const
{
    return m_name       == other.m_name &&
            m_surname   == other.m_surname &&
            m_lastName  == other.m_lastName &&
            m_course    == other.m_course &&
            m_enroll    == other.m_enroll &&
            m_institute == other.m_institute &&
            m_chair     == other.m_chair &&
            m_isBudget  == other.m_isBudget;
}

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

void
Student::readJson(const QJsonObject& json)
{
    if (json.contains(QStringLiteral("name")))
    {
        m_name = json["name"].toString(EMPTY_STR);
    }
    if (json.contains("surname"))
    {
        m_surname = json["surname"].toString(EMPTY_STR);
    }
    if (json.contains("last_name"))
    {
        m_lastName = json["last_name"].toString(EMPTY_STR);
    }
    if (json.contains("course"))
    {
        m_course = json["course"].toDouble(0);
    }
    if (json.contains("enroll"))
    {
        m_enroll = QDate::fromString(json["enroll"].toString(), JSON_DATE_FORMAT);
    }
    if (json.contains("institute"))
    {
        m_institute = json["institute"].toString(EMPTY_STR);
    }
    if (json.contains("chair"))
    {
        m_chair = json["chair"].toString(EMPTY_STR);
    }
    if (json.contains("is_budget"))
    {
        m_chair = json["is_budget"].toBool(false);
    }
}
